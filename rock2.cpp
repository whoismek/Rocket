#include<stdio.h>
#include<map>
#include<list>
#include<iostream>
#include<queue>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<cstring>
#include<string>
#include<unordered_map>
#include<unordered_set>
#include <inttypes.h>
#include<memory>
using namespace std;

struct fun {
    string name;
    uintptr_t off;
    int size;
    int flag;
};
unsigned int ldasm(void *code);
struct ELFR {
    char* bufx;
    int bsz;
    unordered_map<string,fun*> name_fn;
    unordered_map<uintptr_t,fun*> off_fn;
    static void placenop(unsigned char* to,int sz){
        //use one inst nop,better performance? 
        //ref http://www.intel.com/Assets/PDF/manual/253667.pdf
        switch(sz){
            case 1:
            *to=0x90;
            break;
            case 2:
            *to=0x66;
            to[1]=0x90;
            break;
            case 3:
            *to=0x0f;
            to[1]=0x1f;
            to[2]=0x00;
            break;
            case 4:
            *to=0x0f;
            to[1]=0x1f;
            to[2]=0x40;
            to[3]=0x00;
            break;
            case 5:
            *to=0x0f;
            to[1]=0x1f;
            to[2]=0x44;
            to[3]=0x00;
            to[4]=0x00;
            break;
        }
    }
    /*int fn_eq(fun* a,fun* b){
        if(a->size==b->size){
            if(memcmp(bufx+a->off,bufx+b->off,a->size)==0) return 1;
        }
        return 0;
    }*/
    int fn_realsz(fun* a){
        int res=a->size;
        res-=!!(bufx[a->off+a->size-1]=='\xC3');
        return res;
    }
    bool f_is_simp(fun* a){
        int nsz=0;
        while(nsz<a->size){
            nsz+=ldasm(bufx+a->off+nsz);
        }
        return nsz==a->size;
    }
    bool f_is_norip(fun* a){
        int nsz=0;
        while(nsz<a->size){
            char* now=bufx+nsz+a->off;
            if(*now=='\xe8') return false;
            nsz+=ldasm(bufx+a->off+nsz);
        }
        return nsz==a->size;
    }
    int auto_inl(fun* a){
        int fg=0;
        if(f_is_simp(a)){
            int nsz=0;
            while(nsz<a->size){
                char* now=bufx+nsz+a->off;
                if(*now=='\xe8'){
                    long next=*((int*)(now+1));
                    next+=5+a->off+nsz;
                    if(off_fn.count(next)){
                        fun* tofn=off_fn[next];
                        int rsz=fn_realsz(tofn);
                        if(rsz<=5){
                            //memset(now,0x90,5);
                            //printf("%p %p %d %d %s %s\n",nsz+a->off,next,rsz,tofn->size,tofn->name.c_str(),a->name.c_str());
                            memcpy(now,bufx+tofn->off,rsz);
                            placenop((unsigned char*)(now+rsz),5-rsz);
                            fg++;
                        }
                    }
                }
                nsz+=ldasm(now);
            }
        }
        return fg;    
    }
    void readelf(const char* fn) {
        int ct=0;
        char buf[10000];
        FILE* sym=popen((string("readelf -s -W ")+fn+" | grep FUNC | grep -v UND | grep DEFA").c_str(),"r");
        //FILE* sym=popen("cat sym.d","r");
        while(fgets(buf,10000,sym)) {
            int l=strlen(buf);
            int mpos=-1,_pos=-1;
            for(int i=0; i<l; ++i) {
                if(buf[i]==':') {
                    mpos=i;
                    break;
                }
            }
            for(int i=l-1; i>mpos; --i) {
                if(buf[i]==' ') {
                    _pos=i;
                    break;
                }
            }
            if(mpos==-1 || _pos==mpos) continue;
            uintptr_t pt;
            int sz;
            sscanf(buf+mpos+2,"%p%d",&pt,&sz);
            if(sz==0) continue;
            string name(buf+_pos+1,l-_pos-2);
            //printf("%p %d %s\n",pt,sz,name.c_str());
            fun* tmp=new fun();
            *tmp=(fun){name,pt,sz,0};
            off_fn[pt]=tmp;
            name_fn[name]=tmp;
            ++ct;
        }
        printf("fun %d\n",ct);
        pclose(sym);
    }
    ELFR(const char* fn) {
        int fd=open(fn,O_RDONLY);
        bsz=lseek(fd,0,SEEK_END);
        lseek(fd,0,SEEK_SET);
        bufx=(char*)malloc(bsz);
        bsz=read(fd,bufx,bsz);
        close(fd);
        readelf(fn);
    }
    void save(){
        int fd=open("server.out",O_WRONLY|O_TRUNC|O_CREAT,S_IRWXU);
        write(fd,bufx,bsz);
        close(fd);
    }
};
ELFR a("bedrock_server");
ELFR b("optiserver");
unordered_map<string,string> patches;
unordered_map<fun*,string> one_patch;
unique_ptr<int> x;
void add_patch(fun* f,const string& to,bool once=false){
    //printf("%p %p %p\n",a.bufx,f->off,f);
    auto x=string(a.bufx+f->off,f->size);
    //printf("add for %s %d\n",f->name.c_str(),to.size());
    if(!once && a.f_is_norip(f)){
        patches[x]=to;
    }else{
        //printf("RIP call founded!patch only a func\n");
        one_patch[f]=to;
    }
}
void add_patch(const string& from,const string& to){
    patches[from]=to;
}
int pct;
string fastmath; //("\xf3\x0f\x1e\xfa\x0f\xae\x5c\x24\xfc\x81\x4c\x24\xfc\x40\x80\x00\x00\x0f\xae\x54\x24\xfc\xe9\x00\x00\x00\x00",27)
char* hole;
uintptr_t hole_off;
void apply_patch(){
    for(auto& i:a.off_fn){
        char* to=i.first+a.bufx;
        if(one_patch.count(i.second)){
            memset(to,0xcc,i.second->size);
            memcpy(to,one_patch[i.second].data(),one_patch[i.second].size());
            if(!hole && one_patch[i.second].size()-i.second->size>=fastmath.size()){
                printf("hole found\n");
                hole_off=i.first+one_patch[i.second].size();
                hole=to+one_patch[i.second].size();
            }
            i.second->size=one_patch[i.second].size();
            ++pct;
            continue;
        }
        auto x=string(to,i.second->size);
        if(patches.count(x)){
            memset(to,0xcc,i.second->size);
            memcpy(to,patches[x].data(),patches[x].size());
            i.second->size=patches[x].size();
            ++pct;
        }
    }
}
void patch_cond(const list<string>& need,const list<string>& need2,const string& to,const list<string>& dont={}){
    for(auto const& i:a.name_fn){
        int sc=1;
        for(auto& j:dont){
            if(i.first.find(j)!=string::npos){
                sc=0;
                break;
            }
        }
        if(!sc) continue;
        for(auto& j:need){
            if(i.first.find(j)==string::npos){
                sc=0;
                break;
            }
        }
        if(!sc) continue;
        sc=0;
        for(auto& j:need2){
            if(i.first.find(j)!=string::npos){
                sc=1;
                break;
            }
        }
        if(!sc && need2.size()!=0) continue;
        add_patch(i.second,to,true); //only patch one
    }
}
int auto_inl(){
    int re=0;
    for(auto& i:a.off_fn){
        re+=a.auto_inl(i.second);
    }
    return re;
}
void reg_patches(bool doL=false){
    //RDI: 0x7fffffff9b48 --> 0x5555592045e0 (push   rbp)
    //get : 0x55555cb77e80 --> 0x55555c870b80 --> 0x55555af846d0
    //m_ptr 0x7fffffff9b50 --> 0x55555cb77e80 --> 0x55555c870b80 --> 0x55555af846d0
    patch_cond({},{"4Core7Profile","12ProfilerLite"},"\xc3"s);
    patch_cond({"_ZNSt10unique_ptr","default_del"},{"EE3getEv","EEdeEv","EEptEv"},"\x48\x8b\x07\xc3"s);
    patch_cond({"_ZNKSt10unique_ptr","default_del"},{"EE3getEv","EEdeEv","EEptEv"},"\x48\x8b\x07\xc3"s); //"EE3getEv","EEdeEv","EEptEv"
    patch_cond({"_ZNKSt10unique_ptr","EEcvbEv","default_del"},{},"\x31\xc0\x48\x83\x3f\x00\x0f\x95\xc0\xc3"s); //bool
    patch_cond({"_ZNSt10unique_ptr","EE7releaseEv","default_del"},{},"\x48\x8b\x07\x48\xc7\x07\x00\x00\x00\x00\xc3"s);
    patch_cond({"St19__shared_ptr_access"},{"_getEv","EEdeEv","EEptEv"},"\x48\x8b\x07\xc3"s);
    patch_cond({"_ZNKSt12__shared_ptr","3getEv"},{},"\x48\x8b\x07\xc3"s);
//    patch_cond({"_ZNSt10unique_ptr"},{"EE3getEv","EEdeEv","EEptEv"},"\x48\x8b\x47\x08\xc3"s,{"default_del"});
//    patch_cond({"_ZNKSt10unique_ptr"},{"EE3getEv","EEdeEv","EEptEv"},"\x48\x8b\x47\x08\xc3"s,{"default_del"}); //"EE3getEv","EEdeEv","EEptEv"
//    patch_cond({"_ZNKSt10unique_ptr","EEcvbEv"},{},"\x31\xc0\x48\x83\x7f\x08\x00\x0f\x95\xc0\xc3"s,{"default_del"}); //bool
    unordered_map<string,string> mP;
    unordered_map<string,string> mR;
    for(auto const& i:b.name_fn){
        auto& name=i.first;
        auto fun=i.second;
        if(name[0]=='P'){
            //pattern for all
            mP[name.substr(1)]=string(b.bufx+fun->off,fun->size);
        }
        if(name[0]=='R'){
            //replace for P
            mR[name.substr(1)]=string(b.bufx+fun->off,fun->size);
        }
        if(doL && name[0]=='L'){
            //spec opti for random
            auto x=a.name_fn[name.substr(1)];
            if(!x){
                printf("can not find %s\n",name.substr(1).c_str());
                continue;
            }
            add_patch(x,string(b.bufx+fun->off,fun->size),true);
        }
        if(name[0]=='Q'){
            //opti all
            auto x=a.name_fn[name.substr(1)];
            if(!x){
                printf("can not find %s\n",name.substr(1).c_str());
                continue;
            }
            add_patch(x,string(b.bufx+fun->off,fun->size),false);
        }
        if(name[0]=='O'){
            //opti spec
            auto x=a.name_fn[name.substr(1)];
            if(!x){
                printf("can not find %s\n",name.substr(1).c_str());
                continue;
            }
            add_patch(x,string(b.bufx+fun->off,fun->size),true);
        }
    }
    for(auto& i:mR){
        if(!mP.count(i.first)) continue;
        auto& from=mP[i.first];
	//printf("from %d to %d\n",from.size(),i.second.size());
        add_patch(from,i.second);
    }
}
void apply_fmath(){
    if(hole){
        printf("injecting fast_math to _start...\n");
        auto _st=a.name_fn["_start"]->off+29+3;
        int finoff=hole_off-(_st+4);
        memcpy(a.bufx+_st,&finoff,4);
        memcpy(hole,fastmath.data(),fastmath.size());
        hole+=fastmath.size()-4; //jmp
        hole_off+=fastmath.size()-4+4;
        finoff=(long)a.name_fn["main"]->off-(long)hole_off;
        memcpy(hole,&finoff,4);
    }
}
int main(int ac,char** av){
    auto x=b.name_fn["set_fast_math"];
    fastmath=string(b.bufx+x->off,x->size-1)+"\xe9\x00\x00\x00\x00"s;
    reg_patches(false);
    apply_patch();
    apply_fmath();
    printf("inlined %d\n",auto_inl());
    printf("patched %d\n",pct);
    a.save();
}

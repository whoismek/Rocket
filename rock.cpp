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
#include <inttypes.h>


using namespace std;

char bufx[218985192];
char optibuf[21898519];
struct fn {
    int sz;
    unsigned long pt;
};

fn nms[516405];
int ptr;
unordered_map<string,int> fn_ref;
unordered_map<string,string> fn_name;
struct sisi {
    string a;
    int b;
    bool operator<(const sisi& c) const {
        return b<c.b;
    }
};
priority_queue<sisi> tmpq;
unordered_map<string,int> fn_ptr;
unordered_map<string,list<unsigned long> > fun_ptr;
unordered_map<unsigned long,int> ptr_sz;
int inl_cnt,uni_cnt;
unsigned int ldasm(void *code);
void do_inl(unsigned long off,int size){
  //ZydisUSize size=ptr_sz[off];
  if(size==0) return;
  int offset = off;
  int tsz=0;
    while (tsz<size)
    {
      char* tmp=bufx+offset;
      if(*tmp=='\xE8'){
        unsigned long next=(long)offset+5+*((int*)(tmp+1));
        int nsz=ptr_sz[next];
        if(nsz>0 && nsz<=6){
          nsz-=!!(bufx[next+nsz-1]=='\xc3');
          ++inl_cnt;
          memset(tmp,0x90,5);
          memcpy(tmp,bufx+next,nsz);
          //printf("next at %p size %d\n",next,ptr_sz[next]);
        }
      }
      int isz=ldasm(tmp);
      offset+=isz;
      tsz+=isz;
    }
  }
  void uniqptr_opti(string& a,int of,int& sz){
      if((a.find("_ZNKSt10unique_ptr")!=string::npos && (a.find("EEptEv")!=string::npos || a.find("EEdeEv")!=string::npos || a.find("EE3getEv")!=string::npos)) \
      ||(a.find("_ZNKSt19__shared_ptr_access")!=string::npos && (a.find("EEptEv")!=string::npos || a.find("EEdeEv")!=string::npos ))){
          sz=4;
          uni_cnt++;
          memcpy(bufx+of,"\x48\x8b\x07\xc3",4);
      }
  }
void rd() {
    fn nm;
    int npatch=0;
//while(scanf(" %*[0-9:] %p%*[ ]%d%*[ ]%*s%*[ ]%*s%*[ ]%*s%*[ ]%*d%*[ ]%[^\n]",&nm.pt,&nm.sz,nm.name)!=EOF){

    /*if(nm.sz<=64){
      string m(buf+nm.pt,nm.sz);
      fp[m]=fp.count(m)?(fp[m]+1):1;
    }*/
    char buf[1000];
    FILE* mcsym=popen("readelf -s -W bedrock_server | grep FUNC | grep -v UND | grep DEFA","r");//mcsym=fopen("sym.d","r");
    while(fgets(buf,1000,mcsym)) {
        int l=strlen(buf);
        //printf("%d\n",l);
        int mpos=-1,_pos=-1;
        for(int i=0; i<l; ++i) {
            if(buf[i]==':') {
                mpos=i;
                break;
            }
        }
        for(int i=mpos+1; i<l; ++i) {
            if(buf[i]=='_') {
                _pos=i;
                break;
            }
        }
        if(mpos==-1 || _pos==-1) continue;
        sscanf(buf+mpos+2,"%p%d",&nms[ptr].pt,&nms[ptr].sz);
        //if(nms[ptr].sz<=){
        auto name=string(buf+_pos,l-_pos-1);
        uniqptr_opti(name,nms[ptr].pt,nms[ptr].sz);
        ptr_sz[nms[ptr].pt]=nms[ptr].sz;
        fn_ptr[name]=ptr;
        fun_ptr[string(bufx+nms[ptr].pt,nms[ptr].sz)].push_back(nms[ptr].pt);
        ++ptr;
        //}
        /*
        if(nms[ptr].sz<=72){
         // nms[ptr].name=string(buf+_pos,l-_pos);
          auto x=string(buf+_pos,l-_pos);
          auto y=string(bufx+nms[ptr].pt,nms[ptr].sz);
          if(y.find('\xE8')==string::npos){
          fn_ref[y]++;
          fn_name[y]=x;
          ptr++;
        }
        }*/
    }
    fclose(mcsym);
    mcsym=popen("readelf -s -W optiserver | grep FUNC | grep -v UND | grep DEFA | grep P","r");
    while(fgets(buf,1000,mcsym)) {
        int l=strlen(buf);
        //printf("%d\n",l);
        int mpos=-1,_pos=-1;
        for(int i=0; i<l; ++i) {
            if(buf[i]==':') {
                mpos=i;
                break;
            }
        }
        for(int i=mpos+1; i<l; ++i) {
            if(buf[i]=='_') {
                _pos=i;
                break;
            }
        }
        if(mpos==-1 || _pos==-1) continue;
        int ptr,siz;
        sscanf(buf+mpos+2,"%p%d",&ptr,&siz);
        string name(buf+_pos-1,l-_pos);
        printf("get %s\n",name.c_str());
        if(name[0]=='P') {
            int fpt=fn_ptr[name.substr(1)];
            //printf("get fpt %d\n",fpt);
            const auto& funs=fun_ptr[string(bufx+nms[fpt].pt,nms[fpt].sz)];
            if(nms[fpt].sz<siz) {
                printf("refused to patch %s\n",name.c_str());
                continue;
            }
            for(auto const i:funs) {
                npatch++;
                ptr_sz[i]=siz;
                memcpy(bufx+i,optibuf+ptr,siz);
            }
        }
    }
    printf("[STAGE1] %d patched uni_cnt %d\n",npatch,uni_cnt);
    printf("[STAGE2] Auto inlining\n");
    for(auto i:ptr_sz) {
        do_inl(i.first,i.second);
    }
    printf("[STAGE3] inline done %d\n",inl_cnt);
    /*
    printf("%d\n",ptr);

    for(auto i:fn_ref){
      //printf("%s %d\n",fn_name[i.first].c_str(),i.second);
      tmpq.push({i.first,i.second});
    }
    for(int i=0;i<25;++i){
      auto x=tmpq.top();
      tmpq.pop();
      printf("%s %d\n",fn_name[x.a].c_str(),x.b);
    }
    */
}
int main() {
    int fd=open("bedrock_server",O_RDONLY);
    int bsz=read(fd,bufx,218985192);
    close(fd);
    fd=open("optiserver",O_RDONLY);
    read(fd,optibuf,218985192);
    close(fd);
    //ios::sync_with_stdio(0);
    rd();
    fd=open("server.out",O_WRONLY|O_CREAT,S_IRWXU);
    write(fd,bufx,bsz);
    close(fd);
}

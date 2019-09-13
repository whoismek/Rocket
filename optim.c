typedef unsigned long u64;
typedef signed long s64;
typedef unsigned int u32;
typedef unsigned int _DWORD;
typedef unsigned char _BYTE;
typedef unsigned short _WORD;
typedef unsigned char uchar;
typedef u64 _QWORD;
u64 Q_ZNK7WeakPtrI11BlockLegacyEdeEv(u64** a){
	return **a;
}
u64 Q_ZNK4Boat10isPushableEv(){
return 1;
}
void P_getu64(){
	asm("push   %rbp;mov    %rsp,%rbp;mov    %rdi,-0x8(%rbp);mov    -0x8(%rbp),%rax;pop    %rbp;");
}
u64 R_getu64(u64 a){
	return a;
}

void P_nop(){
asm("push   %rbp;mov    %rsp,%rbp;mov    %rdi,-0x8(%rbp);pop    %rbp;");
}
void R_nop(){

}
char Q_ZN13ChunkBlockPosC2Ehsh(char *thi, uchar a2, short a3, uchar a4)
{
  char result; // al
  *(_BYTE *)thi = a2;
  result = a4;
  *((_BYTE *)thi + 1) = a4;
  *((_WORD *)thi + 1) = a3;
  return result;
}
char Q_ZN16SubChunkBlockPosC2ERK13ChunkBlockPos(char *thi, char *a2)
{
  char result; // al
  *(_BYTE *)thi = *(_BYTE *)a2;
  *((_BYTE *)thi + 1) = *((_WORD *)a2 + 1) & 0xF;
  result = *((_BYTE *)a2 + 1);
  *((_BYTE *)thi + 2) = result;
  return result;
}
u64 O_Zan13BlockPropertyS_(u64 a,u64 b){
	return a&b;
}
u64 Q_ZNK7WeakPtrI11BlockLegacyE3getEv(u64** a1){
u64* a2=*a1;
return a2?*a2:0;
}
int* Q_ZSt3maxIiERKT_S2_S2_(int* a,int* b){
	return *a>*b?a:b;
}
char Q_ZN7NewTypeIhEC2ERKh(char* a1,char* a2){
	return *a1=*a2;
}
char Q_ZN7NewTypeIhEC2ERKS0_(char* a1,char* a2){
	return *a1=*a2;
}
typedef struct Vec3{
	float x,y,z;
} Vec3;
float Q_ZNK4Vec33dotERKS_(Vec3* a,Vec3* b){
	return a->x*b->x+a->y*b->y+a->z*b->z;
}
Vec3* Q_ZN4Vec3C2Efff(Vec3* a,float b,float c,float d){
*a=(Vec3){b,c,d};
return a;
}

char O_ZNKSt6atomicI10ChunkStateEcvS0_Ev(char* a){
return *a;
}
char O_ZNKSt6atomicI10ChunkStateE4loadESt12memory_order(char* a){
return *a;
}

void O_ZNSt6atomicI10ChunkStateE5storeES0_St12memory_order(char* a,char b){
	*a=b;
}
u64 O_ZNKSt6atomicIP20SubChunkBlockStorageE4loadESt12memory_order(u64* a){
return *a;
}


u64 O_ZNK8SubChunk8getBlockEt(u64 th,unsigned short a2){
return (*(u64 (**)(u64, u64))(**((u64 **)th + 4) + 24LL))(*((u64 *)th + 4), a2);
}
u64 Q_ZNK28SubChunkBlockStoragePalettedILm4ELN20SubChunkBlockStorage4TypeE4EE8getBlockEt(u64 a1,unsigned short a2){
	return *(u64 *)(a1 + 8LL * ((*(int *)(a1 + 4 * ((u64)a2 >> 3) + 8) >> 4 * (a2 & 7)) & 0xF) + 2056);
}
u64 Q_ZNK28SubChunkBlockStoragePalettedILm5ELN20SubChunkBlockStorage4TypeE5EE8getBlockEt(u64 a1,unsigned short a2){
	return *(_QWORD *)(a1 + 8LL * ((*(_DWORD *)(a1 + 4 * (a2 / 6uLL) + 8) >> 5 * (a2 % 6u)) & 0x1F) + 2744);
}

u64 Q_ZNKSt6vectorI8EntityIdSaIS0_EE4sizeEv(u64* a1){
return (a1[1] - *a1) >> 2;
}

u64 Q_ZNK5RangeIiLi1EE8iteratorneERKS1_(int* a,int* b){
	return *a!=*b;
}

u64 Q_ZNK17SubChunkRelighter26_computeInternalIdxFromPosERK3Pos(u64* a,u64* a2){
 return ((*((_DWORD *)a2 + 2) & 0xF) << 6) | (*((_DWORD *)a2 + 2) / 16 << 10) | *((_DWORD *)a2 + 1) & 0xF | 16 * (*((_DWORD *)a2 + 1) / 16) | ((*(_DWORD *)a2 & 0xF) << 12) | (unsigned int)(*(_DWORD *)a2 / 16 << 16);
}
u64 Q_ZNK17SubChunkRelighter27_computeSubchunkCoordinatesEjRjS0_S0_S0_(u64 *th, unsigned int a2, unsigned int *a3, unsigned int *a4, unsigned int *a5, unsigned int *a6)
{
  u64 result; // rax
  *a3 = ((a2 & 0xF000) >> 4) | ((a2 & 0x3C0) >> 2) | a2 & 0xF;
  *a4 = (a2 >> 16) & 3;
  *a5 = (a2 >> 4) & 3;
  result = (a2 >> 10) & 3;
  *a6 = result;
  return result;
}
void Q_ZNK17SubChunkRelighter26_computeInternalCoordinateEjRjS0_S0_ (u64 *th, unsigned int a2, unsigned int *a3, unsigned int *a4, unsigned int *a5)
{
  *a3 = 16 * ((a2 >> 16) & 3) + ((unsigned short)a2 >> 12);
  *a4 = 16 * ((a2 >> 4) & 3) + (a2 & 0xF);
  *a5 = 16 * ((a2 >> 10) & 3) + ((a2 >> 6) & 0xF);
}
void Q_ZN8BlockPosC2Eiii(int* a,int b,int c,int d){
a[0]=b;a[1]=c;a[2]=d;
}
u64* O_ZNSt8__detail19_Node_iterator_baseISt4pairIK8ChunkPosSt8weak_ptrI10LevelChunkEELb1EE7_M_incrEv(u64** x){
	return *x=**x;
}
u64* O_ZNSt8__detail14_Node_iteratorISt4pairIK8ChunkPosSt8weak_ptrI10LevelChunkEELb0ELb1EEppEv(u64** x){
	return *x=**x;
}
char Q_ZNK8ChunkPoseqERKS_(u64* a,u64* b){
	return *a==*b;
}
u64 Q_ZNKSt8__detail10_Hash_nodeISt4pairIK8ChunkPosSt8weak_ptrI10LevelChunkEELb1EE7_M_nextEv(u64* x){
	return *x;
}
char Q_ZNK11BlockLegacyeqERKS_(u64 a,u64 b){
	return a==b;
}
//SubChunkBlockStoragePaletted<5ul, (SubChunkBlockStorage::Type)5>::getBlock(unsigned short)

typedef unsigned int u32;

u32* L_ZN6Random16_initGenRandFastEj(u32* a,u32 b) {
    a[1]=b;
    a[2]=b*233;
    a[3]=b*2333;
    return a;
}

int L_ZN6Random13_genRandInt32Ev(int* a) {
    a[2]=(((long long)a[2])*1103515245ll+12345);
    a[3]=(((long long)a[3])*1103515245ll+12345);
    return a[2]^a[3];
}
/*
int L_ZN6Random7nextIntEi(int* a,int t) {
    a[2]=(((long long)a[2])*1103515245ll+12345);
    a[3]=(((long long)a[3])*1103515245ll+12345);
    return ((a[2]^a[3])>>1)%t;
}
*/
void L_ZN6Random13_genRandReal2Ev() {
	//return (double)L_ZN6Random13_genRandInt32Ev(a)*2.328306436538696e-10;
	asm("imull	$1103515245, 8(%rdi), %eax; \
	vxorps	%xmm0, %xmm0, %xmm0; \
	imull	$1103515245, 12(%rdi), %edx; \
	addl	$12345, %eax; \
	addl	$12345, %edx; \
	movl	%eax, 8(%rdi); \
	xorl	%edx, %eax; \
	movl	%edx, 12(%rdi); \
	vcvtsi2sdl	%eax, %xmm0, %xmm0; \
	vmulsd	.XXX(%rip), %xmm0, %xmm0; \
	ret; \
	.align 8; \
.XXX: \
	.long	4294967295; \
	.long	1039138815; \
");
}


int main(){
}

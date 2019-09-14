# Rocket
Optimize Minecraft Bedrock Server by hot function rewriting, simply auto inlining,and fast math enabling.
<pre>
fast install:
ln ../bedrock_server . -s
sh run.sh
cp server.out ..
cd ..
LD_LIBRARY_PATH=. ./server.out

Complie Rocket:
g++ -Ofast -march=native rock2.cpp LDasm.c -o rocket
Complie optimize:
gcc -Ofast -march=native optim.cpp -o optiserver

Run Rocket:
./rocket
Run Server:
./server.out
Hint:
for better performance,consider using https://github.com/Sysca11/bdlauncher with "opti.so"

使用方法
gcc -Ofast -march=native optim.cpp -o optiserver
./rocket
使用./server.out执行服务器
QQ group 955765957

</pre>

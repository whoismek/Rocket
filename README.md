# Rocket
Optimize Minecraft Bedrock Server by hot function rewriting and auto inlining
<pre>
Complie Rocket:
g++ -Ofast -march=native rock.cpp LDasm.c -o rocket
gcc -Ofast -march=native optim.cpp -o optiserver
Run Rocket:
./rocket
Run Server:
./server.out
Hint:
for better performance,consider using https://github.com/Sysca11/bdlauncher with "opti.so"
</pre>

1. Project Description & Requirements:
This system facilitates synchronized, non-blocking, multi-threaded communication 
between separate terminal interfaces via Unix Named Pipes (FIFOs). It automatically 
creates two persistent communication pathways ('fifo1' and 'fifo2') inside the 
operating system file layer. Using custom POSIX threading (pthread), the program splits 
each user interface into two concurrent workers: a sending worker that captures keyboard 
data and routes it outward, and a receiving worker that continuously polls the pipe input 
endpoint. This bypasses structural reading block constraints, ensuring real-time message 
exchange without screen freeze bugs.

2. Compilation Instructions:
Compile the threaded multi-task engine cleanly by executing:
make

The compiler automatically links runtime system thread definitions (-pthread).

3. Execution Instructions (Requires Two Terminals running concurrently):

Step 1: Open Terminal Window #1 and spin up the environment as User 1:
./chat parmida

Step 2: Open Terminal Window #2 and spin up the environment as User 2:
./chat sara

4. Verification & Testing Examples:

[Terminal 1 View (parmida)]:
Welcome parmida! Connecting...
Connected! Start chatting.
Hi Sara, are the system buffers flushing live?
[sara] Yes! Both pipelines are running asynchronously.
quit

[Terminal 2 View (sara)]:
Welcome sara! Connecting...
Connected! Start chatting.
[parmida] Hi Sara, are the system buffers flushing live?
Yes! Both pipelines are running asynchronously.
Partner left the chat. Type 'quit' to exit.
quit

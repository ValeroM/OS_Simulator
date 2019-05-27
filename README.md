<h1>Start of Program</h1>
<p>
At the start, my program asks the user three questions:<br>
1) How much RAM memory is there on the simulated computer? The program receives the number in bytes (no kilobytes or words). The user can enter any number up to 4000000000 (4 billions). <br>
2) What is the size of a page/frame? The enumeration of pages starts from 0. <br>
3) How many hard disks does the simulated computer have? The enumeration of hard disks starts with 0.
</p>

<h1>How To Use</h1>
<p>
  <b>'A'</b> -- Means that a new process has been created. When a new process arrives, the program creates its PCB and places the process in the end of the ready-queue. Initially, no memory is allocated for the new process. Note: Processes start from 2 (1 belongs to the first system process) and go up.
</p>

<p>
  <b>'Q'</b> -- The currently running process has spent a time quantum using the CPU. Such process is preempted to the end of the ready-queue.
</p>

<p>
  <b>"fork"</b> -- The process forks a child. The child is placed in the end of the ready­-queue.
</p>

<p>
  <b>"exit"</b> -- The process that is currently using the CPU terminates. It releases all the memory used by this process immediately. This system implements the cascading termination (i.e., if a process terminates, all of its descendants terminate with it). If its parent is already waiting, the process terminates immediately and the parent becomes runnable (goes to the end of the ready­-queue). If its parent hasn’t called wait yet, the process turns into a zombie process. 
</p>

<p> 
  <b>"wait"</b> -- The process wants to pause and wait for any of its child processes to terminate. Once the wait is over, the process goes to the end of the ready­-queue. If a zombie­ child already exists, the process proceeds right away (keeps using the CPU) and the zombie­-child disappears completely. If more than one zombie­-child exists, the system uses one of them (any) to immediately restart the parent and other zombie children keep waiting for the next wait command from the parent.
</p>

<p>
  <b>d number file_name</b> --  The process that currently uses the CPU requests the hard disk #number. It wants to read or write file file_name.
</p>

<p>
  <b>D number</b> -- The hard disk #number has finished the work for one process.
</p>

<p> 
  <b>m address</b> -- The process that is currently using the CPU requests a memory operation for the logical address.
</p>

<p>
  <b>S r</b> -- Snapshot. Shows what process is currently using the CPU and what processes are waiting in the ready­-queue.
</p> 

<p>
  <b>S i</b> -- Snapshot. Shows what processes are currently using the hard disks and what processes are waiting to use them. For each busy hard disk, it shows the process that uses it and show its I/O queue. It also displays the filenames for each process. Note: The enumeration of hard disks starts from 0.
</p>

<p>
  <b>S m</b> -- Snapshot. Shows the state of memory. For each used frame, it displays the process number that occupies it and the page number stored in it. Note: The enumeration of pages and frames starts from 0.
</p>

<h1>How to run program</h1>
<p>To compile: <br>
make</p> 
<p>To clean: <br> 
make clean</p> 
<p>To quit:<br> 
control + c</p>

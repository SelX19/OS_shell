# OS_shell  
## Members:  
Idris Hasanović, Selma Đozić
  
## Files/directories in submission;  
custom_shell_1.c

## Answers to the questions in the assignment;  
#### Question 1.5.1: What does the mmap() function do?  
The mmap() function in C is used to map files or devices into memory. It allows a process to treat a file or a device as if it were an array in memory, enabling direct read and write operations on that memory. mmap() can be very useful for scenarios such as memory-mapped I/O, implementing shared memory between processes, and efficient file I/O operations. However, it requires careful management to avoid potential pitfalls such as buffer overflows or data corruption. The function takes six arguments:  
```
void *mmap(void *address, size_t length, int protect, int flags, int filedes, off_t offset)
```  
* address - It provides the preferred starting address used for mapping. If there is no other mapping, the kernel will pick the nearby page boundary, creating a mapping. Else, the kernel will pick the new address. If the value for this parameter is NULL, the kernel will place the mapping where it sees fit.
* length - The bytes’ number is mapped.
* protect - It controls what type of access is allowed. For instance, the PROT_READ for reading access, the PROT_WRITE for write access, and the PROT_EXEC for execution.
* flags - It is used for controlling the map’s nature. Some of the common and useful flags are listed below:
- MAP_SHARED - share mapping with other processes.
- MAP_FIXED - The system is forced to use the same mapping address given via the address parameter.
- MAP_ANONYMOUS / MAP_ANON - It creates anonymous mapping.
- MAP_PRIVATE - The mapping would be private and not visible to others while using this flag.
* filedes - The file descriptor is supposed to be mapped.
* offset - The file mapping starts from this offset.  

#### Question 1.5.2: What happens during a context switch? Do we want the OS to perform many or few context switches? Explain.  
During a context switch, the operating system saves the state of a process or thread that is currently running on the CPU and restores the state of a different process or thread so that it can start executing. This involves saving the contents of CPU registers, the program counter, and other relevant information associated with the process or thread that is being switched out, and then loading the corresponding information for the process or thread that is being switched in. Context switches are necessary for multitasking operating systems to give the illusion of concurrent execution to multiple processes or threads, even though the CPU can only execute one task at a time. However, context switches are not free. They incur overhead due to the need to save and restore the state of the CPU, and they can also lead to decreased performance if they occur too frequently. 
  
#### Question 1.5.3: What is the output of the following code snippet? You are given that the exec system call in the child does not succeed.
The output of the code will be:
> child
> parent



## An outline of what you did for the assignment;  


## Instructions for compiling your programs/scripts;  

### Step 1:  
```
git clone https://github.com/SelX19/OS_shell.git 
```  
### Step 2:  
Check if the C code is compiled, if not you need to compile it. We reccomend using this command:  
```
gcc -Wall -o "filenameYouWant" "custom_shell_1.c" 
```  
If you are on windows you will need to install the gcc/mingw and compile the code into an .exe:  
```
gcc -Wall -o "filenameYouWant.exe" "custom_shell_1.c"   
```   

## Any challenges you encountered along the way;  
Everything.

## Any sources/tutorials you have used to complete the assignment;  
ChatGPT, StackOverflow, YouTube  
  

If your implementation does not work, you should document the problems, preferably with your explanation of why it does not work and how you would solve it if you had more time;  


Of course, you should comment on your code.  


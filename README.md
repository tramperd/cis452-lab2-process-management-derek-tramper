
CIS 452 Lab 2: Process Management Concepts
-------------------------------------------------

Lab originally written by Prof. Greg Wolffe.

### Overview

The purpose of this lab is to become familiar with the mechanisms used by
operating systems for
process creation, execution management, blocking, and termination.
Specifically (for UNIX / Linux systems),
it introduces and experiments with the
`fork()`, `exec()`,` wait()`, and `exit()` system calls.

### Activities

* Work your way through the following exercises,
  demonstrating your knowledge of the material by answering the numbered
  questions.

* Write a program that demonstrates mastery of process operations.

* Submit a detailed lab report (one per group).
    * Include the answers to the numbered questions.

* Be prepared to demonstrate the Programming Assignment next week and to answer
  questions about its operation.

### Process Creation

All processes in UNIX are created, or spawned,
from existing processes via the `fork()` system call.
Both processes (parent and child) continue execution after the call.
Review your classnotes and read your textbook, the man pages, or a reference
book to understand what the `fork()` call does and how it operates.
Familiarize yourself with the `ps` (Process Status) utility and its various
options ---
it provides a great deal of useful information about a process.
Review the command-line mechanism (`&`) for inducing background execution of a
process and the `sleep()` function for temporarily suspending execution of a
process.

The two following sample programs illustrate the operation of the `fork()`
system call.

*Sample Program 1*

```c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    puts("Before fork");
    fork();
    puts("After fork");
    return 0;
}
```

Start up a terminal session,
which provides you with the UNIX command line interface.
**Perform the following operations and answer the questions:**

* compile and run Sample Program 1

1. how many lines are printed by the program?
2. describe what is happening to produce the answer observed for the above
   question

* insert a 10-second call to the function `sleep()` after the fork in Sample
  Program 1 and recompile
* run Program 1 in the background (use `&`)

3. consult the man pages for the `ps` (process status) utility;
   they will help you determine how to display and interpret the various types
   of information that is reported.
   Look especially for "verbose mode" or "long format".
   Then, using the *appropriate* options,
   observe and report the PIDs and the status (i.e. execution state info) of your
   executing program.
   Provide a brief explanation of your observations.

*Sample Program 2*

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int i, limit;

    if (argc < 2) {
        fputs("Usage: must supply a limit value\n", stderr);
        exit(1);
    }
    limit = atoi(argv[1]);

    fork();
    fork();
    printf ("PID#: %d\n", getpid());
    for (i=0; i<limit; i++)
        printf("%d\n", i);
    return 0;
}
```

**Perform the following operations and answer the questions:**

Study the code for Sample Program 2 until you understand it.

4. Create a diagram illustrating how Sample Program 2 executes.
   I.e., give a process hierarchy diagram
   (as described in class and in your textbook).

* Suggestion:
  run the program several times with small input values (e.g.,  2, ..., 5).
  Examine the output carefully so that you understand *exactly* what is
  happening.  

5. In the context of our classroom discussions on process state,
   process operations, and especially *process scheduling*,
   describe what you observed and try to explain what is happening to produce the
   observed results.

### Process Suspension and Termination

This section introduces the `wait()` system call and the `exit()` function,
which are usually related as in parent and child.
Note that there are several different versions of `wait()`
(e.g., some specify who to wait for).
The `exit()` function causes program termination;
resources are recovered, files are closed, resource usage statistics are
recorded, and the parent is notified via a signal
(provided it has executed a `wait`).
Refer to the man pages to learn the syntax for using these functions.

*Sample Program 3*

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{

    // use these variables

    pid_t pid, child;
    int status;

    if ((pid = fork()) < 0) {
        perror("fork failure");
        exit(1);
    }
    else if (pid == 0) {
        printf("I am child PID %ld\n", (long) getpid());
        /* insert an appropriate form of the exit() function here */

    }
    else {
        /* insert an appropriate form of the wait() system call here */

        printf("Child PID %ld terminated with return status %d\n", (long) child, status);
    }
    return 0;
}
```

**Perform the following operations and answer the questions:**

* add function calls to Sample Program 3 so that it *correctly* uses `wait()`
  and `exit()`.
  Basically, implement the comments,
  making use of the pre-declared variables referenced in the `printf()`
  statement.

6. provide the exact line of code that you inserted for the `wait()` system
   call.
7. who prints first, the child or the parent?  Why? Describe the interaction between 
   the `exit()` function and the `wait()` system call.
   You may want to experiment by changing the value to better understand the
   interaction.

### Process Execution

The `exec()` family of system calls provides a means of specifying that a
process
(typically the just-spawned child)
should be overlayed with a new executable.
The child will then execute different code from its parent.
As mentioned in class,
there are several different forms of the `exec()` system call.
Those with a 'v' (e.g., `execve()`) require a vector of pointers,
whereas those with an 'l' (e.g., `execle()`) expect a list of pointers.
Those with an 'e' allow you to specify an environmental variable,
and those with a 'p' allow you to specify a pathname to the executable.
The following sample program shows one form of the `exec()` call.
It is used in this program to execute any command (e.g., "`ls`", "`ps`")
that is issued by the user.

*Sample Program 4*

```c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        fputs("Usage: must supply a command\n", stderr);
        exit(1);
    }

    puts("Before the exec");
    if (execvp(argv[1], &argv[1]) < 0) {
        perror("exec failed");
        exit(1);
    }
    puts("After the exec");

    return 0;
}
```

**Perform the following operations and answer the questions:**

* compile, run and test Sample Program 4 using various commands
  (e.g., "`date`", "`ls`")

8. when is the second print line ("`After the exec`") printed?
   Explain your answer.
9. explain how the second argument passed to `execvp()` is used?

### Lab Programming Assignment (Simple Shell)

At this point you should have a good understanding of the relationship between
the fundamental system calls and library functions involved in process
management on a UNIX system.
You should now be able to write your own simple command interpreter, or shell. 
To do so, you will need to combine all of the ideas
(creation, suspension, execution, termination)
covered in class and in the lab.
A good starting point is to copy the code in `sampleProgram4.c` to `simpleShell.c`.

**Your program should:**

* display a prompt to the user
* get and parse the user input
    * read a line into a character array (preferably using `fgets()`)
    * depending on your implementation:
        * you can tokenize the line using your own custom function
        * *or* by using `strtok()`
            * `word_1 = strtok (line, " ");`
            * `word_2 = strtok (NULL, " ");`
        * *or* by using `strsep()`
            * `word_1 = strsep (&lineptr, " ");`
            * `word_2 = strsep (&lineptr, " ");`

* spawn a child process to execute the command
    * preferably use `execvp()/execve()` as in Sample Program 4 of this lab. 
      Note: you must create and pass a *vector* of pointers to arguments.
    * *or* use `execlp()/execle()` as in the example call below.
      Note: in this case you must pass a fixed *list* of arguments.

* Additional requirement:
  find and use the appropriate system call to collect *resource usage* statistics
  about each executed process

    * output the "user CPU time used" for *each* individual child process
    spawned by the shell
    * output the number of "involuntary context switches" experienced by
    *each* individual child process spawned by the shell

* Note: This is a very *simple* shell.
  You do not need to worry about
  pipeing (`|`), running background processes (`&`), or multiple commands per
  line (`;`),
  all of which a fully-featured shell would handle.


### Submission

Make sure all the required components of the lab are present in this repository (writeup, code, diagrams)
(and named such that I can tell what they are), then commit and push your changes. 
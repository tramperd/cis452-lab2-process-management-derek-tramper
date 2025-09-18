#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

int main() {
    char input[MAX_LINE];
    char *args[MAX_ARGS];
    struct rusage usage;

    while (1) {
        printf("myshell> ");
        if (!fgets(input, MAX_LINE, stdin)) {
            break;
        }

        // Exit command
        if (strcmp(input, "exit\n") == 0) {
            break;
        }

        // Tokenize input
        int i = 0;
        char *token = strtok(input, " \t\n");
        while (token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " \t\n");
        }
        args[i] = NULL;

        if (args[0] == NULL) {
            continue;
        }

        pid_t pid = fork();

        if (pid == 0) {
            // In child
            execvp(args[0], args);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            // In parent
            int status;
            wait4(pid, &status, 0, &usage);

            printf("User time: %ld.%06ld sec\n",
                   usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
            printf("Involuntary context switches: %ld\n", usage.ru_nivcsw);
        } else {
            perror("fork");
        }
    }

    return 0;
}


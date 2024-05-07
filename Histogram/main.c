#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#define MAX_FILES 100


int pipe_fds[MAX_FILES][2]; // Global array of pipes
pid_t child_pids[MAX_FILES]; // Array to keep track of pipe pids
int num_children = 0;
int currProcess = 0;

void sigchld_handler(int signum) {
    int status;
    pid_t pid;

    // Keep looping until no more terminated child processes are found
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("SIGCHLD caught, child PID: %d\n", pid);
        if (WIFEXITED(status)) {
            // Child exited normally
            printf("Child terminated normally with status: %d\n", WEXITSTATUS(status));
            for (int i = 0; i < num_children; i++) {
                if (child_pids[i] == pid) {
                    // Read data from the corresponding pipe and print the histogram
                    int *char_count = (int *)calloc(26, sizeof(int));
                    if (char_count == NULL) {
                        perror("Memory allocation failed");
                        exit(EXIT_FAILURE);
                    }

                    close(pipe_fds[i][1]); // Close the write end of the pipe in the parent
                    read(pipe_fds[i][0], char_count, sizeof(int) * 26);

                    // Save character counts to a file named "filePID.hist"
                    char filename[20]; // Array to store the filename
                    sprintf(filename, "file%d.hist", pid); // Construct the filename
                    FILE *file = fopen(filename, "w");
                    if (file == NULL) {
                        perror("Error opening file");
                        exit(EXIT_FAILURE);
                    }
                    // Write character counts to the file
                    for (int j = 0; j < 26; j++) {
                        fprintf(file, "%c %d\n", 'a' + j, char_count[j]);
                    }
                    fclose(file);

                    free(char_count);
                    break;
                }
            }
        } else if (WIFSIGNALED(status) ) {
            // Child terminated by a signal
            printf("Child terminated by signal: %d\n", WTERMSIG(status));
            currProcess--;
            break;
        }
    }

    currProcess--;
    if(currProcess <= 0)
    {
        exit(EXIT_SUCCESS);
    }

    // Re-register the signal handler
    signal(SIGCHLD, sigchld_handler);
}




void process_file(const char *filename, int index) {

    // Open the file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Initialize an array to store the count of each character
    int *char_count = (int *)calloc(26, sizeof(int));
    if (char_count == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }



    // Read the file character by character and update the character count
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (isalpha(c)) {
            c = tolower(c); // Convert to lowercase
            char_count[c - 'a']++;
        }
    }

    fclose(file);

    sleep(10 + 3 * (index)); // sleep i amount of seconds to avoid clashes upon process termination

    // Write the histogram data to the pipe
    close(pipe_fds[index][0]); // Close the read end of the pipe in the child
    write(pipe_fds[index][1], char_count, sizeof(int) * 26);
    close(pipe_fds[index][1]); // Close the write end of the pipe in the child

    // printf("Removing num children from 3 to count %d\n", num_children);

    // Free dynamically allocated memory
    free(char_count);
}



int main(int argc, char *argv[]) {
    // Check if there are enough arguments, if not error msg and exit

    signal(SIGCHLD, sigchld_handler);

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> <file2> ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    num_children = argc-1;
    currProcess = num_children;

    // Make a pipe for each file
    for (int i = 1; i < argc; i++) {
        
        // Normal file processing
        if (pipe(pipe_fds[i - 1]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Loop through each file specified in command line arguments
    for (int k = 1; k < argc; k++) 
    {

        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } 
        
        else if (pid == 0) 
        { // Child process
            int child_index = k-1;
            close(pipe_fds[child_index][0]); // Close the read end of the pipe in the child
            
            if (strcmp(argv[k], "SIG") == 0) 
            {
                // Send SIGINT to the corresponding child
                if (child_index > 0 && child_pids[child_index - 1] != 0) 
                {
                    if (kill(child_pids[child_index - 1], SIGINT) == -1) 
                    {
                        perror("kill");
                        exit(EXIT_FAILURE);
                    }
                } 
                
                else 
                {
                    fprintf(stderr, "No previous child process to send SIGINT\n");
                    exit(EXIT_FAILURE);
                }
            } 
            
            else 
            {
                // Normal file processing
                process_file(argv[k], child_index);
                exit(EXIT_SUCCESS);
            }
        }
        
        else 
        {  // Parent process

            // Wait for the child process to finish
            // printf("******PARENT PID NUMBER %d*****\n", getpid());

            close(pipe_fds[k-1][1]); // close writing process
            child_pids[k-1] = pid;
        }
    }

    while (currProcess>0) {
        sleep(1);
    }
    return 0;
}
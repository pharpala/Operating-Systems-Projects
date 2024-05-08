#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_WORD_LENGTH 100
#define MAX_FILENAME_LENGTH 100
#define MAX_UNKNOWN_WORDS 2000

int active_threads = 0;
int files_processed = 0;
int total_spelling_errors = 0;

// Structure to pass arguments to the worker thread
typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
} MisspelledWord;

typedef struct {
    char text_file[MAX_FILENAME_LENGTH];
    char dictionary_file[MAX_FILENAME_LENGTH];
    int mistakes;
    MisspelledWord unknown_words[MAX_UNKNOWN_WORDS];
} SpellcheckerArgs;

SpellcheckerArgs globalArgs[MAX_UNKNOWN_WORDS];
pthread_mutex_t mutexFun = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexGlob = PTHREAD_MUTEX_INITIALIZER;




void initializeGlobalArray() {
    pthread_mutex_lock(&mutexGlob);
    for (int i = 0; i < MAX_UNKNOWN_WORDS; i++) {
        strcpy(globalArgs->unknown_words[i].word, "");
        globalArgs->unknown_words[i].count = 0;
    }
    pthread_mutex_unlock(&mutexGlob);
}

void preprocess_word(char *word) {
    int i, j = 0;
    for (i = 0; word[i] != '\0'; i++) 
    {
        word[j] = tolower(word[i]);
        j++;
    }
    word[j] = '\0';
}

void getTopThree () {
    int max_counts[3] = {0, 0, 0};
    char max_words[3][MAX_WORD_LENGTH] = {0};
    pthread_mutex_lock(&mutexGlob);

        for (int i = 0; i < MAX_UNKNOWN_WORDS; i++) 
        {
            if (globalArgs->unknown_words[i].count > max_counts[0]) 
            {
                max_counts[2] = max_counts[1];
                strcpy(max_words[2], max_words[1]);
                max_counts[1] = max_counts[0];
                strcpy(max_words[1], max_words[0]);
                max_counts[0] = globalArgs->unknown_words[i].count;
                strcpy(max_words[0], globalArgs->unknown_words[i].word);
            } 
            
            else if (globalArgs->unknown_words[i].count > max_counts[1]) 
            {
                max_counts[2] = max_counts[1];
                strcpy(max_words[2], max_words[1]);
                max_counts[1] = globalArgs->unknown_words[i].count;
                strcpy(max_words[1], globalArgs->unknown_words[i].word);
            } 
            
            else if (globalArgs->unknown_words[i].count > max_counts[2]) 
            {
                max_counts[2] = globalArgs->unknown_words[i].count;
                strcpy(max_words[2], globalArgs->unknown_words[i].word);
            }
        }

        printf("Three most frequently occurring unknown words:\n");
        for (int i = 0; i < 3; i++) 
        {
            if (max_counts[i] > 0) 
            {
                printf("%s (%d occurrences) ", max_words[i], max_counts[i]);
            }
        }

    printf("\n\n");
    pthread_mutex_unlock(&mutexGlob);
}


void getTopThreeTh (SpellcheckerArgs *spell_args) {
    int max_counts[3] = {0, 0, 0};
    char max_words[3][MAX_WORD_LENGTH] = {0};

        for (int i = 0; i < MAX_UNKNOWN_WORDS; i++) 
        {
            if (spell_args->unknown_words[i].count > max_counts[0]) 
            {
                max_counts[2] = max_counts[1];
                max_counts[1] = max_counts[0];
                strcpy(max_words[1], max_words[0]);
                max_counts[0] = spell_args->unknown_words[i].count;
                strcpy(max_words[0], spell_args->unknown_words[i].word);
            } 
            
            else if (spell_args->unknown_words[i].count > max_counts[1]) 
            {
                max_counts[2] = max_counts[1];
                strcpy(max_words[2], max_words[1]);
                max_counts[1] = spell_args->unknown_words[i].count;
                strcpy(max_words[1], spell_args->unknown_words[i].word);
            } 
            
            else if (spell_args->unknown_words[i].count > max_counts[2]) 
            {
                max_counts[2] = spell_args->unknown_words[i].count;
                strcpy(max_words[2], spell_args->unknown_words[i].word);
            }
        }

        // printf("Three most frequently occurring unknown words:\n");
        for (int i = 0; i < 3; i++) 
        {
            if (max_counts[i] > 0) 
            {
                printf("%s (%d occurrences) ", max_words[i], max_counts[i]);
            }
        }

    printf("\n\n");
}


// Function to perform spellchecking
void *spellcheck(void *args) {
    active_threads++;
    pthread_mutex_lock(&mutexFun);
    SpellcheckerArgs *spell_args = (SpellcheckerArgs *)args;
    FILE *text_fp, *dict_fp;
    char word[MAX_WORD_LENGTH];
    spell_args->mistakes = 0;

    text_fp = fopen(spell_args->text_file, "r");
    if (text_fp == NULL) {
        perror("Error opening text file"); // This should never trigger since we have error handling in main, JIC
        exit(EXIT_FAILURE);
    }

    dict_fp = fopen(spell_args->dictionary_file, "r");
    if (dict_fp == NULL) {
        perror("Error opening dictionary file"); // This should never happen since we have error handling in main, JIC
        exit(EXIT_FAILURE);
    }

    // Initialize unknown words array
    for (int i = 0; i < MAX_UNKNOWN_WORDS; i++) {
        spell_args->unknown_words[i].count = 0;
        strcpy(spell_args->unknown_words[i].word, "");
    }

    // Read each word from text file
    while (fscanf(text_fp, "%s", word) == 1) {
        preprocess_word(word);

        // Check if word exists in dictionary
        char dict_word[MAX_WORD_LENGTH];
        int found = 0;
        rewind(dict_fp); // Start from beginning of dictionary file
        while (fscanf(dict_fp, "%s", dict_word) == 1) {
            if (strcmp(word, dict_word) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            // printf("Misspelled word: %s\n", word);
            spell_args->mistakes++;
            
            // global variable mistake
            total_spelling_errors++;

            int index = -1;
            for(int i = 0; i < spell_args->mistakes; i++) {
                if(strcmp(spell_args->unknown_words[i].word, word) == 0) {
                    index = i;
                    break;
                }
                if(index == -1 || spell_args->unknown_words[i].count < spell_args->unknown_words[index].count) {
                    index = i;
                }
            }

            if(index != -1) {
                strcpy(spell_args->unknown_words[index].word, word);
                spell_args->unknown_words[index].count++;
                
                // Store mispellings in global variable
                strcpy(globalArgs->unknown_words[index].word, word);
                globalArgs->unknown_words[index].count++;
            }
        }
    }

    // printf("THIS IS THE File Name: %s.txt\n", spell_args->text_file);
    freopen("pharpala_A2.out", "a", stdout);

    printf("%s.txt ", spell_args->text_file);
    printf(" %d\n", spell_args->mistakes);
    // This will calculate and display the top 3 frequent words
    getTopThreeTh(spell_args);

    fclose(text_fp);
    fclose(dict_fp);
    fclose(stdout);
    freopen("/dev/tty", "w", stdout);

    // sleep(10); // Simulate long running task (10 seconds)
    files_processed++;
    active_threads--;
    pthread_mutex_unlock(&mutexFun);
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    pthread_t worker_thread;
    SpellcheckerArgs args;
    int choice;
    char text_file[MAX_FILENAME_LENGTH], dictionary_file[MAX_FILENAME_LENGTH];
    initializeGlobalArray();
    int display_summary = 1;
    FILE *errorDict, *errorText;

    if(argc == 2 && strcmp(argv[1], "-l") == 0)
    {
        display_summary = 0;
    }
    while(1)
    {
        printf("************* Main Menu ******************\n");
        printf("Enter 1 to start a new spellchecking task\n");
        printf("Enter 2 to exit\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        printf("******************************************\n");

        switch(choice) 
        {
            case 1:

                printf("Enter the name of the dictionary file (0 to exit): ");
                scanf("%s", dictionary_file);
                if (strcmp(dictionary_file, "0") == 0)
                {
                    printf("Returning to main menu...\n"); // Returning to main menur routine
                    break;
                }

                // Error handler in case dictionary file is not found
                errorDict = fopen(dictionary_file, "r");
                if (errorDict == NULL) {
                    perror("Error opening dictionary file");
                    exit(EXIT_FAILURE);
                }
                fclose(errorDict);
                
                
                printf("Enter the name of the text file (0 to exit): ");
                scanf("%s", text_file);
                if (strcmp(text_file, "0") == 0)
                {
                    printf("Returning to main menu...\n"); // Returning to main menur routine
                    break;
                } 

                // Error handler in case text file is not found
                errorText = fopen(text_file, "r");
                if (errorText == NULL) {
                    perror("Error opening input file");
                    exit(EXIT_FAILURE);
                }
                fclose(errorText); 

        
                strcpy(args.text_file, text_file);
                strcpy(args.dictionary_file, dictionary_file);

                // Create worker thread
                if (pthread_create(&worker_thread, NULL, spellcheck, (void *)&args) != 0) {
                    perror("Error creating thread");
                    exit(EXIT_FAILURE);
                }
                
                int result = pthread_detach(worker_thread);
                if (result != 0) {
                    perror("Thread detachment failed");
                    exit(EXIT_FAILURE);
                }

                // Wait for worker thread to finish
                /*if (pthread_join(worker_thread, NULL) != 0) {
                    perror("Error joining thread");
                    exit(EXIT_FAILURE);
                }*/

                printf("\n");
                break;


            case 2:
                printf("Summary: \n");
                if (active_threads > 0) 
                {
                    printf("There are %d active threads. Exiting...\n", active_threads);
                } 
                else {
                        printf("All threads have finished running. Exiting...\n");
                }

                if(display_summary)
                {
                    printf("Total files processed: %d\n", files_processed);
                    printf("Total spelling mistakes: %d\n", total_spelling_errors);
                    // This will calculate and display the top 3 frequent words
                    getTopThree();
                } 
                
                else 
                {
                    freopen("pharpala_A2.out", "a", stdout);

                    printf("Total files processed: %d\n", files_processed);
                    printf("Total spelling mistakes: %d\n", total_spelling_errors);
                    // This will calculate and display the top 3 frequent words
                    getTopThree();
                    fclose(stdout);
                }

                exit(EXIT_SUCCESS);
            default:
                printf("Invalid choice\n");
                exit(0);
        }
    }

    pthread_mutex_destroy(&mutexFun);
    pthread_mutex_destroy(&mutexGlob);


    return 0;
}

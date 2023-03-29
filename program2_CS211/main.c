#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//takes goal word, first word(any iteration), and desired word length, checks both are correct length, return true is they are
bool checkLength(char *firstWord, char *lastWord, int wordSearchLength){
    int firstWordLength, lastWordLength;
    firstWordLength = (int)strlen(firstWord);
    lastWordLength = (int)strlen(lastWord);

    if(firstWordLength != wordSearchLength){
        printf("Your word, '%s', is not a %d-letter word. Try again.\n", firstWord, wordSearchLength);
        return false;
    }

    if(lastWordLength != wordSearchLength){
        printf("Your word, '%s', is not a %d-letter word. Try again.\n", lastWord, wordSearchLength);
        return false;
    }
    return true;
}

//takes proper word length, scans dict. and counts for all words with matching length, returns num. words w/matching length
int scanForAllWordsWithLength(int wordSearchLength){
    int wordCount = 0;
    char inputString[81];

    char filename[] = "/Users/jailinturner/Desktop/CS 211/words.txt";
    FILE *filePtr =
    fopen(filename, "r"); //"r" means we open the file for reading

    if(filePtr == NULL){
        printf("Error: could not open %s for reading\n", filename);
        exit(EXIT_FAILURE);
    }

    //scan for words w/proper
    while(fscanf(filePtr, "%s", inputString) != EOF){
        if(wordSearchLength == strlen(inputString)){
            wordCount++;
        }
    }

    fclose(filePtr);
    return wordCount;
}

//takes 2D wordArray, num words in array w/proper len., both input words
//if in play mode, returns boolean T/F of print if not in play mode if words are valid or not
bool verifyWords(char ***wordArray, char *firstWord, char *lastWord, int numberOfWords, int playing){
    int firstWordFound = 0;
    int secondWordFound = 0;
    int i;

    for(i = 0; i < numberOfWords; i++){
        if(strcmp((*wordArray)[i], firstWord) == 0){
            firstWordFound = 1;
            break;
        }
    }

    for (i = 0; i < numberOfWords; i++) {
        if(strcmp((*wordArray)[i], lastWord) == 0){
            secondWordFound = 1;
            break;
        }
    }

    //if both words found print the following, if playing return true
    if(firstWordFound && secondWordFound && playing == 0){
        printf("Your starting word is: %s.\n", firstWord);
        printf("Your ending word is: %s.\n", lastWord);
    }
    else if(firstWordFound && secondWordFound && playing == 1){
        return true;
    }

    //return false and print not valid, if user is playing or not
    else if(!firstWordFound){
        printf("Your word, '%s', is not a valid dictionary word. Try again.\n", firstWord);
        return false;
    }
    else if(!secondWordFound){
        printf("Your word, '%s', is not a valid dictionary word. Try again.\n", lastWord);
        return false;
    }
    return true;
}

//takes in empty 2D array, length of the words, fills array dynamically with all words with matching length
void storeWordsFoundOfLength(char ***wordArray, int wordSearchLength){
    char inputString[81];
    char filename[] = "/Users/jailinturner/Desktop/CS 211/words.txt";
    int maxWords = 10;
    int numWords = 0;
    *wordArray = malloc(maxWords * sizeof(char*));  //allocating memory space for 2D array

    FILE *filePtr = fopen(filename, "r");

    if(filePtr == NULL){
        printf("Error: could not open %s for reading\n", filename);
        exit(EXIT_FAILURE);
    }

    if(*wordArray == NULL){
        printf("Error: could not allocate memory for word array\n");
        exit(EXIT_FAILURE);
    }

    //double size of array if it's full and allocate more memory each time
    while(fscanf(filePtr, "%s", inputString) != EOF){
        if(strlen(inputString) == wordSearchLength){
            if(numWords == maxWords){
                maxWords *= 2;
                *wordArray = realloc(*wordArray, maxWords * sizeof(char*));
                if (*wordArray == NULL) {
                    printf("Error: could not reallocate memory for word array\n");
                    exit(EXIT_FAILURE);
                }
            }

            (*wordArray)[numWords] = malloc((wordSearchLength + 2) * sizeof(char));
            if((*wordArray)[numWords] == NULL){
                printf("Error: could not allocate memory for word %d\n", numWords);
                exit(EXIT_FAILURE);
            }

            strncpy((*wordArray)[numWords], inputString, wordSearchLength + 1);
            numWords++;
        }
    }

  // Shrink array to actual size when done with filling array
    *wordArray = realloc(*wordArray, numWords * sizeof(char*));
    if(*wordArray == NULL && numWords > 0){
        printf("Error: could not reallocate memory for word array\n");
        exit(EXIT_FAILURE);
    }
    fclose(filePtr);
}

//while playing check if all words passed are different by only one char while within the length of each word
bool checkForCorrectDifference(char *newWord, char *startingWord, char *goalWord, int wordSearchLength){
    int differenceCount = 0;

    for(int i = 0; i < wordSearchLength; i++){
        if (newWord[i] != startingWord[i]) {
            differenceCount++;
        }
    }

    if((differenceCount == 0) || (differenceCount > 1)){
        printf("Your word, '%s', is not exactly 1 character different. Try again.\n", newWord);
        return false;
    }
    else if(differenceCount == 1){
        return true;
    }

    return 0;
}

//playing after user inputs 2 initial valid words (or "r"), takes length of each word, the array holding the words, and count of all words in the array
void playWeaver(char *startingWord, char *goalWord, int wordSearchLength, char ***wordArray, int numberOfWords){
    int count = 1;
    int playing = 1;
    bool sameWordYay = false;
    char *newWord = malloc((wordSearchLength + 2) * sizeof(char));
    char *holdStartingWord = malloc((wordSearchLength + 2) * sizeof(char));

    printf("\n%d. Previous word is '%s'. Goal word is '%s'. Next word: ", count, startingWord, goalWord);
    scanf(" %s", newWord);

    strcpy(holdStartingWord, startingWord); //copy the starting word from main that needs to change into goal word

    while(sameWordYay == false){
        if(strcmp(newWord, "q") == 0){  //user decides to quit playing
            break;
        }

        //check if new word entered is valid and playable, if not continue at top of while loop
        if(checkLength(newWord, goalWord, wordSearchLength) == false || verifyWords(wordArray, newWord, goalWord, numberOfWords, playing) == false){
            printf("\n%d. Previous word is '%s'. Goal word is '%s'. Next word: ", count, startingWord, goalWord);
            scanf(" %s", newWord);
            continue;
        }
        else if(checkLength(newWord, goalWord, wordSearchLength) == true && verifyWords(wordArray, newWord, goalWord, numberOfWords, playing) == true){
            if(checkForCorrectDifference(newWord, startingWord, goalWord, wordSearchLength) == false){
                printf("\n%d. Previous word is '%s'. Goal word is '%s'. Next word: ",
                count, startingWord, goalWord);
                scanf(" %s", newWord);
                continue;
            }

            //if words are of correct difference and made it to goal word, user is done playing and can return to main
            else if(checkForCorrectDifference(newWord, startingWord, goalWord, wordSearchLength) == true){
                if(strcmp(newWord, goalWord) == 0){
                    printf("Congratulations! You changed '%s' into '%s' in %d moves.\n", holdStartingWord, goalWord, count);
                    sameWordYay = true;
                }

                //otherwise continue while loop w/starting word as the new word
                else if(strcmp(newWord, goalWord) != 0){
                    count++;
                    strcpy(startingWord, newWord);
                    printf("\n%d. Previous word is '%s'. Goal word is '%s'. Next word: ", count, startingWord, goalWord);
                    scanf(" %s", newWord);
                }
            }
        }
    }
    free(newWord);
    free(holdStartingWord);
}

//main function initiates the game and only allows user to start playing once both words are valid
int main(void){
    time_t startTime;
    startTime = time(NULL);
    srand(1);
    int wordSearchLength;
    int playing = 0;
    int userInput = 0;
    int randomNumber;
    char **wordArray; //2D array for storing all words of proper length
    char *firstWord;
    char *lastWord;

    printf("Weaver is a game where you try to find a way to get from the starting word to the ending word.\n");
    printf("You can change only one letter at a time, and each word along the way must be a valid word.\n");
    printf("Enjoy!\n\n");
    printf("How many letters do you want to have in the words? ");
    scanf("%d", &wordSearchLength);

    int numberOfWords = scanForAllWordsWithLength(wordSearchLength);    //counting words w/length entered

    firstWord = malloc((wordSearchLength + 1) * sizeof(char*));
    lastWord = malloc((wordSearchLength + 1) * sizeof(char*));

    storeWordsFoundOfLength(&wordArray, wordSearchLength);  //filling array with words of length entered
    
    printf("Number of %d-letter words found: %d.\n\n", wordSearchLength, numberOfWords);

    while(userInput != 3){

        printf("Enter starting and ending words, or 'r' for either for a random word: ");
        scanf("%s%s", firstWord, lastWord);

        //random word generators
        if(strcmp(firstWord, "r") == 0){
            randomNumber = rand() % numberOfWords;
            strcpy(firstWord, wordArray[randomNumber]);
        }
        if(strcmp(lastWord, "r") == 0){
            randomNumber = rand() % numberOfWords;
            strcpy(lastWord, wordArray[randomNumber]);
        }

        if(checkLength(firstWord, lastWord, wordSearchLength) == false){
            continue;
        }
        if(verifyWords(&wordArray, firstWord, lastWord, numberOfWords, playing) == false){
            continue;
        }

        printf("On each move enter a word of the same length that is at most 1 character different and is also in the dictionary.\n");
        printf("You may also type in 'q' to quit guessing.\n");

        playWeaver(firstWord, lastWord, wordSearchLength, &wordArray, numberOfWords);   //play if words are valid, continue while loop if not

        //options if the user changed first word to last word or decided to quit playing
        printf("\nEnter: \t1 to play again,\n");
        printf("\t2 to change the number of letters in the words and then play again, or\n");
        printf("\t3 to exit the program.\n");
        printf("Your choice --> ");
        scanf("%d", &userInput);

        if(userInput == 1){
            continue;
        }

        //if user wants to change num. letters the word array and each iteration must be freed otherwise memory leaks occur
        else if(userInput == 2){
            for(int i = 0; i < numberOfWords; i++){
                free(wordArray[i]);
            }
            free(wordArray);
            printf("How many letters do you want to have in the words? ");
            scanf("%d", &wordSearchLength);
            numberOfWords = scanForAllWordsWithLength(wordSearchLength);
            printf("Number of %d-letter words found: %d.\n\n", wordSearchLength, numberOfWords);
            storeWordsFoundOfLength(&wordArray, wordSearchLength);
            continue;
        }
        else{
            printf("\n\nThanks for playing!\nExiting...");
            break;
        }
    }

    //free everything after program finishes running so memory leaks are created
    free(firstWord);
    free(lastWord);

    for(int i = 0; i < numberOfWords; i++){
        free(wordArray[i]);
    }
    free(wordArray);
    printf("\n\nrunning total time: %6.1f\n", difftime(time(NULL), startTime));
    return 0;
}


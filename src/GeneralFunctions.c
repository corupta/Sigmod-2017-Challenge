/*
 * Made for ACM SIGMOD 2017 Programming Contest
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "GeneralFunctions.h"
#include "FastPrintF.h"


//PRIVATE DECLERATIONS

//private variables

static unsigned int currentOpTime;

//private functions

unsigned int* convToNGram(unsigned char*str) {
    //unsigned char* str = strdup(input);
    unsigned char* saveptr, *token;
    unsigned int* ngram;
    unsigned int len = 1, arrSize = 127;
    ngram = malloc(arrSize * sizeof (unsigned int));
    token = strtok_r(str, " \n", &saveptr);
    while (token != NULL) {
        if (++len == arrSize) {
            arrSize = (arrSize << 1) + 1;
            ngram = realloc(ngram, arrSize * sizeof (unsigned int));
        }
        ngram[len - 1] = insertWord(token);
        token = strtok_r(NULL, " \n", &saveptr);
    }
    //len - 1 is important
    ngram[0] = len - 1;
    ngram = realloc(ngram, len * sizeof ( unsigned int));
    //free(str);
    return (ngram);
}

void addNewNGramInit(unsigned char *input) {
    //currTime = 0;
  //  printf("NewNGram: %s",input);
    unsigned int* ngram = convToNGram(input);
    unsigned int id;
   // for(id=1;id<=ngram[0];++id)printf(" %u",ngram[id]);
    //printf("\n");
    //it is given that inits dont exist already
    //not exists
    //fake you have to check it
    if(ngram[0]){
        id = insertNGramToPrefixTree(ngram);
      //  printf("MyInsertedId: %u\n",id);
        //addNGramToPrefixTree(input)
        if( getNGramEndState(id) == 0 ){
            updNGramBegState(id);
            updNGramEndState(id);
        }
    }
}

void addNewNGram(unsigned char *input) {
    //check prefix tree for NGram
    unsigned int *ngram = convToNGram(input);
    unsigned int id;
    //if found
    //id = found_id
    //DESTROYS POINTER ngram
    //free(ngram);
    //else if not exists
    id = insertNGramToPrefixTree(ngram);
    if (getNGramEndState(id) == 0) {
        addOpState(id);
        updNGramEndState(id);
        ++currentOpTime;
    }
}

void deleteNewNGram(unsigned char*input) {
    unsigned int* ngram = convToNGram(input);
    unsigned int id;
    //check prefix tree for NGram
    //if not found
    //error!!
    //id = found_id
    id = getNGram(ngram);
    if (getNGramEndState(id) == 1) {
        addOpState(id);
        updNGramEndState(id);
        ++currentOpTime;
    }
    //DESTROYS POINTER ngram
    free(ngram);
}


/*
 * call this function with queryNewLine( strdup(input), currLine);
 * it frees the string given to it
 */
void queryNewLine(unsigned char*str, Line* myLine) {
    //lots of mutexes
    //unsigned char* str = strdup(input);
    unsigned char* saveptr, *token;
    unsigned int* currNGram;
    unsigned int len = 1, nextWord, i, j = 0, arrSize = 127;
    WordGroup *myGroup;
    currNGram = malloc(arrSize * sizeof (unsigned int));
    token = strtok_r(str, " \n", &saveptr);
    while (token != NULL) {
        nextWord = getIdByWord(token);
        if (nextWord) {
            if (++len == arrSize) {
                arrSize = (arrSize << 1) + 1;
                currNGram = realloc(currNGram, arrSize * sizeof (unsigned int));
            }
            currNGram[len - 1] = nextWord;
           // if( len> 2 && currNGram[len -1] == 136 && currNGram[len-2] == 136)
             //   printf("\nmeowth!\n");
        } else if (len > 1) {
            //a word group is ready
            currNGram[0] = len - 1;
            //len - 1 is important
            currNGram = realloc(currNGram, len * sizeof ( unsigned int));
            myGroup = pushCreateWordGroup(myLine, len - 1);
            //start a new thread of search
            searchNGrams(currNGram, myGroup);
            //thread creation finished
            arrSize = 127;
            currNGram = malloc(arrSize * sizeof (unsigned int));
            len = 1;
        }
        //else null after null so skip
        token = strtok_r(NULL, " \n", &saveptr);
    }
    if (len > 1) {
        //a word group is ready
        currNGram[0] = len - 1;
        //len - 1 is important
        currNGram = realloc(currNGram, len * sizeof ( unsigned int));
        myGroup = pushCreateWordGroup(myLine, len - 1);
        //start a new thread of search
        searchNGrams(currNGram, myGroup);
        //thread creation finished
    } else {
        free(currNGram);
    }
    pushEnd(myLine->groups);
    free(str);
}

//PUBLIC DECLERATIONS

void initializer() {
    initializeWordLibrary();
    initializeNGramLibrary();
    initializePrefixTree();
    initializeFastPrinter();
    initializePrinter();
}
void lineReader() {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, stdin)) != -1) {
#ifdef DBG
        //printf("read %ld bytes: %s",read,line);
#endif
        if (read == 2 && line[0] == 'S'){
#ifdef DBG
            printf("WordLibrary:\n%s\n", dumpWordLibrary());printf("NGramLibrary:\n%s\n", dumpNGramLibrary());//dumpPrefixTree(1);dumpPrefixTree(0);
#else
            break;
#endif
        }else{
            
            addNewNGramInit(line);
        }
#ifdef DBG
        //printf("next Line:");
#endif
        //printf("ended init, read: %ld\n", read);
    }
    
    
    printf("R\n");
    while ((read = getline(&line, &len, stdin)) != -1) {
        if(read == 0 || line[0] == '\n')
            break;
        switch (line[0]) {
            case 'Q':
                //printf("started q: %s\n",line);
                //call query in new thread
                queryNewLine(strdup(line + 2), pushCreateLine(currentOpTime));
                //end thread creation
#ifdef LIN_PRT
                superPrinter();
                superWriter();
#endif
                //printf("ended q\n");
                /*printf("WordLibrary:\n%s\n", dumpWordLibrary());
                printf("NGramLibrary:\n%s\n", dumpNGramLibrary());
                dumpPrefixTree();*/
                break;
            case 'D':
                //printf("started d\n");
                deleteNewNGram(line + 2);
                //printf("ended d\n");
                break;
            case 'A':
                //printf("started a\n");
                addNewNGram(line + 2);
                /*printf("WordLibrary:\n%s\n", dumpWordLibrary());
                printf("NGramLibrary:\n%s\n", dumpNGramLibrary());
                dumpPrefixTree();*/
                //printf("ended a\n");
            default:;
        }
        /*if (read == 2 && line[0] == 'F')
            break;*/
    }
    free(line);
}


/*
 todo list
 * fix bug
 * experiment size speed
 * make parallel
 */
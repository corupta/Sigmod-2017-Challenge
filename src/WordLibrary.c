/*
 * Made for ACM SIGMOD 2017 Programming Contest
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WordLibrary.h"

//PRIVATE STUFF DECLERATIONS


//private variables

static const unsigned int WORD_LIST_INIT_SIZE = 127;
static unsigned int arrSize;
static unsigned int lastGivenId;
static Trie** wordList;

//private functions

void doubleWordLibrarySize() {
    //lock mutex
    arrSize = (arrSize << 1) + 1;
    wordList = realloc(wordList, arrSize * sizeof(Trie*));
    //unlock
}

/*FREE THE POINTER AFTER USE!
 */
unsigned char* getWordById(unsigned int i) {
    //called by print function
    //check mutex lock for list
    return getWordFromTrie(wordList [ i ]);
}

//PUBLIC STUFF DECLERATIONS

void initializeWordLibrary() {
    initializeTrie();
    arrSize = WORD_LIST_INIT_SIZE ;
    wordList = malloc(arrSize * sizeof(Trie*));
}

unsigned int insertWord(unsigned char *str) {
    //printf("insert %s\n",str);
    Trie* newNode = insertIntoTrie(str);
    unsigned int myId = getIdFromEndNode(newNode);
    if(myId>lastGivenId){
        if (++lastGivenId == arrSize)
            doubleWordLibrarySize();
        wordList[ myId ] = newNode;
    }
    return myId;
}

unsigned int getIdByWord(unsigned char *str) {
    return getFromTrie(str);
}


void printWord(unsigned int i){
    unsigned char* s;
    s = getWordById(i);
    printStr(s);
    free(s);
}

//DEBUG
#ifdef DBG
unsigned char* dumpWordLibrary(){
    unsigned int i;
    char *ans;
    ans = malloc ( 2000000 );
    memset(ans,0,sizeof(ans));
    char tmp[500000];
    for(i=1;i<=lastGivenId;++i){
        sprintf(tmp,"%u => %u => %s => len: %u \n",i,wordList[i],getWordById(i), strlen(getWordById(i)));
        strcat(ans,tmp);
    }
    return ans;
}
#endif

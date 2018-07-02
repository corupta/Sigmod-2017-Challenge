/*
 * Made for ACM SIGMOD 2017 Programming Contest
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "NGramLibrary.h"
#include "WordLibrary.h"

//PRIVATE STUFF DECLERATIONS


//private variables

static const unsigned int NGRAM_LIST_INIT_SIZE = 127;
static unsigned int arrSize;
static unsigned int lastGivenId;
static unsigned int currTime;
NGram * nGramList;
OpState *currOp, *lastOp;

//private functions

void doubleNGramLibrarySize() {
    //lock mutex
    arrSize = (arrSize << 1) + 1;
    nGramList = realloc(nGramList, arrSize * sizeof(NGram));
    //unlock
}

//PUBLIC STUFF DECLERATIONS

void initializeNGramLibrary() {
    arrSize = NGRAM_LIST_INIT_SIZE ;
    nGramList = malloc(arrSize * sizeof(NGram));
    lastOp = malloc(sizeof(OpState));
    lastOp->next = NULL;
    currOp = lastOp;
}

unsigned int insertNGram(unsigned int *ngram) {
    if (++lastGivenId == arrSize)
        doubleNGramLibrarySize();
    /*
    unsigned int len = ngram[0] + 1;
    nGramList[ lastGivenId ].val = malloc(len * sizeof (unsigned int));
    memcpy(nGramList[ lastGivenId ].val, ngram, len * sizeof ( unsigned int));
    */
    nGramList [ lastGivenId ].val = ngram;
    nGramList[ lastGivenId ].beg = 0;
    nGramList[ lastGivenId ].end = 0;
    return lastGivenId;
}
/*
/*avoid using this function !!!

NGram * getNGramById(unsigned int id) {
    //check if list is mutex locked
    return &nGramList[id];
}
*/
/*  not used anymore !!
 
void readyAllInitialNGrams(){
    unsigned int i;
    for(i=1;i<=lastGivenId;++i)
        nGramList [ i ].beg = 1;
}
*/
unsigned int getValAtNGramInd(unsigned int id, unsigned int index){
    //called by other threads
    unsigned int ans;
    //check if list is mutex locked
    ans = nGramList[id].val[index];
    //end check
    return ans;
    
}
unsigned int getValAtNGramIndUnSafeFast(unsigned int id, unsigned int index){
    return nGramList[id].val[index];
}
unsigned char getNGramBegState(unsigned int id){
    //called by print function
    //check if list is mutex locked
    return nGramList[id].beg;
}
unsigned char getNGramEndState(unsigned int id){
    //called by insert/delete function
    return nGramList[id].end;
}
void updNGramBegState(unsigned int id){
    //called by print function
    //check if list is mutex locked
    nGramList[id].beg = !nGramList[id].beg;
}
void updNGramEndState(unsigned int id){
    //called by insert/delete function
    nGramList[id].end = !nGramList[id].end;
}

void addOpState(unsigned int id){
    lastOp->next = malloc(sizeof(OpState));
    lastOp = (OpState*)(lastOp->next);
    lastOp->id = id;
    lastOp->next = NULL;
}

void useOpState(unsigned int endTime){
    while(currTime < endTime){
        OpState* tmp = currOp;
        currOp = currOp->next;
        ++currTime;
        updNGramBegState(currOp->id);
        free(tmp);
    }
}

void printNGram(unsigned int id){
    unsigned int len, j, wordId;
    len = getValAtNGramInd(id,0);
    for( j = 1 ; j< len; ++j){
        wordId = getValAtNGramInd(id,j);
        printWord(wordId);
        printChar(' ');
    }
    wordId = getValAtNGramInd(id,j);
    printWord(wordId);
}

/*
 * depreciated
unsigned int getNextId(){
    return lastGivenId + 1;
}*/

//DEBUG

#ifdef DBG

unsigned char* dumpNGramLibrary(){
    unsigned int i,j;
    char *ans;
    ans = malloc ( 120000 );
    memset(ans,0,sizeof(ans));
    char tmp[12000] = {0};
    for(i=1;i<=lastGivenId;++i){
        
        sprintf(tmp,"%u => %u => ( ",i,nGramList[i]);
        strcat(ans,tmp);
        for(j = 1 ; j <= getValAtNGramInd(i,0) ; ++j ){
            sprintf(tmp, "%u => %s , ",getValAtNGramInd(i,j),getWordById(getValAtNGramInd(i,j)));
            strcat(ans,tmp);
        }
        sprintf(tmp," )\n");
        strcat(ans,tmp);
    }
    return ans;
}

#endif
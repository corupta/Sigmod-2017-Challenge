/*
 * Made for ACM SIGMOD 2017 Programming Contest
 */

#include <stddef.h>
#include <stdlib.h>

#include "Printer.h"
#include "Trie.h"
#include "NGramLibrary.h"
#include "FastPrintF.h"



//PRIVATE DECLERATIONS

//private variables

static unsigned int currRunTime;
static Queue* lines;

//private functions

QueueNode* createEmptyQueueNode(){
    QueueNode* tmp;
    tmp = malloc(sizeof(QueueNode));
    tmp->nextNode = NULL;
    tmp->val = NULL;
    return tmp;
}

QueueNode* createQueueNode(void * t){
    QueueNode* tmp;
    tmp = malloc(sizeof(QueueNode));
    tmp->nextNode = NULL;
    tmp->val = t;
    return tmp;
}

Queue* createQueue(){
    Queue* tmp;
    tmp = malloc(sizeof(Queue));
    tmp->endNode = tmp->currNode = createEmptyQueueNode();
    return tmp;
}

void push( Queue* q, void* t){
    QueueNode* tmp;
    tmp = createEmptyQueueNode();
    q->endNode->val = t;
    q->endNode->nextNode = tmp;
    q->endNode = tmp;
}

void* pop( Queue* q ){
    QueueNode* tN = q->currNode;
    void* tmp = tN->val;
    q->currNode = tN->nextNode;
    free(tN);
    return tmp;
}

unsigned char dead( Queue* q){
    if( q->currNode == NULL ){
        free(q);
        return 1;
    }
    return 0;
}

unsigned char empty( Queue* q){
    return (q->currNode == q->endNode);
}

PrintPosition* createArrayPrintPosition( unsigned int len){
    unsigned int i;
    PrintPosition* tmp;
    tmp = malloc(len*sizeof(PrintPosition));
    for(i=0;i<len;++i){
        tmp[i].first = createQueue();
        tmp[i].second = createQueue();
    }
    return tmp;
}

WordGroup* createWordGroup( unsigned int len){
    WordGroup* tmp;
    tmp = malloc(sizeof(WordGroup));
    tmp->currPos = 0;
    tmp->lastPos = len;
    tmp->printPos = createArrayPrintPosition(len);
    return tmp;
}

Line* createLine(){
    Line* tmp;
    tmp = malloc(sizeof(Line));
    tmp->groups = createQueue();
    return tmp;
}

//PUBLIC DECLERATIONS

void pushEnd( Queue* q ){
    q->endNode->val = NULL;
    q->endNode->nextNode = NULL;
    q->endNode = NULL;
}

Line* pushCreateLine(unsigned int currOpTime){
    Line* tmp = createLine();
    tmp->currOpTime = currOpTime;
    push(lines,tmp);
    return tmp;
}

WordGroup* pushCreateWordGroup( Line* myLine, unsigned int len){
    WordGroup* t = createWordGroup(len);
    push(myLine->groups,t);
    return t;
}

void pushRealNode( WordGroup* myGroup, RealNode* t, unsigned int pos){
    push(myGroup->printPos[pos].first, t);
}

void pushEndRealNode( WordGroup* myGroup, unsigned int pos ){
    pushEnd(myGroup->printPos[pos].first);
}

void initializePrinter() {
    lines = createQueue();
    currRunTime = 0;
}

void superPrinter(){
    Line* currLine;
    WordGroup* currGroup;
    RealNode* currNode;
    unsigned int i, t;
    unsigned char printedBefore;
#ifndef LIN_PRT
    while( 1 ) if( ! empty (lines) ){
#endif
        currLine = pop(lines);
#ifndef LIN_PRT
        if( dead( lines ) )
            break;
#endif
        ++currRunTime;
        useOpState(currLine->currOpTime);
        printedBefore = 0;
        while ( /*1 )if(*/ ! empty ( currLine->groups ) ){
            currGroup = pop(currLine->groups);
            //if( currGroup == 49548928 )
             //   printf("\nmiyavcacaca\n");
            if( dead( currLine->groups) )
                break;
            i = 0;
            while( i < currGroup->lastPos )/*if( i < currGroup->currPos )*/{
                pushEnd( currGroup->printPos[ i ] .second );
                while ( /*1 )if (*/ ! empty( currGroup->printPos[i].second ) ){
                    currNode = pop( currGroup->printPos[i].second );
                    if ( dead( currGroup->printPos[i].second ) )
                        break;
                    if(/* currNode!=NULL &&*/ currNode->visitRun < currRunTime ){
                        currNode->visitRun = currRunTime;
                        printedBefore = printOnlyMe(printedBefore,currNode);
                        t = currNode->depth;
                        currNode = getParent(currNode);
                        t -= currNode->depth;
                        if( currNode->visitRun < currRunTime )
                            push( currGroup->printPos[i + t].second,currNode);
                    }
                }
                while ( /*1 ) if(*/ ! empty ( currGroup->printPos[i].first ) ){
                    currNode = pop( currGroup->printPos[i].first );
                    if ( dead( currGroup->printPos[i].first ) )
                        break;
                    if( /*currNode!=NULL &&*/ currNode->visitRun < currRunTime ){
                        currNode->visitRun = currRunTime;
                        printedBefore = printOnlyMe(printedBefore,currNode);
                        t = currNode->depth;
                        currNode = getParent(currNode);
                        t -= currNode->depth;
                        if( currNode->visitRun < currRunTime )
                            push( currGroup->printPos[i + t].second,currNode);
                    }
                }
                ++i;
            }
            free(currGroup->printPos);
            free(currGroup);
        }
        if(printedBefore == 0)
            printStr("-1");
        printChar('\n');
        free(currLine);
#ifndef LIN_PRT
    }
    free(lines);
#endif
}

















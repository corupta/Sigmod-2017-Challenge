/*
 * Made for ACM SIGMOD 2017 Programming Contest
 */

/* 
 * File:   Some Structs.h
 * Author: Halit Ozsoy <halitozsoy1584@gmail.com>
 *
 * Created on March 22, 2017, 7:32 AM
 */

#ifndef SOME_STRUCTS_H
#define SOME_STRUCTS_H

#ifdef __cplusplus
extern "C" {
#endif
    
    
    typedef struct {
        void* child;
    } HiddenNode;

    typedef struct {
        unsigned int id; // id & (1U<<31) = printable    id & 0x7f returns the id of the printable NGram
        unsigned int pos;
        unsigned int len;
        void* par;
    } Skip;

    typedef struct {
        Skip *skip;
        unsigned int visitRun;
        unsigned int depth;
        void* childLink; // other nodes use this linker to access this child/next node
        //void* prtLink; // linker for next printable suffix node
        //printable can be checked by if( pos == 0 && getValAtNGramInd(id,0) == currLen + len);
        void* child; //connected to linkerNodes
        void* next; //connected to realNodes via linkerNodes but linkerNode is not stored internally
    } RealNode;

    typedef struct {
        RealNode *node;
    } LinkerNode;
    
    
    typedef struct QueueNode{
        void* val;
        struct QueueNode* nextNode;
    } QueueNode;

    typedef struct {
        QueueNode* currNode; //while(currNode != endNode) t = pop(); if( t != NULL ) do stuff ; loop if( dead )return; 
        QueueNode* endNode;
    } Queue;

    typedef struct {
        Queue* first;
        Queue* second;
    } PrintPosition;

    typedef struct {
        unsigned int currPos;
        unsigned int lastPos;
        PrintPosition* printPos;
    } WordGroup;

    typedef struct {
        Queue* groups;
        unsigned int currOpTime;
    } Line;



#ifdef __cplusplus
}
#endif

#endif /* SOME_STRUCTS_H */


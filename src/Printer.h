/*
 * Made for ACM SIGMOD 2017 Programming Contest
 */

/* 
 * File:   Printer.h
 * Author: Halit Ozsoy <halitozsoy1584@gmail.com>
 *
 * Created on March 19, 2017, 4:22 AM
 */

#ifndef PRINTER_H
#define PRINTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Some Structs.h"
#include "PrefixTree.h"



    //private stuff

    static QueueNode* createEmptyQueueNode();

    static QueueNode* createQueueNode(void *);
    static Queue* createQueue();
    static void push(Queue*, void*);
    static void* pop(Queue*);
    static unsigned char dead(Queue*);
    static unsigned char empty(Queue*);
    static PrintPosition* createArrayPrintPosition(unsigned int); //len
    static WordGroup* createWordGroup(unsigned int); //len
    static Line* createLine();

    //public stuff

    extern void pushEnd(Queue*);
    extern Line* pushCreateLine(unsigned int); //currOpTime !!!
    extern WordGroup* pushCreateWordGroup(Line*, unsigned int); //len

    /*pushes to queue first only
     */
    extern void pushRealNode(WordGroup*, RealNode*, unsigned int); //pos
    extern void pushEndRealNode(WordGroup*, unsigned int); //pos
    extern void initializePrinter();
    extern void superPrinter();

    //TODO
    //mutex stuff
    //parallel improve



#ifdef __cplusplus
}
#endif

#endif /* PRINTER_H */


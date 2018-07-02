/*
 * Made for ACM SIGMOD 2017 Programming Contest
 */

/* 
 * File:   WordLibrary.h
 * Author: Halit Ozsoy <halitozsoy1584@gmail.com>
 *
 * Created on March 17, 2017, 8:19 PM
 */

#ifndef WORDLIBRARY_H
#define WORDLIBRARY_H
//#define DBG


#ifdef __cplusplus
extern "C" {
#endif

#include "Trie.h"
#include "FastPrintF.h"

    //private stuff
    
    static void doubleWordLibrarySize();
    
    /*called by print function check mutex lock for list
     */
    extern unsigned char* getWordById(unsigned int);

    //public stuff
    
    extern void initializeWordLibrary();
    
    extern unsigned int insertWord(unsigned char *);

    extern unsigned int getIdByWord(unsigned char *);
    
    extern void printWord(unsigned int );

    //debug
#ifdef DBG
    extern unsigned char* dumpWordLibrary();
#endif
    
#ifdef __cplusplus
}
#endif

#endif /* WORDLIBRARY_H */


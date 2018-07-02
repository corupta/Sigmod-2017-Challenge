/*
 * Made for ACM SIGMOD 2017 Programming Contest
 */

/* 
 * File:   GeneralFunctions.h
 * Author: Halit Ozsoy <halitozsoy1584@gmail.com>
 *
 * Created on March 18, 2017, 7:29 AM
 */

#ifndef GENERALFUNCTIONS_H
#define GENERALFUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Printer.h"
#include "PrefixTree.h"
#include "NGramLibrary.h"
#include "WordLibrary.h"

    //private
    static unsigned int* convToNGram(unsigned char*);
    static void addNewNGramInit(unsigned char *);
    static void addNewNGram(unsigned char *);
    static void deleteNewNGram(unsigned char*);
    static void queryNewLine(unsigned char*, Line*);
    //public
    extern void initializer();
    extern void lineReader();

#ifdef __cplusplus
}
#endif

#endif /* GENERALFUNCTIONS_H */


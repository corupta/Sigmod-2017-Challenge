/*
 * Made for ACM SIGMOD 2017 Programming Contest
 */

/* 
 * File:   FastPrintF.h
 * Author: Halit Ozsoy <halitozsoy1584@gmail.com>
 *
 * Created on March 19, 2017, 1:41 AM
 */

#ifndef FASTPRINTF_H
#define FASTPRINTF_H

#define LIN_PRT
#define SMALL_BUFF

#ifdef __cplusplus
extern "C" {
#endif

    //public stuff
    extern void superWriter();
    extern void initializeFastPrinter();
    extern void printStr(unsigned char *);
    extern void printChar(unsigned char);
    extern unsigned char printFinished();

#ifdef __cplusplus
}
#endif

#endif /* FASTPRINTF_H */


/*
 * Made for ACM SIGMOD 2017 Programming Contest
 */

#include "FastPrintF.h"

#ifdef LIN_PRT
//for linear programming
#include <stdio.h>
#endif


#if defined(DYN_WRT) | defined(LIN_WRT)
//for parallel programming
#include <unistd.h>
#include <string.h>
#endif

#ifdef SMALL_BUFF 
#define buffSize (1U << 24)
const unsigned int maxVal = 0xfffff; //0x7fffffff
#else
#define buffSize (1U << 31)
const unsigned int maxVal = 0x7fffffff;
#endif

static unsigned char buffer[buffSize];
static volatile unsigned int l;
static volatile unsigned int r;
static unsigned char printRunning;

void superWriter() {
#ifdef DYN_WRT
    while (1) {
#endif
#if defined(DYN_WRT) | defined(LIN_WRT)
        int currR = r;
        if (l < currR) {
            printRunning = 1;
            write(2, buffer + l, currR - l);
            l = currR;
            printRunning = 0;
        } else if (l > currR) {
            printRunning = 1;
            write(2, buffer + l, buffSize - l);
            l = 0;
            printRunning = 0;
        }
#endif
#ifdef DYN_WRT
    }
#endif
}

void initializeFastPrinter() {
    l = 0;
    r = 0;
    printRunning = 0;
    //create new thread to run superWriter constantly
}

void printStr(unsigned char* s) {
    unsigned int currL;
    //for linear programming
#ifdef LIN_PRT
    printf("%s", s);
#endif
    //for parallel programming
#if defined(DYN_WRT) | defined(LIN_WRT)
    unsigned int len = strlen(s);
#ifdef DYN_WRT
    while (currL = l, (currL <= r && currL <= ((r + len) & maxVal)) || ( currL > r && r + len >= currL ));
#endif
#ifdef LIN_WRT
    if((currL <= r && currL <= ((r + len) & maxVal)) || ( currL > r && r + len >= currL ))
        superWriter();
#endif 
            if (r + len > buffSize) {
            memcpy(buffer + r, s, buffSize - r);
                    memcpy(buffer, s + buffSize - r, len - buffSize + r);
        } else {

            memcpy(buffer, s, len);
        }
    r = (r + len) & maxVal;
#endif
}

void printChar(unsigned char c) {
#ifdef LIN_PRT

    printf("%c", c);
#endif

            //for parallel programming
#if defined(DYN_WRT) | defined(LIN_WRT)
#ifdef DYN_WRT
    while( l == (r+1)&maxVal );
#endif
#ifdef LIN_WRT
    if( l == (r+1)&maxVal)
        superWriter();
#endif
            buffer[r] = c;
            r = (r + 1) & maxVal;
#endif
}

unsigned char isPrintFinished() {
    return !(printRunning || r - l);
}


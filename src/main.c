/*
 * Made for ACM SIGMOD 2017 Programming Contest
 */

/* 
 * File:   main.c
 * Author: Halit Ozsoy <halitozsoy1584@gmail.com>
 *
 * Created on March 16, 2017, 9:31 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "GeneralFunctions.h"

/*
 * 
 */
int main(int argc, char** argv) {
    initializer();
    /*
    addNewNGram("Domates aslan kaplan kalem salyangoz");
    addNewNGram("Pattis kaplan salyangoz kertenkele");
    printf("%s",dumpNGramLibrary());
     */
    lineReader();
    return (EXIT_SUCCESS);
}


/*
 * Made for ACM SIGMOD 2017 Programming Contest
 */

/* 
 * File:   NGramLibrary.h
 * Author: Halit Ozsoy <halitozsoy1584@gmail.com>
 *
 * Created on March 18, 2017, 5:00 AM
 */

#ifndef NGRAMLIBRARY_H
#define NGRAMLIBRARY_H
//#define DBG


#ifdef __cplusplus
extern "C" {
#endif
    typedef struct {
        unsigned int * val;
        unsigned char beg;
        unsigned char end;
    } NGram;
    
    typedef struct{
        unsigned int id;
        void* next;   
    } OpState;
    
    //private stuff
    
    static void doubleNGramLibrarysize();

    //public stuff
    
    extern void initializeNGramLibrary();
    
    /* dont try to reuse ngram, and dont free it, it's memory is used in library;
     */
    extern unsigned int insertNGram(unsigned int *);

    //extern unsigned int getIdByNGram(unsigned char *);
    /*
    extern NGram* getNGramById(unsigned int) __attribute__ ((deprecated));
    
    extern NGram* getNGramById(unsigned int);   // id - avoid using !!
    */
    
    /*called by other threads. mutex-lock check for list
     */
    extern unsigned int getValAtNGramInd(unsigned int, unsigned int); // id , index
    
    extern unsigned int getValAtNGramIndUnSafeFast(unsigned int, unsigned int); // id , index
    
    /*called by print function. mutex-lock check for list
     */
    extern unsigned char getNGramBegState(unsigned int);  //id
    
    /*called by insert/delete operation. no need for mutex lock check
     */
    extern unsigned char getNGramEndState(unsigned int);  //id
    
    /*called by print function. mutex-lock check for list
     */
    extern void updNGramBegState(unsigned int);
    
    /*called by insert/delete operation. no need for mutex lock check
     */
    extern void updNGramEndState(unsigned int);
    
    extern void addOpState(unsigned int); //id
    
    extern void useOpState(unsigned int); //time
    
    //check mutex lock at each step
    extern void printNGram(unsigned int); //print NGram with the given id
    
    //extern unsigned int getNextId();
    
    //DEBUG
#ifdef DBG
    unsigned char* dumpNGramLibrary();
#endif
    



#ifdef __cplusplus
}
#endif

#endif /* NGRAMLIBRARY_H */


/*
 * Made for ACM SIGMOD 2017 Programming Contest
 */

/* 
 * File:   PrefixTree.h
 * Author: Halit Ozsoy <halitozsoy1584@gmail.com>
 *
 * Created on March 18, 2017, 9:58 PM
 */

#ifndef PREFIXTREE_H
#define PREFIXTREE_H

#ifdef __cplusplus
extern "C" {
#endif
   
//#define DBG 
#include "Some Structs.h"
#include "FastPrintF.h"
#include "Printer.h"
#include "NGramLibrary.h"


    //private stuff

    static HiddenNode* newMinimalHiddenNode();

    static HiddenNode* newHiddenNode(unsigned int); //level

    static RealNode* newRealNode();

    static LinkerNode* newLinkerNode();

    static LinkerNode* newLinkerToRealNode(RealNode*);

    static RealNode* nextLinkerNodeFromHiddenNodeLevelGround(HiddenNode*, unsigned int);

    static RealNode* childRealNode(RealNode*, unsigned int);

    static RealNode* nextRealNode(RealNode*, unsigned int);

    static RealNode* nextRealNodeUnSafeFast(RealNode*, unsigned int);

    static RealNode* getParentUnsafeFast(RealNode*);

    static RealNode* findNextUnsafeFast(RealNode*, unsigned int);

    static RealNode* findNextParUnsafeFast(RealNode*, unsigned int);

    static unsigned int isPrintableUnSafeFast(RealNode*);

    static HiddenNode* coolNextAndChildSetter(LinkerNode*, unsigned int, unsigned int);

    /*sets curr as the child of child's par in the same step using black magic
     */
    static void setChildBlackMagic(RealNode*, RealNode*);
    
    static void setChild(RealNode*, RealNode*, unsigned int);

    static void setNext(RealNode*, RealNode*, unsigned int);

    static void swapSkip(RealNode*, Skip*);

    //public stuff

    extern RealNode * getParent(RealNode*);
    
    extern unsigned int isPrintable(RealNode*);

    extern void initializePrefixTree();

    extern RealNode* findNext(RealNode*, unsigned int);

    extern unsigned int insertNGramToPrefixTree(unsigned int*);
    
    extern unsigned int getNGram(unsigned int*);
    
    extern unsigned char printOnlyMe( unsigned char, RealNode*);
    
  //  extern void searchNGrams(unsigned int*, WordGroup*);
    
#ifdef DBG
    static void dumpNode(RealNode*, unsigned char);
    static void dumpChildrenRecursive( HiddenNode*, unsigned int, unsigned char);
    extern void dumpPrefixTree(unsigned char);
#endif

#ifdef __cplusplus
}
#endif

#endif /* PREFIXTREE_H */


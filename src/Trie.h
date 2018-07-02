/*
 * Made for ACM SIGMOD 2017 Programming Contest
 */

/* 
 * File:   Trie.h
 * Author: Halit Ozsoy <halitozsoy1584@gmail.com>
 *
 * Created on March 17, 2017, 5:17 PM
 */

#ifndef TRIE_H
#define TRIE_H

#ifdef __cplusplus
extern "C" {
#endif
    typedef struct{
        void* edge;
        void* children;
        //0 goes to end point => 
    } Trie;

    typedef struct{
        Trie* par;
        unsigned char val;
    } Edge;
    
    //private stuff

    static Trie* createMiddleNode(); //children[16] = NULL

    static Trie* createEndNode(); // children = u_integer ++lastId

    static Trie* nextNodeCreateMiddle(Trie*, unsigned char); //move by 4-bit | create a middle node in null state
   
    static Trie* nextNodeCreateMiddleWithEdge(Trie*, Trie*, unsigned char); //move by 4-bit | create a middle node in null state connect to par

    static Trie* nextNodeCreateEnd(Trie*, Trie*); //move by 4-bit | create an end node with id ++lastId connect to par

    static Trie* nextNodeReturnNull(Trie*, unsigned char); //move by 4-bit | return null

    static Trie* insertIntoNode(Trie*, unsigned char*, unsigned int); // insert string to given node

    static unsigned int getFromNode(Trie*, unsigned char*, unsigned int); // find string starting at given node

    //public stuff

    extern void initializeTrie(); // set the rootNode as a MiddleNode to initialize the trie

    extern Trie* insertIntoTrie(unsigned char* str); // insert string to rootNode

    extern unsigned int getFromTrie(unsigned char* str); // find string starting at rootNode
    
    extern unsigned char* getWordFromTrie(Trie*); // get the word that ends in the given node
    
    extern unsigned int getIdFromEndNode(Trie*); // get the id of an endNode

#ifdef __cplusplus
}
#endif

#endif /* TRIE_H */


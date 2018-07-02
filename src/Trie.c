
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "Trie.h"

/*
 * Made for ACM SIGMOD 2017 Programming Contest
 */

// PRIVATE STUFF DECLERATIONS


// private variables

static unsigned int lastGivenId;

static Trie* rootNode;

// private functions

Trie* createMiddleNode() {
    Trie* myTrie;
    myTrie = malloc(sizeof (Trie));
    myTrie->children = malloc(16 * sizeof (Trie*));
    memset(myTrie->children, 0, 16 * sizeof (Trie*));
    myTrie->edge = NULL;
    return myTrie;
}

Trie* createEndNode() {
    Trie* myTrie;
    myTrie = malloc(sizeof (Trie));
    myTrie->children = malloc(sizeof ( unsigned int));
    *((unsigned int*) myTrie->children) = ++lastGivenId;
    myTrie->edge = NULL;
    return myTrie;
}

void setEdge(Trie* root, Trie* child, unsigned char c) {
    child->edge = malloc(sizeof (Edge));
    ((Edge*) (child->edge))->par = root;
    ((Edge*) (child->edge))->val = c;
}

Trie* nextNodeCreateMiddle(Trie* root, unsigned char c) {
    if (((Trie**) root-> children) [ c & 0xf ] == NULL) {
        ((Trie**) root -> children) [ c & 0xf ] = createMiddleNode();
    }
    return ((Trie**) root -> children) [ c & 0xf ];
}

Trie* nextNodeCreateMiddleWithEdge(Trie* topRoot, Trie* root, unsigned char c) {
    if (((Trie**) root-> children) [ c & 0xf ] == NULL) {
        ((Trie**) root -> children) [ c & 0xf ] = createMiddleNode();
        setEdge( topRoot, ((Trie**)root->children)[c&0xf], c<<(1<<2) | c>>(1<<2));
    }
    return ((Trie**) root -> children) [ c & 0xf ];
}

Trie* nextNodeCreateEnd(Trie* topRoot, Trie* root) {
    if (((Trie**) root-> children) [ 0 ] == NULL) {
        ((Trie**) root->children) [ 0 ] = createEndNode();
        setEdge(topRoot, ((Trie**) root->children) [ 0 ], (unsigned char) 0);
    }
    //printf("New EndNode! %u\n", (unsigned int)((((Trie**) root->children) [0])->children) );
    return ((Trie**) root->children) [0];
}

Trie* nextNodeReturnNull(Trie* root, unsigned char c) {
    return ((Trie**) root->children) [ c & 0xf ];
}

Trie* insertIntoNode(Trie* root, unsigned char* str, unsigned int len) {
    // root is a middle node !!!
    Trie* mid;
    while (len-- > 0) {
        mid = nextNodeCreateMiddle(root, *str);
        (*str) = ((*str) >> (1 << 2)) | ((*str) << (1 << 2));
        root = nextNodeCreateMiddleWithEdge(root, mid, *str);
        ++str;
    }
    mid = nextNodeCreateMiddle(root, *str);
    (*str) = ((*str) >> (1 << 2)) | ((*str) << (1 << 2));
    root = nextNodeCreateEnd(root, mid);
    return root;
}

unsigned int getFromNode(Trie* root, unsigned char* str, unsigned int len) {
    // root is a middle node !!!
    while (len-- > 0) {
        root = nextNodeReturnNull(root, *str);
        if (root == NULL)
            return 0;
        (*str) = ((*str) >> (1 << 2)) | ((*str) << (1 << 2));
        root = nextNodeReturnNull(root, *str);
        if (root == NULL)
            return 0;
        ++str;
    }
    root = nextNodeReturnNull(root, *str);
    if (root == NULL)
        return 0;
    (*str) = ((*str) >> (1 << 2)) | ((*str) << (1 << 2));
    root = nextNodeReturnNull(root, *str);
    if (root == NULL)
        return 0;
    return getIdFromEndNode( root );
}

// PUBLIC STUFF DECLERATIONS

void initializeTrie() {
    rootNode = createMiddleNode();
}

Trie* insertIntoTrie(unsigned char* str) {
    unsigned int len = strlen(str);
    return insertIntoNode(rootNode, str, len);
}

unsigned int getFromTrie(unsigned char* str) {
    unsigned int len = strlen(str);
    return getFromNode(rootNode, str, len);
}

unsigned char * getWordFromTrie( Trie * endNode){
    unsigned char * ans;
    unsigned int strLen = 0;
    Trie * tmp = endNode;
    while( tmp -> edge != NULL){
        ++strLen;
        tmp = ((Edge*)(tmp->edge))->par;
        //printf("%c\n",((Edge*)(tmp->edge))->val);
    }
   //printf("%u =< %u\n",getIdFromEndNode(endNode),strLen);
    ans = malloc(strLen * sizeof(unsigned char) );
    while( strLen-- ){
        ans[strLen] = ((Edge*)(endNode->edge))->val;
        endNode = ((Edge*)(endNode->edge))->par;
    }
    return ans;
}

unsigned int getIdFromEndNode( Trie * t){
   return *((unsigned int*)(t->children));
}
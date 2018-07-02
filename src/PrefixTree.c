/*
 * Made for ACM SIGMOD 2017 Programming Contest
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "PrefixTree.h"

#define JUMP_SIZE 32
#define JUMPER 0x1f
#define JUMP_NUM 6
#define JUMPER_BITSIZE 5
//max = 64^5 = 2^30
//max = 32^6 = 2^30

//PRIVATE DECLERATIONS

//private variables

static HiddenNode* myHiddenNull[JUMP_NUM - 1];
static LinkerNode* myLinkerNull;
static RealNode* myRealNull;
static RealNode* root;

//private functions

HiddenNode* newMinimalHiddenNode() {
    HiddenNode* tmp = malloc(sizeof (HiddenNode));
    tmp -> child = NULL;
    return tmp;
}

HiddenNode* newHiddenNode(unsigned int level) {
    unsigned int i;
    HiddenNode* tmp = malloc(sizeof (HiddenNode));
    if (level < JUMP_NUM - 2) {
        tmp->child = malloc(JUMP_SIZE * sizeof (HiddenNode*));
        for (i = 0; i < JUMP_SIZE; ++i)
            ((HiddenNode**) (tmp->child))[i] = myHiddenNull[level + 1];
    } else {
        tmp->child = malloc(JUMP_SIZE * sizeof (LinkerNode*));
        for (i = 0; i < JUMP_SIZE; ++i)
            ((LinkerNode**) (tmp->child))[i] = myLinkerNull;
    }
    return tmp;
}

RealNode* newRealNode() {
    unsigned int i;
    RealNode* tmp = malloc(sizeof (RealNode));
    tmp->child = malloc(JUMP_SIZE * sizeof (HiddenNode*));
    tmp->next = malloc(JUMP_SIZE * sizeof (HiddenNode*));
    tmp->skip = malloc(sizeof (Skip));
    tmp->skip->len = 0;
    tmp->skip->id = 0;
    tmp->skip->par = myRealNull;
    tmp->visitRun = 0;
    tmp->depth = 0;
    for (i = 0; i < JUMP_SIZE; ++i) {
        ((HiddenNode**) tmp->child)[i] = myHiddenNull[0];
        ((HiddenNode**) tmp->next)[i] = myHiddenNull[0];
    }
    tmp->childLink = malloc(sizeof (LinkerNode));
    ((LinkerNode*) (tmp->childLink))->node = tmp;
    return tmp;
}

/*returns a new null linker
 * maybe use newLinkerToRealNode( RealNode* )
 */
LinkerNode* newLinkerNode() {
    LinkerNode* tmp = malloc(sizeof (LinkerNode));
    tmp->node = myRealNull;
    return tmp;
}

LinkerNode* newLinkerToRealNode(RealNode* dest) {
    LinkerNode* tmp = malloc(sizeof (LinkerNode));
    tmp->node = dest;
    return tmp;
}
/*  real1
 *  hid0    1j      tmp tmp2
 *  hid1    2j      tmp
 *  hid2    3j
 *  hidk    k+1j    tmp
 *  link    k+2j
 *  real2   k+2j + 1lj
 * jump num = k + 2
 
 */
LinkerNode* nextLinkerNodeFromHiddenNodeGroundLevel(HiddenNode* tmp, unsigned int c) {
    //for jump num-2 (5-2 = 3)
    int i;
    for (i = 0; i < JUMP_NUM - 2; ++i) {
        tmp = ((HiddenNode**) tmp->child)[c & JUMPER];
        c >>= JUMPER_BITSIZE;
    }
    return ((LinkerNode**) tmp->child)[c & JUMPER];
}

RealNode* childRealNode(RealNode* curr, unsigned int c) {
    HiddenNode* tmp = ((HiddenNode**) curr->child)[ c & JUMPER];
    c >>= JUMPER_BITSIZE;
    return nextLinkerNodeFromHiddenNodeGroundLevel(tmp, c)->node;
}

RealNode* nextRealNode(RealNode* curr, unsigned int c) {
    HiddenNode* tmp;
    //mutex check
    tmp = ((HiddenNode**) curr->next)[ c & JUMPER];
    //done
    c >>= JUMPER_BITSIZE;
    return nextLinkerNodeFromHiddenNodeGroundLevel(tmp, c)->node;
}

RealNode* nextRealNodeUnSafeFast(RealNode* curr, unsigned int c) {
    HiddenNode* tmp = ((HiddenNode**) curr->next)[ c & JUMPER];
    c >>= JUMPER_BITSIZE;
    return nextLinkerNodeFromHiddenNodeGroundLevel(tmp, c)->node;
}

RealNode* getParentUnsafeFast(RealNode* curr) {
    return curr->skip->par;
}

RealNode* findNextUnsafeFast(RealNode* curr, unsigned int step) {
    curr = getParentUnsafeFast(curr);
    RealNode* tmp = nextRealNodeUnSafeFast(curr, step);
    while (tmp == myRealNull && curr != root) {
        curr = getParentUnsafeFast(curr);
        tmp = nextRealNodeUnSafeFast(curr, step);
    }
    if(tmp == myRealNull)
        return root;
    return tmp;
}

RealNode* findNextParUnsafeFast(RealNode* curr, unsigned int step) {
    RealNode* tmp = nextRealNodeUnSafeFast(curr, step);
    while (tmp == myRealNull && curr != root) {
        curr = getParentUnsafeFast(curr);
        tmp = nextRealNodeUnSafeFast(curr, step);
    }
    return curr;
}

unsigned int isPrintableUnSafeFast(RealNode* curr) {
    if (curr->depth == getValAtNGramIndUnSafeFast(curr->skip->id, 0))
        return curr->skip->id;
    return 0;
}

HiddenNode* coolNextAndChildSetter(LinkerNode* nextOrChild, unsigned int step, unsigned int level) {
    step>>=JUMPER_BITSIZE;
    HiddenNode*tmp, *tmp2;
    tmp = newHiddenNode(level);
    tmp2 = tmp;
    while (++level < JUMP_NUM - 1) {
        ((HiddenNode**) (tmp->child))[step & JUMPER ] = newHiddenNode(level);
        tmp = ((HiddenNode**) (tmp->child))[step & JUMPER ];
        step >>= JUMPER_BITSIZE;
    }
    ((LinkerNode**) (tmp->child))[ step & JUMPER ] = nextOrChild;
    return tmp2;
}

void setChildBlackMagic(RealNode*curr, RealNode*child){
    ((LinkerNode*)child->childLink)->node = curr;
    ((LinkerNode*)curr->childLink)->node = child;
    //black magic to set the curr as child of child's parent
    LinkerNode * tmp;
    tmp = child->childLink;
    child->childLink = curr->childLink;
    curr->childLink = tmp;
}

/*  CALLED WHEN NULLPTR
 */
void setChild(RealNode*curr, RealNode*child, unsigned int step) {
    //no need for mutex check/lock etc
    HiddenNode *tmp, *tmp2;
    LinkerNode* fin;
    unsigned int level = 0;
    
    fin = child->childLink;

    tmp = ((HiddenNode**) (curr->child)) [ step & JUMPER ];

    while (tmp != myHiddenNull[ level ] && ++level<JUMP_NUM - 1) {
        step >>= JUMPER_BITSIZE;
        tmp2 = tmp;
        tmp = ((HiddenNode**) (tmp->child)) [ step & JUMPER ];
    }
    if (level == JUMP_NUM - 1) {
        step >>= JUMPER_BITSIZE;
        ((LinkerNode**) (tmp->child)) [ step & JUMPER ] = fin;
    } else {
        tmp = coolNextAndChildSetter(fin, step, level);
        if (level == 0) {
            ((HiddenNode**) (curr->child))[ step & JUMPER ] = tmp;
        } else {
            ((HiddenNode**) (tmp2->child)) [ step & JUMPER ] = tmp;

        }
    }
}

/*CALLED WHEN NULLPTR
 * IMPORTANT MUTEXT LOCK / UNLOCK
 */
void setNext(RealNode*curr, RealNode*next, unsigned int step) {
    HiddenNode *tmp, *tmp2;
    LinkerNode* fin;
    unsigned int level = 0;

    tmp = ((HiddenNode**) (curr->next)) [ step & JUMPER ];
    fin = newLinkerToRealNode(next);

    while (tmp != myHiddenNull[ level ] && ++level<JUMP_NUM - 1) {
        step >>= JUMPER_BITSIZE;
        tmp2 = tmp;
        tmp = ((HiddenNode**) (tmp->child)) [ step & JUMPER ];
    }
    if (level == JUMP_NUM - 1) {
        step >>= JUMPER_BITSIZE;
        //MUTEX LOCK curr->next
        ((LinkerNode**) (tmp->child)) [ step & JUMPER ] = fin;
        //MUTEX UNLOCK
    } else {
        tmp = coolNextAndChildSetter(fin, step, level);
        if (level == 0) {
            //MUTEX LOCK curr->next
            ((HiddenNode**) (curr->next))[ step & JUMPER ] = tmp;
            //MUTEX UNLOCK
        } else {
            //MUTEX LOCK curr->next
            ((HiddenNode**) (tmp2->child)) [ step & JUMPER ] = tmp;
            //MUTEX UNLOCK

        }
    }
}

void swapSkip(RealNode*curr, Skip* tSkip) {
    Skip *bkp;
    bkp = curr->skip;
    //MUTEX LOCK curr->skip
    curr->skip = tSkip;
    //MUTEX UNLOCK
    free(bkp);
}

//PUBLIC DECLERATIONS


RealNode * getParent(RealNode* curr) {
    RealNode* par;
    //check for mutex lock
    par = curr->skip->par;
    //done;
    return par;
}

unsigned int isPrintable(RealNode* curr) {
    unsigned int id, depth;
    //check for mutex lock
    id = curr->skip->id;
    /*
    if(id == 1696){
        printf("\nkediiiacaca %d %d\n",getValAtNGramInd(id, 0),curr->depth);
    }*/
    //done
    depth = getValAtNGramInd(id, 0);
    if (depth == curr->depth)
        return id;
    return 0;
}

void initializePrefixTree() {
    unsigned int i, j;
    for (i = 0; i < JUMP_NUM - 1; ++i) {
        myHiddenNull[i] = newMinimalHiddenNode();
    }
    for (i = 0; i < JUMP_NUM - 2; ++i) {
        myHiddenNull[i]->child = malloc(JUMP_SIZE * sizeof (HiddenNode*));
    }
    myHiddenNull[JUMP_NUM - 2]->child = malloc(JUMP_SIZE * sizeof (LinkerNode*));
    myRealNull = newRealNode();
    myRealNull->skip->par = myRealNull;
    myLinkerNull = newLinkerNode();
    for (i = 0; i < JUMP_SIZE; ++i) {
        for (j = 1; j < JUMP_NUM - 1; ++j)
            ((HiddenNode**) myHiddenNull[j - 1]->child)[i] = myHiddenNull[j];
        ((LinkerNode**) myHiddenNull[JUMP_NUM - 2]->child)[i] = myLinkerNull;
    }
    root = newRealNode();
    root->skip->par = root;
    root->visitRun = 0xffffffff;
}

RealNode* findNext(RealNode* curr, unsigned int step) {
    RealNode* tmp = nextRealNode(curr, step);
    while (tmp == myRealNull && curr != root) {
        curr = getParent(curr);
        tmp = nextRealNode(curr, step);
    }
    return tmp;
}

unsigned int insertNGramToPrefixTree(unsigned int* ngram) {
    //OMG THE FUCNTION FINISHED
    unsigned int i, t;
    unsigned int helpLen = 0; //extra len of helperLen
    unsigned int currLen = 0; //extra len of currLen
    unsigned int myId = 0;
    Skip* tSkip;
    RealNode* helper, *helperMid, *curr, *tmp, *tmp2, *tmp3, *tmp4;
   /* if( ngram[0] == 0 )
        exit(EXIT_FAILURE);*/
        
    helper = nextRealNodeUnSafeFast(root, ngram[1]);
    helperMid = myRealNull;
    curr = root;

    unsigned int pos = 0;
    unsigned int len = ngram[0];
    while (++pos <= len) {

        if (helper == myRealNull) {
            //find new helper{
            tmp = findNextUnsafeFast(curr, ngram[pos]);
            helper = childRealNode(tmp, ngram[pos - tmp->depth]);
            if (helper == myRealNull) {
                //no helper found
                if (myId == 0)
                    myId = insertNGram(ngram);
                tmp2 = newRealNode();
                tmp2->depth = pos;
                tmp2->skip->id = myId;
                tmp2->skip->pos = tmp2->skip->len = pos - tmp->depth;
                tmp2->skip->par = tmp;
                setChild(tmp, tmp2, ngram[tmp2->skip->pos]);
                setNext(curr, tmp2, ngram[pos]);
                curr = tmp2;
                helperMid = myRealNull;
                continue;
            } else {
                //helper found -> set helper
                t = helper->skip->len;
                if (pos - tmp->depth < t)
                    t = pos - tmp->depth;
                i = 0;
                while (++i < t && getValAtNGramIndUnSafeFast(helper->skip->id, helper->skip->pos - i) == ngram[ pos - tmp->depth - i]);
                //dont mistakenly delete the above semicolon !!!
                //for(i = 1;i<t;++i)if( getValAtNGramIndUnSafeFast(helper->skip->id,helper->skip->pos - i) != ngram[ pos - tmp->depth - i] )break;
                helpLen = helper->skip->len - i;
                if (currLen != pos - tmp->depth - i)
                    helperMid = myRealNull;
                currLen = pos - tmp->depth - i;
            }
        }
        //there is a helper
        if (helpLen == 0 && currLen == 0) {
            //changed here
            /*
            if( findNextUnsafeFast(curr,ngram[pos]) != helper )
                setNext(curr,helper,ngram[pos]);
            */
            //change end
            curr = helper;
           /* if(ngram[pos] == 136 || ngram[pos] == 22){
                printf("(%u,%u) - %u %u %u %u\n",ngram[pos-1],ngram[pos],curr, pos, len ,isPrintableUnSafeFast(curr));
            }*/
            if (pos == len && isPrintableUnSafeFast(curr) == 0) {
                tSkip = malloc(sizeof (Skip));
                if (myId == 0)
                    myId = insertNGram(ngram);
                tSkip->id = myId;
                tSkip->len = curr->skip->len;
                tSkip->pos = curr->skip->len;
                tSkip->par = curr->skip->par;
                swapSkip(curr, tSkip);
            }
        } else {
            //continue with helper
            if (myId == 0)
                myId = insertNGram(ngram);
            if (helpLen > 0) {
                //up bot
                tmp4 = newRealNode();
                tmp4->depth = helper->depth - helpLen;
                tmp4->skip->id = myId;
                tmp4->skip->len = helper->skip->len - helpLen;
                tmp4->skip->pos = currLen + tmp4->skip->len;
                tmp4->skip->par = helper->skip->par;
                setChildBlackMagic( tmp4, helper);
                //left bot
                tSkip = malloc(sizeof (Skip));
                tSkip->id = helper->skip->id;
                tSkip->len = helpLen;
                tSkip->pos = helper->skip->pos - tmp4->skip->len;
                tSkip->par = tmp4;
                setChild(tmp4, helper, getValAtNGramIndUnSafeFast(tSkip->id, tSkip->pos));
                swapSkip(helper, tSkip);
            }
            if (currLen > 0) {
                //right bot
                tmp3 = newRealNode();
                tmp3->depth = helper->depth - helpLen + currLen;
                tmp3->skip->id = myId;
                tmp3->skip->len = currLen;
                tmp3->skip->pos = currLen;
                if (helpLen > 0) {
                    tmp3->skip->par = tmp4;
                    setChild(tmp4, tmp3, ngram[currLen]);
                    if(helperMid == myRealNull){
                        tmp = curr;
                        while( tmp->depth + 1 > tmp4->depth )
                            tmp = getParentUnsafeFast(tmp);
                        if(tmp->depth + 1 == tmp4->depth)
                            helperMid = tmp;
                    }
                } else {
                    tmp3->skip->par = helper;
                    setChild(helper, tmp3, ngram[currLen]);
                }
                setNext(curr,tmp3,ngram[pos]);
                curr = tmp3;
            }else{
                setNext(curr,tmp4,ngram[pos]);
                curr = tmp4;
            }
            if (helperMid != myRealNull)
                setNext(helperMid, tmp4, ngram[pos]);
            if (currLen > 0 && helpLen > 0) {
                helperMid = tmp4;
            } else {
                helperMid = myRealNull;
            }
        }
        tmp = nextRealNodeUnSafeFast(helper, ngram[pos+1]);
        if (tmp == myRealNull && helpLen == 0)
            helperMid = helper;
        helper = tmp;
    }
    if(myId == 0)
        free(ngram);
    return curr->skip->id;
}

unsigned int getNGram(unsigned int* ngram) {
    RealNode* curr;
    curr = root;
    int i, n = ngram[0];
    for (i = 1; i <= n; ++i)
        curr = nextRealNodeUnSafeFast(curr, ngram[i]);
    return curr->skip->id;
}

unsigned char printOnlyMe( unsigned char printSeparator, RealNode* curr){
    unsigned int id;
    id = isPrintable(curr);
    //if(id== 1696){
     //   printf("\nkediii! %u\n",getNGramBegState(id));
    //}
    if(id && getNGramBegState(id)){
        if(printSeparator)
            printChar('|');
        printNGram( id );
        return 1;
    }
    return printSeparator;
}

/*
unsigned char printMe(unsigned char printSeperatorAtBeginning, RealNode* curr, unsigned int currRun) {
    unsigned int printId, arrSize = 127, len = 0;
    unsigned int* tmpNGramList;
    tmpNGramList = malloc(arrSize);
    while (curr!=root&&curr->visitRun <= currRun) {
        curr->visitRun = currRun + 1;
        printId = isPrintable(curr);
        if (printId) {
            if (len + 1 == arrSize) {
                arrSize = (arrSize << 1) + 1;
                tmpNGramList = realloc(tmpNGramList, arrSize * sizeof(unsigned int) );
            }
            tmpNGramList [ len ] = printId;
            ++len;
        }
        curr = getParent(curr);
    }
    if(len){
        if (printSeperatorAtBeginning)
            printChar('|');
        while (--len) {
            printNGram(tmpNGramList[len]);
            printChar('|');
        }
        printNGram(tmpNGramList[0]);
        return 1;
    }
    return 0;
}*/

void searchNGrams(unsigned int* input, WordGroup* myGroup) {
    //new thread - mutexes everywhere
   // unsigned int kedi = 0;
    unsigned int i, n = input[0], id, j=0;
    RealNode* curr = root;
    for (i = 1; i <= n; ++i) {
     /*   if( i>1 && input[i] == 136 && input[i-1]==136){
            kedi = 1;
            printf("\ndaha da miyav\n");
            for( j=1; j<=n;++j){
                printf("%u\n",input[j]);
            }
        }*/
        curr = findNext(curr, input[i]);/*
        if(kedi){
            printf("%u   %u   %u   %u\n", curr, root, curr->depth, myRealNull);
        }*/
        while(j< i - curr->depth)
            pushEndRealNode(myGroup,j++);
        if (curr == myRealNull){
            curr = root;
        }else {
            pushRealNode(myGroup,curr,i-curr->depth);
        }
    }
    while(j<n)
        pushEndRealNode(myGroup,j++);
    free(input);
}

//huge debug
#ifdef DBG
void dumpNode(RealNode*, unsigned char);
void dumpChildrenRecursive( HiddenNode* curr, unsigned int level, unsigned char k){
    //printf("InvestigatingHiddenNode: %u\n",curr);
    unsigned int i;
    //error is right fukin here mate
    if(level<JUMP_NUM -1){
        for(i=0;i<JUMP_SIZE;++i)
            if(((HiddenNode**)(curr->child)) [ i ] != myHiddenNull[level] )
                dumpChildrenRecursive( ((HiddenNode**)curr->child) [ i ] , level+1, k);
    }else{
        for(i=0;i<JUMP_SIZE;++i)
            if(((LinkerNode**)curr->child)[ i] != myLinkerNull )
                dumpNode( ((LinkerNode**)curr->child)[ i] ->node , k);
    }
}
void dumpNode( RealNode* curr, unsigned char k){
    if(curr == myRealNull)
        return;
    unsigned int i;
//    printf("InvestigatingRealNode: %x\n",curr);
    for( i = 0 ; i < JUMP_SIZE; ++i)
        if(((HiddenNode**)(k?curr->next:curr->child))[i] != myHiddenNull[0])
            dumpChildrenRecursive(((HiddenNode**)(k?curr->next:curr->child))[i], 1, (k?k+1:k));
    i = 1+curr->depth - curr->skip->len + 1;
    if(curr!=root){
//        printf("p\n");
        if(k){
            i = k;
            while(--i>0)printf("     ");
            i = curr->skip->len + 1; 
           // printf("InvestigatingRealNode: %u\n",i);
            printf("~%.4u",getValAtNGramIndUnSafeFast(curr->skip->id, curr->depth ));
           printf("\n");
        }else{
            while(--i>0)printf("     ");
           // printf("InvestigatingRealNode: %u\n",i);
            while(++i <= curr->skip->len)printf("~%.4u",getValAtNGramIndUnSafeFast(curr->skip->id,curr->skip->pos +1 - i));
           printf("\n");
        }
    }
}
void dumpPrefixTree(unsigned char k){
    printf("dumping prefix tree\n");
    dumpNode(root,k);
    printf("~%.4u\n",0U);
}
#endif
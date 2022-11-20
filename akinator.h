#ifndef AKINATOR_H_INCLUDED
#define AKINATOR_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "dump.h"
#include "log.h"
#include "html.h"


#define checkNode(node)                                                 \
    if (!node)  {                                                       \
        SpeakAndPrint("There is no such object in current dataBase\n"); \
        clearBuff();                                                    \
        continue;                                                       \
    }

#define GET_OBJ(name, node)                                         \
    name = (char*)calloc(MAX_NAME_LEN, sizeof(char));               \
    ASSERT(name != nullptr);                                        \
    scanf("%s", name);                                              \
    NodeByName(name, root, &node);                                  \
    free(name);                                                     \
    checkNode(node);

enum UserInput {
    GUESS = 'g',
    OTHERNESS = 'o',
    DEFINE = 'd',
    EXIT = '#',
    YES = 'y',
    NO = 'n',
};

const int LEFT  = 1;
const int RIGHT = 0;

struct node {
    char*         data;
    struct node*  parent;
    struct node*  left;
    struct node*  right;
};

struct Tree {
    struct node*  root;
};

static int imageNum = 1;
const char png[5] = ".png";

const size_t MAX_NAME_LEN = 50;
const size_t MAX_CMD_LEN = 100;
void clearBuff();

struct node* CreateLeftNode(struct node* node, const char* str);
struct node* CreateRightNode(struct node* node, const char* str);

void NodeByName(const char* str, struct node* node, struct node** result);
void checkCondition(struct node*);

void findOtherness(struct node* node1, struct node* node2);
void PrintParents(struct node* node, int isTrue);
void PrintNode(struct node* node);

void Play(struct node* root);
void TreeCtor(struct Tree* tree, char* rootData);
void TreeDtor(struct Tree* tree);
void NodesDtor(struct node* node);
void GraphTreeDump(struct node* root);
void DrawTree(struct node* node, const char* branchName, FILE* DumpFile);
void setHTML(const char* image);

void ScanDataBase(struct node* root);
void scanTree(struct node* node, FILE* dataBase);

void WriteDataBase(struct node* root);
void writeTree(struct node* node, FILE* dataBase, int spaceCt);
void printSpaces(int spaceCt, FILE* dataBase);

void SpeakAndPrint(const char* str...);



#endif // AKINATOR_H_INCLUDED

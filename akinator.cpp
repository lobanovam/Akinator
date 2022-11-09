#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "dump.h"
#include "log.h"
#include "html.h"

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

const size_t MAX_NAME_LEN = 30;
void clearBuff();
void PrintNode(struct node* node);
struct node* CreateLeftNode(struct node* node, char* str);
struct node* CreateRightNode(struct node* node, char* str);
void GetDef(const char* str, struct node* node, int* found);
void checkCondition(struct node*);
void PrintParents(struct node* node, int isTrue);
void createTree();
void Play(struct node* root);
void TreeCtor(struct Tree* tree, char* rootData);
void GraphTreeDump(struct node* root);
void DrawTree(struct node* node, const char* branchName, FILE* DumpFile);
void setHTML(char* image);

int main() {
    struct Tree tree = {nullptr};
    TreeCtor(&tree, "is animal");
    //struct node node1 = {"is animal", nullptr, nullptr};
    struct node* node2 = CreateLeftNode(tree.root, "cat");
    struct node* node3 = CreateRightNode(tree.root, "sings");
    struct node* node4 = CreateLeftNode(node3, "alla");
    struct node* node5 = CreateRightNode(node3, "ded");

    Play(tree.root);

    GraphTreeDump(tree.root);

    printf("out of checkCondition\n");
    PrintNode(tree.root);

return 0;
}

void GraphTreeDump(struct node* root) {
    FILE* DumpFile = openDump();

    dumpPrint("digraph {\n");
    dumpPrint("graph [dpi = 100]");
    dumpPrint("rankdir = TB;\n");
    dumpPrint("node [ shape=record ];\n");

    DrawTree(root, "root", DumpFile);

    dumpPrint("}");

    fclose(DumpFile);

    char cmd[100] = "dot -T png dump.dot -o ";
    char name[30] = "graphCode";
    char num[20] = " ";

    itoa(imageNum, num, 10);

    strcat(num, png);
    strcat(name, num);
    strcat(cmd, name);

    printf("%d\n", system("cd C:\\Users\\alexm\\Documents\\Akinator"));
    printf("%d\n", system(cmd));

    printf("RESULT NAME IS %s\n", name);

    setHTML(name);

    imageNum++;
}

void DrawTree(struct node* node, const char* branchName, FILE* DumpFile) {
    dumpPrint("struct%p [\nlabel = \"{<data>data: %s|<parent>parent: %p|<left>left: %p|<right>right: %p}\", style = \"filled\", color = \"black\", fillcolor = \"aquamarine\" \n];\n", node, node->data, node->parent, node->left, node->right);
    if (node->parent != NULL)
    {
        dumpPrint("struct%p -> struct%p [xlabel = \"%s\" weight=900 constraint=true color=red];\n", node->parent, node, branchName);
    }
    if (node->left != NULL)
    {
        DrawTree(node->left, "yes", DumpFile);
    }
    if (node->right != NULL)
    {
        DrawTree(node->right, "no", DumpFile);
    }
}

void setHTML(char* image) {
    printHTML("<img src = \"%s\">", image);
    printHTML("<hr>");
}


void TreeCtor(struct Tree* tree, char* rootData) {
    struct node* node1 = (struct node*)(calloc(1, sizeof(struct node)));
    node1->data = rootData;
    node1->left = nullptr;
    node1->right = nullptr;
    node1->parent = nullptr;
    tree->root = node1;
}

void Play(struct node* root) {
    printf("welcome to the program\n");
    char ch = ' ';
    while (ch != '#') {
        printf("enter '#' to stop, 'd' - to definition, 'g' - to guess, 'o' - to otherness \n");
        ch = getchar();
        clearBuff();
        if (ch == 'g'){
            printf("please answer y (yes) or n (no):\n");
            checkCondition(root);
            clearBuff();
        } else if (ch == 'd') {
            printf("enter a name of object to define\n");
            char* name = (char*)calloc(MAX_NAME_LEN, sizeof(char));
            scanf("%s", name);
            int found = 0;
            GetDef(name, root, &found);
            if (!found) printf("There is no such object in current tree\n");
            clearBuff();
        } else if (ch == 'o') {
            printf("otherness\n");
            clearBuff();
        }
    }
}

void PrintNode(struct node* node) {
    if (node == nullptr) return;

    printf(" { %s ", node->data);
    if (node->left) {
        PrintNode(node->left);
    }
    if (node->right) {
        PrintNode(node->right);
    }
    printf(" } ");
}

struct node* CreateLeftNode(struct node* node, char* str) {
    struct node* tmp_node = (struct node*)(calloc(1, sizeof(struct node)));
    if (node->left == nullptr) {
        node->left = tmp_node;
        tmp_node->parent = node;
        tmp_node->data = str;
    } else {
        printf("can't create left node, there is one already\n");
        return nullptr;
    }
    return tmp_node;
};

struct node* CreateRightNode(struct node* node, char* str) {
    struct node* tmp_node = (struct node*)(calloc(1, sizeof(struct node)));
    if (node->right == nullptr) {
        node->right = tmp_node;
        tmp_node->parent = node;
        tmp_node->data = str;
    } else {
        printf("can't create right node, there is one already\n");
        return nullptr;
    }
    return tmp_node;
};

void checkCondition(struct node* node){
    if (!(node->left) || !(node->right)){
        printf("Let me guess.. it is %s? y or n\n", node->data);
        char ch = ' ';
        if ((ch = getchar()) == 'y'){
            printf("it was too easy!\n");
            //clearBuff();
        } else if (ch == 'n') {
            printf("hmmm, then who was it?\n");
            char* name = (char*)calloc(MAX_NAME_LEN, sizeof(char));
            scanf("%s", name);
            printf("and what is %s's difference from %s?\n", name, node->data);

            char* diff = (char*)calloc(MAX_NAME_LEN, sizeof(char));
            scanf("%s", diff);
            printf("ok, now i know!\n");

            CreateLeftNode(node, name);
            CreateRightNode(node, node->data);

            node->data = diff;
            //clearBuff();
        }
    } else {
        printf("%s?\n", node->data);
        char ch = ' ';
        if ((ch = getchar()) == 'n') {
            clearBuff();
            checkCondition(node->right);
        } else if (ch == 'y') {
            clearBuff();
            checkCondition(node->left);
        } else {
            printf("no such answer\n");
            clearBuff();
            checkCondition(node);
        }
    }
}

void GetDef(const char* str, struct node* node, int* found) {
    if (stricmp(node->data, str) == 0) {
        *found = 1;
        printf("the definition of %s: ", node->data);
        PrintParents(node, 0);
        printf("\n");
    }
    if (node->left) {
        GetDef(str, node->left, found);
    }
    if (node->right) {
        GetDef(str, node->right, found);
    }
};

void PrintParents(struct node* node, int isTrue) {
    if (node->parent){
        if (node->parent->left == node) PrintParents(node->parent, 1);
        else PrintParents(node->parent, 0);
    }
    if (node->left == nullptr && node->right == nullptr) return;
    if (!isTrue) printf("not ");
    printf("%s, ", node->data);
}

void clearBuff() {
    char ch = 0;
    while((ch = getchar()) != '\n')
        continue;
}


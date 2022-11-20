#include "akinator.h"


int main() {

    struct Tree tree = {nullptr};
    TreeCtor(&tree, " ");

    ScanDataBase(tree.root);

    GraphTreeDump(tree.root);

    Play(tree.root);

    GraphTreeDump(tree.root);

    WriteDataBase(tree.root);

    PrintNode(tree.root);

    TreeDtor(&tree);

return 0;
}

void SpeakAndPrint(const char* str...) {
    ASSERT(str != nullptr);

    va_list args = {0};
    va_start(args, str);

    char message[MAX_NAME_LEN] = "";
    vsprintf(message, str, args);

    printf("%s\n", message);

    char cmd[MAX_CMD_LEN] = "";
    sprintf(cmd, ".\\eSpeak\\command_line\\espeak.exe \"%s\"", message);
    system(cmd);
}

void NodesDtor(struct node* node) {
    ASSERT(node != nullptr);

    if (node->left) {
        NodesDtor(node->left);
        NodesDtor(node->right);
    }
    free(node->data);
    node->data = nullptr;
    node->left = nullptr;
    node->right = nullptr;
    node->parent = nullptr;
}

void TreeDtor(struct Tree* tree) {
    ASSERT(tree != nullptr);

    NodesDtor(tree->root);
    tree->root = nullptr;
}

void printSpaces(int spaceCt, FILE* dataBase) {
    ASSERT(dataBase != nullptr);

    while (spaceCt-- > 0) {
        fprintf(dataBase, " ");
    }
}

void writeTree(struct node* node, FILE* dataBase, int spaceCt) {
    ASSERT(node != nullptr);
    ASSERT(dataBase != nullptr);

    if (node->left){

        printSpaces(spaceCt, dataBase);

        fprintf(dataBase, "?%s\n", node->data);

        writeTree(node->left, dataBase, spaceCt+2);
        writeTree(node->right, dataBase, spaceCt+2);

        spaceCt+=2;
    }
    if (node->left == nullptr && node->right == nullptr) {
        printSpaces(spaceCt, dataBase);
        fprintf(dataBase, "*%s\n", node->data);
    }
}

void WriteDataBase(struct node* root) {
    logprint("trying to write dataBase\n");

    ASSERT(root != nullptr);

    FILE* dataBase = nullptr;
    if ((dataBase = fopen("dataBase.txt", "w")) == NULL) {
        printf("can't open dataBase file\n");
    }
    writeTree(root, dataBase, 0);
    fclose(dataBase);

    logprint("done writing dataBase\n");
}

void ScanDataBase(struct node* root) {
    logprint("trying to scan dataBase\n");

    ASSERT(root != nullptr);

    FILE* dataBase = nullptr;
    if ((dataBase = fopen("dataBase.txt", "r")) == NULL) {
        printf("can't open dataBase file\n");
    }
    scanTree(root, dataBase);
    fclose(dataBase);

    logprint("done scanning dataBase\n");
}

void scanTree(struct node* node, FILE* dataBase) {
    ASSERT(node != nullptr);
    ASSERT(dataBase != nullptr);

    char *line = (char*)calloc(MAX_NAME_LEN, sizeof(char));
    ASSERT (line != nullptr);

    fscanf(dataBase, "%s", line);
    node->data = line+1;
    if (line[0] == '?') {
        scanTree(CreateLeftNode(node, " "), dataBase);
        scanTree(CreateRightNode(node, " "), dataBase);
    }
}

void findOtherness(struct node* node1, struct node* node2) {
    ASSERT(node1 != nullptr);
    ASSERT(node2 != nullptr);

    struct node* origin1 = node1;
    struct node* origin2 = node2;

    struct node* parents1[30] = {nullptr};
    struct node* parents2[30] = {nullptr};
    int i = 0, j = 0;

    while (node1->parent) {
        parents1[i++] = node1;
        node1 = node1->parent;
    }
    parents1[i] = node1;


    while (node2->parent) {
        parents2[j++] = node2;
        node2 = node2->parent;
    }
    parents2[j] = node2;

    /*now parents1[0] = obj1, parents1[i] = root*/
    /*now parents2[0] = obj2, parents1[j] = root*/

    if (parents1[i-1] == parents2[j-1]) SpeakAndPrint("both %s and %s ", origin1->data, origin2->data);
    while (parents1[i--] == parents2[j--]) {
        if (parents1[i] == parents2[j]){
            if (parents1[i]->parent->left == parents1[i]) {
                SpeakAndPrint("%s, ", parents1[i+1]->data);
            } else {
                SpeakAndPrint("NOT %s, ", parents1[i+1]->data);
            }
        }
    }
    //printf("now i %d, j %d\n", i, j);
    i+=2;
    j+=2;

    if (i > 0) SpeakAndPrint("otherness: %s\n", origin1->data);
    while (i > 0) {
        if (parents1[i-1]->parent->left == parents1[i-1]) {
            SpeakAndPrint("%s, ", parents1[i]->data);
        } else {
            SpeakAndPrint("NOT %s, ", parents1[i]->data);
        }
        i--;
    }
    if (j > 0) SpeakAndPrint("meanwhile %s ", origin2->data);

    while (j > 0) {
       if (parents2[j-1]->parent->left == parents2[j-1]) {
            SpeakAndPrint("%s, ", parents2[j]->data);
        } else {
            SpeakAndPrint("NOT %s, ", parents2[j]->data);
        }
        j--;
    }
    printf("\n");
}

void GraphTreeDump(struct node* root) {
    ASSERT(root != nullptr);

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

    logprint("%d\n", system("cd C:\\Users\\alexm\\Documents\\Akinator"));
    logprint("%d\n", system(cmd));

    logprint("RESULT NAME IS %s\n", name);

    setHTML(name);

    imageNum++;
}

void DrawTree(struct node* node, const char* branchName, FILE* DumpFile) {
    ASSERT(node != nullptr);
    ASSERT(branchName != nullptr);
    ASSERT(DumpFile != nullptr);

    char* color = "lightgrey";
    if (node->left == nullptr && node->right == nullptr) color = "green";
    if (node->parent == nullptr) color = "cyan";

    dumpPrint("struct%p [\nlabel = \"{<data>data: %s|<parent>parent: %p|<left>left: %p|<right>right: %p}\", style = \"filled\", color = \"black\", fillcolor = \"%s\" \n];\n", node, node->data, node->parent, node->left, node->right, color);
    if (node->parent != NULL) {
        dumpPrint("struct%p -> struct%p [label = \"%s\" weight=900 constraint=true color=red];\n", node->parent, node, branchName);
    }
    if (node->left != NULL) {
        DrawTree(node->left, "yes", DumpFile);
    }
    if (node->right != NULL) {
        DrawTree(node->right, "no", DumpFile);
    }
}

void setHTML(const char* image) {
    ASSERT(image != nullptr);

    printHTML("<img src = \"%s\">", image);
    printHTML("<hr>");
}

void TreeCtor(struct Tree* tree, char* rootData) {
    ASSERT(tree != nullptr);
    ASSERT(rootData != nullptr);

    struct node* node1 = (struct node*)(calloc(1, sizeof(struct node)));
    node1->data   = rootData;
    node1->left   = nullptr;
    node1->right  = nullptr;
    node1->parent = nullptr;
    tree->root    = node1;
}

void Play(struct node* root) {
    ASSERT(root != nullptr);

    SpeakAndPrint("HELLO GYM BRO, WANNA PLAY SOME GAMES?\n");
    int ch = 0;
    while (ch != EXIT) {
        SpeakAndPrint("enter '#'-  to stop,  'd' - to definition");
        SpeakAndPrint("      'g' - to guess, 'o' - to otherness \n");
        ch = getchar();
        clearBuff();
        if (ch == GUESS){
            SpeakAndPrint("please answer yes (y) or no (n):\n");
            checkCondition(root);
            clearBuff();
        } else if (ch == DEFINE) {
            SpeakAndPrint("enter the name of object to define\n");

            char* name = nullptr;
            struct node* result = nullptr;

            GET_OBJ(name, result);

            PrintParents(result, RIGHT);
            printf("\n");
            clearBuff();

        } else if (ch == OTHERNESS) {
            SpeakAndPrint("enter the name of the first object:\n");
            char* name1 = nullptr;
            struct node* node1 = nullptr;

            GET_OBJ(name1, node1);

            SpeakAndPrint("now the second one\n");
            char* name2 = nullptr;
            struct node* node2 = nullptr;

            GET_OBJ(name2, node2);

            findOtherness(node1, node2);
            clearBuff();
        }
    }
}

void PrintNode(struct node* node) {
    ASSERT(node != nullptr);

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

struct node* CreateLeftNode(struct node* node, const char* str) {
    ASSERT(node != nullptr);
    ASSERT(str != nullptr);

    struct node* tmp_node = (struct node*)(calloc(1, sizeof(struct node)));
    ASSERT(tmp_node != nullptr);

    if (node->left == nullptr) {
        node->left = tmp_node;
        tmp_node->parent = node;
        tmp_node->data = (char*) str;
    } else {
        logprint("can't create left node, there is one already\n");
        return nullptr;
    }
    return tmp_node;
};

struct node* CreateRightNode(struct node* node, const char* str) {
    ASSERT(node != nullptr);
    ASSERT(str != nullptr);

    struct node* tmp_node = (struct node*)(calloc(1, sizeof(struct node)));
    ASSERT(tmp_node != nullptr);

    if (node->right == nullptr) {
        node->right = tmp_node;
        tmp_node->parent = node;
        tmp_node->data = (char*) str;
    } else {
        logprint("can't create right node, there is one already\n");
        return nullptr;
    }
    return tmp_node;
};

void checkCondition(struct node* node){
    ASSERT(node != nullptr);

    if (!(node->left) || !(node->right)){
        SpeakAndPrint("Let me guess.. it is %s? y or n\n", node->data);
        int ch = 0;
        if ((ch = getchar()) == YES){
            SpeakAndPrint("it was too easy!\n");
        } else if (ch == NO) {
            SpeakAndPrint("hmmm, then who was it?\n");
            char* name = (char*)calloc(MAX_NAME_LEN, sizeof(char));
            ASSERT(name != nullptr);

            scanf("%s", name);
            SpeakAndPrint("and what is %s's difference from %s?\n", name, node->data);

            char* diff = (char*)calloc(MAX_NAME_LEN, sizeof(char));
            ASSERT(diff != nullptr);

            scanf("%s", diff);
            SpeakAndPrint("ok, now i know!\n");

            CreateLeftNode(node, name);
            CreateRightNode(node, node->data);

            node->data = diff;
        }
    } else {
        SpeakAndPrint("%s?\n", node->data);
        char ch = ' ';
        if ((ch = getchar()) == NO) {
            clearBuff();
            checkCondition(node->right);
        } else if (ch == YES) {
            clearBuff();
            checkCondition(node->left);
        } else {
            SpeakAndPrint("no such answer\n");
            clearBuff();
            checkCondition(node);
        }
    }
}

void NodeByName(const char* name, struct node* node, struct node** result) {
    ASSERT(name != nullptr);
    ASSERT(node != nullptr);
    ASSERT(result != nullptr);

    if (stricmp(node->data, name) == 0) {
        *result = node;
    }
    if (node->left) {
        NodeByName(name, node->left, result);
    }
    if (node->right) {
        NodeByName(name, node->right, result);
    }
};

void PrintParents(struct node* node, int child) {
    ASSERT(node !=nullptr);

    if (node->parent){
        if (node->parent->left == node) PrintParents(node->parent, LEFT);
        else PrintParents(node->parent, RIGHT);
    }
    if (node->left == nullptr && node->right == nullptr) return;
    if (child == RIGHT) SpeakAndPrint("not ");
    SpeakAndPrint("%s, ", node->data);
}

void clearBuff() {
    char ch = 0;
    while((ch = getchar()) != '\n')
        continue;
}


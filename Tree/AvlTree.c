#include "AvlTree.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../Data/block.h"
#include "../../Util/Utils.h"
#include "../DoublyLinkedList/DoublyLinkedList.h"
#include "../HashTable/HashTable.h"

enum BalanceCases {
    RR,
    LL,
    RL,
    LR
};

enum FatorDeBalanceamento {
    MAX,
    MIN
};

typedef struct node {
    List data;

    double max, min;

    struct node* Father;
    struct node* Right;
    struct node* Left;

} node;

typedef struct tree {
    node* Root;

    unsigned int size;

} tree;

unsigned int avl_len(AvlTree in) {
    return ((tree*)in)->size;
}

void memAvl() {
    printf("Tamanho da tree = %ld\nTamanho do Node Avl = %ld\n", sizeof(tree), sizeof(node));
}

Node avl_right(Node Current) {
    return Current ? ((node*)Current)->Right : NULL;
}

Node avl_left(Node Current) {
    return Current ? ((node*)Current)->Left : NULL;
}

Node avl_father(Node Current) {
    return Current ? ((node*)Current)->Father : NULL;
}

List AvlList(Node Currrent) {
    return Currrent ? ((node*)Currrent)->data : NULL;
}

Node avl_data(Node Currrent) {
    return Currrent ? ((node*)Currrent)->data : NULL;
}

Node avl_root(AvlTree Tree) {
    return Tree ? ((tree*)Tree)->Root : NULL;
}

void Avlset_Left(Node Current, Node Left) {
    node* current = Current;
    if (current) {
        current->Left = Left;
    }
}

void Avlset_Right(Node Current, Node Right) {
    node* current = Current;
    if (current) {
        current->Right = Right;
    }
}

AvlTree new_avl() {
    tree* avl = calloc(1, sizeof(tree));

    if (!avl) {
        printf("Memoria cheia!\nFalha ao criar  void* avl!\n");
        return NULL;
    }

    avl->size = 0;
    avl->Root = NULL;

    return avl;
}

bool avl_is_empty(AvlTree t) {
    return t ? (!((tree*)t)->Root ? true : false) : false;
}

int AvlDirection(node* CurrentNode) {
    if (!CurrentNode->Father)
        return -1;

    if (CurrentNode->Father->Right == CurrentNode)
        return 0;

    return 1;
}

void FixReference(node* up, node* current, tree* Tree) {
    switch (AvlDirection(current)) {
        case -1:
            Tree->Root = up;
            break;

        case 0:
            current->Father->Right = up;
            break;

        case 1:
            current->Father->Left = up;
            break;
    }

    up->Father = current->Father;

    current->Father = up;

    if (Tree->Root == current)
        Tree->Root = up;
}

void printDouble(void* num);

int* get_max_mim(List block_list) {
    double max = -99999, min = 99999;

    for (void* runner = list_head(block_list); runner; runner = list_next(runner)) {
        if (block_get_x(list_data(runner)) < min)
            min = block_get_x(list_data(runner));
        if (block_get_x(list_data(runner)) + block_get_w(list_data(runner)) > max)
            min = block_get_x(list_data(runner)) + block_get_w(list_data(runner));
    }

    int* out = calloc(2, sizeof(int));

    out[MAX] = max;
    out[MIN] = min;

    return out;
}

bool RotationR(Node CurrentNode, tree* Tree) {
    if (!CurrentNode) return false;

    node* current = CurrentNode;

    node *up, *aux;

    up = avl_left(current);
    aux = avl_right(up);
    Avlset_Right(up, current);
    Avlset_Left(current, aux);

    if (aux)
        aux->Father = current;

    FixReference(up, current, Tree);

    //Corrigindo o max e min do current

    current->min = block_get_x(list_data(list_head(avl_data(current))));
    current->max = current->min + block_get_w(list_data(list_head(avl_data(current))));

    if (current->Right)
        if (current->Right->max > current->max) current->max = current->Right->max;
    if (current->Left)
        if (current->Left->max > current->max) current->max = current->Left->max;

    if (current->Right)
        if (current->Right->min < current->min) current->min = current->Right->min;
    if (current->Left)
        if (current->Left->min < current->min) current->min = current->Left->min;

    //Corrigindo max e min do up
    if (current->max > up->max) up->max = current->max;

    return true;
}

bool RotationL(Node CurrentNode, tree* Tree) {
    if (!CurrentNode) return false;

    node* current = CurrentNode;

    node *up, *aux;

    up = avl_right(current);
    aux = avl_left(up);
    Avlset_Left(up, current);
    Avlset_Right(current, aux);

    if (aux)
        aux->Father = current;

    FixReference(up, current, Tree);

    current->min = block_get_x(list_data(list_head(avl_data(current))));
    current->max = current->min + block_get_w(list_data(list_head(avl_data(current))));

    //Corrigindo o max e min do current
    if (current->Right)
        if (current->Right->max > current->max) current->max = current->Right->max;
    if (current->Left)
        if (current->Left->max > current->max) current->max = current->Left->max;

    if (current->Right)
        if (current->Right->min < current->min) current->min = current->Right->min;
    if (current->Left)
        if (current->Left->min < current->min) current->min = current->Left->min;

    //Corrigindo max e min do up
    up->min = current->min;
    if (current->max > up->max) up->max = current->max;

    return true;
}

unsigned int NodeHeight(Node CurrentNode) {
    if (!CurrentNode) return 0;

    return NodeHeight(avl_right(CurrentNode)) > NodeHeight(avl_left(CurrentNode)) ? NodeHeight(avl_right(CurrentNode)) + 1 : NodeHeight(avl_left(CurrentNode)) + 1;
}

int CheckBalanceCase(node* CurrentNode, tree* Tree) {
    if (NodeHeight(avl_left(CurrentNode)) > NodeHeight(avl_right(CurrentNode))) {
        if (NodeHeight(avl_left(avl_left(CurrentNode))) < NodeHeight(avl_right(avl_left(CurrentNode))))
            return LR;
        else
            return LL;
    }

    if (NodeHeight(avl_right(avl_right(CurrentNode))) < NodeHeight(avl_left(avl_right(CurrentNode))))
        return RL;
    else
        return RR;
}

void AvlBalance(node* CurrentNode, tree* Tree) {
    if (!CurrentNode) return;

    if (avl_right(CurrentNode) || avl_left(CurrentNode))
        if (abs(NodeHeight(avl_right(CurrentNode)) - NodeHeight(avl_left(CurrentNode))) >= 2)
            switch (CheckBalanceCase(CurrentNode, Tree)) {
                case RR:
                    RotationL(CurrentNode, Tree);
                    break;
                case LL:
                    RotationR(CurrentNode, Tree);
                    break;
                case RL:
                    RotationR(avl_right(CurrentNode), Tree);
                    RotationL(CurrentNode, Tree);
                    break;
                case LR:
                    RotationL(avl_left(CurrentNode), Tree);
                    RotationR(CurrentNode, Tree);
                    break;
            }
}

node* CreateLeaf(void* Info, Node Father) {
    node* leaf = calloc(1, sizeof(node));

    leaf->data = new_list(block_compare_cep, not_to_string, not_free);
    list_insert(leaf->data, Info);

    leaf->Father = Father;
    leaf->Right = leaf->Left = NULL;

    leaf->min = block_get_x(Info);
    leaf->max = block_get_x(Info) + block_get_w(Info);

    return leaf;
}

void InsertRecursion(node* CurrentNode, void* Info, double (*compare_keys)(void*, void*), tree* Tree) {
    if (block_get_x(Info) <= CurrentNode->min)
        CurrentNode->min = block_get_x(Info);

    if (block_get_x(Info) + block_get_w(Info) >= CurrentNode->max)
        CurrentNode->max = block_get_x(Info) + block_get_w(Info);

    if (compare_keys(Info, list_data(list_head(avl_data(CurrentNode)))) == 0) {
        list_insert(AvlList(CurrentNode), Info);
        return;
    }

    if (compare_keys(Info, list_data(list_head(avl_data(CurrentNode)))) > 0) {
        if (avl_right(CurrentNode))
            InsertRecursion(avl_right(CurrentNode), Info, compare_keys, Tree);

        else
            Avlset_Right(CurrentNode, CreateLeaf(Info, CurrentNode));
    }

    else if (avl_left(CurrentNode))
        InsertRecursion(avl_left(CurrentNode), Info, compare_keys, Tree);
    else
        Avlset_Left(CurrentNode, CreateLeaf(Info, CurrentNode));

    AvlBalance(CurrentNode, Tree);
}

bool avl_insert(AvlTree Tree, void* Info, double (*compare_keys)(void*, void*)) {
    if (!Tree || !Info) return false;

    tree* AvlTree = Tree;

    if (!AvlTree->Root)
        AvlTree->Root = CreateLeaf(Info, NULL);
    else
        InsertRecursion(avl_root(AvlTree), Info, compare_keys, Tree);

    AvlTree->size++;
    return true;
}

bool avl_exists_keyRecursion(node* current, void* Info, double (*compare_keys)(void*, void*)) {
    if (!current) return false;

    if (avl_exists_keyRecursion(avl_left(current), Info, compare_keys)) return true;

    if (avl_exists_keyRecursion(avl_right(current), Info, compare_keys)) return true;

    for (void* runner = list_head(avl_data(current)); runner; runner = list_next(runner)) {
        if (compare_keys(Info, list_data(runner)) == 0)
            return true;
    }

    return false;
}

bool avl_exists_key(AvlTree Tree, void* Info, double (*compare_keys)(void*, void*)) {
    return avl_exists_keyRecursion(((tree*)Tree)->Root, Info, compare_keys);
}

double min_replace, max_replace;

node* fullLeft(node* Node) {
    void* out = avl_left(Node) ? fullLeft(avl_left(Node)) : Node;

    Node->min = block_get_x(list_data(list_head(avl_data(Node))));
    if (min_replace < Node->min)
        Node->min = min_replace;
    else
        min_replace = Node->min;

    Node->max = Node->min + block_get_w(list_data(list_head(avl_data(Node))));
    if (max_replace > Node->max)
        Node->max = max_replace;
    else
        max_replace = Node->max;

    return out;
}

node* nodeToReplace(node* Node) {
    if (avl_left(Node) && !avl_right(Node))
        return avl_left(Node);

    if (avl_right(Node) && !avl_left(Node))
        return avl_right(Node);

    max_replace = -99999;
    min_replace = 99999;

    return avl_right(Node) ? fullLeft(avl_right(Node)) : NULL;
}

void replaceNode(node* old, node* new, tree* Tree) {
    //Caso 1
    if (!new)
        AvlDirection(old) ? (old->Father->Left = NULL) : (old->Father->Right = NULL);

    //Caso 2
    else if ((old->Left && !old->Right) || (old->Right && !old->Left)) {
        if (old == avl_root(Tree))
            Tree->Root = new;
        else
            AvlDirection(old) ? (old->Father->Left = new) : (old->Father->Right = new);

        new->Father = old->Father;
    }

    //Caso 3
    else {
        //Caso 3.1
        if (new == avl_right(old)) {
            if (avl_root(Tree) != old) {
                AvlDirection(old) ? (old->Father->Left = new) : (old->Father->Right = new);
            } else {
                Tree->Root = new;
            }
        } else {
            if (avl_root(Tree) != old)
                AvlDirection(old) ? (old->Father->Left = new) : (old->Father->Right = new);
            else {
                Tree->Root = new;
            }
            new->Father->Left = new->Right;
            new->Right = old->Right;
        }
        old->Right->Father = new;
        old->Left->Father = new;
        new->Left = old->Left;
        new->Father = old->Father;
    }
}

void att_max_min(node* No) {
    No->min = block_get_x(list_data(list_head(avl_data(No))));
    No->max = No->min + block_get_w(list_data(list_head(avl_data(No))));

    if (No->Left) {
        if (No->Left->min < No->min)
            No->min = No->Left->min;

        if (No->Left->max > No->max)
            No->max = No->Left->max;
    }

    if (No->Right) {
        if (No->Right->min < No->min)
            No->min = No->Right->min;

        if (No->Right->max > No->max)
            No->max = No->Right->max;
    }
}

void* avl_removeRecursion(node* current, tree* Tree, void* data, double (*compare_keys)(void*, void*), double (*compare_data)(void*, void*)) {
    if (compare_keys(data, list_data(list_head(avl_data(current)))) > 0)
        if (avl_right(current))
            return avl_removeRecursion(avl_right(current), Tree, data, compare_keys, compare_data);
 
    if (compare_keys(data, list_data(list_head(avl_data(current)))) < 0)
        if (avl_left(current)) 
            return avl_removeRecursion(avl_left(current), Tree, data, compare_keys, compare_data);

    void* out;
    if (compare_keys(data, list_data(list_head(avl_data(current)))) == 0) {
        out = list_remove(current->data, list_index_of(current->data, block_get_cep(data)));

        if (!list_head(current->data)) {
            node* replace = nodeToReplace(current);

            list_destroy(current->data);

            if (replace) {
                replaceNode(current, replace, Tree);
                AvlBalance(replace, Tree);
                att_max_min(replace);
            } else {
                AvlDirection(current) ? (current->Father->Left = NULL) : (current->Father->Right = NULL);
                att_max_min(current->Father);
            }
        }

        Tree->size--;

        return out;
    }

    if (current)
        AvlBalance(current, Tree);

    return NULL;
}

void* avl_remove(AvlTree Tree, void* data, double (*compare_keys)(void*, void*), double (*compare_data)(void*, void*)) {
    if (!avl_exists_key(Tree, data, compare_keys)) return NULL;

    void* out = avl_removeRecursion(avl_root(Tree), Tree, data, compare_keys, compare_data);

    AvlBalance(avl_root(Tree), Tree);

    return out;
}

List avl_getRecursion(node* current, double x, double y, double w, double h, List quadLst) {
    for (void* runner = list_head(avl_data(current)); runner; runner = list_next(runner)) {
        if (block_inside(list_data(runner), x, y, w, h)) {
            list_insert(quadLst, list_data(runner));
        }
    }

    if (current->Left)
        if (current->Left->max >= x && current->Left->min <= x + w)
            avl_getRecursion(avl_left(current), x, y, w, h, quadLst);

    if (current->Right)
        if (current->Right->max >= x && current->Right->min <= x + w)
            avl_getRecursion(avl_right(current), x, y, w, h, quadLst);

    return NULL;
}

List avl_get(AvlTree Tree, double x, double y, double w, double h) {
    List quadLst = new_list(not_compare, not_to_string, not_free);

    avl_getRecursion(avl_root(Tree), x, y, w, h, quadLst);

    return quadLst;
}

void avl_symmetrical(Node CurrentNode, void (*NodePrecessor)(Node)) {
    if (!CurrentNode) return;

    avl_symmetrical(avl_left(CurrentNode), NodePrecessor);

    avl_symmetrical(avl_right(CurrentNode), NodePrecessor);

    NodePrecessor(avl_data(CurrentNode));
}

void avl_destroyRecursion(node* current, tree* Tree, void (*free_func)(void*)) {
    if (!current) return;

    avl_destroyRecursion(avl_left(current), Tree, free_func);

    avl_destroyRecursion(avl_right(current), Tree, free_func);

    list_destroy(current->data);

    free(current);
}

void avl_destroy(AvlTree Tree, void (*free_func)(void*)) {
    avl_destroyRecursion(avl_root(Tree), Tree, free_func);

    free(Tree);
}

void avl_print_dot_file(FILE* file, Node no) {
    node* No = no;

    if (!no) return;

    avl_print_dot_file(file, avl_left(no));

    avl_print_dot_file(file, avl_right(no));

    if (avl_left(no))
        fprintf(file, "\t%lf->%lf;\n", block_get_x(list_data(list_head(No->data))), block_get_x(list_data(list_head(No->Left->data))));
    if (avl_right(no))
        fprintf(file, "\t%lf->%lf;\n", block_get_x(list_data(list_head(No->data))), block_get_x(list_data(list_head(No->Right->data))));

    fprintf(file, "\t%lf[penwidth  = 2,fontsize = 5,fillcolor=\"Black:Black\",shape = Msquare,label = \"X = %lf\n", block_get_x(list_data(list_head(No->data))), block_get_x(list_data(list_head(No->data))));
    void* runner = list_head(No->data);
    fprintf(file, "\nQuantidade de quadras = %ld\n", list_len(No->data));
    fprintf(file, "\nFator de balanceamento = %d\n", NodeHeight(No->Left) - NodeHeight(No->Right));
    fprintf(file, "\nCEPs: ");
    for (int i = 0; i < 3; i++) {
        fprintf(file, "%s %c ", block_get_cep(list_data(runner)), i != 2 ? ',' : ' ');
        runner = list_next(runner);
        if (!runner)
            break;
    }
    fprintf(file, ". . .\n");

    fprintf(file, "\n\tAltura = %d\n\n\n\n\n\n\n\n\n\n\n\n\n", NodeHeight(No));
    fprintf(file, "\nMin=%lf                                       Max=%lf\"]\n", No->min, No->max);
}

void convertToHasTableRecursion(node* current, HashTable out, char* (*Key)(void*)) {
    if (current->Right)
        convertToHasTableRecursion(current->Right, out, Key);

    if (current->Left)
        convertToHasTableRecursion(current->Left, out, Key);

    for (void* runner = list_head(current->data); runner; runner = list_next(runner))
        hash_insert(out, Key(list_data(runner)), list_data(runner));
}

HashTable avl_to_hash(AvlTree In, char* (*Key)(void*), double (*_compare)(void*, void*), char* (*_to_string)(void*), void (*free_func)(void*)) {
    tree* in = In;

    HashTable out = new_hash(in->size, _compare, _to_string, free_func);

    convertToHasTableRecursion(in->Root, out, Key);

    return out;
}

//avl max_level
int max_level(Node no) {
    if (!no) return 0;

    int left = max_level(avl_left(no));
    int right = max_level(avl_right(no));

    return left > right ? left + 1 : right + 1;
}
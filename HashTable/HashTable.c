
#include "HashTable.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../DoublyLinkedList/DoublyLinkedList.h"

typedef struct Reg {

    List RegisterList;

} Reg;

typedef struct hashTable {

    unsigned int size;
    
    Reg* Registers;

    double (*_compare)(void*, void*);
    char* (*_to_string)(void*);
    void (*free_func)(void*);

} hashTable;

unsigned int hash_len(HashTable __HT) {
    return __HT ? ((hashTable*)__HT)->size : 0;
}

void memHashTable() {
    printf("Tamanho da HT = %ld\nTamanho do REG = %ld\n", sizeof(hashTable), sizeof(Reg));
}

HashTable new_hash(unsigned int len, double (*_compare)(void*, void*), char* (*_to_string)(void*), void (*free_func)(void*)) {
    hashTable* HT = calloc(1, sizeof(hashTable));

    HT->size = len;

    HT->Registers = calloc(HT->size, sizeof(Reg));

    HT->_compare = _compare;
    HT->_to_string = _to_string;
    HT->free_func = free_func;

    return HT;
}

unsigned int hash_table_len(HashTable __HT) {
    return ((hashTable*)__HT)->size;
}

unsigned int CreateKey(HashTable __HT, char __StringKey[]) {
    unsigned int key = 0;

    for (char* runner = &__StringKey[0]; *runner != '\0'; runner++) {
        key += *runner;
    }

    return key % hash_table_len(__HT);
}

void hash_insert(HashTable __HT, char Key[], void* Data) {
    unsigned int key = CreateKey(__HT, Key);

    hashTable* ht = __HT;

    if (!hash_exist_key(__HT, Key))
        ht->Registers[key].RegisterList = new_list(ht->_compare, ht->_to_string, ht->free_func);

    list_insert(ht->Registers[key].RegisterList, Data);
}

bool hash_exist_key(HashTable __HT, char Key[]) {
    unsigned int key = CreateKey(__HT, Key);

    return ((hashTable*)__HT)->Registers[key].RegisterList ? true : false;
}

void* hash_get(HashTable __HT, char Key[]) {
    unsigned int key = CreateKey(__HT, Key);

    return ((hashTable*)__HT)->Registers[key].RegisterList;
}

bool hash_remove(HashTable __HT, char Key[], double (*compare)(void*, void*)) {
    if (!hash_exist_key(__HT, Key)) return NULL;

    unsigned int key = CreateKey(__HT, Key);

    hashTable* ht = __HT;

    list_remove(ht->Registers[key].RegisterList, list_index_of(ht->Registers[key].RegisterList, Key));

    return true;
}

void hash_destroy(HashTable __HT, void (*free_func)(void*)) {
    hashTable* ht = __HT;

    for (int cont = 0; cont < ht->size; cont++) {
        list_destroy(ht->Registers[cont].RegisterList);
    }

    free(ht->Registers);
    free(ht);
}

void hash_view(HashTable __HT) {
    hashTable* ht = __HT;

    printf("Index\tElements\n");

    for (int cont = 0; cont < ht->size; cont++) {
        printf("  %d\t    %ld\n", cont, list_len(ht->Registers[cont].RegisterList));
    }
}
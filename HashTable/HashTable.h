
#include <stdbool.h>

#ifndef __HASH_TABLE_
#define __HASH_TABLE_

void memHashTable();

typedef void* HashTable;

/*
*
*   Retorna o tamanho da hash table HT
*
*/
unsigned int hash_len(HashTable __HT);

/*
*
*   Cria uma estrutura hash table e a retorna
*
*/
HashTable new_hash(unsigned int len, double (*_compare)(void*, void*), char* (*_to_string)(void*), void (*free_func)(void*));

/*
*
*   Insere data na leu posição da hash table 
*
*/
void hash_insert(HashTable __HT, char Key[], void* Data);

/*
*
*   Retorna verdadeiro caso exista um valor armazenado no index Key
*
*/
bool hash_exist_key(HashTable __HT, char Key[]);

/*
*
*   Retorna a lista localizada na posiçao key da hash table
*
*/
void* hash_get(HashTable __HT, char Key[]);

/*
*
*   Remove o elemento que possui a respectiva key de um index especifico da hash table 
*
*/
bool hash_remove(HashTable __HT, char Key[], double (*compare)(void*, void*));

/*
*
*   Libera toda a memoria alocada pela hash table
*
*/
void hash_destroy(HashTable __HT, void (*free_func)(void*));

/*
*
*   Exibe uma visao geral do conteudo da hash table
*
*/
void hash_view(HashTable __HT);

#endif
#include <stdio.h>
#include <stdlib.h>

#ifndef __DOUBLY_LINKED_LIST_
#define __DOUBLY_LINKED_LIST_

typedef void* List;
typedef void* Node;

/*      Node Functions      */

/*  
    Retorna o próximo nó de NODE
     
    Retorna NULL caso ele não tenha um próximo nó ou caso ele seja NULL
    
*/
Node list_next(Node __node);
/*  
    
    Retorna o nó anterior de NODE
     
    Retorna NULL caso ele não tenha um nó anterior ou caso ele seja NULL
    
*/
Node list_prev(Node __node);
/*  
    
    Retorna a informação armazeda em NODE
    
    Retorna NULL caso não haja nenhuma informação ou NODE seja NULL
    
*/
void* list_data(Node __node);

/*      List Functions      */

/*  
    
    Retorna uma  void* vazia    
    
*/
List new_list(double (*compare)(void*, void*), char* (*to_string)(void*), void (*free_func)(void*));
/*  
    
    Exclui uma  void* e libera toda a sua memória

    Não há segmentation fault caso LIST ou FREE forem NULL 
    
*/
void list_destroy(List __list);
/*  
    
    Cria um novo nó com DATA armazenada e a insere ao fim de LIST

    Não há segmentation fault caso LIST ou DATA forem NULL 
    
*/
void list_insert(List __list, void* __data);
/*
    
    Retorna o nó de LIST na posição INDEX

    Caso INDEX seja maior que o tamanho de LIST, a célula retornada será a última

    Não há segmentation fault caso LIST for NULL 
    
*/
Node list_index(List __list, unsigned int __index);
/*  
   
    Remove o nó de LIST na posição INDEX e libera a memória de seus dados com a função FREE_DATA 

    Caso INDEX seja maior que o tamanho de LIST, a célula retornada será a última

    Não há segmentation fault caso LIST ou FREE_DATA forem NULL 

*/
void* list_remove(List __list, unsigned int __index);
/*  

    Aplica a função PRINT_DATA nos dados de todos os nós de LIST em ordem crescente

    Não há segmentation fault caso LIST ou PRINT_DATA forem NULL 
       
*/
void list_print(List __list);
/*  
    
    Retorna o primeiro nó de LIST

    Não há segmentation fault caso LIST for NULL 

*/
Node list_head(List __list);
/*  
    
    Retorna o ultimo nó de LIST

    Não há segmentation fault caso LIST for NULL 

*/
Node list_tail(List __list);
/*  

    Retorna o tamanho de LIST

    Não há segmentation fault caso LIST for NULL 

*/
size_t list_len(List __list);

/*
*
*   Faz uma busca na lista aplicando a função compare ao data e a informação contida em cada nó
*   e retorna um no caso o conteudo dele seja igual ao conteudo de data
*
*/
void* list_search(List __list, void* data);

/*
*
*   Faz uma busca na lista aplicando a função compare ao data e a informação contida em cada nó
*   e retorna o index de um no caso o conteudo dele seja igual ao conteudo de data
*
*/
int list_index_of(List __list, void* data);

void list_set_data(Node node, void* data);
void list_sort(List __list) ;
void list_new_head(List __list, void* data);
void* list_to_array(List _list);
void list_remove_and_destroy(List _list,unsigned int _index);

#endif
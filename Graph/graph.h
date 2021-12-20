#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../DoublyLinkedList/DoublyLinkedList.h"

#ifndef GRAPH_H
#define GRAPH_H

//Tipo grafo
typedef void* Graph;

//Tipo aresta
typedef void* Edge;

//Tipo vertice
typedef void* Vertex;


char* _graph_get_vertex_id(void* a);
char* char_djikstra_id(void* n);
void svg_make_path(FILE* file, char* name, List vertex_list, char* color,double g);

/*
@param size_t len - tamanho do vetor de vertices
@return Graph - retorna um grafo
*/
Graph new_graph(size_t len);

/*
@param Graph graph - grafo
@param char* id - id do vertice
@return int - retorna o indice do vertice
*/
int graph_search_vertex_by_id(Graph graph, char* id);

/*
Insere um vertice no grafo
@param Graph graph - grafo
@param char* id - id do vertice
@param double x - coordenada x do vertice
@param double y - coordenada y do vertice
*/
void graph_add_vertex(Graph graph, char* id, double x, double y);

/*
Insere uma aresta no grafo
@param Graph graph - grafo
@param char* v1 - id do vertice 1
@param char* v2 - id do vertice 2
@param char* lesq - id do vertice de entrada da aresta
@param char* ldir - id do vertice de saida da aresta
@param double cmp - comprimento da aresta
@param double vm - velocidade media da aresta
*/
void graph_add_edge(Graph graph, char* v1, char* v2, char* lesq, char* ldir, char* nome, double cmp, double vm);

/*
Procura uma aresta no grafo
@param Graph graph - grafo
@param char* v1 - id do vertice 1
@param char* v2 - id do vertice 2
@return Edge - retorna a aresta
*/
Edge graph_search_edge(Graph graph, char* v1, char* v2);

/*
@param Edge aresta
@return void* - retorna os dados de uma aresta
*/
void* graph_get_edge_info(Edge edge);

/*
Remove uma aresta do grafo
@param Graph graph - grafo
@param char* v1 - id do vertice 1
@param char* v2 - id do vertice 2
*/
void graph_remove_edge(Graph graph, char* v1, char* v2);

/*
@param Graph graph - grafo
@param char* v1 - id do vertice 1
@param char* v2 - id do vertice 2
@return bool - retorna true se a aresta v1 esta conectada a v2, false caso contrario
*/
bool graph_is_connected(Graph graph, char* v1, char* v2);

/*
@param Graph graph - grafo
@param char* v - id do vertice
@return List -- Lista com todos os vertices que v se liga
*/
List graph_all_conections(Graph graph, char* v);

/*
Printa todos os vertices do grafo
@param Graph graph - grafo
*/
void graph_print(Graph graph);

/*
Printa o id do vertice
@param char* v - vertice
*/
void print_vertex_id(void* v);

/*
Printa o id do vertice ao fim da aresta
@param Edge Aresta - aresta
*/
void print_aresta(Edge Aresta);

void print_djikstra_node_id(void* n);

/*
Retorna uma lista de vertices que são a caminho mais curto entre v1 e v2
@param Graph graph - grafo
@param char* v1 - id do vertice 1
@param char* v2 - id do vertice 2
*/
List graph_shortest_path(Graph graph, char* v1, char* v2);

/*
Retorna uma lista de vertices que são a caminho mais rapido entre v1 e v2
@param Graph graph - grafo
@param char* v1 - id do vertice 1
@param char* v2 - id do vertice 2
*/
List graph_fastest_path(Graph graph, char* v1, char* v2);

/*
Libera a memória do grafo
@param Graph graph - grafo
*/
void graph_destroy(Graph graph);

void graph_set_info(Graph graph, char* v1, char* v2, void* info);

void graph_add_edge_wi(Graph graph, char* v1, char* v2);

double graph_compare_aresta(void* a, void* b);

void** graph_kruskal(List lista_arestas);

Edge get_edge_by_vertex(Graph graph, char* v1, char* v2);

List edge_list(Graph graph);

Edge _create_edge(char* nome, char* ldir, char* lesq, double cmp, double vm, void* ini, void* fim);

Vertex _create_vertex(char* id, double x, double y);

void grahp_remove_vertex(Graph graph, char* id);

void graph_remove_index(Graph graph, int index);

void descricao_djikstra(FILE* txt_report, List lista_de_vertices);

List kosaraju(Graph graph, double limiar, FILE* under_svg);

char* _print_edge_vertexes_ids(void* A);

Vertex graph_get_ini(Edge e);

Vertex graph_get_fim(Edge e);

void graph_edge_new_vm(Edge e, double new);

List remove_duplicate_vertex(List vertex_list);

double graph_compare_vertex(void* v1, void* v2);

bool graph_vertex_in_region(Edge v, double x, double y, double w, double h);

void graph_print_on_svg(Graph g, FILE* svg_file);

void set_compare_vertex(double x, double y);

double _distance_between_vertex(void* a, double x, double y);

Edge graph_get_vertex_i(Graph graph, int i);

char* vertex_to_string(void* vertex);

double graph_get_vm(Edge e);

double vertex_get_x(void* vertex);

size_t graph_len(Graph graph);

double vertex_get_y(void* vertex);

char* _graph_get_edge_id(void* a);

List graph_get_arestas(Vertex v);



#endif
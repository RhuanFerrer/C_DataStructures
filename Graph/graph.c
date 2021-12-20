#include "graph.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../DoublyLinkedList/DoublyLinkedList.h"
#include "../PriorityQueue/pqueue.h"
#include "../Stack/stack.h"
#include "../Tree/AvlTree.h"

typedef struct vertice {
    
    double x, y;
    char* id;

    List arestas;

    bool visit;
} vertice;

typedef struct dados {
    char* nome;  //Nome da rua a qual pertence o segmento
    double cmp;  //comprimento(em metros) do segmento de rua
} dados;

typedef struct aresta {
    vertice* ini;
    vertice* fim;

    dados info;

} aresta;

typedef struct grafo {
    int cont;
    size_t len;
    vertice* vertices;
} grafo;

size_t graph_len(Graph graph) {
    grafo* g = (grafo*)graph;
    return g->len;
}

double _graph_compare_edges(void* a, void* b);

char* _graph_get_edge_id(void* a);

Graph new_graph(size_t len) {
    grafo* g = malloc(sizeof(grafo));
    g->cont = 0;
    g->len = len;
    g->vertices = malloc(sizeof(vertice) * len);

    for (int i = 0; i < len; i++) {
        g->vertices[i].arestas = NULL;
    }

    return g;
}

void set_graph_visit_false(Graph g) {
    grafo* graph = (grafo*)g;
    for (int i = 0; i < graph->len; i++) {
        graph->vertices[i].visit = false;
    }
}

char* _print_edge_vertexes_ids(void* A) {
    aresta* a = (aresta*)A;
    char* str = (char*)malloc(sizeof(char) * (strlen(a->ini->id) + strlen(a->fim->id) + 5));
    strcpy(str, "[");
    strcat(str, a->ini->id);
    strcat(str, ":");
    strcat(str, a->fim->id);
    strcat(str, "]");
    return str;
}

int graph_search_vertex_by_id(Graph graph, char* id) {
    grafo* g = (grafo*)graph;
    for (int i = 0; i < g->len; i++) {
        if (g->vertices[i].id != NULL) {
            if (strcmp(g->vertices[i].id, id) == 0) {
                return i;
            }
        }
    }

    return -1;
}

bool graph_vertex_in_region(Edge v, double x, double y, double w, double h) {
    vertice* vv = (vertice*)v;
    if (vv->x >= x && vv->x <= x + w && vv->y >= y && vv->y <= y + h) {
        return true;
    }
    return false;
}

void grahp_remove_vertex(Graph graph, char* id) {
    grafo* g = (grafo*)graph;
    int index = graph_search_vertex_by_id(graph, id);
    if (index == -1) {
        return;
    }
    list_destroy(g->vertices[index].arestas);
    g->vertices[index].arestas = NULL;
    g->vertices[index].id = NULL;
    g->vertices[index].x = 0;
    g->vertices[index].y = 0;
}

double _graph_compare_edges(void* a, void* b) {
    aresta* e1 = (aresta*)a;
    aresta* e2 = (aresta*)b;
    return !strcmp(e1->info.nome, e2->info.nome);
}

double _graph_compare_vertex(void* a, void* b) {
    vertice* v1 = (vertice*)a;
    vertice* v2 = (vertice*)b;
    return !strcmp(v1->id, v2->id);
}

char* _graph_get_vertex_id(void* a) {
    vertice* v = (vertice*)a;
    return copyString(v->id);
}

void _destroy_vertex(void* a) {
    vertice* v = (vertice*)a;
    free(v->id);
    free(v);
}

void _destroy_edge(void* a) {
    aresta* e = (aresta*)a;
    free(e->info.nome);
    free(e);
}

Vertex _create_vertex(char* id, double x, double y) {
    vertice* v = malloc(sizeof(vertice));
    v->id = id;
    v->x = x;
    v->y = y;
    v->visit = false;
    v->arestas = new_list(_graph_compare_edges, _graph_get_edge_id, _destroy_edge);
    return v;
}

void graph_add_vertex(Graph graph, char* id, double x, double y) {
    grafo* g = (grafo*)graph;

    vertice* v = _create_vertex(id, x, y);
    g->vertices[g->cont++] = *v;
}

Edge _create_edge(char* nome, char* ldir, char* lesq, double cmp, double vm, void* ini, void* fim) {
    aresta* e = malloc(sizeof(aresta));
    e->ini = (vertice*)ini;
    e->fim = (vertice*)fim;
    e->info.nome = nome;
  
    e->info.cmp = cmp;

    return e;
}

void graph_set_info(Graph graph, char* v1, char* v2, void* info){
    aresta* a = graph_search_edge(graph, v1, v2);
    a->info = *((dados*)info);
}

void graph_add_edge_wi(Graph graph, char* v1, char* v2){
    grafo* g = (grafo*)graph;
    int index1 = graph_search_vertex_by_id(graph, v1);
    int index2 = graph_search_vertex_by_id(graph, v2);
    if (index1 == -1 || index2 == -1) {
        return;
    }
    aresta* e = calloc(1, sizeof(aresta));
    e->ini = &g->vertices[index1];
    e->fim = &g->vertices[index2];
}

void graph_add_edge(Graph graph, char* v1, char* v2, char* lesq, char* ldir, char* nome, double cmp, double vm) {
    grafo* g = (grafo*)graph;
    int i1 = graph_search_vertex_by_id(graph, v1);
    int i2 = graph_search_vertex_by_id(graph, v2);

    if (i1 == -1 || i2 == -1) {
        printf("v1: %s, v2: %s\n", v1, v2);
        printf("Vertice não encontrado\n");
        return;
    }

    aresta* a = _create_edge(nome, ldir, lesq, cmp, vm, &g->vertices[i1], &g->vertices[i2]);

    list_insert(a->ini->arestas, a);
}

Edge graph_search_edge(Graph graph, char* v1, char* v2) {
    grafo* g = (grafo*)graph;
    int i1 = graph_search_vertex_by_id(graph, v1);

    if (i1 == -1) {
        printf("Vertice não encontrado\n");
        return NULL;
    }

    aresta* runner;
    for (runner = list_head(g->vertices[i1].arestas); runner != NULL; runner = list_next(runner)) {
        if (strcmp(runner->fim->id, v2) == 0) {
            return runner;
        }
    }

    return NULL;
}

Vertex graph_get_ini(Edge e) {
    aresta* a = (aresta*)e;
    return a->ini;
}

Vertex graph_get_fim(Edge e) {
    aresta* a = (aresta*)e;
    return a->fim;
}

void* graph_get_edge_info(Edge edge) {
    aresta* a = (aresta*)edge;
    return &a->info;
}

void graph_remove_edge(Graph graph, char* v1, char* v2) {
    grafo* g = (grafo*)graph;
    int i1 = graph_search_vertex_by_id(graph, v1);
    int i2 = graph_search_vertex_by_id(graph, v2);

    if (i1 == -1 || i2 == -1) {
        printf("Vertice não encontrado\n");
        return;
    }

    aresta* runner;
    int cont = 0;
    for (runner = list_head(g->vertices[i1].arestas); runner != NULL; runner = list_next(runner)) {
        if (strcmp(runner->fim->id, v2) == 0) {
            list_remove(g->vertices[i1].arestas, cont);
            return;
        }
        cont++;
    }
}

bool graph_is_connected(Graph graph, char* v1, char* v2) {
    grafo* g = (grafo*)graph;
    int i1 = graph_search_vertex_by_id(graph, v1);
    int i2 = graph_search_vertex_by_id(graph, v2);

    if (i1 == -1 || i2 == -1) {
        printf("Vertice não encontrado\n");
        return false;
    }

    aresta* runner;
    for (runner = list_head(g->vertices[i1].arestas); runner != NULL; runner = list_next(runner)) {
        if (strcmp(runner->fim->id, v2) == 0) {
            return true;
        }
    }

    return false;
}

List graph_all_conections(Graph graph, char* v) {
    grafo* g = (grafo*)graph;
    int i = graph_search_vertex_by_id(graph, v);

    if (i == -1) {
        printf("Vertice não encontrado\n");
        return NULL;
    }

    return g->vertices[i].arestas;
}

void print_aresta_fim_id(void* edge) {
    aresta* a = (aresta*)edge;
    printf("%s", a->fim->id);
}

void graph_print(Graph graph) {
    grafo* g = (grafo*)graph;
    for (int i = 0; i < g->cont; i++) {
        printf("%s", g->vertices[i].id);
        list_print(g->vertices[i].arestas);
    }
}

double graph_compare_vertex(void* v1, void* v2) {
    vertice* V1 = (vertice*)v1;
    return !strcmp(V1->id, (char*)v2);
}

void print_vertex_id(void* v) {
    vertice* v1 = (vertice*)v;
    printf("%s", v1->id);
}

/*________________________________________djikstra IMPLEMENTATION________________________________________*/

typedef struct djikstra_node {
    char* id;
    char* from;
    double dist;
} djikstra_node;

void _destroy_djikstra_node(void* a) {
    djikstra_node* d = (djikstra_node*)a;
    free(d->id);
    free(d->from);
    free(d);
}

djikstra_node* make_node(char* id, char* from, double dist) {
    djikstra_node* n = malloc(sizeof(djikstra_node));
    n->id = id;
    n->from = from;
    n->dist = dist;
    return n;
}

double _graph_compare_node(void* n1, void* n2) {
    djikstra_node* v = n1;
    return !strcmp(v->id, (char*)n2);
}

void print_djikstra_node_id(void* n) {
    djikstra_node* n1 = (djikstra_node*)n;
    printf("%s", n1->id);
}

double graph_s_dist(Graph graph, char* v1, char* v2) {
    grafo* g = (grafo*)graph;

    int i1 = graph_search_vertex_by_id(graph, v1);
    int i2 = graph_search_vertex_by_id(graph, v2);

    if (i1 == -1 || i2 == -1) {
        printf("Vertice não encontrado\n");
        return -1;
    }

    void* runner;
    for (runner = list_head(g->vertices[i1].arestas); runner != NULL; runner = list_next(runner)) {
        aresta* aresta_runner = list_data(runner);
        if (aresta_runner->fim->id)
            if (!strcmp(aresta_runner->fim->id, v2)) {
                return aresta_runner->info.cmp;
            }
    }

    return -1;
}

List graph_shortest_path(Graph graph, char* v1, char* v2) {
    grafo* g = (grafo*)graph;
    P_Queue pq = new_pqueue(graph_compare_vertex);

    List visitados = new_list(_graph_compare_node, char_djikstra_id, _destroy_djikstra_node);

    int i1 = graph_search_vertex_by_id(graph, v1);
    int i2 = graph_search_vertex_by_id(graph, v2);

    vertice* vertex1 = &g->vertices[i1];
    vertice* vertex2 = &g->vertices[i2];

    list_insert(visitados, make_node(vertex1->id, NULL, 0));

    void* runner;
    for (runner = list_head(vertex1->arestas); runner != NULL; runner = list_next(runner)) {
        aresta* runner_aresta = list_data(runner);
        pqueue_add(pq, make_node(runner_aresta->fim->id, vertex1->id, graph_s_dist(graph, vertex1->id, runner_aresta->fim->id)), graph_s_dist(graph, vertex1->id, runner_aresta->fim->id));
    }

    djikstra_node* current = pqueue_pop(pq);

    while (strcmp(current->id, vertex2->id) != 0) {
        void* aux = g->vertices[graph_search_vertex_by_id(graph, current->id)].arestas;

        if (!list_search(visitados, current->id)) {
            for (runner = list_head(aux); runner != NULL; runner = list_next(runner)) {
                aresta* runner_aresta = list_data(runner);
                if (runner_aresta->fim->id && current->id)
                    pqueue_add(pq, make_node(runner_aresta->fim->id, current->id, graph_s_dist(graph, current->id, runner_aresta->fim->id) + current->dist), graph_s_dist(graph, current->id, runner_aresta->fim->id) + current->dist);
            }
            list_insert(visitados, current);
        }

        current = pqueue_pop(pq);

        if (!current) {
            return NULL;
        }
    }

    list_insert(visitados, current);

    List path = new_list(graph_compare_vertex, _graph_get_vertex_id, _destroy_vertex);

    while (current->from != NULL) {
        list_new_head(path, graph_get_vertex_i(graph, graph_search_vertex_by_id(graph, current->id)));
        current = list_data(list_search(visitados, current->from));
    }

    list_new_head(path, graph_get_vertex_i(graph, graph_search_vertex_by_id(graph, current->id)));

    return path;
}

char* char_djikstra_id(void* n) {
    djikstra_node* n1 = (djikstra_node*)n;
    return copyString(n1->id);
}
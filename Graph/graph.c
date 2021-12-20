#include "graph.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../../Outputs/svgWriter.h"
#include "../../Util/Utils.h"
#include "../DoublyLinkedList/DoublyLinkedList.h"
#include "../PriorityQueue/pqueue.h"
#include "../Stack/stack.h"
#include "../Tree/AvlTree.h"

enum sentido {
    RETO,
    DIREITA,
    ESQUERDA
};

typedef struct vertice {
    double x, y;
    char* id;
    List insidentes;
    List arestas;

    bool visit;
} vertice;

typedef struct dados {
    char* nome;  //Nome da rua a qual pertence o segmento
    char* ldir;  //Cep da quadra que está do lado direito do segmento de rua
    char* lesq;  //Idem para o lado esquerdo
    double cmp;  //comprimento(em metros) do segmento de rua
    double vm;   //Velocidade média (m/s) que os carros trafegam neste segmento de rua
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

char* _graph_get_edge_id(void* a) {
    aresta* e = (aresta*)a;
    char id[9999];
    sprintf(id, "Comprimento: %.5lf\tVelocidade: %.5lf\tLDIR: %s\tLESQ: %s\tNome: %s\t", e->info.cmp, e->info.vm, e->info.ldir, e->info.lesq, e->info.nome);
    return copyString(id);
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
    free(e->info.ldir);
    free(e->info.lesq);
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
    e->info.ldir = ldir;
    e->info.lesq = lesq;
    e->info.cmp = cmp;
    e->info.vm = vm;
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

void graph_edge_new_vm(Edge e, double new) {
    aresta* a = (aresta*)e;
    a->info.vm = new;
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

double graph_get_vm(Edge e) {
    aresta* a = (aresta*)e;
    return a->info.vm;
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

void graph_print_on_svg(Graph g, FILE* svg_file) {
    grafo* graph = (grafo*)g;

    for (int i = 0; i < graph->cont; i++) {
        vertice* v = &graph->vertices[i];
        fprintf(svg_file, "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%d\" fill=\"black\" />\n", v->x, v->y, 1);
    }

    for (int i = 0; i < graph->cont; i++) {
        vertice* v = &graph->vertices[i];

        for (void* runner = list_head(v->arestas); runner != NULL; runner = list_next(runner)) {
            char txt[500];
            aresta* a = ((aresta*)list_data(runner));
            sprintf(txt, "%.2lf", ((aresta*)list_data(runner))->info.cmp);
            if (a->ini->id && a->fim->id) {
                svg_arrow(svg_file, v->x, v->y, ((aresta*)list_data(runner))->fim->x, ((aresta*)list_data(runner))->fim->y, "Black", 8);
            }
        }
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

double graph_v_dist(Graph graph, char* v1, char* v2) {
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
                return aresta_runner->info.cmp / aresta_runner->info.vm;
            }
    }

    return -1;
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

List _djikstra(Graph graph, char* v1, char* v2, double (*graph_dist)(Graph graph, char* v1, char* v2));

List graph_fastest_path(Graph graph, char* v1, char* v2) {
    return _djikstra(graph, v1, v2, graph_v_dist);
}

List graph_shortest_path(Graph graph, char* v1, char* v2) {
    return _djikstra(graph, v1, v2, graph_s_dist);
}

int sentido_da_curva(Vertex inicio, Vertex atual, Vertex proximo);

void descricao_djikstra(FILE* txt_report, List lista_de_vertices) {
    for (void* runner = list_head(lista_de_vertices); runner != NULL; runner = list_next(runner)) {
        vertice* inicio = list_data(list_prev(runner));
        vertice* atual = list_data(runner);
        vertice* proximo = list_data(list_next(runner));

        if (inicio == NULL) {
            fprintf(txt_report, "Iniciando o trajeto na rua %s,\n\t", ((aresta*)list_data(list_head(atual->arestas)))->info.nome);
            continue;
        }

        if (proximo == NULL) {
            fprintf(txt_report, "você chegará ao seu destino na rua %s\n", ((aresta*)list_data(list_head(atual->arestas)))->info.nome);
        } else if (sentido_da_curva(inicio, atual, proximo) == DIREITA)
            fprintf(txt_report, "siga reto até o cruzamento com a rua %s,\n\tdepois vire à direita e ", ((aresta*)list_data(list_head(proximo->arestas)))->info.nome);
        else if (sentido_da_curva(inicio, atual, proximo) == ESQUERDA)
            fprintf(txt_report, "siga reto até o cruzamento com a rua %s,\n\tdepois vire à esquerda e ", ((aresta*)list_data(list_head(proximo->arestas)))->info.nome);
    }
}

List _djikstra(Graph graph, char* v1, char* v2, double (*graph_dist)(Graph graph, char* v1, char* v2)) {
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
        pqueue_add(pq, make_node(runner_aresta->fim->id, vertex1->id, graph_dist(graph, vertex1->id, runner_aresta->fim->id)), graph_dist(graph, vertex1->id, runner_aresta->fim->id));
    }

    djikstra_node* current = pqueue_pop(pq);

    while (strcmp(current->id, vertex2->id) != 0) {
        void* aux = g->vertices[graph_search_vertex_by_id(graph, current->id)].arestas;

        if (!list_search(visitados, current->id)) {
            for (runner = list_head(aux); runner != NULL; runner = list_next(runner)) {
                aresta* runner_aresta = list_data(runner);
                if (runner_aresta->fim->id && current->id)
                    pqueue_add(pq, make_node(runner_aresta->fim->id, current->id, graph_dist(graph, current->id, runner_aresta->fim->id) + current->dist), graph_dist(graph, current->id, runner_aresta->fim->id) + current->dist);
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


/*________________________________________kosaraju IMPLEMENTATION________________________________________*/

List graph_get_arestas(Vertex v) {
    vertice* v1 = (vertice*)v;
    return v1->arestas;
}

Graph traversal_graph(Graph graph, double limiar, FILE* svg) {
    grafo* g = (grafo*)graph;
    
    Graph g_new = new_graph(g->len);
    
    int i;
    
    for (i = 0; i < g->len; i++) {
        graph_add_vertex(g_new, g->vertices[i].id, g->vertices[i].x, g->vertices[i].y);
    }

    for (i = 0; i < g->len; i++) {
        void* runner;
        for (runner = list_head(g->vertices[i].arestas); runner != NULL; runner = list_next(runner)) {
            aresta* runner_aresta = list_data(runner);
            if (runner_aresta->info.vm > limiar)
                graph_add_edge(g_new, runner_aresta->fim->id, runner_aresta->ini->id, runner_aresta->info.lesq, runner_aresta->info.ldir, runner_aresta->info.nome, runner_aresta->info.cmp, runner_aresta->info.vm);
            else
                svg_line(svg, runner_aresta->ini->x, runner_aresta->ini->y, runner_aresta->fim->x, runner_aresta->fim->y, "Red", 4);
        }
    }

    return g_new;
}

void kosaraju_dfs_stack(vertice* current, Stack stack, double limiar, FILE* under_svg) {
    if (current->visit)
        return;

    current->visit = true;

    for (void* runner = list_head(current->arestas); runner != NULL; runner = list_next(runner)) {
        if (((aresta*)list_data(runner))->info.vm > limiar) {
            if (((aresta*)list_data(runner))->fim->visit == false)
                kosaraju_dfs_stack(((aresta*)list_data(runner))->fim, stack, limiar, under_svg);
        } else {
            svg_line(under_svg, ((aresta*)list_data(runner))->ini->x, ((aresta*)list_data(runner))->ini->y, ((aresta*)list_data(runner))->fim->x, ((aresta*)list_data(runner))->fim->y, "Red", 4);
        }
    }

    stack_add(stack, current);
}

void kosaraju_regions(FILE* under_svg, vertice* current, List region, double limiar) {
    current->visit = true;

    for (void* runner = list_head(current->arestas); runner != NULL; runner = list_next(runner)) {
        aresta* a = list_data(runner);

        if (a->info.vm > limiar) {
            if (a->fim->visit == false)
                kosaraju_regions(under_svg, a->fim, region, limiar);
        } 
    }

    list_insert(region, current);
}

List kosaraju(Graph graph, double limiar, FILE* under_svg) {
    grafo* g = (grafo*)graph;
    Stack stack = new_stack();

    for (int i = 0; i < g->len; i++) {
        kosaraju_dfs_stack(&g->vertices[i], stack, limiar, under_svg);
    }

    grafo* traversalGraph = traversal_graph(graph, limiar, under_svg);

    List floresta = new_list(not_compare, not_to_string, not_free);

    set_graph_visit_false(traversalGraph);
   
    for (vertice* stack_node = stack_pop(stack); stack_node; stack_node = stack_pop(stack)) {
       
        vertice* traversal_node = &(traversalGraph->vertices[graph_search_vertex_by_id(traversalGraph, stack_node->id)]);

        if (traversal_node->visit)
            continue;

        List region = new_list(not_compare, not_to_string, not_free);

        list_insert(region, traversal_node);
        traversal_node->visit = true;

        for (void* runner = list_head(traversal_node->arestas); runner != NULL; runner = list_next(runner)) {
           
            if (((aresta*)list_data(runner))->info.vm > limiar) {
                if (((aresta*)list_data(runner))->fim->visit == false)
                    kosaraju_regions(under_svg, ((aresta*)list_data(runner))->fim, region, limiar);
            }
        }

        list_insert(floresta, region);
    }

    return floresta;
}

int sentido_da_curva(Vertex inicio, Vertex atual, Vertex proximo) {
    vertice* v1 = (vertice*)inicio;
    vertice* v2 = (vertice*)atual;
    vertice* v3 = (vertice*)proximo;

    double x1 = 0;
    double y1 = 0;
    double x2 = 0;
    double y2 = 0;
    double x3 = 0;
    double y3 = 0;

    if (v1) {
        x1 = v1->x;
        y1 = v1->y;
    }

    if (v2) {
        x2 = v2->x;
        y2 = v2->y;
    }

    if (v3) {
        x3 = v3->x;
        y3 = v3->y;
    }

    double m1 = (y2 - y1) / (x2 - x1);
    double m2 = (y3 - y2) / (x3 - x2);

    if (m1 == m2) {
        return RETO;
    }

    if (m1 > m2) {
        return DIREITA;
    }

    return ESQUERDA;
}

void svg_make_path(FILE* file, char* name, List vertex_list, char* color, double g) {
    fprintf(file, "\t<path d=\"");
    void* runner;
    for (runner = list_head(vertex_list); runner != NULL; runner = list_next(runner)) {
        vertice* v = list_data(runner);
        fprintf(file, "M %f %f ", v->x, v->y);
        if (list_next(runner)) {
            vertice* dest = list_data(list_next(runner));
            fprintf(file, "L %f %f ", dest->x, dest->y);
        }
    }
    fprintf(file, "\" stroke=\"%s\" stroke-width=\"%lf\" fill=\"none\" id=\"%s\"/>\n", color, g, name);
}

void graph_remove_index(Graph graph, int index) {
    grafo* g = (grafo*)graph;
    if (index < 0 || index >= g->len) {
        printf("Índice inválido: %d|%ld\n", index, g->len);
        return;
    }
    list_destroy(g->vertices[index].arestas);
    g->vertices[index].arestas = NULL;
    g->vertices[index].id = NULL;
    g->vertices[index].x = 0;
    g->vertices[index].y = 0;
}

void graph_destroy(Graph graph) {
    grafo* g = (grafo*)graph;
    int i;
    for (i = 0; i < g->len; i++) {
        list_destroy(g->vertices[i].arestas);
    }
    free(g->vertices);
    free(g);
}

void print_aresta(void* Aresta) {
    aresta* a = (aresta*)Aresta;
    printf("%s\n", a->fim->id);
}

double graph_compare_aresta(void* a, void* b) {
    aresta* a1 = (aresta*)a;
    aresta* a2 = (aresta*)b;
    return a1->info.cmp - a2->info.cmp;
}

/*________________________________________KRUSKAL IMPLEMENTATION________________________________________*/

typedef struct kruskal_node {
    vertice* info;
    int rank;
    int parent;
} kruskal_node;

kruskal_node* _make_kruskal_node(int parent, void* info) {
    kruskal_node* node = malloc(sizeof(kruskal_node));
    node->info = info;
    node->rank = 0;
    node->parent = parent;
    return node;
}

int _kruskal_find(kruskal_node* nodes, int i) {
    if (nodes[i].parent == i)
        return i;
    return _kruskal_find(nodes, nodes[i].parent);
}

int kruskal_id_index(kruskal_node* groups, char* id, int len) {
    int i;
    for (i = 0; i < len; i++) {
        if (!strcmp(groups[i].info->id, id)) {
            return i;
        }
    }
    return -1;
}

void _kruskal_union(kruskal_node* nodes, int i, int j, int len) {
    int i_root = _kruskal_find(nodes, i);
    int j_root = _kruskal_find(nodes, j);
    if (nodes[i_root].rank < nodes[j_root].rank) {
        nodes[i_root].parent = j_root;
    } else if (nodes[i_root].rank > nodes[j_root].rank) {
        nodes[j_root].parent = i_root;
    } else {
        nodes[j_root].parent = i_root;
        nodes[i_root].rank++;
    }
}

List all_vertex_list(List list_edge) {
    List list_vertex = new_list(graph_compare_vertex, _graph_get_vertex_id, _destroy_vertex);
    void* runner;
    for (runner = list_head(list_edge); runner != NULL; runner = list_next(runner)) {
        aresta* runner_aresta = list_data(runner);
        list_new_head(list_vertex, runner_aresta->ini);
        list_new_head(list_vertex, runner_aresta->fim);
    }
    return list_vertex;
}

kruskal_node* vertex_list_to_array(List vertex_sort_list) {
    kruskal_node* vertex_array = malloc(sizeof(kruskal_node) * list_len(vertex_sort_list));
    void* runner;
    int i = 0;
    for (runner = list_head(vertex_sort_list); runner != NULL; runner = list_next(runner)) {
        vertice* runner_vertex = list_data(runner);
        vertex_array[i] = *_make_kruskal_node(i, runner_vertex);
        i++;
    }
    return vertex_array;
}

//Remove os vértices duplicados de uma lista ordenada
List remove_duplicate_vertex(List vertex_list) {
    List vertex_list_new = new_list(graph_compare_vertex, _graph_get_vertex_id, _destroy_vertex);
    void* runner;
    for (runner = list_head(vertex_list); runner != NULL; runner = list_next(runner)) {
        vertice* runner_vertex = list_data(runner);
        if (!list_search(vertex_list_new, runner_vertex->id)) {
            list_new_head(vertex_list_new, runner_vertex);
        }
    }
    return vertex_list_new;
}

List edge_list(Graph graph) {
    grafo* g = (grafo*)graph;
    List lista_arestas = new_list(graph_compare_aresta, not_to_string, not_free);

    int i;

    for (i = 0; i < g->len; i++) {
        for (void* runner = list_head(g->vertices[i].arestas); runner != NULL; runner = list_next(runner)) {
            list_insert(lista_arestas, list_data(runner));
        }
    }

    return lista_arestas;
}

double _organize_compare(void* a, void* b) {
    aresta* a1 = (aresta*)a;
    aresta* a2 = (aresta*)b;
    return !strcmp(a1->ini->id, a2->ini->id) || !strcmp(a1->ini->id, a2->fim->id) || !strcmp(a1->fim->id, a2->ini->id) || !strcmp(a1->fim->id, a2->fim->id);
}

void** graph_kruskal(List lista_arestas) {
    List vertex_sort_list = all_vertex_list(lista_arestas);
    list_sort(vertex_sort_list);
    list_sort(lista_arestas);
    vertex_sort_list = remove_duplicate_vertex(vertex_sort_list);
    kruskal_node* groups = vertex_list_to_array(vertex_sort_list);

    for (int i = 0; i < list_len(vertex_sort_list); i++) {
        groups[i] = *_make_kruskal_node(i, list_data(list_index(vertex_sort_list, i)));
    }

    List result = new_list(not_compare, not_to_string, not_free);

    for (void* runner = list_head(lista_arestas); runner != NULL; runner = list_next(runner)) {
        aresta* runner_aresta = list_data(runner);

        int i = _kruskal_find(groups, kruskal_id_index(groups, runner_aresta->ini->id, list_len(vertex_sort_list)));
        int j = _kruskal_find(groups, kruskal_id_index(groups, runner_aresta->fim->id, list_len(vertex_sort_list)));

        if (i != j) {
            list_insert(result, runner_aresta);
            _kruskal_union(groups, i, j, list_len(vertex_sort_list));
        } else if (runner_aresta->ini->id == runner_aresta->fim->id) {
            list_insert(result, runner_aresta);
        }
    }

    void** msts = calloc(list_len(result), sizeof(List));

    for (void* runner = list_head(result); runner != NULL; runner = list_next(runner)) {
        for (int i = 0; i < list_len(result); i++) {
            aresta* a1;
            aresta* a2 = list_data(runner);

            int ai1;
            int ai2 = kruskal_id_index(groups, a2->ini->id, list_len(vertex_sort_list));

            if (!msts[i]) {
                msts[i] = new_list(_organize_compare, _print_edge_vertexes_ids, not_free);
                list_insert(msts[i], _create_edge(NULL, NULL, NULL, 0, 0, _create_vertex(groups[groups[ai2].parent].info->id, 0, 0), _create_vertex(groups[groups[ai2].parent].info->id, 0, 0)));
                list_insert(msts[i], list_data(runner));
                break;
            }

            a1 = list_data(list_head(msts[i]));
            ai1 = kruskal_id_index(groups, a1->ini->id, list_len(vertex_sort_list));

            if (_kruskal_find(groups, ai1) == _kruskal_find(groups, ai2)) {
                list_insert(msts[i], list_data(runner));
                break;
            }
        }
    }

    free(groups);
    list_destroy(lista_arestas);

    return msts;
}

char* vertex_to_string(void* vertex) {
    vertice* v = (vertice*)vertex;
    char* str = calloc(100, sizeof(char));
    sprintf(str, "[%.0lf:%.0lf]", v->x, v->y);
    return str;
}

Vertex graph_get_vertex_i(Graph graph, int i) {
    grafo* g = (grafo*)graph;
    return &g->vertices[i];
}

vertice* compare_vertex;

void set_compare_vertex(double x, double y) {
    compare_vertex = _create_vertex(NULL, x, y);
}

double vertex_get_x(void* vertex) {
    vertice* v = (vertice*)vertex;
    return v->x;
}

double vertex_get_y(void* vertex) {
    vertice* v = (vertice*)vertex;
    return v->y;
}

//comapara a dintancia entre dois vertices por teorema de pitagoras
double _distance_between_vertex(void* a, double x, double y) {
    vertice* a1 = (vertice*)a;

    //double d2 rece a distancia de v2 a compare_vertex
    return sqrt(pow(a1->x - x, 2) + pow(a1->y - y, 2));
}

//função get edge por v1 e v2
Edge get_edge_by_vertex(Graph graph, char* V1, char* V2) {
    grafo* g = (grafo*)graph;
    vertice* v1 = &g->vertices[graph_search_vertex_by_id(graph, V1)];
    vertice* v2 = &g->vertices[graph_search_vertex_by_id(graph, V2)];
    for (int i = 0; i < g->len; i++) {
        if (!strcmp(g->vertices[i].id, v1->id)) {
            for (void* runner = list_head(g->vertices[i].arestas); runner != NULL; runner = list_next(runner)) {
                aresta* runner_aresta = list_data(runner);
                if (!strcmp(runner_aresta->ini->id, v2->id) || !strcmp(runner_aresta->fim->id, v2->id)) {
                    return runner_aresta;
                }
            }
        }
    }
    return NULL;
}
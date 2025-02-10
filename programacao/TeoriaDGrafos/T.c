#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Estrutura para representar um nó na lista de adjacências
struct Node {
    int vertex;
    struct Node* next;
};

// Estrutura para representar o grafo
struct Graph {
    int numVertices;
    struct Node** adjLists;
    int* visited;
    int* parent;
};

// Função para criar um nó
struct Node* createNode(int v) {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

// Função para criar o grafo
struct Graph* createGraph(int vertices) {
    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->numVertices = vertices;

    graph->adjLists = malloc(vertices * sizeof(struct Node*));
    graph->visited = malloc(vertices * sizeof(int));
    graph->parent = malloc(vertices * sizeof(int));

    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;
        graph->parent[i] = -1;
    }

    return graph;
}

// Função para adicionar uma aresta ao grafo
void addEdge(struct Graph* graph, int src, int dest) {
    // Adiciona uma aresta de src para dest
    struct Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    // Adiciona uma aresta de dest para src (grafo não direcionado)
    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

// Função para realizar a busca em largura (BFS)
void BFS(struct Graph* graph, int startVertex, int endVertex) {
    int queue[graph->numVertices];
    int front = 0, rear = 0;

    graph->visited[startVertex] = 1;
    queue[rear++] = startVertex;

    while (front < rear) {
        int currentVertex = queue[front++];
        printf("Visitando vértice %d\n", currentVertex);

        struct Node* temp = graph->adjLists[currentVertex];

        while (temp) {
            int adjVertex = temp->vertex;

            if (graph->visited[adjVertex] == 0) {
                graph->visited[adjVertex] = 1;
                graph->parent[adjVertex] = currentVertex;
                queue[rear++] = adjVertex;

                if (adjVertex == endVertex) {
                    printf("Caminho encontrado!\n");
                    return;
                }
            }
            temp = temp->next;
        }
    }
    printf("Caminho não encontrado!\n");
}

// Função para imprimir o caminho encontrado
void printPath(struct Graph* graph, int startVertex, int endVertex) {
    if (endVertex == -1) {
        printf("%d ", startVertex);
        return;
    }
    printPath(graph, startVertex, graph->parent[endVertex]);
    printf("%d ", endVertex);
}

int main() {
    int numVertices = 6;
    struct Graph* graph = createGraph(numVertices);

    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 2, 3);
    addEdge(graph, 3, 4);
    addEdge(graph, 4, 5);

    int startVertex = 0;
    int endVertex = 5;

    BFS(graph, startVertex, endVertex);

    if (graph->visited[endVertex]) {
        printf("Caminho de %d para %d: ", startVertex, endVertex);
        printPath(graph, startVertex, endVertex);
        printf("\n");
    } else {
        printf("Não há caminho de %d para %d\n", startVertex, endVertex);
    }

    // Libera a memória alocada
    for (int i = 0; i < numVertices; i++) {
        struct Node* temp = graph->adjLists[i];
        while (temp) {
            struct Node* next = temp->next;
            free(temp);
            temp = next;
        }
    }
    free(graph->adjLists);
    free(graph->visited);
    free(graph->parent);
    free(graph);

    return 0;
}
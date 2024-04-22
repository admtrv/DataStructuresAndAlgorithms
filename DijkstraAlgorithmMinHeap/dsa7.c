#include <stdio.h>
#include <stdlib.h>

#define INT_MAX 2147483647

typedef struct Edge {
    int destination;
    int weight;
    struct Edge *next;
} Edge;

typedef struct Graph {
    int size;
    Edge **edges;
} Graph;

void initGraph(Graph *graph, int size)
{
    graph->size = size;

    graph->edges = (Edge**)malloc(size * sizeof(Edge*));
    for (int i = 0; i < size; i++)
    {
        graph->edges[i] = NULL;
    }
}

void freeGraph(Graph *graph)
{
    for (int i = 0; i < graph->size; i++)
    {
        Edge *current = graph->edges[i];
        while (current != NULL)
        {
            Edge *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph->edges);
}

void insert(Graph *graph, int from, int to, int weight, int *isFirstPrint)
{
    if (from == to)
    {
        if (*isFirstPrint)
            *isFirstPrint = 0;
        else
            printf("\n");

        printf("insert %d %d failed", from, to);
        return;
    }

    for (Edge* current = graph->edges[from]; current != NULL; current = current->next)
    {
        if (current->destination == to)
        {
            if (*isFirstPrint)
                *isFirstPrint = 0;
            else
                printf("\n");

            printf("insert %d %d failed", from, to);
            return;
        }
    }

    Edge *edge = (Edge*)malloc(sizeof(Edge));
    edge->destination = to;
    edge->weight = weight;
    edge->next = graph->edges[from];
    graph->edges[from] = edge;

    edge = (Edge*)malloc(sizeof(Edge));
    edge->destination = from;
    edge->weight = weight;
    edge->next = graph->edges[to];
    graph->edges[to] = edge;
}

void delete(Graph *graph, int from, int to, int *isFirstPrint)
{
    if (from == to)
    {
        if (*isFirstPrint)
            *isFirstPrint = 0;
        else
            printf("\n");

        printf("delete %d %d failed", from, to);
        return;
    }
    int found = 0;

    Edge **current = &graph->edges[from];
    while (*current)
    {
        Edge *entry = *current;

        if (entry->destination == to)
        {
            *current = entry->next;
            free(entry);
            found = 1;
            break;
        }
        else
        {
            current = &entry->next;
        }
    }

    if(!found)
    {
        if (*isFirstPrint)
            *isFirstPrint = 0;
        else
            printf("\n");

        printf("delete %d %d failed", from, to);
        return;
    }

    current = &graph->edges[to];
    while (*current)
    {
        Edge *entry = *current;
        if (entry->destination == from)
        {
            *current = entry->next;
            free(entry);
            break;
        }
        else
        {
            current = &entry->next;
        }
    }
}

void update(Graph *graph, int from, int to, int deltaWeight, int *isFirstPrint)
{
    if (from == to)
    {
        if (*isFirstPrint)
            *isFirstPrint = 0;
        else
            printf("\n");

        printf("update %d %d failed", from, to);
        return;
    }

    int found = 0;
    for (Edge *current = graph->edges[from]; current != NULL; current = current->next) {
        if (current->destination == to) {
            if (current->weight + deltaWeight < 0) {
                if (*isFirstPrint)
                    *isFirstPrint = 0;
                else
                    printf("\n");
                printf("update %d %d failed", from, to);
                return;
            }
            current->weight += deltaWeight;
            found = 1;
            break;
        }
    }

    if (!found)
    {
        if (*isFirstPrint)
            *isFirstPrint = 0;
        else
            printf("\n");
        printf("update %d %d failed", from, to);
        return;
    }

    for (Edge *current = graph->edges[to]; current != NULL; current = current->next)
    {
        if (current->destination == from)
        {
            current->weight += deltaWeight;
        }
    }
}

typedef struct Node {
    int vertex;
    int distance;
} Node;

typedef struct MinHeap {
    int size;
    int *positions;
    int *parents;
    Node *nodes;

} MinHeap;

MinHeap* initMinHeap(int size) {

    MinHeap* minHeap =(MinHeap*)malloc(sizeof(MinHeap));
    minHeap->positions = (int*)malloc(size * sizeof(int));
    minHeap->parents = (int*)malloc(size * sizeof(int));
    minHeap->size= 0;
    minHeap->nodes = (Node*)malloc(size * sizeof(Node*));
    return minHeap;
}

void freeMinHeap(MinHeap* minHeap)
{
    free(minHeap->nodes);
    free(minHeap->positions);
    free(minHeap->parents);
    free(minHeap);
}

void swapHeapNode(Node * a, Node * b)
{
    Node t = *a;
    *a = *b;
    *b = t;
}

void minHeapifyAlg(MinHeap *minHeap, int i)
{
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < minHeap->size && minHeap->nodes[left].distance <= minHeap->nodes[i].distance)
        smallest = left;

    if (right < minHeap->size && minHeap->nodes[right].distance <= minHeap->nodes[smallest].distance)
        smallest = right;

    if (smallest != i)
    {
        Node smallestNode = minHeap->nodes[smallest];
        Node iNode = minHeap->nodes[i];

        minHeap->positions[smallestNode.vertex] = i;
        minHeap->positions[iNode.vertex] = smallest;

        swapHeapNode( &minHeap->nodes[i],  &minHeap->nodes[smallest]);

        minHeapifyAlg(minHeap, smallest);
    }
}

Node extractMinNode(MinHeap *minHeap)
{
    if (minHeap->size <= 0)
        return (Node){-1, INT_MAX};

    Node root = minHeap->nodes[0];

    Node lastNode = minHeap->nodes[minHeap->size - 1];
    minHeap->nodes[0] = lastNode;

    minHeap->positions[root.vertex] = minHeap->size - 1;
    minHeap->positions[lastNode.vertex] = 0;

    minHeap->size -= 1;
    minHeapifyAlg(minHeap, 0);

    return root;
}

void updateVertex(MinHeap *minHeap, int vertex, int distance, int parent)
{
    int i = minHeap->positions[vertex];

    minHeap->nodes[i].distance = distance;
    minHeap->parents[vertex] = parent;
    while (i != 0 && minHeap->nodes[i].distance <= minHeap->nodes[(int)((i - 1) / 2)].distance)
    {
        minHeap->positions[minHeap->nodes[i].vertex] = (int)(i - 1) / 2;
        minHeap->positions[minHeap->nodes[(int)(i - 1) / 2].vertex] = i;
        swapHeapNode( &minHeap->nodes[i], &minHeap->nodes[(int) ((i - 1) / 2)]);

        i = (int)((i - 1) / 2);
    }
}

void printMinHeap(MinHeap *minHeap) {
    printf("\nMinHeap: [");
    for (int i = 0; i < minHeap->size; i++) {
        if (i > 0) {
            printf(", ");
        }
        printf("(%d, %d, %d)", minHeap->nodes[i].vertex, minHeap->nodes[i].distance ,minHeap->positions[i]);
    }
    printf("]");
}

void dijkstra(Graph *graph, int start, int end, int *isFirstPrint)
{

    if (start == end)
    {
        if (*isFirstPrint)
            *isFirstPrint = 0;
        else
            printf("\n");

        printf("search failed");
        return;
    }

    int size = graph->size;
    int *distances = (int*)malloc(size * sizeof(int));
    int *visited = (int*)malloc(size * sizeof(int));
    MinHeap *minHeap = initMinHeap(size);

    for (int i = 0; i < size; i++)
    {
        distances[i] = INT_MAX;
        minHeap->nodes[i].vertex = i;
        minHeap->nodes[i].distance = distances[i];
        minHeap->positions[i] = i;
        minHeap->parents[i] = -1;
        visited[i] = 0;
    }

    minHeap->nodes[start].vertex = start;
    minHeap->nodes[start].distance = distances[start];
    minHeap->positions[start] = start;

    distances[start] = 0;
    updateVertex(minHeap, start, distances[start], -1);

    minHeap->size = size;
    while (minHeap->size != 0)
    {
        Node minHeapNode = extractMinNode(minHeap);

        int u = minHeapNode.vertex;

        visited[u] = 1;

        Edge* adjEdge = graph->edges[u];
        while (adjEdge != NULL)
        {
            int v = adjEdge->destination;

            if (!visited[v] && distances[u] != INT_MAX && adjEdge->weight + distances[u] < distances[v])
            {
                distances[v] = distances[u] + adjEdge->weight;

                updateVertex(minHeap, v, distances[v], u);
            }

            adjEdge = adjEdge->next;
        }

    }

    if (*isFirstPrint)
        *isFirstPrint = 0;
    else
        printf("\n");

    if (distances[end] == INT_MAX) {
        printf("search failed");
        freeMinHeap(minHeap);
        free(distances);
        free(visited);
        return;
    }

    printf("%d: [", distances[end]);
    int path[graph->size];
    int pathSize = 0;

    for (int i = end; i != -1; i = minHeap->parents[i])
    {
        path[pathSize] = i;
        pathSize+=1;
    }

    for (int i = pathSize - 1; i >= 0; i--)
    {
        printf("%d", path[i]);
        if (i > 0)
        {
            printf(", ");
        }
    }
    printf("]");

    free(distances);
    free(visited);
    freeMinHeap(minHeap);
}


int main()
{
    int n = 0;
    int m = 0;
    scanf("%d %d", &n, &m);

    Graph graph;
    initGraph(&graph, n);
    int isFirstPrint = 1;

    for (int i = 0; i < m; i++)
    {
        int v1 = 0;
        int v2 = 0;
        int weight = 0;
        scanf(" (%d, %d, %d)", &v1, &v2, &weight);
        insert(&graph, v1, v2, weight, &isFirstPrint);
    }

    char command;
    while (scanf("%c", &command) == 1)
    {
        int v1, v2, weight;
        switch (command)
        {
            case 'i':
                scanf(" %d %d %d", &v1, &v2, &weight);
                insert(&graph, v1, v2, weight, &isFirstPrint);
                break;
            case 'd':
                scanf(" %d %d", &v1, &v2);
                delete(&graph, v1, v2, &isFirstPrint);
                break;
            case 'u':
                scanf(" %d %d %d", &v1, &v2, &weight);
                update(&graph, v1, v2, weight, &isFirstPrint);
                break;
            case 's':
                scanf(" %d %d", &v1, &v2);
                dijkstra(&graph, v1, v2, &isFirstPrint);
                break;
        }
    }

    freeGraph(&graph);

    return 0;
}

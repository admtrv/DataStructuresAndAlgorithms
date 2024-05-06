#include <stdio.h>
#include <stdlib.h>

#define LL_MAX 9223372036854775807

typedef struct Edge {
    int destination;
    long long weight;
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

void insert(Graph *graph, int from, int to, long long  weight, int *isFirstPrint)
{

    if (from < 0 || from >= graph->size)
    {
        if (*isFirstPrint)
            *isFirstPrint = 0;
        else
            printf("\n");

        printf("insert %d %d failed", from, to);
        return;
    }

    if (to < 0 || to >= graph->size)
    {
        if (*isFirstPrint)
            *isFirstPrint = 0;
        else
            printf("\n");

        printf("insert %d %d failed", from, to);
        return;
    }

    if (from == to)
    {
        if (*isFirstPrint)
            *isFirstPrint = 0;
        else
            printf("\n");

        printf("insert %d %d failed", from, to);
        return;
    }

    if(weight < 0)
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

    // Добавляем ребро в обратном направлении
    edge = (Edge*)malloc(sizeof(Edge));
    edge->destination = from;
    edge->weight = weight;
    edge->next = graph->edges[to];
    graph->edges[to] = edge;
}

void delete(Graph *graph, int from, int to, int *isFirstPrint)
{
    if (from < 0 || from >= graph->size)
    {
        if (*isFirstPrint)
            *isFirstPrint = 0;
        else
            printf("\n");

        printf("delete %d %d failed", from, to);
        return;
    }

    if (to < 0 || to >= graph->size)
    {
        if (*isFirstPrint)
            *isFirstPrint = 0;
        else
            printf("\n");

        printf("delete %d %d failed", from, to);
        return;
    }

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

void update(Graph *graph, int from, int to, long long  deltaWeight, int *isFirstPrint)
{
    if (from < 0 || from >= graph->size)
    {
        if (*isFirstPrint)
            *isFirstPrint = 0;
        else
            printf("\n");

        printf("update %d %d failed", from, to);
        return;
    }

    if (to < 0 || to >= graph->size)
    {
        if (*isFirstPrint)
            *isFirstPrint = 0;
        else
            printf("\n");

        printf("update %d %d failed", from, to);
        return;
    }

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
    for (Edge *current = graph->edges[from]; current != NULL; current = current->next) 
    {
        if (current->destination == to) 
        {
            if (current->weight + deltaWeight < 0)
            {
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
    long long distance;  
} Node;

typedef struct MinHeap {
    int size;         
    int *positions;    
    Node *nodes;     

} MinHeap;

MinHeap* initMinHeap(int size)
{
    MinHeap* minHeap =(MinHeap*)malloc(sizeof(MinHeap));
    minHeap->size= 0;
    minHeap->nodes = (Node*)malloc(size * sizeof(Node));
    minHeap->positions = (int*)malloc(size * sizeof(int));
    return minHeap;
}

void freeMinHeap(MinHeap* minHeap)
{
    free(minHeap->nodes);
    free(minHeap->positions);
    free(minHeap);
}

void swapNode(Node * a, Node * b)
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

        swapNode(&minHeap->nodes[i], &minHeap->nodes[smallest]);

        minHeapifyAlg(minHeap, smallest);
    }
}

Node extractMinNode(MinHeap *minHeap)
{
    if (minHeap->size <= 0)
        return (Node){-1, LL_MAX};

    Node root = minHeap->nodes[0];
    Node lastNode = minHeap->nodes[minHeap->size - 1];
    minHeap->nodes[0] = lastNode;
    minHeap->positions[root.vertex] = -1; 
    minHeap->positions[lastNode.vertex] = 0;

    minHeap->size -= 1;
    minHeapifyAlg(minHeap, 0);

    return root;
}


void updateVertex(MinHeap *minHeap, int vertex, long long distance)
{
    int i = minHeap->positions[vertex];

    minHeap->nodes[i].distance = distance;

    while (i != 0 && minHeap->nodes[i].distance <= minHeap->nodes[(int)((i - 1) / 2)].distance)
    {
        minHeap->positions[minHeap->nodes[i].vertex] = (int)(i - 1) / 2;
        minHeap->positions[minHeap->nodes[(int)(i - 1) / 2].vertex] = i;
        swapNode(&minHeap->nodes[i], &minHeap->nodes[(int) ((i - 1) / 2)]);

        i = (int)((i - 1) / 2);
    }
}

void insertNode(MinHeap *minHeap, int vertex, long long distance)
{
    
    int size = minHeap->size;
    minHeap->size+=1;

    int i = size;

    minHeap->nodes[i].vertex = vertex;
    minHeap->nodes[i].distance = distance;
    minHeap->positions[vertex] = i;

    updateVertex(minHeap, vertex, distance);
}

void merge(int array[][2], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int leftArray[n1][2];
    int rightArray[n2][2];

    for (int i = 0; i < n1; i++)
    {
        leftArray[i][0] = array[left + i][0];
        leftArray[i][1] = array[left + i][1];
    }

    for (int j = 0; j < n2; j++)
    {
        rightArray[j][0] = array[mid + 1 + j][0];
        rightArray[j][1] = array[mid + 1 + j][1];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2)
    {
        if (leftArray[i][0] < rightArray[j][0] || (leftArray[i][0] == rightArray[j][0] && leftArray[i][1] <= rightArray[j][1]))
        {
            array[k][0] = leftArray[i][0];
            array[k][1] = leftArray[i][1];
            i+=1;
        }
        else
        {
            array[k][0] = rightArray[j][0];
            array[k][1] = rightArray[j][1];
            j+=1;
        }
        k+=1;
    }

    // rest are coping
    while (i < n1)
    {
        array[k][0] = leftArray[i][0];
        array[k][1] = leftArray[i][1];
        i+=1;
        k+=1;
    }

    while (j < n2)
    {
        array[k][0] = rightArray[j][0];
        array[k][1] = rightArray[j][1];
        j+=1;
        k+=1;
    }
}

void mergeSort(int array[][2], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergeSort(array, left, mid);
        mergeSort(array, mid + 1, right);

        merge(array, left, mid, right);
    }
}

void prim(Graph *graph, int start, int *isFirstPrint)
{

    if (!graph->edges)
    {
        if (*isFirstPrint)
            *isFirstPrint = 0;
        else
            printf("\n");
        printf("search %d failed", start);
        return;
    }

    if (start < 0 || start >= graph->size)
    {
        if (*isFirstPrint)
            *isFirstPrint = 0;
        else
            printf("\n");
        printf("search %d failed", start);
        return;
    }

    if (graph->edges[start] == NULL)
    {
        if (*isFirstPrint)
            *isFirstPrint = 0;
        else
            printf("\n");
        printf("search %d failed", start);
        return;
    }

    int size = graph->size;
    long long *distances = malloc(size * sizeof(long long));
    int *parents = (int*)malloc(size * sizeof(int));
    int *visited = (int*)calloc(size, sizeof(int)); 

    int (*msp)[2] = malloc(size * sizeof(int[2])); // [child, parent]
    long long total = 0;
    int num = 0;

    MinHeap *minHeap = initMinHeap(size);

    for (int i = 0; i < size; i++) {
        distances[i] = LL_MAX;
        minHeap->positions[i] = -1; 
        parents[i] = -1;
    }
    
    distances[start] = 0;
    insertNode(minHeap, start, distances[start]);
    
    while (minHeap->size > 0 && num < size)
    {
        Node minHeapNode = extractMinNode(minHeap);
        int u = minHeapNode.vertex;
        
        if (visited[u])
            continue;

        if (parents[u] != -1)
        {
            if (u > parents[u])
            {
                msp[num][0] = parents[u];
                msp[num][1] = u;
            }
            else
            {
                msp[num][0] = u;
                msp[num][1] = parents[u];
            }
            total+= distances[u];
            num+=1;
        }

        visited[u] = 1;

        Edge* adjEdge = graph->edges[u];

        while (adjEdge != NULL)
        {
            int v = adjEdge->destination;

            if (!visited[v] && adjEdge->weight < distances[v])
            {
                distances[v] = adjEdge->weight;
                parents[v] = u;

                if (minHeap->positions[v] == -1)
                    insertNode(minHeap, v, distances[v]);
                else
                    updateVertex(minHeap, v, distances[v]);
            }
            adjEdge = adjEdge->next;
        }
    }

    mergeSort(msp, 0, num - 1);
    
    if (*isFirstPrint)
        *isFirstPrint = 0;
    else
        printf("\n");

    if (num == 0)
    {
        printf("search %d failed", start);
        return;
    }

    printf("%llu: [", total);
    for (int i = 0; i < num; ++i)
    {
        if (i > 0)
            printf(", ");
        printf("(%d, %d)", msp[i][0], msp[i][1]);
    }
    printf("]");


    free(distances);
    free(parents);
    free(visited);
    free(msp);
    freeMinHeap(minHeap);
}


int main()
{
    int n = 0;
    scanf("%d ", &n);

    Graph graph;
    initGraph(&graph, n);

    int isFirstPrint = 1;

    int v1 = 0;
    int v2 = 0;
    long long weight = 0;

    char line[256];
    while (fgets(line, sizeof(line), stdin))
    {
        if (line[0] == 'i')
        {
            sscanf(line, "i %d %d %lld", &v1, &v2, &weight);
            insert(&graph, v1, v2, weight, &isFirstPrint);
        }
        else if (line[0] == 'd')
        {
            sscanf(line, "d %d %d", &v1, &v2);
            delete(&graph, v1, v2, &isFirstPrint);
        }
        else if (line[0] == 'u')
        {
            sscanf(line, "u %d %d %lld", &v1, &v2, &weight);
            update(&graph, v1, v2, weight, &isFirstPrint);
        }
        else if (line[0] == 's')
        {
            sscanf(line, "s %d", &v1);
            prim(&graph, v1, &isFirstPrint);
        }
        else
        {
            sscanf(line, "(%d, %d, %lld)", &v1, &v2, &weight);
            insert(&graph, v1, v2, weight, &isFirstPrint);
        }
    }

    freeGraph(&graph);

    return 0;
}

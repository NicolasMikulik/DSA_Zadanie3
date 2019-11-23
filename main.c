// zadanie3.c -- Nicolas Mikulík, 11.11.2019 08:12

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#define D 8
#define P3 4
#define P2 2
#define P1 1

typedef struct {
    int x; // 4 B
    int y; // 4 B
    int nieco; // 4 B
    short niecodalsie; // 2 B
    void *nejakypointer; // 2 B padding na osmickovy offset + 8 B
} STR; // 24 B
//STR x[100][100][512];

struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode* next;
};

struct AdjList
{
    struct AdjListNode *head;  // pointer to head node of list
};

struct Graph
{
    int V;
    struct AdjList* array;
};

struct AdjListNode* newAdjListNode(int dest, int weight)
{
    struct AdjListNode* newNode =
            (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;

    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));

    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

void addEdge(struct Graph* graph, int src, int dest, int weight)
{
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

struct MinHeapNode
{
    int  v;
    int dist;
};

struct MinHeap
{
    int size;      // Number of heap nodes present currently
    int capacity;  // Capacity of min heap
    int *pos;     // This is needed for decreaseKey()
    struct MinHeapNode **array;
};

struct MinHeapNode* newMinHeapNode(int v, int dist)
{
    struct MinHeapNode* minHeapNode =
            (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

struct MinHeap* createMinHeap(int capacity)
{
    struct MinHeap* minHeap =
            (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =
            (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size &&
        minHeap->array[left]->dist < minHeap->array[smallest]->dist )
        smallest = left;

    if (right < minHeap->size &&
        minHeap->array[right]->dist < minHeap->array[smallest]->dist )
        smallest = right;

    if (smallest != idx)
    {
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];

        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

int isEmpty(struct MinHeap* minHeap)
{
    return minHeap->size == 0;
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    if (isEmpty(minHeap))
        return NULL;

    struct MinHeapNode* root = minHeap->array[0];

    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;

    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

void decreaseKey(struct MinHeap* minHeap, int v, int dist)
{
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;

    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

bool isInMinHeap(struct MinHeap *minHeap, int v)
{
    if (minHeap->pos[v] < minHeap->size)
        return true;
    return false;
}

void printArr(int dist[], int n)
{
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}

int *dijkstra(struct Graph* graph, int src, int *princess_dist)
{
    int V = graph->V;// Get the number of vertices in graph
    int dist[V];      // dist values used to pick minimum weight edge in cut
    int *parent = (int *)malloc(V*sizeof(int));
    struct MinHeap* minHeap = createMinHeap(V);

    for (int v = 0; v < V; ++v)
    {
        parent[v] = -1;
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }
    free(minHeap->array[src]);
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src]   = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);

    minHeap->size = V;

    while (!isEmpty(minHeap))
    {
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v; // Store the extracted vertex number

        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;

            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX &&
                pCrawl->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->weight;
                parent[v] = u;
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }
    int min=INT_MAX, select=-1;
    for(int i=1; i<7; i+=2){
        if(dist[princess_dist[i]]<min){
            min=dist[princess_dist[i]];
            select=princess_dist[i];
        }
    }
    princess_dist[0] = select;
    //printArr(dist, V);
    return parent;
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty)
{
    int *princess_dist = (int*)malloc(7*sizeof(int));
    int princess_dist_count = 1;
    int vertices = n*m*16; //width*height*status
    int mapsize = n*m;
    struct Graph* graph = createGraph(vertices);
    int i=0, j=0, row=n, col=m, princess_position[6], princess_found=3;
    for(i=0; i<0; i++)
        princess_position[i] = -1;
    for(i=0; i<7; i++) princess_dist[i] = -1;
    for(i=0; i<row; i++){
        for(j=0; j<col; j++){
            if(mapa[i][j] == 'P'){
                princess_found--;
                princess_position[princess_found*2] = i;    //x coordinate of princess
                princess_position[princess_found*2 + 1] = j;//y coordinate of princess
                printf("Princess found at %d %d\n", i, j);
                princess_dist[princess_dist_count] = mapsize*15+(i*col+j);
                princess_dist_count += 2;
            }
            printf("%c ",mapa[i][j]);
        }
        printf("\n");
    }
    for(i=0; i<row; i++){
        for(j=0; j<col; j++){
            //printf("%c %d ",mapa[i][j],i*col+j);
            if(mapa[i][j] == 'C'){ //cesta
                if(j+1 < col && mapa[i][j+1] != 'N'){ //right
                    for(int level=0; level<16; level++)
                        if(level == 0 || (level & D) > 0){
                            //printf("->Connecting %d with %d\n",mapsize*level+(i*col+j), mapsize*level+(i*col+j+1));
                            addEdge(graph, mapsize*level+(i*col+j), mapsize*level+(i*col+j+1), 1);
                        }
                }
                if(i+1 < row && mapa[i+1][j] != 'N'){ //down
                    for(int level=0; level<16; level++)
                        if(level == 0 || (level & D) > 0){
                            //printf("V Connecting %d with %d\n",mapsize*level+(i*col+j), mapsize*level+((i+1)*col+j));
                            addEdge(graph, mapsize*level+(i*col+j), mapsize*level+((i+1)*col+j), 1);
                        }
                }
            }
            if(mapa[i][j] == 'H'){ //cesta
                if(j+1 < col && mapa[i][j+1] != 'N'){ //right
                    for(int level=0; level<16; level++)
                        if(level == 0 || (level & D) > 0){
                            //printf("->Connecting %d with %d\n",mapsize*level+(i*col+j), mapsize*level+(i*col+j+1));
                            addEdge(graph, mapsize*level+(i*col+j), mapsize*level+(i*col+j+1), 2);
                        }
                }
                if(i+1 < row && mapa[i+1][j] != 'N'){ //down
                    for(int level=0; level<16; level++)
                        if(level == 0 || (level & D) > 0){
                            //printf("V Connecting %d with %d\n",mapsize*level+(i*col+j), mapsize*level+((i+1)*col+j));
                            addEdge(graph, mapsize*level+(i*col+j), mapsize*level+((i+1)*col+j), 2);
                        }
                }
            }
            if(mapa[i][j] == 'D'){ //drak
                printf("D Connecting %d with %d\n",(i*col+j), mapsize*2+(i*col+j));
                addEdge(graph, (i*col+j), mapsize*D+(i*col+j), 0);
                if(j+1 < col && mapa[i][j+1] != 'N'){ //right
                    for(int level=0; level<16; level++)
                        if(level == 0 || (level & D) > 0){
                            //printf("->Connecting %d with %d\n",mapsize*level+(i*col+j), mapsize*level+(i*col+j+1));
                            addEdge(graph, mapsize*level+(i*col+j), mapsize*level+(i*col+j+1), 1);
                        }
                }
                if(i+1 < row && mapa[i+1][j] != 'N'){ //down
                    for(int level=0; level<16; level++)
                        if(level == 0 || (level & D) > 0){
                            //printf("V Connecting %d with %d\n",mapsize*level+(i*col+j), mapsize*level+((i+1)*col+j));
                            addEdge(graph, mapsize*level+(i*col+j), mapsize*level+((i+1)*col+j), 1);
                        }
                }
            }
            if(mapa[i][j] == 'P'){ //princezne
                for(int level=0; level<16; level++){
                    if(i == princess_position[0] && j == princess_position[1]){
                        if((level & D) > 0 && (level & P1) == 0){
                            //printf("P Connecting %d with %d\n",mapsize*level+(i*col+j), mapsize*(level | P1)+(i*col+j));
                            addEdge(graph, mapsize*level+(i*col+j), mapsize*(level | P1)+(i*col+j), 0);
                        }
                    }
                    if(i == princess_position[2] && j == princess_position[3]){
                        if((level & D) > 0 && (level & P2) == 0){
                            //printf("P Connecting %d with %d\n",mapsize*level+(i*col+j), mapsize*(level | P2)+(i*col+j));
                            addEdge(graph, mapsize*level+(i*col+j), mapsize*(level | P2)+(i*col+j), 0);
                        }
                    }
                    if(i == princess_position[4] && j == princess_position[5]){
                        if((level & D) > 0 && (level & P3) == 0){
                            //printf("P Connecting %d with %d\n",mapsize*level+(i*col+j), mapsize*(level | P3)+(i*col+j));
                            addEdge(graph, mapsize*level+(i*col+j), mapsize*(level | P3)+(i*col+j), 0);
                        }
                    }
                }
                if(j+1 < col && mapa[i][j+1] != 'N'){ //right
                    for(int level=0; level<16; level++)
                        if(level == 0 || (level & D) > 0){
                            //printf("P->Connecting %d with %d\n",mapsize*level+(i*col+j), mapsize*level+(i*col+j+1));
                            addEdge(graph, mapsize*level+(i*col+j), mapsize*level+(i*col+j+1), 1);
                        }
                }
                if(i+1 < row && mapa[i+1][j] != 'N'){ //down
                    for(int level=0; level<16; level++)
                        if(level == 0 || (level & D) > 0){
                            //printf("PV Connecting %d with %d\n",mapsize*level+(i*col+j), mapsize*level+((i+1)*col+j));
                            addEdge(graph, mapsize*level+(i*col+j), mapsize*level+((i+1)*col+j), 1);
                        }
                }
            }
        }
    }
    int *parent_array = dijkstra(graph, 0, princess_dist);
    i = princess_dist[0]; int x = 0, y = 0, count = 0;
    int *p = (int*)malloc(100*sizeof(int));
    while(parent_array[i] != -1){
        x = (i % mapsize)/col;
        y = (i % mapsize)%col;
        printf("%d %d %d %d\n", i, parent_array[i], x, y);
        i = parent_array[i];
        if(count > 0 && p[count-2] == x && p[count-1] == y){
            printf("NO\n");}
        else{
            p[count++]=x;
            p[count++]=y;
        }
    }
    p[count++]=0;
    p[count++]=0;
    printf("Count %d\n", count);
    int *arr = (int*)malloc(count*sizeof(int));
    printf("HELLO\n");
    for(i=0;i<count;i+=2){
        arr[i]=p[count-i-1]; //y
        arr[i+1]=p[count-i-2]; //x
    }
    *(dlzka_cesty) = count / 2;
    for(i=0;i<count;i+=2)
        printf("%d %d\n", arr[i], arr[i+1]);
    return arr;
}

// Vlastna funkcia main() je pre vase osobne testovanie. Dolezite: pri testovacich scenaroch sa nebude spustat!
int main()
{
    int vertices = 2*3*4; //width*height*status
    struct Graph* graph = createGraph(vertices);
    int row=2, col=3, i=0, j=0;
    char map[2][3] = {{'C', 'C', 'C'},
                      {'P', 'C', 'D'}};

    for(i=0; i<row; i++){
        for(j=0; j<col; j++)
            printf("%c ",map[i][j]);
        printf("\n");
    }
    for(i=0; i<row; i++){
        for(j=0; j<col; j++){
            //printf("%c %d ",map[i][j],i*col+j);
            if(map[i][j] == 'C'){
                if(j+1 < col && map[i][j+1] != 'N'){ //right
                    for(int level=0; level<4; level++)
                        if(level == 0 || (level & 2) > 0){
                            //printf("->Connecting %d with %d\n",6*level+(i*col+j), 6*level+(i*col+j+1));
                            addEdge(graph, 6*level+(i*col+j), 6*level+(i*col+j+1), 1);
                        }
                }
                if(i+1 < row && map[i+1][j] != 'N'){ //down
                    for(int level=0; level<4; level++)
                        if(level == 0 || (level & 2) > 0){
                            //printf("V Connecting %d with %d\n",6*level+(i*col+j), 6*level+((i+1)*col+j));
                            addEdge(graph, 6*level+(i*col+j), 6*level+((i+1)*col+j), 1);
                        }
                }
            }
            if(map[i][j] == 'D'){
                //printf("D Connecting %d with %d\n",(i*col+j), 6*2+(i*col+j));
                addEdge(graph, (i*col+j), 6*2+(i*col+j), 0);
                if(j+1 < col && map[i][j+1] != 'N'){ //right
                    for(int level=0; level<4; level++)
                        if(level == 0 || (level & 2) > 0){
                            //printf("->Connecting %d with %d\n",6*level+(i*col+j), 6*level+(i*col+j+1));
                            addEdge(graph, 6*level+(i*col+j), 6*level+(i*col+j+1), 1);
                        }
                }
                if(i+1 < row && map[i+1][j] != 'N'){ //down
                    for(int level=0; level<4; level++)
                        if(level == 0 || (level & 2) > 0){
                            //printf("V Connecting %d with %d\n",6*level+(i*col+j), 6*level+((i+1)*col+j));
                            addEdge(graph, 6*level+(i*col+j), 6*level+((i+1)*col+j), 1);
                        }
                }
            }
            if(map[i][j] == 'P'){
                for(int level=0; level<4; level++)
                    if((level & 2) > 0 && (level & 1) == 0){
                        //printf("P Connecting %d with %d\n",6*level+(i*col+j), 6*(level | 1)+(i*col+j));
                        addEdge(graph, 6*level+(i*col+j), 6*(level | 1)+(i*col+j), 0);
                    }
                if(j+1 < col && map[i][j+1] != 'N'){ //right
                    for(int level=0; level<4; level++)
                        if(level == 0 || (level & 2) > 0){
                            //printf("->Connecting %d with %d\n",6*level+(i*col+j), 6*level+(i*col+j+1));
                            addEdge(graph, 6*level+(i*col+j), 6*level+(i*col+j+1), 1);
                        }
                }
                if(i+1 < row && map[i+1][j] != 'N'){ //down
                    for(int level=0; level<4; level++)
                        if(level == 0 || (level & 2) > 0){
                            //printf("V Connecting %d with %d\n",6*level+(i*col+j), 6*level+((i+1)*col+j));
                            addEdge(graph, 6*level+(i*col+j), 6*level+((i+1)*col+j), 1);
                        }
                }
            }
        }
    }
    //dijkstra(graph, 0);
    return 0;
}
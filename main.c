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


// A structure to represent a node in adjacency list
struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode* next;
};

// A structure to represent an adjacency list
struct AdjList
{
    struct AdjListNode *head;  // pointer to head node of list
};

// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph
{
    int V;
    struct AdjList* array;
};

// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest, int weight)
{
    struct AdjListNode* newNode =
            (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// A utility function that creates a graph of V vertices
struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;

    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));

    // Initialize each adjacency list as empty by making head as NULL
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest, int weight)
{
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the beginning
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

// Structure to represent a min heap node
struct MinHeapNode
{
    int  v;
    int dist;
};

// Structure to represent a min heap
struct MinHeap
{
    int size;      // Number of heap nodes present currently
    int capacity;  // Capacity of min heap
    int *pos;     // This is needed for decreaseKey()
    struct MinHeapNode **array;
};

// A utility function to create a new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, int dist)
{
    struct MinHeapNode* minHeapNode =
            (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

// A utility function to create a Min Heap
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

// A utility function to swap two nodes of min heap. Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// A standard function to heapify at given idx
// This function also updates position of nodes when they are swapped.
// Position is needed for decreaseKey()
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
        // The nodes to be swapped in min heap
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];

        // Swap positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

// A utility function to check if the given minHeap is ampty or not
int isEmpty(struct MinHeap* minHeap)
{
    return minHeap->size == 0;
}

// Standard function to extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    if (isEmpty(minHeap))
        return NULL;

    // Store the root node
    struct MinHeapNode* root = minHeap->array[0];

    // Replace root node with last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // Update position of last node
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;

    // Reduce heap size and heapify root
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

int *dijkstra(struct Graph* graph, int src)
{
    int V = graph->V;// Get the number of vertices in graph
    int dist[V];      // dist values used to pick minimum weight edge in cut
    int *parent = (int *)malloc(V*sizeof(int));
    struct MinHeap* minHeap = createMinHeap(V);

    // Initialize min heap with all vertices. dist value of all vertices
    for (int v = 0; v < V; ++v)
    {
        parent[v] = -1;
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    // Make dist value of src vertex as 0 so that it is extracted first
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src]   = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);

    // Initially size of min heap is equal to V
    minHeap->size = V;

    // In the followin loop, min heap contains all nodes
    // whose shortest distance is not yet finalized.
    while (!isEmpty(minHeap))
    {
        // Extract the vertex with minimum distance value
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v; // Store the extracted vertex number

        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their distance values
        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;

            // If shortest distance to v is not finalized yet, and distance to v
            // through u is less than its previously calculated distance
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX &&
                pCrawl->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->weight;
                parent[v] = u;
                // update distance value in min heap also
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }
    printArr(dist, V);
    return parent;
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty)
{
    int vertices = n*m*16; //width*height*status
    int mapsize = n*m;
    struct Graph* graph = createGraph(vertices);
    int i=0, j=0, row=n, col=m, princess_position[6], princess_found=3;
    for(i=0; i<0; i++)
        princess_position[i] = -1;
    for(i=0; i<row; i++){
        for(j=0; j<col; j++){
            if(mapa[i][j] == 'P'){
                princess_found--;
                princess_position[princess_found*2] = i;    //x coordinate of princess
                princess_position[princess_found*2 + 1] = j;//y coordinate of princess
                //printf("Princess found at %d %d\n", i, j);
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
    int *parent_array = dijkstra(graph, 0);
    i = 375; int x = 0, y = 0, count = 0;
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
    /*for(i=0;i<count/2;i++){
        j=p[i];
        p[i]=p[count-1-i];
        p[count-1-i]=j;
    }*/
    int *arr = (int*)malloc(count*sizeof(int));
    for(i=0;i<count;i+=2){
        arr[i]=p[count-i-1]; //y
        arr[i+1]=p[count-i-2]; //x
    }
    /*arr[0]=0;
    arr[1]=0;
    arr[2]=1;
    arr[3]=0;
    arr[4]=1;
    arr[5]=1;
    arr[6]=1;
    arr[7]=2;
    arr[8]=1;
    arr[9]=3;
    arr[10]=2;
    arr[11]=3;
    arr[12]=2;
    arr[13]=4;
    arr[14]=3;
    arr[15]=4;
    arr[16]=2;
    arr[17]=4;
    arr[18]=1;
    arr[19]=4;
    arr[20]=1;
    arr[21]=3;
    arr[22]=1;
    arr[23]=2;
    arr[24]=1;
    arr[25]=1;
    arr[26]=1;
    arr[27]=0;
    arr[28]=0;
    arr[29]=0;*/
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
    dijkstra(graph, 0);
    return 0;
}
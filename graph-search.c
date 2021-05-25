/*
 * Graph-Search
 *
 * Data Structures
 *
 * Department of Computer Science
 * at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX 10

#define FALSE 0
#define TRUE 1
short int visited[MAX_VERTEX] = { 0 };       //DFS,BFS 정점 방문을 확인하기 위해 필요한 배열  
short int check_vertex[MAX_VERTEX] = { 0 };  //인접리스트에 삽입된 정점을 확인하기 위해 필요한 배열 


typedef struct _graphNode {     //인접리스트의 노드
    struct graphNode* next;
    int vertex;
}graphNode;

graphNode* adjLists[MAX_VERTEX]; //그래프 정점들에 대한 헤드 포인터를 담는 배열


/* for stack */
#define MAX_STACK_SIZE 10
int stack[MAX_STACK_SIZE];   
int top = -1;

int pop();
void push(int vertex);


/* for queue */
#define MAX_QUEUE_SIZE 10
int queue[MAX_QUEUE_SIZE];   

int front = -1;
int rear = -1;

int deQueue();
void enQueue(int vertex);

/* 함수 선언 */
 //!!!!반환형 check하기!!!!! 
void initialize_Graph();        /*초기화*/
int insert_Vertex(int u);       /*정점삽입*/
int insert_Edge(int u, int v);  /*간선삽입*/
int DFS(int u);                 /*DFS*/
int BFS(int u);                 /*BFS*/
void print_Graph();             /*인접리스트 print*/
int freeGraph();                /*메모리해제 */


int main()
{
    char command;
    int u,v; //정점 

    printf("[----- [jinyounglee] [2020039063] -----]\n");

    do {
        printf("\n\n");
        printf("----------------------------------------------------------------\n");
        printf("                        Graph Searches                        \n");
        printf("----------------------------------------------------------------\n");
        printf(" Initialize Graph     = z                                       \n");
        printf(" Insert Vertex        = v      Insert Edge                  = e \n");
        printf(" Depth First Search   = d      Breath First Search          = b \n");
        printf(" Print Graph          = p      Quit                         = q \n");
        printf("----------------------------------------------------------------\n");

        printf("Command = ");
        scanf(" %c", &command);

        switch (command) {
        case 'z': case 'Z':
            initialize_Graph();         
            break;
        case 'v': case 'V':
            printf("Input one vertex: ");
            scanf("%d", &u);
            insert_Vertex(u);
            break;
        case 'e': case 'E':
            printf("Please input two vertices: ");
            scanf("%d %d", &u, &v);
            insert_Edge(u, v);
            break;
        case 'd': case 'D':
            printf("Input the vertex to be DFS: ");
            scanf("%d", &u);
            DFS(u);
            break;
        case 'b': case 'B':
            printf("Input the vertex to be BFS: ");
            scanf("%d", &u);
            BFS(u);
            break;
        case 'p': case 'P':
            print_Graph();      //인접리스트 출력
            break;
        case 'q': case 'Q':
            freeGraph();      
            break;
        default:
            printf("\n       >>>>>   Concentration!!   <<<<<     \n");
            break;
        }

    } while (command != 'q' && command != 'Q');

    return 1;
}


/* 초기화 */
void initialize_Graph()    
{
    //인접리스트에 간선관련 노드가 존재하는지 검사  
    for (int i = 0; i < MAX_VERTEX; i++)
    {
        if (adjLists[i] != NULL)    //인접리스트에 간선관련 노드가 존재시 
        {
            freeGraph();
            break;
        }
    }

    //인접리스트 노드 모두 해제시킨 후  
    for (int i = 0; i < MAX_VERTEX; i++)
        adjLists[i] = NULL;         //포인터배열 NULL로 초기화
}


/* 정점 삽입 */
int insert_Vertex(int u)
{
    if (u < 0 || u >= MAX_VERTEX)  //오류 검사        
    {
        printf("Error: It is not a vertex that satisfies the conditions \n");
        return -1;
    }

    if (check_vertex[u] == 1)    //이미 존재하는 정점일 경우
    {
        printf("ERROR: A vertex that already exists \n");
        return -1;
    }

    //check_vertex[u]==0으로, 이전에 u가 삽입되지 않았을 때
    check_vertex[u] = 1;
    return 0;
}


/* 간선 삽입 -> 오름차순으로 정렬 */
int insert_Edge(int u, int v)  //u->v  
{
    if (u < 0 || u >= MAX_VERTEX || v < 0 || v >= MAX_VERTEX)   //오류검사             
    {
        printf("Error: There are not vertexs that satisfy the conditions \n");
        return -1;
    }
    if (check_vertex[u] == 0)  //삽입된 정점이 아닌 경우 오류 발생 
    {
        printf("Error: Not an inserted vertex \n");
        return -1;
    }

    //간선 삽입을 위해 새로운 노드 생성 및 초기화
    graphNode* newnode = NULL;
    newnode = (graphNode*)malloc(sizeof(graphNode));
    newnode->vertex = v;
    newnode->next = NULL;

    //노드들을 정렬하기 위해 graphNode형 포인터 2개 생성 및 초기화 
    graphNode* prev = NULL;
    graphNode* cur = adjLists[u];

    //해당 정점에 간선이 존재하지 않을 때
    if (!cur) //cur=NULL
    {
        adjLists[u] = newnode;
        return 0;
    }
    //해당 정점에 간선이 하나 존재할 때
    else if (!(cur->next))    //cur->next==NULL
    {
        if (v < cur->vertex)
        {
            adjLists[u] = newnode;
            newnode->next = cur;
            return 0;
        }
        else
        {
            cur->next = newnode;
            return 0;
        }
    }
    /* 해당 정점에 간선이 두 개 이상 존재할 때
        -> 각각의 정점의 맨 앞과 맨 뒤에 간선이 삽입될 때 고려하기!! */
    else
    {
        while (cur)
        {
            if (v < cur->vertex)
            {
                if (cur == adjLists[u]) //해당 정점 맨 앞에 삽입될 때 
                {
                    adjLists[u] = newnode;
                    newnode->next = cur;
                    return 0;
                }
                //그 이외에 삽입될 때
                prev->next = newnode;
                newnode->next = cur;
                return 0;
            }
            else //v>=cur->vertex일 때, prev와 cur 이동
            {
                prev = cur;
                cur = cur->next;
            }
        }
        //cur==NULL일 때 -> prev가 가리키는 노드가 마지막 노드일 때(정점 v가 가장 큰 값일 때) 
        prev->next = newnode;
        return 0;
    }
}


/*그래프 탐색이란, 하나의 정점으로부터 시작해 차례대로 모든 정점들을 한 번씩 방문하는 것
  => DFS, BFS */

/* Depth First Search => tree의 preorder traversal과 유사
    방법 2가지: recursive(visit flag), iterative(stack+visit flag) */

int DFS(int u)                      ///반환형 check하기 !!+ 오류검사하기 
{
    graphNode* cur = adjLists[u];
    for (int i = 0; i < MAX_VERTEX; i++)
        visited[i] = FALSE;           //0으로 초기화 

    printf("DFS: ");
    push(u);
    visited[u] = TRUE;

    /* iterative 방식 */
    while (top != -1)  //스택이 공백이 될 때까지 수행
    {
        if (!visited[cur->vertex])   //방문하지 않은 정점 방문시
        {
            push(cur->vertex);
            visited[cur->vertex] = TRUE;
            cur = adjLists[cur->vertex];
        }
        else                        //방문한 적 있는 정점 방문시 -> stack에서 pop()실행
        {
            //해당 정점 u와 간선을 이루는 노드들을 모두 방문했을 때
            if (!(cur->next))  //cur->next == NULL
            {
                printf("%2d->", pop());
                cur = adjLists[(stack[top])];           //!!!여기 조금 수정하기!!! -> 시간복잡도가 너무 커!!!
            }
            //해당 정점 u와 간선을 이루는 노드 중 일부 방문하지 않았을 때 
            else
                cur = cur->next;
        }
    }
}


/* 스택에서 원소 삭제하는 함수 */
int pop()
{
    /* stack이 Empty인지 검사 */
    if (top == -1) //top=-1일때 스택이 비어있는상태로, 삭제할 원소가 존재하지 않음
        return NULL; //NULL리턴 
    else
        return stack[top--]; //top에 위치한 원소를 반환하고, top의 위치를 -1한다
}


/* 스택에 원소 삽입하는 함수 */
void push(int vertex)
{
    /* stack이 Full인지 검사 */
    if (top < MAX_STACK_SIZE - 1) //top>=MAX_STACK_SIZE-1이면, stack이 가득찬 상태로 원소를 삽입할 수 없음
        stack[++top] = vertex; //top을 1증가시킨 위치에 원소를 삽입한다
}


/* Breath First Search => tree의 levelorder traversal과 유사
        방법 2가지: recursive(visit flag), iterative(queue+visit flag) */
int BFS(int u)
{
    //정점 u가 존재하지 않을 때 오류검사하기!       
    if (u < 0 || u >= MAX_VERTEX)
    {
        printf("Error: It is not a vertex that satisfies the conditions \n");
        return -1;
    }

    for (int i = 0; i < MAX_VERTEX; i++)
        visited[i] = FALSE;     //BFS를 통해 탐색을 할 때, 방문한 vertex를 check하기 위해 0으로 초기화 

    /* iterative 방식 */
    graphNode* cur = NULL;
    visited[u] = TRUE;          //visiting marking
    enQueue(u);
    printf("BFS: ");

    //공백 queue이기전까지 반복 
    while (front!=rear)                
    {
        u = deQueue();
        printf("%2d->", u);
        for (cur = adjLists[u]; cur; cur = cur->next)          
        {
            if (!visited[cur->vertex])              //방문하지 않은 노드일 때 
            {
                visited[cur->vertex] = TRUE;        //visiting marking
                enQueue(cur->vertex);               //queue에 삽입 
            }
            else    //방문한 노드일 때 
                continue;
        }

    }
    //공백 queue일 때 (= 모든 vertex방문)
    printf("\n");
    return 0;
}

/* queue에서 원소 삭제하는 함수 */
int deQueue()
{
    /* queue가 empty인지 검사 */
    if (rear == front) //queue가 empty일 때(rear과 front 동일) -> 삭제할 원소 없음
        return -1;
    return queue[(++front) % MAX_QUEUE_SIZE]; //front를 front+1로 재설정하고, 변경된 front에 위치한 queue의 원소 반환
}

/* queue에서 원소 삽입하는 함수 */
void enQueue(int vertex)
{
    /* queue가 full인지 검사 */
    if ((rear + 1) % MAX_QUEUE_SIZE != front) //rear+1과 front 동일(queue가 full인 상태) -> queue에 삽입할 수 없음
        queue[(++rear) % MAX_QUEUE_SIZE] = vertex; //rear를 rear+1로 재설정하고, 변경된 rear위치에 vertex 삽입
}


/* 인접리스트 출력 */
void print_Graph()  
{
    graphNode* cur = NULL;
    printf("adjLists: \n");
    for (int i = 0; i < MAX_VERTEX; i++)
    {
        printf("vertex %2d: ", i);
        cur = adjLists[i];                  //각각의 vertex와 인접한 노드 출력 
        while (cur)
        {
            printf("%2d->", cur->vertex);
            cur = cur->next;
        }
        printf("\n");
    }
}


/* 메모리 해제 */
int freeGraph()
{
    /* 인접리스트 메모리 해제를 위해 graphNode* 포인터 2개 생성 */
    graphNode* trail = NULL;
    graphNode* middle = NULL;

    for (int i = 0; i < MAX_VERTEX; i++)       
    {
        middle = adjLists[i];
        while (middle)
        {
            trail = middle;
            middle = middle->next;
            free(trail);
        }
    }
    return 0;
}
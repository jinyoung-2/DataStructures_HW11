/*
 * Graph-Search
 *
 * Data Structures
 *
 * Department of Computer Science
 * at Chungbuk National University
 *
 */

///////함수 원형 모두 작성됐는지 확인하기 

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX 10

#define FALSE 0
#define TRUE 1
short int visited[MAX_VERTEX] = { 0 };  //전역배열 visited 0으로 모두 초기화 
short int check_vertex[MAX_VERTEX] = { 0 };  //삽입된 정점 check하기 위해 


typedef struct _graphNode {
    struct graphNode* next;
    int vertex;
}graphNode;

graphNode* adjLists[MAX_VERTEX]; //그래프 정점들에 대한 헤드 포인터를 담는 배열(다른 정점을 가리키고 있는지를 확인하기 위해)

/* for stack */
#define MAX_STACK_SIZE 10
int stack[MAX_STACK_SIZE];   //graphNode* stack[MAX_STACK_SIZE];? 이렇게 해야하는 이유가 있을까? 굳이?
int top = -1;

int pop();
void push(int vertex);


/* for queue */
#define MAX_QUEUE_SIZE 10
int queue[MAX_QUEUE_SIZE];   //graphNode* queue[MAX_QUEUE_SIZE];? 이렇게 해야하는 이유가 있을까? 굳이?

int front = -1;
int rear = -1;

int deQueue();
void enQueue(int vertex);

/* 함수 */
 //!!!!반환형 check하기!!!!! 


/* 초기화 */
void initialize_Graph()    //Q. 왜 이중포인터로 사용안해?
{
    //인접리스트에 간선관련 노드가 존재하는지 검사  
    for (int i = 0; i < MAX_VERTEX; i++)
    {
        if (adjLists[i] != NULL)    //인접리스트에 간선관련 노드가 존재할 때 
        {
            freeGraph();
            break;
        }
    }

    //인접리스트 노드 모두 해제시킨 후,  
    for (int i = 0; i < MAX_VERTEX; i++)
        adjLists[i] = NULL;   //포인터배열 NULL로 초기화
}

///short int check_vertex[MAX_VERTEX]={0};  //삽입된 정점 check하기 위해  ---> 제출하기 전에 삭제하기 
//정점 삽입 
void insert_Vertex(int u)
{
    if (u < 0 || u >= MAX_VERTEX)  //오류 검사        
    {
        printf("Error: It is not a vertex that satisfies the conditions \n");
        return;
    }

    if (check_vertex[u] == 1)    //이미 존재하는 정점일 경우
    {
        printf("A vertex that already exists \n");
        return;
    }

    //check_vertex[u]==0으로, 이전에 u가 삽입되지 않았을 때
    check_vertex[u] = 1;
    return;
}


//간선 삽입 -> 오름차순으로 정렬 
int insert_Edge(int u, int v)  //u->v   //Q. graphNode** adjLists[]가 매개변수가 아닌지 생각해보기
{
    if (u < 0 || u >= MAX_VERTEX || v < 0 || v >= MAX_VERTEX)   //오류검사             
    {
        printf("Error:It is not vertexs that satisfy the conditions \n");
        return -1;
    }
    if (check_vertex[u] == 0)  //삽입된 정점이 아닌 경우 오류 발생 
    {
        printf("Error:Not an inserted vertex \n");
        return -1;
    }
    //** 기존에 삽입한 간선일 때-> Q.Q.  이미 존재한다고 어떻게 검사하면 좋을까? 

    graphNode* newnode = NULL;
    newnode = (graphNode*)malloc(sizeof(graphNode));
    newnode->vertex = v;
    newnode->next = NULL;

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
    //해당 정점에 간선이 두 개 이상 존재할 때
    //각각의 정점의 맨 앞과 맨 뒤에 간선이 삽입될 때 고려하기!! 
    else
    {
        while (cur)
        {
            if (v < cur->vertex)
            {
                if (cur == adjLists[u]) //해당 정점 맨 앞에 삽입될 때 
                {
                    newnode->next = cur;
                    adjLists[u] = newnode;
                    return 0;
                }
                //그 이외에 삽입될 때
                newnode->next = cur;
                prev->next = newnode;
                return 0;
            }
            else //v>=cur->vertex
            {
                prev = cur;
                cur = cur->next;
            }
        }
        //cur=NULL일 때 -> prev이 마지막 노드 
        prev->next = newnode;
        return 0;
    }
}

/*그래프 탐색이란, 하나의 정점으로부터 시작해 차례대로 모든 정점들을 한 번씩 방문하는 것
  => DFS, BFS */

  /* Depth First Search => tree의 preorder traversal과 유사
      방법 2가지: recursive(visit flag), iterative(stack+visit flag) */

//pop할 때 print하기 
void DFS(int u)
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
    //정점 v가 존재하지 않을 때 오류검사하기!       
    if (u < 0 || u >= MAX_VERTEX)
    {
        printf("ERROR: \n");
        return -1;
    }

    for (int i = 0; i < MAX_VERTEX; i++)
        visited[i] = FALSE;

    /* iterative 방식 */
    graphNode* cur = NULL;
    visited[u] = TRUE;    //visit marking
    enQueue(u);

    printf("BFS: ");

    //공백 queue이기전까지 반복 -> 조건식 check
    while (front!=rear)                //!!여기서 큐는 순환큐를 굳이 사용할 필요가 없음. 
    {
        u = deQueue();
        printf("%2d->", u);
        for (cur = adjLists[u]; cur; cur = cur->next)          //Q. check_vertex 배열사용 안 해도 괘찬"?
        {
            if (!visited[cur->vertex])   //방문하지 않은 노드일 때 
            {
                visited[cur->vertex] = TRUE;
                enQueue(cur->vertex);
            }
            else    //방문한 노드일 때 
                continue;
        }

    }
    //공백 queue일 때 (=모든 vertex방문)
    printf("\n");
    return 0;
}


/* queue에서 원소 삭제하는 함수 */
int deQueue()
{
    /* queue가 empty인지 검사 */
    if (rear == front) //queue가 empty일 때(rear과 front 동일) -> 삭제할 원소 없음
        return NULL; //NULL 반환
    return queue[(++front) % MAX_QUEUE_SIZE]; //front를 front+1로 재설정하고, 변경된 front에 위치한 queue의 원소 반환
}


/* queue에서 원소 삽입하는 함수 */
void enQueue(int vertex)
{
    /* queue가 full인지 검사 */
    if ((rear + 1) % MAX_QUEUE_SIZE != front) //rear+1과 front 동일(queue가 full인 상태) -> queue에 삽입할 수 없음
        queue[(++rear) % MAX_QUEUE_SIZE] = vertex; //rear를 rear+1로 재설정하고, 변경된 rear위치에 vertex 삽입
}


void print_Graph()  //check하기 !! 매개변수 형태 저렇게 써도 되나?
{
    graphNode* cur = NULL;
    printf("adjLists: \n");
    for (int i = 0; i < MAX_VERTEX; i++)
    {
        printf("vertex %2d: ", i);
        cur = adjLists[i];          //Q. cur=h->adjLists[i]->next;로 둬야 해당 정점과 edge를 가지는 정점이 출력되지 않나? 
        while (cur)
        {
            printf("%2d->", cur->vertex);
            cur = cur->next;
        }
        printf("\n");
    }
}

int freeGraph()
{
    graphNode* trail = NULL;
    graphNode* middle = NULL;

    for (int i = 0; i < MAX_VERTEX; i++)       //조건식 수정하기!!!!
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
        printf(" Print Graph          = p      Quit                         = q\n");
        printf("----------------------------------------------------------------\n");

        printf("Command = ");
        scanf(" %c", &command);

        switch (command) {
        case 'z': case 'Z':
            initialize_Graph();         //인자 형태 맞는지 체크하기   
            break;
        case 'v': case 'V':
            printf("Input one vertex: ");
            scanf("%d", &u);
            insert_Vertex(u);
            break;
        case 'e': case 'E':
            printf("Please input two vertices. ");
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
            print_Graph(); //인접리스트 출력함수
            break;
        case 'q': case 'Q':
            freeGraph();       //인자 형태 맞는지 체크하기
            break;
        default:
            printf("\n       >>>>>   Concentration!!   <<<<<     \n");
            break;
        }

    } while (command != 'q' && command != 'Q');

    return 1;
}
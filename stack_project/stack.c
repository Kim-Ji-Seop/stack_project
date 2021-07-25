#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//연결리스트 , 스택
//계산기 이론
//중위표기법, 후위표기법
//괄호처리
typedef struct _Node
{
    char data[100];
    struct _Node* next;
} Node;

typedef struct _Stack
{
    Node* top;
} Stack;

void push(Stack* stack, char* data)
{
    Node* node = (Node*)malloc(sizeof(Node));
    strcpy(node->data, data);
    node->next = stack->top;
    stack->top = node;
}

char* getTop(Stack* stack)
{
    Node* top = stack->top;
    return top->data;
}
char* pop(Stack* stack)
{
    if (stack->top == NULL)
    {
        printf("stack underflow\n");
        return NULL;
    }
    Node* node = stack->top;
    char* data = (char*)malloc(sizeof(char) * 100);
    strcpy(data, node->data);
    stack->top = node->next;
    free(node);
    return data;
}
//중위 -> 후위표기
//1. 피연산자가 들어오면 바로 출력
//2. 연산자가 들어오면 자기보다 우선순위가 높거나 같은 것들을 빼고 자신을 스택에 담는다.
//3. 여는괄호 '('를 만나면 무조건 스택에 담는다.
//4. 닫는괄호 ')'를 만나면 '('를 만날 때까지 스택에서 출력한다.
int getPriority(char* i)
{
    if (!strcmp(i, "("))
        return 0; //strcmp함수는 두 문자열이 같은 경우 0을 반환하므로 !를 붙여주었다.
    if (!strcmp(i, "+") || !strcmp(i, "-"))
        return 1;
    if (!strcmp(i, "*") || !strcmp(i, "/"))
        return 2;
    return 3;
}

char* transition(Stack* stack, char** s, int size, char* res)
{ //중위표기식 -> 후위표기식으로 변환해주는 함수
    //char res[1000] = "";
    for (int i = 0; i < size; i++)
    {
        if (!strcmp(s[i], "+") || !strcmp(s[i], "-") || !strcmp(s[i], "*") || !strcmp(s[i], "/"))
        {

            while (stack->top != NULL && getPriority(getTop(stack)) >= getPriority(s[i]))
            {
                strcat(res, pop(stack));
                strcat(res, " ");
            }
            push(stack, s[i]);
        }
        else if (!strcmp(s[i], "("))
        {
            push(stack, s[i]);
        }
        else if (!strcmp(s[i], ")"))
        {
            while (strcmp(getTop(stack), "("))
            {
                strcat(res, pop(stack));
                strcat(res, " ");
            }
            pop(stack);
        }
        else
        {
            strcat(res, s[i]);
            strcat(res, " ");
        }
    }
    while (stack->top != NULL)
    {
        strcat(res, pop(stack));
        strcat(res, " ");
    }
    return res;
}
// 후위 표기법을 계산하는 방법
// 1. 피연산자가 들어오면 스택에 담는다.
// 2. 연산자를 만나면 스택에서 두개의 피연산자를 꺼내서 연산한 뒤에 그 결과를 스택에 담는다.
// 3. 연산을 마친 뒤에 스택에 남아있는 하나의 피연산자가 연산 수행 결과다.
void calculate(Stack* stack, char** s, int size)
{
    int x, y, z;
    for (int i = 0; i < size; i++)
    {
        if (!strcmp(s[i], "+") || !strcmp(s[i], "-") || !strcmp(s[i], "*") || !strcmp(s[i], "/"))
        {
            x = atoi(pop(stack)); //x는 가장 최근에 들어간 원소이고 y는 과거에 들어간 원소이기 때문에 (스택의 특징)
            y = atoi(pop(stack)); //y->x순으로 연산을 수행한다.
            if (!strcmp(s[i], "+"))
                z = y + x;
            if (!strcmp(s[i], "-"))
                z = y - x;
            if (!strcmp(s[i], "*"))
                z = y * x;
            if (!strcmp(s[i], "/"))
                z = y / x;
            char buffer[100];
            sprintf(buffer, "%d", z);
            push(stack, buffer);
        }
        else
        {
            push(stack, s[i]);
        }
    }
    printf("%s\n", pop(stack));
}

int main(void)
{
    Stack stack;
    stack.top = NULL;
    char res[1000] = "";
    char a[100] = "( ( 3 + 4 ) * 5 ) - 5 * 7 * 5 - 5 * 10";
    int size = 1;
    for (int i = 0; i < strlen(a); i++)
    {
        if (a[i] == ' ')
            size++;
    }
    char* ptr = strtok(a, " ");
    char** input = (char**)malloc(sizeof(char*) * size);
    for (int i = 0; i < size; i++)
    {
        input[i] = (char*)malloc(sizeof(char) * 100);
    }
    for (int i = 0; i < size; i++)
    {
        strcpy(input[i], ptr);
        ptr = strtok(NULL, " ");
    }
    char b[1000] = "";
    strcpy(b, transition(&stack, input, size, res));
    printf("후위표기법: %s\n", b);
    size = 1;
    for (int i = 0; i < strlen(b) - 1; i++)
    {
        if (b[i] == ' ')
            size++;
    }
    char* ptr2 = strtok(b, " ");
    for (int i = 0; i < size; i++)
    {
        strcpy(input[i], ptr2);
        ptr2 = strtok(NULL, " ");
    }
    calculate(&stack, input, size);
    return 0;
}

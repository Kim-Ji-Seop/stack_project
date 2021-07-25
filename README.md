# stack_project
## 스택을 연결리스트로 구현하여 계산기의 원리를 학습하는 도중 한가지 흥미로운 점을 발견했다.
<pre><code>char *transition(Stack *stack, char **s, int size, char *res)
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
}</code></pre>

## 문제는 이 함수에서 시작하여,,

<pre><code>char res[1000] = "";
    char a[100] = "( ( 3 + 4 ) * 5 ) - 5 * 7 * 5 - 5 * 10";
    int size = 1;
    for (int i = 0; i < strlen(a); i++)
    {
        if (a[i] == ' ')
            size++;
    }
    char *ptr = strtok(a, " ");
    char **input = (char **)malloc(sizeof(char *) * size);
    for (int i = 0; i < size; i++)
    {
        input[i] = (char *)malloc(sizeof(char) * 100);
    }
    for (int i = 0; i < size; i++)
    {
        strcpy(input[i], ptr);
        ptr = strtok(NULL, " ");
    }
    char b[1000] = "";
    strcpy(b, transition(&stack, input, size, res));
    printf("후위표기법: %s\n", b);
    </code></pre>
    
## 메인함수의 transition의 함수호출에서 문제가 되는 것이였다..
    
### 근데 저 코드는 오류없이 잘 돌아간다.. 내가 다 고친 코드이기 때문이다.
    
* 그럼 뭐가 문제였을까?

## 나의 원래 코드는 아래와 같았다..
<pre><code>char* transition(Stack* stack, char** s, int size)
{ //중위표기식 -> 후위표기식으로 변환해주는 함수
    char res[1000] = "";
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
}</code></pre>

## 이렇게 해서 코드를 돌렸을 때..
<pre><code>stack.c:77:12: warning: function returns address of local variable [-Wreturn-local-addr]
      return res;
            ^~~</code></pre>
            
## 이런 오류가 뜬다!
- 왜인지 이 오류를 구글링하여 찾아보았다.
- 원인은 transition함수 내의 res가 지역변수 였던 것..
 - 따라서 함수호출이 끝나면 이 res는 소멸해버리는 지역변수 였던 것이다.
## 그렇다면 당시 main에서는 어떻게 출력을 했었을까?
<pre><code>
    char b[1000] = "";
    strcpy(b, transition(&stack, input, size));
    printf("후위표기법: %s\n", b);
    size = 1;
    for (int i = 0; i < strlen(b) - 1; i++) {
        if (b[i] == ' ') size++;
    }
    char* ptr2 = strtok(b, " ");
    for (int i = 0; i < size; i++) {
        strcpy(input[i], ptr2);
        ptr2 = strtok(NULL, " ");
    }
    calculate(&stack, input, size);
    return 0;
}</code></pre>

## 너무 코드 전체를 올려버리면 보는사람에게 피해가 갈 것 같아 문제라고 생각하는 부분을 보여주겠다.

** 이 문제가 되는 코드를 돌리면 후위표기식은 출력이 잘되지만 이후에 calculation함수의 실행에서 문제가 된다.. **
 - 왜? -> res가 소멸했으니 계산할 값들이 없잖아? 그래서 0이 출력되는 기이한 일이 벌어지게 된다!

### 많은 검색을 해봤을때 해결할 수 있는 방안은 세가지가 있었다.
1. res변수를 static(정적변수)으로 두고 소멸을 스택영역에 할당(?)하는 것.
2. res변수를 동적할당으로 힙영역에 할당(?)해주는 것.
3. main함수에 res를 만들고 transition함수로 넘겨준다음 처리를 하는 것.

### 나는 3번째 방법을 선택했다.
 - 왜냐하면 굳이 이 변수하나로 동적할당과 해제를 해야한다는 번거로움을 피하고 싶었고
 - 무엇보다 정적변수나 동적메모리할당 보다 메모리의 사용을 줄이는 방법으로써
 - 파라미터를 포인터로 넘겨받아 res가 유지되도록 하는 것이 적절하다고 생각했기 때문이다.

## 간단하게 스택으로 계산기를 구현하려고 했던 내 목적이 다른 문제로 옮겨졌다..
### 하지만 오늘도 하나 배울 수 있었다. -> 리턴값을 넘겨줄 때 주의해야 할 것이 무엇인지..
  


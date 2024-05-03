#include <stdlib.h>
#include <stdio.h>


#define OPERAND 1
#define OPERATOR 2
#define FUNCTION 3
#define MAX_FORMULA 30
#define DEFNUM 32767
#define DEFSIGN '@'
#define LEFT_P 4
#define RIGHT_P 5
#define IF '?'
#define N 'N'
#define MIN 6
#define MAX 7


typedef struct Node{
    char sign;
    int num;
    struct Node* next;
}Node;

typedef struct{
    Node* top;
}Stack;

void pushSign(Stack* stack,char sign){
    Node* newNode = (Node*)malloc(sizeof(Node)); //memory beyond the scope
    newNode->num = DEFNUM;
    newNode->sign = sign;
    newNode->next = stack->top; //the order is from up to down, not the order of creation, so it points to the current top
    stack->top = newNode;
}

void pushNum(Stack* stack,int Num){
    Node* newNode = (Node*)malloc(sizeof(Node)); //memory beyond the scope
    newNode->num = Num;
    newNode->sign = DEFSIGN;
    newNode->next = stack->top; //the order is from up to down, not the order of creation, so it points to the current top
    stack->top = newNode;
}

int popNum(Stack* stack){
    if (stack->top == NULL) {
        return -1;
    }
    Node* temp = stack->top;
    int num = temp->num;
    stack->top = temp->next;
    free(temp);
    return num;
}

char popSign(Stack* stack){
    if (stack->top == NULL) {
        return -1;
    }
    Node* temp = stack->top;
    char sign = temp->sign;
    stack->top = temp->next;
    free(temp);
    return sign;
}

void iniStack(Stack* stack){
    stack->top = NULL;
}

int isEmpty(Stack* stack){
    return stack->top == NULL;
}

int tokenCheck(char token) {
    if (token >= '0' && token <= '9') {
        return OPERAND;
    }
    else if (token == '+' || token == '-' || token == '*' || token == '/') {
        return OPERATOR;
    } 
    else if (token == '('){
        return LEFT_P;
    } 
    else if (token == ')'){
        return RIGHT_P;
    }
    else if (token == '?' || token == 'N'){
        return FUNCTION;
    }
    else
        return -1;
}

int getLength(char infix[]){
    int count = 0;
    while (infix[count]!='\0') {
        count++;
    }
    return count;
}

int getPrecedence(char token){
    switch (token) {
        case '+':
            return 1;
            break;
        case '-':
            return 1;
            break;
        case '*':
            return 2;
            break;
        case '/':
            return 2;
            break;
        case 'N':
            return 3;
        case '?':
            return 4;
        default:
            return 0;
            break;
    }
}

void printStackContent(Stack* temp) {
    for (Node* currentS = temp->top; currentS != NULL; currentS = currentS->next) {
        if (currentS ->num == DEFNUM) {
            if (currentS->sign == IF) {
                printf("%c%c ",'I','F');
            }
            else printf("%c ", currentS ->sign);
            
        }
        else if(currentS->sign == DEFSIGN){
            printf("%d ", currentS ->num);
        }
        
    }
    printf("\n");
}

void printFormula(char postfix[]){
    int size = getLength(postfix);
    for (int i = 0; i < size; i++) {
        
        if (i+1 < size && postfix[i+1] == '&') {
            printf("%c",postfix[i]);
            i++;
        }
        else if (postfix[i]==IF){
            printf("%c%c ",'I','F');
            
        }
        else {
            printf("%c ",postfix[i]);
        }
    }
    
}


void calculation(char postfix[]){
    Stack temp;
    iniStack(&temp);
    int cal_index = 0;
    
    
    while (postfix[cal_index] != '\0') {
        char cur_token = postfix[cal_index];
        
        if (tokenCheck(cur_token) == OPERATOR) { // Check if the current token is an operand
            pushSign(&temp, cur_token);//push in for printing the whole stack with the operator
            printStackContent(&temp);//print the whole operator
            popSign(&temp);//push out, cause we dont use it anymore
            int op_2 = popNum(&temp);
            int op_1 = popNum(&temp);
            int result = 0;
            // Perform the operation and push the result back onto the stack
            switch (cur_token) {
                case '+':
                    result = op_1 + op_2;
                    pushNum(&temp,result);
                    break;
                case '-':
                    result = op_1 - op_2;
                    pushNum(&temp,result);
                    break;
                case '*':
                    result = op_1 * op_2;
                    pushNum(&temp,result);
                    break;
                case '/':
                    if (op_2 == 0){
                        printf("ERROR\n");
                        return;
                    }
                    else{result = op_1 / op_2;}
                    pushNum(&temp,result);
                    break;
            }
        }
        else if(tokenCheck(cur_token) == OPERAND){
            
            pushNum(&temp,cur_token - '0');
        }
        else if (cur_token == '&'){
            int tenDigi = popNum(&temp);
            pushNum(&temp, tenDigi*10 + (postfix[++cal_index] - '0'));
            
        }
        else if (tokenCheck(cur_token) == FUNCTION){
            pushSign(&temp, cur_token);
            printStackContent(&temp);
            popSign(&temp);//push out, cause we dont use it anymore
            if (cur_token == IF) {
                int c = popNum(&temp);
                int b = popNum(&temp);
                int a = popNum(&temp);
                if (a > 0) {
                    pushNum(&temp,b);
                }
                else pushNum(&temp,c);
            }
            else if (cur_token == N){
                int target = popNum(&temp);
                target = - target;
                pushNum(&temp, target);
            }
            
        }
        cal_index++;
    }

    // The final result should be the only element left on the stack
    int finalResult = popNum(&temp);
    printf("%d\n\n", finalResult);
}


void transformation(char formula[],char postfix[]){
    int index = 0;
    int len = getLength(formula);
    Stack Operator;
    iniStack(&Operator);
    for (int i = 0; i<len; i++) {
        char currentToken = formula[i];
        if (currentToken == '&') {
            postfix[index++]=currentToken;
        }
        
        if (tokenCheck(currentToken) == OPERATOR ||tokenCheck(currentToken) == FUNCTION) {
           
            while(!isEmpty(&Operator) && getPrecedence(currentToken)<=getPrecedence(Operator.top->sign)) {
                if ((postfix[0]=='\0' && currentToken == N) || (currentToken == N && Operator.top->sign == N)) {
                    break;
                }
                else postfix[index++]=popSign(&Operator);
            }
            pushSign(&Operator, currentToken);
           
        }
        else if (tokenCheck(currentToken)==OPERAND)
            postfix[index++] = currentToken;
        else if (tokenCheck(currentToken)==LEFT_P)
            pushSign(&Operator, currentToken);
        else if (tokenCheck(currentToken)==RIGHT_P){
            while (!isEmpty(&Operator) && tokenCheck(Operator.top->sign)!=LEFT_P) {
                postfix[index++] = popSign(&Operator);
            }
            popSign(&Operator);//pop the left_P out.
            if ((!isEmpty(&Operator) && Operator.top->sign == IF) ||(!isEmpty(&Operator) && Operator.top->sign == N)) {
                postfix[index++] = popSign(&Operator);
            }
        }
//      pop all the operators and functions before left_p
        else if (currentToken == ','){
            while (!isEmpty(&Operator) && tokenCheck(Operator.top->sign) != LEFT_P) {
                postfix[index++]=popSign(&Operator);
            }
        }
        
        
    }
    while (!isEmpty(&Operator)) {
        postfix[index++]=popSign(&Operator);
    }
    postfix[index] = '\0';
    
    printFormula(postfix);
    printf("\n");
    calculation(postfix);
        
}



int main(void) {
    int n;
    scanf("%d",&n);
    getchar();
    for (int i = 0; i < n; i++) {
        char currentChar;
        char lastChar = '\0';
//        char formula[10000]={0};
//        char postfix[10000]={0};
        char *formula = (char*)calloc(10000, sizeof(char));
        char *postfix = (char*)calloc(10000, sizeof(char));
        int charIndex = 0;
        while ((currentChar = getchar()) != '.') {
            if (currentChar == '\n' || currentChar == '\t') {
                continue;
            }
            if (currentChar == ' '){
                lastChar = ' ';
                continue;
            }
            if (tokenCheck(lastChar) == OPERAND) {
                //formula[charIndex-1] = (((lastChar - '0') * 10) + (currentChar - '0')) + '0';
                formula[charIndex++] = '&';
                formula[charIndex++] = currentChar;
            }
            else if (currentChar == 'F'){
                formula[charIndex-1] = '?';//use ? represent 'IF', so overwrite the 'I' with "?"
            }
            else
                formula[charIndex++] = currentChar; //store the ','
                lastChar = currentChar;
        }
        getchar();
        formula[charIndex] = '\0';
        transformation(formula, postfix);
        free(formula);
        free(postfix);
    }

    return 0;
}


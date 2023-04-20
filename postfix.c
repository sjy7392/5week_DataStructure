#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위는 내림차순, lparen = 0 가장 낮음 */
typedef enum {
	lparen = 0, /* ( 왼쪽 괄호 */
	rparen = 9, /* ) 오른쪽 괄호*/
	times = 7, /* * 곱셈 */
	divide = 6, /* / 나눗셈 */
	plus = 5, /* + 덧셈 */
	minus = 4, /* - 뺄셈 */
	operand = 1 /* 피연산자 */
}precedence;

char infixExp[MAX_EXPRESSION_SIZE]; //infix 식 저장
char postfixExp[MAX_EXPRESSION_SIZE]; //변경된 postfix 문자 저장
char postfixStack[MAX_STACK_SIZE]; //postfix 변환 위한 스택
int evalStack[MAX_STACK_SIZE]; // 계산 위한 스택
int postfixStackTop = -1; //postfixStack의 Top
int evalStackTop = -1; //evalStack의 Top
int evalResult = 0; //계산 결과

//함수 프로토타입 선언
void postfixPush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main() {
	char command;
	do {
		printf("----------------------------------------------------------------\n");
		printf(" Infix to Postfix, then Evaluation\n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i, Postfix=p, Eval=e, Debug=d, Reset=r, Quit=q \n");
		printf("----------------------------------------------------------------\n");
    	printf("[----- [Son Ji Yeon] [2020039104] -----]\n");

		printf("Command = "); //"Command = " 출력
		scanf(" %c", &command); //command 입력받기

		switch(command) {
			case 'i': case 'I':
			    getInfix(); //getInfix() 실행
			    break;
			case 'p': case 'P':
                toPostfix(); //toPostfix() 실행
                break;
			case 'e': case 'E':
                evaluation(); //evaluation() 실행
                break;
			case 'd': case 'D':
                debug(); //debug() 실행
                break;
			case 'r': case 'R':
                reset(); //reset() 실행
                break;
			case 'q': case 'Q':
			    break;
			default:
                printf("\n >>>>> Concentration!! <<<<<\n");
                break;
		}
	}
	while(command != 'q' && command != 'Q'); //q나 Q를 작성하지 않을 동안 실행
	return 1;
}

//postfixStack 배열에 x 저장
void postfixPush(char x) {
	postfixStack[++postfixStackTop] = x; 
}

//postfixStack 배열에서 값을 꺼내 반환
char postfixPop() {
	char x;
	if(postfixStackTop == -1)
	 return '\0'; else {
		x = postfixStack[postfixStackTop--];
	}
	return x;
}

//evalStack 배열에 x 값을 저장
void evalPush(int x) {
	evalStack[++evalStackTop] = x;
}

//evalStack 배열에서 값을 꺼내 반환
int evalPop() {
	if(evalStackTop == -1)
	 	return -1;
	else
		return evalStack[evalStackTop--];
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix() {
	printf("Type the expression >>> ");
	scanf("%s",infixExp);
}

//symbol의 문자열에 맞는 값 반환
precedence getToken(char symbol) {
	switch(symbol) {
		case '(' : return lparen;
		case ')' : return rparen;
		case '+' : return plus;
		case '-' : return minus;
		case '/' : return divide;
		case '*' : return times;
		default : return operand;
	}
}

//x의 우선순위 반환
precedence getPriority(char x) {
	return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c) {
	if (postfixExp == '\0')
	    strncpy(postfixExp, c, 1);
    else
	    strncat(postfixExp, c, 1);
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix() {
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;
	char x;
	/* 문자하나를 임시로 저장하기 위한 변수 */
	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0') { //exp 문자열의 끝에 갈 때까지 반복
		if(getPriority(*exp) == operand) { //현재 문자가 피연산자인 경우
			x = *exp; //x에 현재 문자 저장
			charCat(&x); //postfixExp에 x 추가저장
		} else if(getPriority(*exp) == lparen) { //현재 문자가 '('일 경우
			postfixPush(*exp); //postfixStack에 push
		} else if(getPriority(*exp) == rparen) { //현재 문자가 ')'일 경우
			while((x = postfixPop()) != '(') { //스택에서 '('가 나올 때까지 pop
				charCat(&x); //charCat 함수 호출해 pop한 문자 postfixExp에 추가
			}
		} else {
			while(getPriority(postfixStack[postfixStackTop]) >=
			getPriority(*exp)) { //스택의 top에 있는 연산자 우선순위가 현재 문자 우선순위보다 크거나 같을 동안
				x = postfixPop(); //스택에서 연산자를 pop해서 x변수에 저장
				charCat(&x); //postfixExp에 x의 연산자 저장
			}
			postfixPush(*exp); //스택에 push
		}
		exp++; //다음 문자로 이동
	}
	while(postfixStackTop != -1) { //수식 모두 처리한 후
		x = postfixPop(); //x에 pop한 값 저장
		charCat(&x); //x 변수에 저장된 연산자를 postfixExp에 추가
	}
}

//결과 출력
void debug() {
	printf("\n---DEBUG\n");
	printf("infixExp = %s\n", infixExp);
	printf("postExp = %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for (int i = 0; i < MAX_STACK_SIZE; i++)
	    printf("%c ", postfixStack[i]);
	printf("\n");
}

//리셋(초기화)
void reset() {
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for (int i = 0; i < MAX_STACK_SIZE; i++)
	    postfixStack[i] = '\0';

	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

//postfixExp Stack 계산
void evaluation() {
	int opr1, opr2, i;

	int length = strlen(postfixExp);
	char symbol;
	evalStackTop = -1;

	for (i = 0; i < length; i++) {
		symbol = postfixExp[i];
		if(getToken(symbol) == operand) { //피연산자일 경우
			evalPush(symbol - '0'); //숫자를 스택에 push
		} else {
			opr2 = evalPop(); //피연산자 pop
			opr1 = evalPop(); //피연산자 pop
			switch(getToken(symbol)) { // 연산자에 따라 연산 수행
				case plus: evalPush(opr1 + opr2);
				    break;
				case minus: evalPush(opr1 - opr2);
				    break;
				case times: evalPush(opr1 * opr2);
				    break;
				case divide: evalPush(opr1 / opr2);
				    break;
				default: break;
			}
		}
	}
	evalResult = evalPop(); //결과 pop 해 evalResult에 저장
}
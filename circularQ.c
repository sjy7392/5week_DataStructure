/**
 * circularQ.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 //원형 큐 최대 사이즈

typedef char element;
typedef struct {
	element queue[MAX_QUEUE_SIZE];
	int front, rear;
}QueueType; //원형 큐 구조체 선언

//함수 프로토타입 선언
QueueType *createQueue(); //원형 큐 생성
int freeQueue(QueueType *cQ); //원형 큐 해제
int isEmpty(QueueType *cQ); //원형 큐 비었는지 확인
int isFull(QueueType *cQ); //원형 큐 찼는지 확인
void enQueue(QueueType *cQ, element item); //원형 큐 요소 삽입
void deQueue(QueueType *cQ, element* item); //원형 큐 요소 삭제
void printQ(QueueType *cQ); //원형 큐 출력
void debugQ(QueueType *cQ); //원형 큐 디버그 모드로 출력
element getElement(); //사용자에게 요소 입력받음

int main(void) {
	QueueType *cQ = createQueue(); //원형 큐 생성
	element data; //입력받은 요소 저장할 변수

	char command; //입력받은 명령어 저장할 변수

	do {
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); //명령어 입력받음

		switch(command) {
			case 'i': case 'I': //i나 I 입력할 시
				data = getElement(); //요소 입력받음
			    enQueue(cQ, data); //enQueue 함수 실행
			    break;
			case 'd': case 'D': //d나 D 입력할 시
				deQueue(cQ,&data); //deQueue 함수 실행
			    break;
			case 'p': case 'P': //p나 P 입력할 시
				printQ(cQ); //printQ 함수 실행
			    break;
			case 'b': case 'B': //b나 B 입력할 시
				debugQ(cQ); //debugQ 실행
			    break;
			case 'q': case 'Q': //q나 Q 입력할 시
				break; //프로그램 종료
			default:
				printf("\n>>>>>Concentration!!<<<<<\n");
			    break;
		}
	}
	while(command != 'q' && command != 'Q'); //q나 Q가 아닌 것을 입력할 동안 작동
	return 1;
}

QueueType *createQueue() {
	QueueType *cQ; //QueType형 원형 큐 생성
	cQ = (QueueType *)malloc(sizeof(QueueType)); //원형 큐에 메모리 동적할당
	cQ->front = 0; //front 초기화
	cQ->rear = 0; //rear 초기화
	return cQ; //큐 포인터 반환
}

int freeQueue(QueueType *cQ) {
	if(cQ == NULL) return 1; //cQ가 NULL일 경우 1 반환
	free(cQ); //cQ 메모리 해제
	return 1; //1 반환
}

element getElement() {
	element item; //element형 item 선언
	printf("Input element = "); //"Input element =" 출력
	scanf(" %c", &item); //사용자한테 item 입력받음
	return item; //item값 반환
}

int isEmpty(QueueType *cQ) {
	if (cQ->front == cQ->rear) { //원형 큐의 front와 rear가 같다면
		printf("Circular Queue is empty!"); //"Circular Queue is empty!" 출력
		return 1; //1 반환
	} else return 0; //그렇지 않음 0 반환
}

int isFull(QueueType *cQ) {
	if (((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front) { //rear의 다음 위치가 front와 같다면
		printf("Circular Queue is full!"); //"Circular Queue is full!" 출력
		return 1; //1 반환
	} else return 0; //그렇지 않음 0 반환
}

void enQueue(QueueType *cQ, element item) {
	if(isFull(cQ)) return; //만약 큐가 가득 찼다면 함수 종료
    else {
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; //rear를 한칸 뒤로 이동 후 배열 사이즈 초과하지 않게 원형 큐 SIZE로 나머지 연산
		cQ->queue[cQ->rear] = item; //rear의 위치에 item 저장
	}
}

void deQueue(QueueType *cQ, element *item) {
	if(isEmpty(cQ)) return; //만약 큐가 비어있다면 함수 종료
    else {
		cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE; //front를 한 칸 뒤로 이동 후 배열사이즈 초과하지 않게 원형 큐 size로 나머지 연산
		*item = cQ->queue[cQ->front]; //front 위치 값을 item에 저장
		return;
	}
}

void printQ(QueueType *cQ) {
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; //front의 다음 위치를 first에 저장
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; //rear의 다음 위치를 last에 저장

	printf("Circular Queue : [");
    
	i = first; //i에 first 대입
	while(i != last) { //i가 last와 같지 않을 동안 반복
		printf("%3c", cQ->queue[i]); //원형 큐의 i번째 값을 출력
		i = (i+1)%MAX_QUEUE_SIZE; //i를 다음 위치로 변경
	}
	printf(" ]\n");
}

void debugQ(QueueType *cQ) {
	printf("\n---DEBUG\n");
	for (int i = 0; i < MAX_QUEUE_SIZE; i++) { //원형 큐 사이즈만큼 반복문 실행
		if(i == cQ->front) { //i가 원형 큐의 front라면
			printf("  [%d] = front\n", i); //front 출력
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); //그렇지 않으면 원형 큐의 인덱스와 요소 출력
	}
	//printQ(cQ);
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); //원형 큐의 front와 rear 출력
}
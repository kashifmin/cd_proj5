#include <stdlib.h>
#define STACK_MAX 300
struct stack {
	int items[STACK_MAX];
	int top;
};

void push(struct stack *theStack, int item) {
	if(theStack->top < STACK_MAX)
		theStack->items[++theStack->top] = item;
	else {
		printf("Stack Overflow!\n");
		exit(0);
	}
}

int pop(struct stack *theStack) {
	if(theStack->top == -1) {
		printf("Stack Underflow!\n");
		exit(0);
	} else {
		return theStack->top--;
	}
}
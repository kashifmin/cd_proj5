#include <stdio.h>
#include "stack.h"
#include "tokens.c"

struct production {
    int number; // The column number in parse table of the LHS
    int symLength; // count of terminals on the RHS i.e |a| in S-->a
    char prodString[30]; // holds the production string in the form of "S --> aAb"
} productions[17];

void initProductions() {
/*
 * Here we store all the productions
 */
    productions[0].number = 0;
    productions[0].symLength = 1;
    strcpy(productions[0].prodString, "S' --> S");

    productions[1].number = 14;
    productions[1].symLength = 3;
    strcpy(productions[1].prodString, "S --> begin A end");

    productions[2].number = 15;
    productions[2].symLength = 1;
    strcpy(productions[2].prodString, "A --> B");

    productions[3].number = 15;
    productions[3].symLength = 2;
    strcpy(productions[3].prodString, "A --> AB");

    productions[4].number = 16;
    productions[4].symLength = 3;
    strcpy(productions[4].prodString, "B --> id := D");

    productions[5].number = 16;
    productions[5].symLength = 2;
    strcpy(productions[5].prodString, "B --> EF");

    productions[6].number = 16;
    productions[6].symLength = 1;
    strcpy(productions[6].prodString, "B --> G");

    productions[7].number = 16;
    productions[7].symLength = 1;
    strcpy(productions[7].prodString, "B --> H");

    productions[8].number = 17;
    productions[8].symLength = 1;
    strcpy(productions[8].prodString, "D --> num");

    productions[9].number = 17;
    productions[9].symLength = 1;
    strcpy(productions[9].prodString, "D --> str");

    productions[10].number = 18;
    productions[10].symLength = 1;
    strcpy(productions[10].prodString, "E --> integer");

    productions[11].number = 18;
    productions[11].symLength = 1;
    strcpy(productions[11].prodString, "E --> real");

    productions[12].number = 18;
    productions[12].symLength = 1;
    strcpy(productions[12].prodString, "E --> string");

    productions[13].number = 19;
    productions[13].symLength = 1;
    strcpy(productions[13].prodString, "F --> id");

    productions[14].number = 19;
    productions[14].symLength = 3;
    strcpy(productions[14].prodString, "F --> F , id");

    productions[15].number = 20;
    productions[15].symLength = 2;
    strcpy(productions[15].prodString, "G --> print str");

    productions[16].number = 21;
    productions[16].symLength = 7;
    strcpy(productions[16].prodString, "H --> for id := num to num G");
}


/*
 * This stores what each column means in the actionTable.
 * ONLY FOR REFERENCE
 
 char aTColumns[14][20] = {
    "BEGIN", //0
    "END", //1
    "ID", //2
    ":=", //3
    "NUM", //4
    "STR LITERAL", //5
    "INTEGER", //6
    "REAL", //7
    "STRING", //8
    "COMMA", //9
    "PRINT", //10
    "FOR", //11
    "TO", //12
    "$" //13
 };
 */

/*
 *
 * Below is an action table (CLR parse table) for the grammar to parse the tokens
 * 0 --> empty
 * postive i--> goto state i
 * negative i --> reduce by production number i
 * -50 --> Accept!
 */
int actionTable[31][22] = {
/*   0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13,   14, 15, 16, 17, 18, 19, 20, 21   */
    {2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    1,  0,  0,  0,  0,  0,  0,  0},  //0
    {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -50,   0,  0,  0,  0,  0,  0,  0,  0},  //1
    {0,  0,  5,  0,  0,  0,  9, 10, 11,  0, 12, 13,  0,  0,    0,  3,  4,  0,  6,  0,  7,  8},  //2
    {0, 14,  5,  0,  0,  0,  9, 10, 11,  0, 12, 13,  0,  0,    0,  0, 15,  0,  6,  0,  7,  8},  //3
    {0, -2, -2,  0,  0,  0, -2, -2, -2,  0, -2, -2,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //4
    {0,  0,  0, 16,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //5
    {0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 17,  0,  0},  //6
    {0, -6, -6,  0,  0,  0, -6, -6, -6,  0, -6, -6,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //7
    {0, -7, -7,  0,  0,  0, -7, -7, -7,  0, -7, -7,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //8
    {0,  0, -10, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //9
    {0,  0, -11, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //10
    {0,  0, -12, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //11
    {0,  0,  0,  0,  0, 19,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //12
    {0,  0, 20,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //13
    {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,    0,  0,  0,  0,  0,  0,  0,  0},  //14
    {0, -3, -3,  0,  0,  0, -3, -3, -3,  0, -3, -3,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //15
    {0,  0,  0,  0, 22, 23,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0, 21,  0,  0,  0,  0},  //16
    {0, -5, -5,  0,  0,  0, -5, -5, -5, 24, -5, -5,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //17
    {0,-13,-13,  0,  0,  0,-13,-13,-13,-13,-13,-13,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //18
    {0,-15,-15,  0,  0,  0,-15,-15,-15,  0,-15,-15,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //19
    {0,  0,  0, 25,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //20
    {0, -4, -4,  0,  0,  0, -4, -4, -4,  0, -4, -4,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //21
    {0, -8, -8,  0,  0,  0, -8, -8, -8,  0, -8, -8,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //22
    {0, -9, -9,  0,  0,  0, -9, -9, -9,  0, -9, -9,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //23
    {0,  0, 26,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //24
    {0,  0,  0,  0, 27,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //25
    {0,-14,-14,  0,  0,  0,-14,-14,-14,-14,-14,-14,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //26
    {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 28,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //27
    {0,  0,  0,  0, 29,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //28
    {0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12,  0,  0,  0,    0,  0,  0,  0,  0,  0, 30,  0},  //29
    {0,-16,-16,  0,  0,  0,-16,-16,-16,  0,-16,-16,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0}   //30
};

void parse() {
    int i;
    int currToken;
    int stackTop;
    int action;
    struct production reduceBy; // the production to reduce by
    // uses actionTable and identifiedTokens to check if the program is valid or not
    struct stack pStack;
    pStack.top = -1;
    push(&pStack, 0);

    currToken = nextToken();

    while(1) {
        stackTop = getStackTop(&pStack);
        action = actionTable[stackTop][currToken];

        printf("Proccessing token %s\n", tokenList[currToken]);

        if(action > 0) { // Shift
            printf("Shift and go to state %d\n", action);
            push(&pStack, currToken);
            push(&pStack, action); // push j to the stack
            currToken = nextToken(); // advance input
        } else if(action == -50) {
            printf("\n****************ACCEPT!****************\n");
            return;
        } else if(action < 0) { // Reduce
            reduceBy = productions[-action];
            printf("Reduce by production %s\n", reduceBy.prodString);

            // we have to pop 2*|a| items from the stack given by production number '|action|'
            for(i=1; i <= 2 * reduceBy.symLength; i++)
                pop(&pStack);

            stackTop = getStackTop(&pStack); // let this be the topmost element, S
            push(&pStack, reduceBy.number); // Push A to the stack where action = reduce by A --> a 
            push(&pStack, actionTable[stackTop][reduceBy.number]); // push action[S, A] on to the stack


        } else {
            printf("\n****************PARSING ERROR!****************\n");
            return;
        }
    }

}

int main(int argc, char* argv[])
{
	int i;

    if(argc != 2) {
        printf("Usage: cmd input\n");
        exit(0);
    }

    initLexer(argv[1]);

    initProductions();

    // Remove the comments below if you want to print only tokens
    
  
    printf("\n*********OUTPUT FROM LEXER***********\n");
    while(forward < codeLength-1) {
    	printf("\n%s", tokenList[nextToken()]);
    }
    printf("\n");
    forward = -1; // reset the forward pointer so that the parser can read using nextToken() again


    printf("\n*********OUTPUT FROM PARSER***********\n\n");
    parse();


    return 0;

}

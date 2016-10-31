#include <stdio.h>
#include "stack.h"
#include "tokens.c"

struct production {
    int number; // The column number in parse table of the LHS
    int symLength; // count of terminals on the RHS i.e |a| in S-->a
} productions[17];

void initProductions() {
/*
 * Here we store all the productions
 */
    productions[0].number = 0;
    productions[0].symLength = 1;

    productions[1].number = 14;
    productions[1].symLength = 3;

    productions[2].number = 15;
    productions[2].symLength = 1;

    productions[3].number = 15;
    productions[3].symLength = 2;

    productions[4].number = 16;
    productions[4].symLength = 3;

    productions[5].number = 16;
    productions[5].symLength = 2;

    productions[6].number = 16;
    productions[6].symLength = 1;

    productions[7].number = 16;
    productions[7].symLength = 1;

    productions[8].number = 17;
    productions[8].symLength = 1;

    productions[8].number = 17;
    productions[8].symLength = 1;

    productions[9].number = 17;
    productions[9].symLength = 1;

    productions[10].number = 18;
    productions[10].symLength = 1;

    productions[11].number = 18;
    productions[11].symLength = 1;

    productions[12].number = 18;
    productions[12].symLength = 1;

    productions[13].number = 19;
    productions[13].symLength = 1;

    productions[14].number = 19;
    productions[14].symLength = 3;

    productions[15].number = 20;
    productions[15].symLength = 2;

    productions[16].number = 21;
    productions[16].symLength = 7;
}


/*
 * This stores what each column means in the actionTable.
 */
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

/*
 *
 * Below is an action table (parse table) for the grammar to parse the tokens
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
    {0, -2,  0,  0,  0,  0, -2, -2, -2,  0, -2, -2,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //4
    {0,  0,  0, 16,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //5
    {0,  0, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 17,  0,  0},  //6
    {0, -6,  0,  0,  0,  0, -6, -6, -6,  0, -6, -6,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //7
    {0, -7,  0,  0,  0,  0, -7, -7, -7,  0, -7, -7,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //8
    {0,  0, -10, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //9
    {0,  0, -11, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //10
    {0,  0, -12, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //11
    {0,  0,  0,  0,  0, 19,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //12
    {0,  0, 20,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //13
    {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,    0,  0,  0,  0,  0,  0,  0,  0},  //14
    {0, -3,  0,  0,  0,  0, -3, -3, -3,  0, -3, -3,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //15
    {0,  0,  0,  0, 22, 23,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0, 21,  0,  0,  0,  0},  //16
    {0, -5,  0,  0,  0,  0, -5, -5, -5,-24, -5, -5,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //17
    {0,-13,  0,  0,  0,  0,-13,-13,-13,-13,-13,-13,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //18
    {0,-15,  0,  0,  0,  0,-15,-15,-15,  0,-15,-15,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //19
    {0,  0,  0, 25,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //20
    {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //21
    {0, -8,  0,  0,  0,  0, -8, -8, -8,  0, -8, -8,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //22
    {0, -9,  0,  0,  0,  0, -9, -9, -9,  0, -9, -9,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //23
    {0,-14, 26,  0,  0,  0,-14,-14,-14,-14,-14,-14,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //24
    {0,  0,  0,  0, 27,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //25
    {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //26
    {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 28,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //27
    {0,  0,  0,  0, 29,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0},  //28
    {0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12,  0,  0,  0,    0,  0,  0,  0,  0,  0, 30,  0},  //29
    {0,-16,  0,  0,  0,  0,-16,-16,-16,  0,-16,-16,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0}   //30
};

void parse() {
    // uses actionTable and identifiedTokens to check if the program is valid or not
}

int main(int argc, char* argv[])
{
	int i;

    if(argc != 2) {
        printf("Usage: cmd input\n");
        exit(0);
    }
    tokenize(argv[1]);

    printf("\nInfo from token table  \n");
    for(i=0; i<itCount; i++) {
        printf("%s\n", tokenList[identifiedTokens[i]]); // print token indices
    	//printf("%s\n", tokenList[i]); // print tokens in the identified order
    }

    initProductions();


    return 0;

}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp;
char ch;
char inputCode[300];
char idTable[50][30] /*,numTable[50] */;
int codeLength = 0;

 char tokenList[14][20] = {
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

int identifiedTokens[100]; // Stores indices of tokens after lexical analysis
int itCount = 0;

char symTable[50][30];
int idCount = 0;
int numCount = 0;
int forward = -1;
int lexeme_beg = -1;
int state = 0, start = 0;
int lex_val;

char nextChar();
void retract(int);

int installId()
{
    int i;
    char id[30];
    for(i=lexeme_beg; i<=forward; i++) {
        id[i-lexeme_beg] = inputCode[i];
    }
    id[i-lexeme_beg] = '\0';
    strcpy(idTable[idCount++], id);
    
    return idCount-1; 
}

/*
int installNum()
{
    int i;
    char id[30];
    for(i=lexeme_beg; i<=forward; i++)
        id[lexeme_beg-i] = inputCode[i];
    strcpy(numTable[idCount++], id);
    return numCount-1; 
}
*/

int fail() {
    switch(state) {
        case 0:
            retract(1);
            start = 40;
            break;

        case 40:
            retract(1);
            start = 43;
            break;

        case 43:
            retract(1);
            start = 51;
            break;

        case 51:
            //exit(0);
            retract(1);
            start = 62; // enter state diagram for NUMBERS
            break;

        case 62:
            start = 0;

        // TODO: Handle fail() from other states
        case 1:
        case 2:
        case 3:
        case 4:
        case 31:
        case 11:
            forward = lexeme_beg;
            start = 40;
            break;


        default:
            printf("Error at state %d\n", state);
            exit(0);

    }

    return start;
}

int nextToken() {

    state = 0;

    while(1) {

        switch(state) {

        /*** IDENTIFIERS ***/
            case 40:
                ch = nextChar();
                if(isalpha(ch)) {
                    state = 41;
                }
                else 
                    state = fail();
                break;
            case 41:
                ch = nextChar();
                if(isalpha(ch) || isdigit(ch)) {
                    state = 41;
                }
                else {
                    state = 42;
                }
                break;
            case 42:
                retract(1);
                installId();
                return 2;

        /*** END IDENTIFIERS ***/


        /*** STRING LITERALS ***/

            case 43:
                ch = nextChar();
                if(ch == 34) // double quote
                    state = 44;
                else state = fail();
                break;

            case 44:
                ch = nextChar();
                if(ch == '\"') { // do not change the state until we get a double quote
                    state = 45;
                } else {
                    state = 44; 
                }
                break;

            case 45:
                return 5;


        /*** END STRING LITERALS ***/

        /*** ASSIGNMENT OPERATOR ***/

            case 51:
                
                ch = nextChar();
                if(ch == ':')
                    state = 52;
                else
                    state = fail();
                break;

            case 52:
                ch = nextChar();
                if(ch == '=')
                    state = 53;
                else
                    state = fail();
                break;
            case 53:
                return 3;
        /*** END ASSIGNMENT OPERATOR ***/

        /*** NUMBERS OR NUMERIC LITERALS ***/

            case 62:
                ch = nextChar();
                if(ch == '+' || ch == '-')
                    state = 54;
                else if(isdigit(ch))
                    state = 55;
                else state = fail();
                break;

            case 54:
                ch = nextChar();
                if(isdigit(ch))
                    state = 55;
                else
                    state = fail();
                break;
            case 55:
                ch = nextChar();
                if(isdigit(ch))
                    //do nothing actually
                    state = 55;
                else if(ch == '.')
                    state = 56;
                else if(ch == 'E')
                    state = 58;
                else 
                    state = 61;
                break;

            case 56:
                ch = nextChar();
                if(isdigit(ch))
                    state = 57;
                else
                    state = fail();
                break;
            case 57:
                ch = nextChar();
                if(isdigit(ch))
                    state = 57;
                else if(ch == 'E')
                    state = 58;
                else
                    state = 61;
                break;
            case 58:
                ch = nextChar();
                if(ch == '+' || ch == '-')
                    state = 59;
                else if(isdigit(ch))
                    state = 60;
                else state = fail();
                break;
            case 59:
                ch = nextChar();
                if(isdigit(ch))
                    state = 60;
                else
                    state = fail();
                break;
            case 60:
                ch = nextChar();
                if(isdigit(ch))
                    state = 60;
                else
                    state = 61;
                break;
            case 61:
                retract(1);
                return 4;

            /*** END NUMBERS OR NUMERIC LITERALS ***/


            case 0: 
                ch = nextChar();
                if(ch == '\0') {
                    return 13;
                }
                if(ch == '\n' || ch == '\t' || ch == ' ') {
                    state = 0;
                    lexeme_beg = forward;
                }
                else if(ch == ',')
                    return 9;
                else if(ch == 'B')
                    state = 1;
                else if(ch == 'P')
                    state = 6;
                else if(ch == 'I')
                    state = 11;
                else if(ch == 'R')
                    state = 18;
                else if(ch == 'S')
                    state = 22; 
                else if(ch == 'F')
                    state = 28; 
                else if(ch == 'E')
                    state = 31;  
                else if(ch == 'T')
                    state = 36;
                
                else {
                    state = fail();
                }
                break;
            case 1:
                ch = nextChar();
                if(ch == 'E')
                    state = 2;
                else state = fail();
                break;
            case 2:
                ch = nextChar();
                if(ch == 'G')
                    state = 3;
                else state = fail();
                break;
            case 3:
                ch = nextChar();
                if(ch == 'I')
                    state = 4;
                else state = fail();
                break;
            case 4:
                ch = nextChar();
                if(ch == 'N')
                    state = 5;
                else state = fail();
                break;

            case 5: 
                return 0;
            case 6:
                ch = nextChar();
                if(ch == 'R')
                    state = 7;
                else state = fail();
                break;
            case 7:
                ch = nextChar();
                if(ch == 'I')
                    state = 8;
                else state = fail();
                break;
            case 8:
                ch = nextChar();
                if(ch == 'N')
                    state = 9;
                else state = fail();
                break;
            case 9:
                ch = nextChar();
                if(ch == 'T')
                    state = 10;
                else state = fail();
                break;

            case 10:    
                
                return 10;
            case 11:
                ch = nextChar();
                if(ch == 'N')
                    state = 12;
                else state = fail();
                break;
            case 12:
                ch = nextChar();
                if(ch == 'T')
                    state = 13;
                else state = fail();
                break;
            case 13:
                ch = nextChar();
                if(ch == 'E')
                    state = 14;
                else state = fail();
                break;
            case 14:
                ch = nextChar();
                if(ch == 'G')
                    state = 15;
                else state = fail();
                break;
            case 15:
                ch = nextChar();
                if(ch == 'E')
                    state = 16;
                else state = fail();
                break;
            case 16:
                ch = nextChar();
                if(ch == 'R')
                    state = 17;
                else state = fail();
                break;
            case 17:    
                
                return 6;
            case 18:
                ch = nextChar();
                if(ch == 'E')
                    state = 19;
                else state = fail();
                break;
            case 19:
                ch = nextChar();
                if(ch == 'A')
                    state = 20;
                else state = fail();
                break;
            case 20:
                ch = nextChar();
                if(ch == 'L')
                    state = 21;
                else state = fail();
                break;
            case 21:    
                
                return 7;
            case 22:
                ch = nextChar();
                if(ch == 'T')
                    state = 23;
                else state = fail();
                break;
            case 23:
                ch = nextChar();
                if(ch == 'R')
                    state = 24;
                else state = fail();
                break;
            case 24:
                ch = nextChar();
                if(ch == 'I')
                    state = 25;
                else state = fail();
                break;
            case 25:
                ch = nextChar();
                if(ch == 'N')
                    state = 26;
                else state = fail();
                break;
            case 26:
                ch = nextChar();
                if(ch == 'G')
                    state = 27;
                else state = fail();
                break;
            case 27:    
                return 8;       
            case 28:
                ch = nextChar();
                if(ch == 'O')
                    state = 29;
                else state = fail();
                break;
            case 29:
                ch = nextChar();
                if(ch == 'R')
                    state = 30;
                else state = fail();
                break;
            case 30:    
                return 11;
            case 31:
                ch = nextChar();
                if(ch == 'N')
                    state = 32;
                else state = fail();
                break;
            case 32:
                ch = nextChar();
                if(ch == 'D')
                    state = 33;
                else state = fail();
                break;
            case 33:    
                
                return 1;

            case 36:
                ch = nextChar();
                if(ch == 'O')
                    state=37;
                else 
                    state = fail();
                break;
            case 37:
                return 12;       

            default:
                return 0;

        }

    }

}

char nextChar() {
    return inputCode[++forward];
}

void retract(int f) {
    forward = forward - f;
}

void tokenize(char* inputFile) {

    int tCount = 0;
    int i = 0;
    int tokens[100];

    fp = fopen(inputFile, "r");

    while(!feof(fp)) {
        inputCode[codeLength++] = fgetc(fp);
    }

    inputCode[codeLength-1] = '\0';
}
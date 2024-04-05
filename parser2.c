#define _CRT_SECURE_NO_WARNINGS
/*parser.c 단순 산술식에 대한 구문 분석기 시스템*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

extern char lexeme[100];
extern int nextToken;
extern FILE* in_fp;
int value;

//토큰 코드들
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

//함수선언
int expr();
int term();
int factor();
int lex();
void error();
void getChar();

/*함수 expr
    다음 규칙으로 생성되는 언어의 문자열을 파싱한다
    <expr> -> <term> {(+|-) <term> }
    */
int expr() {
    /*첫번째 term을 파싱한다.*/
    int value = term();
    printf("Enter <expr>\n");
    /*다음 번째 토큰이 + 또는 - 이면 다음 번째 토큰을 가져오고, 다음 번째 term을 파싱한다.*/
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        int op = nextToken;
        lex();
        int nextTerm = term();
        if (op == ADD_OP) {
            value += nextTerm;
        }
        else {
            value -= nextTerm;
        }
    }
    return value;
    printf("Exit <expr>\n");
}

/*
함수 term
다음 규칙으로 생성되는 언어의 문자열을 파싱한다:
<term> -> <factor> {( * | / ) <factor> }
*/

int term() {
    /*첫번째 factor을 파싱한다.*/
    int value = factor();
    printf("Enter <term>\n");
    /*다음번째 토큰이 * 또는 / 이면 lex를 호출하여 다음 번째 토큰을 가져오고, 다음 번째 term을 파싱한다 */
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        int op = nextToken;
        lex();
        int nextFactor = factor();
        if (op == MULT_OP) {
            value *= nextFactor;
        }
        else {
            value /= nextFactor;
        }
    }
    return value;
    printf("Exit <term>\n");
}

/*
factor함수
다음 규칙으로 생성되는 언어의 문자열을 파싱한다.
<factor> -> id | int_constatn | (<expr>)
*/

int factor() {
    int value = 0;
    printf("Enter <factor>\n");
    /*어느 RHS를 파싱할 것인지를 결정한다.*/
    if (nextToken == IDENT || nextToken == INT_LIT) {
        value = atoi(lexeme);
        lex(); //다음 토큰을 가져온다.

    }
    /*RHS가 (<expr>)이면 lex 를 호출하여 왼쪽 괄호를 전달하고, expr을 호출하고 오른쪽 괄호를 검사한다.*/
    else if (nextToken == LEFT_PAREN) {
        lex();
        value = expr();
        if (nextToken == RIGHT_PAREN)
            lex();
        else
            error();
    }// if (nextToken == ...의 끝)
    // id, 정수리터럴, 또는 왼쪽괄호가 아니다
    else
        error();

    printf("Exit <factor>\n");
    return value;
}

int main() {
    if ((in_fp = fopen("input.txt", "r")) == NULL)
        printf("ERROR-cannot open front.in \n");
    else {
        getChar();
        do {
            lex();
            value = expr();
        } while (nextToken != EOF);
        printf("**********************************************\n\n\n");
        printf("value: %d\n", value);
    }   
}

void error() {
    printf(":< 404 Not Found");
}

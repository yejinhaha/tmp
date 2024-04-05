/*parser.c 단순 산술식에 대한 구문 분석기 시스템*/
#include <stdio.h>
#include <ctype.h>

extern int nextToken;
extern FILE *in_fp;

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
void expr();
void term();
void factor();
void lex();
void error();
void getChar();

/*함수 expr
    다음 규칙으로 생성되는 언어의 문자열을 파싱한다
    <expr> -> <term> {(+|-) <term> }
    */
void expr(){
    printf("Enter <expr>\n");
    /*첫번째 term을 파싱한다.*/
    term();
    /*다음 번째 토큰이 + 또는 - 이면 다음 번째 토큰을 가져오고, 다음 번째 term을 파싱한다.*/
    while(nextToken == ADD_OP || nextToken == SUB_OP ){
        lex();
        term();
    }
    printf("Exit <expr>\n");
}

/*
함수 term
다음 규칙으로 생성되는 언어의 문자열을 파싱한다:
<term> -> <factor> {( * | / ) <factor> }
*/

void term () {
    printf("Enter <term>\n");
    /*첫번째 factor을 파싱한다.*/
    factor();
    /*다음번째 토큰이 * 또는 / 이면 lex를 호출하여 다음 번째 토큰을 가져오고, 다음 번째 term을 파싱한다 */
    while (nextToken == MULT_OP || nextToken == DIV_OP ){
        lex();
        factor();
    }
    printf("Exit <term>\n");
}

/*
factor함수 
다음 규칙으로 생성되는 언어의 문자열을 파싱한다.
<factor> -> id | int_constatn | (<expr>)
*/

void factor(){
    printf("Enter <factor>\n");
    /*어느 RHS를 파싱할 것인지를 결정한다.*/
    if (nextToken == IDENT || nextToken == INT_LIT )
        lex(); //다음 토큰을 가져온다.
    
    /*RHS가 (<expr>)이면 lex 를 호출하여 왼쪽 괄호를 전달하고, expr을 호출하고 오른쪽 괄호를 검사한다.*/
    else if (nextToken == LEFT_PAREN) {
        lex();
        expr();
        if (nextToken == RIGHT_PAREN)
            lex();
        else
            error();
    }// if (nextToken == ...의 끝)
    // id, 정수리터럴, 또는 왼쪽괄호가 아니다
    else
        error();
        
    printf("Exit <factor>\n");
}

int main(){
    if ((in_fp = fopen("front.in", "r")) == NULL)
        printf("ERROR-cannot open front.in \n");
    else{
        getChar();
        do{
            lex();
            expr();
        }while (nextToken != EOF);
    }
}

void error(){
    printf(":< 404 Not Found");
}
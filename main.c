#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum tokenizer {start = 0, az = 1, singleop = 2, lieq = 3, lieq2 = 4, ri = 5, rieq = 6, literal = 7, trap = 8};

enum tokenizer state = start;

int isAcceptingState(enum tokenizer state){
    switch (state) {
        case start: return 0;
        case lieq: return 0;
        case ri: return 0;
        default: return 1;
    }
}

int findMaxToken(enum tokenizer state, char* x){
    char* currunt = x;
    char* next = x + 1;
    enum tokenizer nextState = trap;
    
    switch (state) {
        case start: 
            if (isalpha(*currunt)) nextState = az;
            if (*currunt == '(' | *currunt == ')' | *currunt == '|' | *currunt == '&' | *currunt == '!') nextState = singleop;
            if (*currunt == '<') nextState = lieq;
            if (*currunt == '=') nextState = ri;
            break;

        case az:
            if (isalpha(*currunt)) nextState = az;
            break;

        case lieq:
            if (*currunt == '=') nextState = lieq2;
            break;

        case lieq2:
            if (*currunt == '>') nextState = rieq;
            break;

        case ri:
            if (*currunt == '>') nextState = rieq;
            break;

        default: 
            break;
    }


    printf("%d\n", (int)nextState);
    if (nextState == trap) return 0;

    int maxToken = findMaxToken(nextState, next);

    if (maxToken == 0 && !(isAcceptingState(nextState))) {return 0;}
    return maxToken + 1;

}

char* str_cutoff(char* str, char* strcut, int cutindex){
    int i = 0;
    strcut = (char *)malloc(strlen(str) + 1);
    while (*(str + i + cutindex) != '\0'){
        *(strcut + i) = *(str + i + cutindex);
        i++;
    }
    return strcut;
}





char* nextToken(char* x){
    int toklen = findMaxToken(start, x);
    //str_cut(x, 0, toklen - 1);
    char* y = NULL;
    return str_cutoff(x, y, toklen);
}

int main(void){
    char* x = "hallo";
    char* y;
    printf("%d\n", findMaxToken(start, x));
    //nextToken(x);
    //puts(x);
    str_cutoff(x, y, 1);
    printf("%c\n", *y);
    printf("%s\n", y);
    
    return EXIT_SUCCESS;

}
    
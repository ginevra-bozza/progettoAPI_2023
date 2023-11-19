#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ADDSTAT "aggiungi-stazione"
#define REMOVESTAT "demolisci-stazione"
#define ADDCAR "aggiungi-auto"
#define REMOVECAR "rottama-auto"
#define LEN 19 //max length of input + 1

typedef struct parkBST_s{
    struct parkBST_s *left, *right;
    int fuel;
} parkBST_t;

typedef struct statBST_s{
    struct statBST_s *left, *right;
    int distance;
    int carCount;
} statBST_t;


int stringCmp(const char* , const char* );


int main(){
    statBST_t *start; //root of the BST containing all stations
    char *input;

    start = malloc(sizeof (statBST_t));
    if(start == NULL){
        printf("Memory allocation error - statBST root");
        return 1;
    }
    input = malloc(sizeof(char)*LEN);
    if(input == NULL){
        printf("Memory allocation error");
        return 1;
    }
    if(fgets(input, LEN, stdin) == NULL){
        printf("\nerror in fgets - input");
        return 1;
    }
    printf("INPUT= %s",input);

    // while(!feof(stdin)){
    if(strcmp(input, ADDSTAT) == 0){
        printf("ADDSTAT");

    } else if(strcmp(input, REMOVESTAT) == 0){
        printf("REMOVESTAT");

    } else if(strcmp(input, ADDCAR) == 0){
        printf("ADDCAR");
    } else if(strcmp(input, REMOVECAR) == 0){
        printf("REMOVECAR");
    } else {
        printf("aaaaaaa");
    }

    /*if(fgets(input, LEN, stdin) == NULL){
        printf("\nerror in fgets - input");
        return 1;
    }*/
    //}


}

int stringCmp(const char* str1, const char* str2){
    int res;

    while(*str1 && (*str2 == *str1)){
        str1++;
        str2++;
    }
    res = *(const char*)str1-*(const char*)str2;
    return res;
}
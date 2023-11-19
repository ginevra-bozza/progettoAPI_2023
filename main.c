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
    parkBST_t *park;
} statBST_t;


int stringCmp(const char* , const char* );


int main(){
    statBST_t *sRoot = NULL; //root of the BST containing all stations
    char input[LEN];
    int numOfCars;

    //creating root of station BST
    sRoot = malloc(sizeof (statBST_t));
    if(sRoot == NULL){
        printf("Memory allocation error - statBST root");
        return 1;
    }
    sRoot->distance = 0;
    sRoot-> carCount = 0;
    sRoot->left = NULL;
    sRoot->right = NULL;
    sRoot->park = NULL;

    //getting input from stdin
    if(fgets(input, LEN, stdin) == NULL){
        printf("\nerror in fgets - input");
        return 1;
    }
    strtok(input, "\n"); //input string has an extra \n at the end


    // while(!feof(stdin)){
    if(strcmp(input, ADDSTAT) == 0){
        printf("ADDSTAT");

        //addNewStation(sRoot)

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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ADDSTAT "aggiungi-stazione"
#define REMOVESTAT "demolisci-stazione"
#define ADDCAR "aggiungi-auto"
#define REMOVECAR "rottama-auto"
#define BESTROUTE "pianifica-percorso"
#define LEN 19 //max length of input + 1

typedef struct parkingBST_s{
    struct parkingBST_s *left, *right;
    int fuel;
} parkingBST_t;

typedef struct statBST_s{
    struct statBST_s *left, *right;
    int distance;
    int carCount;
    parkingBST_t *parking;
} statBST_t;


int stringCmp(const char* , const char* );

statBST_t *addStation(statBST_t **, int);
int addCar(int, int);
int removeStation(statBST_t *, int);
int removeCar(int, int);
void planRoute(int, int);

int main(){
    statBST_t *sRoot = NULL; //root of the BST containing all stations
    char input[LEN];
    int numOfCars = 0;
    int carAutonomy = 0;
    int distance = 0;

    statBST_t *station = NULL;
    statBST_t  *addedStation = NULL;
    parkingBST_t * carParking = NULL;

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
    sRoot->parking = NULL;

    printf("ROOT station : distance->%d, carCount->%d, left->%d, right->%d, parking->%d ",sRoot->distance, sRoot->carCount, sRoot->left, sRoot->right,sRoot->parking);

    /*
    //getting input from stdin
    if(fgets(input, LEN, stdin) == NULL){
        printf("\nerror in fgets - input");
        return 1;
    }
    strtok(input, "\n"); //input string has an extra \n at the end
     */
    scanf("%s",input);

    // while(!feof(stdin)){
    if(strcmp(input, ADDSTAT) == 0){
        printf("ADDSTAT\n");

        scanf("%d",&distance);
        addedStation = addStation(&sRoot, distance); //addStation return a pointer to the newly added station if added, else return NULL

        if (addedStation != NULL){
            scanf("%d",&numOfCars);
            printf("Num of cars: %d",numOfCars);
            addedStation -> carCount = numOfCars;

            carParking = malloc(sizeof (parkingBST_t));
            if(carParking == NULL){
                printf("Memory allocation error - new parking");
                return 1;
            }
            for(int i = 0; i <= numOfCars; i++){
                scanf("%d",&carAutonomy);
                addCar(addedStation->distance, carAutonomy);
            }
            printf("aggiunta");
        } else {
            printf("non aggiunta");
            free(station);
        }

    } else if(strcmp(input, REMOVESTAT) == 0){
        printf("REMOVESTAT");

        scanf("%d",&distance);
        if(distance > 0){
            if(removeStation(sRoot,distance)){
                printf("demolita");
            } else {
                printf("non demolita");
            }
        } else {
            printf("Error in removal - invalid distance");
        }

    } else if(strcmp(input, ADDCAR) == 0){
        printf("ADDCAR");
        scanf("%d", &distance);
        scanf("%d",&carAutonomy);
        if(addCar(distance, carAutonomy)){
            printf("aggiunta");
        } else {
            printf("non aggiunta");
        }

    } else if(strcmp(input, REMOVECAR) == 0){
        printf("REMOVECAR");
        scanf("%d", &distance);
        scanf("%d",&carAutonomy);
        if(removeCar(distance, carAutonomy)){
            printf("demolita");
        } else {
            printf("non demolita");
        }

    } else if(strcmp(input,BESTROUTE) == 0){
        printf("BESTROUTE");
        int start;
        int end;
        scanf("%d",&start);
        scanf("%d",&end);
        planRoute(start,end);

    } else {
        printf("aaaaaaa");
    }

    /*if(fgets(input, LEN, stdin) == NULL){
        printf("\nerror in fgets - input");
        return 1;
    }*/
    //}


}

statBST_t * addStation(statBST_t ** root, int dist) {
    statBST_t* current = *root;
    statBST_t* parent = NULL;
    statBST_t *station = NULL;

    // Traverse the tree to find the appropriate position for the new node
    while (current != NULL) {
        if (dist == current->distance) {
            // If the dist already exists in the tree, return NULL
            return NULL;
        } else if (dist < current->distance) {
            parent = current;
            current = current->left;
        } else {
            parent = current;
            current = current->right;
        }
    }

    station = malloc(sizeof (statBST_t));
    if(station == NULL){
        printf("Memory allocation error - new station");
        return NULL;
    }
    station->distance = dist;
    station->right = NULL;
    station->left = NULL;
    station->parking = NULL;

    if(dist < parent->distance)
        parent->left = station;
    else
        parent->right = station;
    return station;

}




int removeStation(statBST_t *pS, int distance) {
    return 0;
}
int addCar(int distance, int autonomy) {

}
int removeCar(int distance, int autonomy) {
    return 0;
}
void planRoute(int start, int anEnd) {

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
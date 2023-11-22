#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ADDSTAT "aggiungi-stazione"
#define REMOVESTAT "demolisci-stazione"
#define ADDCAR "aggiungi-auto"
#define REMOVECAR "rottama-auto"
#define BESTROUTE "pianifica-percorso"
#define LEN 19 //max length of input + 1

typedef struct carBST_s{
    struct carBST_s *left, *right;
    int autonomy;
    int carCounter;
} carBST_t;

typedef struct statBST_s{
    struct statBST_s *left, *right;
    int distance;
    int carCount;
    carBST_t *parking;
} statBST_t;


int stringCmp(const char* , const char* );

statBST_t *addStation(statBST_t **, int);
int addCar(carBST_t **, int);
int removeStation(statBST_t *, int);
int removeCar(int, int);
void planRoute(int, int);

statBST_t *checkStation(int distance);

int main(){
    statBST_t *sRoot = NULL; //root of the BST containing all stations
    char input[LEN];
    int numOfCars = 0;
    int carAutonomy = 0;
    int distance = 0;

    statBST_t *station = NULL;
    statBST_t  *addedStation = NULL;
    carBST_t * carParking = NULL;

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

    while(!feof(stdin)) {
        scanf("%s",input);
        if (strcmp(input, ADDSTAT) == 0) {
            printf("ADDSTAT\n");

            scanf("%d", &distance);
            addedStation = addStation(&sRoot,
                                      distance); //addStation return a pointer to the newly added station if added, else return NULL

            if (addedStation != NULL) {
                scanf("%d", &numOfCars);
                printf("Num of cars: %d", numOfCars);
                addedStation->carCount = numOfCars;

                carParking = malloc(sizeof(carBST_t));
                if (carParking == NULL) {
                    printf("Memory allocation error - new parking");
                    return 1;
                }
                for (int i = 1; i <= numOfCars; i++) {
                    scanf("%d", &carAutonomy);
                    addCar(&addedStation->parking, carAutonomy);
                }
                printf("aggiunta");
            } else {
                printf("non aggiunta");
                free(station);
            }

        } else if (strcmp(input, REMOVESTAT) == 0) {
            printf("REMOVESTAT");

            scanf("%d", &distance);
            if (distance > 0) {
                if (removeStation(sRoot, distance)) {
                    printf("demolita");
                } else {
                    printf("non demolita");
                }
            } else {
                printf("Error in removal - invalid distance");
            }

        } else if (strcmp(input, ADDCAR) == 0) {
            printf("ADDCAR");
            scanf("%d", &distance);
            scanf("%d", &carAutonomy);

            station = checkStation(distance);
            if(station != NULL){
                addCar((carBST_t **) &station->parking, carAutonomy);

            } else {
                printf("non aggiunta");
            }



            if (addCar(NULL, carAutonomy)) {
                printf("aggiunta");
            } else {
                printf("non aggiunta");
            }

        } else if (strcmp(input, REMOVECAR) == 0) {
            printf("REMOVECAR");
            scanf("%d", &distance);
            scanf("%d", &carAutonomy);
            if (removeCar(distance, carAutonomy)) {
                printf("demolita");
            } else {
                printf("non demolita");
            }

        } else if (strcmp(input, BESTROUTE) == 0) {
            printf("BESTROUTE");
            int start;
            int end;
            scanf("%d", &start);
            scanf("%d", &end);
            planRoute(start, end);

        } else {
            printf("aaaaaaa");
        }
    }

    /*if(fgets(input, LEN, stdin) == NULL){
        printf("\nerror in fgets - input");
        return 1;
    }*/
    //}


}

statBST_t *checkStation(int distance) {
    return NULL;
}

statBST_t * addStation(statBST_t ** root, int dist){
    statBST_t *current = *root;
    statBST_t *parent = NULL;
    statBST_t *station = NULL;

    while (current != NULL) {
        if (dist == current->distance) {
            return NULL;// if the dist is already present in the tree, return NULL

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

    //insert the station in the tree
    if(dist < parent->distance)
        parent->left = station;
    else
        parent->right = station;
    return station;

}




int removeStation(statBST_t *pS, int distance) {
    return 0;
}
int addCar(carBST_t ** root, int autonomy) {

    carBST_t *current = *root;
    carBST_t *parent = NULL;
    carBST_t *car = NULL;

    if(current == NULL){
        car = malloc(sizeof (carBST_t ));
        if(car == NULL){
            printf("Memory allocation error - new car");
            return 0;
        }
        car->carCounter = 1;
        car->autonomy = autonomy;
        car->right = NULL;
        car->left = NULL;

        *root = car;
        return 1;
    }

    while (current != NULL) {
        if (autonomy == current->autonomy) {
            current->carCounter++;
            return 1;
        } else if (autonomy < current->autonomy) {
            parent = current;
            current = current->left;
        } else {
            parent = current;
            current = current->right;
        }
    }

    car = malloc(sizeof (carBST_t ));
    if(car == NULL){
        printf("Memory allocation error - new car");
        return 0;
    }
    car->carCounter = 1;
    car->autonomy = autonomy;
    car->right = NULL;
    car->left = NULL;

    //insert the car in the tree

    if(autonomy < parent->autonomy)
        parent->left = car;
    else
        parent->right = car;

    return 1;
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
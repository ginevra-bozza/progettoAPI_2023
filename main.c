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
int removeStation(statBST_t **, int);
int removeCar(carBST_t **, int);
void planRoute(int, int);
statBST_t *checkStation(statBST_t *, int distance);
statBST_t * findStation(statBST_t **, int dist, statBST_t **);

statBST_t *min(struct statBST_s *pS);

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

    while(!feof(stdin)) {
        scanf("%s",input);
        if (strcmp(input, ADDSTAT) == 0) {
            printf("ADDSTAT\n");

            scanf("%d", &distance);
            addedStation = addStation(&sRoot,distance); //addStation return a pointer to the newly added station if added, else return NULL

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
                if (removeStation(&sRoot, distance)) {
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

            station = checkStation(sRoot, distance);
            if(station != NULL){
                addCar((carBST_t **) &station->parking, carAutonomy);
                printf("aggiunta");
            } else {
                printf("non aggiunta");
            }

        } else if (strcmp(input, REMOVECAR) == 0) {
            printf("REMOVECAR");
            scanf("%d", &distance);
            scanf("%d", &carAutonomy);

            station = checkStation(sRoot,distance);
            if(station != NULL){
                if (removeCar(&station->parking, carAutonomy)) {
                    printf("demolita");
                } else {
                    printf("non demolita");
                }
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

statBST_t *checkStation(statBST_t * root, int dist) {
    statBST_t * current = root;

    while (current != NULL) {
        if (dist == current->distance) {
            return current;
        } else if (dist < current->distance) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return NULL;
}

/*statBST_t * findStation(statBST_t ** root, int dist, statBST_t ** parent) {
    statBST_t * current = *root;

    while (current != NULL) {
        *parent = current; //saves the parent node of the station
        if (dist == current->distance) {
            return current; //return the station when found
        } else if (dist < current->distance) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return NULL; //return NULL if the station is not found
}*/


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


int removeStation (statBST_t ** root, int dist)
{
    statBST_t* current = *root;
    statBST_t* parent = NULL;


    while (current != NULL && current->distance != dist) {
        parent = current;
        if (dist < current->distance)
            current = current->left;
        else
            current = current->right;
    }
    if (current == NULL) {
        return 0; //the station is not present and cannot be deleted
    }

    //the station has 0 or only 1 child
    if (current->left == NULL || current->right == NULL) {

        statBST_t * tempSt;

        if (current->left == NULL)
            tempSt = current->right;
        else
            tempSt = current->left;

        if (current == parent->left)
            parent->left = tempSt;
        else
            parent->right = tempSt;

        free(current);

    } else { //the station node has 2 children
        statBST_t * temp1 = NULL;
        statBST_t * temp2 = NULL;

        temp2 = current->right;
        while (temp2->left != NULL) {
            temp1 = temp2;
            temp2 = temp2->left;
        }

        if (temp1 != NULL)
            temp1->left = temp2->right;
        else
            current->right = temp2->right;

        current->distance = temp2->distance;
        free(temp2);
    }
    return 1; //station deleted successfully
}


/*int deleteStation(statBST_t ** root, int distance) {

    statBST_t * current = *root;
    statBST_t * parent = NULL;
    statBST_t * temp = NULL;


    // Find the station to be removed
    while (current->distance != distance) {
        parent = current;
        if (distance < current->distance) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (current == NULL) {
        return 0; // station not found
    }

    if (current->left == NULL || current->right == NULL) {
        temp = current->left;
        if (temp == NULL) {
            temp = current->right;
        }
        if(parent->left == current){
            parent->left = temp;
        } else {
            parent->right = temp;
        }
        free(current);
        current = temp;
    }

    if (parent->left == current) {
        parent->left = NULL;
    } else {
        parent->right = NULL;
    }
    free(current);

    return 1; // Removal successful
}*/





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
int removeCar(carBST_t ** root, int aut) {
    carBST_t * current = *root;
    carBST_t * parent = NULL;


    while (current != NULL && current->autonomy != aut) {
        parent = current;
        if (aut < current->autonomy)
            current = current->left;
        else
            current = current->right;
    }
    if (current == NULL) {
        return 0; //the car is not present and cannot be deleted
    }

    //the car has 0 or only 1 child
    if (current->left == NULL || current->right == NULL) {

        carBST_t * tempC;

        if (current->left == NULL)
            tempC = current->right;
        else
            tempC = current->left;

        if (current == parent->left)
            parent->left = tempC;
        else
            parent->right = tempC;

        free(current);

    } else { //the station node has 2 children
        carBST_t * temp1 = NULL;
        carBST_t * temp2 = NULL;

        temp2 = current->right;
        while (temp2->left != NULL) {
            temp1 = temp2;
            temp2 = temp2->left;
        }

        if (temp1 != NULL)
            temp1->left = temp2->right;
        else
            current->right = temp2->right;

        current->autonomy = temp2->autonomy;
        free(temp2);
    }
    return 1; //car deleted successfully

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
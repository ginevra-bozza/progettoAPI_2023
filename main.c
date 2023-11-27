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
    int maxFuel;
    carBST_t *parking;
} statBST_t;

typedef struct stopList_s {
    struct stopList_s * next;
    int distance;
    int maxFuel;
} stopList_t;


int stringCmp(const char* , const char* );

statBST_t *addStation(statBST_t **, int);
int addCar(carBST_t **, int);
int removeStation(statBST_t **, int);
int removeCar(carBST_t **, int);
void findBestPath(int start, int end);
statBST_t *checkStation(statBST_t *, int distance);
statBST_t * findStation(statBST_t **, int dist, statBST_t **);

statBST_t *min(struct statBST_s *pS);

void fromBSTtoList(statBST_t*,int ,int,stopList_t **);

int findNewMaxFuel(carBST_t *cRoot);

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
                addedStation->maxFuel = 0;
                carParking = malloc(sizeof(carBST_t));
                if (carParking == NULL) {
                    printf("Memory allocation error - new parking");
                    return 1;
                }
                for (int i = 1; i <= numOfCars; i++) {
                    scanf("%d", &carAutonomy);
                    addCar(&addedStation->parking, carAutonomy);
                    if(carAutonomy > addedStation->maxFuel)
                        addedStation->maxFuel = carAutonomy;
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
                if(carAutonomy > station->maxFuel)
                    station->maxFuel = carAutonomy;
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
                    if(station->maxFuel == carAutonomy){
                        station->maxFuel = findNewMaxFuel(station->parking);
                    }
                    printf("demolita");
                } else {
                    printf("non demolita");
                }
            } else {
                printf("non demolita");
            }

        } else if (strcmp(input, BESTROUTE) == 0) {
            printf("BESTROUTE");
            int start,end;
            statBST_t * startStat = NULL;
            statBST_t *endStat = NULL;
            stopList_t * listHead = NULL;
            scanf("%d", &start);
            scanf("%d", &end);

            startStat = checkStation(sRoot, start);
            if(startStat == NULL){
                printf(("nessun percorso"));
            }
            endStat = checkStation(sRoot, end);
            if(endStat == NULL){
                printf(("nessun percorso"));
            }
            fromBSTtoList(sRoot, start,end,&listHead);

            stopList_t * temp = listHead;
            printf("\n");

            while(temp != NULL){
                printf("%d ",temp->distance);
                temp = temp->next;
            }

            findBestPath(start, end);

        } else {
            printf("aaaaaaa");
        }
    }

}

int findNewMaxFuel(carBST_t * car) {
    while (car->right != NULL) {
        car = car->right;
    }
    return car->autonomy;
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
    //if there are more cars with the same autonomy
    if(current->carCounter>1){
        current->carCounter--;
        return 1;
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

void fromBSTtoList(statBST_t* root, int start, int end, stopList_t ** sListHead){

    if(root == NULL)
        return;

    fromBSTtoList(root->left,start,end,sListHead);
    if(root->distance >= start && root->distance <= end){
        stopList_t * newStop = (stopList_t *)(malloc(sizeof (stopList_t)));
        newStop->distance = root->distance;
        newStop->maxFuel = root->maxFuel;
        if(*sListHead == NULL){
            *sListHead = newStop;
        } else {
            stopList_t * tempStop = *sListHead;
            while (tempStop->next != NULL){
                tempStop = tempStop->next;
            }
            tempStop->next = newStop;
        }
    }
    fromBSTtoList(root->right,start,end,sListHead);
}




void findBestPath(int start, int end) {





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
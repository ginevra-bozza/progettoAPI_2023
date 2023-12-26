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
    struct statBST_s * parent;
    int distance;
    int carCount;
    int maxFuel;
    carBST_t *parking;
} statBST_t;

typedef struct stationList_s {
    struct stationList_s * next;
    int distance;
    int maxFuel;
} stationList_t;

typedef struct pathList_s{
    struct pathList_s * next;
    int statDist;
} pathList_t;

int stringCmp(const char* , const char* );

statBST_t *addStation(statBST_t **, int);
int addCar(carBST_t **, int);
int removeStation(statBST_t **, int);
int removeCar(carBST_t **, int);
pathList_t *findBestPath(statBST_t *,statBST_t *, statBST_t* );
statBST_t *checkStation(statBST_t *, int distance);
statBST_t* min(statBST_t*);
statBST_t* successor(statBST_t*, statBST_t*);



void fromBSTtoList(statBST_t *, int, int, stationList_t **, int *stationCount);

int findNewMaxFuel(carBST_t *);

pathList_t *createNewPath(int startDistance);
pathList_t *add(pathList_t *pS, statBST_t *pS1);

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
            stationList_t * listHead = NULL;
            int stationCounter = 0;
            scanf("%d", &start);
            scanf("%d", &end);

            startStat = checkStation(sRoot, start);
            endStat = checkStation(sRoot, end);

            if(startStat == NULL || endStat == NULL){
                printf(("nessun percorso"));
            }
            else{
                pathList_t * bestPath = findBestPath(sRoot,startStat, endStat);
                if(bestPath == NULL){
                    printf("nessun percorso");
                } else {
                    for(pathList_t * s = bestPath; s != NULL; s = s->next){
                        printf(" %d ", s->statDist);
                    }
                }
            }

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
    station->parent = parent;

    //insert the station in the tree
    if(dist < parent->distance)
        parent->left = station;
    else
        parent->right = station;
    return station;

}

//DA AGGIUNGERE: modifica del nodo parent
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

pathList_t * findBestPath1(statBST_t * root,statBST_t * start, statBST_t * end) {
    statBST_t *current = start;
    statBST_t *predecessor = NULL;
    pathList_t * bestPath = NULL;
    statBST_t * nextBestStation = NULL;
    statBST_t * temp = NULL;
    int shortestDistToEnd = end->distance - start->distance;
    int found = 0;

    if(start->maxFuel == 0)
        return NULL; //can't reach next station;

    bestPath = createNewPath(start->distance);

    if(start->maxFuel >= shortestDistToEnd){
        bestPath = add(bestPath, end);
        return bestPath; //can reach end station directly
    }


    //find the first next station
    temp = successor(root, start);
    nextBestStation = temp;

    while(current != end && found == 0){
        shortestDistToEnd = end->distance - current->distance;
        while(temp->distance <= current->distance + current->maxFuel){

            //if from the temp station the next station is not reachable, the path doesn't exist
            if (temp->left == NULL) {
                if(temp->maxFuel <= 0 || temp->distance + temp->maxFuel < temp->right->distance){
                    return NULL;
                }

                //see if the station is the new best
                int distToEvaluate = end->distance - (temp->distance + temp->maxFuel);
                if( distToEvaluate < shortestDistToEnd){
                    nextBestStation = temp;
                    shortestDistToEnd = distToEvaluate;
                } else if(distToEvaluate == shortestDistToEnd){
                    if(temp->distance < nextBestStation->distance){
                        nextBestStation = temp;
                    }
                }
                //if the successive station is reachable from the current one, proceed
                if(current->maxFuel >= (temp->right->distance - current->distance)){
                    temp = temp->right;
                } else
                    return NULL;

            } else {
                // find the predecessor in the left subtree
                predecessor = temp->left;
                while (predecessor->right != NULL && predecessor->right != temp) {
                    predecessor = predecessor->right;
                }

                if (predecessor->right == NULL) {
                    // if the predecessor's right is null, establish a temporary link from the predecessor to the current node
                    predecessor->right = temp;
                    temp = temp->left;
                } else {
                    // if the predecessor's right is the current node, revert the temporary link and print the current node
                    predecessor->right = NULL;

                    //see if the station is the new best
                    int distToEvaluate = end->distance - (temp->distance + temp->maxFuel);
                    if( distToEvaluate < shortestDistToEnd){
                        nextBestStation = temp;
                        shortestDistToEnd = distToEvaluate;
                    } else if(distToEvaluate == shortestDistToEnd){
                        if(temp->distance < nextBestStation->distance){
                            nextBestStation = temp;
                        }
                    }
                    //if the next station is reachable from the current one, proceed
                    if(current->maxFuel >= (temp->right->distance - current->distance)){
                        temp = temp->right;
                    } else
                        return NULL;

                    temp = temp->right;
                }
            }
        }
        //bestPath is a pointer to the head of the path list
        bestPath = add(bestPath, nextBestStation);
        if(end->distance - nextBestStation->distance <= nextBestStation->maxFuel){
            bestPath = add(bestPath,end);
            found = 1;
        } else {
            current = nextBestStation;
        }

    }
    return bestPath;

}


pathList_t * findBestPath(statBST_t * root,statBST_t * start, statBST_t * end){

    statBST_t *current = start;
    pathList_t * bestPath = NULL;
    statBST_t * nextBestStation = NULL;
    statBST_t * temp = NULL;
    int maxReachableDist = 0;
    int newReachableDist = 0;

    if(start->maxFuel == 0)
        return NULL; //can't reach next station;

    bestPath = createNewPath(start->distance);

    if(start->maxFuel + start->distance >= end->distance){
        bestPath = add(bestPath, end);
        return bestPath; //can reach end station directly
    }

    current = start;
    while(current != end){
        temp = successor(root, current);
        maxReachableDist = current->distance + current->maxFuel;
        if(maxReachableDist >= end->distance){
            bestPath = add(bestPath, end);
            return bestPath;
        }
        newReachableDist = 0;
        while(temp->distance <= maxReachableDist && temp != end && temp != NULL){
            if(temp->distance + temp->maxFuel > newReachableDist){
                newReachableDist = temp->distance + temp->maxFuel;
                nextBestStation = temp;
            }
            temp = successor(root, temp);
        }
        if(newReachableDist == 0 || (temp == NULL && nextBestStation != end))
            return NULL;
        bestPath = add(bestPath, nextBestStation);
        current = nextBestStation;

    }



    //find the first next station
    /*temp = successor(root, start);
    nextBestStation = temp;
    while(current != NULL && current != end){
        shortestDistToEnd = end->distance - current->distance;

        while(temp != end){
            tempNext = successor(root, temp);
            if(temp->maxFuel != 0 && temp->maxFuel + temp->distance >= tempNext->distance) {
                if((end->distance - (temp->distance + temp->maxFuel)) < shortestDistToEnd){
                    nextBestStation = temp;
                    temp = tempNext;
                }
            }

        }
        //bestPath is a pointer to the head of the path list
        bestPath = add(bestPath, nextBestStation);
        if(nextBestStation->distance == end->distance){
            return bestPath;
        }
        if(end->distance - nextBestStation->distance <= nextBestStation->maxFuel){
            bestPath = add(bestPath,end);
            return bestPath;
        }
        current = nextBestStation;
    }*/

}

pathList_t *add(pathList_t * headStation, statBST_t * station) {

    pathList_t * newNode = (pathList_t*) malloc(sizeof (pathList_t));
    newNode->next = NULL;
    newNode->statDist = station->distance;

    pathList_t * temp = headStation;
    while(temp->next!=NULL)
        temp = temp->next;
    temp->next = newNode;

    return headStation;
}

pathList_t *createNewPath(int startDistance) {
    pathList_t * path = (pathList_t*) malloc(sizeof (pathList_t));
    path->next = NULL;
    path->statDist = startDistance;
    return path;
}


statBST_t * successor(statBST_t * root, statBST_t * target)
{
    statBST_t *successor = NULL;

    while (root != NULL) {
        if (target->distance < root->distance) {
            // If target is in the left subtree, update the successor and move to the left subtree
            successor = root;
            root = root->left;
        } else {
            // If target is in the right subtree, move to the right subtree
            root = root->right;
        }
    }

    return successor;
}
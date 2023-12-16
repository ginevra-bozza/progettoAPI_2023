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
pathList_t *findBestPath(statBST_t *, statBST_t* );
statBST_t *checkStation(statBST_t *, int distance);
statBST_t * findStation(statBST_t **, int dist, statBST_t **);

void printBSTInOrder(statBST_t *);
void printInorder(statBST_t *);

statBST_t *min(struct statBST_s *pS);

void fromBSTtoList(statBST_t *, int, int, stationList_t **, int *stationCount);

int findNewMaxFuel(carBST_t *);

pathList_t *insertStation(pathList_t *h, stationList_t *station);

pathList_t *createNewPath(statBST_t *startStation);

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
                //printBSTInOrder(sRoot);
                //printInorder(sRoot);

                /*fromBSTtoList(sRoot, start, end, &listHead, &stationCounter);
                stationList_t * temp = listHead;
                while(temp != NULL){
                    printf(" %d ", temp->distance);
                    temp = temp->next;
                }*/
                pathList_t * bestPath = findBestPath(startStat, endStat);
                for(pathList_t * s = bestPath; s != NULL; s = s->next){
                    printf(" %d ", s->statDist);
                }
                /*
               pathList_t * bestPath = findBestPath(listHead, stationCounter);
               pathList_t * temp = bestPath;
               printf("\n");

               while(temp != NULL){
                   printf("%d ",temp->station->distance);
                   temp = temp->next;
               }
               printf("ciao");*/
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

void fromBSTtoList(statBST_t *root, int start, int end, stationList_t **sListHead, int *stationCount) {

    if(root == NULL)
        return;

    fromBSTtoList(root->left, start, end, sListHead, stationCount);

    if(root->distance >= start && root->distance <= end){
        stationList_t * newStop = (stationList_t *)(malloc(sizeof (stationList_t)));
        newStop->distance = root->distance;
        newStop->maxFuel = root->maxFuel;
        newStop->next = NULL;
        *stationCount = (*stationCount) + 1;
        if(*sListHead == NULL){
            *sListHead = newStop;
        } else {
            stationList_t * tempStop = (*sListHead);
            while (tempStop->next != NULL){
                tempStop = tempStop->next;
            }
            tempStop->next = newStop;
        }
    }
    fromBSTtoList(root->right, start, end, sListHead, stationCount);
}




/*pathList_t *findBestPath(stationList_t *firstStation, int stationCount) {

    pathList_t* temp = NULL;
    pathList_t* bestPath = NULL;
    pathList_t* curr = NULL;

    int minStopsCount = stationCount;

    temp = (pathList_t*) malloc(sizeof (pathList_t));
    if(temp == NULL){
        printf("Memory allocation error - path");
        return NULL;
    }
    temp->next = NULL;
    temp->station = firstStation;

    while(temp != NULL){
        curr  = temp;
        temp = temp->next;

        if(curr->station->distance == 0 && minStopsCount > 0) {
            minStopsCount = 0;
            bestPath = curr;
        }

        for(stationList_t *nextStat = firstStation; nextStat != NULL; nextStat = nextStat->next) {
            int reachableDist = curr->station->distance + curr->station->maxFuel;
            if(nextStat->distance <= reachableDist){
                int stopsCount = 1;
                int check = 0;
                for(pathList_t * s = curr; s != NULL && check == 0; s = s->next){
                    stopsCount++;
                    if(s->station->distance + s->station->maxFuel >= nextStat->distance){
                        check = 1;
                    }
                }
                bestPath = insertStation(bestPath, nextStat);

                temp = insertStation(temp, nextStat);
            }
        }
    }
    return bestPath;
}

pathList_t *insertStation(pathList_t *h, stationList_t *station) {
    pathList_t * p = NULL;
    p = (pathList_t*)malloc(sizeof (pathList_t));
    if(p == NULL){
        printf("Mem error - path station");
        return NULL;
    }
    p->station = station;
    p->next = NULL;

    if (p != NULL) {
        if (h == NULL) {
            return p;
        } else {
            pathList_t *temp = h;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = p;
        }
    }
    return h;
}*/


int stringCmp(const char* str1, const char* str2){
    int res;

    while(*str1 && (*str2 == *str1)){
        str1++;
        str2++;
    }
    res = *(const char*)str1-*(const char*)str2;
    return res;
}


void printBSTInOrder(statBST_t * root){
    statBST_t * curr, *prev;

    curr = root;
    while(curr != NULL){
        if(curr->left == NULL){
            printf("stampa inizio %d ", curr->distance);
            curr = curr->right;
        } else {
            prev = curr->left;
            while(prev->right != NULL && prev->right != curr){
                prev = prev->right;
            }
            if(prev->right == NULL){
                prev->right = curr;
                curr = curr->right;
            } else {
                prev->right = NULL;
                printf("stampa fine %d ", curr->distance);
                curr = curr->left;
            }
        }
    }
}

/*void printInorder(statBST_t * node) //recursive
{
    if (node == NULL)
        return;

    // First recur on left child
    printInorder(node->left);

    // Then print the data of node
    printf("%d ", node->distance);

    // Now recur on right child
    printInorder(node->right);
}*/

void printInorder(statBST_t *root) {
    statBST_t *current = root;
    statBST_t *predecessor = NULL;

    while (current != NULL) {
        if (current->left == NULL) {
            // If the left subtree is null, print the current node and move to the right subtree
            printf("%d ", current->distance);
            current = current->right;
        } else {
            // Find the predecessor in the left subtree
            predecessor = current->left;
            while (predecessor->right != NULL && predecessor->right != current) {
                predecessor = predecessor->right;
            }

            if (predecessor->right == NULL) {
                // If the predecessor's right is null, establish a temporary link from the predecessor to the current node
                predecessor->right = current;
                current = current->left;
            } else {
                // If the predecessor's right is the current node, revert the temporary link and print the current node
                predecessor->right = NULL;
            printf("%d ", current->distance);
            current = current->right;
            }
        }
    }
}

pathList_t * findBestPath(statBST_t * start, statBST_t * end) {
    statBST_t *current = start;
    statBST_t *predecessor = NULL;
    pathList_t * bestPath = NULL;
    statBST_t * nextBestStation = NULL;
    statBST_t * temp = NULL;
    int shortestDistToEnd = end->distance - start->distance;
    int found = 0;

    if(start->maxFuel = 0)
        return NULL; //can't reach next station;

    bestPath = createNewPath(start);

    if(start->maxFuel >= shortestDistToEnd){
        bestPath = add(bestPath, end);
        return bestPath; //can reach end station directly
    }

    current = start;
    temp = start;

    while(current != end && found == 0){
        shortestDistToEnd = end->distance - current->distance;
        while(temp->distance <= current->distance + current->maxFuel){

            if (temp->left == NULL) {
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
                if(temp->maxFuel >= (temp->right->distance - temp->distance)){
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
                    if(temp->maxFuel >= (temp->right->distance - temp->distance)){
                        temp = temp->right;
                    } else
                        return NULL;

                    temp = temp->right;
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

    }

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

pathList_t *createNewPath(statBST_t *startStation) {
    pathList_t * path = (pathList_t*) malloc(sizeof (pathList_t));
    path->next = NULL;
    path->statDist = startStation->distance;
    return path;
}

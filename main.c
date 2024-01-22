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
    //struct statBST_s * parent;
    int distance;
    int maxFuel;
    carBST_t *parking;
} statBST_t;


typedef struct pathList_s{
    struct pathList_s * next;
    struct pathList_s * prev;
    statBST_t * station;
    int statDist;
} pathList_t;

statBST_t *addStation(statBST_t **, int);
int addCar(carBST_t **, int);
int removeStation(statBST_t **, int);
int removeCar(carBST_t **, int);
pathList_t *findBestPath(statBST_t *,statBST_t *, statBST_t* );
pathList_t *findBestPathReversed(statBST_t *,statBST_t *, statBST_t* );


statBST_t *checkStation(statBST_t *, int distance);
statBST_t* successor(statBST_t*, statBST_t*);

void printTree(statBST_t *root); //debug!!!!

int findNewMaxFuel(carBST_t *);

pathList_t *createNewPath(statBST_t* );
pathList_t *addToPath(pathList_t *headStation, statBST_t *station);

statBST_t *predecessor(statBST_t *, statBST_t *);

void printReverse(pathList_t *);

int main(){
    statBST_t *sRoot = NULL; //root of the BST containing all stations
    char input[LEN];
    int numOfCars = 0;
    int carAutonomy = 0;
    int distance = 0;

    statBST_t *station = NULL;
    statBST_t  *addedStation = NULL;

    //creating root of station BST
    sRoot = malloc(sizeof (statBST_t));
    if(sRoot == NULL){
        printf("Memory allocation error - statBST root");
        return 1;
    }
    sRoot->distance = 0;
    sRoot->left = NULL;
    sRoot->right = NULL;
    sRoot->parking = NULL;


    while(!feof(stdin)) {
        scanf("%s",input);
        if (strcmp(input, ADDSTAT) == 0) {
            scanf("%d", &distance);
            addedStation = addStation(&sRoot,distance); //addStation return a pointer to the newly added station if added, else return NULL

            if (addedStation != NULL) {
                scanf("%d", &numOfCars);
                addedStation->maxFuel = 0;

                //root node of the car parking bst
                addedStation->parking = (carBST_t* )malloc(sizeof (carBST_t));
                addedStation->parking->autonomy = 0;
                addedStation->parking->right = NULL;
                addedStation->parking->left = NULL;
                addedStation->parking->carCounter = 0;

                if(numOfCars > 0){
                    for (int i = 1; i <= numOfCars; i++) {
                        scanf("%d", &carAutonomy);
                        addCar(&addedStation->parking, carAutonomy);
                        if(carAutonomy > addedStation->maxFuel)
                            addedStation->maxFuel = carAutonomy;
                    }
                }

                printf("aggiunta\n");
            } else {
                printf("non aggiunta\n");
            }

        } else if (strcmp(input, REMOVESTAT) == 0) {
            scanf("%d", &distance);
            if (distance > 0) {
                if (removeStation(&sRoot, distance)) {
                    printf("demolita\n");
                } else {
                    printf("non demolita\n");
                }
            } else {
                printf("Error in removal - invalid distance");
            }

        } else if (strcmp(input, ADDCAR) == 0) {
            scanf("%d", &distance);
            scanf("%d", &carAutonomy);

            station = checkStation(sRoot, distance);
            if(station != NULL){
                addCar((carBST_t **) &station->parking, carAutonomy);
                if(carAutonomy > station->maxFuel)
                    station->maxFuel = carAutonomy;
                printf("aggiunta\n");
            } else {
                printf("non aggiunta\n");
            }

        } else if (strcmp(input, REMOVECAR) == 0) {
            scanf("%d", &distance);
            scanf("%d", &carAutonomy);

            station = checkStation(sRoot,distance);
            if(station != NULL){
                if (removeCar(&station->parking, carAutonomy)) {
                    if(station->maxFuel == carAutonomy){
                        station->maxFuel = findNewMaxFuel(station->parking);
                    }
                    printf("rottamata\n");
                } else {
                    printf("non rottamata\n");
                }
            } else {
                printf("non rottamata\n");
            }

        } else if (strcmp(input, BESTROUTE) == 0) {
            int start,end;
            statBST_t * startStat = NULL;
            statBST_t *endStat = NULL;
            scanf("%d", &start);
            scanf("%d", &end);

            startStat = checkStation(sRoot, start);
            endStat = checkStation(sRoot, end);

            if(startStat == NULL || endStat == NULL){
                printf(("nessun percorso\n"));
            }
            else{
                pathList_t * bestPath = NULL;
                if(start < end){
                    bestPath = findBestPath(sRoot,startStat, endStat);
                    if(bestPath == NULL){
                        printf("nessun percorso");
                    } else {
                        printReverse(bestPath);
                    }
                } else if(start > end){
                    bestPath = findBestPathReversed(sRoot,startStat, endStat);
                    if(bestPath == NULL){
                        printf("nessun percorso");
                    } else {
                        if(bestPath->statDist == startStat->distance){ //bestPath
                            pathList_t *s = bestPath;
                            pathList_t *temp = NULL;
                            while (s->next != NULL) {
                                printf("%d ", s->statDist);
                                temp = s->next;
                                free(s);
                                s = temp;
                            }
                            printf("%d", s->statDist);
                        } else{
                            printReverse(bestPath);
                        }

                    }
                } else if(start == end){
                    printf("%d",start);
                }
                printf("\n");
            }
        }
        else if(strcmp(input, "stampa-albero") == 0){
            printTree(sRoot);
        }

        input[0] = 1;

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


    if(dist == 0){
        return (*root);
    }

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
    //station->parent = parent;

    //insert the station in the tree
    if(parent != NULL){
        if(dist < parent->distance)
            parent->left = station;
        else
            parent->right = station;
        return station;

    }
    return NULL;

}

int removeStation (statBST_t ** root, int dist)
{
    statBST_t* current = *root;
    statBST_t* predecessor = *root;

    //if I want to remove the root of the bst
    if(dist == 0){
        carBST_t * parking = (*root)->parking;
        (*root)->parking = NULL;
        free(parking);
        (*root)->maxFuel = 0;
        return 1;
    }

    while (current != NULL && current->distance != dist) {
        predecessor = current;
        if (dist < current->distance)
            current = current->left;
        else
            current = current->right;
    }

    //the station is not present and cannot be deleted
    if (current == NULL) {
        return 0;
    }

    if(current->left == NULL || current->right == NULL){
        statBST_t * temp;
        if(current->left == NULL)
            temp = current->right;
        else
            temp = current->left;

        if(predecessor == NULL){
            free(current);
            return 1;
        }
        if(current == predecessor->left)
            predecessor->left = temp;
        else
            predecessor->right = temp;

        free(current);

    } else {
        statBST_t * parent = NULL;
        statBST_t * temp;
        temp = current->right;
        while(temp->left != NULL){
            parent = temp;
            temp = temp->left;
        }
        if(parent != NULL)
            parent->left = temp->right;
        else
            current->right = temp->right;
        current->distance = temp->distance;
        current->maxFuel = temp->maxFuel;
        current->parking = temp->parking;
        free(temp);
    }

    return 1; //station deleted successfully
}



int addCar(carBST_t ** root, int autonomy) {

    carBST_t *current = *root;
    carBST_t *parent = *root;
    carBST_t *car = NULL;

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
    carBST_t * prev = NULL;


    while (current != NULL && current->autonomy != aut) {
        prev = current;
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

    if(current->left == NULL || current->right == NULL){
        carBST_t * temp;
        if(current->left == NULL)
            temp = current->right;
        else
            temp = current->left;

        if(prev == NULL){
            free(temp);
            return 1;
        }

        if(current == prev->left)
            prev->left = temp;
        else
            prev->right = temp;

        free(current);
    } else {
        carBST_t * parent = NULL;
        carBST_t * temp;
        temp = current->right;
        while(temp->left != NULL){
            parent = temp;
            temp = temp->left;
        }
        if(parent != NULL)
            parent->left = temp->right;
        else
            current->right = temp->right;

        current->autonomy = temp->autonomy;
        current->carCounter = temp->carCounter;
        free(temp);
    }
    return 1;

}


pathList_t * findBestPath(statBST_t * root,statBST_t * start, statBST_t * end){
    statBST_t *current = NULL;
    pathList_t * bestPath = NULL;
    statBST_t * nextBestStation = NULL;
    statBST_t * temp = NULL;
    int stationPresent;

    if(start->maxFuel == 0)
        return NULL; //can't reach next station;

    bestPath = createNewPath(end);

    if(start->maxFuel + start->distance >= end->distance){
        bestPath = addToPath(bestPath, start);
        return bestPath; //can reach end station directly
    }
    //starting from the last station, goes backward checking all the stations to find the one closer to the start from which I can reach the current one
    current = end;
    while(current != start){
        temp = predecessor(root, current);
        if(temp == start && start->maxFuel + start->distance >= current->distance){
            bestPath = addToPath(bestPath, start);
            return bestPath;
        }
        nextBestStation = current;
        stationPresent = 0;
        while(temp != start){
            //checks all the previous stations from current to start
            if(temp->distance + temp->maxFuel >= current-> distance && temp->distance <= nextBestStation->distance){
                nextBestStation = temp;
                stationPresent = 1;
            }
            temp = predecessor(root, temp);
        }
        if(!stationPresent){
            //free the memory of the rest of the path
            pathList_t * p = bestPath;
            pathList_t * s = NULL;
            while(p != NULL){
                s = p->next;
                free(p);
                p = s;
            }
            return NULL; //there's no station from where I can reach the current one
        }
        bestPath = addToPath(bestPath, nextBestStation);
        //printf("%d\n",nextBestStation->distance);
        current = nextBestStation;
        if(start->distance + start->maxFuel >= nextBestStation->distance){
            bestPath = addToPath(bestPath, start);
            return bestPath;
        }
    }
    bestPath = addToPath(bestPath, start);

    return bestPath;
}

pathList_t * findBestPathReversed(statBST_t * root,statBST_t * start, statBST_t * end){

    statBST_t *current = NULL;
    pathList_t * bestPath = NULL;
    statBST_t * nextBestStation = NULL;
    statBST_t * temp = NULL;
    int maxReachableDist;
    int newReachableDist;
    int pathFound = 0;

    if(start->maxFuel == 0)
        return NULL; //can't reach next station;

    bestPath = createNewPath(start);

    if(start->distance - start->maxFuel <= end->distance){
        bestPath = addToPath(bestPath, end);
        return bestPath; //can reach end station directly
    }

    current = start;
    while(current != end && !pathFound){
        // temp = predecessor(root, current);
        maxReachableDist = current->distance - current->maxFuel;
        if(maxReachableDist <= end->distance){
            bestPath = addToPath(bestPath, end);
            pathFound = 1;
        } else {
            newReachableDist = start->distance;
            temp = predecessor(root, current);
            while(temp->distance >= maxReachableDist && temp != end){
                if(temp->distance - temp->maxFuel < newReachableDist){
                    newReachableDist = temp->distance - temp->maxFuel;
                    nextBestStation = temp;
                } else if(temp->distance - temp->maxFuel == newReachableDist && temp->distance < nextBestStation->distance){
                    nextBestStation = temp;
                }
                temp = predecessor(root, temp);
            }
            if(newReachableDist == start->distance || nextBestStation == NULL)
                return NULL;

            bestPath = addToPath(bestPath, nextBestStation);
            current = nextBestStation;
            nextBestStation = NULL;
        }

    }

    //search for a better path, starting from the end

    pathList_t * revisedPath = NULL;
    pathList_t *s1, *s2, *s3;
    statBST_t * sBST1, *sBST2, *sBST3, *newSBst2, *tempBST2;
    int isDifferent = 0;


    //finding the last element of the path list
    s1 = bestPath;
    while(s1->next != NULL){
        s1 = s1->next;
    }

    revisedPath = createNewPath(end);
    sBST1 = s1->station;

    while(s1->prev->prev != NULL && s1->prev != NULL){
        s2 = s1->prev;
        s3 = s2->prev;

      //  while(s1->statDist != start->distance){
        //sBST1 = s1->station;

        sBST2 = s2->station;
        sBST3 = s3->station;
        newSBst2 = NULL;
        tempBST2 = successor(root, sBST1);
        if(tempBST2->distance == s2->statDist){
            revisedPath = addToPath(revisedPath, sBST2);
        } else {
            while(tempBST2->distance < sBST2->distance && newSBst2 == NULL){
                if(tempBST2->distance >= (sBST3->distance - sBST3->maxFuel) && tempBST2->distance - tempBST2->maxFuel <= sBST1->distance){
                    newSBst2 = tempBST2;
                }
                tempBST2 = successor(root, tempBST2);
            }
            if(newSBst2 != NULL){
                revisedPath = addToPath(revisedPath, newSBst2);
                isDifferent = 1;
                sBST1 = newSBst2;
            }
            else{
                revisedPath = addToPath(revisedPath, sBST2);
                sBST1 = sBST2;
            }
        }

        //toDelete = s1;
        s1 = s2;
        //free(toDelete)
       // }

    }
    revisedPath = addToPath(revisedPath, start);
    //free(s2)
    //free(s3)

    if(isDifferent)
        return revisedPath;
    return bestPath;
}


pathList_t *addToPath(pathList_t * headStation, statBST_t *station) {

    pathList_t * newNode = (pathList_t*) malloc(sizeof (pathList_t));
    newNode->next = NULL;
    newNode->statDist = station->distance;
    newNode->station = station;

    pathList_t * temp = headStation;
    while(temp->next!=NULL)
        temp = temp->next;
    temp->next = newNode;

    newNode->prev = temp;

    return headStation;
}

pathList_t *createNewPath(statBST_t * start) {
    pathList_t * path = (pathList_t*) malloc(sizeof (pathList_t));
    path->next = NULL;
    path->prev = NULL;
    path->statDist = start->distance;
    path->station = start;
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

statBST_t * predecessor(statBST_t * root, statBST_t * target)
{
    statBST_t *predecessor = NULL;

    while (root != NULL) {
        if (target->distance > root->distance) {
            // If target is in the right subtree, update the predecessor and move to the right subtree
            predecessor = root;
            root = root->right;
        } else {
            // If target is in the left subtree, move to the left subtree
            root = root->left;
        }
    }

    return predecessor;
}


void printReverse(pathList_t * start)
{
    if (start->next == NULL){
        printf("%d",start->statDist);
        return;
    }
    printReverse(start->next);
    printf(" %d", start->statDist);
}

void printTree(statBST_t *root) {
    if (root == NULL) {
        return;
    }

    printTree(root->left);
    printf("%d ", root->distance);
    printf("(%d)\n",root->maxFuel);
    printTree(root->right);
}
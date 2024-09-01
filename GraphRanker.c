#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//************************** custom types **************************
typedef struct linkedListContent{
    int score;
    int inputOrdinal;
    struct linkedListContent *pNext;
    struct linkedListContent *pPrevious;
} linkedListElement;

typedef struct reusableRow{
    int *row;
    struct reusableRow *pNext;
}unusedRow;


//************************** global **************************
int numberOfNodes, nextRowToRead;
int ordinalPosition;
int bestK, alreadySaved;
int useless;
bool *visited;
int **matrixWithPointers;
unusedRow *reuseList;
linkedListElement *head, *tail;



//************************** prototypes **************************
void topK();
int calculateScore();
void addToLinkedList(int score);
int addUntil (int last);
void removeFromMatrix(int rowNumber);
void flushUnusedInput();


//************************** main **************************
int main() {
    if (2 != scanf("%d,%d%*c", &numberOfNodes, &bestK)){
        return -1;
    }
    char request;
    int score;
    bool array[numberOfNodes];

    if (bestK != 0){
        head = NULL;
        tail = NULL;
        reuseList = NULL;
        alreadySaved = 0;
        ordinalPosition = 0;
        visited = array;
        matrixWithPointers = malloc(numberOfNodes * sizeof(int));
    }


    while (1){
        printf("wiatingForInput...");
        if (1 != scanf("%c%*s%*c", &request)){
            return 0;
        }

        if ('T' == request){
            topK();
        } else if ('A' == request){
            nextRowToRead = 0;
            //serve molta inizializzazione
            if (0 != bestK && (alreadySaved == bestK && tail->score != 0 || alreadySaved < bestK) ){
                score = calculateScore();
                printf("\n-----> score: %d", score);
                addToLinkedList(score);
            } else {
                flushUnusedInput();
            }
            ordinalPosition ++;
            printf("returned");
        } else {
            return 0;
        }
    }
}


//************************** topK **************************
void topK(){
    linkedListElement *currentElement = tail;
    bool printSpace = false;
    while(NULL != currentElement){
        if (printSpace)
            printf(" ");
        else
            printSpace = true;

        printf("%d", currentElement->inputOrdinal);
        currentElement = currentElement->pNext;
    }
    printf("\n");
}


//************************** calculateScore **************************
int calculateScore(){
    if (0 == addUntil(0)){
        removeFromMatrix(0);
        flushUnusedInput();
        return 0; //nothing is reachable
    }

    int sum = 0;
    int currentDistance;

    //conviene mettere puntatore globale e instanziarli la prima volta che si chiama add dal main
    int *distance;
    int currentMinValue = 0;
    int currentMinPosition = 0;
    bool keepSearching = true;

    //inizializzazione, passo 0
    visited[0] = true;
    for (int i=1; i<numberOfNodes; i++)
        visited[i] = false;

    distance = matrixWithPointers[0];
    matrixWithPointers[0] = NULL;
    distance[0] = 0;
    for (int i=1; i<numberOfNodes; i++){
        if(distance[i] != 0 && (currentMinPosition == 0 || distance[i] <= currentMinValue)){
            currentMinPosition = i;
            currentMinValue = distance[i];
        }
    }

    while(keepSearching){
        //DEBUG
        printf("\n %d,%d \n", currentMinValue, currentMinPosition);

        visited[currentMinPosition] = true;
        //leggo fino alla riga che serve
        if(currentMinPosition >= nextRowToRead)
            addUntil(currentMinPosition);

        currentDistance = distance[currentMinPosition];
        //aggiorno distanze
        for (int i=0; i<numberOfNodes; i++){
            if (!visited[i] && matrixWithPointers[currentMinPosition][i] != 0 && (distance[i] == 0 || (matrixWithPointers[currentMinPosition][i] + currentMinValue) < distance[i]))
                distance[i] = matrixWithPointers[currentMinPosition][i] + currentDistance;
        }

        removeFromMatrix(currentMinPosition);
        keepSearching = false;
        currentMinPosition = 0;

        //trovo minore
        for (int i=0; i<numberOfNodes; i++){
            if(!visited[i] && distance[i] != 0 && (currentMinPosition == 0 || distance[i] <= currentMinValue)){
                currentMinPosition = i;
                currentMinValue = distance[i];
                keepSearching = true;
            }
        }
    }

    if(nextRowToRead <= numberOfNodes)
        flushUnusedInput();

    for (int i=0; i<numberOfNodes; i++)
        sum += distance[i];

    matrixWithPointers[0] = distance;
    for (int i=0; i<nextRowToRead; i++){
        if (NULL != matrixWithPointers[i])
            removeFromMatrix(i);
    }

    printf("\n-----> score: %d", sum);

    return sum;
}


//************************** addToLinkedList **************************
//prima di chiamare questa funzione, deve controllare se è migliore almeno del peggiore già salvato oppure se null
void addToLinkedList (int score){
    if (alreadySaved == bestK && score >= tail->score)
        return;

    linkedListElement *newElement;
    if (alreadySaved < bestK){
        newElement = malloc(sizeof(linkedListElement));
        alreadySaved ++;
    } else {
        newElement = tail;
        tail = tail->pNext;
        if (tail == NULL){
            head = NULL;
        } else {
            tail->pPrevious = NULL;
        }
    }

    newElement->inputOrdinal = ordinalPosition;
    newElement->score = score;

    if (tail == NULL ){
        head = newElement;
        tail = newElement;
        newElement->pPrevious = NULL;
        newElement->pNext = NULL;
    } else {
        if (score >= tail->score){
            newElement->pNext = tail;
            newElement->pPrevious = NULL;
            tail->pPrevious = newElement;
            tail = newElement;
        } else if (score <= head->score){
            newElement->pPrevious = head;
            newElement->pNext = NULL;
            head->pNext = newElement;
            head = newElement;
            printf("added 0\n");
        } else {
            linkedListElement *followingElement, *previousElement;
            if (tail->score - score < score - head->score){
                followingElement = tail;
                while (followingElement != NULL && followingElement->score > score)
                    followingElement = followingElement->pNext;
                previousElement = followingElement->pPrevious;
            } else {
                previousElement = head;
                while (previousElement != NULL && previousElement->score < score)
                    previousElement = previousElement->pPrevious;
                followingElement = previousElement->pNext;
            }
            newElement->pPrevious = previousElement;
            newElement->pNext = followingElement;
            previousElement->pNext = newElement;
            followingElement->pPrevious = newElement;
        }
    }
}

int addUntil (int last){
    int *singleRow;
    for (; nextRowToRead <= last; nextRowToRead++){

        if(NULL == reuseList){ //controllo se serve allocare nuova memoria o posso riutilizzare
            singleRow = malloc(numberOfNodes * sizeof(int));
        } else{
            unusedRow *toBeRemoved;
            singleRow = reuseList->row;
            toBeRemoved = reuseList;
            reuseList = reuseList->pNext;
            free(toBeRemoved);
        }

        for (int i=0; i<numberOfNodes; i++)
            scanf("%d%*c", &singleRow[i]);

        if (nextRowToRead == 0){
            bool nothingReachable = true;
            for (int j=0; j<numberOfNodes && nothingReachable; j++){
                if (0 != singleRow[j]){
                    nothingReachable = false;
                }
            }
            if (nothingReachable) { // caso in cui non c'è nulla di raggiungibile --> chiamo addToLinkedList(0);
                nextRowToRead++;
                return 0;
            }
        }
        matrixWithPointers[nextRowToRead] = singleRow;
    }
    return 1;
}

void removeFromMatrix(int rowNumber){
    unusedRow *pNewElement = malloc(sizeof(unusedRow));

    pNewElement->row = matrixWithPointers[rowNumber];
    matrixWithPointers[rowNumber] = NULL;

    if(NULL == reuseList){
        pNewElement->pNext = NULL;
        reuseList = pNewElement;
    } else {
        pNewElement->pNext = reuseList;
        reuseList = pNewElement;
    }
}

void flushUnusedInput(){
    for (int i=0; i<(numberOfNodes - nextRowToRead); i++)
        scanf("%*s%*c");
}


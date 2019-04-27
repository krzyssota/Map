//
// Created by krzubuntu on 27.04.19.
//

#include <stddef.h>
#include "dijkstra.h"
#include "structures.h"
#include "map.h"
#include "roadsRelated.h"
#include "routeRelated.h"

void push(Queue** queue, QueueElement* element){

    if((*queue)->head == NULL){
        (*queue)->head = element;
    } else {

        QueueElement* tmp = (*queue)->head;
        QueueElement* tmpPrev = NULL;

        while(tmp != NULL && tmp->distance <= element->distance){
            tmpPrev = tmp;
            tmp = tmp->next;
        }

        if(tmp == NULL){ // add as last element
            tmpPrev->next = element;
            element->prev = tmpPrev;

        } else if(tmp == (*queue)->head){ // while hasn't been entered; new head

            ((*queue)->head)->prev = element;
            element->next = (*queue)->head;

            (*queue)->head = element;

        } else { // insert between two elements

            tmpPrev->next = element;
            element->prev = tmpPrev;

            element->next = tmp;
            tmp->prev = element;
        }

    }
}

QueueElement* pop(Queue** queue){

    QueueElement* result = (*queue)->head;
    (*queue)->head = result->next;
    if((*queue)->head != NULL) {
        (*queue)->head->prev = NULL;
    }

    return result;
}

bool isEmpty(Queue* queue){
    return queue->head == NULL;
}

Queue* prepareQueue(Map* map,City* cityA, City* cityB) {

    Queue* queue = newQueue(cityB);
    push(&queue, newQueueElement(cityA, 0, NULL, NULL)); // source

    CityList* tmp = map->cityList;
    while (tmp != NULL) {

        if(tmp->city != cityA) {
            push(&queue, newQueueElement(tmp->city, INF, NULL, NULL));
        }
        tmp = tmp->next;
    }

    return queue;
}



QueueElement* findQueueElement(Queue* queue, City* city){

    QueueElement* tmp = queue->head;
    while(tmp != NULL && tmp->city != city){
        tmp = tmp->next;
    }

    return tmp;
}

void popElement(Queue **queue, QueueElement* element){

    /*QueueElement* tmp = (*queue)->head;
    while(tmp != NULL && tmp != element){
        tmp = tmp->next;
    }*/
    if(element == (*queue)->head){
        (*queue)->head = element->next;
    }

    if(element->prev != NULL) {
        (element->prev)->next = element->next;
    }
    if(element->next != NULL) {
        (element->next)->prev = element->prev;
    }
}


void updateElement(Queue **queue, QueueElement* faux){

    QueueElement* original = findQueueElement((*queue), faux->city);

    if(original->distance > faux->distance){

        popElement(queue, original);
        push(queue, faux);

    } else if(original->distance == faux->distance){

        if(original->oldestRoad->year < faux->oldestRoad->year){

            popElement(queue, original);
            push(queue, faux);
        }
    }
    //TODO free original or faux
}

void processNeighbours(Queue* queue, QueueElement* element){

    RoadList* tmp = element->city->roadList;

    while(tmp != NULL) {

        City* neighbour = getOtherCity(tmp->road, element->city);

        if(findQueueElement(queue, neighbour) != NULL) { // neighbour still in the queue

            long int fauxDistance = element->distance + (long int) findRoad(neighbour->roadList, element->city)->length;
            QueueElement* fauxNeighbour = newQueueElement(neighbour, fauxDistance, element, olderRoad(element->oldestRoad, tmp->road));

            updateElement(&queue, fauxNeighbour);
        }

        tmp = tmp->next;
    }
}

QueueElement* Dijkstra(Map* map,City* cityA, City* cityB){

    Queue* queue = prepareQueue(map, cityA, cityB);

    QueueElement* destinationElement = findQueueElement(queue, queue->destination)

    QueueElement* currElement = NULL;
    while(findQueueElement(queue, queue->destination) != NULL) { // destination is still in the queue

        destinationElement = findQueueElement(queue, queue->destination);

        currElement = pop(&queue);
        processNeighbours(queue, currElement);
    }

    if(destinationElement->distance == INF){
        return NULL;
    }

    return destinationElement;
}

































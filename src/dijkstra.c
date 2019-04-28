//
// Created by krzubuntu on 27.04.19.
//

#include <stddef.h>
#include "dijkstra.h"
#include "structures.h"
#include "map.h"
#include "roadsRelated.h"
#include "routeRelated.h"
#include "deleteStructure.h"

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

Queue* prepareQueue(Map* map, Route* routeA, Route* routeB, City* cityA, City* cityB) {

    Queue* queue = newQueue(cityB);
    push(&queue, newQueueElement(cityA, 0, NULL, NULL)); // source

    if(routeA->cityList != NULL && cityB == routeA->cityList->city){ // extending by adding prefix
        push(&queue, newQueueElement(cityB, INF, NULL, NULL));
    }

    CityList* tmp = map->cityList;
    while (tmp != NULL) {

        if(tmp->city != cityA && !routeContainsCity(routeA, tmp->city) && !routeContainsCity(routeB, tmp->city)) {
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

QueueElement* popElement(Queue **queue, QueueElement* element){

    if(element == (*queue)->head){
        (*queue)->head = element->next;
    }

    if(element->prev != NULL) {
        (element->prev)->next = element->next;
    }
    if(element->next != NULL) {
        (element->next)->prev = element->prev;
    }
    return element;
}


void updateElement(Queue **queue, QueueElement* alternative){

    QueueElement* original = findQueueElement((*queue), alternative->city);

    if(original->distance > alternative->distance){

        free(popElement(queue, original));
        push(queue, alternative);

    } else if(original->distance == alternative->distance){

        if(original->oldestRoad->year < alternative->oldestRoad->year){

            free(popElement(queue, original));
            push(queue, alternative);
        }
    } else {
        free(alternative);
    }
}

void processNeighbours(Queue* queue, Route* route, QueueElement* element, Road* roadRemoved){

    RoadList* tmp = element->city->roadList;

    while(tmp != NULL) {

        if(!routeContainsRoad(route, tmp->road) && tmp->road != roadRemoved) { // do not accept road that is removed

            City* neighbour = getOtherCity(tmp->road, element->city);

            if (findQueueElement(queue, neighbour) != NULL) { // neighbour still in the queue

                long int alternativeDistance = element->distance + (long int) findRoad(neighbour, element->city)->length;
                QueueElement* alternativeNeighbour = newQueueElement(neighbour, alternativeDistance, element,
                                                              olderRoad(element->oldestRoad, tmp->road));

                updateElement(&queue, alternativeNeighbour);
            }
        }

        tmp = tmp->next;
    }
}


QueueElement* Dijkstra(Map* map, Route* routeA, Route* routeB, City* cityA, City* cityB, Road* roadRemoved){

    Queue* queue = prepareQueue(map, routeA, routeB, cityA, cityB);

    QueueElement* destinationElement = findQueueElement(queue, queue->destination);

    QueueElement* currElement = NULL;
    while(findQueueElement(queue, queue->destination) != NULL) { // destination is still in the queue

        destinationElement = findQueueElement(queue, queue->destination);

        currElement = pop(&queue);
        processNeighbours(queue, routeA, currElement, roadRemoved);
    }
    cleanQueue(&queue);
    free(queue);

    if(destinationElement->distance == INF){
        return NULL;
    }

    return destinationElement;
}

































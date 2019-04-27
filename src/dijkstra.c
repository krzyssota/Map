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
        QueueElement* tmpPrev = tmp->prev;

        while(tmp != NULL && tmp->distance <= element->distance){
            tmpPrev = tmp;
            tmp = tmp->next;
        }

        if(tmp == NULL){ // add as last element
            tmpPrev->next = element;
            element->prev = tmpPrev;

        } else if(tmp == (*queue)->head){ // while hasn't been entered; new head

            ((*queue)->head)->prev = tmp;
            tmp->next = (*queue)->head;

            (*queue)->head = tmp;

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
void updateNeighbourInQueue(queue, )

void processNeighbours(Map* map, Queue* queue, QueueElement* element){

    RoadList* tmp = element->city->roadList;
    while(tmp != NULL) {

        City* neighbour = getOtherCity(tmp->road, element->city);
        updateNeighbourInQueue(queue, )
    }
}

QueueElement* Dijkstra(Map* map,City* cityA, City* cityB){

    Queue* queue = prepareQueue(map, cityA, cityB);

    while(!isEmpty(queue)) {

        QueueElement* currElement = pop(&queue);
        processNeighbours(map, queue, currElement);
    }


}

































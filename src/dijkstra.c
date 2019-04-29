//
// Created by krzubuntu on 27.04.19.
//

#include <stddef.h>
#include <assert.h>
#include <values.h>
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

        while(tmp != NULL && tmp->distance < element->distance){
            tmpPrev = tmp;
            tmp = tmp->next;
        }

        if(tmp == NULL){
            assert(tmpPrev != NULL);
            tmpPrev->next = element;
            element->prev = tmpPrev;

        } else if(tmp == (*queue)->head){

            ((*queue)->head)->prev = element;
            element->next = (*queue)->head;

            (*queue)->head = element;

        } else {

            int tmpOldestRoadYear = INT_MIN;
            int elementOldestRoadYear = INT_MIN;
            if(tmp->oldestRoad != NULL ){
                tmpOldestRoadYear = tmp->oldestRoad->year;
            }
            if(element->oldestRoad != NULL){
                elementOldestRoadYear = element->oldestRoad->year;
            }

            if (tmp->distance == element->distance && tmpOldestRoadYear > elementOldestRoadYear) {

                    QueueElement *nextEl = tmp->next;
                    tmp->next = element;
                    element->prev = tmp;
                    element->next = nextEl;
                    if (nextEl != NULL) {
                        nextEl->prev = element;
                    }

            }
            assert(tmpPrev != NULL);
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
/** @brief Dodaje odpowiednie elementy do kolejki.
 * Odpowiednimi miastami sa miasta spoza drogi krajowej A, drogi krajowej B.
* @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeA      – wskaźnik na droge krajowa.
* @param[in] routeB      – wskaźnik na droge krajowa.
* @param[in] cityA      – wskaźnik na miasto.
* @param[in] cityB      – wskaźnik na miasto.
* @return Wskaznik na kolejke. NULL jesli nie udalo sie zaalokowac pamieci.
*/
Queue* prepareQueue(Map* map, Route* routeA, Route* routeB, City* cityA, City* cityB) {

    Queue* queue = newQueue(cityB);
    if(queue == NULL){
        return NULL;
    }
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



QueueElement* findQueueElement(Queue* queue, City* city){ ///< Find element pointing to city in the queue.

    QueueElement* tmp = queue->head;
    while(tmp != NULL && tmp->city != city){
        tmp = tmp->next;
    }

    return tmp;
}

QueueElement* popElement(Queue **queue, QueueElement* element){ ///< Take element out of the queue.

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

/** @brief Sprawdza czy da sie zmniejszyc priorytet elementu.
 * @param[in] queue      – wskaźnik na kolejke.
*  @param[in] alternative   –  element z alternatywnym priorytetem.
*/
void updateElement(Queue **queue, QueueElement* alternative){

    QueueElement* original = findQueueElement((*queue), alternative->city);

    if(original->distance > alternative->distance){

        free(popElement(queue, original));
        push(queue, alternative);

    } else if(original->distance == alternative->distance){

        if(original->oldestRoad->year < alternative->oldestRoad->year){

            free(popElement(queue, original));
            push(queue, alternative);
        } else if(original->oldestRoad->year == alternative->oldestRoad->year) {
            original->ambiguous = true;
            free(alternative);
        }
    } else {
        free(alternative);
    }
}

/** @brief Sprawdza czy da sie zmniejszyc priorytet sasiadom elementu.
 * @param[in] queue      – wskaźnik na kolejke.
 * * @param[in] route      – wskaźnik na droge krajowa.
 * * @param[in] element      – wskaźnik na element kolejki.
*  @param[in] roadRemoved   –  droga usuwana w funkcji removeRoad. Dla innych wywolan wartosc NULL.
*/
void processNeighbours(Queue* queue, Route* route, QueueElement* element, Road* roadRemoved){

    RoadList* tmp = element->city->roadList;

    while(tmp != NULL) {

        if(!routeContainsRoad(route, tmp->road) && tmp->road != roadRemoved) { ///< Do not consider road that is being removed.

            City* neighbour = getOtherCity(tmp->road, element->city);

            if (findQueueElement(queue, neighbour) != NULL) { ///< Neighbour still in the queue (can be processed)

                long int alternativeDistance = element->distance + (long int) findRoad(neighbour, element->city)->length;

                QueueElement* alternativeNeighbour = newQueueElement(neighbour, alternativeDistance, element,
                                                              olderRoad(element->oldestRoad, tmp->road));

                updateElement(&queue, alternativeNeighbour);
            }
        }

        tmp = tmp->next;
    }
}

/** @brief Sprawdza czy da sie zmniejszyc priorytet sasiadom elementu.
 * @param[in] map      – wskaźnik na mape.
 * @param[in] routeA      – wskaźnik na droge krajowa.
 *  @param[in] routeB      – wskaźnik na droge krajowa.
 * @param[in] cityA      – wskaźnik na miasto.
 * @param[in] cityB      – wskaźnik na miasto.
 * @param[in] roadRemoved   –  droga usuwana w funkcji removeRoad. Dla innych wywolan wartosc NULL.
 * @return Wskaznik na element kolejki wskazujacy na miasto cel.
*/
QueueElement* Dijkstra(Map* map, Route* routeA, Route* routeB, City* cityA, City* cityB, Road* roadRemoved){

    Queue* queue = prepareQueue(map, routeA, routeB, cityA, cityB); ///< Prepare priority queue.
    if(queue == NULL){
        return NULL;
    }

    QueueElement* destinationElement = findQueueElement(queue, queue->destination);

    QueueElement* currElement = NULL;
    while(findQueueElement(queue, queue->destination) != NULL) { ///< Destination is still in the queue

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

































/**
 * @file
 * Moduł zawiera funkcję pomocnicze dla algorytmu Dijkstry i funkcję obsługujące kolejkę priorytetową.
 * @author Krzysztof Sota
 * @date 18.05.2019
 * */

#include <stddef.h>
#include <assert.h>
#include <values.h>
#include "dijkstra.h"
#include "structures.h"
#include "map.h"
#include "roadsRelated.h"
#include "routeRelated.h"
#include "deleteStructure.h"

/**@brief Dodaje element na kolejkę priorytetową.
 * @param[in, out] queue - wskaźnik na wskaźnik na kolejkę priorytetową
 * @param element  - element kolejki
 */
void push(Queue** queue, QueueElement* element){

    if((*queue)->head == NULL){
        (*queue)->head = element;
    } else {

        QueueElement* curr = (*queue)->head;
        QueueElement* prevCurr = NULL;

        while(curr != NULL && element->distance > curr->distance){
            prevCurr = curr;
            curr = curr->next;
        }

        int currOldestRoadYear = INT_MIN;
        int elementOldestRoadYear = INT_MIN;
        if(curr != NULL && curr->oldestRoad != NULL ){
            currOldestRoadYear = curr->oldestRoad->year;
        }
        if(element->oldestRoad != NULL){
            elementOldestRoadYear = element->oldestRoad->year;
        }

        if(curr == NULL) { ///< na koniec kolejki

            assert(prevCurr != NULL);
            prevCurr->next = element;
            element->prev = prevCurr;

        } else if(curr == (*queue)->head){ ///< przed lub tuż za wierzchem (tuż za tzn. równe odległości, starsza droga)

            if(element->distance < curr->distance){ ///< krotsza droga niz wierzch

                curr->prev = element;
                element->next = curr;

                (*queue)->head = element;

            } else {

                if (elementOldestRoadYear > currOldestRoadYear) { ///< taka sama droga ale młodsza

                    curr->prev = element;
                    element->next = curr;

                    (*queue)->head = element;

                } else { ///< taka sama droga ale starsza

                    QueueElement *tmp = curr->next;

                    curr->next = element;
                    element->prev = curr;

                    element->next = tmp;
                    if (tmp != NULL) {
                        tmp->prev = element;
                    }
                }
            }

        } else { ///< w środek kolejki, przed curr lub tuż za (tuż za tzn. równe odległości, starsza droga)

            if(elementOldestRoadYear > currOldestRoadYear){ ///< taka sama droga ale młodsza

                curr->prev = element;
                element->next = curr;

                prevCurr->next = element;
                element->prev = prevCurr;


            } else { ///< taka sama droga ale starsza

                QueueElement* tmp = curr->next;

                curr->next = element;
                element->prev = curr;

                element->next = tmp;
                if(tmp != NULL) {
                    tmp->prev = element;
                }
            }
        }

    }
}
/**@brief Zdejmuje element z najniższym priorytetem z kolejki.
 * @param[in, out] queue - wskaźnik na wskaźnik na kolejkę priorytetową
 * @return wskaźnik na element z najniższym priorytetem.
 */
QueueElement *pop(Queue **queue) {

    QueueElement *result = (*queue)->head;

    if (result != NULL) {

        (*queue)->head = (*queue)->head->next;

        result->prev = result->next = NULL;

        if ((*queue)->head != NULL) {
            (*queue)->head->prev = NULL;
        }
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

    push(&queue, newQueueElement(cityA, 0, NULL, NULL)); // origin


    CityList* currCityList = map->cityList;
    while (currCityList != NULL) {

        if(!routeContainsCity(routeA, currCityList->city) && !routeContainsCity(routeB, currCityList->city)
            && currCityList->city != cityA && currCityList->city != cityB) {

            push(&queue, newQueueElement(currCityList->city, INF, NULL, NULL));
        }
        currCityList = currCityList->next;
    }
    push(&queue, newQueueElement(cityB, INF, NULL, NULL)); // target


    return queue;
}

/**@brief Znajduję element kolejki wskazujący na miasto.
 * @param[in, out] queue - wskaźnik na wskaźnik na kolejkę priorytetową
 * @param[in] city - wskaźnik na miasto
 * @return wskaźnik na element kolejki wskazujący na miasto, lub NULL jeśli element taki nie istnieje.
 */
QueueElement* findQueueElement(Queue* queue, City* city){ ///<

    QueueElement* currElement = queue->head;
    while(currElement != NULL && currElement->city != city){
        currElement = currElement->next;
    }

    return currElement;
}
/**@brief Zdejmuje konkretny element z kolejki priorytetowej.
 * @param[in, out] queue - wskaźnik na wskaźnik na kolejkę priorytetową
 * @param[in] element - wskaźnik na element kolejki
 * @return wskaźnik na usunięty element.
 */
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

    element->prev = element->next = NULL;

    return element;
}

/**@brief Sprawdza czy da sie zmniejszyc priorytet elementu. Jeśli tak to robi to.
 * @param[in,out] queue - wskaźnik na wskaźnik na kolejkę priorytetową.
 * @param[in] original - wskaźnik na element, którego priorytet rozważamy.
 * @param[in] alternativeDistance - alternatywna długość ścieżce od źródła do elementu
 * @param[in] alternativePredecessor - alternatywna poprzednik na ścieżce od źródła do elementu
 * @param[in] alternativeOldestRoad - alternatywna najstarsza droga na ścieżce od źródła do elementu
 */
void updateElement(Queue** queue, QueueElement* original, long int alternativeDistance, QueueElement* alternativePredecessor, Road* alternativeOldestRoad){


    if(original->distance > alternativeDistance){

        original->distance = alternativeDistance;
        original->oldestRoad = alternativeOldestRoad;
        original->predecessor = alternativePredecessor;
        if(original->ambiguous == true){
            original->ambiguous = false;
        }
        popElement(queue, original);
        push(queue, original);

    } else if(original->distance == alternativeDistance){

        int originalOldestRoadYear = INT_MIN;
        int alternativeOldestRoadYear = INT_MIN;
        if(original->oldestRoad != NULL ){
            originalOldestRoadYear = original->oldestRoad->year;
        }
        if(alternativeOldestRoad != NULL){
            alternativeOldestRoadYear = alternativeOldestRoad->year;
        }

        if(originalOldestRoadYear < alternativeOldestRoadYear){

            original->distance = alternativeDistance;
            original->oldestRoad = alternativeOldestRoad;
            original->predecessor = alternativePredecessor;
            if(original->ambiguous == true){
                original->ambiguous = false;
            }
            popElement(queue, original);
            push(queue, original);

        } else if(originalOldestRoadYear == alternativeOldestRoadYear){ ///< && original->distance == alternativeDistance

            original->ambiguous = true;

        }
    }
}

/** @brief Sprawdza czy da sie zmniejszyc priorytet sasiadom elementu.
 * @param[in, out] queue - wskaźnik na wskaźnik na kolejkę priorytetową
 * * @param[in] routeA      – wskaźnik na droge krajowa. Z tych miast algorytm nie może skorzystać przy budowie nowej drogi krajowej.
 * @param[in] routeB      – wskaźnik na droge krajowa. Z tych miast algorytm nie może skorzystać przy budowie nowej drogi krajowej.
 * * @param[in] element      – wskaźnik na element kolejki.
*  @param[in] roadRemoved   –  droga usuwana w funkcji removeRoad. Dla innych wywolan wartosc NULL.
*/
void processNeighbours(Queue** queue, Route* routeA, Route* routeB, QueueElement* element, Road* roadRemoved){

    RoadList* currRoadList = element->city->roadList;

    while(currRoadList != NULL) {

        if(!routeContainsRoad(routeA, currRoadList->road) && !routeContainsRoad(routeB, currRoadList->road) && currRoadList->road != roadRemoved) {
            ///< Do not consider road that is being removed or roads already used in the route.

            City* neighbourCity = getOtherCity(currRoadList->road, element->city);

            QueueElement* originalNeighbour = findQueueElement(*queue, neighbourCity);

            if (originalNeighbour != NULL) { ///< Neighbour still in the queue (can be processed)

                long int alternativeDistance = element->distance + (long int) currRoadList->road->length;

                updateElement(queue, originalNeighbour, alternativeDistance, element, olderRoad(element->oldestRoad, currRoadList->road));
            }
        }

        currRoadList = currRoadList->next;
    }
}

QueueElement* Dijkstra(Map* map, Route* routeA, Route* routeB, City* cityA, City* cityB, Road* roadRemoved, Queue** storage){

    Queue* queue = prepareQueue(map, routeA, routeB, cityA, cityB); ///< Prepare priority queue.
    if(queue == NULL){
        return NULL;
    }

    QueueElement* targetElement = NULL;

    QueueElement* currElement = NULL;

    do {

        currElement = popElement(&queue, queue->head);
        if(currElement->city == queue->target){
            targetElement = currElement;
        }
        push(storage, currElement);

        processNeighbours(&queue, routeA, routeB, currElement, roadRemoved);

    } while(currElement->city != queue->target);

    while(queue->head != NULL){
        push(storage, popElement(&queue, queue->head));
    }
    free(queue);

    assert(targetElement != NULL);
    return targetElement;
}

































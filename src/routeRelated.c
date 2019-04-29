#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "routeRelated.h"
#include "structures.h"
#include "map.h"
#include "dijkstra.h"
#include "roadsRelated.h"

Road* olderRoad(Road* roadA, Road* roadB){

    int a = INT16_MAX;
    if(roadA != NULL) {
        a = roadA->year;
    }
    int b = INT16_MAX;
    if(roadB != NULL) {
        b = roadB->year;
    }

    if(a > b) return roadB;
    return roadA;
}

/** W drogach skladajacych sie na droge krajowa zapisuje o tym informacje.
 */
void addRouteInfoToRoads(Route* route){

    CityList* cityList = route->cityList;

    while(cityList != NULL){

        RoadList* roadList = cityList->city->roadList;

        while(roadList != NULL){
            if(routeContainsRoad(route, roadList->road)) {
                roadList->road->routesBelonging[route->routeId] = route;
            }
            roadList = roadList->next;
        }

        cityList = cityList->next;
    }
}
/** @brief Znajduje najkrotsza sciezke pomiedzy miastem A i miastem B.
 * Rozwiazanie nie zawiera miast z drogi krajowej A, drogi krajowej B, ani drogi usuwanej.
 * @param[in] map      – wskaźnik na mape.
 * @param[in] routeA      – wskaźnik na droge krajowa.
 * @param[in] routeB      – wskaźnik na droge krajowa.
 * @param[in] cityA      – wskaźnik na miasto.
 * @param[in] cityB      – wskaźnik na miasto.
 * @param[in] yearOfOldestRoad      – wskaznik na wiek najstarszej drogi ze znalezionej sciezki.
 * @param[in] roadRemoved   –  droga usuwana w funkcji removeRoad. Dla innych wywolan wartosc NULL.
 * @return Wskaznik na liste miast z ktorych sklada sie sciezka jesli sciezka zostala znaleziona.
 * Wartość NULL, jeśli nie zostala znaleziona.
 */
CityList* findShortestPath(Map* map, Route* routeA, Route* routeB, City* cityA, City* cityB, int* yearOfOldestRoad, Road* roadRemoved){

    QueueElement* destination = Dijkstra(map, routeA, routeB, cityA, cityB, roadRemoved);

    if(destination == NULL){ // no optimal path found
        return NULL;
    }

    (*yearOfOldestRoad) = destination->oldestRoad->year;

    QueueElement* tmp = destination;
    CityList* cityList = NULL;
    int flag = 1;
    while(tmp != NULL && flag == 1) {

        if(tmp->ambiguous == true){
            flag = 0;
        }
        CityList* newStartingElement = newCityList();

        newStartingElement->city = tmp->city;

        newStartingElement->next = cityList;
        if(cityList != NULL) {
            cityList->prev = newStartingElement;
        }

        cityList = newStartingElement;
        QueueElement* toDelete = tmp;
        tmp = tmp->predecessor;
        free(toDelete);

    }
    if(flag == 0){

        while(cityList != NULL){
            CityList* toDelete = cityList;
            cityList = cityList->next;
            free(toDelete);
        }
        return NULL;
    }
    return cityList;


}

City* getOtherCity(Road *road, City *city){

    if(city == road->cityA) return road->cityB;
    if(city == road->cityB) return road->cityA;
    return NULL;
}

bool routeContainsCity(Route* route, City* city){
    if(route == NULL){
        return false;
    }
    CityList* cityList = route->cityList;
    while(cityList != NULL && cityList->city != city){
        cityList = cityList->next;
    }

    if(cityList != NULL){
        return true;
    }
    return false;
}

bool routeContainsRoad(Route* route, Road* road){

    CityList* cityList = route->cityList;
    if(cityList == NULL){ ///< Wywolana w addRoute lub extendRoute. road jako zmienna dummy = NULL
        return false;
    }

    bool contains = false;

    while(cityList->next != NULL && contains == false){

        if((road->cityA == cityList->city && road->cityB == cityList->next->city)
          ||(road->cityB == cityList->city && road->cityA == cityList->next->city)){
            contains = true;
        }

        cityList = cityList->next;
    }
    return contains;
}

unsigned calculateLength(CityList* path){

    unsigned length = 0;

    while(path->next != NULL){

        Road* road = findRoad(path->city, path->next->city);
        assert(road != NULL);

        length += road->length;

        path = path->next;
    }

    return length;
}

int betterPath(int firstOldestRoadYear, unsigned firstLength, int secondOldestRoadYear, unsigned secondLength){

    if(firstLength < secondLength) return 1;
    if(firstLength > secondLength) return 2;

    else {

        if (firstOldestRoadYear > secondOldestRoadYear) return 1;
        if (firstOldestRoadYear < secondOldestRoadYear) return 2;

        return 0;
    }


}

/** @brief Wklada liste miast ze sciezki do drogi krajowej.
 * @param[in] path      – wskaźnik na sciezke.
 * @param[in] route      – wskaźnik na droge krajowa.
 * @param[in] cityA      – wskaźnik na miasto.
 * @param[in] cityB      – wskaźnik na miasto.
 * @param[in] yearOfOldestRoad      – wskaznik na wiek najstarszej drogi ze znalezionej sciezki.
 */
void insertPathIntoRoute(CityList* path, Route* route, City* cityA, City* cityB){

    CityList* cityList = route->cityList;

    while(cityList != NULL && cityList->city != cityA){
        cityList = cityList->next;
    }
    assert(cityList != NULL);

    CityList* toDelete = path;

    CityList* otherEnd = cityList->next;

    cityList->next = path->next;
    path->next->prev = cityList;

    path = path->next;

    while(path != NULL && path->city != cityB){
        path = path->next;
    }
    assert(path != NULL);

    path->next = otherEnd->next;
    otherEnd->next->prev = path;

    free(toDelete);
    free(otherEnd);
}


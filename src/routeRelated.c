#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "routeRelated.h"
#include "structures.h"
#include "map.h"
#include "dijkstra.h"
#include "roadsRelated.h"
#include "stringsHandling.h"
#include "deleteStructure.h"
#include <string.h>
#include <values.h>

Road* olderRoad(Road* roadA, Road* roadB){

        int a = INT_MAX;
    if(roadA != NULL) {
        a = roadA->year;
    }
    int b = INT_MAX;
    if(roadB != NULL) {
        b = roadB->year;
    }

    if(a > b) return roadB;
    return roadA;
}


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

void deleteRouteInfoFromRoad(Route* route){

    CityList* cityList = route->cityList;

    while(cityList != NULL){

        RoadList* roadList = cityList->city->roadList;

        while(roadList != NULL){
            if(routeContainsRoad(route, roadList->road)) {
                roadList->road->routesBelonging[route->routeId] = NULL;
            }
            roadList = roadList->next;
        }

        cityList = cityList->next;
    }
}

CityList* recoverPath(QueueElement* target, ShortestPathResult* resultContainer) {

    QueueElement *curr = target;
    CityList *resultingCityList = NULL;

    while (curr != NULL) {

        if (curr->ambiguous == true && resultContainer->resultEnum == FOUND) {
            resultContainer->resultEnum = AMBIGUOUS;
        }

        CityList *newStartingElement = newCityList();
        newStartingElement->city = curr->city;

        newStartingElement->next = resultingCityList;
        if (resultingCityList != NULL) {
            resultingCityList->prev = newStartingElement;
        }

        resultingCityList = newStartingElement;

        curr = curr->predecessor;

    }

    return resultingCityList;
}

ShortestPathResult* findShortestPath(Map* map, Route* routeA, Route* routeB, City* cityA, City* cityB, Road* roadRemoved){

    Queue* storage = newQueue(NULL); ///< błąd pamięci
    if(storage == NULL){
        return NULL;
    }
    QueueElement* target = Dijkstra(map, routeA, routeB, cityA, cityB, roadRemoved, &storage);

    ShortestPathResult* resultContainer = newShortestPathResult();
    if(resultContainer == NULL){ ///< błąd pamięci
        cleanQueue(&storage);
        free(storage);
        return NULL;
    }
    if(target == NULL){
        deleteShortestPathResult(resultContainer); ///< błąd pamięci w Dijkstrze
        free(resultContainer);
        cleanQueue(&storage);
        free(storage);
        return NULL;
    }

    if(target->distance == INF || target->oldestRoad == NULL){
        resultContainer->resultEnum = NOT_FOUND;
    }
    if(target->oldestRoad != NULL){
        resultContainer->oldestRoadYear = target->oldestRoad->year;
    }
    resultContainer->length = target->distance;

    CityList* resultingCityList = recoverPath(target, resultContainer);

    cleanQueue(&storage);
    free(storage);

    resultContainer->path = resultingCityList;

    return resultContainer;
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

bool routeContainsRoad(Route *route, Road *road) {

    if (route == NULL) {
        return false;
    }

    CityList *cityList = route->cityList;
    if (cityList == NULL) {
        return false;
    }

    bool contains = false;

    while (cityList->next != NULL && contains == false) {

        if ((road->cityA == cityList->city && road->cityB == cityList->next->city)
            || (road->cityB == cityList->city && road->cityA == cityList->next->city)) {
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

int betterPath(ShortestPathResult* res1, ShortestPathResult* res2){

    if(res1->resultEnum == FOUND && res2->resultEnum == FOUND){
        if(res1->length < res2->length) {
            return 1;
        } else if(res1->length > res2->length){
            return 2;
        } else { // TODO tutaj trzeba jeszcze porównywać najstarsze drogi
            return 0;
        }
    }

    if(res1->resultEnum == FOUND && res2->resultEnum == NOT_FOUND){
        return 1;
    }
    if(res1->resultEnum == NOT_FOUND && res2->resultEnum == FOUND){
        return 2;
    }

    if(res1->resultEnum == AMBIGUOUS){
        if(res2->resultEnum == FOUND && res2->length < res1->length){
            return 2;
        } else {
            return 0;
        }
    }
    if(res2->resultEnum == AMBIGUOUS){
        if(res1->resultEnum == FOUND && res1->length < res2->length){
            return 1;
        } else {
            return 0;
        }
    }

    if(res1->resultEnum == FOUND && res2->resultEnum == FOUND){
        if(res1->length < res2->length) {
            return 1;
        } else if(res1->length > res2->length){
            return 2;
        } else {
            return 0;
        }
    } else { ///< res1->resultEnum == NOT_FOUND && res2->resultEnum == NOT_FOUND
        return 0;
    }

}


void insertPathIntoRoute(CityList* path, Route* route, City* cityA, City* cityB){

    CityList* cityList = route->cityList;

    while(cityList != NULL && cityList->city != cityA){
        cityList = cityList->next;
    }
    assert(cityList != NULL);

    CityList* toDelete = path;

    CityList* otherEnd = cityList->next;

    cityList->next = path->next;
    assert(path->next != NULL); ///< ścieżka jest co najmniej długości 2
    path->next->prev = cityList;

    path = path->next;
    free(toDelete);

    while(path != NULL && path->city != cityB){
        path = path->next;
    }
    assert(path != NULL);

    path->next = otherEnd->next;
    if(otherEnd->next != NULL) {
        otherEnd->next->prev = path;
    }
    free(otherEnd);

    addRouteInfoToRoads(route);
}

bool addCityToRoute(City* city, Route* route){

    CityList* cityList = route->cityList;

    if(cityList == NULL){

        route->cityList = newCityList();
        if(route->cityList == NULL){
            return false;
        }

        route->cityList->city = city;

        return true;

    } else {

        while (cityList->next != NULL) {
            cityList = cityList->next;
        }

        cityList->next = newCityList();
        if(cityList->next == NULL){
            return false;
        }
        (cityList->next)->prev = cityList;

        cityList->next->city = city;

        return true;

    }
}

bool newRouteFromRouteParam(Map* map, RouteParam* routeParam) {

    if(!correctId(routeParam->id)){
        return false;
    }

    if(map->routes[routeParam->id] != NULL){ ///< Droga krajowa o podanym id juz istnieje.
        return false;
    }

    Route* newRoute = createNewRoute(routeParam->id);
    if(newRoute == NULL){
        return false;
    }


    char* city1 = routeParam->cities[0];
    if(!correctName(city1)){
        deleteRoute(newRoute);
        return false;
    }
    char* city2;

    int i = 1;
    while(i < routeParam->cFilled){ ///< Sprawdza czy można dodać drogi o parametrach podanych w routeParam.

        city2 = routeParam->cities[i];

        Road* road = findRoad(findCityByName(map->cityList, city1), findCityByName(map->cityList, city2));

        if(road != NULL){

            if(road->length != routeParam->lengths[i-1] || road->year > routeParam->years[i-1]) {
                deleteRoute(newRoute);
                return false;
            }

        }

        city1 = city2;
        i++;
    }

    city1 = routeParam->cities[0];
    i = 1;
    while(i < routeParam->cFilled){ ///< Dodaje drogi i miasta o parametrach podanych w routeParam.

        city2 = routeParam->cities[i];

        Road* road = findRoad(findCityByName(map->cityList, city1), findCityByName(map->cityList, city2));

        if(road != NULL){

            if(!repairRoad(map, city1, city2, routeParam->years[i-1])){
                deleteRoute(newRoute);
                return false;
            }

        } else if(!addRoad(map, city1, city2, routeParam->lengths[i-1], routeParam->years[i-1])){
            deleteRoute(newRoute);
            return false;
        }

        if(i == 1) {
            if(!addCityToRoute(findCityByName(map->cityList, city1), newRoute)){
                deleteRoute(newRoute);
                return false;
            }
        }
        if(!addCityToRoute(findCityByName(map->cityList, city2), newRoute)){
            deleteRoute(newRoute);
            return false;
        }

        city1 = city2;
        i++;
    }

    addRouteInfoToRoads(newRoute); ///< Drogi maja informacje do jakich drog krajowych naleza.

    map->routes[newRoute->routeId] = newRoute;

    return true;
}

City* occurrenceInRoute(int x, Route *route, City *cityA, City *cityB){

    CityList* curr = route->cityList;

    while(assert(curr != NULL), curr->city != cityA && curr->city != cityB){
        curr = curr->next;
    }

    if(curr->city == cityA){
        if(x == 1) return cityA;
        if(x == 2) return cityB;
    } else { ///< curr->city == cityA
        if(x == 1) return cityB;
        else return cityA;
    }
    return NULL;
}




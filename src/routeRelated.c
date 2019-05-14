#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "routeRelated.h"
#include "structures.h"
#include "map.h"
#include "dijkstra.h"
#include "roadsRelated.h"
#include "additionalFunctions.h"
#include <string.h>

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

bool newRouteFromRouteParam( Map* map, RouteParam* routeParam, int lineNo) {


    char* city1 = routeParam->cities[0];
    if(!correctName(city1)){
        return false;
    }
    char* city2;

    int i = 1;
    while(i < routeParam->cFilled){

        city2 = routeParam->cities[i];

        if(!correctName(city2)){ ///< Któryś z parametrów ma niepoprawną wartość.
            return false;
        }
        if(strcmp(city1, city2) == 0){ ///< Podane nazwy miast są identyczne.
            return false;
        }

        city1 = city2;
        i++;
    }

    if(!correctId(routeParam->id)){
        return false;
    }

    if(map->routes[routeParam->id] != NULL){ ///< Droga krajowa o podanym Id juz istnieje.
        return false;
    }

//--------------
//  TO W PETLI WHILE
// TODO tu skonczylem
    Road* newRoad = createNewRoad();
    if(newRoad == NULL){
        return false;
    }

    City* cityA = findCityByName(map->cityList, city1);
    if(cityA == NULL){ ///< Create city if it is not yet in the structure.
        cityA = newCity(city1);
        if(cityA == NULL){
            return false;
        }

        if(!addCity(map, cityA)){
            return false;
        }
    }
    newRoad->cityA = cityA;

    City* cityB = findCityByName(map->cityList, city2);
    if(cityB == NULL){
        cityB = newCity(city2);
        if(cityB == NULL){
            return false;
        }

        if(!addCity(map, cityB)){
            return false;
        }
    }
    newRoad->cityB = cityB;

    newRoad->length = length;
    newRoad->year = builtYear;



    if(!addRoadToCity(cityA, cityA->roadList, newRoad)){ ///< Odcinek drogi między tymi miastami już istnieje.
        free(newRoad);
        return false;
    }
    addRoadToCity(cityB, cityB->roadList, newRoad);

//---------------------------


    Route* newRoute = createNewRoute(routeId);
    if(newRoute == NULL){
        return false;
    }

    addRouteInfoToRoads(newRoute); ///< Drogi maja informacje do jakich drog krajowych naleza.

    map->routes[newRoute->routeId] = newRoute;
}


//
// Created by krzubuntu on 27.04.19.
//

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

void addRouteInfoToRoads(Route* route){

    CityList* cityList = route->cityList;

    while(cityList != NULL){

        RoadList* roadList = cityList->city->roadList;

        while(roadList != NULL){

            roadList->road->routesBelonging[route->routeId] = route;
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

    while(tmp != NULL) {


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
    if(cityList == NULL){ // no route yet
        return false;
    }
    // invoked during extendRoute
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
    if(firstLength == secondLength){
        if(firstOldestRoadYear > secondOldestRoadYear) return 1;
        if(firstOldestRoadYear < secondOldestRoadYear) return 2;
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

    /*CityList* toDelete2 = otherEnd;*/

    path->next = otherEnd->next;
    otherEnd->next->prev = path;

    free(toDelete);
    free(otherEnd);
}

























//
// Created by krzubuntu on 27.04.19.
//

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "routeRelated.h"
#include "structures.h"
#include "map.h"
#include "dijkstra.h"

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

CityList* findShortestPath(Map* map, City* cityA, City* cityB){

    QueueElement* destination = Dijkstra(map, cityA, cityB);

    if(destination == NULL){ // no optimal path found
        return NULL;
    }

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

        tmp = tmp->predecessor;

    }
    //TODO free queue
    return cityList;


}

City* getOtherCity(Road *road, City *city){

    if(road->cityA != city) return road->cityA;
    return road->cityB;
}
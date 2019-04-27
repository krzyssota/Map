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


void addRouteInfoToRoads(Route* route){
    CityList* tmp = route->cityList;
    while(tmp->next != NULL){

    }
}

CityList* findShortestPath(Map* map, City* cityA, City* cityB){

    QueueElement* destination = Dijkstra(map, cityA, cityB);

    if(destination == NULL){ // no optimal route found
        return NULL;
    }

    QueueElement* tmp = destination;
    CityList* cityList = newCityList();

    while(tmp != NULL) {

        CityList* newCityListElement = newCityList();
        newCityListElement->city = tmp->predecessor->city;

        newCityListElement->next = cityList;
        cityList->prev = newCityListElement;

        tmp = tmp->predecessor;
        cityList = cityList->prev;
    }

    return cityList;


}

City* getOtherCity(Road *road, City *city){

    if(road->cityA != city) return road->cityA;
    return road->cityB;
}
//
// Created by krzubuntu on 27.04.19.
//

#include <stddef.h>
#include <stdint.h>
#include "routeRelated.h"
#include "structures.h"
#include "map.h"
#include "dijkstra.h"


CityList* findShortestPath(Map* map, City* cityA, City* cityB){

    QueueElement* destination = Dijkstra(map, cityA, cityB);

    /*CityList* cityList = NULL;
    while(destination != NULL) {
        CityList* newCityListElement = recoverPath(destination);
        cityList->next = newCityListElement;
        if()
    }*/


}

City* getOtherCity(Road *road, City *city){

    if(road->cityA != city) return road->cityA;
    return road->cityB;
}
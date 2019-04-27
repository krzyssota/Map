//
// Created by krzubuntu on 27.04.19.
//

#include <stddef.h>
#include "structures.h"
#include "additionalFunctions.h"
#include <stdlib.h>

RoadList* newRoadList(){
    RoadList* newRoadList = malloc(sizeof(RoadList));
    if(newRoadList != NULL) {
        newRoadList->prev = newRoadList->next = NULL;
        newRoadList->road = NULL;
    }
    return newRoadList;
}

City* newCity(const char *name){
    City* newCity = malloc(sizeof(City));
    if(newCity != NULL) {
        newCity->name = getName(name);
        newCity->roadList = NULL;
    }
    return newCity;
}

CityList* newCityList(){
    CityList* newCityList = malloc(sizeof(CityList));
    if(newCityList != NULL){
        newCityList->next = newCityList->prev = NULL;
        newCityList->city = NULL;
    }
    return newCityList;
}

Route* createNewRoute(unsigned id){
    Route* newRoute = malloc(sizeof(Route));
    if(newRoute != NULL){
        newRoute->cityList = NULL;
        newRoute->routeId = id;
    }
    return newRoute;
}

Queue* newQueue(City* destination){
    Queue* newQueue = malloc(sizeof(Queue));
    if(newQueue != NULL){
        newQueue->head = NULL;
        newQueue->destination = destination;
    }
    return newQueue;
}

QueueElement* newQueueElement(City* city, long int distance, QueueElement* predecessor, Road* road){
    QueueElement* newElement = malloc(sizeof(QueueElement));
    if(newElement != NULL){
        newElement->city = city;
        newElement->distance = distance;
        newElement->predecessor = predecessor;
        newElement->oldestRoad = road;
        newElement->prev = newElement->next = NULL;
    }
    return newElement;
}



























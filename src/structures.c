#include <stddef.h>
#include "structures.h"
#include "stringsHandling.h"
#include "map.h"
#include <stdlib.h>
#include <string.h>

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

Queue* newQueue(City* target){
    Queue* newQueue = malloc(sizeof(Queue));
    if(newQueue != NULL){
        newQueue->head = NULL;
        newQueue->target = target;
    }
    return newQueue;
}

QueueElement* newQueueElement(City* city, long int distance, QueueElement* predecessor, Road* oldestRoad){
    QueueElement* newElement = malloc(sizeof(QueueElement));
    if(newElement != NULL){
        newElement->city = city;
        newElement->distance = distance;
        newElement->predecessor = predecessor;
        newElement->oldestRoad = oldestRoad;
        newElement->prev = newElement->next = NULL;
        newElement->ambiguous = false;
    }
    return newElement;
}

ShortestPathResult* newShortestPathResult(){
    ShortestPathResult* structure = malloc(sizeof(struct ShortestPathResult));
    if(structure != NULL){
        structure->path = NULL;
        structure->length = 0;
        structure->oldestRoadYear = 0;
        structure->resultEnum = FOUND;
    }
    return structure;
}

Road* createNewRoad(){

    Road* newRoad = malloc(sizeof(Road));
    if(newRoad != NULL){

        newRoad->cityA = newRoad->cityB = NULL;
        newRoad->year = 0;
        newRoad->length = 0;
        /*newRoad->routesBelonging = malloc(sizeof(Route*) * 1000);
        if(newRoad->routesBelonging == NULL){
            return NULL;
        }*/
        for (int i = 0; i <= 999; ++i) {
            newRoad->routesBelonging[i] = NULL;
        }
    }
    return newRoad;
}

RouteParam* newRouteParam(unsigned id){

    RouteParam* routeParam = malloc(sizeof(RouteParam));

    if(routeParam != NULL) {

        routeParam->id = id;

        routeParam->cities = malloc(sizeof(char *) * 2);
        routeParam->cFilled = 0;
        routeParam->cSize = 2;

        routeParam->lengths = malloc(sizeof(unsigned));
        routeParam->lFilled = 0;
        routeParam->lSize = 1;

        routeParam->years = malloc(sizeof(int));
        routeParam->yFilled = 0;
        routeParam->ySize = 1;
    }
    return routeParam;

}

bool cityAlreadyInRouteParam(RouteParam* routeParam, char* cityName) {
    for(int i = 0; i < routeParam->cFilled; i++){
        if(strcmp(routeParam->cities[i], cityName) == 0){
            return true;
        }
    }
    return false;
}

bool addCityToRouteParam(RouteParam* routeParam, char* cityName) {

    if(routeParam->cFilled == routeParam->cSize){
        routeParam->cities = realloc(routeParam->cities, sizeof(char*) * (2*routeParam->cSize));
        if(routeParam->cities == NULL){
            return false;
        }
        routeParam->cSize *= 2;
    }
    routeParam->cities[routeParam->cFilled] = cityName;
    routeParam->cFilled++;
    return true;

}

bool addRoadToRouteParam(RouteParam* routeParam, unsigned roadLength, int year){

    if(routeParam->lFilled == routeParam->lSize){
        routeParam->lengths = realloc(routeParam->lengths, sizeof(char*) * (2*routeParam->lSize));
        if(routeParam->lengths == NULL){
            return false;
        }
        routeParam->lSize *= 2;
    }
    if(routeParam->yFilled == routeParam->ySize){
        routeParam->years = realloc(routeParam->years, sizeof(char*) * (2*routeParam->ySize));
        if(routeParam->years == NULL){
            return false;
        }
        routeParam->ySize *= 2;
    }
    routeParam->lengths[routeParam->lFilled] = roadLength;
    routeParam->lFilled++;

    routeParam->years[routeParam->yFilled] = year;
    routeParam->yFilled++;
    return true;
}


/*
StringList* newStringList(char* string){
    StringList* stringList = malloc(sizeof(StringList));
    if(stringList != NULL){
        stringList->string = string;
        stringList->prev = stringList->next = NULL;
    }
    return stringList;
}

void addStringList(Map* map, StringList* stringList){

    StringList* tmp = map->stringList;
    if(tmp == NULL){
        map->stringList = stringList;
    } else {
        while (tmp->next != NULL) {

            tmp = tmp->next;
        }
        tmp->next = stringList;
        stringList->prev = tmp;
    }

}*/










































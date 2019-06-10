#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "deleteStructure.h"
#include "structures.h"
#include "roadsRelated.h"
#include "dijkstra.h"

void deleteRoadListElement(City* city, RoadList* roadList){

    if(roadList->prev != NULL){ // not first element in the list

        (roadList->prev)->next = roadList->next;
        if(roadList->next != NULL){
            (roadList->next)->prev = roadList->prev;
        }

        free(roadList);

    } else {

        city->roadList = roadList->next;
        if(roadList->next != NULL) {
            (roadList->next)->prev = NULL;
        }

        free(roadList);
    }
}



void deleteRoadAndTwoRoadLists(Road *road){
    RoadList* roadListA = findRoadListElement(road->cityA->roadList, road);
    RoadList* roadListB = findRoadListElement(road->cityB->roadList, road);
    deleteRoadListElement(road->cityA, roadListA);
    deleteRoadListElement(road->cityB, roadListB);

    free(road);
}

void deleteRoadsList(RoadList* roadList){
    while(roadList != NULL){
        RoadList* tmp = roadList;
        roadList = roadList->next;
        deleteRoadAndTwoRoadLists(tmp->road);
        if(roadList != NULL) {
            roadList->prev = NULL;
        }
    }
}

void deleteCity(City* city){
    deleteRoadsList(city->roadList);
    free(city->name);
    free(city);
}

void deleteCitiesRoads(Map *map, CityList *cityList){
    while(cityList != NULL){
        deleteCity(cityList->city);
        CityList* tmp = cityList;
        cityList = cityList->next;
        if(cityList != NULL) {
            cityList->prev = NULL;
        }
        free(tmp);
    }
    map->cityList = NULL;
}

void cleanQueue(Queue **queue){
    while((*queue)->head != NULL){
        free(pop(queue));
    }
}

void deleteRoutes(Route **routeList){

    for (int i = 0; i <= 999 ; ++i) {

        if(routeList[i] != NULL){

            CityList* cityList = routeList[i]->cityList;

            while(cityList != NULL){

                CityList* tmp = cityList;
                cityList = cityList->next;
                free(tmp);
            }

            free(routeList[i]);
        }
    }
}

void deleteCityList(CityList* cityList){
    while(cityList != NULL) {
        CityList *tmp = cityList;
        cityList = cityList->next;
        free(tmp);
    }
}

void deleteRoute(Route* route){

    CityList* cityList = route->cityList;

    while(cityList != NULL){

        CityList* tmp = cityList;
        cityList = cityList->next;
        free(tmp);
    }

    free(route);
}

void deleteLine(char* line){
    if(line != NULL){
        free(line);
    }
}

void deleteRouteParam(RouteParam* routeParam){
    if(routeParam != NULL) {
        free(routeParam->cities);
        free(routeParam->years);
        free(routeParam->lengths);
        free(routeParam);
    }
}

void deleteShortestPathResult(ShortestPathResult* shortestPathResult){

    if(shortestPathResult != NULL) {
        CityList *curr = shortestPathResult->path;
        while (curr != NULL) {
            CityList *toDelete = curr;
            curr = curr->next;
            free(toDelete);
        }
        free(shortestPathResult);
    }
}





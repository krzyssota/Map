//
// Created by krzubuntu on 26.04.19.
//

#include <stddef.h>
#include <stdlib.h>
#include "deleteStructure.h"
#include "structures.h"
#include "roadsRelated.h"
#include "dijkstra.h"

void deleteRoadListElement(Map* map, City* city, RoadList* roadList){

    if(roadList->prev != NULL){ // not first element in the list

        (roadList->prev)->next = roadList->next;
        if(roadList->next != NULL){
            (roadList->next)->prev = roadList->prev;
        }

        free(roadList);

    } else {

        /*City* city = findCityByFirstRoadList(map, roadList);*/

        city->roadList = roadList->next;
        if(roadList->next != NULL) {
            (roadList->next)->prev = NULL;
        }

        free(roadList);
    }
}



void deleteRoadAndTwoRoadLists(Map *map, Road *road){
    RoadList* roadListA = findRoadListElement(road->cityA->roadList, road);
    RoadList* roadListB = findRoadListElement(road->cityB->roadList, road);
    deleteRoadListElement(map, road->cityA, roadListA);
    deleteRoadListElement(map, road->cityB, roadListB);

    /*free(road->routesBelonging);*/
    free(road);
}

void deleteRoadsList(Map* map, RoadList* roadList, City* city){
    while(roadList != NULL){
        RoadList* tmp = roadList;
        roadList = roadList->next;
        deleteRoadAndTwoRoadLists(map, tmp->road);
        if(roadList != NULL) {
            roadList->prev = NULL;
        }
    }
}

void deleteCity(Map* map, City* city){
    deleteRoadsList(map, city->roadList, city);
    free(city->name);
    free(city);
}

void deleteCitiesRoads(Map *map, CityList *cityList){
    while(cityList != NULL){
        deleteCity(map, cityList->city);
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
    (*queue)->head = NULL;
}

void deleteRouteList(Route* routeList[]){

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

    /*free(routeList);*/
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




//
// Created by krzubuntu on 26.04.19.
//

#include <stddef.h>
#include <stdlib.h>
#include "deleteStructure.h"
#include "structures.h"
#include "roadsRelated.h"

void deleteRoadListElement(Map* map, RoadList* roadList){
    if(roadList->prev != NULL){ // not first element in the list
        (roadList->prev)->next = roadList->next;
        if(roadList->next != NULL){
            (roadList->next)->prev = roadList->prev;
        }
        free(roadList);
    } else {
        City* city = findCityByFirstRoadList(map, roadList);
        city->roadList = roadList->next;
        if(roadList->next != NULL) {
            (roadList->next)->prev = NULL;
        }
        free(roadList);
    }
}



void deleteRoad(Map* map, Road* road){
    RoadList* roadListA = findRoadListElement(road->cityA->roadList, road);
    RoadList* roadListB = findRoadListElement(road->cityB->roadList, road);
    deleteRoadListElement(map, roadListA);
    deleteRoadListElement(map, roadListB);
    free(road);
}

void deleteRoadsList(Map* map, RoadList* roadList, City* city){
    while(roadList != NULL){
        deleteRoad(map, roadList->road);
        RoadList* tmp = roadList;
        roadList = roadList->next;
        free(tmp);
    }
}

void deleteCity(Map* map, City* city){
    deleteRoadsList(map, city->roadList, city);
    free(city->name);
    free(city);
}

void deleteCityList(Map* map, CityList* cityList){
    while(cityList != NULL){
        deleteCity(map, cityList->city);
        CityList* tmp = cityList;
        cityList = cityList->next;
        free(tmp);
    }
}
/*

#include <stddef.h>
#include <stdlib.h>
#include "deleteStructure.h"
#include "structures.h"
#include "roadsRelated.h"

void deleteRoadListElement(Map* map, RoadList* roadList){
    if(roadList->prev != NULL){ // not first element in the list
        (roadList->prev)->next = roadList->next;
        if(roadList->next != NULL){
            (roadList->next)->prev = roadList->prev;
        }
        free(roadList);
    } else {
        City* city = findCityByFirstRoadList(map, roadList);
        city->roadList = roadList->next;
        if(roadList->next != NULL) {
            (roadList->next)->prev = NULL;
        }
        free(roadList);
    }
}



void deleteRoad(Map* map, Road* road){
    RoadList* roadListA = findRoadListElement(road->cityA->roadList, road);
    RoadList* roadListB = findRoadListElement(road->cityB->roadList, road);
    deleteRoadListElement(map, roadListA);
    deleteRoadListElement(map, roadListB);
    free(road);
}

void deleteRoadsList(Map* map, RoadList* roadList){
    while(roadList != NULL){
        deleteRoad(map, roadList->road);
        RoadList* tmp = roadList;
        roadList = roadList->next;
        free(tmp);
    }
}

void deleteCity(Map* map, City* city){
    deleteRoadsList(map, city->roadList);
    free(city->name);
    free(city);
}

void deleteCityList(Map* map, CityList* cityList){
    while(cityList != NULL){
        deleteCity(map, cityList->city);
        CityList* tmp = cityList;
        cityList = cityList->next;
        free(tmp);
    }
}*/



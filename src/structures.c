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
        newCity->proximity = 0;
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
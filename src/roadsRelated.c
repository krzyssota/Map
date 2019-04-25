//
// Created by krzubuntu on 25.04.19.
//

#include "roadsRelated.h"

City* findCity(CityList* clist, const char* cityString) {

    while(clist != NULL && strcmp(clist->city->name, cityString) != 0){
        clist = clist->next;
    }
    if(clist == NULL){
        return NULL;
    }
    return clist->city;
}

RoadList* newRoadList(){
    RoadList* newRoadList = malloc(sizeof(RoadList));
    newRoadList->prev = newRoadList->next = NULL;
    return newRoadList;
}

bool roadToList(RoadList* rlist, Road* newRoad){

    while(rlist->next != NULL && rlist->road != newRoad) rlist = rlist->next;

    if(rlist->road == newRoad) return false;

    rlist->next = newRoadList();
    (rlist->next)->prev = rlist;
    (rlist->next)->road = newRoad;

    return true;
}

City* newCity(const char *name){
    City* newCity = malloc(sizeof(City));
    newCity->name = getName(name);
    return newCity;
}

void addCity(Map* map, City* city){

    CityList* tmp = map->cityList;
    while(tmp->next != NULL){
        tmp = tmp->next;
    }

    tmp->next = malloc(sizeof(CityList));
    (tmp->next)->prev = tmp;
    (tmp->next)->next = NULL;
    (tmp->next)->city = city;
}

bool findRoad(RoadList* rlist, City* city, Road* rfound){

    while(rlist->next != NULL && rlist->road->cityB != city) rlist = rlist->next;

    rfound = rlist->road;

    if(rlist->road->cityB == city){
        return true;
    }
    return false;

}
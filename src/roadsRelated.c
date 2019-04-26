//
// Created by krzubuntu on 25.04.19.
//

#include "roadsRelated.h"
#include "map.h"
#include "map.h"
#include "structures.h"
#include "additionalFunctions.h"
#include <string.h>
#include "stdlib.h"
#include "stdbool.h"

// TODO miasto bez nazwy, citylist wskazuje na null miasto, niepoprawne prev w liscie
City* findCityByFirstRoadList(Map *map, RoadList *roadList){
    CityList* tmp = map->cityList;

    while(tmp != NULL && tmp->city->roadList != roadList){
        tmp = tmp->next;
    }
    if(tmp != NULL) {
        return tmp->city;
    }
    return NULL;
}

RoadList* findRoadListElement(RoadList *roadList, Road *road){

    while(roadList != NULL && roadList->road != road){
        roadList = roadList->next;
    }
    return roadList;

}

City* findCityByName(CityList *clist, const char *cityString) {

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
    if(newRoadList != NULL) {
        newRoadList->prev = newRoadList->next = NULL;
    }
    return newRoadList;
}

bool sameRoad(Road* roadA, Road* roadB){
    if((roadA->cityA == roadB->cityA && roadA->cityB == roadB->cityB)
        || (roadA->cityA == roadB->cityB && roadA->cityB == roadB->cityA)){
        return true;
    }
    return false;
}

bool addRoadToCity(City *city, Road *newRoad){
    RoadList* rlist = city->roadList;
    if(rlist == NULL){ // city has got no roads yet
        rlist = newRoadList();
        if(rlist == NULL){
            return false;
        }
        city->roadList = rlist;
        city->roadList->road = newRoad;
        return true;

    } else {
        while (rlist->next != NULL && !sameRoad(rlist->road, newRoad)){
            rlist = rlist->next;
        }

        if (sameRoad(rlist->road, newRoad)){
            return false;
        }

        rlist->next = newRoadList();
        if (rlist->next == NULL){
            return false; // nieudana alokacja
        }

        (rlist->next)->prev = rlist;
        (rlist->next)->road = newRoad;

        return true;
    }
}

City* newCity(const char *name){
    City* newCity = malloc(sizeof(City));
    if(newCity != NULL) {
        newCity->name = getName(name);
    }
    return newCity;
}
CityList* newCityList(){
    CityList* newCityList = malloc(sizeof(newCityList));
    if(newCityList == NULL){
        return newCityList;
    }
    newCityList->next = newCityList->prev = NULL;
    newCityList->city = NULL;
    return newCityList;
}

bool addCity(Map* map, City* city){

    CityList* tmp = map->cityList;
    if(tmp == NULL){
        tmp = map->cityList = newCityList();
        if(tmp == NULL){
            return false;
        }
    }
    if(tmp->city == NULL){
        tmp->city = city;
        return true;
    }
    while(tmp->next != NULL){
        tmp = tmp->next;
    }

    tmp->next = newCityList();
    if(tmp->next == NULL){
        return false;
    }
    (tmp->next)->prev = tmp;
    (tmp->next)->city = city;
    return true;
}

Road* findRoad(RoadList* rlist, City* city){

    while(rlist->next != NULL && (rlist->road->cityA != city && rlist->road->cityB != city)){
        rlist = rlist->next;
    }

    if(rlist->road->cityA == city || rlist->road->cityB == city){
        return rlist->road;
    }
    return NULL;

}
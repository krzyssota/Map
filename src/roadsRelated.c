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


bool sameRoad(Road* roadA, Road* roadB){
    if((roadA->cityA == roadB->cityA && roadA->cityB == roadB->cityB)
        || (roadA->cityA == roadB->cityB && roadA->cityB == roadB->cityA)){
        return true;
    }
    return false;
}

bool addRoadToCity(City *city, RoadList* rlist, Road *newRoad){

    /*RoadList* rlist = city->roadList;*/

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




bool addCity(Map* map, City* city){

    CityList* tmp = map->cityList;
    if(tmp == NULL){ // adding first city to the structure
        tmp = map->cityList = newCityList();
        if(tmp == NULL){
            return false;
        }
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
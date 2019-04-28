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

/*CityList* findCityList(Map* map, City* city){

    CityList* tmp = map->cityList;
    while(tmp != NULL && tmp->city != city){
        tmp = tmp->next;
    }

    return tmp;


}*/

RoadList* findRoadListElement(RoadList *roadList, Road *road){

    while(roadList != NULL && roadList->road != road){
        roadList = roadList->next;
    }
    return roadList;

}

City* findCityByName(CityList *cityList, const char *cityName) {

    while(cityList != NULL && strcmp(cityList->city->name, cityName) != 0){
        cityList = cityList->next;
    }
    if(cityList == NULL){
        return NULL;
    }
    return cityList->city;
}


bool sameRoad(Road* roadA, Road* roadB){
    if((roadA->cityA == roadB->cityA && roadA->cityB == roadB->cityB)
        || (roadA->cityA == roadB->cityB && roadA->cityB == roadB->cityA)){
        return true;
    }
    return false;
}

bool addRoadToCity(City *city, RoadList* roadList, Road *newRoad){

    /*RoadList* roadList = city->roadList;*/

    if(roadList == NULL){ // city has got no roads yet
        roadList = newRoadList();
        if(roadList == NULL){
            return false;
        }
        city->roadList = roadList;
        city->roadList->road = newRoad;
        return true;

    } else {
        while (roadList->next != NULL && !sameRoad(roadList->road, newRoad)){
            roadList = roadList->next;
        }

        if (sameRoad(roadList->road, newRoad)){
            return false;
        }

        roadList->next = newRoadList();
        if (roadList->next == NULL){
            return false; // nieudana alokacja
        }

        (roadList->next)->prev = roadList;
        (roadList->next)->road = newRoad;

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

Road* findRoad(City* city1, City* city2){

    RoadList* roadList = city1->roadList;
    while(roadList != NULL && (roadList->road->cityA != city2 && roadList->road->cityB != city2)){
        roadList = roadList->next;
    }

    if(roadList != NULL && (roadList->road->cityA == city2 || roadList->road->cityB == city2)){
        return roadList->road;
    }
    return NULL;

}
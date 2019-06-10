/**
 * @file
 * Moduł udostępnia funcję wykorzystywane przy operacjach na drogach.
 * @author Krzysztof Sota
 * @date 18.05.2019
 * */
#include "roadsRelated.h"
#include "map.h"
#include "map.h"
#include "structures.h"
#include "stringsHandling.h"
#include <string.h>
#include "stdlib.h"
#include "stdbool.h"


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

/**@brief Sprawdza czy dwie drogi łączą te same miasta.
 * @param roadA  - wskaźnik na pierwszą drogę
 * @param roadB - wskaźnik na drugą drogę
 * @return Wartość @p true jeśli łączą te same miasta, @p false w przeciwnym wypadku.
 */
bool sameRoad(Road* roadA, Road* roadB){
    if((roadA->cityA == roadB->cityA && roadA->cityB == roadB->cityB)
        || (roadA->cityA == roadB->cityB && roadA->cityB == roadB->cityA)){
        return true;
    }
    return false;
}

bool addRoadToCity(City *city, RoadList* roadList, Road *newRoad){

    if(roadList == NULL){ ///< City has got no roads yet/
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
            return false;
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

Road* findRoad(City* cityA, City* cityB){
    if(cityA == NULL || cityB == NULL || cityA == cityB){
        return NULL;
    }
    RoadList* roadList = cityA->roadList;
    while(roadList != NULL && (roadList->road->cityA != cityB && roadList->road->cityB != cityB)){
        roadList = roadList->next;
    }

    if(roadList != NULL && (roadList->road->cityA == cityB || roadList->road->cityB == cityB)){
        return roadList->road;
    }
    return NULL;

}
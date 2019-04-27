//
// Created by krzubuntu on 25.04.19.
//

#ifndef DROGI_ROADSRELATED_H
#define DROGI_ROADSRELATED_H

#include "map.h"
#include "structures.h"
#include "stdlib.h"
#include "stdbool.h"

/*CityList* findCityList(Map* map, City* city);*/
/*City* findCityByFirstRoadList(Map* map, RoadList* roadList);*/
Road* olderRoad(Road* roadA, Road* roadB);
RoadList* findRoadListElement(RoadList *roadList, Road *road);
City* findCityByName(CityList *cityList, const char *cityName) ;
RoadList* newRoadList();
bool addRoadToCity(City *city,RoadList* rlist, Road *newRoad);
City* newCity(const char *name);
bool addCity(Map* map, City* city);
Road* findRoad(RoadList* rlist, City* city);

#endif //DROGI_ROADSRELATED_H

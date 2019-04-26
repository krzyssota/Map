//
// Created by krzubuntu on 25.04.19.
//

#ifndef DROGI_ROADSRELATED_H
#define DROGI_ROADSRELATED_H

#include "map.h"
#include "structures.h"
#include "stdlib.h"
#include "stdbool.h"

City* findCity(CityList* clist, const char* cityString) ;
RoadList* newRoadList();
bool addRoadToCity(City *city, Road *newRoad);
City* newCity(const char *name);
bool addCity(Map* map, City* city);
Road* findRoad(RoadList* rlist, City* city);

#endif //DROGI_ROADSRELATED_H

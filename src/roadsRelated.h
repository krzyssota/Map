//
// Created by krzubuntu on 25.04.19.
//

#ifndef DROGI_ROADSRELATED_H
#define DROGI_ROADSRELATED_H

#include "map.h"

City* findCity(CityList* clist, const char* cityString) ;
RoadList* newRoadList();
bool roadToList(RoadList* rlist, Road* newRoad);
City* newCity(const char *name);
void addCity(Map* map, City* city);
bool findRoad(RoadList* rlist, City* city, Road* rfound);

#endif //DROGI_ROADSRELATED_H

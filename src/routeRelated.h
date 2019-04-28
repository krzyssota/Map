//
// Created by krzubuntu on 27.04.19.
//

#include "structures.h"
#include "map.h"

#ifndef DROGI_ROUTERELATED_H
#define DROGI_ROUTERELATED_H

City* getOtherCity(Road *road, City *city);
CityList* findShortestPath(Map* map, Route* route, City* cityA, City* cityB, Road* oldestRoad);
void addRouteInfoToRoads(Route* Route);
bool routeContainsRoad(Route* route, Road* road);
unsigned calculateLength(CityList* path);

#endif //DROGI_ROUTERELATED_H

//
// Created by krzubuntu on 27.04.19.
//

#include "structures.h"
#include "map.h"

#ifndef DROGI_ROUTERELATED_H
#define DROGI_ROUTERELATED_H

City* getOtherCity(Road *road, City *city);
CityList* findShortestPath(Map* map, City* cityA, City* cityB);
void addRouteInfoToRoads(Route* Route);

#endif //DROGI_ROUTERELATED_H

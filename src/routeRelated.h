#include "structures.h"
#include "map.h"

#ifndef DROGI_ROUTERELATED_H
#define DROGI_ROUTERELATED_H

City *getOtherCity(Road *road, City *city);

CityList *findShortestPath(Map *map, Route *routeA, Route *routeB, City *cityA, City *cityB, int *oldestRoadYear, Road* roadRemoved);

void addRouteInfoToRoads(Route *Route);

bool routeContainsRoad(Route *route, Road *road);

unsigned calculateLength(CityList *path);

int betterPath(int firstOldestRoadYear, unsigned firstLength, int secondOldestRoadYear, unsigned secondLength);

void insertPathIntoRoute(CityList* path, Route* route, City* cityA, City* cityB);

#endif //DROGI_ROUTERELATED_H

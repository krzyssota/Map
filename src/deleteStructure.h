//
// Created by krzubuntu on 26.04.19.
//

#ifndef DROGI_DELETESTRUCTURE_H
#define DROGI_DELETESTRUCTURE_H

#include "structures.h"
#include "map.h"


void deleteCitiesRoads(Map *map, CityList *cityList);
void cleanQueue(Queue **queue);
void deleteRouteList(Route* routeList[]);
void deleteCityList(CityList* cityList);
void deleteRoute(Route* route);




#endif //DROGI_DELETESTRUCTURE_H

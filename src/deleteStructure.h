#ifndef DROGI_DELETESTRUCTURE_H
#define DROGI_DELETESTRUCTURE_H

#include "structures.h"
#include "map.h"


void deleteCitiesRoads(Map *map, CityList *cityList);
void cleanQueue(Queue **queue);
void deleteRouteList(Route* routeList[]);
void deleteCityList(CityList* cityList);
void deleteRoute(Route* route);
void deleteRoadAndTwoRoadLists(Road *road);




#endif //DROGI_DELETESTRUCTURE_H

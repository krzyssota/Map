#ifndef DROGI_ROADSRELATED_H
#define DROGI_ROADSRELATED_H

#include "map.h"
#include "structures.h"
#include "stdlib.h"
#include "stdbool.h"

Road* olderRoad(Road* roadA, Road* roadB);
RoadList* findRoadListElement(RoadList *roadList, Road *road);
City* findCityByName(CityList *cityList, const char *cityName) ;
/*RoadList* newRoadList();*/
bool addRoadToCity(City *city,RoadList* roadList, Road *newRoad);
/*City* newCity(const char *name);*/
bool addCity(Map* map, City* city);
Road* findRoad(City* city1, City* city2);
bool routeContainsCity(Route* route, City* city);

#endif //DROGI_ROADSRELATED_H

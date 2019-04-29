#ifndef DROGI_DIJKSTRA_H
#define DROGI_DIJKSTRA_H


#include "structures.h"
#include "map.h"

QueueElement* Dijkstra(Map* map, Route* routeA, Route* routeB, City* cityA, City* cityB, Road* roadRemoved);
QueueElement* pop(Queue** queue);

#endif //DROGI_DIJKSTRA_H

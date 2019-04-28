//
// Created by krzubuntu on 27.04.19.
//

#ifndef DROGI_DIJKSTRA_H
#define DROGI_DIJKSTRA_H


#include "structures.h"
#include "map.h"

QueueElement* Dijkstra(Map* map, Route* route, City* cityA, City* cityB);
QueueElement* pop(Queue** queue);

#endif //DROGI_DIJKSTRA_H

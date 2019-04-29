
#ifndef DROGI_STRUCTURES_H
#define DROGI_STRUCTURES_H

#include <stdint.h>
#include "map.h"
#include <stdint.h>

#define INF UINT32_MAX

typedef struct City City; // kazde miasto ma liste drog
typedef struct Road Road; // miastA miastoB
typedef struct CityList CityList; // lista miast
typedef struct RouteList RouteList; // kazda route ma liste miast
typedef struct RoadList RoadList;
typedef struct Route Route;

typedef struct Route{

    CityList* cityList;
    unsigned routeId;

} Route;


typedef struct RoadList{

    RoadList* prev;
    RoadList* next;
    Road* road;

} RoadList;

struct Road{

    City* cityA;
    City* cityB;

    unsigned length;
    int year;

    Route* routesBelonging[1000];
};

typedef struct CityList{

    struct CityList* next;
    struct CityList* prev;
    City* city;

} CityList;

struct City{

    char* name;
    RoadList* roadList;

};

typedef struct QueueElement{

    City* city;
    long int distance;
    struct QueueElement* predecessor;
    Road* oldestRoad;
    struct QueueElement* next;
    struct QueueElement* prev;

} QueueElement;

typedef struct Queue{

    QueueElement* head;
    City* destination;

} Queue;

RoadList* newRoadList();
City* newCity(const char *name);
CityList* newCityList();
Route* createNewRoute(unsigned id);
Queue* newQueue(City* destination);
QueueElement* newQueueElement(City* city, long int distance, QueueElement* predecessor, Road* road);
Road* createNewRoad();


#endif //DROGI_STRUCTURES_H

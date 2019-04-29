
#ifndef DROGI_STRUCTURES_H
#define DROGI_STRUCTURES_H

#include <stdint.h>
#include "map.h"
#include <stdint.h>

#define INF UINT32_MAX

typedef struct City City;
typedef struct Road Road;
typedef struct CityList CityList;
typedef struct RouteList RouteList;
typedef struct RoadList RoadList;
typedef struct Route Route;
/**
 * Droga krajowa zawiera wskaznik liste miast.
 */
typedef struct Route{

    CityList* cityList;
    unsigned routeId;

} Route;

typedef struct RoadList{

    RoadList* prev;
    RoadList* next;
    Road* road;

} RoadList;
/**
 * Droga zawiera wskaznik na miast, ktore laczy, swoja dlugosc, rok budowy/remontu i drogi krajowe, ktorych jest czescia
 */
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

/**
 * Miasto ma wskaznik nadane imie i wskaznik na liste drog, ktore z niego wychodza.
 */
struct City{

    char* name;
    RoadList* roadList;

};

/**
 * Element kolejki ma wskaznik na miasto, dystans od zrodla, wskaznik na poprzednika,
 * wskaznik na najstarsza droge po drodze od zrodla i wskaznik na kolejny i poprzedni element.
 */
typedef struct QueueElement{

    City* city;
    long int distance;
    struct QueueElement* predecessor;
    Road* oldestRoad;
    struct QueueElement* next;
    struct QueueElement* prev;
    bool ambiguous;

} QueueElement;

/** Kolejka priorytetowa.
 */
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

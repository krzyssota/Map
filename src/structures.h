
#ifndef DROGI_STRUCTURES_H
#define DROGI_STRUCTURES_H

typedef struct City City; // kazde miasto ma liste drog
typedef struct Road Road; // miastA miastoB
typedef struct CityList CityList; // lista miast
typedef struct RouteList RouteList; // kazda route ma liste miast
typedef struct RoadList RoadList;

typedef struct Route{


    CityList* cities;
    unsigned routeId;
    RoadList* roads;

} Route;

typedef struct RouteList{

    RouteList* prevRoute;
    RouteList* nextRoute;
    Route* route;

} RouteList;


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

    RouteList routeList;
};

typedef struct CityList{

    struct CityList* next;
    struct CityList* prev;
    City* city;

} CityList;

struct City{

    char* name;
    RoadList* roadList;
    int proximity;

};

RoadList* newRoadList();
City* newCity(const char *name);
CityList* newCityList();


#endif //DROGI_STRUCTURES_H

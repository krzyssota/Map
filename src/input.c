#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "map.h"
#include <string.h>
#include "additionalFunctions.h"


enum Command {ADD_ROUTE, ADD_ROAD, REPAIR_ROAD, DESCRIBE_ROUTE, ERROR};
/*
 numer drogi krajowej;nazwa miasta;długość odcinka drogi;rok budowy lub ostatniego remontu;nazwa miasta

 Format polecenia jest taki sam, jak w wyniku funkcji getRouteDescription.
 To polecenie tworzy drogę krajową o podanym numerze i przebiegu.
 Jeśli jakieś miasto lub odcinek drogi nie istnieje, to go tworzy.
 Jeśli odcinek drogi już istnieje, ale ma wcześniejszy rok budowy lub ostatniego remontu, to modyfikuje ten atrybut odcinka drogi.
 Za błąd uznajemy, jeśli odcinek drogi już istnieje, ale ma inną długość albo późniejszy rok budowy lub ostatniego remontu.
 To polecenie niczego nie wypisuje na standardowe wyjście.


addRoad;city1;city2;length;builtYear
        Wywołuje na mapie dróg funkcję addRoad z podanymi parametrami. Niczego nie wypisuje na standardowe wyjście.


repairRoad;city1;city2;repairYear
        Wywołuje na mapie dróg funkcję repairRoad z podanymi parametrami. Niczego nie wypisuje na standardowe wyjście.


getRouteDescription;routeId
        Wywołuje na mapie dróg funkcję getRouteDescription z podanym parametrem.
        Jeśli wynik działania tej funkcji jest inny niż NULL, to wypisuje na standardowe wyjście jedną linię z wynikiem działania tej funkcji.


 Każde polecenie znajduje się w osobnej linii.
 Puste linie i linie zaczynające się znakiem # należy ignorować.
 W poleceniach nazwa miasta jest niepustym napisem niezawierającym kodów od 0 do 31 ani średnika,
 liczby są zapisywane przy podstawie 10. Spacje w nazwie miasta są istotne.
*/

bool putZeroes(char *line, size_t lineLength){

    if(line[lineLength - 1] == ';'){
        return false;
    }
    int i = 0;
    int ch = line[i];

    while (ch != '\n' && ch != EOF){

        if(ch == ';'){
            line[i] = 0;
        }

        i++;
        ch = line[i];
    }
    if(ch == '\n'){
        line[i] = 0;
        return true;
    }
    return false;
}
enum Command getCommand(char* line){

    char* command = line;

    if(strcmp(command, "addRoute") == 0) return ADD_ROUTE;
    if(strcmp(command, "addRoad") == 0) return ADD_ROAD;
    if(strcmp(command, "repairRoad") == 0) return REPAIR_ROAD;
    if(strcmp(command, "getRouteDescription") == 0) return DESCRIBE_ROUTE;
    return ERROR;

}
bool readId(char** line, size_t* lineLength, unsigned* id, int lineNo){

    char* end = NULL;

    *id = (int)strtol(*line, &end, 10);
    if(*end != 0){
        handleError(lineNo);
        return false;
    }

    size_t digitsInId = digitsInNumber(*id);

    *lineLength -= (digitsInId+1);
    *line = &(*line)[digitsInId + 1];

    return true;
}
bool readCity(char** line, size_t* lineLength, char** city){

    *city = *line;

    *lineLength -= (strlen(*city)+1);
    *line = &(*line)[strlen(*city) + 1];
}

bool readRoadLength(char** line, size_t* lineLength, unsigned* roadLength, int lineNo){

    char* end = NULL;

    *roadLength = (int)strtol(*line, &end, 10);
    if(*end != 0){
        handleError(lineNo);
        return false;
    }

    size_t digitsInLength = digitsInNumber(*roadLength);

    *lineLength -= (digitsInLength + 1);
    *line = &(*line)[digitsInLength + 1];
}

bool readRoadYear(char** line, size_t* lineLength, int* year, int lineNo){

    char* end = NULL;

    *year = (int)strtol(*line, &end, 10);
    if(*end != 0){
        handleError(lineNo);
        return false;
    }

    size_t digitsInYear= digitsInNumber(*year);

    *lineLength -= (digitsInYear + 1);
    *line = &(*line)[digitsInYear + 1];
}

void getParametersAndAddRoute(char* line, size_t lineLength, Map* map, int lineNo){

    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    unsigned id;
    if(!readId(&line, &lineLength, &id, lineNo) || (lineLength == 0)){
        handleError(lineNo);
        return;
    }

    char* firstCity;
    if(!readCity(&line, &lineLength, &firstCity) || (lineLength == 0)){
        handleError(lineNo);
        return;
    }

    RouteParam* routeParam = newRouteParam(id);
    if(!addCityToRouteParam(routeParam, firstCity)){
        exit(0);
    }

    while(line > 0){

        unsigned roadLength;
        if(!readRoadLength(&line, &lineLength, &roadLength, lineNo) ||  lineLength == 0){
            handleError(lineNo);
            return;
        }

        int year;
        if(!readRoadYear(&line, &lineLength, &year, lineNo) ||  lineLength == 0){
            handleError(lineNo);
            return;
        }

        if (!addRoadToRouteParam(routeParam, roadLength, year)){
            exit(0);
        }

        char* city;
        if(!readCity(&line, &lineLength, &city)){
            handleError(lineNo);
            return;
        }

        if(!addCityToRouteParam(routeParam, city)){
            exit(0);
        }
    }

    if(!newRouteFromRouteParam(map, routeParam, lineNo) || lineLength != 0){
        //TODO tu skonczylem
    }

}
void getParametersAndAddRoad(char* line, size_t lineLength, Map* map, int lineNo){

    if(lineLength == 0){
        handleError(lineNo);
    }

    char* cityA;
    if(!readCity(&line, &lineLength, &cityA) || (lineLength == 0)){
        handleError(lineNo);
        return;
    }

    char* cityB;
    if(!readCity(&line, &lineLength, &cityB) || (lineLength == 0)){
        handleError(lineNo);
        return;
    }

    unsigned roadLength;
    if(!readRoadLength(&line, &lineLength, &roadLength, lineNo) ||  lineLength == 0){
        handleError(lineNo);
        return;
    }

    int year;
    if(!readRoadYear(&line, &lineLength, &year, lineNo) ||  lineLength != 0){
        handleError(lineNo);
        return;
    }

    if(!addRoad(map, (const char*) cityA, (const char*) cityB, roadLength, year)){
        handleError(lineNo);
    }
}
void getParametersAndRepairRoad(char* line, size_t lineLength, Map* map, int lineNo){

    if(lineLength == 0){
        handleError(lineNo);
    }

    char* cityA;
    if(!readCity(&line, &lineLength, &cityA) || (lineLength == 0)){
        handleError(lineNo);
        return;
    }

    char* cityB;
    if(!readCity(&line, &lineLength, &cityB) || (lineLength == 0)){
        handleError(lineNo);
        return;
    }

    int repairYear;
    if(!readRoadYear(&line, &lineLength, &repairYear, lineNo) ||  lineLength != 0){
        handleError(lineNo);
        return;
    }

    if(!repairRoad(map, (const char*) cityA, (const char*) cityB, repairYear)){
        handleError(lineNo);
    }
}
void getParametersAndGetRouteDescription(char* line, size_t lineLength, Map* map, int lineNo){

    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    unsigned id;
    if(!readId(&line, &lineLength, &id, lineNo) || lineLength != 0){
        handleError(lineNo);
        return;
    }

    const char* description = getRouteDescription(map, id);

    if(description != NULL){
        printf("%s", description);

    } else {
        handleError(lineNo);
    }
}

void executeCommand(enum Command command, char* line, size_t lineLength, Map* map, int lineNo){
    switch(command){
        case ADD_ROUTE:
            getParametersAndAddRoute(&line[strlen(line) + 1], lineLength-(strlen(line) + 1), map, lineNo);
            break;
        case ADD_ROAD:
            getParametersAndAddRoad(&line[strlen(line) + 1], lineLength-(strlen(line) + 1), map, lineNo);
            break;
        case REPAIR_ROAD:
            getParametersAndRepairRoad(&line[strlen(line) + 1], lineLength-(strlen(line) + 1), map, lineNo);
            break;
        case DESCRIBE_ROUTE:
            getParametersAndGetRouteDescription(&line[strlen(line) + 1], lineLength-(strlen(line) + 1), map, lineNo);
            break;
        default:
            exit(1);
    }
}

void readInput(Map* map) {

    int lineNo = 0;
    // podac line jako argument, w main struktura currentLine ktora wskazuja na obecna linijke i ją podaje do deleteMao
    char *line = NULL;
    ssize_t lineLength = 0;
    size_t dummy;

    do {
        lineLength = getline(&line, &dummy, stdin);
        lineNo++;
        map->inputLine = line;

        if(line[0] == '#' || line[0] == '\n'){
            continue;
        }

        if(!putZeroes(line, lineLength)){
            handleError(lineNo);
            continue;
        }

        enum Command command = getCommand(line);

        if(command == ERROR){
            handleError(lineNo);
            continue;
        }

        executeCommand(command, line, lineLength--, map, lineNo);

        char* tmp = line;
        map->inputLine = line = NULL;
        free(tmp);


    } while (lineLength  != -1);
}




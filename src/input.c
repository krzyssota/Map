#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "map.h"
#include <string.h>
#include "additionalFunctions.h"
#include "routeRelated.h"
#include "deleteStructure.h"


enum Command {ADD_ROUTE, ADD_ROAD, REPAIR_ROAD, DESCRIBE_ROUTE, IGNORE, ERROR};
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



void readCity(char** line, size_t* lineLength, char** city){

    *city = *line;

    *lineLength -= strlen(*city);
    if(*lineLength != 0)  *lineLength -= 1;

    *line = &(*line)[strlen(*city) + 1];
}

bool readPositiveNumber(char **line, size_t *lineLength, unsigned *number, int lineNo){

    char* end = NULL;

    *number = (int)strtol(*line, &end, 10); // strtol reads only positive numbers
    if(*end != 0){
        return false;
    }

    size_t digits = digitsInNumber(*number);

    *lineLength -= digits;
    if(*lineLength != 0)  *lineLength -= 1;
    *line = &(*line)[digits + 1];
    return true;
}

bool readNumber(char **line, size_t *lineLength, int *year, int lineNo){

    char* end = NULL;
    
    bool negative = false;
    if(**line == '-'){
        negative = true;
        (*line)++;
    }

    *year = (int)strtol(*line, &end, 10);
    if(*end != 0){
        return false;
    }
    

    
    size_t digitsInYear= digitsInNumber(*year);

    if(negative){
        *year *= -1;
        digitsInYear++;
    }

    *lineLength -= digitsInYear;
    if(*lineLength != 0)  *lineLength -= 1;

    if(negative){
        *line = &(*line)[(digitsInYear-1) + 1];
    } else {
        *line = &(*line)[digitsInYear + 1];
    }

    return true;
}



void getParametersAndAddRoute(char* line, size_t lineLength, Map* map, int lineNo){

    if(lineLength == 0){
        handleError(lineNo);
        return;
    }


    unsigned id;
    if(!readPositiveNumber(&line, &lineLength, &id, lineNo) || !correctId(id) || (lineLength == 0)){
        handleError(lineNo);
        return;
    }

    char* firstCity;
    readCity(&line, &lineLength, &firstCity);
    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    RouteParam* routeParam = newRouteParam(id);
    if(!addCityToRouteParam(routeParam, firstCity)){
        deleteRouteParam(routeParam);
        exit(0);
    }

    while(lineLength > 0){

        unsigned roadLength;
        if(!readPositiveNumber(&line, &lineLength, &roadLength, lineNo) || roadLength == 0 || lineLength == 0){
            handleError(lineNo);
            deleteRouteParam(routeParam);
            return;
        }

        int year;
        if(!readNumber(&line, &lineLength, &year, lineNo) || year == 0 || lineLength == 0){
            handleError(lineNo);
            deleteRouteParam(routeParam);
            return;
        }

        if (!addRoadToRouteParam(routeParam, roadLength, year)){
            deleteRouteParam(routeParam);
            exit(0);
        }

        char* city;
        readCity(&line, &lineLength, &city);

        if(cityAlreadyInRouteParam(routeParam, city)){
            handleError(lineNo);
            deleteRouteParam(routeParam);
            return;
        }


        if( !addCityToRouteParam(routeParam, city)){
            deleteRouteParam(routeParam);
            exit(0);
        }
    }

    if(!newRouteFromRouteParam(map, routeParam) || lineLength > 0){ // TODO nastapila zmiana z != >
        handleError(lineNo);
        deleteRouteParam(routeParam);
        return;
    }

    deleteRouteParam(routeParam);

}
void getParametersAndAddRoad(char* line, size_t lineLength, Map* map, int lineNo){

    if(lineLength == 0){
        handleError(lineNo);
    }

    char* cityA;
    readCity(&line, &lineLength, &cityA);
    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    char* cityB;
    readCity(&line, &lineLength, &cityB);
    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    unsigned roadLength;
    if(!readPositiveNumber(&line, &lineLength, &roadLength, lineNo) || roadLength == 0 ||  lineLength == 0){
        handleError(lineNo);
        return;
    }

    int year;
    if(!readNumber(&line, &lineLength, &year, lineNo) || year == 0 || lineLength > 0){
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
    readCity(&line, &lineLength, &cityA);
    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    char* cityB;
    readCity(&line, &lineLength, &cityB);
    if(lineLength == 0){
        handleError(lineNo);
        return;
    }

    int repairYear;
    if(!readNumber(&line, &lineLength, &repairYear, lineNo) || repairYear == 0 || lineLength > 0){
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
    if(!readPositiveNumber(&line, &lineLength, &id, lineNo) || !correctId(id) || lineLength > 0){
        handleError(lineNo);
        return;
    }

    char* description = (char*)getRouteDescription(map, id);

    if(description != NULL){
        printf("%s\n", description);
        free(description);
    } else {
        handleError(lineNo);
    }
}

void executeCommand(enum Command command, char* line, size_t lineLength, Map* map, int lineNo){
    switch(command){

        case ADD_ROUTE:
            getParametersAndAddRoute(line, lineLength, map, lineNo);
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

        case IGNORE:
            break;

        case ERROR:
            handleError(lineNo);
            break;

        default:
            exit(1);
    }
}

enum Command getCommand(char* line){

    if(line[0] == '#' || line[0] == '\n'){
        return IGNORE;
    }
    
    if(strcmp(line, "addRoad") == 0) return ADD_ROAD;
    if(strcmp(line, "repairRoad") == 0) return REPAIR_ROAD;
    if(strcmp(line, "getRouteDescription") == 0) return DESCRIBE_ROUTE;

    char* end;
    strtol(line, &end, 10);
    if(*end == 0){
        return ADD_ROUTE;
    }

    return ERROR;

}
bool segmentLine(char *line, size_t lineLength){

    if(line[lineLength - 1] == ';'){
        return false;
    }
    int i = 0;
    int ch = line[i];

    while (ch != '\n' && ch != EOF && lineLength > 0){

        if(ch == ';'){
            line[i] = 0;
        }

        i++;
        lineLength--;
        ch = line[i];
    }
    if(ch == '\n'){
        line[i] = 0;
        return true;
    }
    return false;
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

        if(lineLength == -1){
            map->inputLine = NULL;
            free(line);
            break;
        }

        if(line[0] != '\n' && !segmentLine(line, lineLength)){
            handleError(lineNo);
            continue;
        }

        enum Command command = getCommand(line);

        executeCommand(command, line, lineLength--, map, lineNo);

        char* tmp = line;
        map->inputLine = line = NULL;
        free(tmp);

    } while (true);
}




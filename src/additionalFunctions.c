#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "additionalFunctions.h"
#include "map.h"
#include "roadsRelated.h"
#include <math.h>

char* getName(const char *name){

    char *result = malloc(strlen(name)*sizeof(char) + 1);

    strcpy(result, name);

    return result;
}

bool correctId(unsigned id){
    return (1 <= id && id <= 999);
}

bool correctName(const char* cityName){

    uint32_t counter = 0;
    char* tmp = (char*) cityName;

    while(!(0 <= (int)(*tmp) && (int)(*tmp) <= 31) && (int)(*tmp) != 59){
        tmp++;
        counter++;
    }
    return (counter == strlen(cityName) && (int)(*tmp) == 0);

}

int countNumber(unsigned number){ ///< How many digits in the number.
    unsigned div = 1;
    unsigned digitCount = 1;

    while(div <= number / 10){
        digitCount++;
        div *= 10;
    }
    return digitCount;
}

int count(Route* route){ ///< How many characters description needs.

    int length = 0;
    length += countNumber((int)route->routeId);


    CityList* cityList = route->cityList;

    while(cityList->next != NULL){

        length += 1;
        length += strlen(cityList->city->name);

        length += 1;
        Road* road = findRoad(cityList->city, cityList->next->city);
        length += countNumber(road->length);

        length += 1;
        if(road->year < 0){
            length += 1;
        }
        length += countNumber(road->year);

        cityList = cityList->next;
    }
    length += 1;
    length += strlen(cityList->city->name);

    return length;
}

void writeNumber(int* i, char* str, unsigned number) { ///< Put integer in description.

    unsigned div = 1;
    unsigned digitCount = 1;

    while(div <= number / 10){
        digitCount++;
        div *= 10;
    }
    while(digitCount > 0){
        str[(*i)] = number/div + '0';
        (*i)++;
        number %= div;
        div /= 10;
        digitCount--;
    }
}

char* getDescription(Route* route, int length){

    char* str = malloc(sizeof(char) * length);
    if(str == NULL){
        return NULL;
    }

    int i = 0;

    writeNumber(&i, str, route->routeId);

    CityList* cityList = route->cityList;

    while(cityList->next != NULL){

        str[i] = ';';
        i++;
        for(unsigned j = 0; j < strlen(cityList->city->name); j++){
            str[i] = cityList->city->name[j];
            i++;
        }

        str[i] = ';';
        i++;
        Road* road = findRoad(cityList->city, cityList->next->city);
        writeNumber(&i, str, road->length);

        str[i] = ';';
        i++;
        int year = road->year;
        if(road->year < 0){
            str[i] = '-';
            i++;
            year = -year;
        }
        writeNumber(&i, str, year);



        cityList = cityList->next;
    }
    str[i] = ';';
    i++;
    for(unsigned j = 0; j < strlen(cityList->city->name); j++){
        str[i] = cityList->city->name[j];
        i++;
    }

    return str;
}



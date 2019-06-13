#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "stringsHandling.h"
#include "map.h"
#include "roadsRelated.h"
#include <math.h>
#include <values.h>


void handleError(int lineNo){
    fprintf(stderr, "ERROR %d\n", lineNo);
}
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
    return (counter == strlen(cityName) && (int)(*tmp) == 0 && strlen(cityName) != 0);

}

int digitsInNonNegativeNumber(unsigned number){
    unsigned div = 1;
    int digitCount = 1;

    while(div <= number / 10){
        digitCount++;
        div *= 10;
    }
    return digitCount;
}

int digitsInNegativeNumber(int number){
    int div = -1;
    int digitCount = 1;

    while(div >= number / 10){
        digitCount++;
        div *= 10;
    }
    return digitCount;
}

int count(Route* route){ ///< How many needed for description.

    int length = 0;
    length += digitsInNonNegativeNumber((int) route->routeId);


    CityList* cityList = route->cityList;

    while(cityList->next != NULL){

        length += 1;
        length += strlen(cityList->city->name);

        length += 1;
        Road* road = findRoad(cityList->city, cityList->next->city);
        length += digitsInNonNegativeNumber(road->length);

        length += 1;
        if(road->year < 0){
            length += 1;
            length += digitsInNegativeNumber(road->year);
        } else {
            length += digitsInNonNegativeNumber(road->year);
        }

        cityList = cityList->next;
    }
    length += 1;
    length += strlen(cityList->city->name);

    length += 1;

    return length;
}

void writeNonNegativeNumber(int* i, char* str, unsigned number) { ///< Put integer in description.

    unsigned div = 1;
    int digitCount = digitsInNonNegativeNumber(number);

    for(int j = 1; j < digitCount; j++){
        div *= 10;
    }
    while(digitCount > 0){
        str[(*i)] =  number/div + '0';
        (*i)++;
        number %= div;
        div /= 10;
        digitCount--;
    }
}

void writeNegativeNumber(int* i, char* str, int number) { ///< Put integer in description.

    unsigned div = 1;
    int digitCount = digitsInNegativeNumber(number);

    for(int j = 1; j < digitCount; j++){
        div *= 10;
    }

    str[(*i)] = '-';
    (*i)++;

    bool first = true;

    while(digitCount > 0){
        if(first && number == INT_MIN){
            str[(*i)] =  2 + '0';
            (*i)++;
            first = false;
        } else {
            str[(*i)] = (-number)/div + '0';
            (*i)++;
        }
        number = -((-number)%div);
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

    writeNonNegativeNumber(&i, str, route->routeId);

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
        writeNonNegativeNumber(&i, str, road->length);

        str[i] = ';';
        i++;
        int year = road->year;
        if(road->year < 0){
            writeNegativeNumber(&i, str, year);
        } else {
            writeNonNegativeNumber(&i, str, year);
        }

        cityList = cityList->next;

    }

    str[i] = ';';
    i++;

    for(unsigned j = 0; j < strlen(cityList->city->name); j++){
        str[i] = cityList->city->name[j];
        i++;
    }

    str[i] = 0;

    return str;
}



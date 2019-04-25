//
// Created by krzubuntu on 25.04.19.
//

#include "additionalFunctions.h"

char* getName(const char *name){
    char *result = NULL;
    strcpy(result, name);
    return result;
}

bool correctId(unsigned id){
    return (1 <= id && id <= 999);
}

bool correctName(const char* cityName){

    uint32_t counter = 0;
    char* tmp = (char*) cityName;

    while((int)(*tmp) > 31 && (int)(*tmp) != 59){
        tmp++;
        counter++;
    }
    return (counter == strlen(cityName) && (int)(*tmp) == 0);

}

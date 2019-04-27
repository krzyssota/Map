//
// Created by krzubuntu on 25.04.19.
//

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "additionalFunctions.h"

char* getName(const char *name){
    /*char *result = NULL;
    result =  malloc(sizeof(*result));*/
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

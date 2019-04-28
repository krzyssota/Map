#include "map.h"

#include <stdlib.h>
#include <string.h>

#undef NDEBUG

#include <assert.h>
#include <stdio.h>

int main() {
  char const* str;

  Map* m = newMap();
  assert(m);

  /*assert(addRoad(m, "Alinów", "Bór", 1, 2020));
  assert(addRoad(m, "Bór", "Cielińsk-Niekłańsk", 2, 2020));
  assert(addRoad(m, "Bór", "Dąb Stary", 3, 2020));
  assert(addRoad(m, "Cielińsk-Niekłańsk", "Emiliew", 4, 2020));
  assert(addRoad(m, "Dąb Stary", "Emiliew", 5, 2020));
  assert(addRoad(m, "Emiliew", "Bór", 8, 2020));
  assert(addRoad(m, "Emiliew", "Fraźnik Nowy", 3, 2020));
  assert(!repairRoad(m, "Emiliew", "Cielińsk-Niekłańsk", 2019));
  assert(repairRoad(m, "Emiliew", "Cielińsk-Niekłańsk", 2021));
  assert(!repairRoad(m, "Emiliew", "Alinów", 2020));
  assert(addRoad(m, "Fraźnik Nowy", "Grzegrzewo", 4, 2020));
  assert(addRoad(m, "Alinów", "Grzegrzewo", 10, 2020));
  assert(addRoad(m, "Homilcin", "Grzegrzewo", 5, 2020));
  assert(addRoad(m, "Fraźnik Nowy", "Cielińsk-Niekłańsk", 2, 2020));
  assert(!addRoad(m, "Fraźnik Nowy", "Cielińsk-Niekłańsk", 2, 2020));
  assert(!addRoad(m, "Cielińsk-Niekłańsk", "Fraźnik Nowy", 2, 2020));
  assert(!repairRoad(m, "Emiliew", "Bór", 2018));
  assert(repairRoad(m, "Emiliew", "Cielińsk-Niekłańsk", 2021));
  assert(repairRoad(m, "Emiliew", "Fraźnik Nowy", 2023));
  assert(addRoad(m, "Homilcin", "Cielińsk-Niekłańsk", 3, 2020));
  assert(newRoute(m, 10, "Alinów", "Emiliew"));*/

    addRoad(m, "A", "B", 1, 2021);
    addRoad(m, "A", "C", 5, 2025);

    addRoad(m, "B", "D", 7, 2027);
    addRoad(m, "C", "D", 2, 2022);

    newRoute(m, 1, "A", "D");

    addRoad(m, "A", "E", 8, 2028);
    addRoad(m, "E", "D", 9, 2029);

    extendRoute(m, 1, "E");

    addRoad(m, "E", "F", 10, 2010);
    addRoad(m, "F", "A", 11, 2011);

    removeRoad(m, "E", "A");

   /* addRoad(m, "A", "B", 1, 2021);
    addRoad(m, "B", "C", 2, 2022);
    addRoad(m, "C", "D", 7, 2027);
    addRoad(m, "D", "H", 7, 2027);

    addRoad(m, "B", "E", 2, 2025);
    addRoad(m, "E", "C", 3, 2026);

    addRoad(m, "B", "F", 1, 2025);
    addRoad(m, "F", "G", 1, 2026);
    addRoad(m, "G", "C", 1, 2026);

    newRoute(m, 1, "A", "H");
    removeRoad(m, "B", "C");*/

    /*str = getRouteDescription(m, 10);
    assert(strcmp(str, "10;Alinów;1;2020;Bór;2;2020;Cielińsk-Niekłańsk;4;2021;Emiliew") == 0);
    free((void *)str);*/

    /*assert(extendRoute(m, 10, "Homilcin"));*/

    /*str = getRouteDescription(m, 10);
    assert(strcmp(str, "10;Alinów;1;2020;Bór;2;2020;Cielińsk-Niekłańsk;4;2021;Emiliew"
                       ";3;2023;Fraźnik Nowy;4;2020;Grzegrzewo;5;2020;Homilcin") == 0);
    free((void *)str);*/



    /*addRoad(m, "First", "Second", 2, 2020);
    addRoad(m, "Second", "Third", 3, 2020);
    repairRoad(m, "First", "Second", 2021);
    repairRoad(m, "Second", "Third", 2023);*/
//

  deleteMap(m);

  return 0;
}

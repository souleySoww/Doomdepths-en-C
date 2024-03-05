#ifndef INVENTORY_H
#define INVENTORY_H

#include "Weapon.h"
#include "Armor.h"

typedef struct {
    Weapon weapons[5];
    int num_weapons;
    Armor armor;
} Inventory;

#endif 

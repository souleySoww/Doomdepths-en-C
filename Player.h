#ifndef PLAYER_H
#define PLAYER_H
#define MAX_SPELLS 5

#include "Inventory.h"
#include "Monster.h"
#include "Weapon.h"
#include "Armor.h"
#include "Spell.h"

typedef struct {
    int health;
    int num_attacks;
    int defense;
    int mana;
    int num_spells;
    Armor armor;
    Inventory inventory;
    Spell spells[MAX_SPELLS];
    Weapon weapon;
}Player;
// Prototypes des fonctions
void rewardPlayerWithAWeapon(Player *player, Weapon *weapon);
void rewardPlayerWithAnArmor(Player *player, Monster *monster);
void displayInventory(Player *player);

#endif 
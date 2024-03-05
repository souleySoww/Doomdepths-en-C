#include "Player.h"
#include <stdio.h>

void rewardPlayerWithAWeapon(Player *player, Weapon *weapon) {
    if (player->inventory.num_weapons < 5) {
        player->inventory.weapons[player->inventory.num_weapons] = *weapon;
        player->inventory.num_weapons++;
    } else {
        printf("L'inventaire d'armes est plein !!!\n");
    }
}

void rewardPlayerWithAnArmor(Player *player, Monster *monster) {
    int add_armor = monster->defense / 4;
    player->inventory.armor.defense += add_armor;
}

void displayInventory(Player *player) {
    printf("Inventaire du joueur : \n");
    printf("Arme : \n");
    for (int i = 0; i < player->inventory.num_weapons; i++) {
        printf("Arme : %d  Attack %d - %d \n", i + 1, player->inventory.weapons[i].min_attack, player->inventory.weapons[i].max_attack);
    }

    printf("Armure : Defense : %d \n", player->inventory.armor.defense);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Inventory.h"
#include "Monster.h"
#include "Player.h"
#include "Spell.h"

void displayHealthBar(int current, int max, char colorCode[]) {
    int barLength = 20;
    int filledLength = (int)((double)current / max * barLength);

    printf("[%s", colorCode);
    for (int i = 0; i < barLength; ++i) {
        if (i < filledLength) {
            printf("#");
        } else {
            printf("-");
        }
    }
    printf("\033[0m] %d/%d\n", current, max);
}
// Fonction pour afficher la barre de mana avec couleur
void displayManaBar(int current, int max) {
    int barLength = 20;
    int filledLength = (int)((double)current / max * barLength);

    printf("[");
    for (int i = 0; i < barLength; ++i) {
        if (i < filledLength) {
            printf("\033[1;34m#");  // Couleur bleue pour la barre de mana
        } else {
            printf("-");
        }
    }
    printf("\033[0m] %d/%d\n", current, max);
}

int main() {
    Player player;
    player.health = 100;
    player.num_attacks = 4;
    player.defense = 50;
    player.mana = 100;
    player.inventory.weapons[0].min_attack = 15;
    player.inventory.weapons[0].max_attack = 30;
    player.inventory.num_weapons = 1;
    player.inventory.armor.defense = 0;
    Spell fireBall;
    strcpy(fireBall.name, "fireBall");
    fireBall.mana_cost = 50;
    fireBall.mana_damage = 25;
    player.spells[0] = fireBall;
    player.num_spells = 1;

    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));
    int num_monsters = rand() % 3 + 3;  // Génération du nombre aléatoire de monstres (entre 3 et 5)
    Monster monsters[num_monsters];  // Tableau pour stocker les informations des monstres générés

    // Génération des statistiques pour chaque monstre
    for (int i = 0; i < num_monsters; i++) {
        monsters[i].life = rand() % 71 + 50;  // Vie entre 50 et 150
        monsters[i].min_attack = rand() % 6 + 10;  // Attaque minimale entre 10 et 30
        monsters[i].max_attack = rand() % 11 + 15;  // Attaque maximale entre 20 et 50
        monsters[i].defense = rand() % 11 + 5;    // Défense entre 5 et 15
    }

    int allMonstersDefeated = 0;

    while (player.health > 0 && !allMonstersDefeated) {
        int remaining_attacks = player.num_attacks;
        int defeated_monsters = 0;

        while (remaining_attacks > 0) {
            // Tour du joueur
            printf("\n");
            printf("###########################\n");
            printf("Tour du joueur\n");
            printf("\n");

            // Affichez les monstres et leurs statistiques
            for (int i = 0; i < num_monsters; i++) {
                if (monsters[i].life > 0) {
                    printf("\n");
                    printf("Monstre %d (Vie: %d, Attaque: %d-%d, Défense: %d)\n", i + 1, monsters[i].life, monsters[i].min_attack, monsters[i].max_attack, monsters[i].defense);
                    displayHealthBar(monsters[i].life, 150, "\033[1;31m");  // Couleur rouge pour la barre de vie du monstre
                    printf("\033[0m");
                }
            }

            // Affichez la barre de vie du joueur en vert
            printf("\n");
            printf("Joueur (Vie: %d, Défense: %d)\n", player.health, player.defense);
            displayHealthBar(player.health, 100, "\033[1;32m");  // Couleur verte pour la barre de vie du joueur
            printf("\033[0m");

            // Affichez la barre de mana du joueur en bleu
            printf("Mana du joueur (Mana: %d)\n", player.mana);
            displayManaBar(player.mana, 100);

            // Demandez au joueur de choisir un monstre
            int choice;
            int spell_choice;
            do {
                printf("\n");
                printf("Choisissez un monstre à attaquer (1 à %d) : ", num_monsters);
                scanf("%d", &choice);
                printf("\n");
            } while (choice < 1 || choice > num_monsters);

            choice--;  // Pour correspondre à l'index du tableau (0-indexed)

            // Les dégâts infligés au monstre choisi
            int damage_to_monster = rand() % (player.inventory.weapons[0].max_attack - player.inventory.weapons[0].min_attack + 1) + player.inventory.weapons[0].min_attack;
            int actual_damage = damage_to_monster - monsters[choice].defense;

            if (actual_damage > 0) {
                monsters[choice].life -= actual_damage;

                if (monsters[choice].defense < 0) {
                    monsters[choice].life += monsters[choice].defense;
                    monsters[choice].defense = 0;
                }

                printf("Le joueur inflige %d dégâts au monstre %d\n", damage_to_monster, choice + 1);
                printf("\n");
                printf("La vie du monstre %d après l'attaque du joueur est de : %d\n", choice + 1, monsters[choice].life);
                displayHealthBar(monsters[choice].life, 150, "\033[1;31m");  // Couleur rouge pour la barre de vie du monstre
                printf("\033[0m");

                if (monsters[choice].life <= 0) {
                    printf("Le monstre %d a été vaincu !\n", choice + 1);
                    defeated_monsters++;
                    player.defense += 10;
                    Monster new_monster;
                    new_monster.life = rand() % 76 + 50;
                    new_monster.defense = rand() % 26 + 25;
                    new_monster.min_attack = rand() % 6 + 15;
                    new_monster.max_attack = rand() % 16 + 25;
                    monsters[choice] = new_monster;

                    printf("\n");
                    printf("Un nouveau monstre vient d'apparaître HHHOOOOOOOO!!!!!\n");
                    printf("Vie : %d,   attaque : %d - %d,   Défense : %d \n", monsters[choice].life, monsters[choice].min_attack, monsters[choice].max_attack, monsters[choice].defense);
                    displayHealthBar(monsters[choice].life, 150, "\033[1;31m");  // Couleur rouge pour la barre de vie du monstre
                    printf("\033[0m");

                    Weapon new_weapon;
                    new_weapon.min_attack = rand() % 6 + 20;
                    new_weapon.max_attack = rand() % 11 + 25;

                    // Fonction pour récompenser le joueur avec une nouvelle arme
                    rewardPlayerWithAWeapon(&player, &new_weapon);
                    printf("Le joueur a obtenu une nouvelle arme ! Attaque : %d-%d\n", new_weapon.min_attack, new_weapon.max_attack);

                    // Fonction pour récompenser le joueur avec une nouvelle armure
                    rewardPlayerWithAnArmor(&player, &new_monster);
                    printf("\n");
                    printf("Le joueur a obtenu une nouvelle armure ! Défense : %d \n", new_monster.defense / 4);
                    printf("\n");

                    // Fonction pour afficher l'inventaire du joueur
                    displayInventory(&player);
                    printf("\n");

                    // Demandez au joueur de choisir une arme
                    int weapon_choice;
                    printf("Vous disposez de %d armes dans votre inventaire, veuillez choisir une arme !!! \n", player.inventory.num_weapons);
                    scanf("%d", &weapon_choice);
                    printf("\n");
                    printf("Arme %d équipée !!!!", weapon_choice);
                    damage_to_monster = rand() % (player.inventory.weapons[weapon_choice - 1].max_attack - player.inventory.weapons[weapon_choice - 1].min_attack + 1) + player.inventory.weapons[weapon_choice - 1].min_attack;
                    printf("\n");
                }
            } else {
                printf("Le monstre %d a esquivé l'attaque !\n", choice + 1);
                displayHealthBar(monsters[choice].life, 150, "\033[1;31m");  // Couleur rouge pour la barre de vie du monstre
                printf("\033[0m");
                printf("\n");
            }

            // Demandez au joueur de choisir un sort
            printf("Choisissez un sort à lancer de : 1 à %d \n", player.num_spells);
            scanf("%d", &spell_choice);
            printf("\n");

            if (spell_choice >= 1 && spell_choice <= player.num_spells) {
                int selected_spell_index = spell_choice - 1; // Index du sort choisi

                if (player.mana >= player.spells[selected_spell_index].mana_cost) {
                    player.mana -= player.spells[selected_spell_index].mana_cost;

                    for (int i = 0; i < num_monsters; i++) {
                        if (monsters[i].life > 0) {
                            monsters[i].life -= player.spells[selected_spell_index].mana_damage;
                            printf("Le joueur lance le sort %s et inflige %d dégâts au monstre %d \n", player.spells[selected_spell_index].name, player.spells[selected_spell_index].mana_damage, i + 1);
                            displayHealthBar(monsters[i].life, 150, "\033[1;31m");  // Couleur rouge pour la barre de vie du monstre
                            printf("\033[0m");
                            printf("\n");
                        }
                    }
                } else {
                    printf("Vous n'avez pas assez de mana pour lancer ce sort.\n");
                }
            }

            remaining_attacks--;
        }

        // Tour des monstres
        printf("\n");
        printf("###########################\n");
        printf("Tour des monstres\n");
        printf("\n");

        allMonstersDefeated = 1;  // Supposons que tous les monstres sont vaincus

        for (int i = 0; i < num_monsters; i++) {
            if (monsters[i].life > 0) {
                allMonstersDefeated = 0;  // Il y a encore des monstres en vie

                // Calcul des dégâts des monstres sur le joueur
                int damage = rand() % (monsters[i].max_attack - monsters[i].min_attack + 1) + monsters[i].min_attack;

                if (player.defense > 0) {
                    if (damage <= player.inventory.armor.defense) {
                        player.inventory.armor.defense -= damage;
                        damage = 0;
                    } else {
                        damage -= player.inventory.armor.defense;
                    }

                    player.defense -= damage;

                    if (player.defense < 0) {
                        player.health += player.defense;
                        player.defense = 0;
                    }
                } else {
                    player.health -= damage;
                }

                printf("\n");
                printf("Le monstre %d inflige %d dégâts au joueur\n Point de Vie du joueur est de : %d.\n Défense restante du joueur : %d \n", i + 1, damage, player.health, player.defense);
                printf("\n");
            }
        }

        if (player.health <= 0) {
            printf("Le joueur est mort, Game Over!\n");
            break;
        }

        if (allMonstersDefeated) {
            printf("TOUS LES MONSTRES ONT ÉTÉ VAINCUS ! \nVOUS AVEZ GAGNÉ !!! \n");
            break;
        }

        // Réinitialisation du nombre d'attaques disponibles du joueur
        player.num_attacks = 4;
        player.mana = 100;
    }

    return 0;
}

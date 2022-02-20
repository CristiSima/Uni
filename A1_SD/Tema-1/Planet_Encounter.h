// Copyright 2021 Simache Cristian

#include "./CircularDoublyLinkedList.h"

#ifndef PLANET_ENCOUNTER_H_
#define PLANET_ENCOUNTER_H_

#define PLANET_NAME_LEN 51

typedef unsigned int uint;

typedef struct planet_t planet_t;
struct planet_t
{
	char name[PLANET_NAME_LEN];
	doubly_linked_list_t *shields;
	uint planet_destroyed;
};

#define planet_get_nth_shield(p,n) (((uint*)(dll_get_nth_node((p)->shields,\
									(n))->data))[0])

planet_t *planet_create(char *name,uint shield_count);
void planet_add(doubly_linked_list_t *list, uint place,
				char *name, uint shield_count);
void planet_remove(doubly_linked_list_t *list,uint planet_id);
void planet_free(planet_t *planet);

void planet_show(doubly_linked_list_t *list, uint planet_id);

void planet_shield_upgrade(doubly_linked_list_t *list, uint planet_id,
							uint shield_id, uint shield_val);
void planet_shield_expand(doubly_linked_list_t * list, uint planet_id,
							uint shield_val);
void planet_shield_remove(doubly_linked_list_t *list, uint planet_id,
							uint shield_id);

void planet_rotate_clk(doubly_linked_list_t *list, uint planet_id, uint units);
void planet_rotate_trig(doubly_linked_list_t *list, uint planet_id, uint units);

void collide(doubly_linked_list_t *list, uint planetA_id, uint planetB_id);
#endif // PLANET_ENCOUNTER_H_

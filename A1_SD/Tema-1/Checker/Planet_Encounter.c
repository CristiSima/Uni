// Copyright 2021 Simache Cristian

#include "./Planet_Encounter.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

planet_t *planet_create(char *name, uint shield_count)
{
	int s = 1;
	planet_t *planet = malloc(sizeof(planet_t));

	strncpy(planet-> name, name, PLANET_NAME_LEN);

	planet-> planet_destroyed = 0;
	planet-> shields = dll_create(sizeof(int));

	s = 1;
	for(uint i = 0; i < shield_count; i++)
		dll_add_nth_node(planet-> shields, 0, &s);

	printf("The planet %s has joined the galaxy.\n", name);
	return planet;
}

void planet_add(doubly_linked_list_t *planets, uint planet_id,
				char *name, uint shield_count)
{
	if (planet_id > planets-> size)
	{
		printf("Planet out of bounds!\n");
		return;
	}
	planet_t *planet = planet_create(name, shield_count);
	dll_add_nth_node(planets, planet_id, planet);
	free(planet);
}

void planet_show(doubly_linked_list_t *list, uint planet_id)
{
	if (planet_id >= list-> size)
	{
		printf("Planet out of bounds!\n");
		return;
	}
	dll_node_t *node = dll_get_nth_node(list, planet_id);
	planet_t *planet = node-> data;
	planet_t *planet_prev = node-> prev-> data;
	planet_t *planet_next = node-> next-> data;
	dll_node_t *aux;

	printf("NAME: %s\n", planet-> name);

	if (planet_prev == planet_next)
	{
		if (planet_prev == planet)
			printf("CLOSEST: none\n");
		else
			printf("CLOSEST: %s\n", planet_prev-> name);
	}
	else
	{
		printf("CLOSEST: %s and %s\n", planet_prev-> name, planet_next-> name);
	}

	printf("SHIELDS: ");
	aux = planet-> shields-> head;
	do
	{
		printf("%d ", ((int*)(aux-> data))[0]);
		aux = aux-> next;
	}while (aux!= planet-> shields-> head);
	printf("\n");

	printf("KILLED: %d\n", planet-> planet_destroyed);
}

void planet_remove(doubly_linked_list_t *list, uint planet_id)
{
	if (planet_id >= list-> size)
	{
		printf("Planet out of bounds!\n");
		return;
	}
	dll_node_t *node = dll_remove_nth_node(list, planet_id);
	printf("The planet %s has been eaten by the vortex.\n",
			((planet_t*)(node-> data))-> name);
	planet_free(node-> data);
	free(node);
}

void planet_free(planet_t *planet)
{
	dll_free(&planet-> shields);
	free(planet);
}

void planet_shield_upgrade(doubly_linked_list_t *list, uint planet_id,
							uint shield_id, uint shield_val)
{
	if (planet_id >= list-> size)
	{
		printf("Planet out of bounds!\n");
		return;
	}
	planet_t *planet = dll_get_nth_node(list, planet_id)-> data;
	if (shield_id >= planet-> shields-> size)
	{
		printf("Shield out of bounds!\n");
		return;
	}
	planet_get_nth_shield(planet, shield_id)+= shield_val;
}

void planet_shield_expand(doubly_linked_list_t * list, uint planet_id,
							uint shield_val)
{
	if (planet_id >= list-> size)
	{
		printf("Planet out of bounds!\n");
		return;
	}
	planet_t *planet = dll_get_nth_node(list, planet_id)-> data;
	dll_add_nth_node(planet-> shields, planet-> shields-> size, &shield_val);
}

void planet_shield_remove(doubly_linked_list_t *list, uint planet_id,
							uint shield_id)
{
	if (planet_id >= list-> size)
	{
		printf("Planet out of bounds!\n");
		return;
	}
	planet_t *planet = dll_get_nth_node(list, planet_id)-> data;
	if (shield_id >= planet-> shields-> size)
	{
		printf("Shield out of bounds!\n");
		return;
	}
	if (planet-> shields-> size == 4)
	{
		printf("A planet cannot have less than 4 shields!\n");
		return;
	}
	dll_node_t *node = dll_remove_nth_node(planet-> shields, shield_id);
	free(node-> data);
	free(node);
}

void planet_rotate_trig(doubly_linked_list_t *list, uint planet_id, uint units)
{
	if (planet_id >= list-> size)
	{
		printf("Planet out of bounds!\n");
		return;
	}
	planet_t *planet = dll_get_nth_node(list, planet_id)-> data;
	units%= planet-> shields-> size;

	for(uint i = 0; i < units; i++)
		planet-> shields-> head = planet-> shields-> head-> next;
}

void planet_rotate_clk(doubly_linked_list_t *list, uint planet_id, uint units)
{
	if (planet_id >= list-> size)
	{
		printf("Planet out of bounds!\n");
		return;
	}
	planet_t *planet = dll_get_nth_node(list, planet_id)-> data;
	units%= planet-> shields-> size;

	for(uint i = 0; i < units; i++)
		planet-> shields-> head = planet-> shields-> head-> prev;
}

void collide(doubly_linked_list_t *list, uint planetA_id, uint planetB_id)
{
	if (planetA_id >= list-> size || planetB_id >= list-> size)
	{
		printf("Planet out of bounds!\n");
		return;
	}
	planet_t *planetA = dll_get_nth_node(list, planetA_id)-> data;
	planet_t *planetB = dll_get_nth_node(list, planetB_id)-> data;

	if (planet_get_nth_shield(planetA, planetA-> shields-> size/4) == 0)
	{
		printf("The planet %s has imploded.\n", planetA-> name);

		if (planetB_id > planetA_id)
			planetB_id--;
		if (planet_get_nth_shield(planetB, (planetB-> shields-> size/4)*3)!= 0)
			planetB-> planet_destroyed++;

		planet_free(planetA);
		free(dll_remove_nth_node(list, planetA_id));

		planetA = NULL;
	}
	if (planet_get_nth_shield(planetB, (planetB-> shields-> size/4)*3) == 0)
	{
		printf("The planet %s has imploded.\n", planetB-> name);

		if (planetA!= NULL)
			planetA-> planet_destroyed++;

		planet_free(planetB);
		free(dll_remove_nth_node(list, planetB_id));

		planetB = NULL;
	}

	if (planetA!= NULL)
		planet_get_nth_shield(planetA, planetA-> shields-> size/4)--;
	if (planetB!= NULL)
		planet_get_nth_shield(planetB, (planetB-> shields-> size/4)*3)--;
}

void process_commands(doubly_linked_list_t *planets)
{
	char cmd[11], name[PLANET_NAME_LEN];
	uint planet_id, planet_id2, shield_id;
	uint shield_val, shield_count;
	uint M;
	scanf("%d", &M);

	for(uint i = 0; i < M; i++)
	{
		scanf("%s", cmd);
		if (!strcmp(cmd, "ADD"))
		{
			scanf("%s %d %d", name, &planet_id, &shield_count);
			planet_add(planets, planet_id, name, shield_count);
		}
		else if (!strcmp(cmd, "BLH"))
		{
			scanf("%d", &planet_id);
			planet_remove(planets, planet_id);
		}
		else if (!strcmp(cmd, "UPG"))
		{
			scanf("%d %d %d", &planet_id, &shield_id, &shield_val);
			planet_shield_upgrade(planets, planet_id, shield_id, shield_val);
		}
		else if (!strcmp(cmd, "EXP"))
		{
			scanf("%d %d", &planet_id, &shield_val);
			planet_shield_expand(planets, planet_id, shield_val);
		}
		else if (!strcmp(cmd, "RMV"))
		{
			scanf("%d %d", &planet_id, &shield_id);
			planet_shield_remove(planets, planet_id, shield_id);
		}
		else if (!strcmp(cmd, "COL"))
		{
			scanf("%d %d", &planet_id, &planet_id2);
			collide(planets, planet_id, planet_id2);
		}
		else if (!strcmp(cmd, "ROT"))
		{
			scanf("%d %c %d", &planet_id, name, &shield_val);
			if (name[0] == 'c')
				planet_rotate_clk(planets, planet_id, shield_val);
			else if (name[0] == 't')
				planet_rotate_trig(planets, planet_id, shield_val);
			else
				printf("Not a valid direction!\n");
		}
		else if (!strcmp(cmd, "SHW"))
		{
			scanf("%d", &planet_id);
			planet_show(planets, planet_id);
		}
		else
		{
		}
	}
}

int main()
{
	doubly_linked_list_t *planets = dll_create(sizeof(planet_t));
	dll_node_t *node;

	process_commands(planets);

	while(planets-> size!= 0)
	{
		node = dll_remove_nth_node(planets, 0);
		planet_free(node-> data);
		free(node);
	}
	free(planets);

	return 0;
}

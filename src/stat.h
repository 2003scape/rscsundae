#ifndef STAT_H
#define STAT_H
#include <stdbool.h>
#include "entity.h"

extern const char *skill_names[MAX_SKILL_ID];

void stat_calculate_table(void);
bool stat_is_combat(int);

void stat_advance(struct player *, int, int, int);

void stat_add(struct mob *, int, int, int);
void stat_remove(struct mob *, int, int, int);
void stat_heal(struct mob *, int, int, int);

bool stat_up(struct mob *mob, int);
bool stat_down(struct mob *mob, int);
bool stat_random(struct mob *, int, double, double);

#endif

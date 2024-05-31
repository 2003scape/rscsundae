#include "config.h"

bool
item_equip_clear(struct item_config *item, int equip_type)
{
	const int basic_types[] = {
		EQUIP_TYPE_SHIELD,
		EQUIP_TYPE_WEAPON,
		EQUIP_TYPE_HEAD,
		EQUIP_TYPE_TORSO,
		EQUIP_TYPE_NECK,
		EQUIP_TYPE_HANDS,
		EQUIP_TYPE_LEGS,
		EQUIP_TYPE_SHOES,
		EQUIP_TYPE_BACK,
		-1
	};

	for (int i = 0; basic_types[i] != -1; ++i) {
		if ((item->equip_type & basic_types[i]) != 0 &&
		    (equip_type & basic_types[i]) != 0) {
			return true;
		}
	}
	return false;
}

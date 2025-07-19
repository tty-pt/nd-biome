#include <nd/nd.h>

unsigned biome_map[BIOME_MAX], biome_n = 0;

unsigned biome_skel_add(char *name, enum color bg) {
	SKEL skel = {
		.type = TYPE_ROOM,
	};

	biome_skel_t biome_skel = {
		.bg = bg,
	};

	strlcpy((char *) skel.name, name, sizeof(skel.name));
	memcpy(skel.data, &biome_skel, sizeof(biome_skel));
	unsigned skid = nd_put(HD_SKEL, NULL, &skel);
	biome_map[biome_n] = skid;
	biome_n ++;
	return skid;
}

void mod_install(void *arg __attribute__((unused))) {
	biome_skel_add("water", BLUE);
	biome_skel_add("permanent ice", WHITE);
	biome_skel_add("tundra", CYAN);
	biome_skel_add("tundra2", CYAN);
	biome_skel_add("tundra3", CYAN);
	biome_skel_add("tundra4", CYAN);
	biome_skel_add("cold desert", CYAN);
	biome_skel_add("shrubland", GREEN);
	biome_skel_add("coniferous forest", GREEN);
	biome_skel_add("boreal forest", GREEN);
	biome_skel_add("temperate grassland", GREEN);
	biome_skel_add("woodland", GREEN);
	biome_skel_add("temperate seasonal forest", GREEN);
	biome_skel_add("temperate rainforest", GREEN);
	biome_skel_add("desert", YELLOW);
	biome_skel_add("savannah", YELLOW);
	biome_skel_add("tropical seasonal forest", GREEN);
	biome_skel_add("tropical rainforest", BLACK);
	biome_skel_add("volcanic", BLACK);

	unsigned ref = 16;
	nd_put(HD_BIOME, &ref, biome_map);
}

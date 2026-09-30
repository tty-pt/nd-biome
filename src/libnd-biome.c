/* main.c — nd-biome, ported to libxylem.
 *
 * Builds the room skeletons for the 19 biomes and publishes the map that maps
 * a biome index to its skeleton.
 *
 * Wave 1 (MODS.md §7): no module dependencies, and like nd-wts it implements
 * no events at all -- it is entirely a table, populated at load. Its only job
 * is proving that `mod_install`-shaped work survives the bus: 19 `HD_SKEL`
 * registrations plus one `HD_BIOME` one, with a `biome_skel_t` payload
 * memcpy'd into the skeleton's data words.
 *
 * `BIOME_MAX` indexes biome_map, so the map is a fixed-size engine-provided
 * array, not a module allocation.
 */

#include <string.h>

#include <ttypt/xy-mod.h>

#include <nd/xy.h>

unsigned biome_map[BIOME_MAX], biome_n = 0;

static unsigned
biome_skel_add(const char *name, enum color bg)
{
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
	biome_n++;
	return skid;
}

XY_MODULE_API void
xy_install(void)
{
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

	/* Biome 16 is where the world generator's biome ids start. */
	unsigned ref = 16;
	nd_put(HD_BIOME, &ref, biome_map);

	WARN("nd-biome: xy_install, %u biomes, HD_BIOME[16]\n", biome_n);
}

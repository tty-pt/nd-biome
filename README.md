# axil-nd-biome

`nd-biome` for [axil-nd](../axil-nd), ported from SIC to libxylem.

Builds the room skeletons for the 19 biomes and publishes the map that relates a
biome index to its skeleton. It implements no events at all — it is entirely a
table, populated at load. Its job is to prove that install-shaped work survives
the bus: 19 `HD_SKEL` registrations plus one `HD_BIOME`, with a `biome_skel_t`
payload `memcpy`'d into each skeleton's data words.

## Install

```sh
make install
```

Installs one file:

```
lib/libnd-biome.so
```

There is deliberately no `lib/nd-biome.so` symlink. `xy_load()` appends `.so`
itself and does not retry with a `lib` prefix, so the engine's `mods.load` names
this module `libnd-biome` and `dlopen`s `libnd-biome.so`. A soname symlink would
also have been silently dropped from the OpenBSD package: `tty-pt/ci` builds the
packing list from `find usr -type f`, which never lists a symlink, so the package
would have shipped the library under one name and asked the loader for another.

Also packaged for deb, apk, rpm, brew and openbsd from a `v*` tag.

It installs no header because it exports no API — every symbol it defines is
discovered by the engine, not called by another module.

## Build from source

```sh
make
```

Needs [libxylem](https://github.com/tty-pt/libxylem) (headers already in
`$(PREFIX)/include`) and the engine's game API, `<nd/xy.h>`, from either an
`axil-nd` checkout beside this repo or an installed `axil-nd`:

```sh
git clone https://github.com/tty-pt/nd-biome && cd nd-biome
git clone https://github.com/tty-pt/axil-nd ../axil-nd
make
```

`<nd/xy.h>` installs to `$(PREFIX)/include/nd/`, the same directory that already
carries `<ttypt/xy.h>`, so an **installed** engine needs no `-I` of its own here.
Against a checkout beside this repo it is `-I../axil-nd/include`; both paths are
on `CFLAGS` at once and a missing `-I` is ignored, so the same command works
either way.

## What it does

* `xy_install()` registers 19 `HD_SKEL` room skeletons, one per biome, each with
  a background colour and the `TYPE_ROOM` type.
* It fills `biome_map`, indexed by `BIOME_MAX`. That is a fixed-size,
  engine-provided array, not a module allocation.
* `nd_put(HD_BIOME, &ref, biome_map)` publishes the map under biome 16, which is
  where the world generator's biome ids start.

## Testing

There is no `test.sh` here. Behaviour is asserted by the engine's own suite,
which builds every module in its `mods.load`, boots, and greps stderr:

```sh
cd ../axil-nd
make && ./test.sh
```

nd-biome is not yet in the engine's `mods.load` (MODS.md §9 records it as
ported but neither loaded nor asserted), so no suite assertion covers it yet.
Its `xy_install` logs `nd-biome: xy_install, %u biomes, HD_BIOME[16]`, which is
the line such an assertion would grep for.

## Notes from the port

* **The include changed spelling.** It was `"papi/nd-xy.h"`, a file that no
  longer exists in any checkout: the engine moved its module-facing tree from
  `papi/` to `nd/`, and `papi/` now holds only `nd.h`. The module could not
  compile until this was fixed.
* `mod_install`/`mod_open` collapsed into `xy_install`. The original had both
  entry points doing the same table build.
* `BIOME_MAX` indexes `biome_map`, so the map is engine-sized rather than a
  module allocation — unchanged from the original.

## License

BSD 2-Clause, carried over from `tty-pt/nd-biome`. See `LICENSE`.

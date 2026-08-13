# C Labyrinth

> Mini labyrinth game in an old school style

---

## Project details

### Technologies

- C
- Cmake
- Valgrind
- Make

### Libraries

- Raylib 5.0
- Libyaml 0.2.5

---

## Assets managing

The assets are mapped using a specific YAML structure and are parsed differently based on the type (if it's a basic game asset like a bomb or if it's a character).

## Assets type

- 1 : Static image
- 2 : Image sequence for continuous animation
- 3 : Image sequence for animation triggered by an action

Every asset of the game (character, till, item etc.) have an associated type. This type will define the way to parse the image and the structure.

- For a static image represented by the `1`, the `path` is a simple string.
- For an image sequence (`2` or `3`), the `path` is a YAML flow sequence (an array of strings), each entry being one frame of the animation, played in the order they are listed.

Alongside `type` and `path`, every image configuration can also define:

- `description` : a free text description of the asset, mostly used for debugging/logging.
- `rotation` : a flip/rotation flag applied when the image is rendered.

### For characters

For characters, after defining every image assets of the every move of the character, an initial configuration file should be defined to map every action with their image sequence.

For exemple

```yaml
front-walk:
  type: 2
  path: [
    "/images/front-walk/1.png",
    "/images/front-walk/2.png",
    "/images/front-walk/3.png",
    "/images/front-walk/4.png",
    "/images/front-walk/5.png",
    "/images/front-walk/6.png",
    "/images/front-walk/7.png",
    "/images/front-walk/8.png",
    "/images/front-walk/9.png",
    "/images/front-walk/10.png",
    "/images/front-walk/11.png",
    "/images/front-walk/12.png",
    "/images/front-walk/13.png",
    "/images/front-walk/14.png"
  ]
  description: "Marche vers le bas"
  rotation: 1
right-walk:
  type: 2
  path: [
    "/images/right-walk/1.png",
    "/images/right-walk/2.png",
    "/images/right-walk/3.png",
    "/images/right-walk/4.png",
    "/images/right-walk/5.png",
    "/images/right-walk/6.png",
    "/images/right-walk/7.png",
    "/images/right-walk/8.png",
    "/images/right-walk/9.png",
    "/images/right-walk/10.png",
    "/images/right-walk/11.png",
    "/images/right-walk/12.png",
    "/images/right-walk/13.png",
    "/images/right-walk/14.png"
  ]
  description: "Marche vers la droite"
  rotation: 1
```

**The `type` key there, defines the sequence type. Described in one the previous section of this MARKDOWN**

The keys used to name each action (`front-walk`, `right-walk`, ...) are not free-form: they are matched against a fixed list of expected action names for the character type, and mapped to an index used internally to store the character's actions. A hero and an enemy don't share the same list of possible actions:

- A **hero** supports 14 actions: `back-animation`, `death-animation`, `front-animation`, `left-animation`, `right-animation`, `victory-animation`, `idle-front-animation`, `idle-back-animation`, `idle-left-animation`, `idle-right-animation`, `front-walk`, `right-walk`, `left-walk`, `back-walk`.
- An **enemy** only supports the 4 walking actions: `front-walk`, `right-walk`, `left-walk`, `back-walk`.

Any key that doesn't match one of these names is ignored while loading the configuration.

After that, you need to map in a second configuration file the list of the characters of this type, so each of them can be loaded from its own directory.

For example, to map all the game enemies, you have to define a configuration file which will list the enemies by mapping an id, a name and the path to the configuration file of this specific enemy

```yaml
1:
  path: "/first/config.yaml"
  name: "Hero 1"
```

The `path` here is relative to the directory containing this mapping file, and points to the character's own configuration file (the one mapping action names to image sequences, as described above).

---

### For basic assets

Basic assets (tills, items, ...) don't have actions: they are a plain `ImageConfig` (or a list of them), without the intermediate action-mapping step used for characters.

Some of them, like tills, are defined directly as a map of id to image configuration, in a single file:

```yaml
1:
  type: 1
  path: "/images/wallstone.png"
  description: "Pierre de mur"
  rotation: 1
2:
  type: 1
  path: "/images/wallstone-fence.png"
  description: "Pierre de clotûre"
  rotation: 1
```

Others, like items, follow the same two-file pattern as characters: a root configuration file maps an id to the path of the item's own configuration file, and that file directly holds the `type`/`path`/`description`/`rotation` of the item (no action names involved).

```yaml
# root config.yaml
1:
  path: "/bomb/config.yaml"
2:
  path: "/gem-1/config.yaml"
```

```yaml
# bomb/config.yaml
type: 1
path: "/images/bomb.png"
description: "Bombe"
rotation: 1
```

In both cases, the numeric id used as the YAML key is not read from inside the asset's own configuration file: it is assigned by the loader from the mapping key itself.

## Map rendering

### Configure a map file

A map is described by its own YAML file, independent from the assets configuration files described above. It is loaded through the generic `loadConfig(path, treatmentFunction)` helper, passing `loadGameMapConfig` as the treatment function, so any file following the format below can be loaded from any path — the game currently only wires up one map (`resources/home-config/map.yaml`, used for the home screen), but nothing in the loader is tied to that specific file.

```yaml
scale: 1
tills: [
  [[0,0,1],[0,0,1]],
  [[30,0,1],[0,0,1]]
]
items: [
  [30,0,1]
]
enemies: [
  [100,90,10]
]
```

A map file defines 4 keys:

- `scale` : a display scale factor applied to every position on the map.
- `tills` : a 2D grid (rows of columns) of tills covering the map's background. Each cell is a `[x, y, id]` triplet: `x`/`y` is the absolute position of that till, and `id` refers to an entry of the tills configuration (`resources/game-assets/tills/config.yaml`).
- `items` : a flat list of items placed freely on the map (not tied to the grid), each one also a `[x, y, id]` triplet, `id` referring to the items configuration.
- `enemies` : a flat list of enemies placed on the map, same `[x, y, id]` format, `id` referring to the enemies configuration.

**The `id` of a till, item or enemy is not validated when the map file is loaded** — it is only resolved against the corresponding configuration (tills/items/enemies) when the map is actually rendered. An `id` that doesn't exist in that configuration will only fail (or misbehave) at that point, not at load time.

### Rendering functions


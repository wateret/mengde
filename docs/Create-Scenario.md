# Create Your Own Scenario

This document is for the ones who want to make scenarios. Not for the game engine developers or players.

Anyone can write a scenario(MOD). If you know about [Lua](https://www.lua.org), the script language for mengde, and
details about how the game flow is like, you can write your own scenarios.

## What you need for creating scenario

- Game Configuration File(.lua)
- Game Scripts(.lua, 1 for each stage)
- Bitmap Resource for GUI

With these files, you can create your own scenario.

Let's take a look at the details for each.

### Scenario Directory Structure

```
sce
└── {example}                          # Scenario ID
    ├── gui                            # GUI resources
    │   ├── equipment                  # Equipment images
    │   │   ├── {heaven_sword}.bmp
    │   │   └── {short_sword}.bmp
    │   ├── magic                      # Magic sprites
    │   │   ├── {fire_0}.bmp
    │   │   ├── {fire_1}.bmp
    │   │   └── {heal_0}.bmp
    │   ├── map                        # Background map images
    │   │   ├── {stage_01}.bmp
    │   │   └── {stage_01}.bmp
    │   ├── model                      # Unit sprites
    │   │   ├── {CaoCao-1}
    │   │   │   ├── atk.bmp
    │   │   │   ├── mov.bmp
    │   │   │   └── spc.bmp
    │   │   └── {GuanYu}
    │   │       ├── atk.bmp
    │   │       ├── mov.bmp
    │   │       └── spc.bmp
    │   └── portrait                   # Hero portrait images
    │       ├── {CaoCao}.bmp
    │       ├── {CaoHong}.bmp
    │       └── {ZhangLiao}.bmp
    └── script                         # Global/Stage Scripts
        ├── {stage_01}.lua
        ├── {stage_02}.lua
        └── config.lua                 # Global config script
```

### Bitmap Resource

Bitmap files should be in this format. blah blah....

### Scenario Configuration File (config.lua)

The configuration file is also written in Lua. This is intended to have dependencies as few as possible. Did not think
worth adopting a data format like JSON, XML, or something.

First the file name must be `config.lua`. You must define a global variable `gconf` which is only things that is
required for this file. For more, like what should be in `gconf`, see *link to another document*.

In the configuration, there will be all information that is samely affected in common for any stages.

### Stage Scripts

#### The Game Flow

One must define these functions in the global scope of Lua.

1. `on_deploy(game)`
	- Called when the very beggining of the stage
	- Generate own units
1. `on_begin(game)`
	- Called right after the deployment
	- Generate allies and enemies
1. `end_condition(game)`
	- Called every time after an unit does its action
	- Must return an enum value - whether victory, defeat or undecided
1. `on_victory(game)`
	- Called when win the stage
	- Add actions for the units on victory
1. `on_defeat(game)`
	- Called when lose the stage
	- Add actions for the units on defeat

#### How to Write Script for Stage

Please refer to `game/sce/example` directory from the source tree for an example scenario.

##### Lua C API

You can call the Game API functions from your script.

List of API functions are TBD.

### Graphical Resources

There are 5 directories that contain graphical resources.

- equipment : Equipment icons
- magic : Magic sprites
- map : Map bitmap
- model : Unit sprites
- portrait : Hero portrait bitmap


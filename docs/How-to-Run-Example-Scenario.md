# Run Example Scenario

As the game is in its early stage of development, the developers who are new would not know what to do with this project.
We do not have much document even code are incomplete. Many features are still not fixed, they can be changed any time. So it is not worth writing much document yet.
For now, We would recommend running the "example" scenario which may be the easist way to get to know how everything works. The "example" scenario will always be compatible with the latest code and will be a reference for scenario developers.

## How to Run

After running `build.py`, we need directory `sce` in the directory that contains executable `game`.

For example,

```
$ cd build/Darwin.x86_64.Debug/game    # Or go to installed directory
$ mkdir sce && cd sce
$ cp -r game/sce/example .             # Or maybe create a symlink
```

And if you have bitmap resources, you can copy it under "example" directory.
Please refer to [Create Scenario](Create-Scenario.md) to learn the scenario directory structure.

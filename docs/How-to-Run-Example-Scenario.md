# Run Example/Demo Scenario

As the game is in its early stage of development, new developers who want to contribute on this project would not know what to do.
We do not have much document and code is incomplete. Many features/APIs are still not fixed, they may change any time. So it is not worth writing much document yet.
For now, we would recommend running the "example" or "demo" scenario which may be the easist way to get to know how everything works. Those scenarios will always be compatible with the latest code and is a reference for scenario developers.

## How to Run

After [build the project](Build.md), you need directory `sce` in the directory that contains executable `game`.

For example,

```
$ cd build/Darwin.x86_64.Debug/game    # Or go to installed directory
$ mkdir sce && cd sce
$ cp -r game/sce/example .             # Or maybe create a symlink
```

NOTE "example" scenario does not contain bitmap resource due to copyright issues, while "demo" scenario does contain it.

If you want to know more about scenario directory structure, refer to [Create Scenario](Create-Scenario.md).

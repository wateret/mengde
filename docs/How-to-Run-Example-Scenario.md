# Run Example/Demo Scenario

As the game is in its early stage of development, new developers who want to contribute on this project would not know what to do.
We do not have much document and code is incomplete. Many features/APIs are still not fixed, they may change any time. So it is not worth writing much document yet.
For now, we would recommend running the "example" or "demo" scenario which may be the easist way to get to know how everything works. Those scenarios will always be compatible with the latest code and is a reference for scenario developers.

## How to Run

After [building the project](Build.md), `sce` directory is also copied to install directory. You can run `tools/play.py` to play mengde.

For example,

```
$ tools/build.py # Binaries and resources(demo scenario are installed in `build/Darwin.x86_64.Debug/install`
$ tools/play.py  # This executes `build/Darwin.x86_64.Debug/install/game`
```

NOTE "example" scenario does not contain bitmap resource due to copyright issues, while "demo" scenario does contain it.

If you want to know more about scenario directory structure, refer to [Create Scenario](Create-Scenario.md).

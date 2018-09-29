# GUI

## Architecture Overview

```
----------------
|  App         | Application specific UI (Dependent on core module)
-----------    |
|  UIFW   |    | General UI framework
----------------
|  Foundation  | C++ SDL wrapper
----------------
|  SDL2        |
----------------
```

## Layers

### Foundation

C++ SDL wrapper. Most of them are for encapsulating C API to C++.

### UIFW

General UI Framework. This contains general components like ButtonView, TextView, ImageView, and so on. It also contains extensible infra like CompoisiteView.

### App

The UI components that is directly related to the application. It means that the most Views here are dependent to `core` module.

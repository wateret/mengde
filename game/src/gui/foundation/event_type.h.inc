// Event Map
//
// Map of SDL_EventType and EventType of our framework

#ifndef MACRO_EVENT_TYPE
#error  Define MACRO_EVENT_TYPE before including this file.
#endif

//               EventType    EventType(lowercase)
MACRO_EVENT_TYPE(Unsupported, unsupported)
MACRO_EVENT_TYPE(Quit,        quit)
// MACRO_EVENT_TYPE(Key,         key)
MACRO_EVENT_TYPE(MouseMotion, mouse_motion)
MACRO_EVENT_TYPE(MouseButton, mouse_button)
MACRO_EVENT_TYPE(MouseWheel,  mouse_wheel)

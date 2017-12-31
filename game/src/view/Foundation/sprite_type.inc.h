// SpriteType Macro

#ifndef MACRO_SPRITE_TYPE
#error  Define MACRO_SPRITE_TYPE before including this file.
#endif

//                enum     bitmap   has    sprite  # of      bitmap
//                name     name     dir    offset  sprites   size
MACRO_SPRITE_TYPE(None,    nullptr, true,  0,      0,         0)
MACRO_SPRITE_TYPE(Move,    "mov",   true,  0,      2,        48)
MACRO_SPRITE_TYPE(Stand,   "mov",   true,  6,      1,        48)
MACRO_SPRITE_TYPE(LowHP,   "mov",   false, 9,      2,        48)
MACRO_SPRITE_TYPE(Attack,  "atk",   true,  0,      4,        64)
MACRO_SPRITE_TYPE(Blocked, "spc",   true,  0,      1,        48)
MACRO_SPRITE_TYPE(Damaged, "spc",   false, 3,      1,        48)
MACRO_SPRITE_TYPE(Buff,    "spc",   false, 4,      1,        48)

#undef MACRO_SPRITE_TYPE


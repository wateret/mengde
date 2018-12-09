#ifndef MACRO_AI_MODE
#error  Define MACRO_AI_MODE before including this file.
#endif

MACRO_AI_MODE(None, none)
MACRO_AI_MODE(Default, default)
MACRO_AI_MODE(Random, random)
MACRO_AI_MODE(UnitInRangeRandom, unit_in_range_random)
MACRO_AI_MODE(UnitInRangePriority, unit_in_range_priority)
MACRO_AI_MODE(HoldPosition, hold_position)

#undef MACRO_AI_MODE

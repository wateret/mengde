gconfig = {
    id = "CaoCao",
    hero_classes = {
        {   id           = "Lord-1",
            attr_grades  = "AAAAA",
            attack_range = "Adjacent4",
            move         = 6,
            hp           = {100, 5},
            mp           = {30, 1}
        },
        {   id           = "Infantry-1",
            attr_grades  = "BASBB",
            attack_range = "Adjacent8",
            move         = 5,
            hp           = {110, 6},
            mp           = {10, 1}
        },
        {   id           = "Cavalry-1",
            attr_grades  = "SBABB",
            attack_range = "Adjacent4",
            move         = 6,
            hp           = {100, 5},
            mp           = {10, 1},
            promotion    = { id = "Cavalry-2", level = 20 }
        },
        {   id           = "Cavalry-2",
            attr_grades  = "SBABB",
            attack_range = "Adjacent8",
            move         = 7,
            hp           = {150, 5},
            mp           = {20, 1}
        },
        {   id           = "Archer-1",
            attr_grades  = "ABBBS",
            attack_range = "Distance2_8",
            move         = 5,
            hp           = {90, 4},
            mp           = {10, 1}
        },
        {   id           = "HorseArcher-1",
            attr_grades  = "SBBBA",
            attack_range = "Distance2_4",
            move         = 6,
            hp           = {100, 5},
            mp           = {10, 1}
        },
        {   id           = "Carriage-1",
            attr_grades  = "SBACA",
            attack_range = "Distance4",
            move         = 4,
            hp           = {90, 4},
            mp           = {10, 1}
        },
        {   id           = "Fighter-1",
            attr_grades  = "ACASB",
            attack_range = "Adjacent4",
            move         = 5,
            hp           = {90, 4},
            mp           = {20, 1}
        },
        {   id           = "Bandit-1",
            attr_grades  = "SCBBS",
            attack_range = "Adjacent4",
            move         = 5,
            hp           = {100, 5},
            mp           = {20, 1}
        },
        {   id           = "Strategist-1",
            attr_grades  = "BSBBB",
            attack_range = "Adjacent4",
            move         = 5,
            hp           = {90, 4},
            mp           = {40, 2}
        },
        {   id           = "Support-1",
            attr_grades  = "CSCAA",
            attack_range = "Adjacent4",
            move         = 5,
            hp           = {80, 3},
            mp           = {50, 2}
        },
        {   id           = "Guru-1",
            attr_grades  = "CSBAB",
            attack_range = "Adjacent4",
            move         = 5,
            hp           = {80, 3},
            mp           = {40, 2}
        },
        {   id           = "HorseStrategist-1",
            attr_grades  = "ASBBC",
            attack_range = "Adjacent4",
            move         = 5,
            hp           = {100, 5},
            mp           = {50, 2}
        },
        {   id           = "Dancer-1",
            attr_grades  = "ABBSB",
            attack_range = "Adjacent4",
            move         = 5,
            hp           = {90, 3},
            mp           = {35, 1}
        }
    },
    terrains = {
        {   id =   "Flatland",
            char = "f"
        },
        {   id =   "Grass",
            char = "g"
        },
        {   id =   "Forest",
            char = "F"
        },
        {   id =   "Wasteland",
            char = "w"
        },
        {   id =   "Mountain",
            char = "m"
        },
        {   id =   "RockyMountain",
            char = "r"
        },
        {   id =   "Wall",
            char = "W"
        }
    },
    terrain_movecost = {
        -- Rows : terrain / Cols : unit class
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {2, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2, 1},
        {2, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2, 1},
        {3, 2, 2, 2, 3, 3, 2, 2, 1, 2, 2, 2, 3, 2},
        {9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
        {9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9}
    },
    terrain_effect = {
        -- Rows : terrain / Cols : unit class
        {100, 100, 110, 110, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 110, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100}
    },
    magics = {
        {   id       = "fire_0",
            target   = "enemy",
            accuracy = "always",
            range    = "Distance4_Incl",
            mp       = 10,
            learnat  = {
                { class = "Lord-1",       level = 10 },
                { class = "Strategist-1", level = 5 }
            },
            effects = {
                {type  = "hp", power = -70}
            }
        },
        {   id       = "heal_0",
            target   = "ally",
            accuracy = "always",
            range    = "Distance4_Incl",
            mp       = 10,
            learnat  = {
                { class = "Support-1",    level = 5 },
                { class = "Strategist-1", level = 15 }
            },
            effects = {
                { type = "hp", power = 50 }
            }
        },
        {   id       = "buff_dex",
            target   = "ally",
            accuracy = "always",
            range    = "Distance3_Incl",
            mp       = 8,
            learnat  = {
                { class = "Support-1", level = 5 }
            },
            effects = {
                { type = "attr", attr = "dex", amount = 20, turns = 3 }
            }
        },
        {   id       = "stun",
            target   = "enemy",
            accuracy = "always",
            range    = "Distance4_Incl",
            mp       = 8,
            learnat  = {
                { class = "Strategist-1", level = 10 }
            },
            effects = {
                { type = "hp", power = -20 },
                { type = "condition", condition = "stunned", turns = 3 }
            }
        },
        {   id       = "root",
            target   = "enemy",
            accuracy = "always",
            range    = "Distance4_Incl",
            mp       = 8,
            learnat  = {
                { class = "Strategist-1", level = 10 }
            },
            effects = {
                { type = "hp", power = -30 },
                { type = "condition", condition = "rooted", turns = 3 }
            }
        },
    },
    heroes = {
        {   id    = "CaoCao",
            class = "Lord-1",
            attr  = {82, 99, 80, 90, 95},
        },
        {   id    = "DianWei",
            class = "Fighter-1",
            attr  = {97, 60, 99, 50, 90},
        },
        {   id    = "ZhangLiao",
            class = "Cavalry-1",
            attr  = {90, 92, 64, 80, 75},
        },
        {   id    = "ManChong",
            class = "Support-1",
            attr  = {20, 60, 50, 90, 50},
        },
        {   id    = "XiahouDun",
            class = "Cavalry-1",
            attr  = {94, 84, 72, 48, 95},
        },
        {   id    = "XunYu",
            class = "Strategist-1",
            attr  = {34, 75, 77, 98, 64},
            model = "Strategist-1-red"
        },
        {   id    = "Bandit",
            class = "Bandit-1",
            attr  = {70, 70, 50, 30, 60},
        },
        {   id    = "Cavalry",
            class = "Cavalry-1",
            attr  = {70, 70, 55, 30, 50},
        },
        {   id    = "LuBu",
            class = "Cavalry-1",
            attr  = {100, 98, 90, 15, 99},
        }
    },
    equipments = {
        {   id          = "short_sword",
            type        = "weapon",
            description = "short_sword_desc",
            equipable   = "all",
            effects     = {
                { event = "action_done", effect = "restore_hp", multiplier = 15 },
                { event = "on_normal_attack", effect = "critical_attack" },
                { event = "on_normal_attack", effect = "reflect_attack", multiplier = 20 }
            },
            modifiers   = {
                { attr = "atk", addend = 20 },
                { attr = "def", addend =  5 }
            }
        },
        {   id          = "heaven_sword",
            type        = "weapon",
            description = "heaven_sword_desc",
            equipable   = "all",
            effects     = {
                { event = "turn_begin",         effect = "restore_hp", multiplier = 15 },
                { event = "on_normal_attacked", effect = "preemptive_attack" },
                { event = "on_normal_attack",   effect = "enhance_basic_attack", multiplier = 5 },
                { event = "on_normal_attack",   effect = "double_attack" },
                { event = "on_normal_attack",   effect = "counter_counter_attack", multiplier = 10 }
            },
            modifiers   = {
                { attr = "atk", addend = 40 }
            }
        }
    },
    consumables = { -- Not used yet. NYI
        {   id          = "recovery_bean",
            usable      = "all",
            effects     = {
                { event = "action_done", effect = "restore_hp", multiplier = 15 }
            },
            modifiers   = {
                { attr = "atk", addend = 20, turn = 3 },
                { attr = "def", addend =  5, turn = 3 }
            }
        }
    },
    stages = { "01" }
}

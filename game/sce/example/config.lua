gconfig = {
    id = "CaoCao",
    unit_classes = {
        {   id           = "Lord",
            promotions   = 3,
            stat_grades  = "AAAAA",
            attack_range = "Adjacent4",
            move         = 6,
            hp           = {100, 5},
            mp           = {30, 1}
        },
        {   id           = "Infantry",
            promotions   = 3,
            stat_grades  = "BASBB",
            attack_range = "Adjacent8",
            move         = 5,
            hp           = {110, 6},
            mp           = {10, 1}
        },
        {   id           = "Cavalry",
            promotions   = 3,
            stat_grades  = "SBABB",
            attack_range = "Adjacent4",
            move         = 6,
            hp           = {100, 5},
            mp           = {10, 1}
        },
        {   id           = "Archer",
            promotions   = 3,
            stat_grades  = "ABBBS",
            attack_range = "Distance2_8",
            move         = 5,
            hp           = {90, 4},
            mp           = {10, 1}
        },
        {   id           = "HorseArcher",
            promotions   = 3,
            stat_grades  = "SBBBA",
            attack_range = "Distance2_4",
            move         = 6,
            hp           = {100, 5},
            mp           = {10, 1}
        },
        {   id           = "Carriage",
            promotions   = 3,
            stat_grades  = "SBACA",
            attack_range = "Distance4",
            move         = 4,
            hp           = {90, 4},
            mp           = {10, 1}
        },
        {   id           = "Fighter",
            promotions   = 3,
            stat_grades  = "ACASB",
            attack_range = "Adjacent4",
            move         = 5,
            hp           = {90, 4},
            mp           = {20, 1}
        },
        {   id =          "Bandit",
            promotions =  3,
            stat_grades =  "SCBBS",
            attack_range = "Adjacent4",
            move =        5,
            hp =          {100, 5},
            mp =          {20, 1}
        },
        {   id =          "Strategist",
            promotions =  3,
            stat_grades =  "BSBBB",
            attack_range = "Adjacent4",
            move =        5,
            hp =          {90, 4},
            mp =          {40, 2}
        },
        {   id =          "Support",
            promotions =  3,
            stat_grades =  "CSCAA",
            attack_range = "Adjacent4",
            move =        5,
            hp =          {80, 3},
            mp =          {50, 2}
        },
        {   id =          "Guru",
            promotions =  3,
            stat_grades =  "CSBAB",
            attack_range = "Adjacent4",
            move =        5,
            hp =          {80, 3},
            mp =          {40, 2}
        },
        {   id =          "HorseStrategist",
            promotions =  3,
            stat_grades =  "ASBBC",
            attack_range = "Adjacent4",
            move =        5,
            hp =          {100, 5},
            mp =          {50, 2}
        },
        {   id =          "Dancer",
            promotions =  3,
            stat_grades =  "ABBSB",
            attack_range = "Adjacent4",
            move =        5,
            hp =          {90, 3},
            mp =          {35, 1}
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
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {2, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2, 1},
        {2, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2, 1},
        {3, 2, 2, 3, 3, 2, 2, 1, 2, 2, 2, 3, 2},
        {9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},
        {9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9}
    },
    terrain_effect = {
        -- Rows : terrain / Cols : unit class
        {100, 100, 110, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 110, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100}
    },
    magics = {
        {   id       = "fire_0",
            target   = "enemy",
            accuracy = "always",
            range    = "Distance4_Incl",
            mp       = 10,
            learnat  = {
                { class = "Lord",       level = 10 },
                { class = "Strategist", level = 5 }
            },
            type  = "deal",
            power = 70
        },
        {   id       = "heal_0",
            target   = "ally",
            accuracy = "always",
            range    = "Distance4_Incl",
            mp       = 10,
            learnat  = {
                { class = "Support",    level = 5 },
                { class = "Strategist", level = 15 }
            },
            type  = "heal",
            power = 50
        },
        {   id       = "buff_dex",
            target   = "ally",
            accuracy = "always",
            range    = "Distance3_Incl",
            mp       = 8,
            learnat  = {
                { class = "Support", level = 5 }
            },
            type   = "stat_mod",
            stat   = "dex",
            amount = 20,
            turns  = 3
        }
    },
    heroes = {
        {   id    = "CaoCao",
            class = "Lord",
            stat  = {82, 99, 80, 90, 95},
            model = "CaoCao-1"
        },
        {   id    = "DianWei",
            class = "Fighter",
            stat  = {97, 60, 99, 50, 90},
            model = "dianwei-1"
        },
        {   id    = "ZhangLiao",
            class = "Cavalry",
            stat  = {90, 92, 64, 80, 75},
            model = "ZhangLiao-1"
        },
        {   id    = "ManChong",
            class = "Support",
            stat  = {20, 60, 50, 90, 50},
            model = "support-1-red"
        },
        {   id    = "XiahouDun",
            class = "Cavalry",
            stat  = {94, 84, 72, 48, 95},
            model = "XiahouDun-1"
        },
        {   id    = "XunYu",
            class = "Strategist",
            stat  = {34, 75, 77, 98, 64},
            model = "Strategist-1-red"
        },
        {   id    = "Bandit",
            class = "Bandit",
            stat  = {70, 70, 50, 30, 60},
            model = "Bandit-1-blue"
        },
        {   id    = "Cavalry",
            class = "Cavalry",
            stat  = {70, 70, 55, 30, 50},
            model = "cavalry-1-blue"
        }
    },
    equipments = {
        {   id          = "short_sword",
            type        = "weapon",
            description = "short_sword_desc",
            equipable   = "all",
            effects     = {
                { event = "action_done", effect = "restore_hp", multiplier = 15 }
            },
            modifiers   = {
                { stat = "atk", addend = 20 },
                { stat = "def", addend =  5 }
            }
        },
        {   id          = "heaven_sword",
            type        = "weapon",
            description = "heaven_sword_desc",
            equipable   = "all",
            effects     = {
                { event = "turn_begin",         effect = "restore_hp", multiplier = 15 },
                { event = "on_normal_attacked", effect = "preemptive_attack" },
                { event = "on_normal_attack",   effect = "enhance_basic_attack", multiplier = 25 }
            },
            modifiers   = {
                { stat = "atk", addend = 40 }
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
                { stat = "atk", addend = 20, turn = 3 },
                { stat = "def", addend =  5, turn = 3 }
            }
        }
    },
    stages = { "01", "02" }
}

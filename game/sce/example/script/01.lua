gstage = {
    title_id = "Stage1",
    turn_limit = 20,
    map = {
        size = {28, 20},
        terrain = {
            "WWWWWWWWffmmmmrrrrrrrrmmmmmm",
            "fffffffWffmmmmmrrrrrmmmmmfff",
            "fffffffWffffmmmmrrrmmmmmmfff",
            "fffffffffffffmmmmrmmmmmfffff",
            "fffffffWffffffmmmmmmmmffffff",
            "fffffffWfffffffmmmmmfffffmmm",
            "fffffffWfffffffmmmmffffmmmmm",
            "ffffWffWffffffffffffffmmmmrr",
            "ffffWffWffffffffffffffmmmrrr",
            "ffffWffffffffffmmffffffmmmrr",
            "ffffWffffffffffmmmmmffffmmmr",
            "ffffWffWfffffffmmmmmfffffmmm",
            "ffffWffWffffffmmmmmmmmfffffm",
            "fffffffWffffffmmrrmmmmmffffm",
            "fffffffWffffffmmrrmmmmmmfffm",
            "fffffffffffffffrrrrrfffffffm",
            "fffffffWfffffffrrrrrffffffmm",
            "fffffffWffffffrrrrrrffffffmm",
            "fffffffWffffffrrrrrfffffffmm",
            "WWWWWWWWfffffrrrrrrffffffmmm"
        },
        file = "map"
    },
    deploy = {
        unselectables = {
            { position = {9, 2}, hero = "CaoCao" },
            { position = {9, 1}, hero = "ZhangLiao" },
            { position = {9, 9}, hero = "XunYu" },
            { position = {5, 3}, hero = "DianWei" },
            { position = {9, 3}, hero = "ManChong" }
        },
        num_required_selectables = 0,
        selectables = {
            { position = {9, 4} },
        }
    },
    rewards = { -- NYI
        equipments = {
            { id = "short_sword", amount = 1 }
        },
        money = 1000
    }
}


owns = {
    caocao = 0,
    zhangliao = 1
}

enemies = {
    lubu = nil
}

function on_deploy(game)
    game:appoint_hero("CaoCao", 80)
    game:appoint_hero("ZhangLiao", 10)
    game:appoint_hero("DianWei", 25)
    game:appoint_hero("ManChong", 20)
    game:appoint_hero("XiahouDun", 23)
    game:appoint_hero("XunYu", 33)
    game:obtain_equipment("short_sword", 10)
    game:obtain_equipment("heaven_sword", 1)
end


function on_begin(game)
    -- Enemies
    enemies.lubu = game:generate_unit("LuBu", 12, Enum.force.enemy, {9, 6})

    -- Set AI mode
    game:set_ai_mode(enemies.lubu, "unit_in_range_random")

--    print(game:get_unit_on_position({9, 2}))

--    game:generate_unit("Cavalry", 18, Enum.force.enemy, {10, 7})

    game:cmd_gain_exp(owns.zhangliao, 199)

    -- Test unit info
    info = game:get_unit_info(owns.caocao)
    -- print(info.hero_class)
    -- print(info.hero_attr.atk, info.hero_attr.def)
    -- print(info.unit_attr.base.atk, info.unit_attr.base.def)
    -- print(info.cur_hp, info.max_hp)
    -- print(info.attack_range["0"].x, info.attack_range["0"].y)

    -- Test terrain
    terrain = game:get_terrain_on_position({0, 0})
    -- print(terrain)

    -- game:cmd_kill(owns.zhangliao)
end


function on_victory(game)
    game:cmd_speak(owns.caocao, "So long, losers!")
    game:cmd_move(owns.caocao, {0, 9})
    game:cmd_move(owns.zhangliao, {0, 10})
end


function on_defeat(game)
    print("<<<< ON_DEFEAT >>>>")
end


function end_condition(game)
    if game:get_num_owns_alive() == 0 then
        return Enum.status.defeat
    end
    if game:get_num_enemies_alive() == 0 then
        return Enum.status.victory
    end
    return Enum.status.undecided
end


function event1_condition(game)
    info = game:get_unit_info(owns.caocao)
    return info.position.x == 9 and info.position.y == 3
end


function event1_handler(game, event_id)
    game:cmd_speak(enemies.lubu, "You just activated my trap card!")
    game:cmd_move(owns.caocao, {12, 3})

    -- Unregister since it is just one time event
    game:unregister_event(event_id)
end


function main(game)
    game:set_on_deploy(on_deploy)
    game:set_on_begin(on_begin)
    game:set_on_victory(on_victory)
    game:set_on_defeat(on_defeat)
    game:set_end_condition(end_condition)

    index = game:register_event(event1_condition, event1_handler)
end

# Glossary

This page is about glossary that are used in this project. This document is for engine developers and possibly scenario creators.

### Assets

Assets indicates the user's properties that keeps along the scenario. It consists of Heroes deployed, equipments, money and so on.

For example, Hero level and exp is updated after victory from a stage.

### Basic Attack

Physically hitting a unit. This is the very basic method to do damage to enemies. There is no cost for basic attack unlike Magic.
The defender does counter-attack if the attacker is also in the range of the defender.

Usually if a unit is strong in Basic Attack the unit tends to be weak in magical ability and vice versa.

### Cmd

Cmd is a unit of operating Units. Cmd stands for "Command".

### Equipment

Units/Heroes can equip an equipment for each type. It makes the Units/Heroes more powerful by modifying stats or having event effects. Some Equipments can only be equipped by those with specific Hero Class.

There are 3 types of Equipment:

- Weapon
- Armor
- Aid

### Event Effect

An effect when specific event is triggered for a Unit. This makes the game not to be boring as it creates various effects while playing game.

Technically there are 2 types of Event Effects.

- On Cmd : Manipulates the given Cmd before doing it
- General : Appends a Cmd

### Hero

Each hero has different class, attributes and features. But itself is just a spec not a real unit that users can operate. A hero can be instantiated to a Unit. How powerful the unit is depends on the Hero's ability.

### Hero Class

Each Hero belongs to exactly one class which represents the type of the troop.

Also Heroes can be promoted, refer to [Hero Promotion](#Hero-Promotion) for more.

### Hero Promotion

Heroes can be promoted to higher class when they reaches a specific level

### HP

Health Point. If it goes 0 or below, the unit dies and disappears from the map.

### Magic

Magic is special skills that can make various effect during the battle. Usually it consumes MP to perform a magic.

### MP

Mana Point. Magics require different amount of MP to perform it.

### Range

A range that a unit can perform Basic Attack or Magic.

### Scenario

Scenario is a whole independent game made by scenario creators. It contains all information needed for a game and bitmap resources. The "all information" means lua scripts.

For more, please refer to [Create Scenario](Create-Scenario.md).

### Stage

A scenario consists of one or more Stages. Stage consists of a script file and bitmap resource for the map. All the events and map information are defined in the stage script.

### Stat Modifier

TBD

### Unit

Units are what you can see on the stages. Users(including non-human user, i.e. enemy AI) can operate them.

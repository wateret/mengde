# Stat System

## Stat Components

### Level and Experience

```
level
exp
```

### HP and MP

`HpMp` class in the source code.

```
hp
mp
```

### Attributes

5 Attributes for a hero(unit).

`Attribute` class in the source code.

```
atk
def
dex
itl
mor
```

#### Hero attributes and Unit attributes

TBD

## How to calculate damage, accuracy and so on

Let's see how the values are calculated based on stats described so far.

### Damage Formula

All damage follows below formulae, but the minimum value is fixed to 1.
In other words, the damage is `max(1, CalculatedDamage)`.

* A : Attacker
* D : Defender

#### Attack Damage

```
((A.atk * TerrainEffect) - (D.def * TerrainEffect)) / 3 + A.lv + 25
```

#### Magic Damage 

```
((A.itl - D.itl) / 3 + A.lv + 25) * PowerOfMagic
```

### Accuracy Formula

* `A.dex >= D.dex * 2`

```
100
```

* `A.dex >= D.dex`

```
min(100, (A.dex - D.dex) * 10 / D.dex + 90)
```

* `A.dex >= D.dex / 2`

```
D.dex' =  D.dex / 2

(A.dex - D.dex') * 30 / D.dex'+ 60
```

* `A.dex < D.dex / 2`

```
D.dex'' =  D.dex / 3

max(A.dex - D.dex'', 0) * 30 / D.dex'' + 30
```

### Critical Attack Chance Formula

* `A.mor >= D.mor * 3`

```
100
```

* `A.mor >= D.mor * 2`

```
D.mor' =  D.mor / 2

(A.mor - D.mor') * 80 / D.mor + 20
```

* `D.mor * 2 > A.mor >= D.mor`

```
(A.mor - D.mor) * 18 / D.mor + 2
```

. `A.mor < D.mor`

```
1
```

### Double Attack Chance Formula

Same as Critical Attack Chance's but replace `mor` with `dex`.


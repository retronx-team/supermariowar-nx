Controlling the Player
----------------------
If a player is AI controlled, then the `Think()` method is called once every 4 game frames
(62 avg. fps / 4 = 15 times/sec)
The method is called with a reference to the player's `COutputControl` struct, which is what is
used to send input to the player.  This is the same struct that the gamepad/keyboard fills out.
Essentially, you are replacing the gamepad.

The `COutputControl` has 6 `bool`s you should set to control the player. Before you start setting this object,
you should probably clear out the values and set everything to false.  The controls are:

- `playerKeys->game_left` -> Move the player left
- `playerKeys->game_right` -> Move right (setting both right and left is like setting neither)
- `playerKeys->game_jump` -> Make the player jump (player's jump will be cut short if jump is set `false` before
						 the maximum jump height is achieved)
- `playerKeys->game_down` -> Set `true` and jump to `true` if you want player to fall through a solid on top tile
- `playerKeys->game_turbo` -> Make the player run faster or shoot a projectile or hold it down to pick up an item
- `playerKeys->game_powerup` -> Use a stored powerup item

Each of the 6 inputs above have an `fPressed` and `fDown` member.  The game code uses only the `fDown` (input
held down).  The menu uses the `fPressed` events so your AI will not use those here.  You'll notice in the
current AI that we're only setting the `fDown` member.

Current AI
----------
The way the current AI works is, first we call `GetNearestObjects()`.  This fills out the `nearestObjects`
class with the nearest player, goal (can be powerups, mode goals like coins, pretty much anything good
to collect), threat (shells, fireballs, etc), teammate (for freeing from jail), and stomp goal (for
figuring out things to jump on in stomp mode).

This class is then used by the `Think()` method to determine what to do.  The first part of `Think()` determines
which of the close items are more important.  Evading threats is more important than stomping goals, which is
more important than tagging teammates/tagging goals, etc.

Once that is figured out, then we deal with the single situation that we picked (set in `actionType`).
Each `actionType` has special handling which involves things like figuring out whether to run away or to head
towards an opponent, whether to drop my current item to pick up another, etc.

After the single closest most important item is dealt with, we worry about trying to avoid falling onto hazards.

Then lastly, we use stored powerups if it is to our advantage (like don't use a projectile powerup if we
already have one).

When you watch the current AI in action, you'll notice that it is very good at stomping other players.  It is
also very good at collecting objects on open maps.  But since it doesn't have any path finding, it is very poor
at navigating the map so in maps with lots of solid platforms, it has trouble jumping onto those platforms to
get to its goal.


Accessing The Map
-----------------
There are some things that will probably be useful when creating your own AI that the current code doesn't use.
First is the map tiles.  This will tell you what tiles are in the map to help you write path finding.  Use:

```cpp
int tile = g_map->map(column, row);
```

to get the tile at that column and row.  There are 20 columns and 15 rows.  To convert object position to map
col and row, use:

```cpp
int tile = g_map->map(object->x / TILESIZE, object->y / TILESIZE);
```

The `int` returned is an bit flag int of all the attributes of tiles like solid, solid-on-top, death on various sides, ice, etc.

Second is the interaction blocks in the map.  You might want to know where a [?] block is so you can go hit it.

```cpp
short blocktype = g_map->blockat(col, row)->iType;
```

`blocktype` will now be set to the type id for the block in that column and row.  If it is set to `-1`, then
there is no interaction block in that space.

	0 == Breakable block
	1 == Powerup Block
	2 == Donut Block
	3 == Flip Block
	4 == Bounce Block
	5 == Note Block
	6 == Throw Block
	7 to 10 == On/Off Switch
	11 to 14 == On/Off [!] Block

Another thing that is probably useful is the state of the block at that location.  A player probably isn't interested
in a [?] that has been used and won't give you a powerup.

To get this information, we use:

```cpp
if(g_map->blockat(col, row)->iType == 1)
{
	B_PowerupBlock * block = (B_PowerupBlock*) g_map->block(col, row);
	if(block->GetState() == 0)
	{
		//Contains powerup and ready to be hit
	}
}
```

Flip blocks and On/Off Blocks have a solid state and a fall through state.  To get this information, use:

```cpp
if(g_map->blockat(col, row)->iType == 3)
{
	B_FlipBlock * block = (B_FlipBlock*) g_map->block(col, row);
	if(block->isTransparent())
	{
		//Flip block is spinning and player will fall through
	}
	else
	{
		//Block is solid
	}
}
```

Substitute `B_FlipBlock` for `B_SwitchBlock` to do switch blocks.

Player Object
-------------
You also have access to the player object that you are controlling.  You must be careful not to set
any of the settings of this player object and use them read only.  There are a lot of assumptions in the
player code that these values will not be modified by the AI.  Here are some members you will probably
find useful:

1. Player is in the air
```cpp
pPlayer->inair
```

2. True if player is on ice:
```cpp
pPlayer->onice
```

3. Player's velocity
```cpp
pPlayer->velx;
pPlayer->vely;
```

4. Player's position:
```cpp
pPlayer->ix;
pPlayer->iy;
```

5. Get the object the player is currently carrying:
```cpp
MO_CarriedObject * object = pPlayer->carriedItem;
MovingObjectType mot = object->getMovingObjectType();

if(mot == movingobject_shell)
	// then do something with the shell
```

other moving object enums you'll probably want to use:
`movingobject_throwblock`, `movingobject_egg`, `movingobject_star`, `movingobject_flag`

6. Player powerups
```
pPlayer->powerup == -1 for none, 0 for bobomb, 1 for fireballs, 2 for hammer, 3 for feather, 4 for boomerang
```

7. Figure out what direction the player is facing (good for knowing if you should throw a shell or other object)
```cpp
pPlayer->IsPlayerFacingRight();
```

8. Player is invincible
```cpp
pPlayer->invincible
```

9. True if the player is a bobomb:
```cpp
pPlayer->bobomb
```

10. Player is "it" in tagged mode:
```cpp
game_values.gamemode->tagged == pPlayer
```

11. Player is "it" in chicken mode:
```cpp
game_values.gamemode->chicken == pPlayer
```

12. Player has star in star mode:
```cpp
if(game_values.gamemode->gamemode == game_mode_star)
{
	bool playerIsStar = ((CGM_Star*)game_values.gamemode)->isplayerstar(pPlayer);
}
```

13. If greater than 0, player is jailed in jail mode:
```cpp
pPlayer->jailtimer;
```

14. When true, these actions were pressed and need to be released before pressing action again:
```cpp
pPlayer->lockfall
pPlayer->lockjump
pPlayer->lockfire
```


Difficulty Settings
-------------------
Your AI code will also need to deal with difficutly settings.  You can access the difficulty level
using the global variable: `game_values.cpudifficulty`.  0 is the easiest setting and 4 is the most
difficult.  Your AI should be unhindered and make the smartest decisions for setting 4.

The current AI uses the difficulty setting in a very simple way.  It randomly just doesn't do anything
on its turn based on the setting.  For setting 0, it only makes a decision 25% of the time, where at
setting 4, it makes decisions 100% of the time.  See below.


Using Your New AI
--------------------
When you create your own AI class, look for where we create the `CPlayer` objects at the beginning of the
`RunGame()` function in `main.cpp`.  You'll see we create a new `CPlayerAI()` for every player that has
`game_values.playercontrol[iPlayer]` set to `2` (AI controlled).  Simply replace:

```cpp
ai = new CPlayerAI();
```

with:

```cpp
ai = new CYourAIClass();
```

and the computer players will now be controlled by your AI.

Issues With The Current AI
--------------------------
1. Bots don't use the tanooki powerup.
2. If a bot is holding something(Not a ztar/shine/egg) in ztar, or yoshi's egg mode, they can't pick up the ztar/shine/egg.
3. Bots can't pick up throwable blocks that they are standing on.


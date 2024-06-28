Runescript API status
=====================

https://web.archive.org/web/20080907105147/http://gowerarchive.googlepages.com/documents#Runescript

Runescript refers to 3D objects as *locations* (locs), 2D walls as
*boundaries* (bounds), items as *objects*, skills as *stats*:

* Locs are referenced by x and y coordinate.
* Bounds are referenced by x, y, and direction.
* Players are referenced by unique ID.
* NPCs are referenced by unique ID.

Most types of entity (objects, bounds, locs, NPCs, spells)
have multiple possible names which are used as script triggers.
Only the first name is visible to players. For example, farmers
have Farmer,Man1 which gives them the generic "man" NPC dialogue.

For inventory-based commands, use the name of the item.

Implemented
-----------

* `thinkbubble()` - implemented as `thinkbubble(player, item)` in Lua
* `ifmale()` - implemented as `male(player)` in Lua
* `openshop(shop)` - implemented as `openshop(player, shop)` in Lua
* `displaybalance()` - implemented as `displaybalance(player)` in Lua
* `delay(delay)` - implemented as-is
* `pause(mindelay, maxdelay)` - implemented as-is
* `ifrandom(probability)` - implemented as `random(probability)` in Lua
* `mes(string)` - implemented as `mes(player, string)` in Lua
* `say(string)` - implemented as `say(player, string)` in Lua
* `multi2(...)` - implemented as `multi(player, ...)` in Lua
* `multi3(...)` - implemented as `multi(player, ...)` in Lua
* `multi4(...)` - implemented as `multi(player, ...)` in Lua
* `ifstatrandom(stat, base-probability, top-probability)` - implemented as `statrandom(player, ...)` in Lua
* `advancestat(stat, base, exp)` - implemented as `advancestat(player, ...)` in Lua
* `addstat(stat, constant, percent)` - implemented as `addstat(player, ...)` in Lua
* `substat(stat, constant, percent)` - implemented as `substat(player, ...)` in Lua
* `healstat(stat, constant, percent)` - implemented as `healstat(player, ...)` in Lua
* `ifstatup(stat)` - implemented as `statup(player, stat)` in Lua
* `ifstatdown(stat)` - implemented as `statdown(player, stat)` in Lua
* `ifstatatleast(stat,variable,value)` - implemented as `statatleast(player, stat, value)` in Lua
* `giveqp(value)` - implemented as `giveqp(player, value)` in Lua
* `randomvar(value)` - implemented as-is
* `addobject(object, count, time)` - implemented as `addobject(player, object, count, x, y)` in Lua, with `time` being inferred from `config.jag`.
* `addloc(location)` - implemented as `addloc(location, x, y, timer)` in Lua.
* `ifblocked()` - implemented as `blocked(x, y)` in Lua.
* `give(object, count)` - implemented as `give(player, object, count)` in Lua.
* `remove(object, count)` - implemented as `remove(player, object, count)` in Lua.
* `ifworn(object)` - implemented as `worn(player, object)` in Lua.
* `ifheld(object, count)` - implemented as `held(player, object, count)` in Lua.
* `sellinv(percentage)` - implemented as `sellinv(player, object, percentage)` in Lua.
* `getvar(variable)` - implemented as `getvar(player, variable)` in Lua.
* `setvar(variable, value)` - implemented as `setvar(player, variable, value)` in Lua.
* `delobject()` - implemented as `delobject(player, object, x, y)` in Lua.
* `takeobject()` - implemented as `takeobject(player, object, x, y)` in Lua.
* `changeloc(location)` - implemented as `changeloc(x, y, location)` in Lua.
* `upstairs()` - implemented as `upstairs(player, x, y)` in Lua.
* `downstairs()` - implemented as `downstairs(player, x, y)` in Lua.
* `changelevelup()` - implemented as `changelevelup(player)` in Lua.
* `changeleveldown()` - implemented as `changeleveldown(player)` in Lua.
* `npcsay(string)` - implemented as `npcsay(npc, string)` in Lua.
* `npcbusy()` - implemented as `npcbusy(npc)` in Lua.
* `npcunbusy()` - implemented as `npcunbusy(npc)` in Lua.
* `shootnpc(projectile)` - implemented as `shootnpc(player, npc, projectile)` in Lua.
* `addnpcstat(stat,constant,percent)` - implemented as `npcaddstat(npc, stat, constant, percent)` in Lua (TODO: rename?)
* `subnpcstat(stat,constant,percent)` - implemented as `npcsubstat(npc, stat, constant, percent)` in Lua (TODO: rename?)
* `healnpcstat(stat,constant,percent)` - implemented as `npchealstat(npc, stat, constant, percent)` in Lua (TODO: rename?)
* `ifnpcstatup(stat)` - implemented as `npcstatup(npc, stat)` in Lua.
* `ifnpcstatdown(stat)` - implemented as `npcstatdown(npc, stat)` in Lua.
* `npcattack()` - implemented as `npcattack(npc, player)` in Lua.
* `shootplayer(projectile)` - implemented as `shootplayer(player, target, projectile)` in Lua.
* `ifqp(value)` - implemented as `qp(player)` in Lua.
* `playercoord()` - implemented as `playercoord(player)` in Lua, returns x, y values.
* `addnpc(npc)` - implemented as `addnpc(npc, x, y)` in Lua, returns npc handle.
* `ifnearnpc(npc)` - implemented as `nearnpc(player, npc)` in Lua, returns npc handle or nil.
* `teleport()` - implemented as `teleport(player, x, y)` in Lua.

Not implemented
---------------

* `modpause(mindelay, maxdelay)`
* `changelevel(level)`
* `showeffect(type)`
* `delinv()`
* `ifobjectvisible()`
* `ifnearvisnpc(npc)`
* `ifnpcvisible()`
* `delnpc()`
* `changenpc(npc)`
* `npcretreat(time)`
* `ifplayervisible()`

Intentionally not implemented
-----------------------------

* Most variable commands. Use Lua operators.
* `addplaystat`, `subplaystat` and friends. Use the regular `addstat` functions.
* `jump(labelname)` - use Lua functions.
* `fork(labelname)` - use Lua functions.
* `end()` - use Lua `return`.
* `ifstatabove(stat,value)` - use `statatleast`.
* `setcoord(coordinate)` - use Lua variables.

Implementation details
----------------------

* The wiki claims that the Vampire should face the player when its
  coffin is opened.  Likely `ifnearnpc` causes this behaviour to happen,
  the docs note that it marks the player as interacting with a NPC.
* Since there is no way to check whether a NPC is busy, it's likely
  that `ifnearvisnpc` ignores NPCs that are busy, and this is how
  distraction mechanics (e.g. in the knight's sword quest) are
  implemented.

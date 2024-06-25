Planned features
================

Steps towards feature completion for 10th December 2001:

Engine
------

- [x] Melee PvP combat
- [x] Melee PvM combat
- [x] Ranged PvP combat
- [x] Ranged PvM combat
- [x] Prayer
- [x] Player design
- [x] Player classes
- [x] Player movement
- [x] Player following
- [x] Player trading
- [x] Player messaging
- [x] Player client settings
- [x] Player privacy settings
- [x] NPC spawn loading
- [x] NPC movement
- [ ] NPC aggression
- [x] NPC "bravery" (retreating)
- [ ] NPC "regenerate" timer
- [ ] Line of sight checks
- [x] Friends list
- [x] Ignore list
- [x] Locs
- [x] Boundaries
- [x] Item spawns
- [x] Dropping items
- [x] Equipping items
- [x] Inventory
- [x] Experience and levelling up
- [x] Stat restoration
- [x] Lua scripting
- [x] Shops
- [x] Item banking
- [ ] Spell effects
- [ ] Player saving (SQLite)
- [x] Protocol: 110
- [ ] Protocol: 110 (new player)
- [ ] Protocol: 110 (password change)
- [x] Protocol: RSA decryption
- [ ] Protocol: 201
- [x] Protocol: 204
- [ ] Five minute movement timer (should be optional)
- [ ] "Your X ability has returned to normal" message

### Known engine bugs

* Can get stuck in combat. Probably due to lack of pathing check
  in player_init_combat.
* NPCs can sometimes walk diagonally through walls, indicating there's
  probably a problem with the collision detection code.
* Chatting between 110 and 204 clients is currently not possible.
* Should be impossible for NPCs to walk through other NPCs and players.
* Should be impossible for players to walk through aggressive NPCs.
* Quest list is broken on 204.

Content
-------

These should all be implemented in Lua script:

### Skills

- [x] Prayer (bone burying)
- [x] Prayer (altars)
- [x] Magic (missiles vs. players)
- [x] Magic (missiles vs. NPCs)
- [ ] Magic (crumble undead) - needs config parser fixes
- [x] Magic (curses)
- [ ] Magic (teleportation)
- [ ] Magic (telegrab)
- [x] Magic (alchemy)
- [ ] Magic (superheat)
- [ ] Magic (enchantment)
- [x] Woodcutting
- [x] Firemaking
- [x] Crafting (pottery)
- [ ] Crafting (gold jewelry)
- [ ] Crafting (silver jewelry)
- [x] Crafting (leatherworking)
- [x] Crafting (wool)
- [x] Fishing
- [x] Cooking (fish/meat)
- [ ] Cooking (bread)
- [x] Cooking (wine)
- [ ] Cooking (pies)
- [ ] Cooking (pizzas)
- [ ] Cooking (cake)
- [x] Cooking (stew)
- [x] Mining
- [ ] Smithing (forging)
- [x] Smithing (smelting)

### Quests

- [ ] Cook's Assistant
- [x] Sheep Shearer
- [ ] The Restless Ghost
- [ ] Demon Slayer
- [ ] Romeo & Juliet
- [ ] Shield of Arrav
- [ ] Ernest the chicken
- [x] Vampire Slayer
- [x] Imp catcher
- [ ] Prince Ali Rescue
- [x] Witch's potion
- [ ] Black knight's fortress
- [x] Doric's quest
- [ ] The knight's sword
- [ ] Goblin diplomacy
- [ ] Pirate's treasure
- [ ] Dragon slayer

### Miscellaneous

- [x] Brass key
- [x] Muddy key and chest
- [x] Hans NPC dialogue
- [x] Silk trader NPC dialogue
- [x] Tanner NPC dialogue
- [x] Kebab Seller NPC dialogue
- [ ] Aggie NPC dialogue
- [ ] Certing (Miles, Niles, Giles NPCs)
- [x] Bartender NPC dialogue
- [x] Barmaid NPC dialogue
- [x] Monk NPC dialogue
- [x] Barbarian NPC dialogue
- [x] Pirate NPC dialogue
- [x] Man NPC dialogue
- [ ] Thrander NPC dialogue
- [ ] Bareak NPC dialogue
- [ ] Apothecary NPC dialogue
- [x] Banker NPC dialogue
- [ ] Cook's Guild
- [ ] Champions' Guild
- [ ] Monastery (Prayer Guild)
- [x] Alcohol (beers, wizards mind bomb)
- [x] Food (fish/meat)
- [x] Food (cabbage)
- [x] Food (bread)
- [x] Food (cake)
- [x] Food (pizzas)
- [x] Food (pies)
- [x] Food (stews)
- [x] Food (kebab)
- [x] Food (spinach roll)
- [x] Strength Potions
- [x] Dying capes
- [x] Mixing dyes
- [x] Monk of Zamorak wine aggression
- [x] Picking plants
- [x] Cutting webs
- [x] Water sources
- [ ] Using the windmill
- [x] Using ladders
- [ ] Using stairs (broken)
- [ ] Al Kharid general store should reject silk
- [ ] Equipment level requirements
- [ ] Silverlight effect

### Drop tables

Most NPCs (that use constant drops) are found within config46.jag.
The following use randomvar and need to be implemented in script:

- [x] Chicken
- [x] Man1 (Man, Thief, Farmer)
- [ ] Mugger (looks like variation of Man1 table)
- [ ] Wiz1 (Darkwizard)
- [ ] Darkerwiz (Darkwizard)
- [ ] Goblin1
- [ ] Goblin2
- [ ] Barbarian
- [ ] Dwarf
- [ ] Warrior
- [ ] Skeleton1
- [ ] Skeleton2
- [ ] Skeleton3
- [ ] Zombie1
- [ ] Zombie2
- [ ] Zombie3
- [ ] Pirate
- [ ] Guard
- [ ] Giant
- [ ] Black Knight
- [ ] White Knight
- [ ] Hobgoblin1
- [ ] Ice Warrior
- [ ] Chaos dwarf
- [ ] Moss Giant
- [ ] Ice Giant
- [ ] Lesser Demon
- [ ] Greater Demon

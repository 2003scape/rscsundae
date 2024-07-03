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
- [x] NPC aggression
- [x] NPC "bravery" (retreating)
- [x] NPC "regenerate" timer
- [x] Line of sight checks
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
- [ ] Login server
- [x] Protocol: 110
- [ ] Protocol: 110 (new player)
- [ ] Protocol: 110 (password change)
- [x] Protocol: RSA decryption
- [ ] Protocol: 201
- [x] Protocol: 204
- [ ] Five minute movement timer (should be optional)
- [ ] Al Kharid general store should reject silk

### Known engine bugs

* Chatting between 110 and 204 clients is currently not possible.
* When using a bar on an anvil that's some distance away,
  the script can hang for a few seconds
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
- [x] Magic (superheat)
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
- [x] Cooking (bread)
- [x] Cooking (wine)
- [x] Cooking (pies)
- [x] Cooking (pizzas)
- [ ] Cooking (cake)
- [x] Cooking (stew)
- [x] Cooking (special range)
- [x] Mining
- [x] Smithing (forging)
- [x] Smithing (smelting)

### Quests

- [x] Cook's Assistant
- [x] Sheep Shearer
- [x] The Restless Ghost
- [ ] Demon Slayer
- [x] Romeo & Juliet (several bugs not implemented)
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
- [x] Apothecary NPC dialogue (a broken dialogue path where he gives the player spot cream twice is not implemented)
- [x] Banker NPC dialogue
- [ ] Cook's Guild
- [ ] Champions' Guild
- [x] Mining Guild
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
- [x] Using the windmill
- [x] Using ladders
- [x] Using stairs
- [x] Equipment level requirements
- [ ] Silverlight effect
- [x] Punching dummies

### Drop tables

Most NPCs (that use constant drops) are found within config46.jag.
The following use randomvar and need to be implemented in script:

- [x] Chicken
- [x] Man1 (Man, Thief, Farmer, Warrior)
- [ ] Mugger (looks like variation of Man1 table)
- [x] Wiz1 (Darkwizard)
- [x] Darkerwiz (Darkwizard)
- [x] Goblin1
- [ ] Goblin2
- [x] Barbarian
- [x] Dwarf
- [ ] Skeleton1
- [ ] Skeleton2
- [ ] Skeleton3
- [x] Zombie1
- [ ] Zombie2
- [ ] Zombie3
- [x] Pirate
- [ ] Guard
- [x] Giant
- [ ] Black Knight
- [ ] White Knight
- [x] Hobgoblin1
- [ ] Ice Warrior
- [ ] Chaos dwarf
- [ ] Moss Giant
- [ ] Ice Giant
- [ ] Lesser Demon
- [ ] Greater Demon

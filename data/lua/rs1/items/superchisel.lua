-- original item: used as developer tool for various quests
-- this script: not intending to be like the original at all

local function superchisel_tele_spot(player)
	local resp = multi(player,
		"Goblin Village",
		"Barbarian Village",
		"Port Sarim chickens")
	if resp == 1 then
		teleport(player, 323, 448)
	elseif resp == 2 then
		teleport(player, 233, 514)
	elseif resp == 3 then
		teleport(player, 275, 604)
	end
end

local function superchisel_tele_deep_wild(player)
	local resp = multi(player,
		"Greater demons",
		"Ice Plateau",
		"Runite mine")
	if resp == 1 then
		teleport(player, 82, 162)
	elseif resp == 2 then
		teleport(player, 327, 159)
	elseif resp == 3 then
		teleport(player, 255, 156)
	end
end

local function superchisel_tele_shallow_wild(player)
	local resp = multi(player,
		"Hobgoblin mine",
		"Bone Yard",
		"Graveyard",
		"Chaos Temple")
	if resp == 1 then
		teleport(player, 224, 255)
	elseif resp == 2 then
		teleport(player, 125, 267)
	elseif resp == 3 then
		teleport(player, 170, 310)
	elseif resp == 4 then
		teleport(player, 116, 364)
	end
end

local function superchisel_tele_wild(player)
	local resp = multi(player,
		"Wilderness",
		"Deep wilderness")
	if resp == 1 then
		superchisel_tele_shallow_wild(player)
	elseif resp == 2 then
		superchisel_tele_deep_wild(player)
	end
end

local function superchisel_tele_mine(player)
	local resp = multi(player,
		"Mining Guild",
		"Rimmington",
		"Varrock East",
		"Varrock West")
	if resp == 1 then
		teleport(player, 270, 3394)
	elseif resp == 2 then
		teleport(player, 313, 641)
	elseif resp == 3 then
		teleport(player, 76, 548)
	elseif resp == 4 then
		teleport(player, 165, 546)
	end
end

local function superchisel_tele_dungeon(player)
	local resp = multi(player,
		"Asgarnia Ice Dungeon (Pirates)",
		"Edgeville Dungeon (Giants)",
		"Varrock Sewers (Moss Giants)")
	if resp == 1 then
		teleport(player, 279, 3525)
	elseif resp == 2 then
		teleport(player, 208, 3319)
	elseif resp == 3 then
		teleport(player, 159, 3297)
	end
end

local function superchisel_tele_town(player)
	local resp = multi(player,
		"Edgeville",
		"Karamja",
		"Draynor Village",
		"Al Kharid")
	if resp == 1 then
		teleport(player, 226, 447)
	elseif resp == 2 then
		teleport(player, 360, 696)
	elseif resp == 3 then
		teleport(player, 214, 632)
	elseif resp == 4 then
		teleport(player, 72, 696)
	end
end

local function superchisel_teleport(player)
	local resp = multi(player,
		"Towns",
		"Dungeons",
		"Mining spots",
		"Training spots",
		"Wilderness")
	if resp == 1 then
		superchisel_tele_town(player)
	elseif resp == 2 then
		superchisel_tele_dungeon(player)
	elseif resp == 3 then
		superchisel_tele_mine(player)
	elseif resp == 4 then
		superchisel_tele_spot(player)
	elseif resp == 5 then
		superchisel_tele_wild(player)
	end
end

local function superchisel_dev(player)
	local resp = multi(player,
		"Max stats",
		"Max equipment",
		"All runes")
	if resp == 1 then
		for i = 0,15 do
			advancestat(player, i, 56000000, 0)
		end
	elseif resp == 2 then
		give(player, "rune 2-handed sword", 1)
		give(player, "large rune helmet", 1)
		if male(player) then
			give(player, "rune plate mail body", 1)
		else
			give(player, "rune plate mail top", 1)
		end
		give(player, "rune plate mail legs", 1)
		give(player, "diamond amulet of power", 1)
		give(player, "bluecape", 1)
	elseif resp == 3 then
		give(player, "air-rune", 1000)
		give(player, "fire-rune", 1000)
		give(player, "water-rune", 1000)
		give(player, "earth-rune", 1000)
		give(player, "body-rune", 1000)
		give(player, "mind-rune", 1000)
		give(player, "chaos-rune", 1000)
		give(player, "death-rune", 1000)
		give(player, "law-rune", 1000)
		give(player, "cosmic-rune", 1000)
		give(player, "nature-rune", 1000)
	end
end

local function superchisel_event(player)
	local resp = multi(player,
		"Spawn pumpkins",
		"Spawn cabbages",
		"Spawn cake")
	if resp == 1 then
		item = "pumpkin"
	elseif resp == 2 then
		item = "magic cabbage"
	elseif resp == 3 then
		item = "slice of cake"
	end
	for x=50,400,6 do
		for y=100,750,6 do
			if not blocked(x, y) then
				addobject(player, item, 1, x, y)
			end
		end
	end
end

function opinv_superchisel(player)
	local resp = multi(player,
		"Teleport",
		"Developer special",
		"Event special")
	if resp == 1 then
		superchisel_teleport(player)
	elseif resp == 2 then
		superchisel_dev(player)
	elseif resp == 3 then
		superchisel_event(player)
	end
end

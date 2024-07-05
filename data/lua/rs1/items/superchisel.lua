-- original item: used as developer tool for various quests
-- this script: not intending to be like the original at all

local function superchisel_teleport(player)
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

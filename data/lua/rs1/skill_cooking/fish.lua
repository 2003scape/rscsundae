-- https://classic.runescape.wiki/w/Manual:Cooking_%26_Fishing_(2002)
-- rsc-preservation.xyz/Skilling/Fishing_Cooking [Barbarian Village] [Fly Fishing] [Fishing Level 22-41] [Cooking Level 15-40] part 1.pcap
-- 1e_Luis/Skilling/Cooking fish requirements

-- rates taken from OSRS
local fish_types = {
	shrimp = {
		level = 1,
		raw = "raw shrimp",
		name = "shrimp",
		burnt = "burntfish1",
		mes = "The shrimp is now nicely cooked",
		xp = 120,
		low = 128,
		high = 512,
		low_special = 138,
		high_special = 532,
	},
	anchovies = {
		level = 1,
		raw = "raw anchovies",
		name = "anchovies",
		burnt = "burntfish1",
		mes = "The anchovies are now nicely cooked",
		xp = 120,
		low = 128,
		high = 512,
		low_special = 138,
		high_special = 532,
	},
	sardine = {
		level = 1,
		raw = "raw sardine",
		name = "sardine",
		burnt = "burntfish2",
		mes = "The sardine is now nicely cooked",
		xp = 160,
		low = 118,
		high = 492,
		low_special = 128,
		high_special = 512,
	},
	herring = {
		level = 5,
		raw = "raw herring",
		name = "herring",
		burnt = "burntfish3",
		mes = "The herring is now nicely cooked",
		xp = 200,
		low = 108,
		high = 472,
		low_special = 118,
		high_special = 492,
	},
	trout = {
		level = 15,
		raw = "raw trout",
		name = "trout",
		burnt = "burntfish2",
		mes = "The trout is now nicely cooked",
		xp = 280,
		low = 88,
		high = 432,
		low_special = 98,
		high_special = 452,
	},
	pike = {
		level = 20,
		raw = "raw pike",
		name = "pike",
		burnt = "burntfish3",
		mes = "The pike is now nicely cooked",
		xp = 320,
		low = 78,
		high = 412,
		low_special = 88,
		high_special = 432,
	},
	salmon = {
		level = 25,
		raw = "raw salmon",
		name = "salmon",
		burnt = "burntfish2",
		mes = "The salmon is now nicely cooked",
		xp = 360,
		low = 68,
		high = 392,
		low_special = 78,
		high_special = 402,
	},
	tuna = {
		level = 30,
		raw = "raw tuna",
		name = "tuna",
		burnt = "burntfish4",
		mes = "The tuna is now nicely cooked",
		xp = 400,
		low = 58,
		high = 372
	},
	lobster = {
		level = 40,
		raw = "raw lobster",
		name = "lobster",
		burnt = "burntfish6",
		mes = "The lobster is now nicely cooked",
		xp = 480,
		low = 38,
		high = 332
	},
	swordfish = {
		level = 45,
		raw = "raw swordfish",
		name = "swordfish",
		burnt = "burntfish5",
		mes = "The swordfish is now nicely cooked",
		xp = 560,
		low = 18,
		high = 292
	}
}

local function cook_fish(player, fish, loc)
	local low = fish.low
	local high = fish.high
	if loc == "cookrange" then
		if fish.low_special then
			low = fish.low_special
		end
		if fish.high_special then
			high = fish.high_special
		end
		loc = "stove"
	end
	if not statatleast(player, STAT_COOKING, fish.level) then
		mes(player,
		    string.format("@que@You need a cooking level of %d to cook %s",
			    fish.level, fish.name))
		return
	end
	thinkbubble(player, fish.raw)
	mes(player, string.format("@que@You cook the %s on the %s", fish.name, loc))
	delay(3)
	remove(player, fish.raw, 1)
	if statrandom(player, STAT_COOKING, fish.low, fish.high) then
		mes(player, fish.mes)
		give(player, fish.name, 1)
		advancestat(player, STAT_COOKING, fish.xp, 0)
	else
		mes(player, "@que@You accidentally burn the " .. fish.name)
		give(player, fish.burnt, 1)
	end
end

for k, v in pairs(fish_types) do
	register_useloc("cookrange", v.raw, function(player, x, y)
		cook_fish(player, v, "cookrange")
	end)

	register_useloc("range", v.raw, function(player, x, y)
		cook_fish(player, v, "stove")
	end)

	register_useloc("fire", v.raw, function(player, x, y)
		cook_fish(player, v, "fire")
	end)
end

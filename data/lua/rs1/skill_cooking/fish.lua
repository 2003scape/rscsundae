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
		high = 512
	},
	anchovies = {
		level = 1,
		raw = "raw anchovies",
		name = "anchovies",
		burnt = "burntfish1",
		mes = "The anchovies are now nicely cooked",
		xp = 120,
		low = 128,
		high = 512
	},
	sardine = {
		level = 1,
		raw = "raw sardine",
		name = "sardine",
		burnt = "burntfish2",
		mes = "The sardine is now nicely cooked",
		xp = 160,
		low = 118,
		high = 492
	},
	herring = {
		level = 5,
		raw = "raw herring",
		name = "herring",
		burnt = "burntfish3",
		mes = "The herring is now nicely cooked",
		xp = 200,
		low = 108,
		high = 472
	},
	trout = {
		level = 15,
		raw = "raw trout",
		name = "trout",
		burnt = "burntfish2",
		mes = "The trout is now nicely cooked",
		xp = 280,
		low = 88,
		high = 432
	},
	pike = {
		level = 20,
		raw = "raw pike",
		name = "pike",
		burnt = "burntfish3",
		mes = "The pike is now nicely cooked",
		xp = 320,
		low = 78,
		high = 412
	},
	salmon = {
		level = 25,
		raw = "raw salmon",
		name = "salmon",
		burnt = "burntfish2",
		mes = "The salmon is now nicely cooked",
		xp = 360,
		low = 68,
		high = 392
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
	if not statatleast(player, STAT_COOKING, fish.level) then
		mes(player,
		    string.format("@que@You need a cooking level of %d to cook %s",
			    fish.level, fish.name))
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

function useloc_range_raw_shrimp(player, x, y)
	cook_fish(player, fish_types["shrimp"], "stove")
end

function useloc_range_raw_anchovies(player, x, y)
	cook_fish(player, fish_types["anchovies"], "stove")
end

function useloc_range_raw_sardine(player, x, y)
	cook_fish(player, fish_types["sardine"], "stove")
end

function useloc_range_raw_herring(player, x, y)
	cook_fish(player, fish_types["herring"], "stove")
end

function useloc_range_raw_trout(player, x, y)
	cook_fish(player, fish_types["trout"], "stove")
end

function useloc_range_raw_pike(player, x, y)
	cook_fish(player, fish_types["pike"], "stove")
end

function useloc_range_raw_salmon(player, x, y)
	cook_fish(player, fish_types["salmon"], "stove")
end

function useloc_range_raw_tuna(player, x, y)
	cook_fish(player, fish_types["tuna"], "stove")
end

function useloc_range_raw_lobster(player, x, y)
	cook_fish(player, fish_types["lobster"], "stove")
end

function useloc_range_raw_swordfish(player, x, y)
	cook_fish(player, fish_types["swordfish"], "stove")
end

function useloc_fire_raw_shrimp(player, x, y)
	cook_fish(player, fish_types["shrimp"], "fire")
end

function useloc_fire_raw_anchovies(player, x, y)
	cook_fish(player, fish_types["anchovies"], "fire")
end

function useloc_fire_raw_sardine(player, x, y)
	cook_fish(player, fish_types["sardine"], "fire")
end

function useloc_fire_raw_herring(player, x, y)
	cook_fish(player, fish_types["herring"], "fire")
end

function useloc_fire_raw_trout(player, x, y)
	cook_fish(player, fish_types["trout"], "fire")
end

function useloc_fire_raw_pike(player, x, y)
	cook_fish(player, fish_types["pike"], "fire")
end

function useloc_fire_raw_salmon(player, x, y)
	cook_fish(player, fish_types["salmon"], "fire")
end

function useloc_fire_raw_tuna(player, x, y)
	cook_fish(player, fish_types["tuna"], "fire")
end

function useloc_fire_raw_lobster(player, x, y)
	cook_fish(player, fish_types["lobster"], "fire")
end

function useloc_fire_raw_swordfish(player, x, y)
	cook_fish(player, fish_types["swordfish"], "fire")
end

register_useloc("range", "raw shrimp", useloc_range_raw_shrimp)
register_useloc("range", "raw anchovies", useloc_range_raw_anchovies)
register_useloc("range", "raw sardine", useloc_range_raw_sardine)
register_useloc("range", "raw herring", useloc_range_raw_herring)
register_useloc("range", "raw trout", useloc_range_raw_trout)
register_useloc("range", "raw pike", useloc_range_raw_pike)
register_useloc("range", "raw salmon", useloc_range_raw_salmon)
register_useloc("range", "raw tuna", useloc_range_raw_tuna)
register_useloc("range", "raw lobster", useloc_range_raw_lobster)
register_useloc("range", "raw swordfish", useloc_range_raw_swordfish)

register_useloc("fire", "raw shrimp", useloc_fire_raw_shrimp)
register_useloc("fire", "raw anchovies", useloc_fire_raw_anchovies)
register_useloc("fire", "raw sardine", useloc_fire_raw_sardine)
register_useloc("fire", "raw herring", useloc_fire_raw_herring)
register_useloc("fire", "raw trout", useloc_fire_raw_trout)
register_useloc("fire", "raw pike", useloc_fire_raw_pike)
register_useloc("fire", "raw salmon", useloc_fire_raw_salmon)
register_useloc("fire", "raw tuna", useloc_fire_raw_tuna)
register_useloc("fire", "raw lobster", useloc_fire_raw_lobster)
register_useloc("fire", "raw swordfish", useloc_fire_raw_swordfish)

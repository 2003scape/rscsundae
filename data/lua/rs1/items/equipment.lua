-- 1e_Luis/Effect testing/Not meeting reqs to wear quest stuff
-- TODO: rune plate mail body requires quest

local function fail_req(player, stat, name, level)
	-- maybe later versions (after attack/defense potions) had a
	-- 'statbaseatleast' type function
	local failed = false
	if not statatleast(player, stat, level) then
		failed = true
	end
	-- magical cabbage
	if statup(player, stat) then
		failed = true
	end
	if failed then
		mes(player,
		    "@que@You are not a high enough level to use this item")
		-- incorrect grammar here is kosher, see replay
		mes(player,
		    string.format("@que@You need to have a %s level of %d", name, level))
		return true
	end
	return false
end

local function fail_body(player)
	if male(player) then
		mes(player, "@que@It doesn't fit!")
		mes(player, "@que@Perhaps I should get someone to adjust it for me")
		return true
	end
	return false
end

register_wearobj("bronze plate mail top", function(player)
	return not fail_body(player)
end)

register_wearobj("iron plate mail top", function(player)
	return not fail_body(player)
end)

local tier5_weapons = {
	"steel dagger",
	"steel short sword",
	"steel long sword",
	"steel 2-handed sword",
	"steel scimitar",
	"steel axe",
	"steel battle axe",
	"steel mace",
}

local tier5_armour = {
	"medium steel helmet",
	"large steel helmet",
	"steel chain mail body",
	"steel plate mail body",
	"steel plate mail legs",
	"steel square shield",
	"steel kite shield",
	"steel plated skirt"
}

for i, item in ipairs(tier5_weapons) do
	register_wearobj(item, function(player)
		return not fail_req(player, STAT_ATTACK, "attack", 5)
	end)
end

for i, item in ipairs(tier5_armour) do
	register_wearobj(item, function(player)
		return not fail_req(player, STAT_DEFENSE, "defense", 5)
	end)
end

register_wearobj("steel plate mail top", function(player)
	local fail = false
	if fail_req(player, STAT_DEFENSE, "defense", 5) then
		fail = true
	end
	if fail_body(player) then
		fail = true
	end
	return not fail
end)

local tier10_weapons = {
	"black dagger",
	"black short sword",
	"black long sword",
	"black 2-handed sword",
	"black scimitar",
	"black axe",
	"black battle axe",
	"black mace",
}

local tier10_armour = {
	"black plate mail body",
	"large black helmet",
	"black plate mail legs",
	"black chain mail body",
	"black square shield",
	"black kite shield",
	"black plated skirt",
	"medium black helmet",
}

for i, item in ipairs(tier10_weapons) do
	register_wearobj(item, function(player)
		return not fail_req(player, STAT_ATTACK, "attack", 10)
	end)
end

for i, item in ipairs(tier10_armour) do
	register_wearobj(item, function(player)
		return not fail_req(player, STAT_DEFENSE, "defense", 10)
	end)
end

register_wearobj("black plate mail top", function(player)
	local fail = false
	if fail_req(player, STAT_DEFENSE, "defense", 10) then
		fail = true
	end
	if fail_body(player) then
		fail = true
	end
	return not fail
end)

local tier20_weapons = {
	"mithril dagger",
	"mithril short sword",
	"mithril long sword",
	"mithril 2-handed sword",
	"mithril scimitar",
	"mithril battle axe",
	"mithril mace",
	"mithril axe",
}

local tier20_armour = {
	"medium mithril helmet",
	"large mithril helmet",
	"mithril chain mail body",
	"mithril plate mail body",
	"mithril plate mail legs",
	"mithril square shield",
	"mithril kite shield",
	"mithril plated skirt",
}

for i, item in ipairs(tier20_weapons) do
	register_wearobj(item, function(player)
		return not fail_req(player, STAT_ATTACK, "attack", 20)
	end)
end

for i, item in ipairs(tier20_armour) do
	register_wearobj(item, function(player)
		return not fail_req(player, STAT_DEFENSE, "defense", 20)
	end)
end

register_wearobj("mithril plate mail top", function(player)
	local fail = false
	if fail_req(player, STAT_DEFENSE, "defense", 20) then
		fail = true
	end
	if fail_body(player) then
		fail = true
	end
	return not fail
end)

local tier30_weapons = {
	"adamantite dagger",
	"adamantite short sword",
	"adamantite long sword",
	"adamantite 2-handed sword",
	"adamantite scimitar",
	"adamantite battle axe",
	"adamantite mace",
	"adamantite axe",
}

local tier30_armour = {
	"medium adamantite helmet",
	"large adamantite helmet",
	"adamantite chain mail body",
	"adamantite plate mail body",
	"adamantite plate mail legs",
	"adamantite square shield",
	"adamantite kite shield",
	"adamantite plated skirt",
}

for i, item in ipairs(tier30_weapons) do
	register_wearobj(item, function(player)
		return not fail_req(player, STAT_ATTACK, "attack", 30)
	end)
end

for i, item in ipairs(tier30_armour) do
	register_wearobj(item, function(player)
		return not fail_req(player, STAT_DEFENSE, "defense", 30)
	end)
end

register_wearobj("adamantite plate mail top", function(player)
	local fail = false
	if fail_req(player, STAT_DEFENSE, "defense", 30) then
		fail = true
	end
	if fail_body(player) then
		fail = true
	end
	return not fail
end)

local tier40_weapons = {
	"rune long sword",
	"rune 2-handed sword",
	"rune battle axe",
	"rune mace",
	"rune dagger",
	"rune short sword",
	"rune scimitar",
	"rune axe",
}

local tier40_armour = {
	"large rune helmet",
	"medium rune helmet",
	"rune chain mail body",
	"rune plate mail body",
	"rune plate mail legs",
	"rune square shield",
	"rune kite shield",
	"rune skirt",
}

for i, item in ipairs(tier40_weapons) do
	register_wearobj(item, function(player)
		return not fail_req(player, STAT_ATTACK, "attack", 40)
	end)
end

for i, item in ipairs(tier40_armour) do
	register_wearobj(item, function(player)
		return not fail_req(player, STAT_DEFENSE, "defense", 40)
	end)
end

register_wearobj("rune plate mail top", function(player)
	local fail = false
	if fail_req(player, STAT_DEFENSE, "defense", 40) then
		fail = true
	end
	if fail_body(player) then
		fail = true
	end
	return not fail
end)

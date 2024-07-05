local function remove_teleport_runes(player, primary_rune_name)
	if not worn(player, "staff of air") then
		remove(player, "air-rune", 3)
	end
	if not worn(player, "staff of " .. primary_rune_name) then
		remove(player, primary_rune_name .. "-rune", 1)
	end
	remove(player, "law-rune", 1)
end

function spellself_varrock_teleport(player)
	-- RSC 2001/replays master archive/Skilling/Magic/spell- law- teleport to varrock
	teleport(player, 120, 504)
	advancestat(player, STAT_MAGIC, 280, 0)
	remove_teleport_runes(player, "fire")
end

function spellself_lumbridge_teleport(player)
	-- RSC 2001/replays master archive/Skilling/Magic/spell- law- teleport to lumbridge
	teleport(player, 120, 648)
	advancestat(player, STAT_MAGIC, 328, 0)
	remove_teleport_runes(player, "earth")
end

function spellself_falador_teleport(player)
	-- RSC 2001/replays master archive/Skilling/Magic/spell- law- teleport to falador
	teleport(player, 312, 552)
	advancestat(player, STAT_MAGIC, 376, 0)
	remove_teleport_runes(player, "water")
end

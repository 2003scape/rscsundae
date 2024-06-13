-- https://classic.runescape.wiki/w/Confuse
-- https://classic.runescape.wiki/w/Weaken
-- https://classic.runescape.wiki/w/Curse

function remove_curse_runes(player)
	if not worn(player, "staff of water") then
		remove(player, "water-rune", 3)
	end
	if not worn(player, "staff of earth") then
		remove(player, "earth-rune", 2)
	end
	remove(player, "body-rune", 1)
end

function skillplayer_confuse(player, target)
	if statdown(target, STAT_ATTACK) then
		mes(player, "Your opponent already has weakened attack")
	else
		mes(player, "@que@Cast spell successfully")
		advancestat(player, STAT_MAGIC, 104, 0)
		remove_curse_runes(player)
		shootplayer(player, target, "dummybolt")
		substat(target, STAT_ATTACK, 0, 5)
		mes(target, "Your attack has been reduced by a confuse spell!")
	end
end

function skillnpc_confuse(player, target)
	if npcstatdown(target, STAT_ATTACK) then
		mes(player, "Your opponent already has weakened attack")
	else
		mes(player, "@que@Cast spell successfully")
		advancestat(player, STAT_MAGIC, 104, 0)
		remove_curse_runes(player)
		shootnpc(player, target, "dummybolt")
		subnpcstat(target, STAT_ATTACK, 0, 5)
	end
end

function skillplayer_weaken(player, target)
	if statdown(target, STAT_STRENGTH) then
		mes(player, "Your opponent already has weakened strength")
	else
		mes(player, "@que@Cast spell successfully")
		advancestat(player, STAT_MAGIC, 168, 0)
		remove_curse_runes(player)
		shootplayer(player, target, "dummybolt")
		substat(target, STAT_STRENGTH, 0, 5)
		mes(target, "Your strength has been reduced by a weaken spell!")
	end
end

function skillnpc_weaken(player, target)
	if npcstatdown(target, STAT_STRENGTH) then
		mes(player, "Your opponent already has weakened strength")
	else
		mes(player, "@que@Cast spell successfully")
		advancestat(player, STAT_MAGIC, 168, 0)
		remove_curse_runes(player)
		shootnpc(player, target, "dummybolt")
		subnpcstat(target, STAT_STRENGTH, 0, 5)
	end
end

function skillplayer_curse(player, target)
	if statdown(target, STAT_DEFENSE) then
		mes(player, "Your opponent already has weakened defence")
	else
		mes(player, "@que@Cast spell successfully")
		advancestat(player, STAT_MAGIC, 232, 0)
		remove_curse_runes(player)
		shootplayer(player, target, "dummybolt")
		substat(target, STAT_DEFENSE, 0, 5)
		mes(target, "Your defense has been reduced by a curse spell!")
	end
end

function skillnpc_curse(player, target)
	if npcstatdown(target, STAT_DEFENSE) then
		mes(player, "Your opponent already has weakened defence")
	else
		mes(player, "@que@Cast spell successfully")
		advancestat(player, STAT_MAGIC, 232, 0)
		remove_curse_runes(player)
		shootnpc(player, target, "dummybolt")
		subnpcstat(target, STAT_DEFENSE, 0, 5)
	end
end

register_skillnpc("_", "confuse", skillnpc_confuse)
register_skillnpc("_", "weaken", skillnpc_weaken)
register_skillnpc("_", "curse", skillnpc_curse)

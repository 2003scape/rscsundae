-- teleport spells have no message to confirm they worked
-- for karamja rum effect, see:
-- RSC 2001/7/iteminfo- karamja rum- teleport from port sarim to falador.pcap
-- TODO: showeffect

local function remove_teleport_runes(player, primary_rune_name)
	if not worn(player, "staff of air") then
		remove(player, "air-rune", 3)
	end
	if not worn(player, "staff of " .. primary_rune_name) then
		remove(player, primary_rune_name .. "-rune", 1)
	end
	remove(player, "law-rune", 1)
end

local function teleport_allowed(player)
	-- flying sno (redacted chat) replays/fsnom2@aol.com/06-19-2018 21.27.32.pcap
	local x, y = playercoord(player)
	if y < 308 then
		mes(player, "A mysterious force blocks your teleport spell!")
		mes(player, "You can't use teleport after level 20 wilderness")
		return false
	end
	return true
end

function spellself_varrock_teleport(player)
	-- RSC 2001/replays master archive/Skilling/Magic/spell- law- teleport to varrock
	if teleport_allowed(player) then
		teleport(player, 120, 504)
		remove(player, "karamja rum", 30)
		advancestat(player, STAT_MAGIC, 280, 0)
		remove_teleport_runes(player, "fire")
	end
end

function spellself_lumbridge_teleport(player)
	-- RSC 2001/replays master archive/Skilling/Magic/spell- law- teleport to lumbridge
	if teleport_allowed(player) then
		teleport(player, 120, 648)
		remove(player, "karamja rum", 30)
		advancestat(player, STAT_MAGIC, 328, 0)
		remove_teleport_runes(player, "earth")
	end
end

function spellself_falador_teleport(player)
	-- RSC 2001/replays master archive/Skilling/Magic/spell- law- teleport to falador
	if teleport_allowed(player) then
		teleport(player, 312, 552)
		remove(player, "karamja rum", 30)
		advancestat(player, STAT_MAGIC, 376, 0)
		remove_teleport_runes(player, "water")
	end
end

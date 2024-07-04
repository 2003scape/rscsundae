-- https://classic.runescape.wiki/w/Black_Knight#Drops
-- https://github.com/2004Scape/Server/blob/main/data/src/scripts/drop%20tables/scripts/black_knight.rs2

function killnpc_black_knight(player, npc, x, y)
	addobject(player, "bones", 1, x, y)
	local ran = randomvar(128)
	if ran < 3 then
		addobject(player, "body-rune", 6, x, y)
		return
	end
	if ran < 6 then
		addobject(player, "chaos-rune", 4, x, y)
		return
	end
	if ran < 9 then
		addobject(player, "earth-rune", 7, x, y)
		return
	end
	if ran < 11 then
		addobject(player, "death-rune", 1, x, y)
		return
	end
	if ran < 13 then
		addobject(player, "law-rune", 2, x, y)
		return
	end
	if ran < 14 then
		addobject(player, "cosmic-rune", 1, x, y)
		return
	end
	if ran < 15 then
		addobject(player, "mind-rune", 1, x, y)
		return
	end
	if ran < 18 then
		randomherb(player, x, y)
		return
	end
	if ran < 21 then
		randomjewel(player, x, y)
		return
	end
	if ran < 58 then
		addobject(player, "coins", 35, x, y)
		return
	end
	if ran < 72 then
		addobject(player, "coins", 12, x, y)
		return
	end
	if ran < 84 then
		addobject(player, "coins", 1, x, y)
		return
	end
	if ran < 96 then
		addobject(player, "coins", 58, x, y)
		return
	end
	if ran < 106 then
		addobject(player, "coins", 6, x, y)
		return
	end
	if ran < 108 then
		addobject(player, "coins", 80, x, y)
		return
	end
	if ran < 113 then
		addobject(player, "steel bar", 1, x, y)
		return
	end
	if ran < 114 then
		addobject(player, "bread", 1, x, y)
		return
	end
	if ran < 115 then
		addobject(player, "flour", 1, x, y)
		return
	end
	if ran < 116 then
		addobject(player, "tin ore", 1, x, y)
		return
	end
	if ran < 120 then
		addobject(player, "iron short sword", 1, x, y)
		return
	end
	if ran < 121 then
		addobject(player, "shortbow", 1, x, y)
		return
	end
	if ran < 122 then
		addobject(player, "steel mace", 1, x, y)
		-- missing return statement is intentional:
		-- ./RSC 2001/replays master archive/NPC (monsters, combat)/Black knight/black knight- jolly boar inn- west- spawn placement
	end
	if ran < 124 then
		addobject(player, "large iron helmet", 1, x, y)
	end
end

-- https://classic.runescape.wiki/w/Wormbrain

function killnpc_goblinmap(player, npc, x, y)
	addobject(player, "bones", 1, x, y)
	addobject(player, "mappart2", 1, x, y)
end

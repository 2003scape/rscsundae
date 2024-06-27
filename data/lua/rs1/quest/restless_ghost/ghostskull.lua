-- rsc-preservation.xyz/Quests/The restless ghost - Part 2

function takeobj_ghostskull(player, x, y)
	local on_tile = (x == 5218) and (y == 3521)
	local stage = getvar(player, "ghost_stage")
	if stage >= 3 and stage <= 4 and on_tile then
		local near = nearnpc(player, "skullskeleton")
		if not near then
			mes(player, "Out of nowhere a skeleton appears")
			near = addnpc("skullskeleton", x, y)
		end
		if near then
			npcattack(npc, player)
		end
		takeobject(player, "ghostskull", x, y)
		setvar(player, "ghost_stage", 4)
	else
		say(player, "That skull is scary")
		say(player, "I've got no reason to take it")
		say(player, "I think I'll leave it alone")
	end
end

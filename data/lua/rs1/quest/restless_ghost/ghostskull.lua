-- rsc-preservation.xyz/Quests/The restless ghost - Part 2

-- XXX the skull was on the floor in RSC's later years, probably due
-- to a bug when they reworked the landscape file format.  here it's
-- on a table and the coordinates are different.

function takeobj_ghostskull(player, x, y)
	local on_tile = (x == 218) and (y == 3520)
	local stage = getvar(player, "ghost_stage")
	if stage >= 3 and stage <= 4 and on_tile and
	    not held(player, "ghostskull", 1) then
		local near = nearnpc(player, "skullskeleton")
		if not near then
			mes(player, "Out of nowhere a skeleton appears")
			near = addnpc("skullskeleton", x, y + 1)
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

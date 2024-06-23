-- rsc-preservation.xyz/Quests/sheep-shearer-zezima

function useloc_spinning_wheel_wool(player, x, y)
	thinkbubble(player, "wool")
	mes(player, "You spin the sheeps wool into a nice ball of wool")
	remove(player, "wool", 1)
	give(player, "ball of wool", 1)
	advancestat(player, STAT_CRAFTING, 10, 0)
end

register_useloc("spinning wheel", "wool", useloc_spinning_wheel_wool)

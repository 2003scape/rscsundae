-- https://classic.runescape.wiki/w/Chest_(Melzar's_Maze)

function oploc1_funchestopen(player, x, y)
	if
		getvar(player, "dragon_funchest") ~= 0
		and not held(player, "mappart1", 1)
	then
		mes(player, "You find a piece of map in the chest")
		give(player, "mappart1", 1)
		setvar(player, "dragon_funchest", 0)
	else
		mes(player, "You find nothing in the chest")
	end
end

function oploc2_funchestopen(player, x, y)
	mes(player, "@que@You close the chest")
	changeloc(x, y, "funchestshut")
end

function oploc1_funchestshut(player, x, y)
	mes(player, "@que@You open the chest")
	changeloc(x, y, "funchestopen")
end

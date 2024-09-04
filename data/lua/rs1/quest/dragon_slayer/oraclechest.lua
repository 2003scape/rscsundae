-- https://classic.runescape.wiki/w/Chest_(Dwarven_Mine)

function oploc1_oraclechestopen(player, x, y)
	if
		getvar(player, "dragon_oraclechest") ~= 0
		and not held(player, "mappart3", 1)
	then
		mes(player, "You find a piece of map in the chest")
		give(player, "mappart3", 1)
		setvar(player, "dragon_oraclechest", 0)
    else
        mes(player, "You find nothing in the chest")
	end
end

function oploc2_oraclechestopen(player, x, y)
    mes(player, "@que@You close the chest")
    changeloc(x, y, "oraclechestshut")
end

function oploc1_oraclechestshut(player, x, y)
    mes(player, "@que@You open the chest")
    changeloc(x, y, "oraclechestopen")
end

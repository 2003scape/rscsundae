-- ./RSC 2001/replays master archive/Scenery/scenery- potato- pick potatos
function oploc2_potato(player, x, y)
	mes(player, "@que@You pick a potato")
	give(player, "potato", 1)
end

-- ./RSC 2001/replays master archive/Scenery/scenery- grain -  lumb windmill field picking for wheat- lumb windmill field (no despawn time)
function oploc2_wheat(player, x, y)
	mes(player, "@que@You get some grain")
	give(player, "grain", 1)
end

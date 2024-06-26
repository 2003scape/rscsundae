function oploc1_ladderup(player, x, y)
	mes(player, "@que@You climb up the ladder")
	changelevelup(player)
end

function oploc1_ladderdown(player, x, y)
	mes(player, "@que@You climb down the ladder")
	changeleveldown(player)
end

function oploc1_woodenstairsup(player, x, y)
	mes(player, "@que@You climb up the stairs");
	upstairs(player, x, y)
end

function oploc1_woodenstairsdown(player, x, y)
	mes(player, "@que@You climb down the stairs");
	downstairs(player, x, y)
end

function oploc1_stonestairsup(player, x, y)
	mes(player, "@que@You climb up the stairs");
	upstairs(player, x, y)
end

function oploc1_stonestairsdown(player, x, y)
	mes(player, "@que@You climb down the stairs");
	downstairs(player, x, y)
end

-- RSC 2001/3/f2p (not 100% complete)/walkaround- varrock- sewers- entrance chamber
function oploc1_manholeopen(player, x, y)
	mes(player, "@que@You climb down the ladder")
	changeleveldown(player)
end

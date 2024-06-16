function oploc1_ladderup(player, x, y)
	mes(player, "@que@You climb up the ladder")
	upstairs(player)
end

function oploc1_ladderdown(player, x, y)
	mes(player, "@que@You climb down the ladder")
	downstairs(player)
end

function oploc1_woodenstairsup(player, x, y)
	mes(player, "@que@You climb up the stairs");
	upstairs(player)
end

function oploc1_woodenstairsdown(player, x, y)
	mes(player, "@que@You climb down the stairs");
	downstairs(player)
end

function oploc1_stonestairsup(player, x, y)
	mes(player, "@que@You climb up the stairs");
	upstairs(player)
end

function oploc1_stonestairsdown(player, x, y)
	mes(player, "@que@You climb down the stairs");
	downstairs(player)
end

-- RSC 2001/3/f2p (not 100% complete)/walkaround- varrock- sewers- entrance chamber
function oploc1_manholeopen(player, x, y)
	mes(player, "@que@You climb down the ladder")
	downstairs(player)
end

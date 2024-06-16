function opbound1_normdoor(player, x, y, dir)
	mes(player, "@que@The door swings open")
	changebound(x, y, dir, "normdoorframe")
	delay(4)
end

function opbound2_normdoorframe(player, x, y, dir)
	mes(player, "@que@The door creaks shut")
	changebound(x, y, dir, "normdoor")
	delay(4)
end

function opbound1_highdoor(player, x, y, dir)
	mes(player, "@que@The door swings open")
	changebound(x, y, dir, "highdoorframe")
	delay(4)
end

function opbound2_highdoorframe(player, x, y, dir)
	mes(player, "@que@The door creaks shut")
	changebound(x, y, dir, "highdoor")
	delay(4)
end

-- ./RSC 2001/3/f2p (not 100% complete)/scenery- draynor manor- front- doors (double)
function oploc2_doubledoorsopen(player, x, y)
	mes(player, "@que@The doors creak shut")
	changeloc(x, y, "doubledoorsclosed")
	delay(4)
end

function oploc1_doubledoorsclosed(player, x, y)
	mes(player, "@que@The doors swing open")
	changeloc(x, y, "doubledoorsopen")
	delay(4)
end

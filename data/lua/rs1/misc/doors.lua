function opbound1_normdoor(player, x, y, dir)
	mes(player, "@que@The door swings open")
	changebound(x, y, dir, "normdoorframe")
end

function opbound2_normdoorframe(player, x, y, dir)
	mes(player, "@que@The door creaks shut")
	changebound(x, y, dir, "normdoor")
end

function opbound1_highdoor(player, x, y, dir)
	mes(player, "@que@The door swings open")
	changebound(x, y, dir, "highdoorframe")
end

function opbound2_highdoorframe(player, x, y, dir)
	mes(player, "@que@The door creaks shut")
	changebound(x, y, dir, "highdoor")
end

function opbound1_normdoor(player, name, x, y, dir)
	changebound(x, y, dir, "normdoorframe")
end

function opbound2_normdoorframe(player, name, x, y, dir)
	changebound(x, y, dir, "normdoor")
end

function opbound1_highdoor(player, name, x, y, dir)
	changebound(x, y, dir, "highdoorframe")
end

function opbound2_highdoorframe(player, name, x, y, dir)
	changebound(x, y, dir, "highdoor")
end

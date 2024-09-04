-- https://classic.runescape.wiki/w/Door_(Elvarg)

function opbound1_dragondoor(player, x, y, dir)
	local player_x = playercoord(player)

    -- TODO not sure if you could leave the door after entering
	if player_x >= x or getvar(player, "dragon_stage") == 3 then
		mes(player, "@que@")
		boundaryteleport(player, x, y, dir)
		changebound(x, y, dir, "openlockeddoor")
		pause(3, 3)
		changebound(x, y, dir, "dragondoor")
	else
		mes(player, "@que@the door is locked")
	end
end

-- https://classic.runescape.wiki/w/Strange_looking_wall

function opbound1_strange_looking_wall(player, x, y, dir)
	local _, player_y = playercoord(player)
	local secret_passage = getvar(player, "dragon_secret_passage") ~= 0

	if player_y >= y then
		if secret_passage then
			mes(player, "You just went through a secret door")
			boundaryteleport(player, x, y, dir)
			changebound(x, y, dir, "openlockeddoor")
			pause(3, 3)
			changebound(x, y, dir, "strange looking wall")
		else
			mes(player, "nothing interesting happens")
		end
	else
		mes(player, "You just went through a secret door")
		boundaryteleport(player, x, y, dir)
		changebound(x, y, dir, "openlockeddoor")

		if not secret_passage then
			mes(player, "You remember where the door is for future use")
			setvar(player, "dragon_ned_in_ship", 0)
			setvar(player, "dragon_secret_passage", 1)
		end

		pause(3, 3)
		changebound(x, y, dir, "strange looking wall")
	end
end

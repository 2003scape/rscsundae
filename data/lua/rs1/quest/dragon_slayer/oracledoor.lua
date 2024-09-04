-- https://classic.runescape.wiki/w/Door_(Oracle_Map_Piece)

local key_items =
	{ "silk", "unfired bowl", "lobster pot", "wizard's mind bomb" }

function opbound1_oracledoor(player, x, y, dir)
	local function enter_door()
		mes(player, "@que@")
		boundaryteleport(player, x, y, dir)
		changebound(x, y, dir, "openlockeddoor")
		pause(3, 3)
		changebound(x, y, dir, "oracledoor")
	end

	local player_x = playercoord(player)

	if player_x < x then
		enter_door()
	else
		local unlocked = false

		if getvar(player, "dragon_stage") == 2 then
			unlocked = true

			for _, item in ipairs(key_items) do
				if not held(player, item, 1) then
					unlocked = false
					break
				end
			end
		end

		if unlocked then
			for _, item in ipairs(key_items) do
				remove(player, item, 1)
			end

			setvar(player, "dragon_oraclechest", 1)
			enter_door()
		else
			mes(player, "@que@the door is locked")
		end
	end
end

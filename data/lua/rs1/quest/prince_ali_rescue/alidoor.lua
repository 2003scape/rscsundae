-- https://classic.runescape.wiki/w/Door_(Draynor_Village_jail)

local function enter_alidoor(player, x, y, dir)
	mes(player, "You go through the door")
	boundaryteleport(player, x, y, dir)
	changebound(x, y, dir, "openlockeddoor")
	pause(3, 3)
	changebound(x, y, dir, "alidoor")
end

local function check_keli(player)
	local keli = nearnpc(player, "lady keli")

	if keli then
		mes(player, "You'd better get rid of Lady Keli before trying to go through there")

		return true
	end

	return false
end

function opbound1_alidoor(player, x, y, dir)
	local player_x = playercoord(player)

	if player_x < x then
		if not check_keli(player) then
			mes(player, "@que@The door is locked")

			if held(player, "princeskey", 1) then
				mes(player, "Maybe you should try using your key on it")
			end
		end
	else
		enter_alidoor(player, x, y, dir)
	end
end

function usebound_alidoor_princeskey(player, x, y, dir)
	local stage = getvar(player, "prince_ali_stage")
	local player_x = playercoord(player)

	if player_x < x then
		if not check_keli(player) then
			mes(player, "You unlock the door")
			enter_alidoor(player, x, y, dir)
		end
	else
		enter_alidoor(player, x, y, dir)
	end
end

register_usebound("alidoor", "princeskey", usebound_alidoor_princeskey)

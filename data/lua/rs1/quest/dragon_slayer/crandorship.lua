-- https://classic.runescape.wiki/w/Lumbridge_Lady

local function enter_ship(player)
	local ned_in_ship = getvar(player, "dragon_ned_in_ship") ~= 0

	if getvar(player, "dragon_ship_fix_stage") == 3 then
		if ned_in_ship then
			teleport(player, 281, 3493)
		else
			teleport(player, 259, 3493)
		end
	else
		if ned_in_ship then
			teleport(player, 281, 3472)
		else
			teleport(player, 259, 3472)
		end
	end
end

-- port sarim location
function oploc1_crandorship(player, x, y)
	if getvar(player, "dragon_owns_ship") ~= 0 then
		setvar(player, "dragon_ship_crandor", 0)
		enter_ship(player)
	else
		local klarense = nearnpc(player, "klarense")

		if klarense then
            npcbusy(klarense)
			talknpc_klarense(player, klarense)
            npcunbusy(klarense)
		else
			mes(player, "You must talk to the owner about this.")
		end
	end
end

-- crandor location
function oploc1_crandorship2(player, x, y)
	setvar(player, "dragon_ship_crandor", 1)
	enter_ship(player)
end

function oploc1_shipladder(player, x, y)
	mes(player, "@que@You leave the ship")

	if getvar(player, "dragon_ship_crandor") ~= 0 then
		teleport(player, 409, 640)
	else
		teleport(player, 259, 641)
	end
end

function useloc_shiphole_plank(player, x, y)
	if getvar(player, "dragon_ship_crandor") ~= 0 then
		mes(player, "The ship doesn't seem easily repairable at the moment")
	elseif getvar(player, "dragon_secret_passage") ~= 0 then
		mes(player, "You don't need to mess about with broken ships")
		mes(player, "Now you have found that secret passage from Karamja")
	elseif not held(player, "nails", 4) then
		mes(player, "You need 4 steel nails to attach the plank with")
	elseif not held(player, "hammer", 1) then
		mes(player, "You need a hammer to hammer the nails in with")
	else
		mes(player, "You hammer the plank over the hole")
		remove(player, "plank", 1)
		remove(player, "nails", 4)

		local fix_stage = getvar(player, "dragon_ship_fix_stage") + 1
		setvar(player, "dragon_ship_fix_stage", fix_stage)

		if fix_stage == 3 then
			mes(player, "You board up the hole in the ship")
            enter_ship(player)
		else
			mes(
				player,
				"You still need more planks to close the hole completely"
			)
		end
	end
end

register_useloc("shiphole", "plank", useloc_shiphole_plank)
register_useloc("shiphole2", "plank", useloc_shiphole_plank)

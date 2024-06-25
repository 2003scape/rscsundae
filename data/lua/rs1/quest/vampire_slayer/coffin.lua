local function vampire_wake_up(player)
	-- https://youtu.be/U3_ci_WbF3Q?t=436
	-- count draynor jumps out of the coffin when you open OR search it

	-- TODO do not allow duplicating vampire
	addnpc("vampire", 205, 3382)
	mes(player, "A vampire jumps out of the coffin")
end

function oploc1_vampshutcoffin(player, x, y)
	local stage = getvar(player, "vampire_stage")

	mes(player, "You open the coffin")
	changeloc(x, y, "vampopencoffin")

	if stage == 1 then
		vampire_wake_up(player)
	end
end

function oploc1_vampopencoffin(player, x, y)
	local stage = getvar(player, "vampire_stage")

	if stage == 1 then
		vampire_wake_up(player)
	else
		mes(player, "There's a pillow in here")
	end
end

function oploc2_vampopencoffin(player, x, y)
	mes(player, "You close the coffin")
	changeloc(x, y, "vampshutcoffin")
end

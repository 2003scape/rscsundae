local levers = { "a", "b", "c", "d", "e", "f" }

local door_combinations = {
	["1to2"] = { down = { "d", "f" }, up = { "a", "b" } },
	["2to3"] = { down = { "d", "f" }, up = { "a", "b" } },
	["4to5"] = { down = { "a", "b", "d" }, up = { "c", "e", "f" } },
	["5to6"] = { down = { "d" }, up = { "a", "b", "c", "e", "f" } },
	["8to9"] = { down = { "c", "d", "f" }, up = { "a", "b", "e" } },
	["2to5"] = { down = { "d", "f" }, up = { "a", "b", "e" } },
	["3to6"] = { down = { "d" }, up = { "a", "b", "c", "e", "f" } },
	["4to7"] = { down = { "a", "b" }, up = { "c", "d", "e", "f" } },
	["5to8"] = { down = { "d" }, up = {} },
}

local function toggle_lever(player, letter)
	local key = "ernest_lever_" .. letter
	local pulled_down = getvar(player, key) == 1
	setvar(player, key, pulled_down and 0 or 1)
	return not pulled_down
end

for _, letter in ipairs(levers) do
	register_oploc1("lever" .. letter, function(player, x, y)
		local position = toggle_lever(player, letter)

		mes(
			player,
			"You pull lever "
				.. string.upper(letter)
				.. " "
				.. (position and "down" or "up")
		)

		mes(player, "@que@you hear a clunk")
	end)

	register_oploc2("lever" .. letter, function(player, x, y)
		local position = getvar(player, "ernest_lever_" .. letter) == 1
		mes(player, "The lever is " .. (position and "down" or "up"))
	end)
end

for door, combinations in pairs(door_combinations) do
	register_opbound1(door, function(player, x, y, dir)
		local locked = false

		for _, letter in ipairs(combinations.up) do
			if getvar(player, "ernest_lever_" .. letter) == 1 then
				locked = true
				break
			end
		end

		if not locked then
			for _, letter in ipairs(combinations.down) do
				if getvar(player, "ernest_lever_" .. letter) == 0 then
					locked = true
					break
				end
			end
		end

		if locked then
			mes(player, "@que@The door is locked")
		else
			mes(player, "@que@The door swings open")
			mes(player, "@que@You go through the door")
			boundaryteleport(player, x, y, dir)
			changebound(x, y, dir, "openlockeddoor")
			pause(3, 3)
			changebound(x, y, dir, door)
		end
	end)
end

function oploc1_puzzladderup(player, x, y)
	for _, letter in ipairs(levers) do
		setvar(player, "ernest_lever_" .. letter, 0)
	end

	mes(player, "@que@You climb up the ladder")
	changelevelup(player, x, y)
end

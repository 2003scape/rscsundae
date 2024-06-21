-- https://classic.runescape.wiki/w/Dyes
-- rscplus-replay: Revisionism\06-15-2018 04.23.22 goblin diplomacy

-- mixing dyes
local function create_dye_mix_handler(dye1, dye2, dye_result, dye_name)
	 local function handler(player)
		  -- "You mix the two dyes and make an orange dye"
		  mes(player, "You mix the two dyes and make " .. dye_name)
		  remove(player, dye1, 1)
		  remove(player, dye2, 1)
		  give(player, dye_result, 1)
	 end

	 register_useinv(dye1, dye2, handler)
end

create_dye_mix_handler("reddye", "yellowdye", "orangedye", "an orange dye")
create_dye_mix_handler("reddye", "bluedye", "purpledye", "a purple dye")
create_dye_mix_handler("yellowdye", "bluedye", "greendye", "a green dye")

-- dying capes
local function create_dye_cape_handler(dye, cape, cape_result,
    cape_name, dye_name)
	 local function handler(player)
		  -- "You dye the black cape red"
		  mes(player, "You dye the " .. cape_name .. " " .. dye_name)
		  remove(player, dye, 1)
		  remove(player, cape, 1)
		  give(player, cape_result, 1)
		  advancestat(player, STAT_CRAFTING, 10, 0)
	 end

	 register_useinv(dye, cape, handler)
end

local colors = { "red", "yellow", "orange", "blue", "green", "purple" }
local capes = {
	 blackcape = "black cape",
	 redcape = "red cape",
	 yellowcape = "yellow cape",
	 orangecape = "orange cape",
	 bluecape = "blue cape",
	 greencape = "green cape",
	 purplecape = "purple cape",
}

for _, color in pairs(colors) do
	 for item_name, cape_name in pairs(capes) do
		  create_dye_cape_handler(color .. "dye", item_name, color .. "cape",
				cape_name, color)
	 end
end

-- https://classic.runescape.wiki/w/Map_(Dragon_slayer)
-- https://classic.runescape.wiki/w/Map_Piece

local function combine_map(player)
	if held(player, "mappart1", 1)
	    and held(player, "mappart2", 1)
	    and held(player, "mappart3", 1) then
		mes(player, "You put all the pieces of map together")
		remove(player, "mappart1", 1)
		remove(player, "mappart2", 1)
		remove(player, "mappart3", 1)
		give(player, "map", 1)
	else
		mes(player, "You still need one more piece of map")
	end
end

register_useinv("mappart1", "mappart2", combine_map)
register_useinv("mappart2", "mappart3", combine_map)
register_useinv("mappart1", "mappart3", combine_map)

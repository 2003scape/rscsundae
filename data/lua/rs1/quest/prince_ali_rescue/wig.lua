-- https://classic.runescape.wiki/w/Wig

register_useinv("yellowdye", "plainwig", function(player)
	remove(player, "yellowdye", 1)
	remove(player, "plainwig", 1)
	give(player, "blondwig", 1)
	mes(player, "You dye the wig blond")
end)

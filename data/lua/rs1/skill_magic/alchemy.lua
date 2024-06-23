-- Logg/Tylerbeg/06-13-2018 20.09.59 high alch from 55 to 60 and I got a dmed lol

-- TODO: onobj message, "You must be holding the object to use this spell"

function low_alchemy(player, item)
	mes(player, "@que@Alchemy spell successful")
	sellinv(player, item, 40)
	advancestat(player, STAT_MAGIC, 248, 0)
	if not worn(player, "staff of fire") then
		remove(player, "fire-rune", 3)
	end
	remove(player, "nature-rune", 1)
end

function high_alchemy(player, item)
	mes(player, "@que@Alchemy spell successful")
	sellinv(player, item, 60)
	advancestat(player, STAT_MAGIC, 520, 0)
	if not worn(player, "staff of fire") then
		remove(player, "fire-rune", 5)
	end
	remove(player, "nature-rune", 1)
end

register_spellinv("_", "low level alchemy", low_alchemy)
register_spellinv("_", "high level alchemy", high_alchemy)

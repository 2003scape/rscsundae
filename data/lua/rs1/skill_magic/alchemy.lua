-- Logg/Tylerbeg/06-13-2018 20.09.59 high alch from 55 to 60 and I got a dmed lol

register_spellinv("_", "low level alchemy", function(player, item)
	mes(player, "@que@Alchemy spell successful")
	sellinv(player, item, 40)
	advancestat(player, STAT_MAGIC, 248, 0)
	if not worn(player, "staff of fire") then
		remove(player, "fire-rune", 3)
	end
	remove(player, "nature-rune", 1)
end)

register_spellinv("_", "high level alchemy", function(player, item)
	mes(player, "@que@Alchemy spell successful")
	sellinv(player, item, 60)
	advancestat(player, STAT_MAGIC, 520, 0)
	if not worn(player, "staff of fire") then
		remove(player, "fire-rune", 5)
	end
	remove(player, "nature-rune", 1)
end)

register_spellobj("_", "low level alchemy", function(player, item, x, y)
	mes(player, "You must be holding the object to use this spell")
end)

register_spellobj("_", "high level alchemy", function(player, item, x, y)
	mes(player, "You must be holding the object to use this spell")
end)

-- https://classic.runescape.wiki/w/Bones
-- https://www.youtube.com/watch?v=9YcA9QMOHIM
-- https://www.youtube.com/watch?v=2UaPGTyXn9I

function bury_bones(player)
	mes(player, "@que@You dig a hole in the ground")
	delay(1)
	mes(player, "@que@You bury the bones")
	advancestat(player, STAT_PRAYER, 15, 0)
	remove(player, "bones", 1)
end

function bury_bigbones(player)
	-- big bones just say "bones" but there's a new grammar mistake
	mes(player, "@que@you dig a hole in the ground")
	delay(1)
	mes(player, "@que@You bury the bones")
	advancestat(player, STAT_PRAYER, 50, 0)
	remove(player, "big bones", 1)
end

register_useobj("bones", bury_bones)
register_useobj("big bones", bury_bigbones)
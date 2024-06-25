-- https://classic.runescape.wiki/w/Evil_Tree
-- RSC 2001/replays master archive/Scenery/scenery- draynor- tree- approachable- search from 99 hp to below 10 hp (full cycle) (99-99-99) (~217armr)

function oploc1_deadtree2(player, x, y)
	mes(player, "@que@The tree seems to lash out at you!")
	delay(1)
	mes(player, "@que@You are badly scratched by the tree")
	substat(player, STAT_HITS, 0, 20)
end

function oploc2_deadtree2(player, x, y)
	oploc1_deadtree2(player, x, y)
end

register_useloc("deadtree2", "bronze axe", oploc1_deadtree2)
register_useloc("deadtree2", "iron axe", oploc1_deadtree2)
register_useloc("deadtree2", "steel axe", oploc1_deadtree2)
register_useloc("deadtree2", "black axe", oploc1_deadtree2)
register_useloc("deadtree2", "mithril axe", oploc1_deadtree2)
register_useloc("deadtree2", "adamantite axe", oploc1_deadtree2)
register_useloc("deadtree2", "rune axe",  oploc1_deadtree2)

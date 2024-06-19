-- ./RSC 2001/5/woodcutting- regular tree- level 1 to 2- rune axe.pcap
-- https://oldschool.runescape.wiki/w/Tree?oldid=14670344#Cut_chance[2]
-- messages from grepping pcaps

local function do_woodcut(player, x, y, success)
	delay(3)
	if success then
		mes(player, "@que@You get some wood")
		give(player, "logs", 1)
		advancestat(player, STAT_WOODCUTTING, 100, 7)
		changeloc(x, y, "treestump")
		restoreloc(x, y, 100)
	else
		mes(player, "@que@You slip and fail to hit the tree")
	end
end

function useloc_tree_bronze_axe(player, x, y)
	thinkbubble(player, "bronze axe")
	mes(player, "@que@You swing your bronze axe at the tree...")
	do_woodcut(player, x, y,
		statrandom(player, STAT_WOODCUTTING, 64, 200))
end

function useloc_tree_iron_axe(player, x, y)
	thinkbubble(player, "iron axe")
	mes(player, "@que@You swing your iron axe at the tree...")
	do_woodcut(player, x, y,
		statrandom(player, STAT_WOODCUTTING, 96, 300))
end

function useloc_tree_steel_axe(player, x, y)
	thinkbubble(player, "steel axe")
	mes(player, "@que@You swing your steel axe at the tree...")
	do_woodcut(player, x, y,
		statrandom(player, STAT_WOODCUTTING, 128, 400))
end

function useloc_tree_black_axe(player, x, y)
	thinkbubble(player, "black axe")
	mes(player, "@que@You swing your black axe at the tree...")
	do_woodcut(player, x, y,
		statrandom(player, STAT_WOODCUTTING, 144, 450))
end

function useloc_tree_mithril_axe(player, x, y)
	thinkbubble(player, "mithril axe")
	mes(player, "@que@You swing your mithril axe at the tree...")
	do_woodcut(player, x, y,
		statrandom(player, STAT_WOODCUTTING, 160, 500))
end

function useloc_tree_adamantite_axe(player, x, y)
	thinkbubble(player, "adamantite axe")
	mes(player, "@que@You swing your adamantite axe at the tree...")
	do_woodcut(player, x, y,
		statrandom(player, STAT_WOODCUTTING, 192, 600))
end

function useloc_tree_rune_axe(player, x, y)
	thinkbubble(player, "rune axe")
	mes(player, "@que@You swing your rune axe at the tree...")
	do_woodcut(player, x, y,
		statrandom(STAT_WOODCUTTING, 224, 700))
end

-- later client versions (after 2002 or so) have one-click woodcutting
function oploc1_tree(player, x, y)
	-- more powerful axes seem to be preferred when the player has
	-- multiple types held, see
	-- ./RSC 2001/replays master archive/Skilling/Woodcutting/Regular trees/woodcutting- skilling- regular trees, steel axe- 36 wc.pcap
	if held(player, "rune axe", 1) then
		useloc_tree_rune_axe(player, x, y)
	elseif held(player, "adamantite axe", 1) then
		useloc_tree_adamantite_axe(player, x, y)
	elseif held(player, "mithril axe", 1) then
		useloc_tree_mithril_axe(player, x, y)
	elseif held(player, "black axe", 1) then
		useloc_tree_black_axe(player, x, y)
	elseif held(player, "steel axe", 1) then
		useloc_tree_steel_axe(player, x, y)
	elseif held(player, "iron axe", 1) then
		useloc_tree_iron_axe(player, x, y)
	elseif held(player, "bronze axe", 1) then
		useloc_tree_bronze_axe(player, x, y)
	else
		-- pcaps/Logg/Loggykins/08-03-2018 21.58.13 lumbridge with tylerbeg.pcap
		mes(player, "@que@You need an axe to chop this tree down")
	end
end

register_useloc("tree", "bronze axe", useloc_tree_bronze_axe)
register_useloc("tree", "iron axe", useloc_tree_iron_axe)
register_useloc("tree", "steel axe", useloc_tree_steel_axe)
register_useloc("tree", "black axe", useloc_tree_black_axe)
register_useloc("tree", "mithril axe", useloc_tree_mithril_axe)
register_useloc("tree", "adamantite axe", useloc_tree_adamantite_axe)
register_useloc("tree", "rune axe", useloc_tree_rune_axe)

--
-- success/failure rate analysis
--
-- level 1, bronze axe: 15 failed 2 success
-- 225/256 chance to fail
-- [./RSC 2001/replays master archive/Skilling/Woodcutting/Regular trees/woodcutting- skilling- regular trees, bronze axe- 1wc - level up to 2 wc.pcap]
--
-- level 2, bronze axe: 5 failed 3 success
-- 160/256 chance to fail
-- [./RSC 2001/replays master archive/Skilling/Woodcutting/Regular trees/woodcutting- skilling- regular trees, bronze axe- 2wc  -level up to 3 wc.pcap]
--
-- level 4, bronze axe: 8 failed 4 success
-- 168/256 chance to fail
-- OSRS has 187/256
-- [./RSC 2001/replays master archive/Skilling/Woodcutting/Regular trees/woodcutting- skilling- regular trees, bronze axe- 4wc - level up to 5 wc.pcap]
--
-- level 16, steel axe: 3 failed 9 success
-- 63/256 chance to fail
-- OSRS has 85/256
-- [./Revisionism/06-12-2018 17.53.58 woodcut in draynor with Steel Axe and Iron Axe.pcap]
--
-- level 36, steel axe: 1 failed 10 success
-- 23/256 chance to fail
-- OSRS has 30/256
-- [./RSC 2001/replays master archive/Skilling/Woodcutting/Regular trees/woodcutting- skilling- regular trees, steel axe- 36 wc.pcap]
--
-- level 1, rune axe: 0 failed 4 success
-- 0/0 chance to fail
-- OSRS has 31/256
-- [./RSC 2001/5/woodcutting- regular tree- level 2 to 1- rune axe.pcap]
-- never seen a failure with rune axe at any level
--
-- conclusions: rates are the same as OSRS, but pre-fletching may have
-- been different
--

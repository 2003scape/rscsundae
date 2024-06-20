local function do_prospect(player, ore)
	mes(player, "@que@You examine the rock for ores...")
	delay(1)
	mes(player, string.format("@que@This rock contains %s", ore))
end

-- https://classic.runescape.wiki/w/Transcript:Rock_(uninteresting)
function oploc2_plainrock1(player, x, y)
	mes(player, "@que@You examine the rock for ores...")
	delay(1)
	mes(player, "@que@You fail to find anything interesting")
end

function oploc2_plainrock2(player, x, y)
	oploc2_plainrock1(player, x, y)
end

-- RSC 2001/replays master archive/Skilling/Mining/mining- prospect- tin
function oploc2_tinrock1(player, x, y)
	do_prospect(player, "tin ore")
end

function oploc2_tinrock2(player, x, y)
	do_prospect(player, "tin ore")
end

function oploc2_tinrock3(player, x, y)
	do_prospect(player, "tin ore")
end

-- RSC 2001/replays master archive/Skilling/Mining/mining- prospect- copper
function oploc2_copperrock1(player, x, y)
	do_prospect(player, "copper ore")
end

function oploc2_copperrock2(player, x, y)
	do_prospect(player, "copper ore")
end

function oploc2_copperrock3(player, x, y)
	do_prospect(player, "copper ore")
end

function oploc2_ironrock1(player, x, y)
	do_prospect(player, "iron ore")
end

function oploc2_ironrock2(player, x, y)
	do_prospect(player, "iron ore")
end

function oploc2_ironrock3(player, x, y)
	do_prospect(player, "iron ore")
end

-- RSC 2001/replays master archive/Skilling/Mining/mining- prospect- coal- mining Lvl99
function oploc2_coalrock1(player, x, y)
	do_prospect(player, "coal")
end

function oploc2_coalrock2(player, x, y)
	do_prospect(player, "coal")
end

function oploc2_coalrock3(player, x, y)
	do_prospect(player, "coal")
end

-- RSC 2001/replays master archive/Skilling/Mining/mining- prospect- silver- mining Lvl99
function oploc2_silverrock1(player, x, y)
	do_prospect(player, "silver")
end

function oploc2_silverrock2(player, x, y)
	do_prospect(player, "silver")
end

function oploc2_silverrock3(player, x, y)
	do_prospect(player, "silver")
end

-- RSC 2001/replays master archive/Skilling/Mining/mining- prospect- clay- mining Lvl99
function oploc2_clayrock1(player, x, y)
	do_prospect(player, "clay")
end

function oploc2_clayrock2(player, x, y)
	do_prospect(player, "clay")
end

function oploc2_clayrock3(player, x, y)
	do_prospect(player, "clay")
end

-- RSC 2001/replays master archive/Skilling/Mining/mining- prospect- gold- mining Lvl99
function oploc2_goldrock1(player, x, y)
	do_prospect(player, "gold!")
end

function oploc2_goldrock2(player, x, y)
	do_prospect(player, "gold!")
end

function oploc2_goldrock3(player, x, y)
	do_prospect(player, "gold!")
end

-- RSC 2001/replays master archive/Skilling/Mining/mining- prospect- mithril= mining Lvl99
function oploc2_mithrilrock1(player, x, y)
	do_prospect(player, "mithril ore!")
end

function oploc2_mithrilrock2(player, x, y)
	do_prospect(player, "mithril ore!")
end

function oploc2_mithrilrock3(player, x, y)
	do_prospect(player, "mithril ore!")
end

-- RSC 2001/replays master archive/Skilling/Mining/mining- prospect- adamantite- mining Lvl99
function oploc2_adamantiterock1(player, x, y)
	do_prospect(player, "adamantite ore!")
end

function oploc2_adamantiterock2(player, x, y)
	do_prospect(player, "adamantite ore!")
end

function oploc2_adamantiterock3(player, x, y)
	do_prospect(player, "adamantite ore!")
end

-- RSC 2001/replays master archive/Skilling/Mining/mining- prospect- runite- mining Lvl99
function oploc2_runiterock1(player, x, y)
	do_prospect(player, "runite ore!")
end

function oploc2_runiterock2(player, x, y)
	do_prospect(player, "runite ore!")
end

function oploc2_runiterock3(player, x, y)
	do_prospect(player, "runite ore!")
end

-- RSC 2001/3/f2p (not 100% complete)/mining- prospect- blurite rock
function oploc2_bluriterock1(player, x, y)
	do_prospect(player, "blurite ore")
end

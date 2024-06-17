-- RSC 2001/replays master archive/Item info/Item type/Cooking - Food/iteminfo- jug - filling at lumb castle.pcap
-- RSC 2001/replays master archive/Scenery/scenery- sink- use bowl on sink- success- fill bowl.pcap
-- RSC 2001/replays master archive/Scenery/scenery- sink- use jug on sink- success- fill jug.pcap
-- Logg/Tylerbeg/08-02-2018 03.50.02 barbarian village and edgeville.pcap

function useloc_sink_jug(player, x, y)
	thinkbubble(player, "jug")
	remove(player, "jug", 1)
	mes(player, "@que@You fill the jug from the sink")
	give(player, "waterjug", 1)
end

function useloc_sink_bowl(player, x, y)
	thinkbubble(player, "bowl")
	remove(player, "bowl", 1)
	mes(player, "@que@You fill the bowl from the sink")
	give(player, "bowl of water", 1)
end

function useloc_sink_bucket(player, x, y)
	thinkbubble(player, "bucket")
	remove(player, "bucket", 1)
	mes(player, "@que@You fill the bucket from the sink")
	give(player, "waterbucket", 1)
end

function useloc_fountain_jug(player, x, y)
	thinkbubble(player, "jug")
	remove(player, "jug", 1)
	mes(player, "@que@You fill the jug from the fountain")
	give(player, "waterjug", 1)
end

function useloc_fountain_bowl(player, x, y)
	thinkbubble(player, "bowl")
	remove(player, "bowl", 1)
	mes(player, "@que@You fill the bowl from the fountain")
	give(player, "bowl of water", 1)
end

function useloc_fountain_bucket(player, x, y)
	thinkbubble(player, "bucket")
	remove(player, "bucket", 1)
	mes(player, "@que@You fill the bucket from the fountain")
	give(player, "waterbucket", 1)
end

function useloc_well_bucket(player, x, y)
	thinkbubble(player, "bucket")
	remove(player, "bucket", 1)
	mes(player, "@que@You fill the bucket from the well")
	give(player, "waterbucket", 1)
end

register_useloc("sink", "jug", useloc_sink_jug)
register_useloc("sink", "bowl", useloc_sink_bowl)
register_useloc("sink", "bucket", useloc_sink_bucket)
register_useloc("fountain", "jug", useloc_fountain_jug)
register_useloc("fountain", "bowl", useloc_fountain_bowl)
register_useloc("fountain", "bucket", useloc_fountain_bucket)
register_useloc("well", "bucket", useloc_well_bucket)

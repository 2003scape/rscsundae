-- RSC 2001/replays master archive/Item info/Alphabetical/S/iteminfo- scenery- drain- silverlight key 3- 100% demon slayer quest- multiple - Copy.pcap

function oploc2_questdrain(player, x, y)
	mes(player, "This is the drainpipe")
	delay(3)
	mes(player, "Running from the kitchen sink to the sewer")
	delay(3)
	mes(player, "I can see a key just inside the drain")
	delay(3)
	mes(player, "I don't seem to be able to quite reach it")
	delay(3)
	mes(player, "It's stuck part way down")
	delay(3)
	mes(player, "I wonder if I can dislodge it somehow")
	delay(3)
	mes(player, "And knock it down into the sewers")
end

local function clear_drain(player)
	mes(player, "You pour the liquid down the drain")
	mes(player, "Ok I think I've washed the key down into the sewer")
	-- rsc-preservation.xyz/Quests/demon-slayer
	addobject(player, "silverlight key 3", 1, 117, 3294)
	delay(3)
	mes(player, "I'd better go down and get it before someone else finds it")
end

register_useloc("questdrain", "waterbucket", function(player, x, y)
	remove(player, "waterbucket", 1)
	give(player, "bucket", 1)
	clear_drain(player)
end)

register_useloc("questdrain", "waterjug", function(player, x, y)
	remove(player, "waterjug", 1)
	give(player, "jug", 1)
	clear_drain(player)
end)

register_useloc("questdrain", "bowl of water", function(player, x, y)
	remove(player, "bowl of water", 1)
	give(player, "bowl", 1)
	clear_drain(player)
end)

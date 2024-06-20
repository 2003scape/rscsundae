-- RSC 2001/7/iteminfo- bucket- use on cow- get milk.pcap
function usenpc_cow_bucket(player, npc)
	thinkbubble(player, "bucket")
	remove(player, "bucket", 1)
	mes(player, "@que@You milk the cow")
	give(player, "milk", 1)
end

-- RSC 2001/7/iteminfo- shears- use on sheep- success and fail.pcap
function usenpc_sheep_shears(player, npc)
	thinkbubble(player, "shears")
	mes(player, "@que@You attempt to shear the sheep")
	delay(3)
	-- taylor botted this years ago and found a roughly 35% fail rate
	-- lost city has it at 1/4
	if random(90) then
		mes(player, "@que@The sheep manages to get away from you!")
	else
		mes(player, "@que@You get some wool")
		give(player, "wool", 1)
	end
end

register_usenpc("cow", "bucket", usenpc_cow_bucket)
register_usenpc("sheep", "shears", usenpc_sheep_shears)

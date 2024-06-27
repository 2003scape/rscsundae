-- RSC 2001/replays master archive/Skilling/Firemaking/firemaking- light- fail due to inside inventory
register_useinv("logs", "tinderbox", function(player)
	mes(player, "@que@I think you should put the logs down before you light them!")
end)

-- rsc-preservation.xyz/Skilling/some woodcutting and firemaking at low levels.pcap
register_useobj("logs", "tinderbox", function(player, x, y)
	if blocked(x, y) then
		mes(player, "@que@You can't light a fire here")
		return
	end
	thinkbubble(player, "tinderbox")
	mes(player, "@que@You attempt to light the logs")
	delay(3)
	--- success rates from the 2018 discord chat
	--- level 10: 5/15 (85/256)
	--- level 20: 9/19 (117/256)
	--- level 30: 19/33 (133/256)
	--- level 40: 30/35 (210/256)
	if statrandom(player, STAT_FIREMAKING, 64, 512) then
		mes(player, "@que@The fire catches and the logs begin to burn")
		advancestat(player, STAT_FIREMAKING, 100, 7)
		delobject(player, "logs", x, y)
		addloc("fire", x, y, 100)
		addobject(player, "ashes", 1, x, y)
	else
		mes(player, "@que@You fail to light a fire")
	end
end)

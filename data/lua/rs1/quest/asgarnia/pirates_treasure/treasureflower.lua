-- RSC 2001/7/scenery- falador- park- flower- dig- aggrressive wyson.pcap
-- RSC 2001/7/scenery- falador- park- flowers- 50% pirates treasure- dig- 'seems a shame to dig'.pcap
-- rsc-preservation.xyz/Quests/Pirates treasure Pt4.pcap

register_useloc("treasureflower", "spade", function(player, x, y)
	local npc = nearvisnpc(player, "wyson the gardener")
	if npc then
		npcsay(npc, "Hey leave off my flowers")
		npcattack(npc, player)
		return
	end
	if getvar(player, "treasure_stage") ~= 3 then
		mes(player, "It seems a shame to dig up these nice flowers for no reason")
		return
	end
	mes(player, "You dig a hole in the ground")
	delay(3)
	mes(player, "You find a little bag of treasure")
	delay(3)
	mes(player, "Well done you have completed the pirate treasure quest")
	give(player, "coins", 250)
	give(player, "gold ring", 1)
	give(player, "emerald", 1)
	setvar(player, "treasure_stage", 4)
	giveqp(player, 2)
end)

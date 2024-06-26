-- rsc-preservation.xyz/Quests/cooks-assistant.pcap

function takeobj_flourheap(player, x, y)
	-- RSC 2001/replays master archive/Item info/Alphabetical/P/itemspawn-pot of flour (chute)- attempt to pick up without pot.pcap
	mes(player, "@que@I can't pick it up!")
	mes(player, "@que@I need a pot to hold it in")
end

function useobj_flourheap_pot(player, x, y)
	delobject(player, "flourheap", x, y)
	mes(player, "@que@You put the flour in the pot")
	remove(player, "pot", 1)
	give(player, "flourpot", 1)
end

register_useobj("flourheap", "pot", useobj_flourheap_pot)

function useloc_hopper1_grain(player, x, y)
	local hopper_grain = getvar(player, "hopper_grain")
	if hopper_grain > 0 then
		mes(player, "@que@There is already grain in the hopper")
	else
		thinkbubble(player, "grain")
		remove(player, "grain", 1)
		mes(player, "@que@You put the grain in the hopper")
		setvar(player, "hopper_grain", 1)
	end
end

register_useloc("hopper1", "grain", useloc_hopper1_grain)

function oploc1_hopper1(player)
	local hopper_grain = getvar(player, "hopper_grain")
	mes(player, "@que@You operate the hopper")
	if hopper_grain > 0 then
		mes(player, "@que@The grain slides down the chute")
		addobject(player, "flourheap", 1, 166, 599)
		setvar(player, "hopper_grain", 0)
	else
		mes(player, "@que@Nothing interesting happens")
	end
end

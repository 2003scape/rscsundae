-- ./Logg/Tylerbeg/08-01-2018 19.32.34 varrock pt2.pcap

register_useloc("hopper2", "grain", function(player, x, y)
	local hopper_grain = getvar(player, "hopper2_grain")
	if hopper_grain > 0 then
		mes(player, "@que@There is already grain in the hopper")
	else
		thinkbubble(player, "grain")
		remove(player, "grain", 1)
		mes(player, "@que@You put the grain in the hopper")
		setvar(player, "hopper2_grain", 1)
	end
end)

function oploc1_hopper2(player)
	local hopper_grain = getvar(player, "hopper2_grain")
	mes(player, "@que@You operate the hopper")
	if hopper_grain > 0 then
		mes(player, "@que@The grain slides down the chute")
		addobject(player, "flourheap", 1, 179, 481)
		setvar(player, "hopper2_grain", 0)
	else
		mes(player, "@que@Nothing interesting happens")
	end
end

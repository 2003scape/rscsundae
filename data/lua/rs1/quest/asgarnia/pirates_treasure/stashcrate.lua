-- rsc-preservation.xyz/Quests/Pirates treasure.pcap
register_useloc("bananacrate", "karamja rum", function(player, x, y)
	local stashed = getvar(player, "rum_stashed")
	local stage = getvar(player, "treasure_stage")
	if stashed == 0 and stage == 1 then
		mes(player, "You stash the rum in the crate")
		remove(player, "karamja rum", 1)
		setvar(player, "rum_stashed", 1)
	else
		mes(player, "@que@I have no reason to do that")
	end
end)

function oploc2_grocerycrate(player, x, y)
	mes(player, "@que@There are a lot of bananas in the crate")
	delay(3)
	if getvar(player, "rum_sent") ~= 0 then
		mes(player, "You find your bottle of rum in amoungst the bananas")
		give(player, "karamja rum", 1)
		setvar(player, "rum_stashed", 0)
		setvar(player, "rum_sent", 0)
		delay(3)
	end
	mes(player, "@que@Do you want to take a banana?")
	local resp = multi(player, "Yes", "No")
	if resp == 1 then
		mes(player, "@que@you take a banana")
		give(player, "banana", 1)
	end
end

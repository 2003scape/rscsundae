-- ./rscplus-replays-master/RSC 2001/replays master archive/Shops/Peksas medium helmet store/dialogue- peksa- all- shop

function talknpc_peksa(player, npc)
	npcsay(npc, "Are you interested in buying or selling a helmet?")
	local resp = multi(player,
		"I could be, yes",
		"No, I'll pass on that")
	if resp == 1 then
		say(player, "I could be, yes")
		npcsay(npc, "Well look at all these great helmets!")
		openshop(player, "helmetshop")
	elseif resp == 2 then
		say(player, "No, I'll pass on that")
		npcsay(npc, "Well come back if you change your mind")
	end
end

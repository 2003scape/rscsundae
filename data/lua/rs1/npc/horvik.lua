-- ./rscplus-replays-master/RSC 2001/3/f2p (not 100% complete)/dialogue- horvik- all- shop

function talknpc_horvik_the_armourer(player, npc)
	npcsay(npc, "Hello, do you need any help?")
	local resp = multi(player,
		"No thanks. I'm just looking around",
		"Do you want to trade?")
	if resp == 1 then
		say(player, "No thanks. I'm just looking around")
	elseif resp == 2 then
		say(player, "Do you want to trade?")
		npcsay(npc, "Yes, I have a fine selection of armour")
		openshop(player, "armourshop")
	end
end

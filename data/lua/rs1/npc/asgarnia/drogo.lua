-- ./rscplus-replays-master/RSC 2001/replays master archive/Shops/Drogos pickaxe shop/dialogue- drogo- all- shop

function talknpc_drogo(player, npc)
	npcsay(npc, "Ello")
	local resp = multi(player,
		"Do you want to trade?",
		"Hello shorty",
		"Why don't you ever restock ores and bars?")
	if resp == 1 then
		say(player, "Do you want to trade?")
		npcsay(npc, "Yeah sure, I run a mining store.")
		openshop(player, "miningstore")
	elseif resp == 2 then
		say(player, "Hello Shorty.")
		npcsay(npc, "I may be short, but at least I've got manners")
	elseif resp == 3 then
		say(player, "Why don't you ever restock ores and bars?")
		npcsay(npc, "The only ores and bars I sell are those sold to me")
	end
end

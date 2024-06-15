-- ./rscplus-replays-master/RSC 2001/replays master archive/Shops/Thessalias fine clothes/dialogue- thessalia- all- shop

function talknpc_thessalia(player, npc)
	say(player, "Hello")
	npcsay(npc, "Do you want to buy any fine clothes?")
	local resp = multi(player,
		"What have you got?",
		"No, thank you")
	if resp == 1 then
		say(player, "What have you got?")
		openshop(player, "clotheshop")
	elseif resp == 2 then
		say(player, "No, thank you")
	end
end

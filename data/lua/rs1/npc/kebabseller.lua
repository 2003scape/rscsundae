-- https://classic.runescape.wiki/w/Transcript:Kebab_Seller

function talknpc_kebab_seller(player, npc)
	npcsay(npc, "Would you like to buy a nice kebab?")
	npcsay(npc, "Only 1 gold")
	local resp = multi(player,
		"I think I'll give it a miss",
		"Yes please")
	if resp == 1 then
		say(player, "I think I'll give it a miss")
	elseif resp == 2 then
		say(player, "Yes please")
		if held(player, "coins", 1) then
			remove(player, "coins", 1)
			give(player, "kebab", 1)
			mes(player, "@que@You buy a kebab")
		else
			say(player, "Oops I forgot to bring any money with me")
			npcsay(npc, "Come back when you have some")
		end
	end
end

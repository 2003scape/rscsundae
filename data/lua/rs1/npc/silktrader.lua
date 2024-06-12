-- https://classic.runescape.wiki/w/Transcript:Silk_trader

function silktrader_haggle(player, npc)
	npcsay(npc, "Two coins and that's as low as I'll go")
	npcsay(npc, "I'm not selling it for any less")
	npcsay(npc, "You'll probably go and sell it in Varrock for a profit anyway")
	local resp = multi(player,
		"Two coins sounds good",
		"No, really. I don't want it")
	if resp == 1 then
		say(player, "Two coins sounds good")
		if held(player, "coins", 2) then
			remove(player, "coins", 2)
			give(player, "silk", 1)
			mes(player, "You buy some silk for 2 coins")
		else
			say(player, "Oh dear. I don't have enough money")
		end
	elseif resp == 2 then
		say(player, "No, really. I don't want it")
		npcsay(npc, "OK, but that's the best price you're going to get")
	end
end

function talknpc_silk_trader(player, npc)
	npcsay(npc, "Do you want to buy any fine silks?")
	local resp = multi(player,
		"How much are they?",
		"No. Silk doesn't suit me")
	if resp == 1 then
		say(player, "How much are they?")
		npcsay(npc, "3 Coins")
		resp = multi(player,
			"No. That's too much for me",
			"OK, that sounds good")
		if resp == 1 then
			say(player, "No. That's too much for me")
			silktrader_haggle(player, npc)
		elseif resp == 2 then
			say(player, "Ok, that sounds good")
			if held(player, "coins", 3) then
				remove(player, "coins", 3)
				give(player, "silk", 1)
				mes(player, "You buy some silk for 3 coins")
			else
				say(player, "Oh dear. I don't have enough money")
			end
		end
	elseif resp == 2 then
		say(player, "No. Silk doesn't suit me")
	end
end

register_talknpc("silk trader", talknpc_silk_trader)

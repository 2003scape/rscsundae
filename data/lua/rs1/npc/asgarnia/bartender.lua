-- https://classic.runescape.wiki/w/Transcript:Bartender_(The_Rusty_Anchor)
-- RSC 2001/3/f2p (not 100% complete)/dialogue- bartender- port sarim- 100% goblin diplomacy- buy beer- space in inv- success
-- RSC 2001/3/f2p (not 100% complete)/dialogue- bartender- port sarim- 100% goblin diplomacy- buy beer- fail- insufficient coins

local function asgtender_buy_beer(player, npc)
	say(player, "Could i buy a beer please?")
	if held(player, "coins", 2) then
		npcsay(npc, "Sure that will be 2 gold coins please")
		say(player, "Ok here you go thanks")
		mes(player, "@que@you buy a pint of beer")
		give(player, "beer", 1)
		remove(player, "coins", 2)
	else
		mes(player, "You dont have enough coins for the beer")
	end
end

function talknpc_asgtender(player, npc)
	if getvar(player, "goblin_stage") == 0 then
		local resp = multi(player,
			"Could i buy a beer please?",
			"Not very busy in here today is it?")

		if resp == 1 then
			asgtender_buy_beer(player, npc)
		elseif resp == 2 then
			say(player, "Not very busy in here today is it")
			npcsay(npc, "No it was earlier")
			npcsay(npc, "There was a guy in here saying the goblins up by the mountain are arguing again")
			npcsay(npc, "Of all things about the colour of their armour.")
			npcsay(npc, "Knowing the goblins, it could easily turn into a full blown war")
			npcsay(npc, "Which wouldn't be good")
			npcsay(npc, "Goblin wars make such a mess of the countryside")
			say(player, "Well if I have time I'll see if I can go and knock some sense into them")
			setvar(player, "goblin_stage", 1)
		end
	else
		local resp = multi(player,
			"Could i buy a beer please?",
			"Have you heard any more rumours in here?")

		if resp == 1 then
			asgtender_buy_beer(player, npc)
		elseif resp == 2 then
			say(player, "Have you heard any more rumours in here?")
			npcsay(npc, "No it hasn't been very busy lately")
		end
	end
end

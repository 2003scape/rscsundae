-- https://classic.runescape.wiki/w/Transcript:Wyson_the_gardener
-- RSC 2001/replays master archive/NPC (dialogue, notes)/Wyson/dialogue- wyson- all- quests 100%.pcap
-- RSC 2001/7/scenery- falador- park- flower- dig- aggrressive wyson.pcap
-- RSC 2001/7/scenery- falador- park- flowers- 50% pirates treasure- dig- 'seems a shame to dig'.pcap

local function wyson_reject(player, npc)
	npcsay(npc, "No No thats far too little. Woad leaves are hard to get you know")
	npcsay(npc, "I used to have plenty but someone kept stealing them off me")
end

local function wyson_buy(player, npc)
	npcsay(npc, "How much are you willing to pay?")
	local resp = multi(player,
		"How about 5 coins?",
		"How about 10 coins?",
		"How about 15 coins?",
		"How about 20 coins?")
	if resp == 1 then
		say(player, "How about 5 coins?")
		wyson_reject(player, npc)
	elseif resp == 2 then
		say(player, "How about 10 coins?")
		wyson_reject(player, npc)
	elseif resp == 3 then
		say(player, "How about 15 coins?")
		npcsay(npc, "Mmmm Ok that sounds fair.")
		if not held(player, "coins", 15) then
			say(player, "I dont have enough coins to buy the leaves. I'll come back later")
			return
		end
		mes(player, "You give wyson 15 coins")
		remove(player, "coins", 15)
		give(player, "woad leaf", 1)
		mes(player, "Wyson the gardener gives you some woad leaves")
	elseif resp == 4 then
		say(player, "How about 20 coins?")
		npcsay(npc, "Ok that's more than fair.")
		if not held(player, "coins", 20) then
			say(player, "I dont have enough coins to buy the leaves. I'll come back later")
			return
		end
		mes(player, "You give wyson 20 coins")
		remove(player, "coins", 20)
		give(player, "woad leaf", 2)
		mes(player, "Wyson the gardener gives you some woad leaves")
		npcsay(npc, "Here have some more you're a generous person")
		mes(player, "Wyson the gardener gives you some more leaves")
	end
end

local function wyson_ask(player, npc)
	say(player, "Who told you that?")
	npcsay(npc, "I can't remember now. Someone who visits this park")
	npcsay(npc, "I happen to have some woad leaves lying around")
	npcsay(npc, "Would you like to buy some?")
	local resp = multi(player,
		"Oh yes please",
		"No thanks not right now")
	if resp == 1 then
		say(player, "Oh yes please")
		wyson_buy(player, npc)
	elseif resp == 2 then
		say(player, "No thanks not right now")
	end
end

function talknpc_wyson_the_gardener(player, npc)
	local stage = getvar(player, "goblin_stage")

	if stage < 5 then
		npcsay(npc, "I am the gardener round here")
		npcsay(npc, "Do you have any gardening that needs doing?")
		local resp = multi(player,
			"I'm looking for woad leaves",
			"Not right now thanks")
		wyson_buy(player, npc)
		if resp == 1 then
			say(player, "I'm looking for woad leaves")
			npcsay(npc, "Well luckily for you I may have some around here somewhere")
			say(player, "Can I buy one please?")
		elseif resp == 2 then
			say(player, "Not right now thanks")
		end
	else
		npcsay(npc, "Hey i have heard you are looking for woad leaves")
		local resp = multi(player,
			"Well yes I am. Can you get some?",
			"Who told you that?")
		if resp == 1 then
			say(player, "Well yes I am. Can you get some?")
			npcsay(npc, "Yes I have some somewhere")
			-- dialogue bug with duplicated line intentionally not replicated
			say(player, "Can I buy one please?")
			wyson_buy(player, npc)
		elseif resp == 2 then
			wyson_ask(player, npc)
		end
	end
end

register_useloc("treasureflower", "spade", function(player, x, y)
	local npc = nearvisnpc(player, "wyson the gardener")
	if npc then
		npcsay(npc, "Hey leave off my flowers")
		npcattack(npc, player)
	else
		mes(player, "It seems a shame to dig up these nice flowers for no reason")
	end
end)

-- https://classic.runescape.wiki/w/Transcript:Redbeard_Frank

local function frank_trade(player, npc)
	say(player, "Do you want to trade?")
	npcsay(npc, "No, I've got nothing to trade")
end

local function frank_arr(player, npc)
	say(player, "Arrrh")
	npcsay(npc, "Arrrh")
end

local function frank_treasure(player, npc)
	say(player, "I'm in search of treasure")
	npcsay(npc, "Arrrh treasure you be after eh?")
	npcsay(npc, "Well I might be able to tell you where to find some.")
	npcsay(npc, "For a price")
	say(player, "What sort of price?")
	npcsay(npc, "Well for example if you can get me a bottle of rum")
	npcsay(npc, "Not just any rum mind")
	npcsay(npc, "I'd like some rum brewed on Karamja island")
	npcsay(npc, "There's no rum like Karamja rum")
	setvar(player, "treasure_stage", 1)
end

local function frank_status(player, npc)
	npcsay(npc, "Have Ye brought some rum for yer old mate Frank")
	if not held(player, "karamja rum", 1) then
		say(player, "No not yet")
		return
	end
	say(player, "Yes I've got some")
	mes(player, "Frank happily takes the rum")
	remove(player, "karamja rum", 1)
	delay(3)
	npcsay(npc, "Now a deals a deal, I'll tell ye about the treasure")
	npcsay(npc, "I used to serve under a pirate captain called One Eyed Hector")
	npcsay(npc, "Hector was a very succesful pirate and became very rich")
	npcsay(npc, "but about a year ago we were boarded by the Royal Asgarnian Navy")
	npcsay(npc, "Hector was killed along with many of the crew")
	npcsay(npc, "I was one of the few to escape")
	npcsay(npc, "And I escaped with this")
	mes(player, "Frank hands you a key")
	give(player, "chest key", 1)
	delay(3)
	npcsay(npc, "This is Hector's key")
	npcsay(npc, "I believe it opens his chest")
	npcsay(npc, "In his old room in the blue moon inn in Varrock")
	npcsay(npc, "With any luck his treasure will be in there")
	setvar(player, "treasure_stage", 2)
	local resp = multi(player,
		"Ok thanks, I'll go and get it",
		"So why didn't you ever get it?")
	if resp == 1 then
		say(player, "Ok thanks, I'll go and get it")
	elseif resp == 2 then
		say(player, "So why didn't you ever get it?")
		npcsay(npc, "I'm not allowed in the blue moon inn")
		npcsay(npc, "Apparently I'm a drunken trouble maker")
	end
end

function talknpc_redbeard_frank(player, npc)
	local stage = getvar(player, "treasure_stage")

	-- RSC 2001/7/dialogue- red beard frank- 50% pirates treasure- get replacement key.pcap
	if not held(player, "chest key", 1) and stage > 1 and stage < 4 then
		npcsay(npc, "Arrrh matey")
		say(player, "I seem to have lost my chest key")
		npcsay(npc, "Arrr silly you")
		npcsay(npc, "Fortunatly I took the precaution to have another made")
		mes(player, "Frank hands you a chest key")
		give(player, "chest key", 1)
		local resp = multi(player,
			"Arrrh",
			"Do you want to trade?")
		if resp == 1 then
			frank_arr(player, npc)
		elseif resp == 2 then
			frank_trade(player, npc)
		end
		return
	end

	npcsay(npc, "Arrrh Matey")

	if stage == 0 then
		local resp = multi(player,
			"I'm in search of treasure",
			"Arrrh",
			"Do you want to trade?")
		if resp == 1 then
			frank_treasure(player, npc)
		elseif resp == 2 then
			frank_arr(player, npc)
		elseif resp == 3 then
			frank_trade(player, npc)
		end
	elseif stage == 1 then
		frank_status(player, npc)
	else
		local resp = multi(player,
			"Arrrh",
			"Do you want to trade?")
		if resp == 1 then
			frank_arr(player, npc)
		elseif resp == 2 then
			frank_trade(player, npc)
		end
	end
end

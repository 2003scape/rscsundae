-- https://classic.runescape.wiki/w/Transcript:Baraek
-- RSC 2001/3/f2p (not 100% complete)/dialogue- baraek- 100% arrav- buy furs.pcap

local function baraek_haggle(player, npc)
	npcsay(npc, "Well, okay I'll go down to 18")
	local resp = multi(player,
		"Okay here you go",
		"No thanks I'll leave it")
	if resp == 1 then
		say(player, "Okay here you go")
		if held(player, "coins", 18) then
			remove(player, "coins", 18)
			give(player, "fur", 1)
			mes(player, "You buy a fur from Baraek")
		else
			say(player, "Oh dear I don't seem to have enough money")
			npcsay(npc, "Well I can't go any cheaper than that mate")
			npcsay(npc, "I have a family to feed")
			say(player, "Ah well never mind")
		end
	elseif resp == 2 then
		say(player, "No thanks, I'll leave it")
		npcsay(npc, "It's your loss mate")
	end
end

local function baraek_sell(player, npc)
	say(player, "Can you sell me some furs?")
	npcsay(npc, "Yeah sure they're 20 gold coins a piece")
	local resp = multi(player,
		"Yeah, okay here you go",
		"20 gold coins thats an outrage")
	if resp == 1 then
		if held(player, "coins", 20) then
			say(player, "Yeah okay here you go")
			remove(player, "coins", 20)
			give(player, "fur", 1)
			mes(player, "You buy a fur from Baraek")
		else
			say(player, "Oh dear I don't seem to have enough money")
			baraek_haggle(player, npc)
		end
	elseif resp == 2 then
		say(player, "20 gold coins that's an outrage")
		baraek_haggle(player, npc)
	end
end

local function baraek_buy(player, npc)
	-- XXX no replay exists of this, assumed from wiki transcript
	say(player, "Would you like to buy my fur?")
	npcsay(npc, "Lets have a look at it")
	mes(player, "Baraek examines the fur")
	delay(3)
	npcsay(npc, "It's not in the best of condition")
	npcsay(npc, "I guess I could give 12 coins to take it off your hands")
	local resp = multi(player,
		"Yeah that'll do",
		"I think I'll keep hold of it actually")
	if resp == 1 then
		say(player, "Yeah that'll do")
		remove(player, "fur", 1)
		give(player, "coins", 12)
		mes(player, "You give Baraek a fur")
		delay(3)
		mes(player, "And he gives you twelve coins")
	elseif resp == 2 then
		say(player, "I think I'll keep hold of it actually")
		npcsay(npc, "Oh ok")
		npcsay(npc, "Didn't want it anyway")
	end
end

local function baraek_phoenix(player, npc)
	say(player, "Can you tell me where I can find the phoenix gang?")
	npcsay(npc, "Sh Sh, not so loud")
	npcsay(npc, "You don't want to get me in trouble")
	say(player, "So do you know where they are?")
	npcsay(npc, "I may do")
	npcsay(npc, "Though I don't want to get into trouble for revealing their hideout")
	npcsay(npc, "Now if I was say 20 gold coins richer")
	npcsay(npc, "I may happen to be more inclined to take that sort of risk")
	local resp = multi(player,
		"Okay have 20 gold coins",
		"No I don't like things like bribery",
		"Yes I'd like to be 20 gold coins richer too")
	if resp == 1 then
		say(player, "Okay have 20 gold coins")
		if not held(player, "coins", 20) then
			say(player, "Oops. I don't have 20 coins. Silly me.")
			return
		end
		remove(player, "coins", 20)
		npcsay(npc, "Cheers")
		npcsay(npc, "Ok to get to the gang hideout")
		npcsay(npc, "After entering Varrock through the south gate")
		npcsay(npc, "If you take the first turning east")
		npcsay(npc, "Somewhere along there is an alleyway to the south")
		npcsay(npc, "The door at the end of there is the entrance to the phoenix gang")
		npcsay(npc, "They're operating there under the name of the VTAM corporation")
		npcsay(npc, "Be careful")
		npcsay(npc, "The phoenix gang ain't the types to be messed with")
		say(player, "Thanks")
		setvar(player, "phoenix_known", 1)
	elseif resp == 2 then
		say(player, "No I don't like things like bribery")
		npcsay(npc, "Heh, if you wanna deal with the phoenix gang")
		npcsay(npc, "They're involved in much worse than a bit of bribery")
	elseif resp == 3 then
		say(player, "Yes I'd like to be 20 gold coins richer too")
	end
end

function talknpc_baraek(player, npc)
	local choices = {}

	if getvar(player, "arrav_stage") >= 3 then
		table.insert(choices, "Can you tell me where I can find the phoenix gang?")
	end
	table.insert(choices, "Can you sell me some furs?")
	table.insert(choices, "Hello I am in search of a quest")
	if held(player, "fur", 1) then
		table.insert(choices, "Would you like to buy my fur?")
	end

	local resp = multi(player, table.unpack(choices))
	resp = choices[resp]

	if resp == "Can you tell me where I can find the phoenix gang?" then
		baraek_phoenix(player, npc)
	elseif resp == "Can you sell me some furs?" then
		baraek_sell(player, npc)
	elseif resp == "Hello I am in search of a quest" then
		say(player, "Hello I am in search of a quest")
		npcsay(npc, "Sorry kiddo, I'm a fur trader not a damsel in distress")
	elseif resp == "Would you like to buy my fur?" then
		baraek_buy(player, npc)
	end
end

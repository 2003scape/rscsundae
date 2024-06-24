-- https://classic.runescape.wiki/w/Transcript:Doric
-- Milestone: first quest

local function doric_anvils(player, npc)
	say(player, "I wanted to use your anvils")
	npcsay(npc, "My anvils get enough work with my own use")
	npcsay(npc, "I make amulets, it takes a lot of work.")
	npcsay(npc, "If you could get me some more materials I could let you use them")
	local resp = multi(player,
		"Yes I will get you materials",	
		"No, hitting rocks is for the boring people, sorry.")
	if resp == 1 then
		say(player, "Yes I will get you materials")
		npcsay(npc, "Well, clay is what I use more than anything. I make casts")
		npcsay(npc, "Could you get me 6 clay, and 4 copper ore and 2 iron ore please?")
		npcsay(npc, "I could pay a little, and let you use my anvils")
		say(player, "Certainly, I will get them for you. goodbye")
		setvar(player, "doric_stage", 1)
	elseif resp == 2 then
		say(player, "No, hitting rocks is for the boring people, sorry")
		npcsay(npc, "That is your choice, nice to meet you anyway")
	end
end

local function doric_initial(player, npc)
	npcsay(npc, "Hello traveller, what brings you to my humble smithy?")
	local resp = multi(player,
		"I wanted to use your anvils",
		"Mind your own business, shortstuff",
		"I was just checking out the landscape",
		"What do you make here?")
	if resp == 1 then
		doric_anvils(player, npc)
	elseif resp == 2 then
		say(player, "Mind your own business, shortstuff")
		npcsay(npc, "How nice to meet someone with such pleasant manners")
		npcsay(npc, "Do come again when you need to shout at someone smaller than you")
	elseif resp == 3 then
		say(player, "I was just checking out the landscape")
		npcsay(npc, "We have a fine town here, it suits us very well")
		npcsay(npc, "Please enjoy your travels. And do visit my friends in their mine")
	elseif resp == 4 then
		say(player, "What do you make here?")
		npcsay(npc, "I make amulets. I am the best maker of them in Runescape")
		say(player, "Do you have any to sell?")
		npcsay(npc, "Not at the moment, sorry. Try again later")
	end
end

local function doric_progress(player, npc)
	npcsay(npc, "Have you got my materials yet traveller?")
	if held(player, "clay", 6) and
	    held(player, "copper ore", 4) and
	    held(player, "iron ore", 2) then
		say(player, "I have everything you need")
		npcsay(npc, "Many thanks, pass them here please")
		mes(player, "You hand the clay, copper and iron to Doric")
		remove(player, "clay", 6)
		remove(player, "copper ore", 4)
		remove(player, "iron ore", 2)
		npcsay(npc, "I can spare you some coins for your trouble")
		mes(player, "Doric hands you 180 coins")
		give(player, "coins", 180)
		npcsay(npc, "Please use my anvils any time you want")
		mes(player, "@gre@You have completed Dorics quest")
		advancestat(player, STAT_MINING, 700, 300)
		setvar(player, "doric_stage", 2)
		giveqp(player, 1)
	else
		say(player, "Sorry, I don't have them all yet")
		npcsay(npc, "Not to worry, stick at it")
		npcsay(npc, "Remember I need 6 Clay, 4 Copper ore and 2 Iron ore")
	end
end

local function doric_end(player, npc)
	npcsay(npc, "Hello traveller, how is your Metalworking coming along?")
	say(player, "Not too bad thanks Doric")
	npcsay(npc, "Good, the love of metal is a thing close to my heart")
end

function talknpc_doric(player, npc)
	local stage = getvar(player, "doric_stage")
	if stage == 0 then
		doric_initial(player, npc)
	elseif stage == 1 then
		doric_progress(player, npc)
	elseif stage == 2 then
		doric_end(player, npc)
	end
end

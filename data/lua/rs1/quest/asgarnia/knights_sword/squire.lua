-- https://classic.runescape.wiki/w/Transcript:Squire

local function squire_find_soon(player, npc)
	say(player, "Well I hope you find it soon")
	npcsay(npc, "Yes me too")
	npcsay(npc, "I'm not looking forward to telling Vyvin I've lost it")
	npcsay(npc, "He's going to want it for the parade next week as well")
end

local function squire_dwarves(player, npc)
	say(player, "So would these dwarves make another one?")
	npcsay(npc, "I'm not a hundred percent sure the Imcando tribe exists anymore")
	npcsay(npc, "I should think Reldo the palace librarian in Varrock will know")
	npcsay(npc, "He has done a lot of research on the races of Runescape")
	npcsay(npc, "I don't suppose you could try and track down the Imcando dwarves for me?")
	npcsay(npc, "I've got so much work to do")

	local resp = multi(player,
		"Ok I'll give it a go",
		"No I've got lots of mining work to do")
	if resp == 1 then
		say(player, "Ok I'll give it a go")
		npcsay(npc, "Thankyou very much")
		npcsay(npc, "As I say the best place to start should be with Reldo")
		setvar(player, "sword_stage", 1)
	elseif resp == 2 then
		say(player, "No I've got lots of mining work to do")
	end
end

local function squire_new_sword(player, npc)
	say(player, "I can make a new sword if you like")
	npcsay(npc, "Thanks for the offer")
	npcsay(npc, "I'd be surprised if you could though")
	npcsay(npc, "The thing is,this sword is a family heirloom")
	npcsay(npc, "It has been passed down through Vyvin's family for five generations")
	npcsay(npc, "It was originally made by the Imcando Dwarves")
	npcsay(npc, "Who were a particularly skilled tribe of dwarven smiths")
	npcsay(npc, "I doubt anyone could make it in the style they do")

	local resp = multi(player,
		"So would these dwarves make another one?",
		"Well I hope you find it soon")
	if resp == 1 then
		squire_dwarves(player, npc)
	elseif resp == 2 then
		squire_find_soon(player, npc)
	end
end

local function squire_abundant(player, npc)
	say(player, "Well the kingdom is fairly abundant with swords")
	npcsay(npc, "Yes you can get bronze swords anywhere")
	npcsay(npc, "But this isn't any old sword")
	npcsay(npc, "The thing is,this sword is a family heirloom")
	squire_new_sword(player, npc)
end

local function squire_lost_where(player, npc)
	say(player, "Well do you know the vague area you lost it in?")
	npcsay(npc, "No I was carrying it for him all the way from where he had it stored in Lumbridge")
	npcsay(npc, "It must have slipped from my pack during the trip")
	npcsay(npc, "And you know what people are like these days")
	npcsay(npc, "Someone will have just picked it up and kept it for themselves")

	local resp = multi(player,
		"I can make a new sword if you like",
		"Well the kingdom is fairly abundant with swords",
		"Well I hope you find it soon")
	if resp == 1 then
		squire_new_sword(player, npc)
	elseif resp == 2 then
		squire_abundant(player, npc)
	elseif resp == 3 then
		squire_find_soon(player, npc)
	end
end

local function squire_angry(player, npc)
	say(player, "Is he angry?")
	npcsay(npc, "He doesn't know yet")
	npcsay(npc, "I was hoping I could think of something to do")
	npcsay(npc, "Before he does find out")
	npcsay(npc, "But I find myself at a loss")

	local resp = multi(player,
		"Well do you know the vague area you lost it?",
		"I can make a new sword if you like",
		"Well the kingdom is fairly abundant with swords",
		"Well I hope you find it soon")
	if resp == 1 then
		squire_lost_where(player, npc)
	elseif resp == 2 then
		squire_new_sword(player, npc)
	elseif resp == 3 then
		squire_abundant(player, npc)
	elseif resp == 4 then
		squire_find_soon(player, npc)
	end
end

local function squire_life(player, npc)
	say(player, "And how is life as a squire?")
	npcsay(npc, "Well Sir Vyvin is a good guy to work for")
	npcsay(npc, "However I'm in a spot of trouble today")
	npcsay(npc, "I've gone and lost Sir Vyvin's sword")

	local resp = multi(player,
		"Well do you know the vague area you lost it?",
		"I can make a new sword if you like",
		"Is he angry?")
	if resp == 1 then
		squire_lost_where(player, npc)
	elseif resp == 2 then
		squire_new_sword(player, npc)
	elseif resp == 3 then
		squire_angry(player, npc)
	end
end

local function squire_init(player, npc)
	npcsay(npc, "Hello I am the squire to Sir Vyvin")

	local resp = multi(player,
		"And how is life as a squire?",
		"Wouldn't you prefer to be a squire for me?")
	if resp == 1 then
		squire_life(player, npc)
	elseif resp == 2 then
		say(player, "Wouldn't you prefer to be a squire for me?")
		npcsay(npc, "No, sorry I'm loyal to Vyvin")
	end
end

function talknpc_squire(player, npc)
	local stage = getvar(player, "sword_stage")
	if stage == 0 then
		squire_init(player, npc)
	elseif stage < 2 then
		npcsay(npc, "So how are you doing getting a sword?")
		say(player, "I'm still looking for Imcando dwarves")
	elseif stage < 3 then
		npcsay(npc, "So how are you doing getting a sword?")
		say(player, "I've found an Imcando dwarf")
		say(player, "But he needs a picture of the sword before he can make it")
		npcsay(npc, "A picture eh?")
		npcsay(npc, "The only one I can think of is in a small portrait of Sir Vyvin's father")
		npcsay(npc, "Sir Vyvin keeps it in a cupboard in his room I think")
	elseif stage < 4 then
		if not held(player, "faladian knight's sword", 1) then
			npcsay(npc, "So how are you doing getting a sword?")
			say(player, "I've found a dwarf who will make the sword")
			say(player, "I've just got to find the materials for it now")
		else 
			-- rsc-preservation.xyz/Quests/The knights sword
			say(player, "I have retrieved your sword for you")
			npcsay(npc, "Thankyou, Thankyou")
			npcsay(npc, "I was seriously worried I'd have to own up to Sir Vyvin")
			mes(player, "You give the sword to the squire")
			remove(player, "faladian knight's sword", 1)
			delay(3)
			mes(player, "Well done you have completed the knight's sword quest")
			setvar(player, "sword_stage", 4)
			advancestat(player, STAT_SMITHING, 1400, 1500)
			giveqp(player, 1)
		end
	else
		npcsay(npc, "Hello friend")
		npcsay(npc, "thanks for your help before")
		npcsay(npc, "Vyvin never even realised it was a different sword")
	end
end

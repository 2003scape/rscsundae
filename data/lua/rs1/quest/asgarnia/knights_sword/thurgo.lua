-- https://classic.runescape.wiki/w/Transcript:Thurgo
-- rsc-preservation.xyz/Quests/The knights sword

local function thurgo_offer_pie(player, npc)
	say(player, "Would you like some redberry Pie?")
	mes(player, "Thurgo's eyes light up")
	delay(3)
	npcsay(npc, "I'd never say no to a redberry pie")
	npcsay(npc, "It's great stuff")
	mes(player, "You hand over the pie")
	remove(player, "redberry pie", 1)
	delay(3)
	mes(player, "Thurgo eats the pie")
	delay(3)
	mes(player, "Thurgo pats his stomach")
	delay(3)
	npcsay(npc, "By Guthix that was good pie")
	npcsay(npc, "Anyone who makes pie like that has gotta be alright")
	setvar(player, "imcando_stage", 2)
end

local function thurgo_ask_imcando(player, npc)
	say(player, "Hello are you are an Imcando Dwarf?")
	npcsay(npc, "Yeah what about it?")
	local resp = multi(player,
		"Would you like some redberry Pie?",
		"Can you make me a special sword?")
	if resp == 1 then
		thurgo_offer_pie(player, npc)
	elseif resp == 2 then
		say(player, "Can you make me a special sword?")
		npcsay(npc, "no I don't do that anymore")
		npcsay(npc, "I'm getting old")
	end
end

function talknpc_thurgo(player, npc)
	local stage = getvar(player, "sword_stage")
	if getvar(player, "imcando_stage") < 1 then
		mes(player, "Thurgo doesn't appear to be interested in talking")
	elseif stage < 2 then
		if getvar(player, "imcando_stage") > 1 then
			say(player, "Can you make me a special sword?")
			npcsay(npc, "Well after you've brought me such a great pie")
			npcsay(npc, "I guess I should give it a go")
			npcsay(npc, "What sort of sword is it?")
			say(player, "I need you to make a sword for one of Falador's knights")
			say(player, "He had one which was passed down through five generations")
			say(player, "But his squire has lost it")
			say(player, "So we need an identical one to replace it")
			npcsay(npc, "A Knight's sword eh?")
			npcsay(npc, "Well I'd need to know exactly how it looked")
			npcsay(npc, "Before I could make a new one")
			npcsay(npc, "All the Faladian knights used to have swords with different designs")
			npcsay(npc, "could you bring me a picture or something?")
			say(player, "I'll see if I can find one")
			say(player, "I'll go and ask his squire")
			setvar(player, "sword_stage", 2)
			return
		end
		if not held(player, "redberry pie", 1) then
			say(player, "Hello are you are an Imcando Dwarf?")
			npcsay(npc, "Yeah what about it?")
			return
		end
		local resp = multi(player,
			"Hello are you an Imcando Dwarf?",
			"Would you like some redberry Pie?")
		if resp == 1 then
			thurgo_ask_imcando(player, npc)
		elseif resp == 2 then
			thurgo_offer_pie(player, npc)
		end
	elseif stage < 3 then
		if not held(player, "portrait", 1) then
			npcsay(npc, "Have you got a picture of the sword for me yet?")
			say(player, "Sorry not yet")
			return
		end
		say(player, "I have found a picture of the sword I would like you to make")
		mes(player, "You give the portrait to Thurgo")
		remove(player, "portrait", 1)
		delay(3)
		mes(player, "Thurgo studies the portrait")
		delay(3)
		npcsay(npc, "Ok you'll need to get me some stuff for me to make this")
		npcsay(npc, "I'll need two Iron bars to make the sword to start with")
		npcsay(npc, "I'll also need an ore called blurite")
		npcsay(npc, "It's useless for making actual weapons for fighting with")
		npcsay(npc, "But I'll need some as decoration for the hilt")
		npcsay(npc, "It is a fairly rare sort of ore")
		npcsay(npc, "The only place I know where to get it")
		npcsay(npc, "Is under this cliff here")
		npcsay(npc, "But it is guarded by a very powerful ice giant")
		npcsay(npc, "Most the rocks in that clif are pretty useless")
		npcsay(npc, "Don't contain much of anything")
		npcsay(npc, "But there's definitly some blurite in there")
		npcsay(npc, "You'll need a little bit of mining experience")
		npcsay(npc, "TO be able to find it")
		say(player, "Ok I'll go and find them")
		setvar(player, "sword_stage", 3)
	elseif not held(player, "faladian knight's sword", 1) and stage < 4 then
		npcsay(npc, "How are you doing finding sword materials?")
		if not held(player, "iron bar", 2) or
		    not held(player, "blurite ore", 1) then
			say(player, "I haven't found everything yet")
			npcsay(npc, "Well come back when you do")
			npcsay(npc, "Remember I need blurite ore and two iron bars")
		else
			say(player, "I have them all")
			mes(player, "You give some blurite ore and two iron bars to Thurgo")
			remove(player, "iron bar", 2)
			remove(player, "blurite ore", 1)
			delay(3)
			mes(player, "Thurgo starts making a sword")
			delay(3)
			mes(player, "Thurgo hammers away")
			delay(3)
			mes(player, "Thurgo hammers some more")
			delay(3)
			mes(player, "Thurgo hands you a sword")
			give(player, "faladian knight's sword", 1)
		end
	else
		say(player, "Thanks for your help in getting the sword for me")
		npcsay(npc, "No worries mate")
	end
end

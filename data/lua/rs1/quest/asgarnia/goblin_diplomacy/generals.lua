-- https://classic.runescape.wiki/w/Transcript:General_Bentnoza
-- https://classic.runescape.wiki/w/Transcript:General_wartface
-- Revisionism/06-15-2018 04.23.22 goblin diplomacy.pcap

local function generals_argue(wartface, bentnoze)
	npcsay(wartface, "green armour best")
	npcsay(bentnoze, "No no Red every time")
	npcsay(wartface, "go away human, we busy")
end

local function generals_talk(player, npc, is_wartface)
	local wartface
	local bentnoze

	if is_wartface then
		wartface = npc
		bentnoze = nearnpc(player, "general bentnoze")
		npcbusy(bentnoze)
	else
		bentnoze = npc
		wartface = nearnpc(player, "general wartface")
		npcbusy(wartface)
	end

	local stage = getvar(player, "goblin_stage")

	if stage == 0 then
		generals_argue(wartface, bentnoze)
	elseif stage == 1 then
		generals_argue(wartface, bentnoze)
		local resp = multi(player,
			"Why are you arguing about the colour of your armour?",
			"Wouldn't you prefer peace?",
			"Do you want me to pick an armour colour for you?")
		if resp == 1 then
			say(player, "Why are you arguing about the colour of your armour?")
			npcsay(wartface, "We decide to celebrate goblin new century")
			npcsay(wartface, "By changing the colour of our armour")
			npcsay(wartface, "Light blue get boring after a bit")
			npcsay(wartface, "And we want change")
			npcsay(wartface, "Problem is they want different change to us")
		elseif resp == 2 then
			say(player, "Wouldn't you prefer peace?")
			npcsay(wartface, "Yeah peace is good as long as it is peace wearing Green armour")
			npcsay(bentnoze, "But green to much like skin!")
			npcsay(bentnoze, "Nearly make you look naked!")
		elseif resp == 3 then
			say(player, "Do you want me to pick an armour colour for you?")
			say(player, "different to either green or red")
			npcsay(wartface, "Hmm me dunno what that'd look like")
			npcsay(wartface, "You'd have to bring me some, so us could decide")
			npcsay(bentnoze, "Yep bring us orange armour")
			npcsay(wartface, "Yep orange might be good")
			setvar(player, "goblin_stage", 2)
		end
	elseif stage == 2 then
		generals_argue(wartface, bentnoze)
		npcsay(wartface, "Oh it you")
		if held(player, "orange goblin armour", 1) then
			say(player, "I have some orange armour")
			mes(player, "You give some goblin armour to the goblins")
			delay(3)
			remove(player, "orange goblin armour", 1)
			npcsay(wartface, "No I don't like that much")
			npcsay(bentnoze, "It clashes with my skin colour")
			npcsay(wartface, "Try bringing us dark blue armour")
			setvar(player, "goblin_stage", 3)
		else
			npcsay(wartface, "Have you got some orange goblin armour yet?")
			say(player, "Err no")
			npcsay(wartface, "Come back when you have some")
		end
	elseif stage == 3 then
		if held(player, "dark blue goblin armour", 1) then
			generals_argue(wartface, bentnoze)
			say(player, "I have some dark blue armour")
			mes(player, "You give some goblin armour to the goblins")
			delay(3)
			remove(player, "dark blue goblin armour", 1)
			npcsay(wartface, "Doesn't seem quite right")
			npcsay(bentnoze, "maybe if it was a bit lighter")
			npcsay(wartface, "Yeah try light blue")
			say(player, "I thought that was the armour you were changing from")
			say(player, "But never mind, anything is worth a try")
			setvar(player, "goblin_stage", 4)
		else
			npcsay(wartface, "Have you got some Dark Blue goblin armour yet?")
			say(player, "Err no")
			npcsay(wartface, "Come back when you have some")
		end
	elseif stage == 4 then
		if held(player, "light blue goblin armour", 1) then
			say(player, "Ok I've got light blue armour")
			mes(player, "You give some goblin armour to the goblins")
			remove(player, "light blue goblin armour", 1)
			npcsay(wartface, "That is rather nice")
			npcsay(bentnoze, "Yes I could see myself wearing somethin' like that")
			npcsay(wartface, "It' a deal then")
			npcsay(wartface, "Light blue it is")
			npcsay(wartface, "Thank you for sorting our argument")
			mes(player, "Well done you have completed the goblin diplomacy quest")
			setvar(player, "goblin_stage", 5)
			giveqp(player, 5)
			advancestat(player, STAT_CRAFTING, 500, 60)
			mes(player, "general wartface gives you a gold bar as thanks")
			give(player, "gold bar", 1)
		else
			npcsay(wartface, "Have you got some Light Blue goblin armour yet?")
			say(player, "Err no")
			npcsay(wartface, "Come back when you have some")
		end
	elseif stage == 5 then
		npcsay(wartface, "Now you've solved our argument we gotta think of something else to do")
		npcsay(bentnoze, "Yep, we bored now")
	end

	npcunbusy(wartface)
	npcunbusy(bentnoze)
end

function talknpc_general_wartface(player, npc)
	generals_talk(player, npc, true)
end

function talknpc_general_bentnoze(player, npc)
	generals_talk(player, npc, false)
end

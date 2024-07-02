-- https://classic.runescape.wiki/w/Transcript:Juliet
-- 1e_Luis/Quests/Romeo & Juliet

local function juliet_give_message(player, npc)
	say(player, "Certinly, I will deliver your message straight away")
	npcsay(npc, "It may be our only hope")
	give(player, "message", 1)
	mes(player, "Juliet gives you a message")
	setvar(player, "rj_stage", 1)
end

local function juliet_met(player, npc)
	say(player, "I did see Romeo somewhere.")
	say(player, "He seemed a bit depressed.")
	npcsay(npc, "Yes, that would be him.")
	npcsay(npc, "Could you please deliver a messge to him?")
	local resp = multi(player,
		"Certainly, I will do so straight away",
		"No, he was a little too weird for me")
	if resp == 1 then
		juliet_give_message(player, npc)
	elseif resp == 2 then
		say(player, "No")
		npcsay(npc, "Oh dear, that will be the ruin of our love")
		npcsay(npc, "Well, I will just stay here and worry")
		npcsay(npc, "You unromantic soul.")
	end
end

local function juliet_notmet(player, npc)
	say(player, "No, I think I would have remembered")
	npcsay(npc, "Could you please deliver a message to him?")
	local resp = multi(player,
		"Certinly, I will do so straight away",
		"No, I have better things to do")
	if resp == 1 then
		juliet_give_message(player, npc)
	elseif resp == 2 then
		say(player, "No, I have better things to do")
		npcsay(npc, "I will not keep you from them. Goodbye")
	end
end

local function juliet_init(player, npc)
	npcsay(npc, "Romeo, Romeo, wherefore art thou Romeo?")
	npcsay(npc, "Bold adventurer, have you seen Romeo on your travels?")
	npcsay(npc, "Skinny guy, a bit wishy washy, head full of poetry")
	local resp = multi(player,
		"Yes I have met him",
		"No, I think I would have remembered if I had",
		"I guess I could find him",
		"I think you could do better")
	if resp == 1 then
		juliet_met(player, npc)
	elseif resp == 2 then
		juliet_notmet(player, npc)
	elseif resp == 3 then
		say(player, "I guess I could find him")
		npcsay(npc, "That is most kind of you")
		npcsay(npc, "Could you please deliver a message to him?")
		juliet_give_message(player, npc)
	elseif resp == 4 then
		say(player, "I think you could do better")
		npcsay(npc, "He has his good points")
		npcsay(npc, "He doesn't spend all day on the internet, at least")
	end
end

function talknpc_juliet(player, npc)
	local stage = getvar(player, "rj_stage")
	if stage == 0 then
		if getvar(player, "romeo_mes") ~= 0 then
			say(player, "Juliet, I come from Romeo")
			say(player, "He begs me tell you he cares still")
			npcsay(npc, "Please, Take this message to him")
			juliet_give_message(player, npc)
		else
			juliet_init(player, npc)
		end
	elseif stage == 1 then
		if held(player, "message", 1) then
			npcsay(npc, "Please, deliver the message to Romeo with all speed")
		else
			local failed = getvar(player, "rj_fail")
			if failed == 0 then
				npcsay(npc, "How could you lose this most important message?")
				npcsay(npc, "Please take this message to him, and please don't lose it")
				mes(player, "Juliet gives you another message")
				give(player, "message", 1)
				setvar(player, "rj_fail", 1)
			elseif failed == 1 then
				npcsay(npc, "It seems I cannot trust you with a simple message")
				npcsay(npc, "I am sorry, I need a more reliable messenger")
				setvar(player, "rj_fail", 2)
			elseif failed == 2 then
				npcsay(npc, "I am sorry, I do need a more reliable messenger")
				npcsay(npc, "Can you send any friends my way?")
				npcsay(npc, "Preferably tall, dark and handsome")
			end
		end
	elseif stage == 2 then
		say(player, "I have passed on your message")
		say(player, "Now I go to Father Lawrence for help")
		npcsay(npc, "Yes, he knows many things that can be done")
		npcsay(npc, "I hope you find him soon")
	elseif stage == 3 then
		say(player, "I found the Father. Now I seek the apothecary")
		npcsay(npc, "I do not know where he lives")
		npcsay(npc, "but please, make haste. My father is close")
	elseif stage == 4 then
		if not held(player, "cadava", 1) then
			say(player, "I have to get a potion made for you")
			say(player, "Not done that bit yet though. Still trying")
			npcsay(npc, "Fair luck to you, the end is close")
			return
		end
		say(player, "I have a potion from Father Lawrence")
		say(player, "it should make you seem dead, and get you away from this place")
		mes(player, "You pass the potion to Juliet")
		delay(5)
		remove(player, "cadava", 1)
		npcsay(npc, "Wonderful. I just hope Romeo can remember to get me from the Crypt")
		npcsay(npc, "Many thanks kind friend")
		npcsay(npc, "Please go to Romeo, make sure he understands")
		npcsay(npc, "He can be a bit dense sometimes")
		setvar(player, "rj_stage", 5)
	elseif stage == 5 then
		npcsay(npc, "Have you seen Romeo? He will reward you for your help")
		npcsay(npc, "He is the wealth in this story")
		npcsay(npc, "I am just the glamour")
	elseif stage == 6 then
		npcsay(npc, "I sat in that cold crypt for ages waiting for Romeo")
		npcsay(npc, "That useless fool never showed up")
		npcsay(npc, "And all I got was indigestion. I am done with men like him")
		npcsay(npc, "Now go away before I call my father!")
	end
end

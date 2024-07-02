-- https://classic.runescape.wiki/w/Transcript:Romeo
-- 1e_Luis/Quests/Romeo & Juliet

local function romeo_give_message(player, npc)
	say(player, "Yes, I will tell her how you feel")
	npcsay(npc, "You are the saviour of my heart, thank you.")
	say(player, "err, yes. Ok. Thats.... nice.")
	setvar(player, "romeo_mes", 1)
end

local function romeo_met(player, npc)
	say(player, "Yes, I have seen her")
	say(player, "I think it was her. Blond, stressed")
	npcsay(npc, "Yes, that sounds like her")
	npcsay(npc, "Please tell her I long to be with her")
	local resp = multi(player,
		"Yes, I will tell her",
		"Sorry, I am too busy. Maybe later?")
	if resp == 1 then
		romeo_give_message(player, npc)
	elseif resp == 2 then
		say(player, "Sorry, I am too busy. Maybe later?")
		npcsay(npc, "Well if you do find her, I would be most grateful")
	end
end

local function romeo_notmet(player, npc)
	say(player, "No, but that's girls for you")
	npcsay(npc, "Not my dear Juliet. She is different")
	npcsay(npc, "Could you find her for me?")
	npcsay(npc, "Please tell her I long to be with her")
	local resp = multi(player,
		"Yes, I will tell her how you feel",
		"I can't, it sounds like work for me")
	if resp == 1 then
		romeo_give_message(player, npc)
	elseif resp == 2 then
		say(player, "I can't, it sounds like work to me")
		npcsay(npc, "Well, I guess you are not the romantic type")
		npcsay(npc, "Goodbye")
	end
end

local function romeo_init(player, npc)
	npcsay(npc, "Juliet, Juliet, Juliet! Wherefore Art thou?")
	npcsay(npc, "Kind friend, Have you seen Juliet?")
	npcsay(npc, "Her and her Father seem to have disappeared")
	local resp = multi(player,
		"Yes, I have seen her",
		"No, but that's a girl for you",
		"Can I help find her for you?")
	if resp == 1 then
		romeo_met(player, npc)
	elseif resp == 2 then
		romeo_notmet(player, npc)
	elseif resp == 3 then
		say(player, "Can I help find her for you?")
		npcsay(npc, "Oh would you? That would be wonderful!")
		npcsay(npc, "Please tell her I long to be with her")
		romeo_give_message(player, npc)
	end
end

local function romeo_father(player, npc)
	npcsay(npc, "Did you find the Father? What did he suggest?")
	local resp = multi(player,
		"He seems keen for you marry Juliet",
		"He sent me to the Apothecary")
	if resp == 1 then
		say(player, "He seems keen for you marry Juliet")
		npcsay(npc, "I think he wants some peace. He was our messenger")
		npcsay(npc, "before you were kind enough to help us")
	elseif resp == 2 then
		say(player, "He sent me to the Apothecary")
		npcsay(npc, "I know him. He lives near the town square")
		npcsay(npc, "the small house behind the sloped building")
		npcsay(npc, "Good luck")
	end
end

function talknpc_romeo(player, npc)
	local stage = getvar(player, "rj_stage")
	if stage == 0 then
		if getvar(player, "romeo_mes") ~= 0 then
			npcsay(npc, "Please find my Juliet. I am so, so sad")
			return
		end
		romeo_init(player, npc)
	elseif stage == 1 then
		say(player, "Romeo, I have a message from Juliet")
		if not held(player, "message", 1) then
			say(player, "Except that I seem to have lost it")
			setvar(player, "romeo_unreliable", 1)
		else
			if getvar(player, "romeo_unreliable") ~= 0 then
				npcsay(npc, "Ah, it seems that you can deliver a message after all")
				npcsay(npc, "My faith in you is restored!")
			end
			mes(player, "You pass Juliet's message to Romeo")
			delay(5)
			remove(player, "message", 1)
			npcsay(npc, "Tragic news. Her father is opposing our marriage")
			npcsay(npc, "If her father sees me, he will kill me")
			npcsay(npc, "I dare not go near his lands")
			npcsay(npc, "She says Father Lawrence can help us")
			npcsay(npc, "Please find him for me. Tell him of our plight")
			setvar(player, "rj_stage", 2)
		end
	elseif stage == 2 then
		npcsay(npc, "Please friend, how goes our quest?")
		npcsay(npc, "Father Lawrence must be told. only he can help")
	elseif stage == 3 then
		romeo_father(player, npc)
	elseif stage == 4 then
		if held(player, "cadava", 1) then
			npcsay(npc, "Ah, you have the potion. I was told what to do by the good Father")
			npcsay(npc, "Better get it to Juliet. She knows what is happening")
		else
			npcsay(npc, "I hope the potion is near ready")
			npcsay(npc, "It is the last step for the great plan")
			npcsay(npc, "I hope I will be with my dear one soon")
		end
	elseif stage == 5 then
		say(player, "Romeo, it's all set. Juliet has the potion")
		npcsay(npc, "Ah right")
		npcsay(npc, "What potion would that be then?")
		say(player, "The one to get her to the crypt.")
		npcsay(npc, "Ah right")
		npcsay(npc, "So she is dead then. Ah thats a shame.")
		npcsay(npc, "Thanks for you help anyway.")
		mes(player, "You have completed the quest of Romeo and Juliet")
		setvar(player, "rj_stage", 6)
		giveqp(player, 5)
	elseif stage == 6 then
		npcsay(npc, "I heard Juliet had died. Terrible business")
		npcsay(npc, "Her cousin and I are getting on well though")
		npcsay(npc, "Thanks for your help")
	end
end

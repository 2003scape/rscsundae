-- https://classic.runescape.wiki/w/Transcript:Father_Lawrence

local function lawrence_init(player, npc)
	npcsay(npc, "Hello adventurer, do you seek a quest?")
	local resp = multi(player,
		"I am always looking for a quest",
		"No, I prefer to kill things",
		"Can you recommend a good bar?")
	if resp == 1 then
		say(player, "I am always looking for a quest")
		npcsay(npc, "Well, I see poor Romeo wandering around the square. I think he may need help")
		npcsay(npc, "I was helping him and Juliet to meet, but it became impossible")
		npcsay(npc, "I am sure he can use some help")
	elseif resp == 2 then
		say(player, "No, I just prefer to kill things")
		npcsay(npc, "That's a fine career in these lands")
		npcsay(npc, "There is more that needs killing every day")
	elseif resp == 3 then
		say(player, "Can you recommend a good bar?")
		npcsay(npc, "Drinking will be the death of you")
		npcsay(npc, "But the Blue Moon in the city is cheap enough")
		npcsay(npc, "And providing you buy one drink an hour they let you stay all night")
	end
end

function talknpc_father_lawrence(player, npc)
	if getvar(player, "rj_drunk") ~= 0 then
		npcsay(npc, "Oh to be a father in the times of whiskey")
		npcsay(npc, "I sing and I drink and I wake up in gutters")
		npcsay(npc, "I need a think I drink")
		return
	end
	if getvar(player, "rj_failed") > 1 then
		npcsay(npc, "Oh to be a father in the times of whiskey")
		npcsay(npc, "I sing and I drink and I wake up in gutters")
		npcsay(npc, "Top of the morning to you")
		npcsay(npc, "To err is human, to forgive, quite difficult")
		setvar(player, "rj_drunk", 1)
		return
	end
	local stage = getvar(player, "rj_stage")
	if stage == 2 then
		say(player, "Romeo sent me. Hey says you can help")
		npcsay(npc, "Ah Romeo, yes. A fine lad, but a little bit confused")
		say(player, "Juliet must be rescued from her fathers control")
		npcsay(npc, "I know just the thing. A potion to make her appear dead")
		npcsay(npc, "Then Romeo can collect her from the crypt")
		npcsay(npc, "Go to the Apothecary, tell him I sent you")
		npcsay(npc, "You need some Cadava Potion")
		setvar(player, "rj_stage", 3)
	elseif stage == 3 then
		npcsay(npc, "Ah have you found the Apothecary yet?")
		npcsay(npc, "Remember, Cadava potion, for Father Lawrence")
	else
		lawrence_init(player, npc)
	end
end

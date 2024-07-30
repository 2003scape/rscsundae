-- https://classic.runescape.wiki/w/Transcript:Captain_Rovin
-- rsc-preservation.xyz/Quests/demon-slayer.pcap

local function rovin_lie(player, npc)
	say(player, "I am one of the palace guard")
	npcsay(npc, "No you're not. I know all the palace guards")
	local resp = multi(player,
		"I'm a new recruit",
		"I've had extensive plastic surgery")
	if resp == 1 then
		say(player, "I'm a new recruit")
		npcsay(npc, "I interview all the new recruits")
		npcsay(npc, "I'd know if you were one of them")
		say(player, "That blows my story out of the window then")
		npcsay(npc, "Get out of my sight")
	elseif resp == 2 then
		say(player, "I've had extensive plastic surgery")
		npcsay(npc, "What kind of surgery is that?")
		npcsay(npc, "Never heard of it")
		npcsay(npc, "Besides, you look reasonably healthy")
		npcsay(npc, "Why is this relevant anyway?")
		npcsay(npc, "You still shouldn't be here")
	end
end

local function rovin_important(player, npc)
	local resp = multi(player,
		"There's a demon who wants to invade this city",
		"Erm I forgot",
		"The castle has just received it's ale delivery")
	if resp == 1 then
		say(player, "There's a demon who wants to invade this city")
		npcsay(npc, "Is it a powerful demon?")
		say(player, "Yes, very")
		npcsay(npc, "Well as good as the palace guards are")
		npcsay(npc, "I don't think they're up to taking on a very powerful demon")
		say(player, "No no, it's not them who's going to fight the demon")
		say(player, "It's me")
		npcsay(npc, "What all by yourself?")
		say(player, "Well I am going to use the powerful sword silverlight")
		say(player, "Which I believe you have one of the keys for")
		npcsay(npc, "Yes you're right")
		npcsay(npc, "Here you go")
		mes(player, "Captain Rovin hands you a key")
		give(player, "silverlight key 2", 1)
	elseif resp == 2 then
		say(player, "Erm I forgot")
		npcsay(npc, "Well it can't be that important then")
		say(player, "How do you know?")
		npcsay(npc, "Just go away")
	elseif resp == 3 then
		say(player, "The castle has just received it's ale delivery")
		npcsay(npc, "Now that is important")
		npcsay(npc, "However, I'm the wrong person to speak to about it")
		npcsay(npc, "Go talk to the kitchen staff")
	end
end

function talknpc_captain_rovin(player, npc)
	local resp

	npcsay(npc, "What are you doing up here?")
	npcsay(npc, "Only the palace guards are allowed up here")
	resp = multi(player,
		"I am one of the palace guards",
		"What about the king?",
		"Yes I know but this important")
	if resp == 1 then
		rovin_lie(player, npc)
	elseif resp == 2 then
		say(player, "What about the king?")
		say(player, "Surely you'd let him up here?")
		npcsay(npc, "Well, yes, I suppose we'd let him up")
		npcsay(npc, "He doesn't generally want to come up here")
		npcsay(npc, "But if he did want to")
		npcsay(npc, "He could come up")
		npcsay(npc, "Anyway, you're not the king either")
		npcsay(npc, "So get out of my sight")
	elseif resp == 3 then
		say(player, "Yes I know but this is important")
		npcsay(npc, "Ok, I'm listening")
		npcsay(npc, "Tell me what's so important")
		if getvar(player, "silverlight_stage") == 1 then
			rovin_important(player, npc)
		end
	end
end

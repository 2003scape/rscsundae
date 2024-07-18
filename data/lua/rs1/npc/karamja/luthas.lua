-- https://classic.runescape.wiki/w/Transcript:Luthas
-- RSC 2001/replays master archive/Quest-- WIP/Pirates Treasure/dialogue/luthas dialogue + crate-/dialogue- luthas- scenery- karamja- crate- is full- get paid- leave dialogue.pcap

local function luthas_customs(player, npc)
	say(player, "That customs officer is annoying isn't she?")
	npcsay(npc, "Well I know her pretty well")
	npcsay(npc, "She doesn't cause me any trouble any more")
	npcsay(npc, "She doesn't even search my export crates any more")
	npcsay(npc, "She knows they only contain bananas")
end

local function luthas_intro(player, npc)
	npcsay(npc, "Hello I'm Luthas, I run the banana plantation here")
	local resp = multi(player,
		"Could you offer me employment on your plantation?",
		"That customs officer is annoying isn't she?")
	if resp == 1 then
		say(player, "Could you offer me employment on your plantation?")
		npcsay(npc, "Yes, I can sort something out")
		npcsay(npc, "Yes there's a crate outside ready for loading up on the ship")
		npcsay(npc, "If you could fill it up with bananas")
		npcsay(npc, "I'll pay you 30 gold")
		setvar(player, "luthas_task", 1)
	elseif resp == 2 then
		luthas_customs(player, npc)
	end
end

local function luthas_progress(player, npc)
	npcsay(npc, "Have you completed your task yet?")
	local resp = multi(player,
		"What did I have to do again?",
		"No, the crate isn't full yet")
	if resp == 1 then
		say(player, "What did I have to do again?")
		npcsay(npc, "There's a crate outside ready for loading up on the ship")
		npcsay(npc, "If you could fill it up with bananas")
		npcsay(npc, "I'll pay you 30 gold")
	elseif resp == 2 then
		say(player, "No, the crate isn't full yet")
		npcsay(npc, "Well come back when it is")
	end
end

local function luthas_complete(player, npc)
	say(player, "I've filled a create with bananas")
	npcsay(npc, "Well done here is your payment")
	mes(player, "Luthas hands you 30 coins")
	give(player, "coins", 30)
	setvar(player, "crate_bananas", 0)
	if getvar(player, "rum_stashed") ~= 0 then
		setvar(player, "rum_stashed", 0)
		setvar(player, "rum_sent", 1)
	end
	delay(3)
	local resp = multi(player,
		"Will you pay me for another crate full?",
		"Thankyou, I'll be on my way",
		"So where are these bananas going to be delivered to?",
		"That customs officer is annoying isn't she?")
	if resp == 1 then
		say(player, "Will you pay me for another crate full?")
		npcsay(npc, "Yes certainly")
		npcsay(npc, "If you go outside you should see the old crate has been loaded on to the ship")
		npcsay(npc, "and there is another empty crate in it's place")
		setvar(player, "luthas_task", 1)
	elseif resp == 2 then
		say(player, "Thankyou, I'll be on my way")
	elseif resp == 3 then
		say(player, "So where are these bananas going to be delivered to?")
		npcsay(npc, "I sell them to Wydin who runs a grocery store in Port Sarim")
	elseif resp == 4 then
		luthas_customs(player, npc)
	end
end

function talknpc_luthas(player, npc)
	local stage = getvar(player, "luthas_task")
	if stage == 0 then
		luthas_intro(player, npc)
	else
		if getvar(player, "crate_bananas") == 10 then
			luthas_complete(player, npc)
		else
			luthas_progress(player, npc)
		end
	end
end

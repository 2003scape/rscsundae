-- https://classic.runescape.wiki/w/Transcript:Tramp

local function tramp_donate(player, npc)
	say(player, "Ok here you go")
	remove(player, "coins", 1)
	npcsay(npc, "Thankyou, thats great")

	local resp = multi(player,
		"No problem",
		"So don't I get some sort of quest hint or something now")
	if resp == 1 then
		say(player, "No problem")
	elseif resp == 2 then
		say(player, "So don't I get some sort of quest hint or something now")
		npcsay(npc, "No that's not why I'm asking for money")
		npcsay(npc, "I just need to eat")
	end
end

local function tramp_infiltrate(player, npc)
	say(player, "Any ideas how I could get in there then?")
	npcsay(npc, "Hmm I dunno")
	npcsay(npc, "Your best bet would probably be to get someone else")
	npcsay(npc, "Someone who isn't a member of the phoenix gang")
	npcsay(npc, "To Infiltrate the ranks of the black arm gang")
	npcsay(npc, "If you find someone")
	npcsay(npc, "Tell em to come to me first")
	local resp = multi(player,
		"Ok good plan",
		"Like who?")
	if resp == 1 then
		say(player, "Ok good plan")
	elseif resp == 2 then
		say(player, "Like who?")
		npcsay(npc, "There's plenty of other adventurers about")
		npcsay(npc, "Besides yourself")
		npcsay(npc, "I'm sure if you asked one of them nicely")
		npcsay(npc, "They would help you")
	end
end

local function tramp_phoenix(player, npc)
	npcsay(npc, "No")
	npcsay(npc, "You're a collaborator with the phoenix gang")
	npcsay(npc, "There's no way they'll let you join")
	local resp = multi(player,
		"How did you know I was in the phoenix gang?",
		"Any ideas how I could get in there then?")
	if resp == 1 then
		say(player, "How did you know I was in the phoenix gang?")
		npcsay(npc, "I spend a lot of time on the streets")
		npcsay(npc, "And you hear those sorta things sometimes")
	elseif resp == 2 then
		tramp_infiltrate(player, npc)
	end
end

local function tramp_alleyway(player, npc)
	say(player, "Is there anything down this alleyway?")
	npcsay(npc, "Yes there is actually")
	npcsay(npc, "A notorious gang of thieves and hoodlums")
	npcsay(npc, "Called the blackarm gang")
	setvar(player, "blackarm_known", 1)
	local resp = multi(player,
		"Thanks for the warning",
		"Do you think they would let me join?")
	if resp == 1 then
		say(player, "Thanks for the warning")
		npcsay(npc, "Don't worry about it")
	elseif resp == 2 then
		say(player, "Do you think they would let me join?")
		if getvar(player, "phoenix_stage") >= 2 then
			tramp_phoenix(player, npc)
		elseif getvar(player, "blackarm_stage") >= 2 then
			npcsay(npc, "I was under the impression you were already a member")
		else
			npcsay(npc, "You never know")
			npcsay(npc, "You'll find a lady down there called Katrine")
			npcsay(npc, "Speak to her")
			npcsay(npc, "But don't upset her, she's pretty dangerous")
		end
	end
end

function talknpc_tramp(player, npc)
	npcsay(npc, "Spare some change guv?")
	local resp = multi(player,
		"Sorry I haven't got any",
		"Go get a job",
		"Ok here you go",
		"Is there anything down this alleyway?")
	if resp == 1 then
		say(player, "Sorry I haven't got any")
		npcsay(npc, "Thanks anyway")
	elseif resp == 2 then
		say(player, "Go get a job")
		npcsay(npc, "You startin?")
	elseif resp == 3 then
		tramp_donate(player, npc)
	elseif resp == 4 then
		tramp_alleyway(player, npc)
	end
end

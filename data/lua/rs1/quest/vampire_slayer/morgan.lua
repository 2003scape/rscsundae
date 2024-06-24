-- https://classic.runescape.wiki/w/Transcript:Morgan

local function morgan_seek_help(player, npc)
	npcsay(npc, "I think first you should seek help")
	npcsay(npc, "I have a friend who is a retired vampire hunter")
	npcsay(npc, "Called Dr Harlow")
	npcsay(npc, "He may be able to give you some tips")
	npcsay(npc, "He can normally be found in the Jolly boar inn these days")
	npcsay(npc, "He's a bit of an old soak")
	npcsay(npc, "Mention his old friend Morgan")
	npcsay(
		npc,
		"I'm sure he wouldn't want his old friend to be killed by a vampire"
	)
	say(player, "I'll look him up then")
	setvar(player, "vampire_stage", 1)
end

function talknpc_morgan(player, npc)
	local stage = getvar(player, "vampire_stage")

	if stage == 0 then
		npcsay(npc, "Please please help us, bold hero")
		say(player, "What's the problem?")
		npcsay(
			npc,
			"Our little village has been dreadfully ravaged by an evil vampire"
		)
		npcsay(npc, "There's hardly any of us left")
		npcsay(npc, "We need someone to get rid of him once and for good")

		local resp = multi(
			player,
			"No. vampires are scary",
			"Ok I'm up for an adventure",
			"I tried fighting him. He wouldn't die"
		)

		if resp == 1 then
			say(player, "No. vampires are scary")
			npcsay(npc, "I don't blame you")
		elseif resp == 2 then
			say(player, "Ok I'm up for an adventure")
            morgan_seek_help(player, npc)
        elseif resp == 3 then
            say(player, "I tried fighting him. He wouldn't die")
            npcsay(npc, "Maybe you are not going about it right")
            morgan_seek_help(player, npc)
		end
    else
        npcsay(npc, "How are you doing with your quest?")

        if stage == 1 then
            say(player, "I'm working on it still")
            npcsay(npc, "Please hurry")
            npcsay(npc, "Every day we live in fear of lives")
            npcsay(npc, "That we will be the vampires next victim")
        elseif stage == 2 then
            say(player, "I have slain the foul creature")
            npcsay(npc, "Thank you, thank you")
            npcsay(npc, "You will always be a hero in our village")
        end
	end
end

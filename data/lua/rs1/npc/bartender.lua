-- https://classic.runescape.wiki/w/Transcript:Bartender_(Blue_Moon_Inn)
-- https://classic.runescape.wiki/w/Transcript:Bartender_(Jolly_Boar_Inn)

function talknpc_moon(player, npc)
	npcsay(npc, "What can I do yer for?")
	local resp = multi(player,
		"A glass of your finest ale please",
		"Can you recommend anywhere an adventurer might make his fortune?",
		"Do you know where I can get some good equipment?")
	if resp == 1 then
		say(player, "A glass of your finest ale please")
		npcsay(npc, "No problemo")
		npcsay(npc, "That'll be 2 coins")
		if held(player, "coins", 2) then
			remove(player, "coins", 2)
			give(player, "beer", 1)
			mes(player, "You buy a pint of beer")
		else
			say(player, "Oh dear. I don't seem to have enough money")
		end
	elseif resp == 2 then
		say(player, "Can you recommend anywhere an adventurer might make his fortune?")
		npcsay(npc, "Ooh I don't know if I should be giving away information")
		npcsay(npc, "Makes the computer game too easy")
		resp = multi(player,
			"Oh ah well",
			"Computer game? What are you talking about?",
			"Just a small clue?")
		if resp == 1 then
			say(player, "Oh ah well")
		elseif resp == 2 then
			say(player, "Computer game?")
			say(player, "What are you talking about?")
			npcsay(npc, "This world around us..")
			npcsay(npc, "is all a computer game..")
			npcsay(npc, "called Runescape")
			say(player, "Nope, still don't understand what you are talking about")
			say(player, "What's a computer?")
			npcsay(npc, "It's a sort of magic box thing,")
			npcsay(npc, "which can do all sorts of different things")
			say(player, "I give up")
			say(player, "You're obviously completely mad!")
		elseif resp == 3 then
			say(player, "Just a small clue?")
			npcsay(npc, "Go and talk to the bartender at the Jolly Boar Inn")
			npcsay(npc, "He doesn't seem to mind giving away clues")
		end
	elseif resp == 3 then
		say(player, "Do you know where I can get some good equipment?")
		npcsay(npc, "Well, there's the sword shop across the road,")
		npcsay(npc, "or there's also all sorts of shops up around the market")
	end
end

function talknpc_jolly(player, npc)
	npcsay(npc, "Yes please?")
	local resp = multi(player,
		"I'll have a beer please",
		"Any hints where I can go adventuring?",
		"Heard any good gossip?")
	if resp == 1 then
		say(player, "I'll have a pint of beer please")
		npcsay(npc, "Ok, that'll be two coins")
		if held(player, "coins", 2) then
			remove(player, "coins", 2)
			give(player, "beer", 1)
			mes(player, "You buy a pint of beer")
		else
			say(player, "Oh dear. I don't seem to have enough money")
		end
	elseif resp == 2 then
		say(player, "Any hints on where I can go adventuring?")
		npcsay(npc, "It's funny you should say that")
		npcsay(npc, "An adventurer passed through here, the other day,")
		npcsay(npc, "claiming to have found a dungeon full of treasure,")
		npcsay(npc, "guarded by vicious skeletal warriors")
		npcsay(npc, "He said he found the entrance in a ruined town")
		npcsay(npc, "deep in the woods to the west of here, behind the palace")
		npcsay(npc, "Now how much faith you put in that story is up to you,")
		npcsay(npc, "but it probably wouldn't do any harm to have a look")
		say(player, "Thanks")
		say(player, "I may try that at some point")
	elseif resp == 3 then
		say(player, "Heard any good gossip?")
		npcsay(npc, "I'm not that well up on the gossip out here")
		npcsay(npc, "I've heard that the bartender in the Blue Moon Inn has gone a little crazy")
		npcsay(npc, "He keeps claiming he is part of something called a computer game")
		npcsay(npc, "What that means, I don't know")
		npcsay(npc, "That's probably old news by now though")
	end
end

register_talknpc("moon", talknpc_moon)
register_talknpc("jolly", talknpc_jolly)

-- RSC 2001/replays master archive/NPC (dialogue, notes)/Barmaid/dialogue- Barmaid- all drinks purchased
-- https://classic.runescape.wiki/w/Transcript:Barmaid

function talknpc_barmaid(player, npc)
	say(player, "Hi, what ales are you serving?")
	npcsay(npc, "Well you can either have a nice Asgarnian Ale or a Wizards Mind Bomb")
	npcsay(npc, "Or a Dwarven Stout")
	local resp = multi(player,
		"One Asgarnian Ale please",
		"I'll try the mind bomb",
		"Can I have a Dwarven Stout?",
		"I don't feel like any of those")
	if resp == 1 then
		say(player, "One Asgarnian Ale please")
		npcsay(npc, "That'll be two gold")
		if held(player, "coins", 2) then
			mes(player, "You buy an Asgarnian Ale")
			remove(player, "coins", 2)
			give(player, "asgarnian ale", 1)
		else
			say(player, "Oh dear. I don't seem to have enough money")
		end
	elseif resp == 2 then
		say(player, "I'll try the mind bomb")
		npcsay(npc, "That'll be two gold")
		if held(player, "coins", 2) then
			mes(player, "You buy a pint of Wizard's Mind Bomb")
			remove(player, "coins", 2)
			give(player, "wizard's mind bomb", 1)
		else
			say(player, "Oh dear. I don't seem to have enough money")
		end
	elseif resp == 3 then
		say(player, "Can I have a Dwarven Stout?")
		npcsay(npc, "That'll be three gold")
		if held(player, "coins", 3) then
			mes(player, "You buy a pint of Dwarven Stout")
			remove(player, "coins", 3)
			give(player, "dwarven stout", 1)
		else
			say(player, "Oh dear. I don't seem to have enough money")
		end
	elseif resp == 4 then
		say(player, "I don't feel like any of those")
	end
end

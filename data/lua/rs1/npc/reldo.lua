-- https://classic.runescape.wiki/w/Transcript:Reldo

local function reldo_init(player, npc)
	say(player, "Hello")
	npcsay(npc, "Hello stranger")

	local choices = {}
	if getvar(player, "arrav_stage") == 0 then
		table.insert(choices, "I'm in search of a quest")
	end
	table.insert(choices, "Do you have anything to trade?")
	table.insert(choices, "What do you do?")
	if getvar(player, "sword_stage") > 0 and
	    getvar(player, "imcando_stage") == 0 then
		table.insert(choices, "What do you know about the Imcando dwarves?")
	end

	local resp = multi(player, table.unpack(choices))
	local answer = choices[resp]
	if answer == "I'm in search of a quest" then
		say(player, "I'm in search of a quest")
		npcsay(npc, "I don't think there's any here")
		npcsay(npc, "Let me think actually")
		npcsay(npc, "If you look in a book")
		npcsay(npc, "called the shield of Arrav")
		npcsay(npc, "You'll find a quest in there")
		npcsay(npc, "I'm not sure where the book is mind you")
		npcsay(npc, "I'm sure it's somewhere in here")
		say(player, "Thankyou")
		setvar(player, "arrav_stage", 1)
	elseif answer == "Do you have anything to trade?" then
		say(player, "Do you have anything to trade?")
		npcsay(npc, "No, sorry. I'm not the trading type")
		say(player, "ah well")
	elseif answer == "What do you do?" then
		say(player, "What do you do?")
		npcsay(npc, "I'm the palace librarian")
		say(player, "Ah that's why you're in the library then")
		npcsay(npc, "Yes")
		npcsay(npc, "Though I might be in here even if I didn't work here")
		npcsay(npc, "I like reading")
	elseif answer == "What do you know about the Imcando dwarves?" then
		say(player, "What do you know about the Imcando dwarves?")
		npcsay(npc, "The Imcando Dwarves, you say?")
		npcsay(npc, "They were the world's most skilled smiths about a hundred years ago")
		npcsay(npc, "They used secret knowledge")
		npcsay(npc, "Which they passed down from generation to generation")
		npcsay(npc, "Unfortunatly about a century ago the once thriving race")
		npcsay(npc, "Was wiped out during the barbarian invasions of that time")
		say(player, "So are there any Imcando left at all?")
		npcsay(npc, "A few of them survived")
		npcsay(npc, "But with the bulk of their population destroyed")
		npcsay(npc, "Their numbers have dwindled even further")
		npcsay(npc, "Last I knew there were a couple living in Asgarnia")
		npcsay(npc, "Near the cliffs on the Asgarnian southern peninsula")
		npcsay(npc, "They tend to keep to themselves")
		npcsay(npc, "They don't tend to tell people that they're the descendants of the Imcando")
		npcsay(npc, "Which is why people think that the tribe has died out totally")
		npcsay(npc, "you may have more luck talking to them if you bring them some red berry pie")
		npcsay(npc, "They really like red berry pie")
		setvar(player, "imcando_stage", 1)
	end
end

function talknpc_reldo(player, npc)
	local stage = getvar(player, "arrav_stage")
	if stage == 2 then
		say(player, "OK I've read the book")
		say(player, "Do you know where I can find the Phoenix Gang")
		npcsay(npc, "No I don't")
		npcsay(npc, "I think I know someone who will though")
		npcsay(npc, "Talk to Baraek, the fur trader in the market place")
		npcsay(npc, "I've heard he has connections with the Phoenix Gang")
		say(player, "Thanks, I'll try that")
		setvar(player, "arrav_stage", 3)
	else
		reldo_init(player, npc)
	end
end

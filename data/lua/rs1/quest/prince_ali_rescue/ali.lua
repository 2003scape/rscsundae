-- https://classic.runescape.wiki/w/Transcript:Prince_Ali

function talknpc_prince_ali(player, npc)
	if getvar(player, "prince_ali_stage") == 3 then
		say(player, "Prince, I come to rescue you")
		npcsay(npc, "That is very very kind of you, how do I get out?")
		say(player, "With a disguise, I have removed the Lady Keli")
		say(player, "She is tied up, but will not stay tied up for long")

		local has_equipment = held(player, "princeskey", 1)
			and held(player, "blondwig", 1)
			and held(player, "pinkskirt", 1)
			and held(player, "skinpaste", 1)

		if has_equipment then
			remove(player, "princeskey", 1)
			remove(player, "blondwig", 1)
			remove(player, "pinkskirt", 1)
			remove(player, "skinpaste", 1)

			mes(player, "You hand the disguise and the key to the prince")

			local disguise_prince = changenpc(npc, "princeindisguise")

			npcsay(disguise_prince, "Thankyou my friend, I must leave you now")
			npcsay(disguise_prince, "My father will pay you well for this")
			say(player, "Go to Leela, she is close to here")

			delnpc("princeindisguise")

			mes(player, "The prince has escaped, well done!")
			mes(player, "You are now a friend of Al kharid")
			mes(player, "And may pass through the Al Kharid toll gate for free")

			setvar(player, "prince_ali_stage", 4)

			pause(100, 100)
			addnpc("prince ali", 202, 641)
		else
			npcsay(npc, "You don't seem to have all I need to escape yet")
			npcsay(npc, "I dare not risk death to these people")
		end
	else
		npcsay(npc, "I owe you my life for that escape")
		npcsay(npc, "You cannot help me this time, they know who you are")
		npcsay(npc, "Go in peace, friend of Al Kharid")
	end
end

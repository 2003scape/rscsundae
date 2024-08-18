-- https://classic.runescape.wiki/w/Transcript:Aggie#During_Prince_Ali_Rescue

function aggie_skin_paste(player, npc)
	say(player, "Could you think of a way to make pink skin paste")

	local water

	if held(player, "waterbucket", 1) then
		water = "waterbucket"
	elseif held(player, "waterjug", 1) then
		water = "waterjug"
	end

	local has_ingredients = water ~= nil
		and held(player, "ashes", 1)
		and held(player, "redberries", 1)
		and held(player, "flourpot", 1)

	if has_ingredients then
		npcsay(npc, "Yes I can, you have the ingredients for it already")
		npcsay(npc, "Would you like me to mix you some?")

		local resp = multi(
			player,
			"Yes please, mix me some skin paste",
			"No thankyou, I don't need paste"
		)

		if resp == 1 then
			say(player, "Yes please, mix me some skin paste")
			npcsay(npc, "That should be simple, hand the things to Aggie then")
			remove(player, water, 1)
			remove(player, "ashes", 1)
			remove(player, "redberries", 1)
			remove(player, "flourpot", 1)
			mes(player, "You hand ash, flour, water, and redberries to Aggie")
			mes(player, "She tips it into a cauldron and mutters some words")
			npcsay(
				npc,
				"Tourniquet, Fenderbaum, Tottenham, MonsterMunch, MarbleArch"
			)
			delay(3)
			mes(player, "Aggie hands you the skin paste")
			give(player, "skinpaste", 1)
			npcsay(npc, "There you go dearie, your skin potion")
			npcsay(npc, "That will make you look good at the Varrock dances")
		elseif resp == 2 then
			say(player, "No thank you, I don't need skin paste")
			npcsay(npc, "Okay dearie, thats always your choice")
		end
	else
		npcsay(npc, "Why, its one of my most popular potions")
		npcsay(npc, "The women here, they like to have smooth looking skin")
		npcsay(npc, "(and I must admit, some of the men buy it too)")
		npcsay(npc, "I can make it for you, just get me what needed")
		say(player, "What do you need to make it?")
		npcsay(npc, "Well deary, you need a base for the paste")
		npcsay(npc, "That's a mix of ash, flour and water")
		npcsay(npc, "Then you need red berries to colour it as you want")
		npcsay(npc, "bring me those four items and I will make you some")
	end
end

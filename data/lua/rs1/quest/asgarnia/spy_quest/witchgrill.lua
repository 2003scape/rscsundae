-- https://classic.runescape.wiki/w/Transcript:Witch

function oploc1_witchgrill(player, x, y)
	if getvar(player, "fortress_stage") == 1 then
		local knight = nearnpc(player, "grillknight")
		npcsay(knight, "So how's the secret weapon coming along?")

		local witch = nearnpc(player, "fortwitch")
		npcsay(witch, "The invincibility potion is almost ready")
		npcsay(witch, "It's taken me five years but it's almost ready")
		npcsay(witch, "Greldo the Goblin here")
		npcsay(witch, "Is just going to fetch the last ingredient for me")
		npcsay(witch, "It's a specially grown cabbage")
		npcsay(witch, "Grown by my cousin Helda who lives in Draynor Manor")
		npcsay(witch, "The soil there is slightly magical")
		npcsay(witch, "And it gives the cabbages slight magic properties")
		npcsay(witch, "Not to mention the trees")
		npcsay(witch, "Now remember Greldo only a Draynor Manor cabbage will do")
		npcsay(witch, "Don't get lazy and bring any old cabbage")
		npcsay(witch, "That would entirely wreck the potion")

		local greldo = nearnpc(player, "greldo")
		npcsay(greldo, "Yeth Mithreth")

		setvar(player, "fortress_stage", 2)
	else
		-- 1e_Luis/Quests/Black Knights Fortress
		mes(player, "I can't hear much right now")
	end
end

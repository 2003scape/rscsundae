-- 1e_Luis/Quests/Witchs Potion

function oploc2_hettycauldron(player, x, y)
	local stage = getvar(player, "hetty_stage")
	if stage ~= 2 then
		say(player, "I'd rather not")
		say(player, "It doesn't look very tasty")
	else
		mes(player, "You drink from the cauldron")
		delay(3)
		mes(player, "You feel yourself imbued with power")
		delay(3)
		mes(player, "@gre@Well done you have completed the witches potion quest")
		advancestat(player, STAT_MAGIC, 900, 200)
		setvar(player, "hetty_stage", 3)
		giveqp(player, 1)
	end
end

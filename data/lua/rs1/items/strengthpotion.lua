-- ./rscplus-replays-master/RSC 2001/replays master archive/Stat effects/Consumable Stat effects/Potions/strength potion- (level 46 base)
-- https://classic.runescape.wiki/w/Strength_Potion

function drink_strength(player)
	mes(player, "You drink some of your strength potion")
	addstat(player, STAT_STRENGTH, 3, 10)
end

function drink_strength_dose4(player)
	drink_strength(player)
	remove(player, "strength4", 1)
	give(player, "3dose1strength", 1)
	delay(1)
	mes(player, "You have 3 doses of potion left")
end

function drink_strength_dose3(player)
	drink_strength(player)
	remove(player, "3dose1strength", 1)
	give(player, "2dose1strength", 1)
	delay(1)
	mes(player, "You have 2 doses of potion left")
end

function drink_strength_dose2(player)
	drink_strength(player)
	remove(player, "2dose1strength", 1)
	give(player, "1dose1strength", 1)
	delay(1)
	mes(player, "You have 1 doses of potion left")
end

function drink_strength_dose1(player)
	drink_strength(player)
	remove(player, "1dose1strength", 1)
	give(player, "emptyvial", 1)
	delay(1)
	mes(player, "You have finished your potion")
end

-- inconsistency in config46.jag, probably bugged at the time
register_useobj("strength4", drink_strength_dose4)
register_useobj("3dose1strength", drink_strength_dose3)
register_useobj("2dose1strength", drink_strength_dose2)
register_useobj("1dose1strength", drink_strength_dose1)

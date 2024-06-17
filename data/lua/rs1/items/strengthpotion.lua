-- ./rscplus-replays-master/RSC 2001/replays master archive/Stat effects/Consumable Stat effects/Potions/strength potion- (level 46 base)
-- https://classic.runescape.wiki/w/Strength_Potion

function drink_strength(player)
	mes(player, "You drink some of your strength potion")
	addstat(player, STAT_STRENGTH, 3, 10)
end

-- inconsistency in config46.jag, probably bugged at the time
function useobj_strength4(player)
	drink_strength(player)
	remove(player, "strength4", 1)
	give(player, "3dose1strength", 1)
	delay(1)
	mes(player, "You have 3 doses of potion left")
end

function useobj_3dose1strength(player)
	drink_strength(player)
	remove(player, "3dose1strength", 1)
	give(player, "2dose1strength", 1)
	delay(1)
	mes(player, "You have 2 doses of potion left")
end

function useobj_2dose1strength(player)
	drink_strength(player)
	remove(player, "2dose1strength", 1)
	give(player, "1dose1strength", 1)
	delay(1)
	mes(player, "You have 1 doses of potion left")
end

function useobj_1dose1strength(player)
	drink_strength(player)
	remove(player, "1dose1strength", 1)
	delay(1)
	mes(player, "You have finished your potion")
end

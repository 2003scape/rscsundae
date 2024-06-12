-- https://oldschool.runescape.wiki/w/Kebab

function eat_kebab(player)
	mes(player, "@que@You eat the Kebab")
	remove(player, "kebab", 1)
	if random(64) then
		-- rscplus-replays-master/Logg/Tylerbeg/05-29-2018\ 12.57.24\ bragging\ about\ santa\ hat\ in\ draynor
		if statdown(player, STAT_HITS) then
			mes(player, "@que@That was a good kebab")
			delay(1)
			mes(player, "@que@You feel a lot better")
			healstat(player, STAT_HITS, 6, 14)
		end
	elseif random(8) then
		-- XXX i can't find a recording of this,
		-- assume the message is the same as the poison chalice
		mes(player, "@que@That tasted a bit dodgy")
		delay(1)
		mes(player, "@que@You feel a bit ill")
		substat(player, STAT_HITS, 3, 0)
	elseif random(8) then
		-- XXX i can't find a recording of this,
		-- assume the message is the same as the poison chalice
		mes(player, "@que@That tasted a bit dodgy")
		delay(1)
		mes(player, "@que@You feel very ill")
		substat(player, STAT_ATTACK, 3, 0)
		substat(player, STAT_DEFENSE, 3, 0)
		substat(player, STAT_STRENGTH, 3, 0)
		substat(player, STAT_HITS, 4, 0)
	elseif random(8) then
		-- XXX i can't find a recording of this,
		-- assume the message is the same as the poison chalice
		mes(player, "@que@Wow that was an amazing kebab!!")
		delay(1)
		mes(player, "@que@You feel really invigorated")
		addstat(player, STAT_ATTACK, 3, 0)
		addstat(player, STAT_DEFENSE, 3, 0)
		addstat(player, STAT_STRENGTH, 3, 0)
		healstat(player, STAT_HITS, 7, 24)
	elseif random(8) then
		-- XXX i can't find a recording of this,
		-- assume the message is the same as RS2
		mes(player, "@que@That kebab didn't seem to do a lot")
	else
		-- rscplus-replays-master/RSC 2001/replays master archive/Stat effects/Consumable Stat effects/Healing/kebab- consume- heals
		-- should be roughly 160/256 chance
		if statdown(player, STAT_HITS) then
			healstat(player, STAT_HITS, 3, 7)
			mes(player, "@que@It heals some health")
		end
	end
end

register_useobj("kebab", eat_kebab)

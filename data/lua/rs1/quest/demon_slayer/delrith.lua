-- https://classic.runescape.wiki/w/Transcript:Delrith
-- pcaps/BerryPi/Demon Slayer.pcap
-- RSC 2001/3/f2p (not 100% complete)/dialogue- delrith- 100% demon slayer- attack- talk to- both fail.pcap

local function delrith_wrong_chant(player)
	mes(player, "As you chant, Delrith is sucked towards the vortex")
	delay(3)
	mes(player, "Suddenly the vortex closes")
	delay(3)
	mes(player, "And Delrith is still here")
	delay(3)
	mes(player, "That was the wrong incantation")
end

function attacknpc_delrith(player, npc)
	if getvar(player, "silverlight_stage") < 1 then
		say(player, "I'd rather not. He looks scary")
		nodefault(player)
		return
	end
	if getvar(player, "delrith_stage") >= 2 then
		mes(player, "You've already done that quest")
		nodefault(player)
		return
	end
	if not worn(player, "silverlight", 1) then
		say(player, "Maybe I'd better wield silverlight first")
		nodefault(player)
	else
		attacknpc_demon(player, npc)
	end
end

function killnpc_delrith(player, npc, x, y)
	npchealstat(npc, STAT_HITS, 0, 100)
	mes(player, "As you strike Delrith a vortex opens up")
	delay(3)
	say(player, "Now what was that incantation again")
	delay(3)
	local resp = multi(player,
		"Carlem Gabindo Purchai Zaree Camerinthum",
		"Purchai Zaree Gabindo Carlem Camerinthum",
		"Purchai Camerinthum Aber Gabindo Carlem",
		"Carlem Aber Camerinthum Purchai Gabindo")
	if resp == 1 then
		say(player, "Carlem Gabindo Purchai Zaree Camerinthum")
		delrith_wrong_chant(player)
	elseif resp == 2 then
		say(player, "Purchai Zaree Gabindo Carlem Camerinthum")
		delrith_wrong_chant(player)
	elseif resp == 3 then
		say(player, "Purchai Camerinthum Aber Gabindo Carlem")
		delrith_wrong_chant(player)
	elseif resp == 4 then
		say(player, "Carlem Aber Camerinthum Purchai Gabindo")
		mes(player, "Delrith is sucked back into the dark dimension from which he came")
		delay(3)
		npcsubstat(npc, STAT_HITS, 0, 100)
		mes(player, "You have completed the demonslayer quest")
		setvar(player, "delrith_stage", 2)
		giveqp(player, 3)
	end
end

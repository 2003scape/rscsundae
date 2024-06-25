-- rsc-preservation.xyz/Quests/Vampire Slayer

function attacknpc_vampire(player, npc)
	if held(player, "garlic", 1) then
		-- https://github.com/2004Scape/Server/blob/438fb2082a1440d82cf7f2a9c7dd32b28438a465/data/src/scripts/quests/quest_vampire/scripts/count_draynor.rs2#L12
		if not npcstatdown(npc, STAT_HITS) then
			npcsubstat(npc, STAT_ATTACK, 10, 0)
			npcsubstat(npc, STAT_DEFENSE, 40, 0)
			npcsubstat(npc, STAT_STRENGTH, 10, 0)
		end
		mes(player, "The vampire appears to weaken")
	end
end

function killnpc_vampire(player, npc, x, y)
	local stage = getvar(player, "vampire_stage")

	if worn(player, "stake", 1) and held(player, "hammer", 1) then
		remove(player, "stake", 1)
		mes(player, "You hammer the stake in to the vampires chest!")

		if stage == 1 then
			advancestat(player, STAT_ATTACK, 1300, 600)
			mes(player, "@gre@Well done you have completed the vampire slayer quest")
			setvar(player, "vampire_stage", 2)
			giveqp(player, 3)
		end
	else
		mes(player, "The vampire seems to regenerate")
		npchealstat(npc, STAT_HITS, 0, 100)
	end
end

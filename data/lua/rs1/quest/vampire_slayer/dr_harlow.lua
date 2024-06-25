-- rsc-preservation.xyz/Quests/Vampire Slayer

local function dr_harlow_give_beer(player, npc)
	say(player, "Ok mate")

	if held(player, "beer", 1) then
		mes(player, "You give a beer to Dr Harlow")
		remove(player, "beer", 1)
		npcsay(npc, "Cheersh matey")
		return true
	end

	say(player, "I'll just go and buy one")
	return false
end

local function dr_harlow_kill_vampires(player, npc)
	say(player, "So tell me how to kill vampires then")
	npcsay(npc, "Yesh yesh vampires I was very good at killing em once")
	mes(player, "Dr Harlow appears to sober up slightly")
	npcsay(npc, "Well you're gonna to kill it with a stake")
	npcsay(npc, "Otherwishe he'll just regenerate")
	npcsay(npc, "Yes your killing blow must be done with a stake")
	npcsay(npc, "I jusht happen to have one on me")
	mes(player, "Dr Harlow hands you a stake")
	give(player, "stake", 1)
	npcsay(npc, "You'll need a hammer to hand to drive it in properly as well")
	npcsay(npc, "One last thing")
	npcsay(npc, "It's wise to carry garlic with you")
	npcsay(npc, "Vampires are weakened somewhat if they can smell garlic")
	npcsay(npc, "Dunno where you'd find that though")
	npcsay(npc, "Remember even then a vampire is a dangeroush foe")
	say(player, "Thank you very much")
end

function talknpc_dr_harlow(player, npc)
	local stage = getvar(player, "vampire_stage")

	npcsay(npc, "Buy me a drrink pleassh")

	local resp

	if stage == 1 and (not held(player, "stake", 1)) then
		resp = multi(player,
			"No you've had enough",
			"Ok mate",
			"Morgan needs your help")
	else
		resp = multi(player,
			"No you've had enough",
			"Ok mate")
	end

	if resp == 1 then
		say(player, "No you've had enough")
	elseif resp == 2 then
		dr_harlow_give_beer(player, npc)
	elseif resp == 3 then
		say(player, "Morgan needs your help")
		npcsay(npc, "Morgan you shhay?")
		say(player, "His village is being terrorised by a vampire")
		say(player, "He wanted me to ask you how i should go about stopping it")
		npcsay(npc, "Buy me a beer then i'll teash you what you need to know")

		local resp = multi(
			player,
			"Ok mate",
			"But this is your friend Morgan we're talking about"
		)

		if resp == 1 then
			if dr_harlow_give_beer(player, npc) then
				dr_harlow_kill_vampires(player, npc)
			end
		elseif resp == 2 then
			say(player, "But this is your friend Morgan we're talking about")
			npcsay(npc, "Buy ush a drink anyway")
		end
	end
end

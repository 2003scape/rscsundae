-- https://classic.runescape.wiki/w/Transcript:Aggie
-- RSC 2001/replays master archive/NPC (dialogue, notes)/Aggie/dialogue- aggie- includes her stealing flour for insults-
-- RSC 2001/replays master archive/NPC (dialogue, notes)/Aggie/dialogue- aggie- stealing 20 coins and stealing pot of flour
-- Logg/Tylerbeg/08-02-2018 13.22.33 draynor village.pcap
-- Revisionism/06-15-2018 04.23.22 goblin diplomacy.pcap

local function aggie_make_dye(player, npc, colour)
	local resp = multi(player,
		"Okay, make me some some " .. colour .. " dye please",
		"I don't think I have all the ingredients yet",
		"I can do without dye at that price")
	if resp == 1 then
		say(player, "Okay, make me some " .. colour .. " dye please")
		if colour == "red" then
			if not held(player, "redberries", 3) then
				mes(player, "You don't have enough berries to make the red dye!")
				return
			end
		elseif colour == "yellow" then
			if not held(player, "onion", 2) then
				mes(player, "You don't have enough onions to make the yellow dye!")
				return
			end
		elseif colour == "blue" then
			if not held(player, "woad leaf", 2) then
				mes(player, "You don't have enough woad leaves to make the blue dye!")
				return
			end
		end
		if not held(player, "coins", 5) then
			mes(player, "You don't have enough coins to pay for the dye!")
			return
		end
		if colour == "red" then
			remove(player, "redberries", 3)
			remove(player, "coins", 5)
			mes(player, "You hand the berries and payment to Aggie")
			mes(player, "she takes a red bottle from nowhere and hands it to you")
			give(player, "reddye", 1)
		elseif colour == "yellow" then
			remove(player, "onion", 2)
			remove(player, "coins", 5)
			mes(player, "You hand the onions and payment to Aggie")
			mes(player, "she takes a yellow bottle from nowhere and hands it to you")
			give(player, "yellowdye", 1)
		elseif colour == "blue" then
			remove(player, "woad leaf", 2)
			remove(player, "coins", 5)
			mes(player, "You hand the woad leaves and payment to Aggie")
			mes(player, "she takes a blue bottle from nowhere and hands it to you")
			give(player, "bluedye", 1)
		end
	elseif resp == 2 then
		say(player, "I don't think I have all the ingredients yet")
		npcsay(npc, "You know what you need to get now, come back when you have them")
		npcsay(npc, "goodbye for now")
	elseif resp == 3 then
		say(player, "I can do without dye at that price")
		npcsay(npc, "Thats your choice, but I would think you have killed for less")
		npcsay(npc, "I can see it in your eyes")
	end
end

local function aggie_dyes(player, npc)
	local resp = multi(player,
		"What do you need to make some red dye please",
		"What do you need to make some yellow dye please",
		"What do you need to make some blue dye please",
		"No thanks, I am happy the colour I am")
	if resp == 1 then
		say(player, "What do you need to make some red dye please")
		npcsay(npc, "3 lots of Red berries, and 5 coins, to you")
		aggie_make_dye(player, npc, "red")
	elseif resp == 2 then
		say(player, "What do you need to make some yellow dye please")
		npcsay(npc, "Yellow is a strange colour to get, comes from onion skins")
		npcsay(npc, "I need 2 onions, and 5 coins to make yellow")
		aggie_make_dye(player, npc, "yellow")
	elseif resp == 3 then
		say(player, "What do you need to make some blue dye please")
		npcsay(npc, "2 woad leaves, and 5 coins, to you")
		aggie_make_dye(player, npc, "blue")
	elseif resp == 4 then
		say(player, "No thanks, I am happy the colour I am")
		npcsay(npc, "You are easily pleased with yourself then")
		npcsay(npc, "when you need dyes, come to me")
	end
end

function talknpc_aggie(player, npc)
	npcsay(npc, "What can I help you with?")
	local prince_ali_stage = getvar(player, "prince_ali_stage")
	local resp
	if prince_ali_stage == 2 or prince_ali_stage == 3 then
		resp = multi(player,
			"Could you think of a way to make pink skin paste",
			"What could you make for me",
			"Cool, do you turn people into frogs?",
			"You mad old witch, you can't help me",
			"Can you make dyes for me please")
	else
		resp = multi(player,
			"What could you make for me",
			"Cool, do you turn people into frogs?",
			"You mad old witch, you can't help me",
			"Can you make dyes for me please")
		resp = resp + 1
	end
	if resp == 1 then
		-- defined in ../quest/prince_ali_rescue/aggie.lua
		aggie_skin_paste(player, npc)
	elseif resp == 2 then
		say(player, "What could you make for me")
		npcsay(npc, "I mostly just make what I find pretty")
		npcsay(npc, "I sometimes make dye for the womens clothes, brighten the place up")
		npcsay(npc, "I can make red,yellow and blue dyes would u like some")
		aggie_dyes(player, npc)
	elseif resp == 3 then
		say(player, "Cool, do you turn people into frogs?")
		npcsay(npc, "Oh, not for years, but if you met a talking chicken,")
		npcsay(npc, "You have probably met the professor in the Manor north of here")
		npcsay(npc, "A few years ago it was flying fish, that machine is a menace")
	elseif resp == 4 then
		say(player, "You mad old witch, you can't help me")
		npcsay(npc, "Oh, you like to call a witch names, do you?")
		if held(player, "coins", 1) then
			remove(player, "coins", 20)
			mes(player, "Aggie waves her hands about, and you seem to be 20 coins poorer")
			npcsay(npc, "Thats a fine for insulting a witch, you should learn some respect")
		elseif held(player, "flourpot", 1) then
			remove(player, "flourpot", 1)
			mes(player, "Aggie waves her hands near you, and you seem to have lost some flour")
			npcsay(npc, "Thankyou for your kind present of flour")
			npcsay(npc, "I am sure you never meant to insult me")
		else
			npcsay(npc, "You should be careful about insulting a Witch")
			npcsay(npc, "You never know what shape you could wake up in")
		end
	elseif resp == 5 then
		say(player, "Can you make dyes for me please")
		npcsay(npc, "What sort of dye would you like? Red, yellow or Blue?")
		aggie_dyes(player, npc)
	end
end

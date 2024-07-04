-- https://classic.runescape.wiki/w/Transcript:Niles
-- RSC 2001/10/certing- fail- you dont have that many certificates.pcap
-- RSC 2001/10/certing- fail- you dont have that many fish.pcap
-- RSC 2001/10/certing- success- 1 cert to 5 cooked lobsters.pcap
-- RSC 2001/10/certing- success- 5 cooked lobsters to 1 cert.pcap

local function certs_for_fish(player, npc)
	local item_type
	local cert_type

	say(player, "I have some certificates to trade in")

	mes(player, "@que@what sort of certificate do you wish to trade in?")
	local resp_item = multi(player,
		"Swordfish",
		"Raw swordfish",
		"Lobster",
		"Raw lobster")

	if resp_item == 1 then
		item_type = "swordfish"
		cert_type = "swordfish certificate"
	elseif resp_item == 2 then
		item_type = "raw swordfish"
		cert_type = "raw swordfish certificate"
	elseif resp_item == 3 then
		item_type = "lobster"
		cert_type = "lobster certificate"
	elseif resp_item == 4 then
		item_type = "raw lobster"
		cert_type = "raw lobster certificate"
	end

	mes(player, "@que@How many certificates do you wish to trade in?")
	local resp_amount = multi(player,
		"One", "two", "Three", "four", "five")

	if held(player, cert_type, resp_amount) then
		mes(player, "@que@You exchange your certificates for fish")
		remove(player, cert_type, resp_amount)
		give(player, item_type, resp_amount * 5)
	else
		mes(player, "@que@You don't have that many certificates")
	end
end

local function fish_for_certs(player, npc)
	local item_type
	local cert_type

	say(player, "I have some fish to trade in")

	mes(player, "@que@what sort of fish do you wish to trade in?")
	local resp_item = multi(player,
		"Lobster",
		"Raw Lobster",
		"swordfish",
		"Raw swordfish")

	if resp_item == 1 then
		item_type = "lobster"
		cert_type = "lobster certificate"
	elseif resp_item == 2 then
		item_type = "raw lobster"
		cert_type = "raw lobster certificate"
	elseif resp_item == 3 then
		item_type = "swordfish"
		cert_type = "swordfish certificate"
	elseif resp_item == 4 then
		item_type = "raw swordfish"
		cert_type = "raw swordfish certificate"
	end

	-- real typo
	mes(player, "@que@How many fishs do you wish to trade in?")
	local resp_amount = multi(player,
		"five", "ten", "Fifteen", "Twenty", "Twentyfive")

	if held(player, item_type, resp_amount * 5) then
		mes(player, "@que@You exchange your fish for certificates")
		remove(player, item_type, resp_amount * 5)
		give(player, cert_type, resp_amount)
	else
		mes(player, "@que@You don't have that much fish")
	end
end

function talknpc_niles(player, npc)
	npcsay(npc, "Welcome to my fish exchange stall")
	local resp = multi(player,
		"I have some certificates to trade in",
		"I have some fish to trade in",
		"What is a fish exchange stall?")
	if resp == 1 then
		certs_for_fish(player, npc)
	elseif resp == 2 then
		fish_for_certs(player, npc)
	elseif resp == 3 then
		say(player, "What is a fish exchange store?")
		npcsay(npc, "You may exchange your fish here")
		npcsay(npc, "For certificates which are light and easy to carry")
		npcsay(npc, "You can carry many of these certificates at once unlike fish")
		npcsay(npc, "5 fish will give you one certificate")
		npcsay(npc, "You may also redeem these certificates here for fish again")
		npcsay(npc, "The advantage of doing this is")
		npcsay(npc, "You can trade large amounts of fish with other players quickly and safely")
	end
end

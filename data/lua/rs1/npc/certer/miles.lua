-- https://classic.runescape.wiki/w/Transcript:Miles
-- Logg/Tylerbeg/08-01-2018 15.51.37 cleaning inventory.pcap

local function certs_for_bars(player, npc)
	local item_type
	local cert_type

	say(player, "I have some certificates to trade in")

	mes(player, "@que@what sort of certificate do you wish to trade in?")
	local resp_item = multi(player,
		"iron bar", "Steel bar", "mithril bar", "Gold bar", "silver bar")

	if resp_item == 1 then
		item_type = "iron bar"
		cert_type = "iron bar certificate"
	elseif resp_item == 2 then
		item_type = "steel bar"
		cert_type = "steel bar certificate"
	elseif resp_item == 3 then
		item_type = "mithril bar"
		cert_type = "mithril bar certificate"
	elseif resp_item == 4 then
		item_type = "gold bar"
		cert_type = "gold bar certificate"
	elseif resp_item == 5 then
		item_type = "silver bar"
		cert_type = "silver bar certificate"
	end

	mes(player, "@que@How many certificates do you wish to trade in?")
	local resp_amount = multi(player,
		"One", "two", "Three", "four", "five")

	if held(player, cert_type, resp_amount) then
		mes(player, "@que@You exchange your certificates for bars")
		remove(player, cert_type, resp_amount)
		give(player, item_type, resp_amount * 5)
	else
		mes(player, "@que@You don't have that many certificates")
	end
end

local function bars_for_certs(player, npc)
	local item_type
	local cert_type

	say(player, "I have some bars to trade in")

	mes(player, "@que@what sort of bars do you wish to trade in?")
	local resp_item = multi(player,
		"iron bar", "Steel bar", "mithril bar", "Gold bar", "silver bar")

	if resp_item == 1 then
		item_type = "iron bar"
		cert_type = "iron bar certificate"
	elseif resp_item == 2 then
		item_type = "steel bar"
		cert_type = "steel bar certificate"
	elseif resp_item == 3 then
		item_type = "mithril bar"
		cert_type = "mithril bar certificate"
	elseif resp_item == 4 then
		item_type = "gold bar"
		cert_type = "gold bar certificate"
	elseif resp_item == 5 then
		item_type = "silver bar"
		cert_type = "silver bar certificate"
	end

	mes(player, "@que@How many bars do you wish to trade in?")
	local resp_amount = multi(player,
		"five", "ten", "Fifteen", "Twenty", "Twentyfive")

	if held(player, item_type, resp_amount * 5) then
		mes(player, "@que@You exchange your bars for certificates")
		remove(player, item_type, resp_amount * 5)
		give(player, cert_type, resp_amount)
	else
		mes(player, "@que@You don't have that many bars")
	end
end

function talknpc_miles(player, npc)
	npcsay(npc, "Welcome to my bar exchange stall")
	local resp = multi(player,
		"I have some certificates to trade in",
		"I have some bars to trade in",
		"What is a bar exchange stall?")
	if resp == 1 then
		certs_for_bars(player, npc)
	elseif resp == 2 then
		bars_for_certs(player, npc)
	elseif resp == 3 then
		say(player, "What is a bar exchange store?")
		npcsay(npc, "You may exchange your bars here")
		npcsay(npc, "For certificates which are light and easy to carry")
		npcsay(npc, "You can carry many of these certificates at once unlike bars")
		npcsay(npc, "5 bars will give you one certificate")
		npcsay(npc, "You may also redeem these certificates here for bars again")
		npcsay(npc, "The advantage of doing this is")
		npcsay(npc, "You can trade large amounts of bars with other players quickly and safely")
	end
end

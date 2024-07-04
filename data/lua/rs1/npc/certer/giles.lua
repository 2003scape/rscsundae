-- https://classic.runescape.wiki/w/Transcript:Giles
-- Logg/Tylerbeg/06-13-2018 18.44.52 start superheat magic training.pcap

local function certs_for_ore(player, npc)
	local item_type
	local cert_type

	say(player, "I have some certificates to trade in")

	mes(player, "@que@what sort of certificate do you wish to trade in?")
	local resp_item = multi(player,
		"iron", "Coal", "mithril", "Gold", "silver")

	if resp_item == 1 then
		item_type = "iron ore"
		cert_type = "iron ore certificate"
	elseif resp_item == 2 then
		item_type = "coal"
		cert_type = "coal certificate"
	elseif resp_item == 3 then
		item_type = "mithril ore"
		cert_type = "mithril ore certificate"
	elseif resp_item == 4 then
		item_type = "gold"
		cert_type = "gold certificate"
	elseif resp_item == 5 then
		item_type = "silver"
		cert_type = "silver certificate"
	end

	mes(player, "@que@How many certificates do you wish to trade in?")
	local resp_amount = multi(player,
		"One", "two", "Three", "four", "five")

	if held(player, cert_type, resp_amount) then
		mes(player, "@que@You exchange your certificates for ore")
		remove(player, cert_type, resp_amount)
		give(player, item_type, resp_amount * 5)
	else
		mes(player, "@que@You don't have that many certificates")
	end
end

local function ore_for_certs(player, npc)
	local item_type
	local cert_type

	say(player, "I have some ore to trade in")

	mes(player, "@que@what sort of ore do you wish to trade in?")
	local resp_item = multi(player,
		"iron", "Coal", "mithril", "Gold", "silver")

	if resp_item == 1 then
		item_type = "iron ore"
		cert_type = "iron ore certificate"
	elseif resp_item == 2 then
		item_type = "coal"
		cert_type = "coal certificate"
	elseif resp_item == 3 then
		item_type = "mithril ore"
		cert_type = "mithril ore certificate"
	elseif resp_item == 4 then
		item_type = "gold"
		cert_type = "gold certificate"
	elseif resp_item == 5 then
		item_type = "silver"
		cert_type = "silver certificate"
	end

	mes(player, "@que@How many ores do you wish to trade in?")
	local resp_amount = multi(player,
		"five", "ten", "Fifteen", "Twenty", "Twentyfive")

	if held(player, item_type, resp_amount * 5) then
		mes(player, "@que@You exchange your ore for certificates")
		remove(player, item_type, resp_amount * 5)
		give(player, cert_type, resp_amount)
	else
		mes(player, "@que@You don't have that much ore")
	end
end

function talknpc_giles(player, npc)
	npcsay(npc, "Welcome to my ore exchange stall")
	local resp = multi(player,
		"I have some certificates to trade in",
		"I have some ore to trade in",
		"What is a ore exchange stall?")
	if resp == 1 then
		certs_for_ore(player, npc)
	elseif resp == 2 then
		ore_for_certs(player, npc)
	elseif resp == 3 then
		say(player, "What is an ore exchange store?")
		npcsay(npc, "You may exchange your ores here")
		npcsay(npc, "For certificates which are light and easy to carry")
		npcsay(npc, "You can carry many of these certificates at once unlike ores")
		npcsay(npc, "5 ores will give you one certificate")
		npcsay(npc, "You may also redeem these certificates here for ores again")
		npcsay(npc, "The advantage of doing this is")
		npcsay(npc, "You can trade large amounts of ores with other players quickly and safely")
	end
end

-- RSC 2001/7/dialogue- wydin-50% pirates treasure door- all- shop
local function wydin_intro(player, npc)
	npcsay(npc, "Welcome to my foodstore")
	npcsay(npc, "Would you like to buy anything")
	local resp = multi(player,
		"yes please",
		"No thankyou",
		"what can you recommend?")
	if resp == 1 then
		say(player, "Yes please")
		openshop(player, "foodshop")
	elseif resp == 3 then
		say(player, "What can you recommend?")
		npcsay(npc, "We have this really exotic fruit")
		npcsay(npc, "All the way from Karamja")
		npcsay(npc, "It's called a banana")
	end
end

-- RSC 2001/7/dialogue- wydin-50% pirates treasure door- open- fail- get hired- success.pcap
local function wydin_guard(player, npc)
	npcsay(npc, "Heh you can't go in there")
	npcsay(npc, "Only employees of the grocery store can go in")
	local resp = multi(player,
		"Well can I get a job here?",
		"Sorry I didn't realise")
	if resp == 1 then
		say(player, "Can I get a job here?")
		npcsay(npc, "Well you're keen I'll give you that")
		npcsay(npc, "Ok I'll give you a go")
		npcsay(npc, "Have you got your own apron?")
		if held(player, "whiteapron", 1) then
			say(player, "Yes I have")
			npcsay(npc, "Wow you are prepared, you're hired")
			npcsay(npc, "Go through to the back and tidy up for me please")
			setvar(player, "wydin_hired", 1)
		else
			say(player, "No")
			npcsay(npc, "Well you can't work here unless you have an apron")
			npcsay(npc, "Health and safety regulations, you understand")
		end
	elseif resp == 2 then
		say(player, "Sorry I didn't realise")
	end
end


-- RSC 2001/3/f2p (not 100% complete)/dialogue- wydin- 100% pirates treasure- all- shop
local function wydin_hired(player, npc)
	npcsay(npc, "Is it nice and tidy round the back now")
	local resp = multi(player,
		"Yes, can I work out front now?",
		"Yes, are you going to pay me yet?",
		"No it's a complete mess",
		"Can I buy something please?")
	if resp == 1 then
		say(player, "Yes, can I work out front now?")
		npcsay(npc, "No I'm the one who works here")
	elseif resp == 2 then
		say(player, "Yes, are you going to pay me yet?")
		npcsay(npc, "Umm no not yet")
	elseif resp == 3 then
		say(player, "No it's a complete mess")
		npcsay(npc, "Ah well, It'll give you something to do won't it")
	elseif resp == 4 then
		say(player, "Can I buy something please")
		npcsay(npc, "Yes Ok")
		openshop(player, "foodshop")
	end
end

function talknpc_wydin(player, npc)
	if getvar(player, "wydin_hired") ~= 0 then
		wydin_hired(player, npc)
	else
		wydin_intro(player, npc)
	end
end

-- RSC 2001/7/dialogue- wydin-50% pirates treasure door- open- fail- get hired- success.pcap
function opbound1_wydindoor(player, x, y, dir)
	local npc = nearnpc(player, "wydin")
	if not npc then
		return
	end
	if getvar(player, "wydin_hired") == 0 then
		wydin_guard(player, npc)
		return
	end
	if not worn(player, "whiteapron") then
		npcsay(npc, "Can you put your apron on before going in there please")
		return
	end
	boundaryteleport(player, x, y, dir)
	mes(player, "@que@")
	changebound(x, y, dir, "openlockeddoor")
	pause(2, 2)
	changebound(x, y, dir, "wydindoor")
end

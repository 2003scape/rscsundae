local restore_locs = {}
local delete_locs = {}
local talknpc_scripts = {}
local killnpc_scripts = {}
local takeobj_scripts = {}
local wearobj_scripts = {}
local attacknpc_scripts = {}
local opinv_scripts = {}
local skillplayer_scripts = {}
local skillnpc_scripts = {}
local spellself_scripts = {}
local spellinv_scripts = {}
local opbound1_scripts = {}
local opbound2_scripts = {}
local oploc1_scripts = {}
local oploc2_scripts = {}
local useloc_scripts = {}
local usebound_scripts = {}
local usenpc_scripts = {}
local useobj_scripts = {}
local useinv_scripts = {}
local player_scripts = {}
local paused_scripts = {}
local active_script = nil

STAT_ATTACK		= 0
STAT_DEFENSE		= 1
STAT_STRENGTH		= 2
STAT_HITS		= 3
STAT_RANGED		= 4
STAT_PRAYER		= 5
STAT_MAGIC		= 6
STAT_COOKING		= 7
STAT_WOODCUTTING	= 8
STAT_FLETCHING		= 9
STAT_FISHING		= 10
STAT_FIREMAKING		= 11
STAT_CRAFTING		= 12
STAT_SMITHING		= 13
STAT_MINING		= 14
STAT_HERBLAW		= 15

function new_player_script(player)
	local ps = {}
	ps.player = player
	ps.delay = 0
	ps.option_count = 0
	ps.npc = nil
	return ps
end

function register_talknpc(name, callback)
	talknpc_scripts[name] = callback;
end

function register_opinv(name, callback)
	opinv_scripts[name] = callback;
end

function register_skillplayer(spell, callback)
	skillplayer_scripts[spell] = callback;
end

function register_opbound1(name, callback)
	opbound1_scripts[name] = callback;
end

function register_opbound2(name, callback)
	opbound2_scripts[name] = callback;
end

function register_oploc1(name, callback)
	oploc1_scripts[name] = callback;
end

function register_oploc2(name, callback)
	oploc2_scripts[name] = callback;
end

function register_attacknpc(name, callback)
	attacknpc_scripts[name] = callback;
end

function register_killnpc(name, callback)
	killnpc_scripts[name] = callback;
end

function register_takeobj(name, callback)
	takeobj_scripts[name] = callback;
end

function register_spellself(name, callback)
	spellself_scripts[name] = callback
end

function register_wearobj(name, callback)
	wearobj_scripts[name] = callback;
end

function register_skillnpc(name, spell, callback)
	if not skillnpc_scripts[spell] then
		skillnpc_scripts[spell] = {}
	end
	skillnpc_scripts[spell][name] = callback
end

function register_spellinv(name, spell, callback)
	if not spellinv_scripts[spell] then
		spellinv_scripts[spell] = {}
	end
	spellinv_scripts[spell][name] = callback
end

function register_usenpc(name, item, callback)
	if not usenpc_scripts[name] then
		usenpc_scripts[name] = {}
	end
	usenpc_scripts[name][item] = callback
end

function register_useloc(name, item, callback)
	if not useloc_scripts[name] then
		useloc_scripts[name] = {}
	end
	useloc_scripts[name][item] = callback
end

function register_usebound(name, item, callback)
	if not usebound_scripts[name] then
		usebound_scripts[name] = {}
	end
	usebound_scripts[name][item] = callback
end

function register_useobj(item, invitem, callback)
	if not useobj_scripts[item] then
		useobj_scripts[item] = {}
	end
	useobj_scripts[item][invitem] = callback
end

function register_useinv(item1, item2, callback)
	if not useinv_scripts[item1] then
		useinv_scripts[item1] = {}
	end
	useinv_scripts[item1][item2] = callback

	if not useinv_scripts[item2] then
		useinv_scripts[item2] = {}
	end
	useinv_scripts[item2][item1] = callback
end

function script_engine_tick()
	for i=#paused_scripts,1,-1 do
		local ps = paused_scripts[i]
		if ps.delay > 0 then
			ps.delay = ps.delay - 1;
		else
			local result, err = coroutine.resume(ps.co)
			if not result then
				print("Script error inside coroutine :" .. err)
			end
			table.remove(paused_scripts, i)
		end
	end
	-- regrow tree stumps, etc.
	for i=#restore_locs,1,-1 do
		local event = restore_locs[i]
		if event.timer > 0 then
			event.timer = event.timer - 1;
		else
			_restoreloc(event.x, event.y)
			table.remove(restore_locs, i)
		end
	end
	-- delete temporarily added locs
	for i=#delete_locs,1,-1 do
		local event = delete_locs[i]
		if event.timer > 0 then
			event.timer = event.timer - 1;
		else
			local result, err = coroutine.resume(event.co)
			if not result then
				print("Script error inside coroutine :" .. err)
			end
			_delloc(event.x, event.y)
			table.remove(delete_locs, i)
		end
	end
end

function script_engine_process(player)
	local ps = player_scripts[player]
	if not ps then
		return
	end
	active_script = ps
	if ps.delay > 0 then
		ps.delay = ps.delay - 1
	end
	if ps.option_count > 0 then
		if os.time() >= (ps.last_active + 10) then
			print("script cancelled due to inactivity")
			script_engine_cancel(player)
		end
	else
		if ps.delay == 0 then
			local result, err = coroutine.resume(ps.co)
			if not result then
				print("Script error inside coroutine: " .. err)
				script_engine_cancel(player)
			end
		end
	end
end

function script_engine_cancel(player)
	local ps = player_scripts[player]
	local co = ps.co
	if ps.npc ~= nil then
		npcunbusy(ps.npc)
	end
	playerunbusy(player)
	player_scripts[player] = nil
	coroutine.close(co)
end

function script_engine_answer(player, answer)
	-- got an answer on a multi dialog
	local ps = player_scripts[player]
	if not ps then
		return
	end
	option_count = ps.option_count
	if answer > option_count then
		print("script cancelled due to invalid multi answer")
		script_engine_cancel(player)
		return
	end
	active_script = ps
	ps.answer = answer
	local result, err = coroutine.resume(ps.co)
	if not result then
		print("Script error inside coroutine: " .. err)
		script_engine_cancel(player)
	end
end

function script_engine_attacknpc(player, npc, name, x, y)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	script = attacknpc_scripts[name]
	if script then
		ps = new_player_script(player)
		ps.co = coroutine.create(function()
			script(player, npc, x, y)
			player_scripts[player] = nil
			playerunbusy(player)
		end)
		player_scripts[player] = ps
		playerbusy(player)
		return true
	end
	return false
end

function script_engine_takeobj(player, name, x, y)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	script = takeobj_scripts[name]
	if script then
		ps = new_player_script(player)
		ps.co = coroutine.create(function()
			script(player, x, y)
			player_scripts[player] = nil
			playerunbusy(player)
		end)
		player_scripts[player] = ps
		playerbusy(player)
		return true
	end
	return false
end

function script_engine_killnpc(player, npc, name, x, y)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	script = killnpc_scripts[name]
	if script then
		ps = new_player_script(player)
		ps.co = coroutine.create(function()
			script(player, npc, x, y)
			player_scripts[player] = nil
			playerunbusy(player)
		end)
		player_scripts[player] = ps
		playerbusy(player)
		return true
	end
	return false
end

function script_engine_talknpc(player, name, npc)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	script = talknpc_scripts[name]
	if script then
		ps = new_player_script(player)
		ps.npc = npc
		ps.co = coroutine.create(function()
			script(player, npc)
			player_scripts[player] = nil
			npcunbusy(npc)
			playerunbusy(player)
		end)
		player_scripts[player] = ps
		npcbusy(npc)
		playerbusy(player)
		return true
	end
	return false
end

function script_engine_skillnpc(player, name, npc, spell)
	local script = player_scripts[player]
	if script then
		return true
	end
	spell = string.lower(spell)
	name = string.lower(name)
	if not skillnpc_scripts[spell] then
		return false
	end
	script = skillnpc_scripts[spell][name]
	if not script then
		script = skillnpc_scripts[spell]["_"]
	end
	if script then
		ps = new_player_script(player)
		ps.co = coroutine.create(function()
			script(player, npc)
			player_scripts[player] = nil
			playerunbusy(player)
		end)
		player_scripts[player] = ps
		playerbusy(player)
		return true
	end
	return false
end

function script_engine_spellself(player, spell)
	local script = player_scripts[player]
	if script then
		return
	end
	spell = string.lower(spell)
	script = spellself_scripts[spell]
	if script then
		ps = new_player_script(player)
		ps.co = coroutine.create(function()
			script(player)
			player_scripts[player] = nil
			playerunbusy(player)
		end)
		player_scripts[player] = ps
		playerbusy(player)
		return
	end
end

function script_engine_spellinv(player, name, spell)
	local script = player_scripts[player]
	if script then
		return true
	end
	spell = string.lower(spell)
	name = string.lower(name)
	if not spellinv_scripts[spell] then
		return false
	end
	script = spellinv_scripts[spell][name]
	if not script then
		script = spellinv_scripts[spell]["_"]
	end
	if script then
		ps = new_player_script(player)
		ps.co = coroutine.create(function()
			script(player, name)
			player_scripts[player] = nil
			playerunbusy(player)
		end)
		player_scripts[player] = ps
		playerbusy(player)
		return true
	end
	return false
end

function script_engine_wearobj(player, name)
	name = string.lower(name)
	script = wearobj_scripts[name]
	if script then
		if not script(player, name) then
			return true
		end
	end
	return false
end

function script_engine_opinv(player, name)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	script = opinv_scripts[name]
	if script then
		ps = new_player_script(player)
		ps.co = coroutine.create(function()
			script(player)
			player_scripts[player] = nil
			playerunbusy(player)
		end)
		player_scripts[player] = ps
		playerbusy(player)
		return true
	end
	return false
end

function script_engine_skillplayer(player, target, name)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	script = skillplayer_scripts[name]
	if script then
		ps = new_player_script(player)
		ps.co = coroutine.create(function()
			script(player, target)
			player_scripts[player] = nil
			playerunbusy(player)
		end)
		player_scripts[player] = ps
		playerbusy(player)
		return true
	end
	return false
end

function script_engine_opbound1(player, name, x, y, dir)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	script = opbound1_scripts[name]
	if script then
		ps = new_player_script(player)
		ps.co = coroutine.create(function()
			script(player, x, y, dir)
			player_scripts[player] = nil
			playerunbusy(player)
		end)
		player_scripts[player] = ps
		playerbusy(player)
		return true
	end
	return false
end

function script_engine_opbound2(player, name, x, y, dir)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	script = opbound2_scripts[name]
	if script then
		ps = new_player_script(player)
		ps.co = coroutine.create(function()
			script(player, x, y, dir)
			player_scripts[player] = nil
			playerunbusy(player)
		end)
		player_scripts[player] = ps
		playerbusy(player)
		return true
	end
	return false
end

function script_engine_oploc1(player, name, x, y)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	script = oploc1_scripts[name]
	if script then
		ps = new_player_script(player)
		ps.co = coroutine.create(function()
			script(player, x, y)
			player_scripts[player] = nil
			playerunbusy(player)
		end)
		player_scripts[player] = ps
		playerbusy(player)
		return true
	end
	return false
end

function script_engine_oploc2(player, name, x, y)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	script = oploc2_scripts[name]
	if script then
		ps = new_player_script(player)
		ps.co = coroutine.create(function()
			script(player, x, y)
			player_scripts[player] = nil
			playerunbusy(player)
		end)
		player_scripts[player] = ps
		playerbusy(player)
		return true
	end
	return false
end

function script_engine_useloc(player, name, x, y, item)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	item = string.lower(item)
	script = useloc_scripts[name]
	if not script then
		return false
	end
	script = useloc_scripts[name][item]
	if script then
		ps = new_player_script(player)
		ps.co = coroutine.create(function()
			script(player, x, y)
			player_scripts[player] = nil
			playerunbusy(player)
		end)
		player_scripts[player] = ps
		playerbusy(player)
		return true
	end
	return false
end

function script_engine_usebound(player, name, x, y, dir, item)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	item = string.lower(item)
	script = usebound_scripts[name]
	if not script then
		return false
	end
	script = usebound_scripts[name][item]
	if script then
		ps = new_player_script(player)
		ps.co = coroutine.create(function()
			script(player, x, y, dir)
			player_scripts[player] = nil
			playerunbusy(player)
		end)
		player_scripts[player] = ps
		playerbusy(player)
		return true
	end
	return false
end

function script_engine_usenpc(player, npc, name, item)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	item = string.lower(item)
	script = usenpc_scripts[name]
	if not script then
		return false
	end
	script = usenpc_scripts[name][item]
	if script then
		ps = new_player_script(player)
		ps.npc = npc
		ps.co = coroutine.create(function()
			script(player, npc)
			player_scripts[player] = nil
			npcunbusy(npc)
			playerunbusy(player)
		end)
		player_scripts[player] = ps
		npcbusy(npc)
		playerbusy(player)
		return true
	end
	return false
end

function script_engine_useobj(player, name, x, y, item)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	item = string.lower(item)
	script = useobj_scripts[name]
	if not script then
		return false
	end
	script = useobj_scripts[name][item]
	if script then
		ps = new_player_script(player)
		ps.co = coroutine.create(function()
			script(player, x, y)
			player_scripts[player] = nil
			playerunbusy(player)
		end)
		player_scripts[player] = ps
		playerbusy(player)
		return true
	end
	return false
end

function script_engine_useinv(player, name, item)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	item = string.lower(item)
	script = useinv_scripts[name]
	if script then
		script = useinv_scripts[name][item]
	else
		script = useinv_scripts[item]
		if not script then
			return false
		end
		script = useinv_scripts[item][name]
	end
	if script then
		ps = new_player_script(player)
		ps.co = coroutine.create(function()
			script(player)
			player_scripts[player] = nil
			playerunbusy(player)
		end)
		player_scripts[player] = ps
		playerbusy(player)
		return true
	end
	return false
end

function pause(min, max)
	local amount = min + (math.random() * max)

	table.insert(paused_scripts, active_script)
	player_scripts[active_script.player] = nil
	playerunbusy(active_script.player)
	delay(amount)
end

function delay(length)
	active_script.delay = length
	coroutine.yield(active_script.co)
end

function addloc(name, x, y, timer)
	_addloc(name, x, y)

	local target = {}
	target.x = x
	target.y = y
	target.timer = timer
	target.co = active_script.co

	table.insert(delete_locs, target)
	player_scripts[active_script.player] = nil
	playerunbusy(active_script.player)
	coroutine.yield(active_script.co)
end

function restoreloc(x, y, timer)
	local target = {}
	target.x = x
	target.y = y
	target.timer = timer

	table.insert(restore_locs, target)
end

function say(player, mes)
	_say(player, mes)
	delay(3)
end

function npcsay(npc, mes)
	_npcsay(npc, mes)
	delay(3)
end

function multi(player, ...)
	local arg = { ... }
	active_script.option_count = #arg
	active_script.last_active = os.time()
	playerunbusy(player)
	_multi(player, arg)
	coroutine.yield(active_script.co)
	active_script.option_count = 0
	playerbusy(player)
	return active_script.answer
end

-- https://github.com/2004Scape/Server/blob/main/data/src/scripts/drop%20tables/scripts/shared_droptables.rs2
function randomherb(player, x, y)
	addobject(player, "coins", 10, x, y)
end

-- https://github.com/2004Scape/Server/blob/main/data/src/scripts/drop%20tables/scripts/shared_droptables.rs2
function randomjewel(player, x, y)
	local ran = randomvar(128)
	if ran < 32 then
		addobject(player, "uncut sapphire", 1, x, y)
	elseif ran < 48 then
		addobject(player, "uncut emerald", 1, x, y)
	elseif ran < 56 then
		addobject(player, "uncut ruby", 1, x, y)
	elseif ran < 58 then
		addobject(player, "uncut diamond", 1, x, y)
	end
end

function check_cookrange(player)
	if getvar(player, "cook_stage") >= 2 then
		return true
	end

	-- https://classic.runescape.wiki/w/Transcript:Cook

	local npc = nearnpc(player, "cook")

	if npc then
		npcsay(npc, "Hey! Who said you could use that?")
	end

	return false
end

dofile("./lua/content.lua")

--
-- automatically register triggers
-- note that this does not work for entities that contain special
-- characters, like "wizard's mind bomb"
--
-- it will also not work for operations that involve two entities as the key
--
for k, v in pairs(_G) do
	if type(v) == "function" then
		if string.match(k, "^opinv_.*") then
			target = string.gsub(string.sub(k, 7), "_", " ")
			register_opinv(target, v)
		elseif string.match(k, "^talknpc_.*") then
			target = string.gsub(string.sub(k, 9), "_", " ")
			register_talknpc(target, v)
		elseif string.match(k, "^skillplayer_.*") then
			target = string.gsub(string.sub(k, 13), "_", " ")
			register_skillplayer(target, v)
		elseif string.match(k, "^opbound1_.*") then
			target = string.gsub(string.sub(k, 10), "_", " ")
			register_opbound1(target, v)
		elseif string.match(k, "^opbound2_.*") then
			target = string.gsub(string.sub(k, 10), "_", " ")
			register_opbound2(target, v)
		elseif string.match(k, "^oploc1_.*") then
			target = string.gsub(string.sub(k, 8), "_", " ")
			register_oploc1(target, v)
		elseif string.match(k, "^oploc2_.*") then
			target = string.gsub(string.sub(k, 8), "_", " ")
			register_oploc2(target, v)
		elseif string.match(k, "^killnpc_.*") then
			target = string.gsub(string.sub(k, 9), "_", " ")
			register_killnpc(target, v)
		elseif string.match(k, "^attacknpc_.*") then
			target = string.gsub(string.sub(k, 11), "_", " ")
			register_attacknpc(target, v)
		elseif string.match(k, "^takeobj_.*") then
			target = string.gsub(string.sub(k, 9), "_", " ")
			register_takeobj(target, v)
		elseif string.match(k, "^spellself_.*") then
			target = string.gsub(string.sub(k, 11), "_", " ")
			register_spellself(target, v)
		end
	end
end

local restore_locs = {}
local talknpc_scripts = {}
local opinv_scripts = {}
local skillplayer_scripts = {}
local skillnpc_scripts = {}
local opbound1_scripts = {}
local opbound2_scripts = {}
local oploc1_scripts = {}
local oploc2_scripts = {}
local useloc_scripts = {}
local player_scripts = {}
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

function new_player_script()
	local ps = {}
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

function register_skillnpc(name, spell, callback)
	if not skillnpc_scripts[spell] then
		skillnpc_scripts[spell] = {}
	end
	skillnpc_scripts[spell][name] = callback
end

function register_useloc(name, item, callback)
	if not useloc_scripts[name] then
		useloc_scripts[name] = {}
	end
	useloc_scripts[name][item] = callback
end

function script_engine_tick()
	for i=1,#restore_locs do
		local event = restore_locs[i]
		if event.timer > 0 then
			event.timer = event.timer - 1;
		else
			_restoreloc(event.x, event.y)
			table.remove(restore_locs, i)
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
			coroutine.resume(ps.co)
		end
	end
end

function script_engine_cancel(player)
	local ps = player_scripts[player]
	local co = ps.co
	if ps.npc ~= nil then
		npcunbusy(ps.npc)
	end
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
	ps.answer = answer
	coroutine.resume(ps.co)
end

function script_engine_talknpc(player, name, npc)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	script = talknpc_scripts[name]
	if script then
		ps = new_player_script()
		ps.npc = npc
		ps.co = coroutine.create(function()
			script(player, npc)
			player_scripts[player] = nil
			npcunbusy(npc)
		end)
		player_scripts[player] = ps
		npcbusy(npc)
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
		ps = new_player_script()
		ps.co = coroutine.create(function()
			script(player, npc)
			player_scripts[player] = nil
		end)
		player_scripts[player] = ps
		return true
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
		ps = new_player_script()
		ps.co = coroutine.create(function()
			script(player)
			player_scripts[player] = nil
		end)
		player_scripts[player] = ps
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
		ps = new_player_script()
		ps.co = coroutine.create(function()
			script(player, target)
			player_scripts[player] = nil
		end)
		player_scripts[player] = ps
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
		ps = new_player_script()
		ps.co = coroutine.create(function()
			script(player, x, y, dir)
			player_scripts[player] = nil
		end)
		player_scripts[player] = ps
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
		ps = new_player_script()
		ps.co = coroutine.create(function()
			script(player, x, y, dir)
			player_scripts[player] = nil
		end)
		player_scripts[player] = ps
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
		ps = new_player_script()
		ps.co = coroutine.create(function()
			script(player, x, y)
			player_scripts[player] = nil
		end)
		player_scripts[player] = ps
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
		ps = new_player_script()
		ps.co = coroutine.create(function()
			script(player, x, y)
			player_scripts[player] = nil
		end)
		player_scripts[player] = ps
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
		ps = new_player_script()
		ps.co = coroutine.create(function()
			script(player, x, y)
			player_scripts[player] = nil
		end)
		player_scripts[player] = ps
		return true
	end
	return false
end

function delay(length)
	active_script.delay = length
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
	_multi(player, arg)
	coroutine.yield(active_script.co)
	active_script.option_count = 0
	return active_script.answer
end

dofile("./data/lua/rs1/npc/asgarnia/barmaid.lua")
dofile("./data/lua/rs1/npc/asgarnia/betty.lua")
dofile("./data/lua/rs1/npc/asgarnia/brian.lua")
dofile("./data/lua/rs1/npc/asgarnia/cassie.lua")
dofile("./data/lua/rs1/npc/asgarnia/drogo.lua")
dofile("./data/lua/rs1/npc/asgarnia/flynn.lua")
dofile("./data/lua/rs1/npc/asgarnia/gerrant.lua")
dofile("./data/lua/rs1/npc/asgarnia/grum.lua")
dofile("./data/lua/rs1/npc/asgarnia/herquin.lua")
dofile("./data/lua/rs1/npc/asgarnia/wayne.lua")
dofile("./data/lua/rs1/npc/karamja/zambo.lua")
dofile("./data/lua/rs1/npc/aubury.lua")
dofile("./data/lua/rs1/npc/craftingshop.lua")
dofile("./data/lua/rs1/npc/hans.lua")
dofile("./data/lua/rs1/npc/horvik.lua")
dofile("./data/lua/rs1/npc/louielegs.lua")
dofile("./data/lua/rs1/npc/lowe.lua")
dofile("./data/lua/rs1/npc/peksa.lua")
dofile("./data/lua/rs1/npc/ranael.lua")
dofile("./data/lua/rs1/npc/scavvo.lua")
dofile("./data/lua/rs1/npc/swordshop.lua")
dofile("./data/lua/rs1/npc/thessalia.lua")
dofile("./data/lua/rs1/npc/valaine.lua")
dofile("./data/lua/rs1/npc/zaff.lua")
dofile("./data/lua/rs1/npc/zeke.lua")
dofile("./data/lua/rs1/npc/barbarian.lua")
dofile("./data/lua/rs1/npc/bartender.lua")
dofile("./data/lua/rs1/npc/banker.lua")
dofile("./data/lua/rs1/npc/genstore.lua")
dofile("./data/lua/rs1/npc/bob.lua")
dofile("./data/lua/rs1/npc/man.lua")
dofile("./data/lua/rs1/npc/kebabseller.lua")
dofile("./data/lua/rs1/npc/silktrader.lua")
dofile("./data/lua/rs1/skill_magic/curses.lua")
dofile("./data/lua/rs1/skill_magic/missiles.lua")
dofile("./data/lua/rs1/skill_prayer/altar.lua")
dofile("./data/lua/rs1/skill_prayer/bones.lua")
dofile("./data/lua/rs1/skill_woodcutting/tree.lua")
dofile("./data/lua/rs1/items/asgarnia/alcohol.lua")
dofile("./data/lua/rs1/items/food.lua")
dofile("./data/lua/rs1/items/cabbage.lua")
dofile("./data/lua/rs1/items/kebab.lua")
dofile("./data/lua/rs1/items/spinachroll.lua")
dofile("./data/lua/rs1/items/meat.lua")
dofile("./data/lua/rs1/items/wine.lua")
dofile("./data/lua/rs1/items/beer.lua")
dofile("./data/lua/rs1/items/pies.lua")
dofile("./data/lua/rs1/items/strengthpotion.lua")
dofile("./data/lua/rs1/items/stew.lua")
dofile("./data/lua/rs1/items/cake.lua")
dofile("./data/lua/rs1/items/pizza.lua")
dofile("./data/lua/rs1/items/pumpkin.lua")
dofile("./data/lua/rs1/misc/clutter.lua")
dofile("./data/lua/rs1/misc/doors.lua")
dofile("./data/lua/rs1/misc/ladders.lua")
dofile("./data/lua/rs1/misc/plants.lua")
dofile("./data/lua/rs1/misc/water.lua")

--
-- automatically register triggers
-- note that this does not work for entities that contain special
-- characters, like "wizard's mind bomb"
--
-- it will also not work for operations that involve two entities as the key
--
for k in pairs(_G) do
	v = _G[k]
	if type(v) == "function" then
		if string.match(k, "opinv_.*") then
			target = string.gsub(string.sub(k, 7), "_", " ")
			register_opinv(target, v)
		elseif string.match(k, "talknpc_.*") then
			target = string.gsub(string.sub(k, 9), "_", " ")
			register_talknpc(target, v)
		elseif string.match(k, "skillplayer_.*") then
			target = string.gsub(string.sub(k, 13), "_", " ")
			register_skillplayer(target, v)
		elseif string.match(k, "opbound1_.*") then
			target = string.gsub(string.sub(k, 10), "_", " ")
			register_opbound1(target, v)
		elseif string.match(k, "opbound2_.*") then
			target = string.gsub(string.sub(k, 10), "_", " ")
			register_opbound2(target, v)
		elseif string.match(k, "oploc1_.*") then
			target = string.gsub(string.sub(k, 8), "_", " ")
			register_oploc1(target, v)
		elseif string.match(k, "oploc2_.*") then
			target = string.gsub(string.sub(k, 8), "_", " ")
			register_oploc2(target, v)
		end
	end
end

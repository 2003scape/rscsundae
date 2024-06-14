ontalknpc_scripts = {}
onuseobj_scripts = {}
onskillplayer_scripts = {}
onskillnpc_scripts = {}
onopbound1_scripts = {}
onopbound2_scripts = {}
player_scripts = {}
active_script = nil

STAT_ATTACK		= 0
STAT_DEFENSE		= 1
STAT_STRENGTH		= 2
STAT_HITS		= 3
STAT_RANGED		= 4
STAT_PRAYER		= 5
STAT_MAGIC		= 6

function new_player_script()
	local ps = {}
	ps.delay = 0
	ps.option_count = 0
	ps.npc = nil
	return ps
end

function register_talknpc(name, callback)
	ontalknpc_scripts[name] = callback;
end

function register_useobj(name, callback)
	onuseobj_scripts[name] = callback;
end

function register_skillplayer(spell, callback)
	onskillplayer_scripts[spell] = callback;
end

function register_opbound1(name, callback)
	onopbound1_scripts[name] = callback;
end

function register_opbound2(name, callback)
	onopbound2_scripts[name] = callback;
end

function register_skillnpc(name, spell, callback)
	if not onskillnpc_scripts[spell] then
		onskillnpc_scripts[spell] = {}
	end
	onskillnpc_scripts[spell][name] = callback
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

function script_engine_ontalknpc(player, name, npc)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	script = ontalknpc_scripts[name]
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

function script_engine_onskillnpc(player, name, npc, spell)
	local script = player_scripts[player]
	if script then
		return true
	end
	spell = string.lower(spell)
	name = string.lower(name)
	if not onskillnpc_scripts[spell] then
		return false
	end
	script = onskillnpc_scripts[spell][name]
	if not script then
		script = onskillnpc_scripts[spell]["_"]
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

function script_engine_onuseobj(player, name)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	script = onuseobj_scripts[name]
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

function script_engine_onskillplayer(player, target, name)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	script = onskillplayer_scripts[name]
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

function script_engine_onopbound1(player, name, x, y, dir)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	script = onopbound1_scripts[name]
	if script then
		ps = new_player_script()
		ps.co = coroutine.create(function()
			script(player, name, x, y, dir)
			player_scripts[player] = nil
		end)
		player_scripts[player] = ps
		return true
	end
	return false
end

function script_engine_onopbound2(player, name, x, y, dir)
	local script = player_scripts[player]
	if script then
		return true
	end
	name = string.lower(name)
	script = onopbound2_scripts[name]
	if script then
		ps = new_player_script()
		ps.co = coroutine.create(function()
			script(player, name, x, y, dir)
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

dofile("./data/lua/rs1/npc/bartender.lua")
dofile("./data/lua/rs1/npc/man.lua")
dofile("./data/lua/rs1/npc/kebabseller.lua")
dofile("./data/lua/rs1/npc/silktrader.lua")
dofile("./data/lua/rs1/skill_magic/curses.lua")
dofile("./data/lua/rs1/skill_magic/missiles.lua")
dofile("./data/lua/rs1/skill_prayer/bones.lua")
dofile("./data/lua/rs1/items/food.lua")
dofile("./data/lua/rs1/items/cabbage.lua")
dofile("./data/lua/rs1/items/kebab.lua")
dofile("./data/lua/rs1/items/spinachroll.lua")
dofile("./data/lua/rs1/items/meat.lua")
dofile("./data/lua/rs1/items/wine.lua")
dofile("./data/lua/rs1/items/beer.lua")
dofile("./data/lua/rs1/items/pies.lua")
dofile("./data/lua/rs1/items/strengthpotion.lua")
dofile("./data/lua/rs1/items/asgarniandrinks.lua")
dofile("./data/lua/rs1/items/stew.lua")
dofile("./data/lua/rs1/items/cake.lua")
dofile("./data/lua/rs1/items/pizza.lua")
dofile("./data/lua/rs1/items/pumpkin.lua")
dofile("./data/lua/rs1/misc/doors.lua")

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
		if string.match(k, "useobj_.*") then
			target = string.gsub(string.sub(k, 8), "_", " ")
			register_useobj(target, v)
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
		end
	end
end

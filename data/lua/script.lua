npc_scripts = {}
useobj_scripts = {}
player_scripts = {}
active_script = nil

STAT_ATTACK		= 0
STAT_DEFENSE		= 1
STAT_STRENGTH		= 2
STAT_HITS		= 3
STAT_RANGED		= 4
STAT_PRAYER		= 5
STAT_MAGIC		= 6

function register_talknpc(name, callback)
	npc_scripts[name] = callback;
end

function register_useobj(name, callback)
	useobj_scripts[name] = callback;
end

function script_engine_process(player)
	local ps = player_scripts[player]
	if not ps then
		print("player has no script")
		return
	end
	active_script = ps 
	if ps.delay > 0 then
		ps.delay = ps.delay - 1
	end
	if ps.delay == 0 then
		print("resume coroutine")
		coroutine.resume(ps.co)
	else
		print("not ready yet")
	end
end

function script_engine_cancel(player)
	couroutine.close(player_scripts[player].co)
	player_scripts[player] = nil
end

function script_engine_ontalknpc(player, name, npc)
	local script = player_scripts[player]
	if script then
		print("already existing script")
		return
	end
	name = string.lower(name)
	print(name)
	script = npc_scripts[name]
	if script then
		ps = {}
		ps.delay = 0
		ps.co = coroutine.create(function()
			script(player, npc)
			player_scripts[player] = nil
		end)
		player_scripts[player] = ps
	else
		print("no such script")
		_default_talk(player, npc)
	end
end

function script_engine_onuseobj(player, name)
	local script = player_scripts[player]
	if script then
		print("already existing script")
		return
	end
	name = string.lower(name)
	print(name)
	script = useobj_scripts[name]
	if script then
		ps = {}
		ps.delay = 0
		ps.co = coroutine.create(function()
			script(player)
			player_scripts[player] = nil
		end)
		player_scripts[player] = ps
	else
		print("no such script")
		_default_action(player, npc)
	end
end

function delay(length)
	active_script.delay = length
	coroutine.yield(active_script.co)
end

function say(player, mes)
	_say(player, mes)
	delay(2)
end

function npcsay(npc, mes)
	_npcsay(npc, mes)
	delay(2)
end

dofile("./src/lua/rs1/npc/man.lua")
dofile("./src/lua/rs1/skill_prayer/bones.lua")

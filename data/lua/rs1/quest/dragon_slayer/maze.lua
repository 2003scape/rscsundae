-- https://classic.runescape.wiki/w/Melzar%27s_Maze

-- melzardoor has no opbound handler
register_usebound("melzardoor", "melzarkey", function (player, x, y, dir)
    mes(player, "@que@you unlock the door")
    mes(player, "@que@You go through the door")
	boundaryteleport(player, x, y, dir)
    changebound(x, y, dir, "openlockeddoor")
    pause(3, 3)
    changebound(x, y, dir, "melzardoor")
end)

-- accurate capitalization
local keys = {
    "red", "Orange", "Yellow", "blue", "magenta", "Black"
}

for _, key in ipairs(keys) do
    local door = string.lower(key) .. "door"
    local item = string.lower(key) .. " key"

    register_usebound(door, item, function (player, x, y, dir)
        if door == "blackdoor" then
            setvar(player, "dragon_funchest", 1)
        end

        mes(player, "@que@you unlock the door")
        remove(player, item, 1)
        mes(player, "@que@You go through the door")
        mes(player, "Your " .. key .. " key has gone!")
        boundaryteleport(player, x, y, dir)
        changebound(x, y, dir, "openlockeddoor")
        pause(3, 3)
        changebound(x, y, dir, door)
    end)

    register_opbound1(door, function (player, x, y, dir)
        mes(player, "@que@The door is locked")
    end)
end

function oploc1_funladder(player, x, y)
	mes(player, "@que@You climb down the ladder")
	changeleveldown(player)
end

function opbound1_funexit(player, x, y, dir)
    local player_x = playercoord(player)

    if player_x >= x then
        mes(player, "@que@this door is locked")
    else
        mes(player, "The door swings open")
        mes(player, "@que@You go through the door")
        boundaryteleport(player, x, y, dir)
        changebound(x, y, dir, "openlockeddoor")
        pause(3, 3)
        changebound(x, y, dir, "funexit")
    end
end

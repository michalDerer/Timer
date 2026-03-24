s = math.sin(23.7)
print("print from lua: " .. s)
a = 7 + 11 + s
a = a +100

print("print from lua: " .. a)

--PlayerTitle = "Squire"
--PlayerName = "Pepa"
--PlayerFamily = "Cigan"
--PlayerLevel = 20

--player = {}
--player["PlayerTitle"]
--player["PlayerName"]
--player["PlayerFamily"]
--player["PlayerLevel"]

player = {PlayerTitle = "Squire", PlayerName = "Pepa", PlayerFamily= "Cigan", PlayerLevel = "20"}

function AddStuff(a,b)
	print("[LUA] AddStuff("..a..", "..b..") called \n")
	return a + b
end
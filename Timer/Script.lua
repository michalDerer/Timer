a = 7  + 11 + math.sin(23.7)
a = a +100

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
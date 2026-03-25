
print("Script.lua start")

--PlayerTitle = "Squire"
--PlayerName = "Pepa"
--PlayerFamily = "Cigan"
--PlayerLevel = 20

--player = {}
--player["PlayerTitle"]
--player["PlayerName"]
--player["PlayerFamily"]
--player["PlayerLevel"]

--player = {PlayerTitle = "Squire", PlayerName = "Pepa", PlayerFamily= "Cigan", PlayerLevel = "20"}

--function AddStuff(a,b)
--	print("[LUA] AddStuff("..a..", "..b..") called \n")
--	return a + b
--end


parent = LuaRectTransform_createInstance()
child = LuaRectTransform_createInstance()
LuaRectTransform_set_parent(parent, child)
set_rootRectTransform(parent)

child = nil
collectgarbage()

print("Script.lua start")

pikachu = LuaTexture.new("pika.png")
parent = LuaRectTransform_new()
child = LuaRectTransform.new()

child:set_parent(parent)

values = {
    anchorMinX = 0.1,
    anchorMinY = 0.1,
    anchorMaxX = 0.9,
    anchorMaxY = 0.9,
    left = 0,
    right = -1,
    top = 0,
    bottom = -1,
}
parent:set_values(values)

--values["anchorMinX"] = 0.5
values.anchorMinX = 0.5
values.anchorMinY = 0.5
values.anchorMaxX = 0.7
values.anchorMaxY = 0.7
child:set_values(values)

pikachu = nil
--child = nil
collectgarbage()
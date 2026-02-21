#file(GLOB LUA_SOURCES *.c)
set(LUA_SOURCES
    lapi.c 
    lauxlib.c 
    lbaselib.c
    lcode.c 
    lcorolib.c 
    lctype.c 
    ldblib.c 
    ldebug.c 
    ldo.c 
    ldump.c 
    lfunc.c 
    lgc.c 
    linit.c
    liolib.c 
    llex.c 
    lmathlib.c
    lmem.c 
    loadlib.c 
    lobject.c 
    lopcodes.c
    loslib.c 
    lparser.c 
    lstate.c 
    lstring.c 
    lstrlib.c 
    ltable.c 
    ltablib.c 
    ltests.c
    ltm.c
    lundump.c 
    lutf8lib.c 
    lvm.c 
    lzio.c)

#prida static library subprojekt do TimerSolution
add_library(lua STATIC ${LUA_SOURCES}) 


#lua je c kod treba nejakym sposobom zaistit, moznost
#set_target_properties(lua PROPERTIES INKER_LANGUAGE C)


#headre su v rovnakej zlozke ako sources, samotny projekt preto nepotrebuje include pridavat, 
#ale ostatne likovane projekty na tento budu mat tymto prikazom k dispozicii headre.
target_include_directories(lua PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) 
#target_include_directories(lua PUBLIC)


# Optional: Lua interpreter
#add_executable(luai lua.c)
#target_link_libraries(luai lua m)

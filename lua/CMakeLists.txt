#file(GLOB LUA_SOURCES *.c)
set(LUA_SOURCES
    lapi.c
    lapi.h 
    lauxlib.c
    lauxlib.h
    lbaselib.c
    lcode.c
    lcode.h 
    lcorolib.c 
    lctype.c 
    lctype.h
    ldblib.c 
    ldebug.c 
    ldebug.h
    ldo.c 
    ldo.h
    ldump.c 
    lfunc.c 
    lfunc.h
    lgc.c 
    lgc.h
    linit.c
    liolib.c 
    ljumptab.h
    llex.c 
    llex.h
    llimits.h
    lmathlib.c
    lmem.c 
    lmem.h
    loadlib.c 
    lobject.c 
    lobject.h
    lopcodes.c
    lopcodes.h
    lopnames.h
    loslib.c 
    lparser.c 
    lparser.h
    lprefix.h
    lstate.c 
    lstate.h
    lstring.c 
    lstring.h
    lstrlib.c 
    ltable.c 
    ltable.h
    ltablib.c 
    #ltests.c   #debagovanie
    #ltests.h
    ltm.c
    ltm.h
    #lua.c      #interpreter
    lua.h
    lua.hpp
    luaconf.h
    lualib.h
    lundump.c 
    lundump.h
    lutf8lib.c 
    lvm.c 
    lvm.h
    lzio.c
    lzio.h
    #onelua.c   #all in one
)

#prida static library subprojekt do TimerSolution
add_library(lua STATIC ${LUA_SOURCES}) 


#lua je c kod treba nejakym sposobom zaistit, moznost
#set_target_properties(lua PROPERTIES INKER_LANGUAGE C)

#preprocesor define MAKE_LIB ignoruje sposobi ze v onelua.c sa neincluduje lua.c (interpreter) a luac.c (compiler)
#target_compile_definitions(lua PRIVATE MAKE_LIB)

#headre su v rovnakej zlozke ako sources, samotny projekt preto nepotrebuje include pridavat, 
#ale ostatne likovane projekty na tento budu mat tymto prikazom k dispozicii headre.
target_include_directories(lua PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) 
#target_include_directories(lua PUBLIC)


# Optional: Lua interpreter
#add_executable(luai lua.c)
#target_link_libraries(luai lua m)

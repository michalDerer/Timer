cmake_minimum_required(VERSION 3.7)

# Lua core library sources
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
    lzio.c
)

add_library(lua_s_lib STATIC ${LUA_SOURCES}) #prida static library subprojekt do TimerSolution

target_include_directories(lua_s_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) #headre su v rovnakej zlozke ako sources, samotny projekt preto nepotrebuje include pridavat, ale ostatne likovane projekty na tento budu mat tymto prikazom k dispozicii headre.
#target_include_directories(lua_s_lib PUBLIC)


# Optional: Lua interpreter
#add_executable(lua lua.c)
#target_link_libraries(lua lua_static m)

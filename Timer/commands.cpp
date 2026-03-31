//#include <algorithm>
//#include <stdexcept>
#include <string>

#include <SDL3_image/SDL_image.h>

//#include "core.hpp"
#include "commands.hpp"
#include "context.hpp"



int LuaTexture_new(lua_State* L)
{
    if (lua_gettop(L) != 1)
    {
        return luaL_error(L, "LuaTexture_new: nespravny pocet argumentov");
    }

    // Converts the Lua value at the given index to a C string.
    // If the value is a number, then lua_tolstring also changes the actual value in the stack to a string.
    // Returns a pointer to a string inside the Lua state.This string always has a zero('\0') after its last character
    // Because Lua has garbage collection, there is no guarantee that the pointer returned by lua_tolstring will be valid after the corresponding Lua value is removed from the stack.
    // Ak sa hodnota neda skonvertovat vyhodi error
    const char* relativePath = luaL_checkstring(L, 1);
    


    if (Context::pathExe == NULL)
    {
        return luaL_error(L, "LuaTexture_new: Context::pathExe nemoze byt null");
    }

    // netestujem spravnost formatu cesty, anim ci ukazuje na existujuci asset
    std::string fullPath = std::string{ Context::pathExe } + relativePath;
    printf("LuaTexture_new: fullPath: %s\n", fullPath.c_str());

    SDL_Surface* surface = IMG_Load(fullPath.c_str());

    if (!surface)
    {
        printf("LuaTexture_new: IMG_Load failed: %s", SDL_GetError());
        return luaL_error(L, "LuaTexture_new: IMG_Load failed");
    }

    if (Context::renderer == NULL)
    {
        return luaL_error(L, "LuaTexture_new: Context::renderer nemoze byt null");
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Context::renderer, surface);
    SDL_DestroySurface(surface);
    surface = NULL;

    if (!texture)
    {
        printf("LuaTexture_new: SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
        return luaL_error(L, "LuaTexture_new: SDL_CreateTextureFromSurface failed");
    }



    void* mem = lua_newuserdata(L, sizeof(LuaTexture));
    LuaTexture* lTexture = new (mem) LuaTexture();
    lTexture->texture = std::shared_ptr<SDL_Texture>(texture, SDL_DestroyTexture);
    
    luaL_getmetatable(L, "LuaTexture");     // pusne do staku na koniec metatable LuaTexture
    lua_setmetatable(L, -2);                // setne metatable, popne 

    return 1;
}

int LuaTexture_gc(lua_State* L)
{
    LuaTexture* lTexture = (LuaTexture*)luaL_checkudata(L, 1, "LuaTexture");

    lTexture->~LuaTexture();

    return 0;
}

void register_LuaTexture(lua_State* L)
{
    luaL_newmetatable(L, "LuaTexture");

    lua_pushcfunction(L, LuaTexture_gc);
    lua_setfield(L, -2, "__gc");

    //lua_pushvalue(L, -1);
    //lua_setfield(L, -2, "__index");

    //luaL_Reg methods[] =
    //{
    //    //{"set_values",  LuaRectTransform_set_values},
    //    { NULL, NULL }
    //};
    //luaL_setfuncs(L, methods, 0);

    lua_newtable(L);
    lua_pushcfunction(L, LuaTexture_new);
    lua_setfield(L, -2, "new");
    lua_setglobal(L, "LuaTexture");         //aj popne

    lua_pop(L, 1);

    //globalne funkcie
    lua_register(L, "LuaTexture_new", LuaTexture_new);
}



int LuaImage_gc(lua_State* L)
{
    LuaImage* lImage = (LuaImage*)luaL_checkudata(L, 1, "LuaImage");

    lImage->~LuaImage();

    return 0;
}

void register_LuaImage(lua_State* L)
{
    luaL_newmetatable(L, "LuaImage");

    lua_pushcfunction(L, LuaImage_gc);
    lua_setfield(L, -2, "__gc");

    lua_pushvalue(L, -1);       
    lua_setfield(L, -2, "__index");

    //luaL_Reg methods[] =
    //{
    //    {"set_values",  LuaRectTransform_set_values},
    //    { NULL, NULL }
    //};
    //luaL_setfuncs(L, methods, 0);

    //lua_newtable(L);
    //lua_pushcfunction(L, LuaImage_new);
    //lua_setfield(L, -2, "new");
    //lua_setglobal(L, "LuaRectTransform");

    lua_pop(L, 1);  //pop metatable
}



LuaRectTransform::LuaRectTransform() : rectTransform()
{
    printf("LuaRectTransform()\n");
}

LuaRectTransform::LuaRectTransform(RectTransform* parent) : rectTransform(parent)
{
    printf("LuaRectTransform(RectTransform*)\n");
}

/// <summary>
/// 
///     parametre: 0 az 1  
///         arg 1: type: userdata LuaRectTransform 
///             volitelny arg predstavujuci parenta
///         
///     returny 1
///         arg 1: type: userdata LuaRectTransform
/// 
/// </summary>
/// <param name="L"></param>
/// <returns></returns>
int LuaRectTransform_new(lua_State* L)
{
    void* mem = nullptr;
    LuaRectTransform* lRectTransformParent = nullptr;
    LuaRectTransform* lRectTransform = nullptr;

    switch (lua_gettop(L))
    {
    case 0:
        mem = lua_newuserdata(L, sizeof(LuaRectTransform));     // alokuje pamat, bez inicializacie
        lRectTransform = new (mem) LuaRectTransform();          // vytvori instanciu na danom bloku pamati

        luaL_getmetatable(L, "LuaRectTransform");   //pusne do staku metatable LuaRectTransform
        lua_setmetatable(L, -2);                    //setne objektu na -2 metatable z vrchu staku a popne, vrch staku

        return 1;                                   //pocet vratenych values metodou: 1 metatable

    case 1:
        lRectTransformParent = (LuaRectTransform*)luaL_checkudata(L, 1, "LuaRectTransform");

        mem = lua_newuserdata(L, sizeof(LuaRectTransform));
        lRectTransform = new (mem) LuaRectTransform(&lRectTransformParent->rectTransform);

        luaL_getmetatable(L, "LuaRectTransform");
        lua_setmetatable(L, -2);

        return 1;

    default:
        return luaL_error(L, "LuaRectTransform_new: nespravny pocet argumentov");
    }
}

int LuaRectTransform_gc(lua_State* L)
{
    LuaRectTransform* lRectTransform = (LuaRectTransform*)luaL_checkudata(L, 1, "LuaRectTransform");

    lRectTransform->~LuaRectTransform();

    return 0;
}

/// <summary>
/// 
///     parametre: 2 
///         arg 1: type: userdata LuaRectTransform 
///             self
///         arg 2: type: userdata LuaRectTransform
///             parent
/// 
///     returny 0
/// 
/// </summary>
/// <param name="L"></param>
/// <returns></returns>
int LuaRectTransform_set_parent(lua_State* L)
{
    if (lua_gettop(L) != 2)
    {
        return luaL_error(L, "LuaRectTransform_set_parent: nespravny pocet argumentov");
    }

    LuaRectTransform* self = (LuaRectTransform*)luaL_checkudata(L, 1, "LuaRectTransform");
    LuaRectTransform* parent = (LuaRectTransform*)luaL_checkudata(L, 2, "LuaRectTransform");
   
    self->rectTransform.set_parent(&parent->rectTransform);

    return 0;
}

/// <summary>
/// 
/// parametre: 2
///     arg 1: type: userdata LuaRectTransform
///         self
///     arg 2: type: table
///         tabulka, obsahujuca volitelne kluce
///             anchorMinX  (float)
///             anchorMinY  (float)
///             anchorMaxX  (float)
///             anchorMaxY  (float)
///             left        (float)
///             right       (float)
///             top         (float)
///             bottom      (float)
/// 
///     returny: 0
/// 
/// </summary>
/// <param name="L"></param>
/// <returns></returns>
int LuaRectTransform_set_values(lua_State* L)
{
    if (lua_gettop(L) != 2)
    {
        return luaL_error(L, "LuaRectTransform_set_values: nespravny pocet argumentov");
    }

    LuaRectTransform* self = (LuaRectTransform*)luaL_checkudata(L, 1, "LuaRectTransform");  // ak arg 1 nieje spravneho typu hodi error

    luaL_checktype(L, 2, LUA_TTABLE);    // ak arg 2 nieje spravneho typu hodi error

    // dve najcastejsie moznosti
    //lua_getfield(L, 1, "anchorMinX");
    // alebo
    //lua_pushstring(L, "anchorMinX");  // pusne key
    //lua_gettable(L, -2)               // popne key, pusne prislusnu value z tabulky, key v tabulke key nieje pusne nil 

    typedef void(RectTransform::* func)(float);
    const std::pair<const char*, func> pairs[]
    //const std::pair<const char*, void(RectTransform::*)(float)> pairs[]
    {
        { "anchorMinX", &RectTransform::set_anchorMinX },
        { "anchorMinY", &RectTransform::set_anchorMinY },
        { "anchorMaxX", &RectTransform::set_anchorMaxX },
        { "anchorMaxY", &RectTransform::set_anchorMaxY },
        { "left",   &RectTransform::set_left },
        { "right",  &RectTransform::set_right },
        { "top",    &RectTransform::set_top },
        { "bottom", &RectTransform::set_bottom }
    };

    for (auto& [name, func] : pairs)
    {
        lua_getfield(L, 2, name);
        //lua_Number number = lua_tonumber(L, -1);                  // Converts the Lua value at the given index to the C type lua_Number, ak konverzia neuspesna ziadna hlaska alebo error
        lua_Number number = luaL_checknumber(L, -1);                // Converts the Lua value at the given index to the C type lua_Number, ak konverzia neuspesna hodi error
        (self->rectTransform.*func)(static_cast<float>(number));
        lua_pop(L, 1);                                              // popne ziskanu value z tabulky (popuje zadany pocet values z vrchu staku)
    }

    return 0;
}

/// <summary>
/// 
/// parametre: 2 a viac, podla typu vytvaraneho behavioru
///     arg 1: type: userdata LuaRectTransform
///         self
///     arg 2: type:string 
///         nazov typu behavioru type string
/// 
/// returny: 1
///     arg 1: type: nil / userdata LuaImage /
///         vytvoreny behaviour alebo nil, ak sa nepodarilo
/// 
/// </summary>
/// <param name="L"></param>
/// <returns></returns>
int LuaRectTransform_add_behaviour(lua_State* L)
{
    if (lua_gettop(L) < 2)  // prvy a druhy parameter vzdy musia byt: self, string
    {
        return luaL_error(L, "LuaRectTransform_add_behaviour: arg 1 self a arg 2 string chibaju");
    }

    LuaRectTransform* self = (LuaRectTransform*)luaL_checkudata(L, 1, "LuaRectTransform");
    const char* str = luaL_checkstring(L, 2);
    std::string behaviourTypeStr = str;

    if (behaviourTypeStr == "Image")
    {
        // Image ma dva konstruktory
        //Image(RectTransform * transform);
        //Image(RectTransform * transform, SDL_Texture * texture);

        Image* image = nullptr;
        void* mem = nullptr;
        LuaImage* lImage = nullptr;
        LuaTexture* lTexture = nullptr;

        auto createLuaImage = [&L, &image, &mem, &lImage]() -> void
        { 
            if (image == nullptr)
            {
                lua_pushnil(L);
                return;
            }

            mem = lua_newuserdata(L, sizeof(LuaImage));
            lImage = new (mem) LuaImage();
            lImage->image = image;

            luaL_getmetatable(L, "LuaImage");
            lua_setmetatable(L, -2);
        };

        switch (lua_gettop(L))
        {
        case 2:

            //TODO: otestovat self->rectTransform.add_behaviour<>(); ako sa sprava ked dame nespravnu vec
            image = self->rectTransform.add_behaviour<Image>();

            //if (image == nullptr)
            //{
            //    lua_pushnil(L);
            //    return 1;
            //}

            //mem = lua_newuserdata(L, sizeof(LuaImage));
            //lImage = new (mem) LuaImage();
            //lImage->image = image;

            //luaL_getmetatable(L, "LuaImage");
            //lua_setmetatable(L, -2);

            createLuaImage();
            return 1;

        case 3:

            lTexture = (LuaTexture*)luaL_checkudata(L, 3, "LuaTexture");

            //TODO: otestovat self->rectTransform.add_behaviour<>(); ako sa sprava ked dame nespravnu vec
            image = self->rectTransform.add_behaviour<Image>(lTexture->texture.get());

            //if (image == nullptr)
            //{
            //    lua_pushnil(L);
            //    return 1;
            //}

            //mem = lua_newuserdata(L, sizeof(LuaImage));
            //lImage = new (mem) LuaImage();
            //lImage->image = image;

            //luaL_getmetatable(L, "LuaImage");
            //lua_setmetatable(L, -2);

            createLuaImage();
            return 1;

        default:
            return luaL_error(L, "LuaRectTransform_set_values: nespravny pocet argumentov");
            //break;
        }
    }
    //else if (behaviourTypeStr == "Button")
    //{
    //
    //}
    else
    {
        lua_pushnil(L);
        return 1;
    }
}

void register_LuaRectTransform(lua_State* L)
{
    luaL_newmetatable(L, "LuaRectTransform");   // vytvor metatable

    lua_pushcfunction(L, LuaRectTransform_gc);  // pusne metodu na stak
    lua_setfield(L, -2, "__gc");                // setne metodu do metatable, popne metodu zo staku

    lua_pushvalue(L, -1);               // skopiruje metatable na vrchu staku (referenciu) a pusne kopiu na stak
    lua_setfield(L, -2, "__index");     // metatable si nastavi do __index fildu kopiue seba z vrchu staku, popne zo staku kopiu metatable

    luaL_Reg methods[] = 
    {
        {"set_parent",      LuaRectTransform_set_parent},
        {"set_values",      LuaRectTransform_set_values},
        {"add_behaviour",   LuaRectTransform_add_behaviour},
        { NULL, NULL }                                      // pole tohoto typu musi koncit vzdy takto
    };
    luaL_setfuncs(L, methods, 0);                           // Registers all functions in the array into the table on the top of the stack, treti parameter je pocet upvalues

    lua_newtable(L);                                        // vytvori table, pusne na stak
    lua_pushcfunction(L, LuaRectTransform_new);             // pusne funkciu na stak
    lua_setfield(L, -2, "new");                             // setne funkciu do tabulky, popne funkciu zo staku
    lua_setglobal(L, "LuaRectTransform");                   // Pops a value from the stack and sets it as the new value of global name.

    lua_pop(L, 1);                                          // popne metatable (popuje zadany pocet values z vrchu staku)

    //globalne funkcie
    lua_register(L, "LuaRectTransform_new", LuaRectTransform_new);
    //lua_register(L, "LuaRectTransform_set_parent", LuaRectTransform_set_parent);
}



void register_API(lua_State* L)
{
    register_LuaRectTransform(L);
    register_LuaTexture(L);
    register_LuaImage(L);


    // ak potrebujeme, aby mala metoda pristup k niecomu specifickemu, musime ju registrovat v closure, ktory zaobaluje lightuserdata a funkciu
    //lua_pushlightuserdata(L, /* void**  */ );
    //lua_pushcclosure(
    //    L, 
    //    [](lua_State* L) -> int 
    //    { 
    //        // Check we got exactly 1 argument (optional but good)
    //        luaL_checktype(L, 1, LUA_TUSERDATA);
    //
    //        // Get userdata argument
    //        LuaRectTransform* ud = (LuaRectTransform*)luaL_checkudata(L, 1, "LuaRectTransform");
    //
    //        // upvalue = RectTransform**
    //        RectTransform** rootRectTransform = (RectTransform**)lua_touserdata(L, lua_upvalueindex(1));
    //
    //        // Assign global pointer
    //        *rootRectTransform = &ud->rectTransform;
    //
    //        return 0; // no return values
    //    },
    //    1);
    //lua_setglobal(L, "set_rootRectTransform");
}

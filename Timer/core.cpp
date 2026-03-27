#include <algorithm>
#include <stdexcept>

#include <SDL3_image/SDL_image.h>

#include "core.hpp"
#include "context.hpp"



RectTransform::RectTransform() : 
    m_anchorMinX(.0f), 
    m_anchorMinY(.0f), 
    m_anchorMaxX(1.f), 
    m_anchorMaxY(1.f),
    m_left(.0f),
    m_right(.0f),
    m_top(.0f),
    m_bottom(.0f),
    //m_pivotOffsetX(.0f),
    //m_pivotOffsetY(.0f),
    m_rect(),
    m_scene(NULL),
    m_parent(NULL),
    m_children(),
    m_behaviours()
{
    std::printf("RectTransform::RectTransform()\n");

    m_scene = SceneManager::get_activeScene();
    m_scene->add_child(this);
}

RectTransform::RectTransform(RectTransform* parent) :
    m_anchorMinX(.0f),
    m_anchorMinY(.0f),
    m_anchorMaxX(1.f),
    m_anchorMaxY(1.f),
    m_left(.0f),
    m_right(.0f),
    m_top(.0f),
    m_bottom(.0f),
    //m_pivotOffsetX(.0f),
    //m_pivotOffsetY(.0f),
    m_rect(),
    m_scene(NULL),
    m_parent(NULL),
    m_children(),
    m_behaviours()
{
    std::printf("RectTransform::RectTransform(RectTransform* parent)\n");

    m_scene = parent->m_scene;
    parent->add_child(this);
}

RectTransform::~RectTransform() 
{
    std::printf("RectTransform::~RectTransform()\n");

    // TODO: treba objekt odobrat z hierarchie RectTransformov, pripadne zo scene

    // odpojenie od parenta
    set_parent(NULL);

    // odpojenie vsetkych potomkov, !!! stracam referenci na potomkov, treba ich niekde odlozit !!!
    while (get_childCount() > 0)
    {
        RectTransform* child = get_child(0);
        remove_child(child);
    }
}



RectTransform*& RectTransform::operator[](int idx)
{
    return get_child(idx);
}



float RectTransform::get_anchorMinX() const
{
    return m_anchorMinX;
}

void RectTransform::set_anchorMinX(float value)
{
    //m_anchorMinX = std::min(std::max(.0f, value), 1.f);
    m_anchorMinX = std::clamp(value, .0f, 1.f);
}

float RectTransform::get_anchorMinY() const
{
    return m_anchorMinY;
}

void RectTransform::set_anchorMinY(float value)
{
    m_anchorMinY = std::clamp(value, .0f, 1.f);
}

float RectTransform::get_anchorMaxX() const
{
    return m_anchorMaxX;
}

void RectTransform::set_anchorMaxX(float value)
{
    m_anchorMaxX = std::clamp(value, .0f, 1.f);
}

float RectTransform::get_anchorMaxY() const
{
    return m_anchorMaxY;
}
 
void RectTransform::set_anchorMaxY(float value)
{
    m_anchorMaxY = std::clamp(value, .0f, 1.f);
}   

float RectTransform::get_left() const
{
    return m_left;
} 

void RectTransform::set_left(float value)
{
    m_left = value;
}         

float RectTransform::get_right() const
{
    return m_right;
}

void RectTransform::set_right(float value)
{
    m_right = value;
}

float RectTransform::get_top() const
{
    return m_top;
}    

void RectTransform::set_top(float value)
{
    m_top = value;
}         

float RectTransform::get_bottom() const
{
    return m_bottom;
}

void RectTransform::set_bottom(float value)
{
    m_bottom = value;
} 

SDL_FRect RectTransform::get_rect() const
{
    return m_rect;
}

Scene* RectTransform::get_scene()
{
    return m_scene;
}

RectTransform* RectTransform::get_parent() 
{
    return m_parent; 
}

void RectTransform::set_parent(RectTransform* parent)
{
    // novy parent moze byt null
    //if (parent == NULL) throw std::runtime_error("RectTransform::set_parent: arg parent nemoze byt null");

    if (m_parent == NULL && parent != NULL)
    {
        // zobrat zo sceny, dat pod noveho parenta

        // parent a child musia byt z tej istej sceny
        if (parent->m_scene != m_scene) throw std::runtime_error("RectTransform::set_parent: nemozem dat child parentovi z inej sceny");

        m_scene->remove_child(this);
        parent->add_child(this);
    }
    else if (m_parent != NULL && parent == NULL)
    {
        // zobrat zo stareho parenta, dat do sceny

        m_parent->remove_child(this);
        m_scene->add_child(this);
    }
    else if (m_parent != NULL && parent != NULL)
    {
        // zobrat zo stareho parenta, dat pod novy parent

        // parent a child musia byt z tej istej sceny
        if (parent->m_scene != m_scene) throw std::runtime_error("RectTransform::set_parent: nemozem dat child parentovi z inej sceny");

        m_parent->remove_child(this);
        parent->add_child(this);
    }
    //else if (m_parent == NULL && parent == NULL)
    //{
    //    // NIC
    //}
}

int RectTransform::get_childCount() noexcept
{
    return static_cast<int>(m_children.size());
}

RectTransform*& RectTransform::get_child(int idx)
{
    if (0 <= idx && idx < m_children.size())
    {
        return m_children[idx];
    }
    else
    {
        throw std::runtime_error("RectTransform::get_child: arg idx mimo hranic");

        //return NULL;
    }
}



RectTransform* RectTransform::create_child()
{
    auto child = new RectTransform(this);

    child->m_parent = this;
    m_children.push_back(child);

    return child;
}

void RectTransform::add_child(RectTransform* child)
{
    // child nemoze byt null
    if (child == NULL) throw std::runtime_error("RectTransform::add_child: arg child nemoze byt null");

    // parent a child musia byt z tej istej sceny
    if (m_scene != child->m_scene) throw std::runtime_error("RectTransform::add_child: nemozem dat child parentovi z inej sceny");

    // child nemoze byt viacejkrat pod parentom, alebo inde v hierarchi sceny
    //if (std::find(m_children.begin(), m_children.end(), child) != m_children.end())
    //{
    //    throw std::runtime_error("RectTransform::add_child: arg child us je potomkom");
    //}
     
    if (child->m_parent != NULL)
    {
        // child ma parenta, musime odobrat child z parenta
        child->m_parent->remove_child(child);
    }
    else
    {
        // child nema parenta je vo kontente sceny, musime odobrat child zo sceny
        //SceneManager::get_activeScene()->remove_child(child);
        child->m_scene->remove_child(child);
    }

    child->m_parent = this;
    m_children.push_back(child);
}

void RectTransform::remove_child(RectTransform* child)
{
    // child nemoze byt null
    if (child == NULL) throw std::runtime_error("RectTransform::remove_child: arg child nemoze byt null");

    // ak child nieje v m_children nic nespravim s child

    //auto it = std::remove(m_children.begin(), m_children.end(), child);
    //m_children.erase(it, m_children.end());

    auto it = std::remove_if(m_children.begin(), m_children.end(),
        [child](RectTransform* ptr)
        {
            return ptr == child;
        });
    
    //vymazat parenta z child
    for (auto start = it; start != m_children.end(); start++)
    {
        (*start)->m_parent = NULL;
    }

    //vymaze koniec vektora, kde je child
    m_children.erase(it, m_children.end());
}

void RectTransform::update_rect(SDL_FRect rect)
{
    float aa = m_anchorMinX * rect.w;
    float bb = m_anchorMinY * rect.h;
    float aX = rect.x + aa + m_left;
    float aY = rect.y + bb + m_top; 
    float aW = rect.w * m_anchorMaxX - aa - m_right;
    float aH = rect.h * m_anchorMaxY - bb - m_bottom;

    m_rect.x = aX;
    m_rect.y = aY;
    m_rect.w = aW;
    m_rect.h = aH;
}

void RectTransform::draw(SDL_Renderer* renderer) const
{   
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderLine(renderer, m_rect.x, m_rect.y, m_rect.x + m_rect.w, m_rect.y);
    SDL_RenderLine(renderer, m_rect.x + m_rect.w, m_rect.y, m_rect.x + m_rect.w, m_rect.y + m_rect.h);
    SDL_RenderLine(renderer, m_rect.x + m_rect.w, m_rect.y + m_rect.h, m_rect.x, m_rect.y + m_rect.h);
    SDL_RenderLine(renderer, m_rect.x, m_rect.y + m_rect.h, m_rect.x, m_rect.y);
}

//Scene--------------------------------------------------------------------------------------------------------------

Scene::Scene() : m_content() {}

Scene::~Scene()
{
    // TODO: vymazat transformy zo sceny
}

void Scene::add_child(RectTransform* child)
{
    // nemoze byt null
    if (child == NULL) throw std::runtime_error("Scene::add_child: arg child nemoze byt null");

    // ak child ma parenta, musime odobrat child z parenta
    if (child->get_parent() != NULL)
    {
        child->get_parent()->remove_child(child);
    }

    m_content.push_back(child);
}

void Scene::remove_child(RectTransform* child)
{
    // child nemoze byt null
    if (child == NULL) throw std::runtime_error("Scene::remove_child: arg child nemoze byt null");

    // ak child nieje v m_children nic nespravim s child

    //auto it = std::remove(m_content.begin(), m_content.end(), child);
    //m_content.erase(it, m_content.end());

    auto it = std::remove_if(m_content.begin(), m_content.end(),
        [child](RectTransform* ptr)
        {
            return ptr == child;
        });

    // ak child ma null parenta je to chyba
    for (auto start = it; start != m_content.end(); start++)
    {
        if ((*start)->get_parent() != NULL) throw std::runtime_error("Scene::remove_child: child nachadzajuci sa v kontente sceny nemoze mat parenta");
    }

    //vymaze koniec vektora, kde je child
    m_content.erase(it, m_content.end());
}

const std::vector<RectTransform*>& Scene::get_content()
{
    return m_content;
}

//SceneManager-------------------------------------------------------------------------------------------------------

Scene* SceneManager::m_activeScene = NULL;

Scene* SceneManager::get_activeScene()
{
    if (m_activeScene == NULL)
    {
        m_activeScene = new Scene();
    }

    return m_activeScene;
}

//Time---------------------------------------------------------------------------------------------------------------

float Time::m_deltaTime = .0f;

float Time::get_deltaTime()
{
    return m_deltaTime;
}

void Time::set_deltaTime(float deltaTime)
{
    m_deltaTime = deltaTime;
}

//Behaviour----------------------------------------------------------------------------------------------------------

Behaviour::Behaviour(RectTransform* transform) : m_transform(transform) {}

RectTransform* Behaviour::get_transform()
{
    return m_transform;
}

//Image--------------------------------------------------------------------------------------------------------------

inline ImageAlignHorizontal operator&(ImageAlignHorizontal a, ImageAlignHorizontal b)
{
    return static_cast<ImageAlignHorizontal>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}

inline ImageAlignVertical operator&(ImageAlignVertical a, ImageAlignVertical b)
{
    return static_cast<ImageAlignVertical>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}

Image::Image(RectTransform* transform, SDL_Texture* texture) : 
    Behaviour(transform), 
    m_texture(texture),
    m_textureRect(),
    m_textureAsp(.0f),
    preserveAspectRation(false),
    alignHorizontal(ImageAlignHorizontal::CENTER),
    alignVertical(ImageAlignVertical::CENTER)
{
    if (!transform) throw std::runtime_error("arg transform is null");
    set_texture(texture);
}

SDL_Texture* Image::get_texture()
{
    return m_texture;
}

void Image::set_texture(SDL_Texture* texture)
{
    if (!texture)
    {
        m_texture = NULL;
        m_textureRect.x = .0f;
        m_textureRect.y = .0f;
        m_textureRect.w = .0f;
        m_textureRect.h = .0f;
        m_textureAsp = .0f;
    }
    else
    {
        m_texture = texture;
        m_textureRect.x = .0f;
        m_textureRect.y = .0f;
        m_textureRect.w = static_cast<float>(m_texture->w);
        m_textureRect.h = static_cast<float>(m_texture->h);
        m_textureAsp = m_textureRect.w / m_textureRect.h;
    }
}

void Image::update()
{
    SDL_FRect dRect = get_transform()->get_rect();

    if (preserveAspectRation)
    {
        calculateAspRect(&dRect);
    }

    if (m_texture)
    {
        SDL_RenderTexture(Context::renderer, m_texture, &m_textureRect, &dRect);
    }
    else
    {
        //Magenta color(RGB: 255, 0, 255)
        SDL_SetRenderDrawColor(Context::renderer, 255, 0, 255, 255);
        SDL_RenderFillRect(Context::renderer, &dRect);
    }
}

void Image::calculateAspRect(SDL_FRect* aspRect)
{
    if (aspRect->h * m_textureAsp <= aspRect->w)
    {
        if ((alignHorizontal & ImageAlignHorizontal::CENTER) == ImageAlignHorizontal::CENTER)
        {
            aspRect->x += aspRect->w / 2.f - (aspRect->h * m_textureAsp) / 2.f;
            //printf("ImageAlign::CENTER_HORIZONTAL\n");
        }
        //else if ((alignHorizontal & ImageAlignHorizontal::LEFT) == ImageAlignHorizontal::LEFT)
        //{
            //netreba ziadny vypocet
            //printf("ImageAlign::LEFT\n");
        //}
        else if ((alignHorizontal & ImageAlignHorizontal::RIGHT) == ImageAlignHorizontal::RIGHT)
        {
            aspRect->x += aspRect->w - aspRect->h * m_textureAsp;
            //printf("ImageAlign::RIGHT\n");
        }

        aspRect->w = aspRect->h * m_textureAsp;
    }
    else
    {
        if ((alignVertical & ImageAlignVertical::CENTER) == ImageAlignVertical::CENTER)
        {
            aspRect->y += aspRect->h / 2.f - (aspRect->w * m_textureAsp) / 2.f;
            //printf("ImageAlign::CENTER_VERTICAL\n");
        }
        //else if ((alignVertical & ImageAlignVertical::TOP) == ImageAlignVertical::TOP)
        //{
            //netreba ziadny vypocet
            //printf("ImageAlign::TOP\n");
        //}
        else if ((alignVertical & ImageAlignVertical::BOTTOM) == ImageAlignVertical::BOTTOM)
        {
            aspRect->y += aspRect->h - aspRect->w * m_textureAsp;
            //printf("ImageAlign::BOTTOM\n");
        }

        aspRect->h = aspRect->w / m_textureAsp;
    }
}

// Lua API ----------------------------------------------------------------------------------------------------------

int LuaTexture_new(lua_State* L)
{

    if (Context::pathExe == NULL)
    {
        return luaL_error(L, "LuaTexture_new: Context::pathExe je null");
    }

    const char* relativePath;
    std::string fullPath = std::string{ Context::pathExe } + relativePath;
    printf("fullPath: %s\n", fullPath.c_str());

    SDL_Surface* surface = IMG_Load(fullPath.c_str());

    if (!surface)
    {
        printf("LuaTexture_new: IMG_Load failed: %s", SDL_GetError());
        return luaL_error(L, "LuaTexture_new: IMG_Load failed");
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Context::renderer, surface);
    SDL_DestroySurface(surface);
    surface = NULL;

    if (!texture)
    {
        printf("LuaTexture_new: SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
        return luaL_error(L, "LuaTexture_new: SDL_CreateTextureFromSurface failed");
    }



    return 1;
}

int LuaTexture_gc(lua_State* L)
{
    if (texturePikachu)
    {
        SDL_DestroyTexture(texturePikachu);
        texturePikachu = nullptr;
    }

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
    lua_setglobal(L, "LuaTexture");

    lua_pop(L, 1);

    //globalne funkcie
    lua_register(L, "LuaTexture_new", LuaTexture_new);
}



int LuaRectTransform_new(lua_State* L)
{
    // argumenty: 0

    // TODO: spravne implementovat konstroktory: RectTransform(),  RectTransform(RectTransform* parent)

    // Allocate userdata 
    // variant 1: alokuje len pamat, nevytvori sa object, nic sa neinicializuje, dostanem len alokovany pamet plnu odpadu
    //LuaRectTransform* ud = (LuaRectTransform*)lua_newuserdata(L, sizeof(LuaRectTransform));

    //variant 2: alokuje pamet a na alokovanej pameti vytvori/inicializuje objekt konstruktorom
    void* mem = lua_newuserdata(L, sizeof(LuaRectTransform));
    LuaRectTransform* ud = new (mem) LuaRectTransform();

    //keby som ma vo vnutry pointer, vytvaral by som instanciu na heape pomocou new, potom v destruktore by som ho musel nicit pomocou delete 
    //ud->obj = new RectTransform();

    //Set metatable
    luaL_getmetatable(L, "LuaRectTransform");   //pusne do staku na koniec metatable LuaRectTransform
    lua_setmetatable(L, -2);                    //setne objektu na -2 metatable z konca staku a popne, posledny prvok zo staku

    // Return userdata to Lua
    return 1;
}

int LuaRectTransform_gc(lua_State* L)
{
    LuaRectTransform* ud = (LuaRectTransform*)luaL_checkudata(L, 1, "LuaRectTransform");

    // destrukcia, ak bol objekt vytvoreny takto:
    //void* mem = lua_newuserdata(L, sizeof(LuaRectTransform));
    //LuaRectTransform* ud = new (mem) LuaRectTransform();
    if (ud)
    {
        ud->~LuaRectTransform();
    }

    // destrukcia, ak bol objekt vytvoreny takto, a vo vnutry by bol smernik:
    //LuaRectTransform* ud = (LuaRectTransform*)lua_newuserdata(L, sizeof(LuaRectTransform));
    //if (ud)
    //{
    //    delete ud->obj;
    //}
    
    return 0;
}

int LuaRectTransform_set_parent(lua_State* L)
{
    // parametre: 2 
    //  arg 1: self,    typ: userdata typu LuaRectTransform 
    //  arg 2: parent,  typ: userdata typu LuaRectTransform

    if (lua_gettop(L) != 2)
    {
        return luaL_error(L, "LuaRectTransform_set_parent: nespravny pocet argumentov");
    }

    LuaRectTransform* self = (LuaRectTransform*)luaL_checkudata(L, 1, "LuaRectTransform");
    LuaRectTransform* parent = (LuaRectTransform*)luaL_checkudata(L, 2, "LuaRectTransform");
   
    self->rectTransform.set_parent(&parent->rectTransform);

    return 0;
}

int LuaRectTransform_set_values(lua_State* L)
{
    // parametre: 2
    //  arg 1: self type: userdata LuaRectTransform
    //  arg 2: tabulka, ktora moze obsahujuca nasledujuce kluce
    //      anchorMinX  (float)
    //      anchorMinY  (float)
    //      anchorMaxX  (float)
    //      anchorMaxY  (float)
    //      left        (float)
    //      right       (float)
    //      top         (float)
    //      bottom      (float)

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


void register_LuaRectTransform(lua_State* L)
{
    luaL_newmetatable(L, "LuaRectTransform");   // vytvor metatable

    lua_pushcfunction(L, LuaRectTransform_gc);  // pusne metodu na stak
    lua_setfield(L, -2, "__gc");                // setne metodu do metatable, popne metodu zo staku

    lua_pushvalue(L, -1);               // skopiruje metatable na vrchu staku (referenciu) a pusne kopiu na stak
    lua_setfield(L, -2, "__index");     // metatable si nastavi do __index fildu kopiue seba z vrchu staku, popne zo staku kopiu metatable

    luaL_Reg methods[] = 
    {
        {"set_parent",  LuaRectTransform_set_parent},
        {"set_values",  LuaRectTransform_set_values},
        { NULL, NULL }                                   // pole tohoto typu musi koncit vzdy takto
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



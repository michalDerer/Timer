#include <algorithm>
#include <stdexcept>

#include "core.hpp"


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
    m_parent(NULL),
    m_children(),
    m_behaviours()
{
    std::printf("RectTransform::RectTransform()\n");
}

RectTransform::~RectTransform() 
{
    // !!! treba RectTransform odobrat z hierarchie, to nastava tuna v destruktore !!!
    std::printf("RectTransform::~RectTransform()\n");

    // odpojenie od parenta
    set_parent(NULL);

    // odpojenie vsetkych potomkov, !!! stracam referenci na potomkov, treba ich niekde odlozit !!!
    while (get_childCount() > 0)
    {
        RectTransform* child = get_child(0);
        remove_child(child);
    }
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

RectTransform* RectTransform::get_parent() 
{
    return m_parent; 
}

void RectTransform::set_parent(RectTransform* parent)
{
    // parent moze byt null
    //if (parent == NULL) throw std::runtime_error("RectTransform::set_parent: arg parent nemoze byt null");

    // ak ma child parenta odoberam child staremu parentovy
    if (m_parent != NULL)
    {
        m_parent->remove_child(this);
    }

    // ak novy parent existuje, child mu pridam
    if (parent != NULL)
    { 
        parent->add_child(this);
    }
}

int RectTransform::get_childCount() noexcept
{
    return static_cast<int>(m_children.size());
}

RectTransform* RectTransform::get_child(int idx)
{
    if (0 <= idx && idx < m_children.size())
    {
        return m_children[idx];
    }
    else
    {
        return NULL;
    }
}



RectTransform* RectTransform::create_child()
{
    /*
    auto child = std::make_unique<RectTransform>();
    child->parent = this;
    RectTransform* rawPtr = child.get();
    children.push_back(std::move(child));

    return rawPtr;
    */

    auto child = new RectTransform();

    child->m_parent = this;
    m_children.push_back(child);

    return child;
}

void RectTransform::add_child(RectTransform* child)
{
    // child nemoze byt null
    if (child == NULL) throw std::runtime_error("RectTransform::add_child: arg child nemoze byt null");

    // child nemoze byt viackrat pod parentom
    //if (std::find(m_children.begin(), m_children.end(), child) != m_children.end())
    //{
    //    throw std::runtime_error("RectTransform::add_child: arg child us je potomkom");
    //}

    // ak child us ma parenta, musime najprv odobrat child z parenta
    if (child->m_parent != NULL)
    {
        child->m_parent->remove_child(child);
    }

    child->m_parent = this;
    m_children.push_back(child);
}

void RectTransform::remove_child(RectTransform* child)
{
    // child nemoze byt null
    if (child == NULL) throw std::runtime_error("RectTransform::remove_child: arg child nemoze byt null");

    // ak child nieje v m_children nic snim nespravi

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

//Time---------------------------------------------------------------------------------------------------------------

Scene::Scene() : content() {}

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
    Image::update(renderer);
};

void Image::update(SDL_Renderer* renderer)
{
    SDL_FRect dRect = get_transform()->get_rect();

    if (preserveAspectRation)
    {
        calculateAspRect(&dRect);
    }

    if (m_texture)
    {
        SDL_RenderTexture(renderer, m_texture, &m_textureRect, &dRect);
    }
    else
    {
        //Magenta color(RGB: 255, 0, 255)
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        //SDL_GetRenderDrawColor()
        SDL_RenderFillRect(renderer, &dRect);
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



int LuaRectTransform_createInstance(lua_State* L)
{
    // argumenty: 0

    // Allocate userdata 
    // variant 1: alokuje len pamat, nevytvori sa object, nic sa neinicializuje, dostanem len alokovany pamet plnu odpadu
    //LuaRectTransform* ud = (LuaRectTransform*)lua_newuserdata(L, sizeof(LuaRectTransform));

    //variant 2: alokuje pamet a na alokovanej pameti vytvori/inicializuje objekt konstruktorom
    void* mem = lua_newuserdata(L, sizeof(LuaRectTransform));
    LuaRectTransform* ud = new (mem) LuaRectTransform();

    //keby som ma vo vnutry pointer, vytvaral by som instanciu na heape pomocou new, potom v destruktore by som ho musel nicit pomocou delete 
    //ud->obj = new RectTransform();

    ud->obj.set_anchorMinX(.15f);
    ud->obj.set_anchorMinY(.15f);
    ud->obj.set_anchorMaxX(.85f);
    ud->obj.set_anchorMaxY(.85f);

    //Set metatable
    luaL_getmetatable(L, "LuaRectTransform");   //pusne do staku na koniec metatable LuaRectTransform
    lua_setmetatable(L, -2);                    //setne objektu na -2 metatable z konca staku a popne, posledny prvok zo staku

    // Return userdata to Lua
    return 1;
}

int LuaRectTransform_gc(lua_State* L)
{
    LuaRectTransform* ud = (LuaRectTransform*)luaL_checkudata(L, 1, "LuaRectTransform");

    // !!! pred znicenim este treba RectTransform odobrat z hierarchie, to nastava v destruktore !!!

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
    //argumenty: 2 parent, child

    LuaRectTransform* parent = (LuaRectTransform*)luaL_checkudata(L, 1, "LuaRectTransform");
    LuaRectTransform* child = (LuaRectTransform*)luaL_checkudata(L, 2, "LuaRectTransform");

    child->obj.set_parent(&parent->obj);
    return 0;
}


int set_rootRectTransform(lua_State* L)
{
    // Check we got exactly 1 argument (optional but good)
    luaL_checktype(L, 1, LUA_TUSERDATA);

    // Get userdata argument
    LuaRectTransform* ud = (LuaRectTransform*)luaL_checkudata(L, 1, "LuaRectTransform");

    // upvalue = RectTransform**
    RectTransform** rootRectTransform = (RectTransform**)lua_touserdata(L, lua_upvalueindex(1));

    // Assign global pointer
    *rootRectTransform = &ud->obj;

    return 0; // no return values
}

void register_LuaRectTransform(lua_State* L)
{
    luaL_newmetatable(L, "LuaRectTransform");

    lua_pushcfunction(L, LuaRectTransform_gc);
    lua_setfield(L, -2, "__gc");

    lua_pop(L, 1);
}

void register_API(lua_State* L, void** rootRectTransform)
{
    register_LuaRectTransform(L);
    lua_register(L, "LuaRectTransform_createInstance", LuaRectTransform_createInstance);
    lua_register(L, "LuaRectTransform_set_parent", LuaRectTransform_set_parent);

    {
        //if not closure is used
        //lua_register(L, "set_rootRectTransform", set_rootRectTransform);
        
        // if closure is used, for upvalue
        lua_pushlightuserdata(L, rootRectTransform);
        lua_pushcclosure(L, set_rootRectTransform, 1);
        lua_setglobal(L, "set_rootRectTransform");
    }
}



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
{}



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
    m_parent = parent;
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

    return m_children.back();
}

void RectTransform::add_child(RectTransform* child)
{
    child->m_parent = this;
    //m_children.push_back(std::move(child));
    m_children.push_back(child);
}

void RectTransform::remove_child(RectTransform* child)
{
    //auto it = std::remove(m_children.begin(), m_children.end(), child);
    //m_children.erase(it, m_children.end());

    auto it = std::remove_if(m_children.begin(), m_children.end(),
        [child](RectTransform* ptr)
        {
            return ptr == child;
        });
    
    //vymaze koniec vektora
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
    int v = luaL_checkinteger(L, 1); // argument from Lua

    //Allocate userdata
    LRectTransform* ud = (LRectTransform*)lua_newuserdata(L, sizeof(LRectTransform));

    // Create C++ object
    ud->obj = new RectTransform();

    //Set metatable
    luaL_getmetatable(L, "LRectTransform_meta");
    lua_setmetatable(L, -2);

    // Return userdata to Lua
    return 1;
}




int l_sin(lua_State* L)
{
    //double d = lua_tonumber(L, 1);  /* get argument */
    double d = luaL_checknumber(L, 1);
    lua_pushnumber(L, sin(d));  /* push result */

    return 1;  /* number of results */
}
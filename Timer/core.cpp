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

int RectTransform::get_child_count() noexcept
{ 
    // return static_cast<int>(children.size());
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

void RectTransform::remove_child(RectTransform*& child)
{
    //poposuva platne prvky dolava a necha volne prvky na konci
    //it je koniec prvkov, ktore splnili podmienku
    auto it = std::remove_if(m_children.begin(), m_children.end(),
        [&child](RectTransform*& ptr)
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

Image::Image(RectTransform* transform, SDL_Texture* texture) : Behaviour(transform), m_texture(texture) 
{
    if (!transform) throw std::runtime_error("arg transform is null");
}

SDL_Texture* Image::get_texture()
{
    return m_texture;
}

void Image::update()
{

};

void Image::update(SDL_Renderer* renderer)
{
    SDL_FRect dRect = get_transform()->get_rect();

    if (m_texture)
    {
        SDL_FRect sRect
        {
            .w = static_cast<float>(m_texture->w),
            .h = static_cast<float>(m_texture->h)
        };

        SDL_RenderTexture(renderer, m_texture, &sRect, &dRect);
    }
    else
    {
        //Magenta color(RGB: 255, 0, 255)
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        //SDL_GetRenderDrawColor()
        SDL_RenderFillRect(renderer, &dRect);
    }
}
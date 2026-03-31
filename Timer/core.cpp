#include <algorithm>
#include <stdexcept>

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
    std::printf("RectTransform::RectTransform(RectTransform*)\n");

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

Behaviour::Behaviour(RectTransform* transform) : m_transform(transform) 
{
    if (!transform) throw std::runtime_error("arg transform is null");
}

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

Image::Image(RectTransform* transform) :
    Behaviour(transform),
    m_texture(nullptr),
    m_textureRect(),
    m_textureAsp(.0f),
    preserveAspectRation(false),
    alignHorizontal(ImageAlignHorizontal::CENTER),
    alignVertical(ImageAlignVertical::CENTER)
{
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



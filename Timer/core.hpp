#pragma once

#include <vector>

#include "SDL3/SDL.h"

#include "lua.hpp"



//----------------------------------------------------------------------------------------------------------
//FPRWARD DECLARATIONS

class RectTransform;    

//----------------------------------------------------------------------------------------------------------
//GLOBAL WARIABLES DECLARATIONS

extern SDL_Renderer* renderer;

//----------------------------------------------------------------------------------------------------------

/// <summary>
/// 
/// </summary>
class Time
{
    static float m_deltaTime;

public:

    static float get_deltaTime();
    static void set_deltaTime(float deltaTime);

};

//----------------------------------------------------------------------------------------------------------

/// <summary>
/// 
/// </summary>
class Behaviour
{
    RectTransform* m_transform;

public:

    Behaviour()                                         = delete;   //default constructor
    Behaviour(RectTransform* transform);

    Behaviour(const Behaviour& other)                   = delete;   //copy constructor
    Behaviour& operator=(const Behaviour& other)        = delete;   //copy assignment

    Behaviour(Behaviour&& other) noexcept               = default;  //move constructor
    Behaviour& operator=(Behaviour&& other) noexcept    = default;  //move assignment

    virtual ~Behaviour() = default;



    RectTransform* get_transform();



    virtual void update() = 0;
};

//----------------------------------------------------------------------------------------------------------

enum class ImageAlignHorizontal : unsigned int
{
    CENTER = 1 << 0,
    LEFT = 1 << 1,
    RIGHT = 1 << 2
};

//inline ImageAlignHorizontal operator|(ImageAlignHorizontal a, ImageAlignHorizontal b);
inline ImageAlignHorizontal operator&(ImageAlignHorizontal a, ImageAlignHorizontal b);

enum class ImageAlignVertical : unsigned int
{
    CENTER = 1 << 0,
    TOP = 1 << 1,
    BOTTOM = 1 << 2
};

//inline ImageAlignVertical operator|(ImageAlignVertical a, ImageAlignVertical b);
inline ImageAlignVertical operator&(ImageAlignVertical a, ImageAlignVertical b);

//----------------------------------------------------------------------------------------------------------

class Image : public Behaviour
{
    SDL_Texture* m_texture;
    SDL_FRect m_textureRect;
    float m_textureAsp;

public:

    bool preserveAspectRation = false;
    /// <summary>
    /// Pouziva len jednu z moznych hodnot enumu.
    /// </summary>
    ImageAlignHorizontal alignHorizontal;
    /// <summary>
    /// Pouziva len jednu z moznych hodnot enumu.
    /// </summary>
    ImageAlignVertical alignVertical;

public:

    Image()                                                 = delete;   //default constructor
    Image(RectTransform* transform, SDL_Texture* texture);

    Image(const Image& other)                               = delete;   //copy constructor
    Image& operator=(const Image& other)                    = delete;   //copy assignment

    Image(Image&& other) noexcept                           = delete;   //move constructor
    Image& operator=(Image&& other) noexcept                = delete;   //move assignment

    ~Image() override                                       = default;



    SDL_Texture* get_texture();
    void set_texture(SDL_Texture* texture);


    void update() override;
    void update(SDL_Renderer* renderer);

private:

    void calculateAspRect(SDL_FRect* rect);
};

//----------------------------------------------------------------------------------------------------------

class RectTransform
{

    float m_anchorMinX;   //relativny posun zlava     rozsah 0f - 1f
    float m_anchorMinY;   //relativny posun zhora     rozsah 0f - 1f

    float m_anchorMaxX;   //relativny posun zprava    rozsah 0f - 1f
    float m_anchorMaxY;   //relativny posun zdola     rozsah 0f - 1f

    float m_left;         //posun aplikovany na anchorMinX dovnutra, ak anchorMinX == anchorMaxX tak funkcia nezmenena
    float m_right;        //posun aplikovany na anchorMaxX dovnutra, ak anchorMinX == anchorMaxX tak right sa stava width
    float m_top;          //posun aplikovany na anchorMinY dovnutra, ak anchorMinY == anchorMaxY tak funkcia nezmenena
    float m_bottom;       //posun aplikovany na anchorMaxY dovnutra, ak anchorMinY == anchorMaxY tak bottom sa stava height
    
    //float m_pivotOffsetX; //offsetPivotu 0,0 vlavo hore
    //float m_pivotOffsetY; //offsetPivotu 0,0 vlavo hore

    SDL_FRect m_rect;
    
    RectTransform* m_parent;
    std::vector<RectTransform*> m_children;

    std::vector<Behaviour*> m_behaviours;
    
public:
    
    RectTransform();                                    //= default;

    RectTransform(const RectTransform&)                 = delete;   //Copying disabled
    RectTransform& operator=(const RectTransform&)      = delete;        

    RectTransform(RectTransform&&) noexcept             = delete;   //Moving disabled
    RectTransform& operator=(RectTransform&&) noexcept  = delete;

    ~RectTransform()                                    = default;



    float get_anchorMinX() const;   
    //max rozsah <0;1>
    void set_anchorMinX(float value); 

    float get_anchorMinY() const;
    //max rozsah <0;1>  
    void set_anchorMinY(float value);  

    float get_anchorMaxX() const;
    //max rozsah <0;1>  
    void set_anchorMaxX(float value); 

    float get_anchorMaxY() const;
    //max rozsah <0;1>   
    void set_anchorMaxY(float value);  

    float get_left() const;         
    void set_left(float value);   
    
    float get_right() const;        
    void set_right(float value); 

    float get_top() const;         
    void set_top(float value);  

    float get_bottom() const; 
    void set_bottom(float value); 

    SDL_FRect get_rect() const;
    
    RectTransform* get_parent();
    void set_parent(RectTransform* parent);

    int get_childCount() noexcept;
    RectTransform* get_child(int idx);



    RectTransform* create_child();
    void add_child(RectTransform* child);
    void remove_child(RectTransform* child);

    void update_rect(SDL_FRect rect);

    void draw(SDL_Renderer* renderer) const;


   
    template<typename T, typename... Args>
    requires std::derived_from<T, Behaviour>
    T* add_behaviour(Args&&... args)
    {
        /*auto behaviour = std::make_unique<T>(this, std::forward<Args>(args)...);
        T* ptr = behaviour.get();
        m_behaviours.push_back(std::move(behaviour));
        return ptr;*/

        T* behaviour = new T(this, std::forward<Args>(args)...);
        m_behaviours.push_back(behaviour);
        return behaviour;
    }

    template<typename T>
    requires std::derived_from<T, Behaviour>
    void add_behaviour(T* behaviour)
    {
        m_behaviours.emplace_back(behaviour);
    }

    template<typename T>
    T* get_behaviour()
    {
        /*
        If you switched to:
        std::vector<std::unique_ptr<Behaviour>>
        Then your template must use:
        b.get()
        correct:
        if (auto casted = dynamic_cast<T*>(notCasted.get()))
        */

        for (const auto& notCasted : m_behaviours)
        {
            if (auto casted = dynamic_cast<T*>(notCasted))
            {
                return casted;
            }
        }

        return nullptr;
    }

};

//----------------------------------------------------------------------------------------------------------

class Scene
{
    Scene* m_activeScene;

public:

    std::vector<RectTransform*> content;

public:

    Scene();                            //= default;

    Scene(const Scene&)                 = delete;   //Copying disabled
    Scene& operator=(const Scene&)      = delete;

    Scene(Scene&&) noexcept             = delete;   //Moving disabled
    Scene& operator=(Scene&&) noexcept  = delete;

    ~Scene() = default;



    Scene* get_activeScene();
    void set_activeScene();
};

//----------------------------------------------------------------------------------------------------------

struct LuaRectTransform
{
    RectTransform* obj;
};



int LuaRectTransform_createInstance(lua_State* L);

//registrovanie do lua state
//lua_register(L, "LuaRectTransform_createInstance", LuaRectTransform_createInstance);

int LuaRectTransform_gc(lua_State* L);
// {
//    AAUserdata* ud = (AAUserdata*)luaL_checkudata(L, 1, "AA_meta");
//    delete ud->obj;
//    return 0;
//}

void register_LuaRectTransform(lua_State* L);
// {
//    luaL_newmetatable(L, "AA_meta");
//
//    lua_pushcfunction(L, aa_gc);
//    lua_setfield(L, -2, "__gc");
//
//    lua_pop(L, 1); // pop metatable
//}



int l_sin(lua_State* L);

//----------------------------------------------------------------------------------------------------------


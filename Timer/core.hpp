#pragma once

#include <vector>

#include "SDL3/SDL.h"


class RectTransform
{

private:

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
    
public:
    
    // -----------------------------
    // Constructors, Destructors
    // -----------------------------


    RectTransform();                                    //= default;

    RectTransform(const RectTransform&)                 = delete;   //Copying disabled
    RectTransform& operator=(const RectTransform&)      = delete;        

    RectTransform(RectTransform&&) noexcept             = default;  //Moving allowed
    RectTransform& operator=(RectTransform&&) noexcept  = default;   

    ~RectTransform()                                    = default;

    
    // -----------------------------
    // Getters
    // -----------------------------


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

    int get_child_count() noexcept;
    RectTransform* get_child(int idx);


    // -----------------------------
    // Methods
    // -----------------------------


    RectTransform* create_child();
    void add_child(RectTransform* child);
    void remove_child(RectTransform*& child);

    void update_rect(SDL_FRect rect);

    void draw(SDL_Renderer* renderer) const;

    template<typename T>
    T* get_behaviour();
};

//----------------------------------------------------------------------------------------------------------

class Time
{
    static float m_deltaTime;

public:

    static float get_deltaTime();
    static void set_deltaTime(float deltaTime);

};

//----------------------------------------------------------------------------------------------------------

class Behaviour
{
    RectTransform* m_transform;

public:

    Behaviour() = delete;
    Behaviour(RectTransform* transform);


    RectTransform* get_transform();


    virtual void update() = 0;
};

//----------------------------------------------------------------------------------------------------------

class Image : public Behaviour
{
    SDL_Texture* m_texture;

public:

    Image() = delete;
    //Image(RectTransform* transform, SDL_Surface* surface);
    Image(RectTransform* transform, SDL_Texture* texture);


    SDL_Texture* get_texture();


    void update() override;

    void update(SDL_Renderer* renderer);
};

//----------------------------------------------------------------------------------------------------------
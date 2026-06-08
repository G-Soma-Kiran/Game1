#include"CVector.h"
#include"CComponents.h"
#include<iostream>
#include<memory>
#include<SFML/Graphics.hpp>


CTransform::CTransform(){};
CTransform::CTransform(const Vector& p , const Vector& v , const float angle) : position(p) , velocity(v) , angle(angle){};


CShape::CShape(const float radius , const size_t points , const sf::Color& fillColor,const sf::Color& outlineColor , const float thickness  ): circle(radius , points)
    {
        circle.setFillColor(fillColor);
        circle.setOutlineThickness(thickness);
        circle.setOutlineColor(outlineColor);
        circle.setOrigin({radius , radius});
        
    };


CCollision::CCollision(){};
CCollision::CCollision(float radius):radius(radius){};

CScore::CScore(int score):score(score){};

CLifespan::CLifespan(int total ):total(total),remaining(total){};
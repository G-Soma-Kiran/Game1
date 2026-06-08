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


void CInput::updateInput(std::optional<sf::Event> opt)
{
    if(auto* p = opt->getIf<sf::Event::KeyPressed>())
    {
        if(p->code == sf::Keyboard::Key::Up){ up = true;}
        if(p->code == sf::Keyboard::Key::Down){ down = true;}
        if(p->code == sf::Keyboard::Key::Left){ left = true;}
        if(p->code == sf::Keyboard::Key::Right){ right = true;}
    }

    if( auto* p = opt->getIf<sf::Event::KeyReleased>() )
    {
        if(p->code == sf::Keyboard::Key::Up){ up = false;}
        if(p->code == sf::Keyboard::Key::Down){ down = false;}
        if(p->code == sf::Keyboard::Key::Left){ left = false;}
        if(p->code == sf::Keyboard::Key::Right){ right = false;}
    }
}

Vector CInput::getVelocityVector()
{
    int positiveYvalue = up? 1 : 0;
    int negativeYvalue = down? -1 : 0;
    int positiveXvalue = right? 1 : 0;
    int negativeXvalue = left? -1 : 0;
    return Vector(positiveXvalue + negativeXvalue , positiveYvalue + negativeYvalue );
}
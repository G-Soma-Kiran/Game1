#pragma once


#include"CVector.hpp"
#include<iostream>
#include<memory>
#include<SFML/Graphics.hpp>
class CTransform
{
    public:
        Vector position = {0 , 0};
        Vector velocity = {0 , 0};
        float angle = 0;
        
        CTransform();
        CTransform(const Vector& p , const Vector& v , const float angle);
};

class CShape
{
    public:
        sf::CircleShape circle;
        CShape(const float radius , const size_t points , const sf::Color& fillColor,const sf::Color& outlineColor , const float thickness);
};

class CCollision
{
    public:
        float radius = 0;
        CCollision();
        CCollision(float radius);
};

class CInput
{
    // private:
    //     Vector unitVelocity = { 0 , 0 };
    public:
        Vector getVelocityVector();
        void updateInput(std::optional<sf::Event> opt);
        bool up = false;
        bool down = false;
        bool left = false;
        bool right = false;

};

class CScore
{
    public:
        int score = 0;
        CScore(int score);
};

class CLifespan
{
    public:
        int remaining = 0;
        int total = 0;
        CLifespan(int total );
};
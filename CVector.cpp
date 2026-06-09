#include<SFML/Graphics.hpp>
#include<math.h>
#include<iostream>
#include"CVector.h"

        Vector::Vector(){};
        Vector::Vector(float x , float y) : x(x) , y(y){};
        bool Vector::operator == (const Vector& other) const{
            return (x == other.x && y == other.y);
        }

        bool Vector::operator != (const Vector& other) const{
            return (!(*this == other));
        }



        Vector Vector::operator + (const Vector& other) const{
            return Vector(x + other.x , y + other.y);
        }

        Vector Vector::operator - (const Vector& other) const{
            return Vector(x - other.x , y - other.y);
        }

        Vector Vector::operator * (const float value ) const{
            return Vector(x * value , y * value);
        }

        Vector Vector::operator / (const float value ) const{
            return Vector(x / value , y / value);
        }


        void Vector::operator += (const Vector& other){
            this->x+=other.x;
            this->y+=other.y;
        }

        void Vector::operator -= (const Vector& other){
            this->x-=other.x;
            this->y-=other.y;
        }

        void Vector::operator *= (const float value){
            this->x*=value;
            this->y*=value;
        }

        void Vector::operator /= (const float value){
            this->x/=value;
            this->y/=value;
        }




        float Vector::lengthSquared() const{
            return x*x + y*y;
        }

        float Vector::length() const{
            return sqrtf(this->lengthSquared());
        }

        float Vector::distanceSquared(const Vector& other) const{
            return ((other.x - x)*(other.x - x) + (other.y - y)*(other.y - y));
        }

        float Vector::distance(const Vector& other) const{
            return sqrtf(this->distanceSquared(other));
        }

        Vector Vector::normalize() const{
            float L = this->length();
            if(L == 0) return Vector(0 , 0);
            return Vector(x/L , y/L);
        }

        void Vector::print() const{   
            if(y>=0)
            std::cout << x << "i + " << y << "j" << std::endl;
            else
            std::cout << x << "i - " << -y << "j" << std::endl;
        }

        sf::Vector2f Vector::toVector2f() const
        {
            return {x , y};
        }
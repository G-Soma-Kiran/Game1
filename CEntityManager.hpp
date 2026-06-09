#pragma once


#include<iostream>
#include<memory>
#include"CComponents.hpp"
#include"CVector.hpp"
#include<vector>
#include<map>


class Entity
{
    private:
        friend class EntityManager;
        bool m_isALive = true;
        size_t m_id = 0 ;
        std::string m_tag = "Default";
        Entity();
        Entity(const size_t id , const std::string& tag);
    public:
        std::shared_ptr<CTransform> cTransform;
        std::shared_ptr<CShape> cShape;
        std::shared_ptr<CCollision> cCollision;
        std::shared_ptr<CInput> cInput;
        std::shared_ptr<CScore> cScore;
        std::shared_ptr<CLifespan> cLifespan;

        bool isAlive() const;
        const std::string& tag() const;
        void destroy();


};

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string , std::vector<std::shared_ptr<Entity>>> EntityMap;


class EntityManager
{
    private:
        std::vector<std::shared_ptr<Entity>> m_toAdd;
        std::vector<std::shared_ptr<Entity>> m_allEntities;
        std::map<std::string , std::vector<std::shared_ptr<Entity>>> m_entityGroup;
        size_t m_totalEntities = 0;   
    public:
        void update();
        std::shared_ptr<Entity> addEntity(const std::string& tag);
        const EntityVec& getEntities();
        const EntityVec& getEntities(const std::string& tag);
        size_t getid(std::shared_ptr<Entity> p);

};
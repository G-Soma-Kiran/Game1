#include<iostream>
#include<memory>
#include"CComponents.hpp"
#include"CVector.hpp"
#include"CEntityManager.hpp"
#include<vector>
#include<map>
#include<algorithm>


Entity::Entity(){};
Entity::Entity(const size_t id , const std::string& tag): m_id(id) , m_tag(tag){};

bool Entity::isAlive() const 
{
    return m_isALive;
};
const std::string& Entity::tag() const
{
    return m_tag;
};
void Entity::destroy()
{
    m_isALive = false;
};


// typedef std::vector<std::shared_ptr<Entity>> EntityVec;
// typedef std::map<std::string , std::vector<std::shared_ptr<Entity>>> EntityMap;



void EntityManager::update()
{
    for(auto p : m_toAdd)
    {
        m_allEntities.push_back(p);
        m_entityGroup[p->tag()].push_back(p);
    }
    m_toAdd.clear();

    for(auto& pair : m_entityGroup)
    {
        pair.second.erase( std::remove_if(pair.second.begin() , pair.second.end() , [](const auto p){ return !p->isAlive();} ), pair.second.end());
    }
    m_allEntities.erase( std::remove_if(m_allEntities.begin() , m_allEntities.end() , [](const auto p){ return !p->isAlive();} ), m_allEntities.end());
};


std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
    std::shared_ptr<Entity> p(new Entity(++m_totalEntities , tag));
    m_toAdd.push_back(p);
    return p;
};


const EntityVec& EntityManager::getEntities()
{
    return m_allEntities;
};


const EntityVec& EntityManager::getEntities(const std::string& tag)
{
    return m_entityGroup[tag];
};


size_t EntityManager::getid(std::shared_ptr<Entity> p)
{
    return p->m_id;
}

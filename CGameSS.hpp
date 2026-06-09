#pragma once

#include<iostream>
#include"CVector.hpp"
#include"CEntityManager.hpp"
#include"CComponents.hpp"
#include<fstream>
#include<SFML/Graphics.hpp>

typedef struct RGB{
    int R;
    int G;
    int B;
}RGB;

typedef struct configWindow{
    unsigned int width;
    unsigned int height;
    int frameRateUpperLimit;
    bool fullScreen;
}configWindow;

typedef struct configFont{
    std::string fontFilePath;
    int fontSize;
    RGB fontColor;
}configFont;

typedef struct configPlayer{
    float shapeRadius;
    int collisionRadius;
    float speed;
    RGB fillColor;
    RGB outlineColor;
    float outlineThickness;
    size_t vertices;
    float rotationAngle;
}configPlayer;


typedef struct configEnemy{
    int shapeRadius;
    int collisionRadius;
    float minSpeed;
    float maxSpeed;
    RGB outlineColor;
    float outlineThickness;
    size_t minVertices;
    size_t maxVertices;
    int brokenPartLifeSpan;
    int spawnInterval;
    float rotationAngle;
}configEnemy;


typedef struct configBullet{
    int shapeRadius;
    int collisionRadius;
    float speed;
    RGB fillColor;
    RGB outlineColor;
    int outlineThickness;
    size_t shapeVertices;
    int lifespan;
    float rotationAngle;
}configBullet;


class Game
{
    private:       
        sf::RenderWindow m_window;
        EntityManager m_entities;
        std::shared_ptr<Entity> m_player;
        sf::Font m_font;
        std::optional<sf::Text> m_text;
        configWindow m_configWindow;
        configFont m_configFont;
        configPlayer m_configPlayer;
        configEnemy m_configEnemy;
        configBullet m_configBullet;
        int m_score = 0;
        int m_spawner = 1;
        bool m_isPaused = false;
        bool m_isRunning = true;

        void init(const std::string& configFilePath);
        void changePause(bool value);

        void sMovement();
        void sUserInputTakingAndHandling();
        void sKiller();
        void sRender();
        void sEnemySpawner();
        void sCollision();

        float randomInRange(float min , float max);
        Vector randomPositionVector();
        Vector randomVelocityVector(float velocityAngle);
        std::shared_ptr<Entity> spawnPlayer();
        void spawnEnemy();
        void spawnBullet(const Vector mousePosition);
        void spawnBrokenParts(std::shared_ptr<Entity> entity);
        void specialWeapon();
        Vector speedToVelocity(const Vector direction , const float speed );
        Vector speedToVelocity(const float angle , const float speed );
        Vector clampPlayerPosition();
    public:
        Game(const std::string& configFilePath);
        void run();
};
#include<iostream>
#include"CVector.hpp"
#include"CEntityManager.hpp"
#include"CComponents.hpp"
#include<fstream>
#include<SFML/Graphics.hpp>
#include"CGameSS.hpp"
#include<memory>
#include<cstdlib>
#include<ctime>
#include<cmath>
// class Game
// {
//     private:       
//         sf::RenderWindow m_window;
//         EntityManager m_entities;
//         sf::Font m_font;
//         sf::Text m_text;
//         configWindow m_configWindow;
//         configFont m_configFont;
//         configPlayer m_configPlayer;
//         configEnemy m_configEnemy;
//         configBullet m_configBullet;
//         int m_score = 0;
//         int m_spawner = 1;
//         bool m_isPaused = false;
//         bool m_isRunning = true;

//         void init(std::string& configFilePath);
//         void changePause(bool value);

//         void sMovement();
//         void sUserMovement();
//         void sKiller();
//         void sRender();
//         void sEnemySpawner();
//         void sCollision();

//         void spawnPlayer();
//         void spawnEnemy();
//         void spawnBullet(const Vector mousePosition);
//         void spawnBrokenParts(std::shared_ptr<Entity> entity);
//         void specialWeapon();
//     public:
//         Game(const std::string& configFilePath);
//         void run();
// };

Game::Game(const std::string& configFilePath) //:  m_text(m_font)
{
    init(configFilePath);
}
void Game::init(const std::string& configFilePath)
{
    std::srand(std::time(0));


    std::ifstream file(configFilePath);
    std::string temp;
    if(file >> temp && temp == "Window")
    file >>m_configWindow.width >> m_configWindow.height >> m_configWindow.frameRateUpperLimit >> m_configWindow.fullScreen ;
    if(file >> temp && temp == "Font")
    file >> m_configFont.fontFilePath >> m_configFont.fontSize >> m_configFont.fontColor.R >> m_configFont.fontColor.G >> m_configFont.fontColor.B ;
    if(file >> temp && temp == "Player")
    file >> m_configPlayer.shapeRadius >> m_configPlayer.collisionRadius >> m_configPlayer.speed >> m_configPlayer.fillColor.R >> m_configPlayer.fillColor.G >> m_configPlayer.fillColor.B >> m_configPlayer.outlineColor.R >> m_configPlayer.outlineColor.G >> m_configPlayer.outlineColor.B >> m_configPlayer.outlineThickness >> m_configPlayer.vertices >> m_configPlayer.rotationAngle ;
    if(file >> temp && temp == "Enemy")
    file >> m_configEnemy.shapeRadius >> m_configEnemy.collisionRadius >> m_configEnemy.minSpeed >> m_configEnemy.maxSpeed >> m_configEnemy.outlineColor.R >> m_configEnemy.outlineColor.G >> m_configEnemy.outlineColor.B >> m_configEnemy.outlineThickness >> m_configEnemy.minVertices >> m_configEnemy.maxVertices >> m_configEnemy.brokenPartLifeSpan >> m_configEnemy.spawnInterval >> m_configEnemy.rotationAngle ;
    if(file >> temp && temp == "Bullet")
    file >> m_configBullet.shapeRadius >> m_configBullet.collisionRadius >> m_configBullet.speed >> m_configBullet.fillColor.R >> m_configBullet.fillColor.G >> m_configBullet.fillColor.B >> m_configBullet.outlineColor.R >> m_configBullet.outlineColor.G >> m_configBullet.outlineColor.B >> m_configBullet.outlineThickness >> m_configBullet.shapeVertices >> m_configBullet.lifespan >> m_configBullet.rotationAngle ;  

    if(!m_font.openFromFile(m_configFont.fontFilePath))
    {
        std::cerr << "Font Problem" << std::endl;
    }
    m_text.emplace(m_font , "Score : 0" , m_configFont.fontSize);
    m_text->setFillColor(sf::Color(m_configFont.fontColor.R, m_configFont.fontColor.G, m_configFont.fontColor.B));
    m_text->setPosition({10.0f , 10.0f}); //Random Positiion to start with.

    if(m_configWindow.fullScreen)
    {
        m_window.create(sf::VideoMode({m_configWindow.width , m_configWindow.height}) , "Geometry Wars!!!" ,sf::Style::Default ,  sf::State::Fullscreen) ;
    }
    else
    {
        m_window.create(sf::VideoMode({m_configWindow.width , m_configWindow.height}) , "Geometry Wars!!!" ,sf::Style::Default ,  sf::State::Windowed) ;
    }
    m_window.setFramerateLimit(m_configWindow.frameRateUpperLimit); 


    m_player = spawnPlayer();

}

void Game::changePause(bool value)
{
    m_isPaused = value;
}

std::shared_ptr<Entity> Game::spawnPlayer()
{
    auto p = m_entities.addEntity("Player");
    p->cInput = std::make_shared<CInput>();
    p->cCollision = std::make_shared<CCollision>(m_configPlayer.collisionRadius);
    p->cShape = std::make_shared<CShape>(m_configPlayer.shapeRadius , m_configPlayer.vertices , sf::Color(m_configPlayer.fillColor.R ,m_configPlayer.fillColor.G , m_configPlayer.fillColor.B ) , sf::Color(m_configPlayer.outlineColor.R , m_configPlayer.outlineColor.G , m_configPlayer.outlineColor.B) , m_configPlayer.outlineThickness);
    p->cTransform = std::make_shared<CTransform>(Vector(m_window.getSize().x/2 , m_window.getSize().y/2) , Vector(0 , 0) , m_configPlayer.rotationAngle);
    return p;
}

float Game::randomInRange(float min , float max)
{   
    // std::srand(std::time(0));
    float r = min + static_cast<float>(std::rand()) / RAND_MAX * (max - min);
    return r;
}

Vector Game::randomPositionVector()
{
    return Vector(
        randomInRange(m_configEnemy.shapeRadius , m_window.getSize().x - m_configEnemy.shapeRadius ) 
        , 
        randomInRange( m_configEnemy.shapeRadius , m_window.getSize().y - m_configEnemy.shapeRadius )
    );    
}

Vector Game::speedToVelocity(const float angle , const float speed)
{
    return Vector ( speed * std::cosf(angle*0.0174533) , -speed * std::sinf(angle*0.0174533));
}

Vector Game::speedToVelocity(const Vector direction , const float speed)
{   
    return   direction.normalize() * speed ;
}

Vector Game::randomVelocityVector(float velocityAngle)
{
    float speed  = randomInRange(m_configEnemy.minSpeed , m_configEnemy.maxSpeed);
    // return Vector(  speed * std::cosf(velocityAngle) , speed * std::sinf(velocityAngle));
    return speedToVelocity(velocityAngle , speed);
}
void Game::spawnEnemy()
{
    auto p = m_entities.addEntity("Enemy");
    float velocityAngle = randomInRange(0 , 359);
    size_t vertices = randomInRange(m_configEnemy.minVertices , m_configEnemy.maxVertices);
    p->cTransform = std::make_shared<CTransform>(randomPositionVector() , randomVelocityVector(velocityAngle) , m_configEnemy.rotationAngle);
    p->cShape = std::make_shared<CShape>(m_configEnemy.shapeRadius , vertices , sf::Color(randomInRange(100 , 255) , randomInRange(100 , 255) , randomInRange(100 , 255)) , sf::Color(m_configEnemy.outlineColor.R , m_configEnemy.outlineColor.G , m_configEnemy.outlineColor.B ) , m_configEnemy.outlineThickness);
    p->cScore = std::make_shared<CScore>( vertices * 100);
    p->cCollision = std::make_shared<CCollision>(m_configEnemy.collisionRadius);

}

void Game::spawnBullet(const Vector mousePosition)
{
    Vector playerPosition = m_player->cTransform->position;
    auto p =  m_entities.addEntity("Bullet");
    p->cShape = std::make_shared<CShape>(m_configBullet.shapeRadius , m_configBullet.shapeVertices , sf::Color(m_configBullet.fillColor.R , m_configBullet.fillColor.G ,m_configBullet.fillColor.B) , sf::Color(m_configBullet.outlineColor.R , m_configBullet.outlineColor.G , m_configBullet.outlineColor.B) , m_configBullet.outlineThickness);
    p->cCollision = std::make_shared<CCollision>(m_configBullet.collisionRadius);
    p->cTransform = std::make_shared<CTransform>(playerPosition , speedToVelocity(mousePosition - playerPosition , m_configBullet.speed) , m_configBullet.rotationAngle);
    p->cLifespan = std::make_shared<CLifespan>(m_configBullet.lifespan);

}
void Game::sRender()
{
    // m_window.clear(sf::Color::Black);
    const EntityVec& allEntities = m_entities.getEntities();
    for ( auto e : allEntities)
    {
        e->cShape->circle.setPosition(e->cTransform->position.toVector2f());
        e->cShape->circle.rotate(sf::degrees(e->cTransform->angle));
        m_window.draw(e->cShape->circle);
    }
    m_text->setString("Score: " + std::to_string(m_score));
    m_window.draw(*m_text);
    // m_window.display();
}

void Game::sMovement()
{
    const EntityVec& allEntities = m_entities.getEntities();
    for ( auto e : allEntities)
    {
        e->cTransform->position+=e->cTransform->velocity;
        if(e->tag() == "Player")
        {
            e->cTransform->position = clampPlayerPosition();
        }
        // e->cShape->circle.rotate(sf::degrees(e->cTransform->angle));   
    }
}

void Game::sUserInputTakingAndHandling()
{
    while(auto opt = m_window.pollEvent())
    {
        if (opt->is<sf::Event::Closed>()) {m_window.close();}

        m_player->cInput->updateInput(opt);
        
        if(auto* mouse = opt->getIf<sf::Event::MouseButtonPressed>())
        {
            if(!m_isPaused) {spawnBullet({mouse->position.x + 0.0f , mouse->position.y + 0.0f});}
        }
        if(auto* p = opt->getIf<sf::Event::KeyPressed>())
        {
            if(p->code == sf::Keyboard::Key::P)
            {
                changePause(!m_isPaused);
                // m_isPaused = !m_isPaused;
            }
            if(p->code == sf::Keyboard::Key::Escape)
            {
                m_window.close();
            }
        }
    }
    // m_player->cTransform->velocity = m_player->cInput->getVelocityVector();
    m_player->cTransform->velocity = speedToVelocity(m_player->cInput->getVelocityVector() , m_configPlayer.speed);
}

void Game::sEnemySpawner()
{
    if(m_spawner == 0)
    {
        spawnEnemy();
        m_spawner = m_configEnemy.spawnInterval;
    }
    else
    {
        m_spawner--;
    }
}

void Game::sKiller()
{
    const EntityVec& allEntities = m_entities.getEntities();
    for( auto e : allEntities)
    {
        if(e->cLifespan)
        {
            if(e->cLifespan->remaining == 0)
            {
                e->destroy();
            }
            else
            {
                e->cLifespan->remaining-=1;
                sf::Color eFillColor = e->cShape->circle.getFillColor();
                sf::Color eOutlineColor = e->cShape->circle.getOutlineColor();
                // e->cShape->circle.setFillColor(sf::Color(eFillColor.r , eFillColor.g , eFillColor.b , eFillColor.a - (255/(e->cLifespan->total)) ));
                // e->cShape->circle.setOutlineColor(sf::Color(eOutlineColor.r , eOutlineColor.g , eOutlineColor.b , eOutlineColor.a - (255/(e->cLifespan->total)) ));
                e->cShape->circle.setFillColor(sf::Color(eFillColor.r , eFillColor.g , eFillColor.b ,e->cLifespan->remaining < 0 ? 255 : 255 * ((e->cLifespan->remaining * 1.0f )/ e->cLifespan->total)));
                e->cShape->circle.setOutlineColor(sf::Color(eOutlineColor.r , eOutlineColor.g , eOutlineColor.b , e->cLifespan->remaining < 0 ? 255 : 255 * ((e->cLifespan->remaining * 1.0f )/ e->cLifespan->total) ));
            }
        }
    }
}
void Game::sCollision()
{
    const EntityVec& allEntities = m_entities.getEntities();
    const EntityVec& allBullets = m_entities.getEntities("Bullet");
    for( auto e : allEntities)
    {
        if(e->cCollision)
        {
            Vector ePos = e->cTransform->position;
            if(ePos.x >= m_window.getSize().x - e->cCollision->radius || ePos.x <= e->cCollision->radius)
            {
                e->cTransform->velocity.x = -(e->cTransform->velocity.x);
            }
            if(ePos.y >= m_window.getSize().y - e->cCollision->radius || ePos.y <= e->cCollision->radius)
            {
                e->cTransform->velocity.y = -(e->cTransform->velocity.y);
            }

            if( (e->tag() == "Enemy" && (m_player->cCollision->radius + e->cCollision->radius)*(m_player->cCollision->radius + e->cCollision->radius) > Vector(ePos - m_player->cTransform->position).lengthSquared()))
            {
                if(e->isAlive())
                {
                    e->destroy();
                    spawnBrokenParts(e);
                    m_score += e->cScore->score;
                }
                m_player->cTransform->position = Vector(m_window.getSize().x/2 , m_window.getSize().y/2);
            }

            if(e->tag() == "Enemy")
            { 
                for( auto eB : allBullets)
                {
                    if( ( eB->cCollision->radius + e->cCollision->radius ) * ( eB->cCollision->radius + e->cCollision->radius ) > Vector(eB->cTransform->position - e->cTransform->position ).lengthSquared() )
                    {
                        if(e->isAlive())
                        {
                            e->destroy();
                            spawnBrokenParts(e);
                            m_score += e->cScore->score;
                            break;
                        }
                    }
                }
            }


        }
    }
}

void Game::spawnBrokenParts(std::shared_ptr<Entity> entity)
{
    size_t vertices = entity->cShape->circle.getPointCount();
    size_t loopIterations = vertices;
    size_t increment = 360/vertices;
    for (size_t i=0; i<360 && loopIterations > 0; i+=increment , loopIterations--)
    {
        auto p = m_entities.addEntity("BrokenPart");
        float velocityAngle = i;
        p->cTransform = std::make_shared<CTransform>(entity->cTransform->position , speedToVelocity(i , entity->cTransform->velocity.length()) , entity->cTransform->angle);
        p->cShape = std::make_shared<CShape>(entity->cShape->circle.getRadius()/2 , entity->cShape->circle.getPointCount() , entity->cShape->circle.getFillColor() , entity->cShape->circle.getOutlineColor() , entity->cShape->circle.getOutlineThickness() );
        p->cCollision = std::make_shared<CCollision>(entity->cCollision->radius/2);
        p->cLifespan = std::make_shared<CLifespan>(m_configEnemy.brokenPartLifeSpan);
    }
}

void Game::run()
{
    while(m_window.isOpen())
    {

        
        sUserInputTakingAndHandling(); //As Input Handling is also done in this function the pausing is integrated inside the function
        if(!m_window.isOpen()) break;  //Efficiency thingy
        if(!m_isPaused)sEnemySpawner();
        
        m_entities.update();

        if(!m_isPaused)sMovement();
        if(!m_isPaused)sCollision();
        if(!m_isPaused)sKiller();
        
        m_window.clear(sf::Color::Black);
        sRender();
        m_window.display();
    }
}

Vector Game::clampPlayerPosition()
{
    Vector ultimate = m_player->cTransform->position;
    if(m_player->cTransform->velocity.x > 0)
    {    
        ultimate.x = std::min(ultimate.x , m_window.getSize().x - m_player->cCollision->radius );      
    }
    if(m_player->cTransform->velocity.x < 0)
    {
        ultimate.x = std::max(ultimate.x , m_player->cCollision->radius);
    }

    if(m_player->cTransform->velocity.y > 0)
    {    
        ultimate.y = std::min(ultimate.y , m_window.getSize().y - m_player->cCollision->radius );      
    }
    if(m_player->cTransform->velocity.y < 0)
    {
        ultimate.y = std::max(ultimate.y , m_player->cCollision->radius);
    }
    return ultimate;
    
}
int main()
{

    Game test("config.txt");
    test.run();

}
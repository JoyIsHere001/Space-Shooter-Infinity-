#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "SpaceShooter.h"
#include "Bullet.h"
#include "HUD.h"
#include "Enemy.h"
#include "Collision.h"
#include <vector>

using namespace sf;
using namespace std;


//class that act as game engine (main game logic).

class Game
{
private:
    // variables 
    enum GameState {
        INTRO,
        MAIN_MENU,
        GAME,
        PAUSED,
        GAME_OVER,
    };

    //window
    RenderWindow* window;
    Event ev;
    bool isFullscreen;

    // Background Image Members:
    Texture backGroundTex;
    Sprite backgroundsprite;
    float backgroundScrollSpeed;
    float backgroundOffset;

    // spaceShooter as player
    SpaceShooter* player;

    // Enemy ships 
    vector<Enemy> enemies;
    Texture enemyTexture;

    // Spawning
    float enemySpawnTimer = 0.f;
    float enemySpawnInterval = 2.f;
    float gameTime = 0.f;
    unsigned int maxEnemies = 3;
    float maxEnemiesFloat = 3.f;

    // Explosion Handling 
    Texture explosionTexture;
    vector<Collision> explosions;

    // Sound Effects 
    SoundBuffer enemyExplosionBuffer;
    SoundBuffer playerExplosionBuffer;
    Sound enemyExplosionSound;
    Sound playerExplosionSound;

    // Player Health 
    float playerHealth = 3.0f; // 3.0 = 3 hits

    // movement time
    sf::Clock dtClock; // SFML clock to measure time between frames
    float deltaTime;   // the delta time value

    // Highest Score 
    int highScore;

    // private fucntions
    void initvariables();
    void initwindow();
    void initbackground();
    void initplayer();
    void initTexts();
    void initAudio();
    void initEnemy();
    void initExplosion();

    //Menu design 
    GameState gamestate;
    Font font;
    Text titleText;
    Text pressKeyText;
    Text gameOvertext;
    RectangleShape replayButton;
    Text replayText;

    /*Main menu Text */
    Text startText;
    Text quitText;
    float titlePulseTimer;
    float blinkTimer;

    // Pause Menu
    sf::RectangleShape pauseOverlay;

    sf::Text pauseText;
    sf::Text resumeText;
    sf::Text exitText;

    bool resumeHovered = false;
    bool exitHovered = false;

    // HUD 
    HUD hud;

    //audio variables 

    Music bgMusic;
    Music gameAmbience;

    SoundBuffer shootBuffer;
    SoundBuffer thrusterBuffer;
    SoundBuffer hoverBuffer;
    SoundBuffer clickBuffer;

    Sound shootSound;
    Sound thrusterSound;
    Sound hoverSound;
    Sound clickSound;

    bool thrusterPlaying = false;
    bool startHovered = false;
    bool quitHovered = false;

public:
    // constructors and destructors 
    Game();
    virtual ~Game();

    // accessors
    const bool running() const;

    // functions
    void resetGame();
    void pollEvents();
    void update();
    void render();
};
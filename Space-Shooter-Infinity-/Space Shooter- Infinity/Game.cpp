#include "Game.h"
#include <iostream>
#include <cmath>

using namespace std;
using namespace sf;

//initializations
void Game::initvariables()
{
    this->window = nullptr;
    this->gamestate = GameState::INTRO;
    this->titlePulseTimer = 0.f;
    this->blinkTimer = 0.f;
    this->backgroundScrollSpeed = 100.f;
    this->backgroundOffset = 0.f;
    this->deltaTime = 0.f;
    this->highScore = 0;

    // spawn defaults
    this->enemySpawnTimer = 0.f;
    this->enemySpawnInterval = 2.f;
    this->gameTime = 0.f;
    this->maxEnemies = 3;
    this->playerHealth = 3.0f;
    this->maxEnemiesFloat = 3.f;
  
}

void Game::initwindow()
{
    VideoMode desktop = VideoMode::getDesktopMode();
    this->window = new RenderWindow(desktop, "Space Shooter", Style:: Fullscreen );
    this->window->setFramerateLimit(60);
    this->isFullscreen = true; // Initial state is fullscreen
}

void Game::initbackground()
{
    if (!this->backGroundTex.loadFromFile("assets/Art_assets/spaceBackground.png"))
        cout << "ERROR: Could not load background texture!\n";

    this->backGroundTex.setSmooth(true);
    this->backGroundTex.setRepeated(true);

    this->backgroundsprite.setTexture(this->backGroundTex);

    const Vector2u texSize = this->backGroundTex.getSize();
    const Vector2u winSize = this->window->getSize();

    const float scaleX = static_cast<float>(winSize.x) / static_cast<float>(texSize.x);
    const float scaleY = static_cast<float>(winSize.y) / static_cast<float>(texSize.y);

    this->backgroundsprite.setScale(scaleX, scaleY);
}

void Game::initplayer()
{
    this->player = new SpaceShooter();
}

void Game::initEnemy()
{
    if (!enemyTexture.loadFromFile("assets/Art_assets/enemy.png"))
    {
        std::cerr << "Failed to load enemy texture\n";
    }
}

void Game::initExplosion()
{
    if (!explosionTexture.loadFromFile("assets/Art_assets/explosion.png"))
        std::cerr << "Failed to load explosion texture\n";
}

void Game::initTexts()
{
    if (!this->font.loadFromFile("assets/Fonts/SpaceNova.ttf"))
        cout << "ERROR: Could not load font!\n";

    // --- INTRO TEXT ---
    this->titleText.setFont(this->font);
    this->titleText.setString("SPACE SHOOTER: INFINITY ");
    this->titleText.setCharacterSize(60);
    this->titleText.setFillColor(Color::Cyan);
    this->titleText.setStyle(Text::Bold);
    this->titleText.setPosition(600.f, 500.f);

    this->pressKeyText.setFont(this->font);
    this->pressKeyText.setString("Press any key to continue...");
    this->pressKeyText.setCharacterSize(30);
    this->pressKeyText.setFillColor(Color::White);
    this->pressKeyText.setPosition(600.f, 600.f);

    // --- MAIN MENU TEXTS ---
    this->startText.setFont(this->font);
    this->startText.setString("START GAME");
    this->startText.setCharacterSize(40);
    this->startText.setPosition(750.f, 550.f);

    this->quitText.setFont(this->font);
    this->quitText.setString("QUIT");
    this->quitText.setCharacterSize(40);
    this->quitText.setPosition(750.f, 650.f);

    // --- GAME OVER UI ---
    gameOvertext.setFont(this->font);
    gameOvertext.setString("GAME OVER");
    gameOvertext.setCharacterSize(72);
    gameOvertext.setFillColor(sf::Color::Red);
    gameOvertext.setStyle(sf::Text::Bold);
    gameOvertext.setPosition(this->window->getSize().x / 2.f - gameOvertext.getGlobalBounds().width / 2.f,
        this->window->getSize().y / 3.f);

    replayButton.setSize(sf::Vector2f(200.f, 60.f));
    replayButton.setFillColor(sf::Color(100, 100, 255));
    replayButton.setPosition(this->window->getSize().x / 2.f - 100.f, this->window->getSize().y / 1.8f);

    replayText.setFont(this->font);
    replayText.setString("PLAY AGAIN");
    replayText.setCharacterSize(23);
    replayText.setFillColor(sf::Color::White);
    replayText.setPosition(replayButton.getPosition().x + 15.f, replayButton.getPosition().y + 10.f);

    // --- PAUSE MENU ---

    pauseOverlay.setSize(sf::Vector2f(
        static_cast<float>(window->getSize().x),
        static_cast<float>(window->getSize().y)
    ));

    pauseOverlay.setFillColor(sf::Color(0, 0, 0, 180));

    pauseText.setFont(this->font);
    pauseText.setString("GAME PAUSED");
    pauseText.setCharacterSize(70);
    pauseText.setFillColor(sf::Color::Yellow);
    pauseText.setStyle(sf::Text::Bold);
    pauseText.setPosition(
        window->getSize().x / 2.f - 350.f,
        250.f
    );

    resumeText.setFont(this->font);
    resumeText.setString("RESUME");
    resumeText.setCharacterSize(45);
    resumeText.setFillColor(sf::Color::White);
    resumeText.setPosition(
        window->getSize().x / 2.f - 200.f,
        450.f
    );

    exitText.setFont(this->font);
    exitText.setString("EXIT GAME");
    exitText.setCharacterSize(45);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(
        window->getSize().x / 2.f - 200.f,
        550.f
    );
}

void Game::initAudio()
{
    auto safeLoadMusic = [&](sf::Music& music, const std::string& path, const std::string& name)
        {
            if (!music.openFromFile(path))
            {
                std::cout << "ERROR: Could not load " << name << " (" << path << ")" << std::endl;
                return false;
            }
            music.setLoop(true);
            music.setVolume(60.f);
            return true;
        };

    bool bgOk = safeLoadMusic(this->bgMusic,
        "assets/Audio/BackgroundAudio/Spaceshooter Title track .ogg",
        "title/menu background music");

    bool gameOk = safeLoadMusic(this->gameAmbience,
        "assets/Audio/BackgroundAudio/In game Ambience.ogg",
        "game ambience");

    if (!this->shootBuffer.loadFromFile("assets/Audio/Sounds/shoot.wav"))
        std::cout << "ERROR: Could not load shoot sound effect!\n";

    if (!this->thrusterBuffer.loadFromFile("assets/Audio/Sounds/thruster.wav"))
        std::cout << "ERROR: Could not load thruster sound effect!\n";

    this->shootSound.setBuffer(this->shootBuffer);
    this->shootSound.setVolume(60.f);

    this->thrusterSound.setBuffer(this->thrusterBuffer);
    this->thrusterSound.setLoop(true);
    this->thrusterSound.setVolume(50.f);
    this->thrusterPlaying = false;

    if (bgOk)
        this->bgMusic.play();

    if (!this->hoverBuffer.loadFromFile("assets/Audio/Sounds/hover.wav"))
        std::cout << "ERROR: Could not load hover sound!" << endl;

    if (!this->clickBuffer.loadFromFile("assets/Audio/Sounds/clikc button .wav"))
        std::cout << "ERROR: Could not load click sound!\n";

    this->hoverSound.setBuffer(this->hoverBuffer);
    this->hoverSound.setVolume(70.f);

    this->clickSound.setBuffer(this->clickBuffer);
    this->clickSound.setVolume(80.f);

    // Moved from constructor
    if (!enemyExplosionBuffer.loadFromFile("assets/Audio/Sounds/enemyexplosion.wav"))
        cout << "Failed to load enemy explosion sound\n";
    enemyExplosionSound.setBuffer(enemyExplosionBuffer);

    if (!playerExplosionBuffer.loadFromFile("assets/Audio/Sounds/playerexplosion.wav"))
        cout << "Failed to load player explosion sound\n";
    playerExplosionSound.setBuffer(playerExplosionBuffer);
}

// Constructors / Destructors

Game::Game()
{
    this->initvariables();
    this->initwindow();
    this->initbackground();
    this->initplayer();
    this->initTexts();
    this->initAudio();
    this->initEnemy();
    this->initExplosion();
}

Game::~Game()
{
    delete this->window;
    delete this->player;
}


// Accessors

const bool Game::running() const
{
    return this->window->isOpen();
}

// reseting Game

void Game::resetGame()
{
    // UPDATE HIGH SCORE BEFORE RESETTING SCORE

    if (hud.score > highScore)
    {
        highScore = hud.score;
    }

    // Reset player health
    playerHealth = 3.0f;

    // Reset HUD
    hud.score = 0;
    hud.highScore = highScore;
    hud.healthPercent = 1.0f;
    hud.updateTexts();

    // Clear vectors and timers
    enemies.clear();
    explosions.clear();
    this->player->getBullets().clear();

    gameTime = 0.f;
    enemySpawnTimer = 0.f;
    enemySpawnInterval = 2.f;
    maxEnemies = 3;
    maxEnemiesFloat = 3.f;

    // Reposition player
    sf::Vector2u win = this->window->getSize();

    sf::Vector2f pos(
        win.x / 2.f - this->player->getSprite().getGlobalBounds().width / 2.f,
        win.y - this->player->getSprite().getGlobalBounds().height - 20.f
    );

    player->setPosition(pos);

    gamestate = GameState::GAME;
}


// Event Handling

void Game::pollEvents()
{
    while (this->window->pollEvent(this->ev))
    {
        if (this->ev.type == Event::Closed)
            this->window->close();

        if (this->ev.type == Event::KeyPressed)
        {
            if (this->ev.key.code == Keyboard::Escape)
            {
                if (gamestate == GameState::GAME)
                {
                    gamestate = GameState::PAUSED;
                }
                else if (gamestate == GameState::PAUSED)
                {
                    gamestate = GameState::GAME;
                }
                else
                {
                    window->close();
                }
            }

            if (this->gamestate == GameState::INTRO)
                this->gamestate = GameState::MAIN_MENU;

            // toggle fullscreen (F11)
            if (this->ev.key.code == Keyboard::F11)
            {
                // Delete old window to prevent memory leak
                delete this->window;

                if (this->isFullscreen)
                {
                    // create windowed
                    this->window = new RenderWindow(VideoMode(1280, 720), "Space Shooter", Style::Titlebar | Style::Close);
                    this->isFullscreen = false; // Set new state
                }
                else
                {
                    //  create fullscreen
                    this->window = new RenderWindow(VideoMode::getDesktopMode(), "Space Shooter", Style::Fullscreen);
                    this->isFullscreen = true; // Set new state
                }
                this->window->setFramerateLimit(60);
            }
        }

        if (this->ev.type == Event::MouseButtonPressed && this->ev.mouseButton.button == Mouse::Left)
        {
            if (this->gamestate == GameState::MAIN_MENU)
            {
                Vector2f mousePos = (Vector2f)Mouse::getPosition(*this->window);

                if (this->startText.getGlobalBounds().contains(mousePos))
                {
                    this->clickSound.play();
                    this->gamestate = GameState::GAME;
                }
                else if (this->quitText.getGlobalBounds().contains(mousePos))
                {
                    this->clickSound.play();
                    this->window->close();
                }
            }

            else if (this->gamestate == GameState::GAME_OVER)
            {
                sf::Vector2f mousePos = (Vector2f)Mouse::getPosition(*window);
                if (replayButton.getGlobalBounds().contains(mousePos)) {
                    clickSound.play();
                    resetGame();
                }
            }

            else if (this->gamestate == GameState::PAUSED)
            {
                sf::Vector2f mousePos =
                    (sf::Vector2f)Mouse::getPosition(*window);

                if (resumeText.getGlobalBounds().contains(mousePos))
                {
                    clickSound.play();
                    gamestate = GameState::GAME;
                }

                else if (exitText.getGlobalBounds().contains(mousePos))
                {
                    clickSound.play();

                    // reset game
                    hud.score = 0;
                    playerHealth = 1.0f;

                    enemies.clear();
                    explosions.clear();
                    player->getBullets().clear();

                    gamestate = GameState::MAIN_MENU;
                }
            }

        }
    }
}

// Update

void Game::update()
{
    // Delta time
    this->deltaTime = this->dtClock.restart().asSeconds();

    // Scroll background
    this->backgroundOffset += this->backgroundScrollSpeed * this->deltaTime;
    if (this->backgroundOffset >= static_cast<float>(this->backGroundTex.getSize().y))
        this->backgroundOffset = 0.f;

    // Poll events
    this->pollEvents();

    // State handling
    if (this->gamestate == GameState::INTRO)
    {
        // Title pulse
        this->titlePulseTimer += this->deltaTime * 2.f;
        float scale = 1.f + 0.05f * sin(this->titlePulseTimer);
        this->titleText.setScale(scale, scale);

        // Blink press key
        this->blinkTimer += this->deltaTime;
        if (fmod(this->blinkTimer, 1.f) < 0.5f)
            this->pressKeyText.setFillColor(Color(255, 255, 255, 60));
        else
            this->pressKeyText.setFillColor(Color::White);
    }
    else if (this->gamestate == GameState::MAIN_MENU)
    {
        Vector2f mousePos = (Vector2f)Mouse::getPosition(*this->window);

        bool startNowHovered = this->startText.getGlobalBounds().contains(mousePos);
        bool quitNowHovered = this->quitText.getGlobalBounds().contains(mousePos);

        // START hover
        if (startNowHovered)
        {
            if (!this->startHovered) { this->hoverSound.play(); this->startHovered = true; }
            this->startText.setFillColor(Color(70, 146, 171));
            this->startText.setScale(1.1f, 1.1f);
        }
        else
        {
            this->startHovered = false;
            this->startText.setFillColor(Color::White);
            this->startText.setScale(1.f, 1.f);
        }

        // QUIT hover
        if (quitNowHovered)
        {
            if (!this->quitHovered) { this->hoverSound.play(); this->quitHovered = true; }
            this->quitText.setFillColor(Color(70, 146, 171));
            this->quitText.setScale(1.1f, 1.1f);
        }
        else
        {
            this->quitHovered = false;
            this->quitText.setFillColor(Color::White);
            this->quitText.setScale(1.f, 1.f);
        }
    }

    //pause menu hover 
    else if (this->gamestate == GameState::PAUSED)
    {
        Vector2f mousePos =
            (Vector2f)Mouse::getPosition(*this->window);

        bool resumeNowHovered =
            this->resumeText.getGlobalBounds().contains(mousePos);

        bool exitNowHovered =
            this->exitText.getGlobalBounds().contains(mousePos);

        // RESUME hover
        if (resumeNowHovered)
        {
            if (!resumeHovered)
            {
                hoverSound.play();
                resumeHovered = true;
            }

            resumeText.setFillColor(Color(70, 146, 171));
            resumeText.setScale(1.1f, 1.1f);
        }
        else
        {
            resumeHovered = false;
            resumeText.setFillColor(Color::White);
            resumeText.setScale(1.f, 1.f);
        }

        // EXIT hover
        if (exitNowHovered)
        {
            if (!exitHovered)
            {
                hoverSound.play();
                exitHovered = true;
            }

            exitText.setFillColor(Color(70, 146, 171));
            exitText.setScale(1.1f, 1.1f);
        }
        else
        {
            exitHovered = false;
            exitText.setFillColor(Color::White);
            exitText.setScale(1.f, 1.f);
        }

        // Pause title pulse animation
        static float pausePulse = 0.f;
        pausePulse += deltaTime * 2.f;

        float scale =
            1.f + 0.05f * sin(pausePulse);

        pauseText.setScale(scale, scale);
    }


    else if (this->gamestate == GameState::GAME)
    {
        // Player movement (left/right)
        float dirX = 0.f;
        if (Keyboard::isKeyPressed(Keyboard::Left)) dirX = -1.f;
        else if (Keyboard::isKeyPressed(Keyboard::Right)) dirX = 1.f;

        this->player->movement(dirX, 0.f, this->deltaTime);
        this->player->update(this->deltaTime, *this->window);

        // Shooting (play sound only here; bullets handled inside player)
        static float shootTimer = 0.f;
        shootTimer += this->deltaTime;
        if (Keyboard::isKeyPressed(Keyboard::Space) && shootTimer >= 0.3f)
        {
            this->shootSound.play();
            shootTimer = 0.f;
        }

        // Movement audio (any direction) - Moved inside GAME state
        bool moving = sf::Keyboard::isKeyPressed(Keyboard::Left) ||
            sf::Keyboard::isKeyPressed(Keyboard::Right) ||
            sf::Keyboard::isKeyPressed(Keyboard::Up) ||
            sf::Keyboard::isKeyPressed(Keyboard::Down);

        if (moving && !this->thrusterPlaying) { this->thrusterSound.play(); this->thrusterPlaying = true; }
        else if (!moving && this->thrusterPlaying) { this->thrusterSound.stop(); this->thrusterPlaying = false; }

        // HUD update only during game
        this->hud.update(this->deltaTime);
    }

    // Audio transitions
    if (this->gamestate == GameState::INTRO || this->gamestate == GameState::MAIN_MENU)
    {
        if (this->gameAmbience.getStatus() == sf::Music::Playing) this->gameAmbience.stop();
        if (this->bgMusic.getStatus() != sf::Music::Playing) this->bgMusic.play();
    }
    else if (this->gamestate == GameState::GAME)
    {
        if (this->bgMusic.getStatus() == sf::Music::Playing) this->bgMusic.stop();
        if (this->gameAmbience.getStatus() != sf::Music::Playing) this->gameAmbience.play();
    }
    // -------------------------
    // Enemy spawning + update
    // -------------------------
    if (this->gamestate == GameState::GAME)
    {
        gameTime += this->deltaTime;
        enemySpawnTimer += this->deltaTime;

        if (enemySpawnInterval > 0.5f)
        {
            // This formula decreases the interval by 0.1 every 10 seconds
            enemySpawnInterval -= 0.01f * this->deltaTime;
        }

        if (maxEnemiesFloat < 10.f)
        {
            // This formula adds 1 to the max enemy count every 15 seconds
            // (1.f / 15.f) = 0.066... per second
            maxEnemiesFloat += (1.f / 15.f) * this->deltaTime;
        }

        maxEnemies = static_cast<unsigned int>(round(maxEnemiesFloat));

        // difficulty progression
        if (gameTime > 15.f && enemySpawnInterval > 1.0f)
            enemySpawnInterval -= 0.1f * this->deltaTime;

        if (gameTime > 30.f) maxEnemies = 6;
        else if (gameTime > 20.f) maxEnemies = 5;
        else if (gameTime > 10.f) maxEnemies = 4;

        // spawn condition (cast size to int to avoid unsigned arithmetic warnings)
        int spawnLimit = static_cast<int>(this->window->getSize().x) - 100;
        if (spawnLimit < 1) spawnLimit = 1;
        if (enemySpawnTimer >= enemySpawnInterval && enemies.size() < maxEnemies)
        {
            enemySpawnTimer = 0.f;
            float xPos = static_cast<float>(rand() % spawnLimit);
            enemies.emplace_back(enemyTexture, Vector2f(xPos, -50.f), Vector2f(0.f, 150.f));
        }

        // update enemies
        for (auto& e : enemies) e.update(this->deltaTime);
    }


    // Explosions: update + cleanup
   
    for (auto& ex : explosions) ex.update(this->deltaTime);
    explosions.erase(
        std::remove_if(explosions.begin(), explosions.end(), [](const Collision& c) { return !c.active; }),
        explosions.end()
    );

    
    // Collisions: bullets vs enemies
    
    if (this->gamestate == GameState::GAME)
    {
        std::vector<Bullet>& playerBullets = this->player->getBullets();

        // iterate bullets and enemies with indices since we erase from both
        for (size_t bi = 0; bi < playerBullets.size(); )
        {
            bool bulletRemoved = false;
            const FloatRect bulletBounds = playerBullets[bi].getBounds();

            for (size_t ei = 0; ei < enemies.size(); )
            {
                const FloatRect enemyBounds = enemies[ei].sprite.getGlobalBounds();

                if (bulletBounds.intersects(enemyBounds))
                {
                    // compute center as floats (fix C4244)
                    sf::Vector2f expPos(
                        enemies[ei].sprite.getPosition().x + enemyBounds.width * 0.5f,
                        enemies[ei].sprite.getPosition().y + enemyBounds.height * 0.5f
                    );

                    explosions.emplace_back(explosionTexture, expPos);
                    enemyExplosionSound.play();
                    // erase enemy and bullet
                    enemies.erase(enemies.begin() + ei);
                    playerBullets.erase(playerBullets.begin() + bi);
                    bulletRemoved = true;

                    hud.score += 10;
                    if (hud.score > highScore)
                    {
                        highScore = hud.score;
                        hud.highScore = highScore;
                    }
                    hud.updateTexts();

                    break; // bullet removed -> break enemy loop
                }
                else
                {
                    ++ei;
                }
            }

            if (!bulletRemoved)
                ++bi;
        }
    }

    // Collisions: enemy vs player
    
    if (this->gamestate == GameState::GAME)
    {
        for (auto eit = enemies.begin(); eit != enemies.end(); )
        {
            if (eit->sprite.getGlobalBounds().intersects(this->player->getSprite().getGlobalBounds()))
            {
                // Always create explosion effect
                explosions.emplace_back(
                    explosionTexture,
                    this->player->getSprite().getPosition()
                );

                // Restart explosion sound every collision
                playerExplosionSound.stop();
                playerExplosionSound.play();

                // remove enemy
                eit = enemies.erase(eit);

                // decrease health, update HUD
                playerHealth -= 1.f;
                if (playerHealth < 0.f) playerHealth = 0.f;

                // HUD health shown as fraction of max (3 units)
                hud.healthPercent = playerHealth / 3.f;
                hud.updateTexts();

                if (playerHealth <= 0.f)
                {
                    gamestate = GameState::GAME_OVER;
                }
                // Removed duplicate game over check
            }
            else
            {
                ++eit;
            }
        }
    }
}

// Rendering

void Game::render()
{
    this->window->clear();

    // background: draw two tiles for infinite seamless scroll
    const float textureHeight = static_cast<float>(this->backGroundTex.getSize().y);
    this->backgroundsprite.setPosition(0.f, this->backgroundOffset);
    this->window->draw(this->backgroundsprite);

    this->backgroundsprite.setPosition(0.f, this->backgroundOffset - textureHeight);
    this->window->draw(this->backgroundsprite);

    // UI / states
    if (this->gamestate == GameState::INTRO)
    {
        this->window->draw(this->titleText);
        this->window->draw(this->pressKeyText);
    }
    else if (this->gamestate == GameState::MAIN_MENU)
    {
        this->window->draw(this->startText);
        this->window->draw(this->quitText);
    }
    else if (this->gamestate == GameState::GAME)
    {
        // Render player, enemies, explosions, HUD
        this->player->render(*this->window);

        for (auto& enemy : enemies) enemy.draw(*this->window);

        for (auto& exp : explosions) exp.draw(*this->window);

        this->hud.draw(*this->window);
    }

    if (this->gamestate == GameState::PAUSED)
    {
        // Draw frozen gameplay
        this->player->render(*this->window);

        for (auto& enemy : enemies)
            enemy.draw(*this->window);

        for (auto& exp : explosions)
            exp.draw(*this->window);

        this->hud.draw(*this->window);

        // Pause overlay
        window->draw(pauseOverlay);

        window->draw(pauseText);
        window->draw(resumeText);
        window->draw(exitText);
    }

    if (this->gamestate == GameState::GAME_OVER)
    {
        // dim background
        sf::RectangleShape overlay(sf::Vector2f(window->getSize().x, window->getSize().y));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window->draw(overlay);

        // draw game over UI (keep replay)
        window->draw(gameOvertext);
        window->draw(replayButton);
        window->draw(replayText);
    }

    this->window->display();
}
#include "../include/Game.h"

Game::Game()
    : character("Player", 100, 0, 0), level(100, 100), camera(0, 0) { }

Game::Game(const Game& other)
    : character(other.character), level(other.level), camera(other.camera) { }

void Game::start() {
    initStory();
    readStory();

    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
    window.create(sf::VideoMode({800, 700}), "Adventure Time", sf::Style::Default);
    ///////////////////////////////////////////////////////////////////////////
    //
    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: mandatory use one of vsync or FPS limit (not both)            ///
    /// This is needed so we do not burn the GPU                            ///
    window.setVerticalSyncEnabled(true);                                    ///
    /// window.setFramerateLimit(60);                                       ///
    ///////////////////////////////////////////////////////////////////////////

    while(window.isOpen()) {
        bool shouldExit = false;
        sf::Event e{};
        while(window.pollEvent(e)) {
            switch(e.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                std::cout << "New width: " << window.getSize().x << '\n'
                          << "New height: " << window.getSize().y << '\n';
                break;
            case sf::Event::KeyPressed:
                // std::cout << "Received key " << (e.key.code == sf::Keyboard::X ? "X" : "(other)") << "\n";
                if(e.key.code == sf::Keyboard::Escape)
                    shouldExit = true;
                break;
            default:
                break;
            }
        }
        if(shouldExit) {
            window.close();
            break;
        }
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(30ms);

        handleInput();
        update();
        window.clear();
        render();
        window.display();
    }
}

void Game::update() {
    camera.follow(character.getX(), character.getY());
}

void Game::render() {
    // Draw Player
    sf::CircleShape playerShape(10);
    playerShape.setFillColor(sf::Color::Green);
    playerShape.setPosition(character.getX(), character.getY());
    window.draw(playerShape);
}

void Game::handleInput() {
    int dx = 0, dy = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        dy -= 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        dx -= 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        dy += 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        dx += 1;
    }

    character.move(dx, dy);
}

void Game::initStory() {
    // Read story from file
    std::cout << "Reading story...\n";

    FILE* file = fopen("..//story.txt", "r");
    if (file == nullptr) {
        std::cerr << "Failed to open file\n";
        return;
    }

    while (!feof(file)) {
        char buffer[256];
        fgets(buffer, 256, file);
        story += buffer;
    }
}

void Game::readStory() {
    for (char c : story) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(30ms);
    }
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << "Game(" << game.character << ", " << game.level << ", " << game.camera << ")";
    return os;
}
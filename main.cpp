#include <SFML/Graphics.hpp>

#include <chrono>
#include <cstdlib>
#include <thread>
#include <unordered_map>

class Object {
    public:
    virtual sf::Int32 GetId() = 0;
    virtual void Draw(sf::RenderWindow& window) = 0;
    virtual void Update(sf::Time elapsedTimeMs) = 0;
};

class Renderer {
    public:
    explicit Renderer(sf::RenderWindow& window, sf::Int32 framerate);

    void Render();
    void Update(sf::Time elapsedTimeMs);
    bool RegisterObject(Object& obj);

    private:
    sf::RenderWindow& mWindow;
    sf::Int32 mFramerate;

    std::unordered_map<sf::Int32, Object&> mObjects;
};

Renderer::Renderer(sf::RenderWindow& window, sf::Int32 framerate)
    : mWindow{window}
    , mFramerate{framerate}
{

}

void Renderer::Render() {
    mWindow.setActive(true);

    sf::Clock clock;
    sf::Time accumulatedTimeMs = sf::milliseconds(0);
    while (mWindow.isOpen()) {
        accumulatedTimeMs += clock.restart();

        if (accumulatedTimeMs >= sf::milliseconds(1000 / mFramerate)) {
            accumulatedTimeMs = sf::milliseconds(1000 % mFramerate);

            mWindow.clear(sf::Color::Black);

            for (auto& pair : mObjects) {
                pair.second.Draw(mWindow);
            }

            mWindow.display();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

bool Renderer::RegisterObject(Object& obj) {
    auto res = mObjects.emplace(obj.GetId(), obj);

    return res.second;
}

class GameState {
    public:
    explicit GameState();

    void Update(sf::Time elapsedTimeMs);
    bool RegisterObject(Object& obj);

    private:
    std::unordered_map<sf::Int32, Object&> mObjects;
};

GameState::GameState() {
}

bool GameState::RegisterObject(Object& obj) {
    auto res = mObjects.emplace(obj.GetId(), obj);

    return res.second;
}

void GameState::Update(sf::Time elapsedTimeMs) {
    for (auto& pair : mObjects) {
        pair.second.Update(elapsedTimeMs);
    }
}

class ColorfulCircle : public Object {
    public:
    ColorfulCircle(sf::Int32 id, sf::Int32 refresh);

    sf::Int32 GetId();
    void Draw(sf::RenderWindow& window);
    void Update(sf::Time elapsedTimeMs);

    private:
    sf::Int32 mId;
    sf::Int32 mRefresh;

    sf::CircleShape mShape;
    sf::Color mColor;

    sf::Time mAccumulatedTimeMs;
};

ColorfulCircle::ColorfulCircle(sf::Int32 id, sf::Int32 refresh)
    : mId{id}
    , mRefresh(refresh)
    , mShape(100.0f)
{
    mColor = sf::Color::Green;
    mShape.setFillColor(mColor);

    mAccumulatedTimeMs = sf::milliseconds(0);
}

sf::Int32 ColorfulCircle::GetId() {
    return mId;
}

void ColorfulCircle::Draw(sf::RenderWindow& window) {
    window.draw(mShape);
}

void ColorfulCircle::Update(sf::Time elapsedTimeMs) {
    mAccumulatedTimeMs += elapsedTimeMs;

    if (mAccumulatedTimeMs.asMilliseconds() / mRefresh < 1) {
        return;
    }

    mAccumulatedTimeMs = sf::milliseconds(mAccumulatedTimeMs.asMilliseconds() % mRefresh);

    mColor.r = rand() % 256;
    mColor.g = rand() % 256;
    mColor.b = rand() % 256;

    mShape.setFillColor(mColor);
}

int main(int argc, char* argv[]) {
    sf::RenderWindow window;
    window.create(sf::VideoMode(200, 200), "Tripe");
    window.setVerticalSyncEnabled(true);
    window.setActive(false);

    Renderer renderer(window, 60);
    GameState state;

    ColorfulCircle circle(1, 500);

    renderer.RegisterObject(circle);
    state.RegisterObject(circle);

    std::thread renderThread(&Renderer::Render, &renderer);

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Time elapsed = clock.restart();

        state.Update(elapsed);

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    renderThread.join();

    return 0;
}

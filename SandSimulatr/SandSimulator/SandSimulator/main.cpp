#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <random>

struct Sand {
    sf::RectangleShape shapeSand;
    std::pair<int, int> ix; //ix is index//
    bool active;
};

void ProcessOneTick(std::vector<std::vector<Sand>>& particles, std::vector<std::pair<int, int>>& process, int end, int& count);
void DrawParticle(std::vector<std::vector<Sand>>& particles, sf::RenderWindow& window);
void MATRIX(std::vector<std::vector<Sand>>& particles, int size, float sSize, sf::Vector2f Start);
void placeSand(sf::RenderWindow& window, std::vector<std::vector<Sand>>& particles, std::vector<std::pair<int, int>>& process, int& count);
void TryLoadFont(sf::Font& font, std::string path);
int genRandom(int min, int max);

int main() {

    sf::RenderWindow window(sf::VideoMode(1000, 600), "SandSimulator");
    sf::Event e;

    sf::Clock sClock;
    sf::Time sFall = sf::milliseconds(3);

    sf::Clock pClock;
    sf::Time sPlace = sf::milliseconds(3);

    std::vector<std::vector<Sand>> particles;
    std::vector<std::pair<int, int>> process;
    int count = 0;
    MATRIX(particles, 200, 3, sf::Vector2f(200, 100));

    while (window.isOpen()) 
    {

        while (window.pollEvent(e)) 
        {
            if (e.type == sf::Event::Closed) 
            {
                window.close();
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
        {
            if (pClock.getElapsedTime() >= sPlace) 
            {
                placeSand(window, particles, process, count);
                pClock.restart();
            }
        }

        if (sClock.getElapsedTime() >= sFall) {
            ProcessOneTick(particles, process, 100, count);

            sClock.restart();
        }

        // --------- clear the screen ----------
        window.clear(sf::Color::Black);

        // --------- draw on the screen ----------
        DrawParticle(particles, window);

        // --------- display on the screen --------
        window.display();
    }

}

void ProcessOneTick(std::vector<std::vector<Sand>>& particles, std::vector<std::pair<int, int>>& process, int end, int& count)
{
    for (int i = 0; i < process.size(); i++)
    {
        int x = process[i].first;
        int y = process[i].second;

        if (y + 1 == end)
        {
            particles[x][y].active = true;
            process.erase(process.begin() + i);
            continue;
        }

        if (!particles[x][y + 1].active && particles[x][y].shapeSand.getPosition().x >= 100 && particles[x][y].shapeSand.getPosition().x <= 900)
        {
            particles[x][y].active = false;
            process[i].second = y + 1;
            particles[x][y + 1].active = true;
            continue;
        }
        else {
            bool left = (x > 0 && !particles[x - 1][y + 1].active);
            bool right = (x < particles.size() - 1 && !particles[x + 1][y + 1].active);

            if (left && right) {
                int direction = genRandom(0, 1);
                if (direction == 0)
                {
                    particles[x][y].active = false;
                    process[i].first = x - 1;
                    process[i].second = y + 1;
                    particles[x - 1][y + 1].active = true;
                }
                else
                {
                    particles[x][y].active = false;
                    process[i].first = x + 1;
                    process[i].second = y + 1;
                    particles[x + 1][y + 1].active = true;
                }
            }
            else if (left)
            {
                particles[x][y].active = false;
                process[i].first = x - 1;
                process[i].second = y + 1;
                particles[x - 1][y + 1].active = true;
            }
            else if (right)
            {
                particles[x][y].active = false;
                process[i].first = x + 1;
                process[i].second = y + 1;
                particles[x + 1][y + 1].active = true;
            }
            else
            {
                particles[x][y].active = true;
                process.erase(process.begin() + i);
            }
        }
    }
}

void DrawParticle(std::vector<std::vector<Sand>>& particles, sf::RenderWindow& window)
{
    int spaceCount = 0;
    int sandCount = 0;

    for (int i = 0; i < particles.size(); i++)
    {
        for (int j = 0; j < particles.size(); j++)
        {
            if (particles[i][j].active)
            {
                window.draw(particles[i][j].shapeSand);
                sandCount++;
            }
            else
            {
                spaceCount++;
            }
        }
    }

    sf::Font font;
    TryLoadFont(font, "8bitfont.ttf");
    sf::Text spaceText("Space: " + std::to_string(spaceCount), font, 20);
    sf::Text sandText("Sand: " + std::to_string(sandCount), font, 20);
    spaceText.setPosition(10, 10);
    sandText.setPosition(10, 40);
    window.draw(spaceText);
    window.draw(sandText);
}

void MATRIX(std::vector<std::vector<Sand>>& particles, int size, float sSize, sf::Vector2f Start) 
{
    std::vector<std::vector<Sand>> vector(size, std::vector<Sand>(size));
    particles = vector;

    for (int i = 0; i < size; i++) 
    {
        for (int j = 0; j < size; j++) 
        {
            Sand sand;
            sand.ix.first = i;
            sand.ix.second = j;
            sand.shapeSand = sf::RectangleShape(sf::Vector2f(sSize, sSize));
            sand.shapeSand.setPosition(sf::Vector2f(Start.x + sSize * i, Start.y + sSize * j));
            sand.active = false;
            particles[i][j] = sand;
        }
    }
}



void placeSand(sf::RenderWindow& window, std::vector<std::vector<Sand>>& particles, std::vector<std::pair<int, int>>& process, int& count) 
{
    sf::Vector2i pos = sf::Mouse::getPosition(window);
    for (int i = 0; i < particles.size(); i++) 
    {
        for (int j = 0; j < particles.size(); j++) 
        {
            if (particles[i][j].shapeSand.getGlobalBounds().contains((sf::Vector2f)pos)) 
            {
                if (particles[i][j].active)
                {
                    return;
                }
                particles[i][j].active = true;
                particles[i][j].shapeSand.setFillColor(sf::Color::White);
                process.push_back(std::make_pair(i, j));
                count++;
            }
        }
    }
}
void TryLoadFont(sf::Font& font, std::string path)
{
    if (!font.loadFromFile(path))
    {
        std::cerr << "Failed to load font: " << path << std::endl;
        exit(EXIT_FAILURE);
    }
}

int genRandom(int min, int max) 
{
    std::random_device rd;
    std::mt19937 gen(rd()); //Mersenne Twister, most popular PRNG//
    std::uniform_int_distribution<> distribution(min, max); 
    return distribution(gen);
}
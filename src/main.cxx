#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>
#include <chrono>

#include "fonts.h"
#include "main.h"
#include "state.h"

#include "states/intro_state.h"
#include "states/adjust_volume_state.h"

namespace pacman
{
    namespace fonts
    {
        sf::Font emulogic;
    }
}

namespace pacman
{
    static std::unique_ptr<base_state> m_current_state;
    save current_save("save.txt");

    base_state& current_state() { return *m_current_state; }
    base_state& set_state(base_state* new_state)
    {
        if(new_state == nullptr)
            return *new_state; // it's too late!

        if(m_current_state)
            m_current_state->on_leave(*new_state);

        new_state->on_enter(m_current_state.get());
        m_current_state.reset(new_state);
    }
}

int main(int argc, char** argv)
{
    pacman::fonts::emulogic.loadFromFile("data/emulogic.ttf");
    
    sf::RenderWindow window(sf::VideoMode(224 * 2 + 200, 288 * 2), "a title");
    bool fullscreen = false;

    sf::RenderTexture target;
    target.create(224, 288);
    target.setSmooth(false);

    window.setFramerateLimit(500);
    window.setVerticalSyncEnabled(false);
    window.setMouseCursorVisible(false);
    window.setKeyRepeatEnabled(false);

    if(pacman::current_save.volume == -1)
        pacman::set_state(new pacman::states::adjust_volume_state(true));
    else
        pacman::set_state(new pacman::states::intro_state());

    std::chrono::high_resolution_clock::time_point last_time = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::duration total_time;

    while(window.isOpen())
    {
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        std::chrono::high_resolution_clock::duration diff = now - last_time;

        total_time += diff;
        last_time = now;

        sf::Event event;
        while(window.pollEvent(event))
        {
            if(!pacman::current_state().on_sf_event(event))
            {
                if(event.type == sf::Event::Closed)
                    window.close();

                if(event.type == sf::Event::KeyPressed)
                {
                    if(event.key.code == sf::Keyboard::F11)
                    {
                        if(fullscreen)
                            window.create(sf::VideoMode(224 * 2 + 200, 288 * 2, 288 * 2), "a title");
                        else
                            window.create(sf::VideoMode::getDesktopMode(), "a fullscreen title", sf::Style::Fullscreen);

                        window.setFramerateLimit(500);

                        // v-sync is only enabled in fullscreen
                        window.setVerticalSyncEnabled(true);
                        
                        window.setMouseCursorVisible(false);
                        window.setKeyRepeatEnabled(false);
                        
                        fullscreen = !fullscreen;
                    }
                }
            }
        }

        window.setView(sf::View({ 0, 0, window.getSize().x, window.getSize().y }));        

        pacman::base_state& state = pacman::current_state();
        
        double delta = std::chrono::duration_cast<std::chrono::duration<double>>(diff).count();
        // FPS <=24 
        if(delta > 1 / 24.0)
        	delta = 1 / 24.0;
        	
        state.update(delta, std::chrono::duration_cast<std::chrono::duration<double>>(total_time).count());

        if(&state == &pacman::current_state()) // if we didn't switch states in update()
        {
            target.clear(sf::Color::Black);
            state.draw(target);
            target.display();
        }

        window.clear(sf::Color::Black);

        float scale = std::min(window.getSize().x / 224.0, window.getSize().y / 288.0);
        if(scale > 1)
            scale = std::floor(scale);

        target.setSmooth(scale < 1);

        sf::Sprite target_spr(target.getTexture());
        target_spr.setPosition({ window.getSize().x / 2 - (224 * scale) / 2, window.getSize().y / 2 - (288 * scale) / 2 });
        target_spr.setScale(scale, scale);

        window.draw(target_spr);
        window.display();
    }

    pacman::current_save.save_to_file("save.txt");
    return EXIT_SUCCESS;
}

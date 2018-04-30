#pragma once

#include "../state.h"
#include "../fonts.h"
#include "../main.h"

#include <SFML/Audio.hpp>

namespace pacman
{
    namespace states
    {
        class unlock_shorts_san_state : public base_state
        {
public:
            unlock_shorts_san_state() 
            {
                m_unlock_music.openFromFile("data/music/unlock.ogg");
                m_unlock_music.setVolume(current_save.volume);

                m_shorts.loadFromFile("data/pacman/shorts/1.png");
            }

            void on_enter(base_state* last_state) override 
            { 
                m_unlock_music.play(); 
                current_save.unlocked[data::characters::SHORTS] = true;
            }

            void update(double delta, double total_time) override;
            void draw(sf::RenderTarget& target) override;
private:
            double start_time = 0;
            double total_time = 0;

            sf::Music m_unlock_music;

            double m_exit = 0;

            double m_shorts_y = 288 + 50;
            sf::Texture m_shorts;

            std::string m_target_text = "New character unlocked:\nDoremi's shorts.";
            std::string m_text = "";
            double m_next_char = 0;
        };
    }
}
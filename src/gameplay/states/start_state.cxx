#include "start_state.h"

#include <cmath>

namespace pacman
{
    namespace gameplay
    {
        void start_state::update(double delta, double total_time)
        {
            if(start_time == 0)
                start_time = total_time;

            accum_new_map_seed_time += delta;
            while(accum_new_map_seed_time >= 0.2)
            {
                srand((int)(parent.map_seed * accum_new_map_seed_time));
                parent.map_seed += rand();

                accum_new_map_seed_time -= 0.2;
            }

            this->total_time = total_time;

            if(m_start_sound.getStatus() != sf::Sound::Playing)
            {
                if(m_exit > 0.2)
                    parent.set_state(m_play_state);
                else
                    m_exit += delta;
            }
        }

        void start_state::draw(sf::RenderTarget& target)
        {
            sf::Text player(parent.player.character.name, fonts::emulogic, 8);
            player.setColor(parent.player.character.primary_color);
            player.setPosition((int)(224 / 2 - player.getLocalBounds().width / 2), 107);

            // player.move(std::sin(total_time * 18) * std::max(total_time * 5 - start_time * 5 - 10, 0.0) * 10, std::cos(total_time * 13) * std::max(total_time * 5 - start_time * 5 - 10, 0.0) * 8);
            target.draw(player);
            // player.move(-std::sin(total_time * 18) * std::max(total_time * 5 - start_time * 5 - 10, 0.0) * 4, -std::cos(total_time * 13) * std::max(total_time * 5 - start_time * 5 - 10, 0.0) * 8);

            sf::Text ready("LEVEL " + std::to_string(parent.level) + "!", fonts::emulogic, 8);
            ready.setColor({ 255, 193, 7 });
            ready.setPosition((int)(224 / 2 - ready.getLocalBounds().width / 2), 155);
            // ready.setPosition(-ready.getLocalBounds().width + (int)((224 + ready.getLocalBounds().width * 2) * ((total_time - start_time) / 4.5)), 155);

            // ready.move(std::sin(total_time * 20) * std::max(total_time * 5 - start_time * 5 - 4.8, 0.0) * 10, std::cos(total_time * 11) * std::max(total_time * 5 - start_time * 5 - 4.8, 0.0) * 8);
            
            if((int)((total_time - start_time) * 1000) % 600 <= 300)
                target.draw(ready);

            // ready.move(-std::sin(total_time * 20) * std::max(total_time * 5 - start_time * 5 - 4.8, 0.0) * 10, -std::cos(total_time * 11) * std::max(total_time * 5 - start_time * 5 - 4.8, 0.0) * 8);
        }
    }
}
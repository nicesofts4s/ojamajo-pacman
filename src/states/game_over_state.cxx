#include "game_over_state.h"
#include "insert_coin_state.h"

#include <cmath>

namespace pacman
{
    namespace states
    {
        void game_over_state::update(double delta, double total_time)
        {
            if(start_time == 0)
                start_time = total_time;

            this->total_time = total_time;

            if(m_exit)
            {
                if(m_exit > 1.4)
                {
                    pacman::set_state(new insert_coin_state());
                    return;
                }

                m_exit += delta;
            }
        }

        void game_over_state::draw(sf::RenderTarget& target)
        {
            if(m_exit)
                return;

            sf::Text game_over_text(
                current_save.selected_character == data::characters::BURYPINK ? 
                "Ebil has won..." : 
                "Game over...", fonts::emulogic, 8);
            
            if(m_player_texture.getSize() != sf::Vector2u { 1, 1 })
            {
                sf::Sprite player(m_player_texture);

                player.setPosition(
                    (int)(224 / 2 - player.getLocalBounds().width / 2), 
                    (int)(288 / 2 - player.getLocalBounds().height / 2) - 40);

                target.draw(player);

                game_over_text.setPosition(
                    (int)(224 / 2 - game_over_text.getLocalBounds().width / 2), 
                    (int)(288 / 2 - game_over_text.getLocalBounds().height / 2) - 20);
            }
            else
            {
                game_over_text.setPosition(
                    (int)(224 / 2 - game_over_text.getLocalBounds().width / 2), 
                    (int)(288 / 2 - game_over_text.getLocalBounds().height / 2) - 30);
            }

            target.draw(game_over_text);

            sf::Text level_reached_text("Level " + std::to_string(m_level_reached) + " reached", fonts::emulogic, 8);

            level_reached_text.setColor({ 255, 255, 255, 120 });
            level_reached_text.setPosition(
                (int)(224 / 2 - level_reached_text.getLocalBounds().width / 2), 
                game_over_text.getPosition().y + 10);

            target.draw(level_reached_text);

            sf::Text options_text("        return to main menu", fonts::emulogic, 8);

            options_text.setPosition(
                (int)(224 / 2 - options_text.getLocalBounds().width / 2), 
                (int)(288 / 2 - options_text.getLocalBounds().height / 2) + 30);

            sf::Text options_color_text("[Enter]", fonts::emulogic, 8);

            options_color_text.setPosition(options_text.getPosition());
            options_color_text.setColor({ 80, 188, 70, 255 });

            if((int)((total_time - start_time) * 1000) % 1000 > 500)
            {
                target.draw(options_text);
                target.draw(options_color_text);
            }
        }
    }
}
#pragma once

#include "../state.h"
#include "../fonts.h"
#include "../main.h"

#include <SFML/Audio.hpp>

namespace pacman
{
    namespace states
    {
        class game_over_state : public base_state
        {
public:
            game_over_state(std::uint64_t level_reached) :
                m_level_reached(level_reached)
            {
                m_click_snd_buf.loadFromFile("data/sound/click.wav");
                
                m_click_snd = sf::Sound(m_click_snd_buf);
                m_click_snd.setVolume(current_save.volume);

                m_player_texture.create(1, 1);

                if(current_save.selected_character != data::characters::DODO &&
                    current_save.selected_character != data::characters::SHORTS &&
                    current_save.selected_character != data::characters::PACMAN)
                {
                    if(current_save.selected_character == data::characters::BURYPINK)
                        m_player_texture.loadFromFile("data/pacman/burypink/buryebil.png");
                    else
                        m_player_texture.loadFromFile("data/pacman/" + std::string(data::characters::enum_to_info(current_save.selected_character).path) + "/froge.png");
                }
            }

            bool on_sf_event(sf::Event& ev) override
            {
                if(!m_exit && ev.type == sf::Event::KeyPressed)
                {
                    if(ev.key.code == sf::Keyboard::Return)
                    {
                        m_exit = true;
                        m_click_snd.play();
                    }
                }

                return false;
            }

            void update(double delta, double total_time) override;
            void draw(sf::RenderTarget& target) override;
private:
            sf::SoundBuffer m_click_snd_buf;
            sf::Sound m_click_snd;

            double start_time = 0;
            double total_time = 0;

            double m_exit = 0;

            std::uint64_t m_level_reached;

            sf::Texture m_player_texture;
        };
    }
}

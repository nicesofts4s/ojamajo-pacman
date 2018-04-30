#pragma once

#include "../../main.h"
#include "../../states/gameplay_state.h"

#include <SFML/Audio.hpp>

namespace pacman
{
    namespace gameplay
    {
        class death_state : public states::gameplay_state::base_state
        {
public:
            death_state(states::gameplay_state& parent) : 
                states::gameplay_state::base_state(parent) 
            { 
                m_death_sound_buf.loadFromFile("data/sound/die.wav");

                m_death_sound = sf::Sound(m_death_sound_buf);
                m_death_sound.setVolume(current_save.volume);

                m_die_texture.loadFromFile("data/pacman/" + std::string(parent.player.character.path) + "/die.png");
            }

            void on_enter(states::gameplay_state::base_state* last_state) override 
            { 
                m_death_sound.play(); 
                parent.map_seed = -1;
            }
            
            void update(double delta, double total_time) override;
            void draw(sf::RenderTarget& target) override;
private:
            sf::SoundBuffer m_death_sound_buf;
            sf::Sound m_death_sound;

            double start_time = 0;
            double total_time = 0;

            double m_exit = 0;

            sf::Texture m_die_texture;
        };
    }
}
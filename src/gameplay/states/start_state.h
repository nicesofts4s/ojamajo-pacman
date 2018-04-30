#pragma once

#include "../../main.h"
#include "../../states/gameplay_state.h"

#include <SFML/Audio.hpp>

namespace pacman
{
    namespace gameplay
    {
        class start_state : public states::gameplay_state::base_state
        {
public:
            start_state(states::gameplay_state& parent) : 
                states::gameplay_state::base_state(parent) 
            { 
                m_start_sound_buf.loadFromFile("data/sound/start.wav");

                m_start_sound = sf::Sound(m_start_sound_buf);
                m_start_sound.setVolume(current_save.volume);
            }

            void on_enter(states::gameplay_state::base_state* last_state) override { m_start_sound.play(); }
            
            void update(double delta, double total_time) override;
            void draw(sf::RenderTarget& target) override;
private:
            sf::SoundBuffer m_start_sound_buf;
            sf::Sound m_start_sound;

            double start_time = 0;
            double total_time = 0;
            double accum_new_map_seed_time = 0;

            double m_exit = 0;
        };
    }
}
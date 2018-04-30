#pragma once

#include "../state.h"
#include "../fonts.h"
#include "../main.h"

#include <SFML/Audio.hpp>

namespace pacman
{
    namespace states
    {
        class adjust_volume_state : public base_state
        {
public:
            adjust_volume_state(bool intro_after) :
                m_intro_after(intro_after)
            {
                m_dodo_sound_buf.loadFromFile("data/sound/adjustvolume.wav");
                m_dodo_sound = sf::Sound(m_dodo_sound_buf);

                if(m_intro_after)
                    current_save.volume = 10;

                m_click_snd_buf.loadFromFile("data/sound/click.wav");
                
                m_click_snd = sf::Sound(m_click_snd_buf);
                m_click_snd.setVolume(current_save.volume);
            }

            bool on_sf_event(sf::Event& ev) override;

            void update(double delta, double total_time) override;
            void draw(sf::RenderTarget& target) override;
private:
            sf::SoundBuffer m_dodo_sound_buf;
            sf::Sound m_dodo_sound;

            double m_shake = 0;
            bool m_intro_after;

            double start_time = 0;
            double total_time = 0;

            sf::SoundBuffer m_click_snd_buf;
            sf::Sound m_click_snd;

            double m_exit = 0;
        };
    }
}

#pragma once

#include "../state.h"
#include "../fonts.h"
#include "../main.h"
#include "../gameplay/data/characters.h"

#include <SFML/Audio.hpp>

namespace pacman
{
    namespace states
    {
        class intro_state : public base_state
        {
public:
            intro_state() :
                m_intro_text(".............. NICESOFT ..............", fonts::emulogic, 8)
            {
                m_intro_text.setColor({ 255, 80, 255, 255 }); // 255 nice dubs
                m_intro_text.setPosition({ 
                    (int)(224 / 2 - m_intro_text.getLocalBounds().width / 2), 
                    (int)(288 / 2 - m_intro_text.getLocalBounds().height / 2), 
                });

                m_chomp_snd_buf.loadFromFile("data/sound/eat.wav");
                m_chomp_snd = sf::Sound(m_chomp_snd_buf);
                m_chomp_snd.setVolume(current_save.volume);

                m_doremi_sprite.setTexture(data::characters::doremi.texture(0));
                m_doremi_sprite.setPosition({ -20, 288 / 2 - data::characters::doremi.texture(0).getSize().y / 2 - 1 });
            }

            bool on_sf_event(sf::Event& ev) override 
            {
                if(ev.type == sf::Event::KeyPressed)
                {
                    if(ev.key.code == sf::Keyboard::Return)
                        m_wants_skip = true;
                }

                return false;
            }

            void update(double delta, double total_time) override;
            void draw(sf::RenderTarget& target) override;
private:
            sf::Text m_intro_text;
            
            sf::SoundBuffer m_chomp_snd_buf;
            sf::Sound m_chomp_snd;

            sf::Sprite m_doremi_sprite;

            double start_time = 0;  

            bool m_last_was_over_150 = false;
            int m_goal = 0;
            int m_char = 0;

            bool m_wants_skip = false;
        };
    }
}
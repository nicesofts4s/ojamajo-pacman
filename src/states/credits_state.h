#pragma once

#include "../state.h"
#include "../fonts.h"
#include "../main.h"

#include <vector>

#include <SFML/Audio.hpp>

namespace pacman
{
    namespace states
    {
        class credits_state : public base_state
        {
public:
            constexpr static std::size_t max_post_num = 8000000;

            credits_state() :
                m_credits({
                    sf::Text("- OJAMAJO PACMAN -", fonts::emulogic, 8),
                    sf::Text("(press escape to", fonts::emulogic, 8),
                    sf::Text("exit credits)", fonts::emulogic, 8),
                    sf::Text("", fonts::emulogic, 8),
                    sf::Text("Created and produced by", fonts::emulogic, 8),
                    sf::Text("NICESOFT for [s4s]", fonts::emulogic, 8),
                    sf::Text("Original game by Namco", fonts::emulogic, 8),
                    sf::Text("Ojamajo Doremi by Toei", fonts::emulogic, 8),
                    sf::Text("", fonts::emulogic, 8),
                    sf::Text("Programming", fonts::emulogic, 8),
                    sf::Text("yuiposter #1 original", fonts::emulogic, 8),
                    sf::Text("Doctor Worse Than Hitler", fonts::emulogic, 8),
                    sf::Text("", fonts::emulogic, 8),
                    sf::Text("Graphics", fonts::emulogic, 8),
                    sf::Text("yuipostingman", fonts::emulogic, 8),
                    sf::Text("Doctor Worse Than Hitler", fonts::emulogic, 8),
                    sf::Text("Namco", fonts::emulogic, 8),
                    sf::Text("", fonts::emulogic, 8),
                    sf::Text("Audio", fonts::emulogic, 8),
                    sf::Text("Namco", fonts::emulogic, 8),
                    sf::Text("Keiichi Oku (Toei)", fonts::emulogic, 8),
                    sf::Text("", fonts::emulogic, 8),
                    sf::Text("Special thanks", fonts::emulogic, 8),
                    sf::Text("The Spikeman <3", fonts::emulogic, 8),
                    sf::Text("All frens on [s4s],", fonts::emulogic, 8),
                    sf::Text("including:", fonts::emulogic, 8),
                    sf::Text("", fonts::emulogic, 8),
                    /* post nums go here */
                })
            {
                m_bg_music.openFromFile("data/music/credits.ogg");
                m_bg_music.setVolume(current_save.volume);
                m_bg_music.setLoop(true);

                int y = 0;
                for(sf::Text& text : m_credits)
                {
                    text.setColor({ 180, 180, 180, 255 });
                    text.setPosition(0 /* set later */, 288 + 20 + 9 * y++);
                }

                m_credits[0].setColor({ 255, 255, 255, 255 });
                m_credits[1].setColor({ 100, 100, 100, 255 });
                m_credits[2].setColor({ 100, 100, 100, 255 });
                m_credits[9].setColor({ 255, 255, 255, 255 });
                m_credits[13].setColor({ 255, 255, 255, 255 });
                m_credits[18].setColor({ 255, 255, 255, 255 });
                m_credits[22].setColor({ 255, 255, 255, 255 });

                m_current_hadzuki.loadFromFile("data/black_1x1.png");
                m_next_hadzuki.loadFromFile("data/black_1x1.png");

                m_current_hadzuki.setSmooth(true);
                m_next_hadzuki.setSmooth(true);

                m_click_snd_buf.loadFromFile("data/sound/click.wav");
                
                m_click_snd = sf::Sound(m_click_snd_buf);
                m_click_snd.setVolume(current_save.volume);
            }

            bool on_sf_event(sf::Event& ev) override;

            void on_enter(base_state* last_state) override;
            void update(double delta, double total_time) override;
            void draw(sf::RenderTarget& target) override;
private:
            sf::Music m_bg_music;

            std::size_t m_current_post_num = 1;
            std::vector<sf::Text> m_credits;

            sf::Texture m_current_hadzuki, m_next_hadzuki;
            float m_next_hadzuki_fade = 0;

            sf::SoundBuffer m_click_snd_buf;
            sf::Sound m_click_snd;

            double m_exit = 0;
        };
    }
}

#pragma once

#include "../state.h"
#include "../fonts.h"
#include "../main.h"

#include <SFML/Audio.hpp>

namespace pacman
{
    namespace states
    {
        class select_character_state : public base_state
        {
public:
            select_character_state(std::int64_t selected = -1) :
                m_selected(selected == -1 ? current_save.selected_character : selected),
                m_selected_display(m_selected * 224),

                m_nicemonye_text(std::to_string(current_save.nicemonye), fonts::emulogic, 8)
            {
                m_arrow_texture.loadFromFile("data/arrow.png");
                m_unknown_texture.loadFromFile("data/pacman/unknown.png");
                m_nicemonye_texture.loadFromFile("data/nicemonye.png");

                m_click_snd_buf.loadFromFile("data/sound/click.wav");
                
                m_click_snd = sf::Sound(m_click_snd_buf);
                m_click_snd.setVolume(current_save.volume);

                m_select_snd_buf.loadFromFile("data/sound/select.wav");
                
                m_select_snd = sf::Sound(m_select_snd_buf);
                m_select_snd.setVolume(current_save.volume);

                m_bg_music.openFromFile("data/music/select.ogg");
                m_bg_music.setVolume(current_save.volume);
                m_bg_music.setLoop(true);

                m_nicemonye_text.setPosition({ 
                    (int)(224 - m_nicemonye_text.getLocalBounds().width - 10), 
                    (int)(288 - m_nicemonye_text.getLocalBounds().height - 10), 
                });
                m_nicemonye_text.setColor({ 255, 108, 231, 180 });
            }

            void on_enter(base_state* last_state) override { m_bg_music.play(); }

            bool on_sf_event(sf::Event& ev) override;

            void update(double delta, double total_time) override;
            void draw(sf::RenderTarget& target) override;
private:
            double start_time = 0;
            double total_time = 0;

            double m_last_selection_time = 0;

            double m_left_arrow_hide = 0, m_right_arrow_hide = 0;

            sf::Music m_bg_music;

            sf::SoundBuffer m_click_snd_buf;
            sf::Sound m_click_snd;

            sf::SoundBuffer m_select_snd_buf;
            sf::Sound m_select_snd;

            double m_exit = 0;
            base_state* m_exit_state = nullptr;

            std::uint64_t m_selected = 0;
            float m_selected_display = 0;

            sf::Texture m_arrow_texture, m_unknown_texture, m_nicemonye_texture;

            sf::Text m_nicemonye_text;
        };
    }
}
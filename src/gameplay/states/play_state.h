#pragma once

#include "../../states/gameplay_state.h"
#include "../ghost.h"

#include "../ai/all.h"

#include "../../states/insert_coin_state.h"

namespace pacman
{
    namespace gameplay
    {
        enum bg_snd_type : std::uint8_t
        {
            BG_SND_NORMAL = 0,
            BG_SND_EATEN = 2,
            BG_SND_SPECIAL = 3
        };

        class play_state : public states::gameplay_state::base_state
        {
public:
            play_state(states::gameplay_state& parent) : 
                states::gameplay_state::base_state(parent),
                m_ghosts({
                    std::make_unique<ai::blinky>(),
                    std::make_unique<ai::clyde>(),
                    std::make_unique<ai::inky>(),
                    std::make_unique<ai::pinky>()
                }),

                m_nicemonye_spawns_left(std::min((std::uint64_t)8, parent.level / 4 + 1))
            { 
                m_click_snd_buf.loadFromFile("data/sound/click.wav");
                
                m_click_snd = sf::Sound(m_click_snd_buf);
                m_click_snd.setVolume(current_save.volume);

                m_eat_ghost_snd_buf.loadFromFile("data/sound/eat_ghost.wav");

                m_eat_ghost_snd = sf::Sound(m_eat_ghost_snd_buf);
                m_eat_ghost_snd.setVolume(current_save.volume);

                for(int i = 0; i < 4; i++)
                {
                    m_bg_snd_buf[i].loadFromFile("data/sound/bg_" + (i == BG_SND_SPECIAL ? "special" : std::to_string(i + 1)) + ".wav");

                    m_bg_snd[i] = sf::Sound(m_bg_snd_buf[i]);
                    m_bg_snd[i].setVolume(current_save.volume);
                    m_bg_snd[i].setLoop(true);
                }

                m_pause_music.openFromFile("data/music/pause.ogg");
                m_pause_music.setVolume(0);

                m_gets_texture.loadFromFile("data/gets.png");

                m_nicemonye_texture.loadFromFile("data/nicemonye.png");
            }

            void on_enter(states::gameplay_state::base_state* last_state) override;

            bool on_sf_event(sf::Event& ev) override 
            { 
                if(!m_exit && ev.type == sf::Event::KeyPressed)
                {
                    if(ev.key.code == sf::Keyboard::Escape && total_time >= m_next_allowed_pause_time)
                    {
                        m_paused = !m_paused;

                        if(!m_paused)
                            m_next_allowed_pause_time = total_time + 0.4;

                        m_pause_start_time = total_time;
                    }

                    if(m_paused && !m_exit)
                    {
                        if(ev.key.code == sf::Keyboard::X)
                        {
                            m_exit = true;
                            m_exit_state = new states::insert_coin_state();

                            m_click_snd.play();
                        }

                        if(ev.key.code == sf::Keyboard::R)
                        {
                            m_exit = true;
                            m_exit_state = new states::gameplay_state(1);

                            m_click_snd.play();
                        }
                    }
                }
                return false; 
            }
            
            void update(double delta, double total_time) override;
            void draw(sf::RenderTarget& target) override;

            std::array<std::unique_ptr<base_ai_ghost>, 4>& ghosts() { return m_ghosts; }
            const std::array<std::unique_ptr<base_ai_ghost>, 4>& ghosts() const { return m_ghosts; }

            void frighten_ghosts(double time) 
            { 
                m_frighten_time = time; 

                for(std::unique_ptr<base_ai_ghost>& ptr : m_ghosts)
                    ptr->transition_behavior(base_ai_ghost::behavior_mode::frightened);
            }

            double frighten_time_left() const { return m_frighten_time; }

            double m_pause_for = 0;
private:
            double start_time = 0;
            double total_time = 0;

            bool m_paused = false;
            double m_pause_start_time = 0;
            double m_next_allowed_pause_time = 0;

            double m_exit = 0;
            pacman::base_state* m_exit_state = nullptr;

            sf::SoundBuffer m_click_snd_buf;
            sf::Sound m_click_snd;

            sf::SoundBuffer m_eat_ghost_snd_buf;
            sf::Sound m_eat_ghost_snd;

            sf::SoundBuffer m_bg_snd_buf[4];
            sf::Sound m_bg_snd[4];

            sf::Music m_pause_music;

            sf::Texture m_gets_texture;

            bg_snd_type m_current_bg_sound = BG_SND_NORMAL;

            std::array<std::unique_ptr<base_ai_ghost>, 4> m_ghosts;

            int m_ai_cycle = 0;
            double m_ai_cycle_accum = 0;

            double m_frighten_time = 0;

            int m_pellet_combo = 0;
            bool m_show_combo = false;
            base_ai_ghost* m_eaten_ghost = nullptr;

            enum class after_pause_action 
            {
                nothing,
                die,
                win
            };

            after_pause_action m_after_pause_action = after_pause_action::nothing;

            sf::Texture m_nicemonye_texture;

            std::uint64_t m_nicemonye_spawns_left;

            sf::Vector2i m_nicemonye_pos = { 0, 0 };
            double m_nicemonye_time_left = 0;
            std::uint64_t m_nicemonye_worth = 0;
        };
    }
}
#include "insert_coin_state.h"
#include "gameplay_state.h"
#include "../main.h"

namespace pacman
{
    namespace states
    {
        void insert_coin_state::pick_new_doremi_y()
        {
            int possibilites[] = { 
                288 - 50,
                20,
                40,
                188,
            };

            std::srand(time(nullptr));

            m_doremi_sprite.setPosition(m_doremi_sprite.getPosition().x, possibilites[std::rand() % 5]);

            do
                m_current_bg_character = std::rand() % data::characters::SIZE_CHARACTER;
            while(m_current_bg_character == data::characters::SHORTS);
        }

        void insert_coin_state::update(double delta, double total_time)
        {
            if(start_time == 0)
                start_time = total_time;

            this->total_time = total_time;

            if(m_exit && m_bg_music.getStatus() == sf::SoundSource::Playing)
                m_bg_music.stop();

            if(m_exit && (m_doremi_sprite.getPosition().x < -20 || m_doremi_sprite.getPosition().x > 224 + 20))
            {
                if(m_exit > 1.4)
                {
                    pacman::set_state(m_exit_state);
                    return;
                }

                m_exit += delta;
            }

            if(m_doremi_sprite.getScale().x == -1)
            {
                m_doremi_sprite.move(-delta * (m_exit ? 140 : 40), 0);

                if(!m_exit && m_doremi_sprite.getPosition().x < -20)
                {
                    pick_new_doremi_y();
                    m_doremi_sprite.setScale(1, 1);
                }
            }
            else
            {
                m_doremi_sprite.move(delta * (m_exit ? 140 : 40), 0);

                if(!m_exit && m_doremi_sprite.getPosition().x > 224 + 20)
                {
                    pick_new_doremi_y();
                    m_doremi_sprite.setScale(-1, 1);
                }           
            }

            m_doremi_sprite.setTexture(
                data::characters::enum_to_info(static_cast<data::characters::character_enum>(m_current_bg_character)).
                texture((int)((total_time - start_time) * (m_exit ? 2000 : 1000)) % 300 > 150), true);
        }

        void insert_coin_state::draw(sf::RenderTarget& target)
        {
            if(!m_exit)
            {
                target.draw(m_title_text);
                target.draw(m_topd_text);

                target.draw(m_highscore_text);

                if((int)((total_time - start_time) * 1000) % 1000 > 500)
                {
                    target.draw(m_buttons_text);
                    target.draw(m_buttons_color_text);
                }
            }

            target.draw(m_doremi_sprite);
        }
    }
}

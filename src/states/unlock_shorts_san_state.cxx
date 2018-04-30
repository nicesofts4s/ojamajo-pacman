#include "unlock_shorts_san_state.h"
#include "select_character_state.h"

namespace pacman
{
    namespace states
    {
        void unlock_shorts_san_state::update(double delta, double total_time)
        {
            if(start_time == 0)
                start_time = total_time;

            this->total_time = total_time;

            if(m_exit)
            {
                if(m_exit > 1.4)
                {
                    set_state(new select_character_state(data::characters::SHORTS));
                    return;
                }

                m_exit += delta;
                return;
            }

            if(m_shorts_y > 80)
                m_shorts_y -= delta * 8;
            else
            {
                m_shorts_y = 80;
            }

            double t = (total_time - start_time);

            if(t >= 38)
            {
                m_next_char -= delta;

                if(m_next_char <= 0 && m_target_text.size() > 0)
                {
                    m_text += m_target_text[0];
                    m_target_text.erase(m_target_text.begin());

                    m_next_char = 0.2;
                }
            }

            if(t >= 55 && t < 58)
                m_unlock_music.setVolume(current_save.volume * (58 - t) / 3);

            if(t >= 58 && !m_exit)
                m_exit = true;
        }

        void unlock_shorts_san_state::draw(sf::RenderTarget& target)
        {
            if(m_exit)
                return;

            sf::Sprite the_shorts(m_shorts);
            the_shorts.setPosition(228 / 2 - m_shorts.getSize().x / 2, (int)m_shorts_y);

            target.draw(the_shorts);

            sf::Text text(m_text, fonts::emulogic, 8);
            text.setPosition(228 / 2 - (int)(text.getLocalBounds().width / 2), 140);

            target.draw(text);

            double t = (total_time - start_time);
            if(t >= 55)
            {
                sf::RectangleShape cover({ 224, 288 });
                cover.setFillColor({ 0, 0, 0, (int)(255 * (1 - (58 - t) / 3)) });

                target.draw(cover);
            }
        }
    }
}
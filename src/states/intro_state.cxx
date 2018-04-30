#include "intro_state.h"
#include "insert_coin_state.h"
#include "../main.h"

#include <cmath>

namespace pacman
{
    namespace states
    {
        void intro_state::update(double delta, double total_time)
        {
            if(m_doremi_sprite.getPosition().x > 224 + 60 || m_wants_skip)
            {
                set_state(new insert_coin_state());
                return;
            }

            if(start_time == 0)
                start_time = total_time;

            if(total_time - start_time > 1)
                m_doremi_sprite.move(delta * 40, 0);

            if(m_doremi_sprite.getPosition().x > 224 + 20)
                return;

            if(m_doremi_sprite.getPosition().x + 2 >= m_intro_text.getPosition().x + m_goal)
            {
                std::string replace = m_intro_text.getString();
                replace[m_char] = ' ';
                m_intro_text.setString(replace);

                m_goal += 8;
                m_char++;

                if(m_char >= replace.length())
                    m_char = replace.length() - 1;
            }

            if(total_time - start_time > 1 && (int)((total_time - start_time) * 1000) % 300 > 150 && !m_last_was_over_150)
                m_chomp_snd.play();

            m_doremi_sprite.setTexture(data::characters::doremi.texture(m_last_was_over_150 = (int)((total_time - start_time) * 1000) % 300 > 150));
        }

        void intro_state::draw(sf::RenderTarget& target)
        {
            target.draw(m_intro_text);
            target.draw(m_doremi_sprite);
        }
    }
}
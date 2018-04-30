#include "credits_state.h"
#include "insert_coin_state.h"

#include <cmath>

namespace pacman
{
    namespace states
    {
        bool credits_state::on_sf_event(sf::Event& ev)
        {
            if(!m_exit && ev.type == sf::Event::KeyPressed)
            {
                if(ev.key.code == sf::Keyboard::Escape)
                {
                    m_exit = true;
                    m_click_snd.play();
                }
            }

            return false;
        }

        void credits_state::on_enter(base_state* last_state)
        {
            m_bg_music.play();
        }
        
        void credits_state::update(double delta, double total_time)
        {
            if(m_exit)
            {
                m_bg_music.stop();

                if(m_exit > 1.4)
                    pacman::set_state(new insert_coin_state());

                m_exit += delta;
            }

            for(std::size_t i = 0; i < m_credits.size(); i++)
            {
                // all text has scrolled into view
                if(i == m_credits.size() - 1 && m_credits[i].getPosition().y < 288 - 8 && m_current_post_num < max_post_num)
                {
                    std::string str = "";
                    for(int i2 = 0; i2 < (m_current_post_num >= 1000 ? 2 : 3); i2++)
                    {
                        if(i2 != 0)
                            str += " ";
                        str += ">>" + std::to_string(m_current_post_num++);

                        if(m_current_post_num >= max_post_num)
                            break;
                    }

                    m_credits.push_back(sf::Text(str, fonts::emulogic, 8));

                    m_credits[m_credits.size() - 1].setColor({ 120, 153, 34 });
                    m_credits[m_credits.size() - 1].setPosition(224 / 2 - m_credits[m_credits.size() - 1].getLocalBounds().width / 2, m_credits[m_credits.size() - 2].getPosition().y + 9);
                }

                if(m_current_post_num == max_post_num)
                {
                    m_credits.push_back(sf::Text("and more!", fonts::emulogic, 8));

                    m_credits[m_credits.size() - 1].setColor({ 0, 20, 128 });
                    m_credits[m_credits.size() - 1].setPosition(224 / 2 - m_credits[m_credits.size() - 1].getLocalBounds().width / 2, m_credits[m_credits.size() - 2].getPosition().y + 9);

                    m_current_post_num++;
                }

                if(m_credits[i].getPosition().y < -8)
                    m_credits.erase(m_credits.begin() + i);
            }

            for(std::size_t i = 0; i < m_credits.size(); i++)
                m_credits[i].setPosition(224 / 2 - m_credits[i].getLocalBounds().width / 2, m_credits[i].getPosition().y - (delta * 8 * (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ? 2 : 1)));

            if(m_next_hadzuki_fade > 3)
            {
                m_current_hadzuki = m_next_hadzuki;

                srand(m_current_post_num ^ time(nullptr));
                m_next_hadzuki.loadFromFile("data/credits/" + std::to_string((rand() % 34) + 1) + ".jpg");
                m_next_hadzuki.setSmooth(true);

                m_next_hadzuki_fade = 0;
            }

            if(m_current_post_num > 100)
                m_next_hadzuki_fade += delta * 0.5;
        }

        void credits_state::draw(sf::RenderTarget& target)
        {
            if(m_exit)
                return;

            float scale = std::max(224.0 / m_current_hadzuki.getSize().x, 288.0 / m_current_hadzuki.getSize().y);

            sf::Sprite current_hadzuki(m_current_hadzuki);
            current_hadzuki.setPosition(224 / 2, 288 / 2);
            current_hadzuki.setOrigin({ m_current_hadzuki.getSize().x / 2 - 30, m_current_hadzuki.getSize().y / 2 });
            current_hadzuki.setScale(scale, scale);
            current_hadzuki.setColor({ 255, 255, 255, 255 });

            target.draw(current_hadzuki);

            scale = std::max(224.0 / m_next_hadzuki.getSize().x, 288.0 / m_next_hadzuki.getSize().y);

            sf::Sprite next_hadzuki(m_next_hadzuki);
            next_hadzuki.setPosition(224 / 2, 288 / 2);
            next_hadzuki.setScale(scale, scale);
            next_hadzuki.setOrigin({ m_next_hadzuki.getSize().x / 2 - 30, m_next_hadzuki.getSize().y / 2 });
            next_hadzuki.setColor({ 255, 255, 255, (int)(255 * std::min(1.0f, m_next_hadzuki_fade)) });

            target.draw(next_hadzuki);

            sf::RectangleShape black({ 224, 288 });
            black.setFillColor({ 0, 0, 0, 255 - 20 });

            target.draw(black);

            for(sf::Text& text : m_credits)
            {
                float last_y = text.getPosition().y;
                text.setPosition((int)text.getPosition().x, std::floor(text.getPosition().y));

                target.draw(text);

                text.setPosition(text.getPosition().x, last_y);
            }
        }
    }
}
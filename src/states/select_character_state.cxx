#include "select_character_state.h"
#include "insert_coin_state.h"

#include <cmath>

namespace pacman
{
    namespace states
    {
        bool select_character_state::on_sf_event(sf::Event& ev)
        {
            if(ev.type == sf::Event::KeyPressed)
            {
                if(!m_exit)
                {
                    if(ev.key.code == sf::Keyboard::Left && m_selected > 0 && m_left_arrow_hide <= 0 && m_right_arrow_hide <= 0)
                    {
                        m_selected--;

                        m_last_selection_time = total_time;
                        m_left_arrow_hide = 0.2;
                    }

                    if(ev.key.code == sf::Keyboard::Right && m_selected < data::characters::SIZE_CHARACTER - 1 && m_left_arrow_hide <= 0 && m_right_arrow_hide <= 0)
                    {
                        m_selected++;

                        m_last_selection_time = total_time;
                        m_right_arrow_hide = 0.2;
                    }

                    if(ev.key.code == sf::Keyboard::Return)
                    {
                        if(!current_save.unlocked[m_selected] && 
                            current_save.nicemonye != -1 && 
                            current_save.nicemonye >= data::characters::enum_to_info(static_cast<data::characters::character_enum>(m_selected)).cost)
                        {
                            current_save.unlocked[m_selected] = true;
                            current_save.nicemonye -= data::characters::enum_to_info(static_cast<data::characters::character_enum>(m_selected)).cost;

                            m_nicemonye_text.setString(std::to_string(current_save.nicemonye));
                            m_nicemonye_text.setPosition({ 
                                (int)(224 - m_nicemonye_text.getLocalBounds().width - 10), 
                                (int)(288 - m_nicemonye_text.getLocalBounds().height - 10), 
                            });
                        }

                        if(current_save.unlocked[m_selected] && current_save.selected_character != m_selected)
                        {
                            current_save.selected_character = static_cast<data::characters::character_enum>(m_selected);
                            m_click_snd.play();
                        }
                    }

                    if(ev.key.code == sf::Keyboard::Escape)
                    {
                        m_exit_state = new insert_coin_state();
                        m_exit = true;

                        m_click_snd.play();
                    }
                }
            }

            return false;
        }

        void select_character_state::update(double delta, double total_time)
        {
            if(start_time == 0)
            {
                start_time = total_time;
                m_last_selection_time = total_time;
            }

            this->total_time = total_time;

            if(m_exit && m_bg_music.getStatus() == sf::SoundSource::Playing)
                m_bg_music.stop();

            if(m_exit)
            {
                if(m_exit > 1.4)
                    pacman::set_state(m_exit_state);

                m_exit += delta;
            }

            m_selected_display = m_selected_display + ((m_selected * 224) - m_selected_display) * delta * 12;

            m_left_arrow_hide -= delta;
            m_right_arrow_hide -= delta;
        }

        void select_character_state::draw(sf::RenderTarget& target)
        {
            if(m_exit)
                return;

            const data::characters::info& selected_info = data::characters::enum_to_info(static_cast<data::characters::character_enum>(m_selected));

            for(int i = 0; i < data::characters::SIZE_CHARACTER; i++)
            {
                const data::characters::info& info = data::characters::enum_to_info(static_cast<data::characters::character_enum>(i));
                bool is_unknown = i == data::characters::SHORTS && !current_save.unlocked[data::characters::SHORTS];
                sf::Sprite character(is_unknown ? m_unknown_texture : info.texture(0));

                character.setPosition((int)-m_selected_display + (224 * i) + 224 / 2 - character.getTexture()->getSize().x / 2, 60);

                if(current_save.selected_character == i ||
                    is_unknown ||
                    (!current_save.unlocked[i] && current_save.nicemonye < info.cost) ||
                    (int)((total_time - m_last_selection_time) * 1000) % 600 > 300 || 
                    std::abs(m_selected_display - (m_selected * 224)) > 1)
                {
                    target.draw(character);
                }

                sf::Text name(is_unknown ? "????????" : info.name, fonts::emulogic, 8);
                name.setPosition((int)-m_selected_display + (224 * i) + 224 / 2 - (int)name.getLocalBounds().width / 2, 100);

                target.draw(name);

                sf::Text description(is_unknown ? "it is a mystery" : info.description, fonts::emulogic, 8);

                description.setPosition((int)-m_selected_display + (224 * i) + 224 / 2 - (int)description.getLocalBounds().width / 2, 110);
                description.setColor({ 255, 255, 255, 140 });

                target.draw(description);
            }

            if(m_selected > 0 && m_left_arrow_hide <= 0)
            {
                sf::Sprite left_arrow(m_arrow_texture);

                left_arrow.setScale(-1, 1);
                left_arrow.setPosition(12 + m_arrow_texture.getSize().x, 62);

                target.draw(left_arrow);
            }

            if(m_selected < data::characters::SIZE_CHARACTER - 1 && m_right_arrow_hide <= 0)
            {
                sf::Sprite right_arrow(m_arrow_texture);
                right_arrow.setPosition(224 - 12 - m_arrow_texture.getSize().x, 62);

                target.draw(right_arrow);
            }

            target.draw(m_nicemonye_text);

            sf::Sprite nicemonye(m_nicemonye_texture);
            nicemonye.setPosition(m_nicemonye_text.getPosition().x - m_nicemonye_texture.getSize().x - 6, m_nicemonye_text.getPosition().y);

            target.draw(nicemonye);

            if(std::abs(m_selected_display - (m_selected * 224)) > 10)
                return;

            if(m_selected == data::characters::SHORTS && !current_save.unlocked[data::characters::SHORTS])
                return;

            for(int i = 0; i < 3; i++)
            { 
                float value = 1;
                std::string label = "";
                std::string percent = "";

                switch(i)
                {
                    case 0:
                        value = selected_info.multipliers.speed;
                        label = "Speed";
                        percent = std::to_string((int)(selected_info.multipliers.speed * 100)) + "%";
                        break;

                    case 1:
                        value = selected_info.multipliers.pts;
                        label = "Points";
                        percent = std::to_string((int)(selected_info.multipliers.pts * 100)) + "%";
                        break;

                    case 2:
                        value = selected_info.multipliers.blue_mode;
                        label = "Blue mode";
                        percent = std::to_string((int)(selected_info.multipliers.blue_mode * 100)) + "%";
                        break;
                }

                sf::RectangleShape outline({ 140, 10 });
                outline.setPosition(224 / 2 - 140 / 2, 288 / 2 - 10 / 2 + 40 + 26 * i + 12);
                outline.setOutlineColor(sf::Color::White);
                outline.setOutlineThickness(1);
                outline.setFillColor({ 0, 0, 0, 0 });

                target.draw(outline);

                sf::RectangleShape multiplier({ (int)(140 * (value / 2)), 10 });
                multiplier.setPosition(outline.getPosition().x, outline.getPosition().y);
                multiplier.setFillColor({ 255, 0, 0, 255 });

                target.draw(multiplier);

                multiplier.setFillColor({ 0, 255, 0, (int)(255 * (value / 2)) });
                target.draw(multiplier);

                sf::Text label_text(label, fonts::emulogic, 8);

                label_text.setPosition(outline.getPosition().x + 2, outline.getPosition().y - 12);
                label_text.setColor({ 255, 255, 255, 180 });

                target.draw(label_text);

                sf::Text percent_text(percent, fonts::emulogic, 8);

                percent_text.setPosition(outline.getPosition().x + 140 - 4 - percent_text.getLocalBounds().width, outline.getPosition().y  - 12);
                percent_text.setColor({ 255, 255, 255, 120 });

                target.draw(percent_text);
            }

            if(!current_save.unlocked[m_selected])
            {
                sf::Text text(current_save.nicemonye >= selected_info.cost ? "        Buy (" + std::to_string(selected_info.cost) + ")" : "Too expensive! (" + std::to_string(selected_info.cost) + ")", fonts::emulogic, 8);
                text.setPosition((int)(224 / 2 - text.getLocalBounds().width / 2), (int)(288 / 2 - text.getLocalBounds().height / 2) + 8);

                target.draw(text);

                if(current_save.nicemonye >= selected_info.cost)
                {
                    sf::Text text_color("[Enter]", fonts::emulogic, 8);
                    text_color.setPosition((int)(224 / 2 - text.getLocalBounds().width / 2), (int)(288 / 2 - text.getLocalBounds().height / 2) + 8);
                    text_color.setColor({ 80, 188, 70, 255 });

                    target.draw(text_color);
                }
            }
            else if(m_selected != current_save.selected_character)
            {
                sf::Text text("        Select", fonts::emulogic, 8);
                text.setPosition((int)(224 / 2 - text.getLocalBounds().width / 2), (int)(288 / 2 - text.getLocalBounds().height / 2) + 8);

                target.draw(text);
                
                sf::Text text_color("[Enter]", fonts::emulogic, 8);
                text_color.setPosition((int)(224 / 2 - text.getLocalBounds().width / 2), (int)(288 / 2 - text.getLocalBounds().height / 2) + 8);
                text_color.setColor({ 80, 188, 70, 255 });

                target.draw(text_color);
            }
        }
    }
}
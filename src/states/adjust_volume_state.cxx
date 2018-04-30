#include "adjust_volume_state.h"
#include "intro_state.h"
#include "insert_coin_state.h"

#include <cmath>

namespace pacman
{
    namespace states
    {
        bool adjust_volume_state::on_sf_event(sf::Event& ev)
        {
            if(ev.type == sf::Event::KeyPressed)
            {
                if(ev.key.code == sf::Keyboard::Left && current_save.volume > 0)
                {
                	if(current_save.volume <= 10)
                		current_save.volume--;
                	else
                    	current_save.volume -= 5;
                    m_shake = 2 + (8 * current_save.volume / 100.0);

                    m_dodo_sound.setVolume(current_save.volume);
                    m_dodo_sound.play();
                }
                
                if(ev.key.code == sf::Keyboard::Right && current_save.volume < 100)
                {
                	if(current_save.volume < 10)
                		current_save.volume++;
                	else
                    	current_save.volume += 5;
                    m_shake = 2 + (8 * current_save.volume / 100.0);

                    m_dodo_sound.setVolume(current_save.volume);
                    m_dodo_sound.play();
                }

                if(!m_exit)
                {
                    if(ev.key.code == sf::Keyboard::Return)
                    {
                        m_exit = true;
                        m_click_snd.play();
                    }
                }
            }

            return false;
        }

        void adjust_volume_state::update(double delta, double total_time)
        {
            if(start_time == 0)
                start_time = total_time;

            this->total_time = total_time;

            m_shake -= delta * 16;
            
            if(m_shake < 0)
                m_shake = 0;

            if(m_exit)
            {
                if(m_exit > 1.4)
                    set_state(m_intro_after ? static_cast<base_state*>(new intro_state()) : static_cast<base_state*>(new insert_coin_state()));

                m_exit += delta;
            }
        }

        void adjust_volume_state::draw(sf::RenderTarget& target)
        {
            if(m_exit)
                return;
                
            srand(time(nullptr) + std::ceil(m_shake * 10000));

            int move_x = m_shake == 0 ? 0 : (rand() % (int)std::ceil(m_shake * 2)) - std::ceil(m_shake);
            int move_y = m_shake == 0 ? 0 : (rand() % (int)std::ceil(m_shake * 2)) - std::ceil(m_shake);

            sf::RectangleShape outline({ 100, 10 });
            outline.setPosition(224 / 2 - 100 / 2, 288 / 2 - 10 / 2 + 10);
            outline.setOutlineColor(sf::Color::White);
            outline.setOutlineThickness(1);
            outline.setFillColor({ 0, 0, 0, 0 });

            outline.move(move_x, move_y);

            target.draw(outline);

            sf::RectangleShape volume({ (int)(100 * (current_save.volume / 100.0)), 10 });
            volume.setPosition(outline.getPosition().x, outline.getPosition().y);
            volume.setFillColor({ 0, 255, 0, 255 });

            target.draw(volume);

            volume.setFillColor({ 255, 0, 0, (int)(current_save.volume / 100.0 * 255.0) });

            target.draw(volume);

            sf::Text volume_text(current_save.volume == 0 ? "Mute" : std::to_string((int)current_save.volume) + "%", fonts::emulogic, 8);
            volume_text.setPosition(224 / 2 - (int)(volume_text.getLocalBounds().width / 2), 288 / 2 - (int)(volume_text.getLocalBounds().height / 2) - 10);
            volume_text.move(0, move_y / 2);

            volume_text.setColor({ 255, 255, 255, 120 });

            target.draw(volume_text);

            if((int)((total_time + 2 - start_time) * 1000) % 1200 > 600)
            {
                sf::Text title("Adjust volume", fonts::emulogic, 8);
                title.setPosition(224 / 2 - (int)(title.getLocalBounds().width / 2), 288 / 2 - (int)(title.getLocalBounds().height / 2) - 40);
                title.move(0, move_y / 6);

                target.draw(title);

                if(m_intro_after)
                {
                    sf::Text start_game("Press ENTER when done", fonts::emulogic, 8);
                    start_game.setPosition(224 / 2 - (int)(start_game.getLocalBounds().width / 2), 288 / 2 - (int)(start_game.getLocalBounds().height / 2) + 40);
                    start_game.move(0, move_y / 6);

                    target.draw(start_game);
                }
            }
        }
    }
}

#include "death_state.h"
#include "../../states/game_over_state.h"

#include <cmath>

namespace pacman
{
    namespace gameplay
    {
        const static sf::Vector2i froge_center_point = { 6, 9 };

        // TODO: the original plan was to turn all the ojamajos into magic frogs instead of just blink + kapoof

        void death_state::update(double delta, double total_time)
        {
            if(start_time == 0)
                start_time = total_time;

            this->total_time = total_time;

            if(m_exit == 0 && (total_time - start_time) > 1.8)
                m_exit = 1;

            if(m_exit)
            {
                if(m_exit > 1.2)
                {
                    // NO extra lives left
                    if(parent.player.extra_lives - 1 < 0)
                    {
                        pacman::set_state(new states::game_over_state(parent.level));
                        return;
                    }
                    else
                    {
                        states::gameplay_state* new_state = new states::gameplay_state(parent.level, parent.map);
                        new_state->player.extra_lives = parent.player.extra_lives - 1;

                        new_state->player.pts = parent.player.pts;
                        new_state->player.pts_until_1up = parent.player.pts_until_1up;

                        new_state->player.dots_eaten = parent.player.dots_eaten;
                        new_state->player.dots_left = parent.player.dots_left;
                        
                        pacman::set_state(new_state);
                        return;
                    }
                }
                    
                m_exit += delta;
            }
        }

        void death_state::draw(sf::RenderTarget& target)
        {
            if(m_exit)
                target.clear();

            if((total_time - start_time) > 1.3)
                return;

            if((total_time - start_time) > 1.1)
            {
                sf::Vector2i pos = 
                    sf::Vector2i { parent.player.hitbox_on_map().left, parent.player.hitbox_on_map().top } +
                    sf::Vector2i { 0, map::top_bottom_border } +
                    sf::Vector2i { player::size / 2 - m_die_texture.getSize().x / 2, player::size / 2 - m_die_texture.getSize().y / 2 };
                
                sf::Sprite spr(m_die_texture);
                spr.setPosition(sf::Vector2f(pos));

                target.draw(spr);
            }
            else if(std::sin(std::pow(((total_time + 0.8) - start_time) * 1.5, 3.8)) > 0)
                parent.player.draw(target);
        }
    }
}
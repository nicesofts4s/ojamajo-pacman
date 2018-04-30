#include "win_state.h"

namespace pacman
{
    namespace gameplay
    {
        void win_state::update(double delta, double total_time)
        {
            if(start_time == 0)
                start_time = total_time;

            this->total_time = total_time;

            if(m_exit == 0 && (total_time - start_time) > 3)
                m_exit = 1;

            if(m_exit)
            {
                if(m_exit > 1.2)
                {
                    states::gameplay_state* new_state = new states::gameplay_state(parent.level + 1);

                    new_state->player.extra_lives = parent.player.extra_lives;
                    new_state->player.pts = parent.player.pts;
                    new_state->player.pts_until_1up = parent.player.pts_until_1up;

                    pacman::set_state(new_state);
                    return;
                }
                    
                m_exit += delta;
            }
        }

        void win_state::draw(sf::RenderTarget& target)
        {
            if(m_exit)
            {
                target.clear();
                return;
            }

            if((int)((total_time - start_time) * 1000) % 1000 <= 500)
            {
                sf::Sprite layout(m_layout_texture);
                layout.setColor({ 245, 255, 245, 255 });

                target.draw(layout);
            }

            parent.player.draw(target);
        }
    }
}
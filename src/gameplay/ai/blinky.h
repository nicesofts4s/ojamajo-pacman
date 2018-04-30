#pragma once

#include "../ghost.h"

namespace pacman
{
    namespace gameplay
    {
        namespace ai
        {
            // the red ghost
            class blinky : public base_ai_ghost
            {
public:
                blinky()
                {
                    current_tile = start_tile();
                    direction = start_direction();
                    
                    tile_offset.x = 0.5;
                }

                sf::Vector2i start_tile() const override { return { 13, 11 }; };
                sf::Vector2i start_direction() const override { return { -1, 0 }; }

                sf::Vector2i home_corner() const override { return { 1, -1 }; }

                // blinky is the only ghost that can (potentially) be faster than pacguy
                float speed() const override 
                {                         
                    return 8.35 * (1 + (0.05 * m_aggressiveness)); 
                }

                bool can_exit_house(int dots_eaten) const override
                {
                    return true;
                }

                int texture_row() const override { return 0; }

                void update_chase(states::gameplay_state& gp_state) override;
                void update_scatter(states::gameplay_state& gp_state) override;
                
                void update_eaten(states::gameplay_state& gp_state) override
                { 
                	target_tile = { 14, 14 };
                    if(current_tile.x == target_tile.x && current_tile.y == target_tile.y && behavior() == behavior_mode::eaten)
                		transition_behavior(behavior_mode::chase);
             	}
private:
                int m_aggressiveness = 0;
            };
        }
    }
}

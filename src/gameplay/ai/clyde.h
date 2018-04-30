#pragma once

#include "../ghost.h"

namespace pacman
{
    namespace gameplay
    {
        namespace ai
        {
            // the yellow ghost
            class clyde : public base_ai_ghost
            {
public:
                clyde()
                {
                    current_tile = start_tile();
                    direction = start_direction();
                    
                    tile_offset.x = 0.5;
                }

                sf::Vector2i start_tile() const override { return { 15, 14 }; };
                sf::Vector2i start_direction() const override { return { 0, -1 }; }

                sf::Vector2i home_corner() const override { return { -1, 1 }; }

                float speed() const override { return 7.85; };

                bool can_exit_house(int dots_eaten) const override
                {
                    return dots_eaten >= 30 + 60;
                }

                int texture_row() const override { return 3; }

                void update_chase(states::gameplay_state& gp_state) override;
            };
        }
    }
}
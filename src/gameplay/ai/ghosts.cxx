#include "all.h"

#include "../states/play_state.h"

namespace pacman
{
    namespace gameplay
    {
        namespace ai
        {
            void blinky::update_chase(states::gameplay_state& gp_state)
            {
                // very simple!
                target_tile = gp_state.player.current_tile;
            }

            void blinky::update_scatter(states::gameplay_state& gp_state)
            {
                if(gp_state.player.dots_left <= 20)
                    m_aggressiveness = 1;
                if(gp_state.player.dots_left <= 10)
                    m_aggressiveness = 2;

                if(m_aggressiveness)
                    update_chase(gp_state);
                else
                    base_ai_ghost::update_scatter(gp_state);
            }

            void pinky::update_chase(states::gameplay_state& gp_state)
            {
                // also very simple!
                // (in the original pacman there's a bug which causes targeting to go
                // four tiles ahead AND four tiles to the left if pacman is facing up
                // however, since [s4s] encourages high-quality contributions, keeping
                // a bug like this would just be wrong!!!!)
                target_tile = gp_state.player.current_tile + (gp_state.player.direction * 4);
            }

            void inky::update_chase(states::gameplay_state& gp_state)
            {
                base_ai_ghost& blinky_fren = *static_cast<play_state*>(&gp_state.sub_state())->ghosts()[0];
                sf::Vector2i start_pos = gp_state.player.current_tile + (gp_state.player.direction * 2);
                
                target_tile = (start_pos - blinky_fren.current_tile) * 2;
            }

            void clyde::update_chase(states::gameplay_state& gp_state)
            {
                if(std::max(std::abs(gp_state.player.current_tile.x - current_tile.x), std::abs(gp_state.player.current_tile.y - current_tile.y)) > 8)
                    target_tile = gp_state.player.current_tile;
                else
                    update_scatter(gp_state);
            }
        }
    }
}

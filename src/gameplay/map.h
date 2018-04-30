#pragma once

#include <cstdint>

#include "data/layouts.h"

namespace pacman
{
    namespace gameplay
    {
        struct map
        {
            constexpr static std::uint8_t tile_count_x = 28;
            constexpr static std::uint8_t tile_count_y = 31;
            // 8x8
            constexpr static std::uint8_t tile_size = 8;

            constexpr static std::uint8_t top_bottom_border = 20;

            char pellets[tile_count_x * tile_count_y];
            char passable_tiles[tile_count_x * tile_count_y];
            char ghost_passable_tiles[tile_count_x * tile_count_y];

            char& pellet_at_xy(std::uint8_t x, std::uint8_t y) 
            { 
                static char empty = ' ';
                if(x < 0 || y < 0 || x >= tile_count_x || y >= tile_count_y)
                    return empty;

                return pellets[(y * tile_count_x) + x]; 
            }

            const char& pellet_at_xy(std::uint8_t x, std::uint8_t y) const
            { 
                static const char empty = ' ';
                if(x < 0 || y < 0 || x >= tile_count_x || y >= tile_count_y)
                    return empty;

                return pellets[(y * tile_count_x) + x]; 
            }

            bool can_pass_xy(int x, int y, bool use_ghost_layout = false) const
            { 
                // to allow wrapping
                if((x < 0 || x > tile_count_x - 1))
                    return y == 14;

                return use_ghost_layout ? ghost_passable_tiles[(y * tile_count_x) + x] == 'x' || ghost_passable_tiles[(y * tile_count_x) + x] == 'X' : passable_tiles[(y * tile_count_x) + x] == 'x'; 
            }

            bool is_intersection(int x, int y, bool use_ghost_layout = false) const
            {
                bool can_go_left = can_pass_xy(x - 1, y, use_ghost_layout);
                bool can_go_right = can_pass_xy(x + 1, y, use_ghost_layout);

                bool can_go_up = can_pass_xy(x, y - 1, use_ghost_layout);
                bool can_go_down = can_pass_xy(x, y  + 1, use_ghost_layout);

                if(can_go_left && can_go_right)
                    return can_go_up || can_go_down;

                if(can_go_up && can_go_down)
                    return can_go_left || can_go_right;

                return false;
            }

            bool is_side_tunnel(int x, int y) const
            {
                if((x < 6 || x > tile_count_x - 7))
                    return y == 14;
                    
                return false;
            }

            bool is_ghost_forbidden_updown_zone(int x, int y) const
            {
                if((x < 0 || x > tile_count_x - 1))
                    return false;

                return ghost_passable_tiles[(y * tile_count_x) + x] == 'X'; 
            }
        };
    }
}

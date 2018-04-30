#pragma once

#include <vector>
#include <limits>
#include <cstdint>

#include "../ghost.h"

namespace pacman
{
    namespace data
    {
        struct _level_multipliers
        {
            // level range
            std::uint64_t start, end;

            double player_norm_speed, player_frightened_speed;
            double ghost_norm_speed, ghost_frightened_speed;
        };

        constexpr _level_multipliers level_multipliers[] = {
            // Level 1
            { 
                1, 1,
                0.8, 0.9,
                0.75, 0.5
            },
            // 2-4
            {
                2, 4,
                0.9, 0.95,
                0.85, 0.55
            },
            // 5-20
            {
                5, 20,
                1, 1,
                0.95, 0.6
            },
            // 21++
            {
                21, std::numeric_limits<std::uint64_t>::max(),
                0.9, /* will never happen............... */ 0.9,
                0.95, 0.95
            }
        };

        inline const _level_multipliers& find_level_multipliers(std::uint64_t level)
        {
            for(int i = 0; i < 3; i++)
            {
                if(level_multipliers[i].start <= level && level_multipliers[i].end >= level)
                    return level_multipliers[i];
            }

            return level_multipliers[3];
        }

        struct _level_ai_behavior_cycles
        {
            std::uint64_t start, end;

            // there are 8 cycles in a game
            // scatter -> chase -> scatter -> etc...
            // -1 = yosho shifts timeline
            double times[8];

            inline const gameplay::base_ai_ghost::behavior_mode mode_for_i(int i) const
            {
                return i % 2 == 0 ? gameplay::base_ai_ghost::behavior_mode::scatter : gameplay::base_ai_ghost::behavior_mode::chase;
            }
        };

        constexpr _level_ai_behavior_cycles level_ai_behavior_cycles[] = {
            // Level 1
            { 
                1, 1,
                {
                    7,
                    20,
                    7,
                    20,
                    5,
                    20,
                    5,
                    -1
                }
            },
            // Level 2-4
            { 
                2, 4,
                {
                    7,
                    20,
                    7,
                    20,
                    5,
                    1033,
                    1.0 / 60.0,
                    -1
                }
            },
            // Level 5+
            { 
                5, std::numeric_limits<std::uint64_t>::max(),
                {
                    7,
                    20,
                    7,
                    20,
                    5,
                    1033,
                    1.0 / 60.0,
                    -1
                }
            }
        };

        inline const _level_ai_behavior_cycles& find_level_ai_cycles(std::uint64_t level)
        {
            for(int i = 0; i < 2; i++)
            {
                if(level_multipliers[i].start <= level && level_multipliers[i].end >= level)
                    return level_ai_behavior_cycles[i];
            }

            return level_ai_behavior_cycles[2];
        }
    }
}
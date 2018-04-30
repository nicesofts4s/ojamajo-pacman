#include "ghost.h"
#include "data/gameplay.h"

#include "states/play_state.h"

#include <iostream>

namespace pacman
{
    namespace gameplay
    {
        std::unique_ptr<sf::Texture> base_ai_ghost::m_texture, base_ai_ghost::m_frightened_texture, base_ai_ghost::m_eyes_texture;

        void base_ai_ghost::update(double delta, states::gameplay_state& gp_state)
        {
            m_animation_should_blink = static_cast<play_state*>(&gp_state.sub_state())->frighten_time_left() < 2;

            if(!can_exit_house(gp_state.player.dots_eaten))
            {
                m_animation_advance += delta * (m_behavior == behavior_mode::frightened ? 1 : 0.5) /* this is needed to ensure the ghosts blinking sync up near the end */;

                if(current_tile.x == 11 || current_tile.x == 12)
                    direction = { 1, 0 };
                else
                    direction = { -1, 0 };
                    
                return;
            }

            m_animation_advance += delta;

            if(gp_state.map.can_pass_xy(current_tile.x + direction.x, current_tile.y + direction.y, true))
            {
                double spd = speed();
                spd *= (gp_state.map.is_side_tunnel(current_tile.x, current_tile.y) ? 0.8f : 1.0f);
                spd *= (gp_state.map.can_pass_xy(current_tile.x, current_tile.y, true) && !gp_state.map.can_pass_xy(current_tile.x, current_tile.y) ? 0.6f : 1.0f);
                
                if(m_behavior == behavior_mode::frightened)
                    spd *= data::find_level_multipliers(gp_state.level).ghost_frightened_speed;
                else
                    spd *= data::find_level_multipliers(gp_state.level).ghost_norm_speed;

                if(m_behavior == behavior_mode::eaten)
                    spd = 12;

                tile_offset += sf::Vector2f(direction) * (float)spd * (float)delta;
            }
            else if(gp_state.map.can_pass_xy(current_tile.x, current_tile.y, true))
            {
            	// quick fix (works) (!! every time !!)
                // if we're stuck as a ghost (which should always find its way around the maze) we're most likely glitched
                // this only applies in the playable area
                // TODO: this problelly happens when direction is reversed in transition_behavior
                //tile_offset.x = 0; // reset
                //tile_offset.y = 0; // reset

                direction *= -1;
            }
            
            sf::Vector2i last_tile = current_tile;
            bool do_pathfind = false;

            // see player.cxx for more details on movement/collision
            if(tile_offset.x <= -1 || tile_offset.x >= 1)
            {
                current_tile.x += (int)tile_offset.x;

                if(gp_state.map.can_pass_xy(current_tile.x + direction.x, current_tile.y + direction.y, true))
                {
                    if(tile_offset.x >= 1)
                        tile_offset.x -= 1;
                    if(tile_offset.x <= -1)
                        tile_offset.x += 1;
                }
                else
                    tile_offset.x = 0;

                do_pathfind = true;

                if(current_tile.x < -3)
                    current_tile.x = map::tile_count_x + 3;
                if(current_tile.x > map::tile_count_x + 3)
                    current_tile.x = -3;
            }

            if(tile_offset.y <= -1 || tile_offset.y >= 1)
            {
                current_tile.y += (int)tile_offset.y;

                if(gp_state.map.can_pass_xy(current_tile.x + direction.x, current_tile.y + direction.y, true))
                {
                    if(tile_offset.y >= 1)
                        tile_offset.y -= 1;
                    if(tile_offset.y <= -1)
                        tile_offset.y += 1;
                }
                else
                    tile_offset.y = 0;

                do_pathfind = true;

                if(current_tile.y < -3)
                    current_tile.y = map::tile_count_y + 3;
                if(current_tile.y > map::tile_count_y + 3)
                    current_tile.y = -3;
            }

            switch(behavior())
            {
                case behavior_mode::chase:
                    update_chase(gp_state);
                    break;

                case behavior_mode::scatter:
                    update_scatter(gp_state);
                    break;

                case behavior_mode::frightened:
                    update_frightened(gp_state);
                    break;

                case behavior_mode::eaten:
                    update_eaten(gp_state);
                    break;
            }

            // special behavior inside the ~spook house~
            if(gp_state.map.can_pass_xy(current_tile.x, current_tile.y, true) && 
                !gp_state.map.can_pass_xy(current_tile.x, current_tile.y, false) &&
                m_behavior != behavior_mode::eaten)
            {
                if(current_tile.x == 11 || current_tile.x == 12)
                    direction = { 1, 0 };
                if(current_tile.x == 13 && (std::abs(tile_offset.x - 0.5) < 0.03 || current_tile.y <= 13))
                {
                    direction = { 0, -1 };
                    tile_offset.x = 0.5;
                }
                else if(current_tile.x == 13)
                    direction = { 1, 0 };
                if(current_tile.x == 14 && (std::abs(tile_offset.x + 0.5) < 0.03 || current_tile.y <= 13))
                {
                    direction = { 0, -1 };
                    tile_offset.x = -0.5;
                }
                else if(current_tile.x == 14)
                    direction = { -1, 0 };
                if(current_tile.x == 15 || current_tile.x == 16)
                    direction = { -1, 0 };
            }
            else if((current_tile.x == 13 || current_tile.x == 14) && last_tile.y == 12 && current_tile.y == 11)
            {
                // ghosts should always go left after exiting the ~spook house~
                direction = { -1, 0 };
            }
            else if(do_pathfind)
            {
                int min_distance_encountered = -1;
                sf::Vector2i new_direction = direction;

                if(gp_state.map.can_pass_xy(current_tile.x, current_tile.y - 1, true) && 
                    direction != sf::Vector2i { 0, 1 } && 
                    (m_behavior == behavior_mode::eaten || !gp_state.map.is_ghost_forbidden_updown_zone(current_tile.x, current_tile.y)))
                {
                    sf::Vector2i this_to_target = target_tile - (current_tile + sf::Vector2i { 0, -1 });
                    int distance = std::max(std::abs(this_to_target.x), std::abs(this_to_target.y));

                    if(min_distance_encountered == -1 || distance < min_distance_encountered)
                    {
                        min_distance_encountered = distance;
                        new_direction = { 0, -1 };
                    }
                }

                if(gp_state.map.can_pass_xy(current_tile.x + 1, current_tile.y, true) && direction != sf::Vector2i { -1, 0 })
                {
                    sf::Vector2i this_to_target = target_tile - (current_tile + sf::Vector2i { 1, 0 });
                    int distance = std::max(std::abs(this_to_target.x), std::abs(this_to_target.y));

                    if(min_distance_encountered == -1 || distance < min_distance_encountered)
                    {
                        min_distance_encountered = distance;
                        new_direction = { 1, 0 };
                    }
                }

                if(gp_state.map.can_pass_xy(current_tile.x, current_tile.y  + 1, true) && 
                    direction != sf::Vector2i { 0, -1 } && 
                    (m_behavior == behavior_mode::eaten || !gp_state.map.is_ghost_forbidden_updown_zone(current_tile.x, current_tile.y)))
                {
                    sf::Vector2i this_to_target = target_tile - (current_tile + sf::Vector2i { 0, 1 });
                    int distance = std::max(std::abs(this_to_target.x), std::abs(this_to_target.y));

                    if(min_distance_encountered == -1 || distance < min_distance_encountered)
                    {
                        min_distance_encountered = distance;
                        new_direction = { 0, 1 };
                    }
                }

                if(gp_state.map.can_pass_xy(current_tile.x - 1, current_tile.y, true) && direction != sf::Vector2i { 1, 0 })
                {
                    sf::Vector2i this_to_target = target_tile - (current_tile + sf::Vector2i { -1, 0 });
                    int distance = std::max(std::abs(this_to_target.x), std::abs(this_to_target.y));

                    if(min_distance_encountered == -1 || distance < min_distance_encountered)
                    {
                        min_distance_encountered = distance;
                        new_direction = { -1, 0 };
                    }
                }

                if(direction != new_direction)
                {
                    direction = new_direction;

                    tile_offset.x = 0;
                    tile_offset.y = 0;
                }
            }
        }

        void base_ai_ghost::update_scatter(states::gameplay_state& gp_state)
        {
            if(home_corner().x == -1)
                target_tile.x = home_corner().y == -1 ? 2 : 0;
            else
                target_tile.x = map::tile_count_x - (home_corner().y == -1 ? 2 : 0) - 1;

            if(home_corner().y == -1)
                target_tile.y = -4;
            else
                target_tile.y = map::tile_count_y + 3;
        }

        void base_ai_ghost::update_frightened(states::gameplay_state& gp_state)
        {
            srand(rand() + time(nullptr));
            target_tile = { rand() % map::tile_count_x, rand() % map::tile_count_y };
        }

        void base_ai_ghost::transition_behavior(behavior_mode new_mode)
        {
            behavior_mode old_behavior = m_behavior;
            m_behavior = new_mode;

            if((old_behavior == behavior_mode::chase || old_behavior == behavior_mode::scatter) && m_behavior != old_behavior)
                direction *= -1;

            if(m_behavior == behavior_mode::frightened)
                m_animation_advance = 0;
        }

        void base_ai_ghost::draw(sf::RenderTarget& target)
        {
            sf::Sprite ghost(behavior() == behavior_mode::eaten ? *m_eyes_texture : m_behavior == behavior_mode::frightened ? *m_frightened_texture : *m_texture);

            if(behavior() == behavior_mode::eaten)
            {
                if(direction == sf::Vector2i { 1, 0 })
                    ghost.setTextureRect({ 0, 0, size, size });
                if(direction == sf::Vector2i { -1, 0 })
                    ghost.setTextureRect({ size, 0, size, size });
                if(direction == sf::Vector2i { 0, -1 })
                    ghost.setTextureRect({ size * 2, 0, size, size });
                if(direction == sf::Vector2i { 0, 1 })
                    ghost.setTextureRect({ size * 3, 0, size, size });
            }
            else if(behavior() == behavior_mode::frightened)
            {
                int tx = 0;

                if(m_animation_should_blink && (int)(m_animation_advance * 1000) % 600 > 300)
                    tx += size * 2;

                if((int)(m_animation_advance * 1000) % 300 > 150)
                    tx += size;

                ghost.setTextureRect({ tx, 0, size, size });
            }
            else
            {
                ghost.setTextureRect({ 0 + ((int)(m_animation_advance * 1000) % 300 > 150 ? size : 0), size * texture_row(), size, size });

                if(direction == sf::Vector2i { -1, 0 })
                    ghost.setTextureRect({ ghost.getTextureRect().left + size, ghost.getTextureRect().top, -size, size });
                if(direction == sf::Vector2i { 0, -1 })
                    ghost.setTextureRect({ ghost.getTextureRect().left + size * 2, ghost.getTextureRect().top, size, size });
                if(direction == sf::Vector2i { 0, 1 })
                    ghost.setTextureRect({ ghost.getTextureRect().left + size * 4, ghost.getTextureRect().top, size, size });
            }

            ghost.setPosition(
                (int)hitbox_on_map().left, 
                (int)hitbox_on_map().top + map::top_bottom_border);

            target.draw(ghost);
        }
    }
}

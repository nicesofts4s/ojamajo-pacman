#include "player.h"
#include "../states/gameplay_state.h"
#include "states/play_state.h"

#include "data/gameplay.h"

#include <cmath>
#include <iostream>

namespace pacman
{
    namespace gameplay
    {
        int round_with_tile_offset(int tile, float offset)
        {
            if(offset < 0)
                return std::floor(tile + offset);
            return std::ceil(tile + offset);
        }

        void player::update(double delta, states::gameplay_state& gp_state)
        {
            sf::Vector2f last_tile_pos = sf::Vector2f(current_tile) + tile_offset;
            bool has_changed_direction = false;

            if(gp_state.map.can_pass_xy(current_tile.x + direction.x, current_tile.y + direction.y))
            {
                double spd = max_tiles_per_second;

                spd *= character.multipliers.speed;
                spd *= (m_distance_hold > 0 ? 0.9f : 1.0f);

                if(static_cast<play_state*>(&gp_state.sub_state())->frighten_time_left())
                    spd *= data::find_level_multipliers(gp_state.level).player_frightened_speed;
                else
                    spd *= data::find_level_multipliers(gp_state.level).player_norm_speed;

                tile_offset += sf::Vector2f(direction) * (float)spd * (float)delta;
                debug_stats.last_speed = spd;
            }
            else
                debug_stats.last_speed = 0;

            m_distance_hold -= delta;

            // advancd gmaeplay tekunique
            if(tile_offset.x <= -0.8 || tile_offset.x >= 0.8 || tile_offset.x == 0)
            {
                sf::Vector2i last_direction = direction;
                int add_tile_offset = tile_offset.x <= -0.8 ? -1 : tile_offset.x >= 0.8 ? 1 : 0;

                // if we've just moved horizontally the current tile might have an opening above or below
                // which means we can accept key inputs for up + down
                // if we did this all the time you could clip into a wall while your tile offset is  > -0.8  != 0  < 0.8  because we can't check in the middle of 
                // tiles with can_pass_xy
                if((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && gp_state.map.can_pass_xy(current_tile.x + add_tile_offset, current_tile.y - 1))
                    direction = { 0, -1 };
                if((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && gp_state.map.can_pass_xy(current_tile.x + add_tile_offset, current_tile.y + 1))
                    direction = { 0, 1 };

                if(last_direction != direction)
                {
                    if(std::abs(tile_offset.x) >= 0.8)
                    {
                        m_turn_corner_fancy_effect_from = { hitbox_on_map().left, hitbox_on_map().top };
                        // m_turn_corner_fancy_effect_percentage = 1 - ((std::abs(tile_offset.x) - 0.8) / 0.2);
                        m_turn_corner_fancy_effect_percentage = 1;
                    }

                    // snap to the next tile
                    tile_offset.x = add_tile_offset;
                }

                has_changed_direction = last_direction != direction;
            }

            if(!has_changed_direction)
            {
                if(tile_offset.y <= -0.8 || tile_offset.y >= 0.8 || tile_offset.y == 0)
                {
                    sf::Vector2i last_direction = direction;
                    int add_tile_offset = tile_offset.y <= -0.8 ? -1 : tile_offset.y >= 0.8 ? 1 : 0;

                    // see above
                    if((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && gp_state.map.can_pass_xy(current_tile.x - 1, current_tile.y + add_tile_offset))
                    {
                        facing_left = true;
                        direction = { -1, 0 };
                    }
                    if((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && gp_state.map.can_pass_xy(current_tile.x + 1, current_tile.y + add_tile_offset))
                    {
                        facing_left = false;
                        direction = { 1, 0 };
                    }

                    if(last_direction != direction)
                    {                      
                        if(std::abs(tile_offset.y) >= 0.8)
                        {
                            m_turn_corner_fancy_effect_from = { hitbox_on_map().left, hitbox_on_map().top };
                            // m_turn_corner_fancy_effect_percentage = 1 - ((std::abs(tile_offset.y) - 0.8) / 0.2);
                            m_turn_corner_fancy_effect_percentage = 1;
                        }

                        tile_offset.y = add_tile_offset;
                    }
                }
            }

            if(tile_offset.x <= -1 || tile_offset.x >= 1 || tile_offset.x == 0)
            {
                current_tile.x += (int)tile_offset.x;

                if(gp_state.map.can_pass_xy(current_tile.x + direction.x, current_tile.y + direction.y))
                {
                    if(tile_offset.x >= 1)
                        tile_offset.x -= 1;
                    if(tile_offset.x <= -1)
                        tile_offset.x += 1;
                }
                else
                    tile_offset.x = 0;

                if(current_tile.x < -3)
                    current_tile.x = map::tile_count_x + 3;
                if(current_tile.x > map::tile_count_x + 3)
                    current_tile.x = -3;
            }

            if(tile_offset.y <= -1 || tile_offset.y >= 1 || tile_offset.y == 0)
            {
                current_tile.y += (int)tile_offset.y;

                if(gp_state.map.can_pass_xy(current_tile.x + direction.x, current_tile.y + direction.y))
                {
                    if(tile_offset.y >= 1)
                        tile_offset.y -= 1;
                    if(tile_offset.y <= -1)
                        tile_offset.y += 1;
                }
                else
                    tile_offset.y = 0;

                if(current_tile.y < -3)
                    current_tile.y = map::tile_count_y + 3;
                if(current_tile.y > map::tile_count_y + 3)
                    current_tile.y = -3;
            }

            // std::round should (hopefully) (no promises) round @ the halfway point which means if you're halfway into a new tile you eat the pellet
            if(gp_state.map.pellet_at_xy(std::round(current_tile.x + tile_offset.x), std::round(current_tile.y + tile_offset.y)) != ' ')
            {
                bool power = gp_state.map.pellet_at_xy(std::round(current_tile.x + tile_offset.x), std::round(current_tile.y + tile_offset.y)) == 'O';
                gp_state.map.pellet_at_xy(std::round(current_tile.x + tile_offset.x), std::round(current_tile.y + tile_offset.y)) = ' ';

                m_eat_snd[m_eat_snd_current].play();

                m_eat_snd_current++;
                m_eat_snd_current = m_eat_snd_current % 2;

                m_distance_hold = (1.0 / 6.0) * (power ? 10 : 1);

                dots_left--;
                dots_eaten++;

                pts += std::floor(10 * character.multipliers.pts * (power ? 5 : 1));
                pts_until_1up -= std::floor(10 * character.multipliers.pts * (power ? 5 : 1));

                if(pts_until_1up <= 0)
                {
                    extra_lives++;
                    pts_until_1up = 10000 + pts_until_1up;
                }

                if(power)
                {
                    static double frighten_times[] = {
                        6, 5, 4, 3, 2, 5, 2, 2, 1, 5, 2, 1, 1, 3, 1, 1, 0, 1, 0
                    };

                    static_cast<play_state*>(&gp_state.sub_state())->frighten_ghosts(
                        frighten_times[std::min(sizeof(frighten_times) / sizeof(double) - 1, gp_state.level - 1)] * character.multipliers.blue_mode);
                }
            }

            if(last_tile_pos != (sf::Vector2f(current_tile) + tile_offset))
                m_animation_advance += delta;

            if(m_turn_corner_fancy_effect_percentage > 0)
                m_turn_corner_fancy_effect_percentage -= 14 * delta;
            if(m_turn_corner_fancy_effect_percentage < 0)
                m_turn_corner_fancy_effect_percentage = 0;

            if(direction.x != 0)
                m_turn_corner_fancy_effect_from.x = hitbox_on_map().left;
            if(direction.y != 0)
                m_turn_corner_fancy_effect_from.y = hitbox_on_map().top;
        }

        void player::draw(sf::RenderTarget& target)
        {
            sf::Sprite player(character.texture((int)(m_animation_advance * 1000) % 250 > 125));
            
            // I DONT LIKE YOU THE sf::Vector2f!!!!
            // I LIKE YOU THE sf::Vector2i!!!!
            player.setOrigin(sf::Vector2f(sf::Vector2i(character.center_point)));

            if(facing_left)
                player.setScale(-1, 1);
            if(direction == sf::Vector2i { 0, -1 })
                player.setRotation(facing_left ? -270 : -90);
            if(direction == sf::Vector2i { 0, 1 })
                player.setRotation(facing_left ? 270 : 90);

            sf::Vector2i pos = sf::Vector2i(hitbox_on_map().left, hitbox_on_map().top);

            /*if(m_turn_corner_fancy_effect_percentage > 0)
            {
                pos = { 
                    pos.x + (m_turn_corner_fancy_effect_from.x - pos.x) * m_turn_corner_fancy_effect_percentage,
                    pos.y + (m_turn_corner_fancy_effect_from.y - pos.y) * m_turn_corner_fancy_effect_percentage,
                };
            }*/

            player.setPosition(
                sf::Vector2f(pos + 
                sf::Vector2i { 0, map::top_bottom_border } +
                sf::Vector2i { size / 2, size / 2 }));

            target.draw(player);
        }
    }
}

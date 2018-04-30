#pragma once

#include "data/characters.h"
#include "map.h"
#include "../main.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace pacman
{
    namespace states
    {
        class gameplay_state;
    }

    namespace gameplay
    {
        class player
        {
public:
            player(const data::characters::info& character) : 
                character(character) 
            { 
                m_eat_snd_buf[0].loadFromFile("data/sound/eat_1.wav");
                m_eat_snd_buf[1].loadFromFile("data/sound/eat_2.wav");

                m_eat_snd[0] = sf::Sound(m_eat_snd_buf[0]);
                m_eat_snd[1] = sf::Sound(m_eat_snd_buf[1]);
                
                m_eat_snd[0].setVolume(current_save.volume);
                m_eat_snd[1].setVolume(current_save.volume);
            }

            const data::characters::info& character;

            // MAX speed in t/s
            // https://gaming.stackexchange.com/questions/93692/how-fast-does-pac-man-move
            constexpr static float max_tiles_per_second = 9.2;

            sf::Vector2i current_tile = { 0, 0 };

            // goes from -1 -> 0 -> 1
            // the idea here is that the player has a "current tile" and then a fractional offset from that
            // if current_tile -1 / +1 isn't passable (can_pass_xy) then tile_offset won't increase in that direction
            // once the offset goes to -1 or 1 it snaps to the next tile
            sf::Vector2f tile_offset = { 0, 0 };

            // 13x13
            constexpr static std::uint8_t size = 13;

            static sf::FloatRect tile_to_screen_pos(sf::Vector2f tile)
            {
                return { 
                    tile.x * map::tile_size - size / 2 + map::tile_size / 2,
                    tile.y * map::tile_size - size / 2 + map::tile_size / 2,
                    size,
                    size
                };
            }

            sf::FloatRect hitbox_on_map() const { return tile_to_screen_pos(sf::Vector2f(current_tile) + tile_offset); }

            // this is for actual movement
            sf::Vector2i direction;

            // this is cosmetic only
            bool facing_left = false;

            std::uint8_t extra_lives = 2;

            // 10 000 = 1-up
            // small dot = 10 pts
            // big dot = 50 pts
            // ghosts = 200 -> 400 -> 800 -> 1600
            // fruit 100-5000
            std::uint64_t pts = 0;

            std::uint64_t pts_until_1up = 10000;

            std::uint64_t dots_eaten = 0; 
            std::uint64_t dots_left = 0;

            void update(double delta, states::gameplay_state& gp_state);
            void draw(sf::RenderTarget& target);

            struct _debug_stats
            {
                double last_speed;
            };

            _debug_stats debug_stats = { 0 };
private:
            double m_animation_advance = 0;

            sf::SoundBuffer m_eat_snd_buf[2];
            sf::Sound m_eat_snd[2];

            int m_eat_snd_current = 0;

            // used for pausing after eating a dot
            double m_distance_hold = 0;

            double m_turn_corner_fancy_effect_percentage = 0;
            sf::Vector2f m_turn_corner_fancy_effect_from = { 0, 0 };
        };
    }
}
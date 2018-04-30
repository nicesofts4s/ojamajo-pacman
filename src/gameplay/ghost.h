#pragma once

#include "../states/gameplay_state.h"

#include <cmath>

#include <SFML/Graphics.hpp>

namespace pacman
{
    namespace gameplay
    {
        // https://www.gamasutra.com/view/feature/3938/the_pacman_dossier.php?print=1
        // https://www.gamedev.net/forums/topic/567673-pathfinding-algorithm-for-pacman/
        class base_ai_ghost
        {
public:
            base_ai_ghost() 
            {
                if(!m_texture)
                {
                    m_texture.reset(new sf::Texture());
                    m_texture->loadFromFile("data/ghosts/ghosts.png");

                    m_frightened_texture.reset(new sf::Texture());
                    m_frightened_texture->loadFromFile("data/ghosts/frightened.png");

                    m_eyes_texture.reset(new sf::Texture());
                    m_eyes_texture->loadFromFile("data/ghosts/eyes.png");
                }
            }

            virtual ~base_ai_ghost() { }

            enum class behavior_mode
            {
                // the ghosts chase the player, each ghost has their own "personality"
                chase, 
                // ghosts head for their home corner
                scatter,
                // ghosts move around aimlessly and are vulnerable
                frightened,
                // ghosts only have eyes left and return to their home sweet home
                // ;_;7
                eaten
            };

            sf::Vector2i current_tile = { 0, 0 };

            // see player.h for more info on tile offsets
            sf::Vector2f tile_offset = { 0, 0 };

            // the current target tile for the ghost
            // pathfinding occurs in update() and setting a new target tile occurs in the behavior functions
            sf::Vector2i target_tile = { 0, 0 };

            sf::Vector2i direction;
            
            virtual void update(double delta, states::gameplay_state& gp_state);
            
            behavior_mode behavior() const { return m_behavior; }
            virtual void transition_behavior(behavior_mode new_mode);

            // PLEASE OVERRIDE!!! LOLE!!!
            // THESE CANT BE PURE VIRTUAL BECAUSE THE CONSTRUCTOR CALLS THEM!!!
            // GCC GOT REALLY rEalLY ANGRY AT ME!!!! GRRRR!!!!!!!!!!!!! LOLE!!!!!!!!!
            // virtual sf::Vector2i start_tile() const { return { -1, -1 }; }
            // virtual sf::Vector2i start_direction() const { return { -1, -1 }; }
            // UPDATE: LOLE!!!!! THIS WONT WORK ANYWAY!!!!
            // THIS IS DUMB!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            virtual sf::Vector2i start_tile() const = 0;
            virtual sf::Vector2i start_direction() const = 0;

            // (-1, -1) = top left
            // (1, -1) = top right
            // (-1, 1) = bottom left
            // (1, 1) = bottom right
            virtual sf::Vector2i home_corner() const = 0;

            virtual float speed() const = 0;
            virtual bool can_exit_house(int dots_eaten) const = 0;

            // row of the first texture/frame in ghosts.png
            virtual int texture_row() const = 0;

            // 14x14
            constexpr static std::uint8_t size = 14;

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

            // no step on gost
            bool collides_with(const player& p) const
            {
                return p.current_tile == sf::Vector2i { std::round(current_tile.x + tile_offset.x), std::round(current_tile.y + tile_offset.y) };
            }

            void draw(sf::RenderTarget& target);
protected:
            virtual void update_chase(states::gameplay_state& gp_state) = 0;

            virtual void update_scatter(states::gameplay_state& gp_state);
            virtual void update_frightened(states::gameplay_state& gp_state);

            virtual void update_eaten(states::gameplay_state& gp_state) 
            { 
		        target_tile = start_tile(); 

                if(current_tile.x == start_tile().x && current_tile.y == start_tile().y && behavior() == behavior_mode::eaten)
                	transition_behavior(behavior_mode::chase);
            }
private:
            behavior_mode m_behavior = behavior_mode::scatter;

            double m_animation_advance = 0;
            bool m_animation_should_blink = false;

            static std::unique_ptr<sf::Texture> m_texture, m_frightened_texture, m_eyes_texture;
        };
    }
}

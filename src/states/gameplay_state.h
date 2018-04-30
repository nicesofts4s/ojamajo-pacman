#pragma once

#include "../state.h"
#include "../fonts.h"

#include "../gameplay/player.h"
#include "../gameplay/map.h"

#include <memory>

namespace pacman
{
    namespace states
    {
        class gameplay_state : public base_state
        {
private:
            gameplay_state() :
                player(data::characters::enum_to_info(current_save.selected_character))
            {
                m_layout_texture.loadFromFile("data/map/layout.png");
                
                m_small_dot_texture.loadFromFile("data/map/dot.png");
                m_big_dot_texture.loadFromFile("data/pacman/" + std::string(player.character.path) + "/powerpellet.png");
            }
public:
            // new level, completely reset
            gameplay_state(std::uint64_t level) :
                gameplay_state::gameplay_state()
            {
                this->level = level;

                for(std::uint8_t x = 0; x < gameplay::map::tile_count_x; x++)
                {
                    for(std::uint8_t y = 0; y < gameplay::map::tile_count_y; y++)
                    {
                        map.pellet_at_xy(x, y) = data::default_pellet_layout[(y * gameplay::map::tile_count_x) + x];
                        if(map.pellet_at_xy(x, y) != ' ')
                            player.dots_left++;

                        map.passable_tiles[(y * gameplay::map::tile_count_x) + x] = data::default_passable_layout[(y * gameplay::map::tile_count_x) + x];
                        map.ghost_passable_tiles[(y * gameplay::map::tile_count_x) + x] = data::ghost_passable_layout[(y * gameplay::map::tile_count_x) + x];
                    }
                }
            }

            // inherits an existing map/pellet layout
            gameplay_state(std::uint64_t level, gameplay::map& map) :
                gameplay_state::gameplay_state()
            {
                this->level = level;
                this->map = map;
            }

            void on_enter(base_state* last_state) override;
            
            void update(double delta, double total_time) override;
            void draw(sf::RenderTarget& target) override;

            gameplay::player player;
            gameplay::map map;

            std::uint64_t level = 1;

            int map_seed = 0;

            class base_state
            {
        public:
                base_state(gameplay_state& parent) : parent(parent) { }
                virtual ~base_state() { };

                virtual void update(double delta, double total_time) = 0;
                virtual void draw(sf::RenderTarget& target) = 0;

                virtual void on_enter(base_state* last_state) { };
                virtual void on_leave(base_state& next_state) { };

                virtual bool on_sf_event(sf::Event& ev) { return false; }

                gameplay_state& parent;
            };

            bool on_sf_event(sf::Event& ev) override { return sub_state().on_sf_event(ev); }

            base_state& sub_state() { return *m_current_sub_state; }
            base_state& set_state(base_state* new_state);
private:
            sf::Texture m_layout_texture;
            sf::Texture m_small_dot_texture, m_big_dot_texture;

            double start_time = 0;
            double total_time = 0;

            double m_highscore_flash = 0;

            std::unique_ptr<base_state> m_current_sub_state;
        };
    }
}

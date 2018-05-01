#include "gameplay_state.h"
#include "../main.h"

#include "../gameplay/states/start_state.h"
#include "../gameplay/states/play_state.h"

#include <iostream>

namespace pacman
{
    namespace states
    {
        static sf::Color color_for_pellet_xy(std::uint8_t x, std::uint8_t y, int seed)
        {
            srand(seed + (x << 8) + y);

            if(current_save.selected_character == data::characters::BURYPINK ||
                current_save.selected_character == data::characters::ESFORESTAN)
            {
                static sf::Color dot_colors[] = {
                    { 10, 167, 60 }, // green
                    { 246, 112, 223 } // pink
                };

                return dot_colors[rand() % 2];
            }

            // based off season 1 ep 1, 00:14:47
            // important 2 source ur colors!
            static sf::Color magic_sphere_colors[] = {
                { 243, 114, 31, 255 }, // orange
                { 199, 31, 127, 255 }, // pink-purple-ish
                { 202, 43, 84, 255 }, // pink-red-ish
                { 135, 189, 236, 255 }, // light blue
                { 134, 53, 145, 255 }, // purple
                { 29, 114, 178, 255 }, // blue
                { 232, 207, 55, 255 } // yellow
            };

            return magic_sphere_colors[rand() % 7];
        }

        void gameplay_state::on_enter(pacman::base_state* last_state)
        {
            set_state(new gameplay::start_state(*this));
            //set_state(new gameplay::play_state(*this));
        }

        void gameplay_state::update(double delta, double total_time)
        {
            if(start_time == 0)
            {
                start_time = total_time;
                map_seed = (int)(start_time * 4);
            }

            this->total_time = total_time;

            sub_state().update(delta, total_time);
            
            m_highscore_flash -= delta;

            if(player.pts > current_save.highscore)
            {
                m_highscore_flash = 1.0 / 60.0;
                current_save.highscore = player.pts;
            }
        }

        void gameplay_state::draw(sf::RenderTarget& target)
        {
            sf::Sprite layout(m_layout_texture);
            layout.setColor(player.character.secondary_color);

            target.draw(layout);

            layout.setColor(sf::Color::White);
            // the small "passthrough" area for the ghosts in the middle
            layout.setTextureRect({ 104, 121, 16, 2 });
            layout.setPosition({ 104, 121 });

            target.draw(layout);
            
            // small hack (like half of the game) to make death_state able to hide the dots
            if(map_seed != -1)
            {
                for(std::uint8_t x = 0; x < gameplay::map::tile_count_x; x++)
                {
                    for(std::uint8_t y = 0; y < gameplay::map::tile_count_y; y++)
                    {
                        sf::Sprite spr;

                        switch(map.pellet_at_xy(x, y))
                        {
                            case 'o':
                                spr = sf::Sprite(m_small_dot_texture);

                                if(player.character.path == data::characters::pacman.path)
                                    spr.setColor({ 255, 170, 163, 255 });
                                else
                                    // small dots are magic spheres
                                    spr.setColor(color_for_pellet_xy(x, y, map_seed));
                                break;
                            
                            case 'O':
                                if(player.character.path != data::characters::pacman.path || (int)((total_time - start_time) * 1000) % 400 <= 200)
                                    spr = sf::Sprite(m_big_dot_texture);
                                break;
                        }

                        if(spr.getTexture() != nullptr)
                        {
                            spr.setPosition(
                                x * gameplay::map::tile_size + (int)(gameplay::map::tile_size / 2 - spr.getTexture()->getSize().x / 2),
                                y * gameplay::map::tile_size + (int)(gameplay::map::tile_size / 2 - spr.getTexture()->getSize().y / 2) + gameplay::map::top_bottom_border);

                            target.draw(spr);
                        }
                    }
                }
            }

            sf::Sprite extra_life_spr(player.character.texture(0));

            for(std::uint8_t i = 0; i < player.extra_lives; i++)
            {
                extra_life_spr.setPosition(2 + (extra_life_spr.getLocalBounds().width + 2) * i, gameplay::map::top_bottom_border + gameplay::map::tile_size * gameplay::map::tile_count_y);
                //extra_life_spr.setScale(0.6, 0.6);

                target.draw(extra_life_spr);
            }

            sf::Text pts_text(std::to_string(player.pts) + " pts", fonts::emulogic, 8);
            pts_text.setPosition(6, 6);

            target.draw(pts_text);

            sf::Text hs_text("Highscore: " + std::to_string(current_save.highscore), fonts::emulogic, 8);
            hs_text.setPosition(224 - 6 - (int)hs_text.getLocalBounds().width, 6);
            hs_text.setColor({ 137, 188, 235, m_highscore_flash <= 0 ? 200 : 255 });

            target.draw(hs_text);

            sub_state().draw(target);
        }

        gameplay_state::base_state& gameplay_state::set_state(gameplay_state::base_state* new_state)
        {
            if(new_state == nullptr)
                return *new_state;

            if(m_current_sub_state)
                m_current_sub_state->on_leave(*new_state);

            new_state->on_enter(m_current_sub_state.get());
            m_current_sub_state.reset(new_state);
        }
    }
}
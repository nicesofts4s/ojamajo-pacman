#include "play_state.h"
#include "death_state.h"
#include "win_state.h"

#include "../data/gameplay.h"

namespace pacman
{
    namespace gameplay
    {
        void play_state::on_enter(states::gameplay_state::base_state* last_state)
        {
            parent.player.current_tile = { 13, 23 };

            parent.player.direction = { -1, 0 };
            parent.player.facing_left = true;
        }

        static sf::Vector2i select_nicemonye_pos(const map& map)
        {
            sf::Vector2i pos = { rand() % (map::tile_count_x - 1) + 1, rand() % (map::tile_count_y - 1) + 1 };

            while(!map.can_pass_xy(pos.x, pos.y))
            {
                switch(rand() % 2)
                {
                    case 0:
                        pos.x += rand() % 2 - 1;
                        break;

                    case 1:
                        pos.y += rand() % 2 - 1;
                        break;
                }

                if(pos.x < 1 || pos.y < 1 || pos.x > map::tile_count_x - 2 || pos.y > map::tile_count_y - 2)
                    return { 1, 1 };
            }

            return pos;
        }

        void play_state::update(double delta, double total_time)
        {
            if(start_time == 0)
                start_time = total_time;

            this->total_time = total_time;

            m_paused = m_paused && m_after_pause_action == after_pause_action::nothing;

            if((!m_paused || m_exit) && m_pause_music.getStatus() == sf::Music::Playing)
                m_pause_music.stop();
            if(m_paused && !m_exit && m_pause_music.getStatus() != sf::Music::Playing && (total_time - m_pause_start_time) >= 5)
            {
                m_pause_music.setVolume(0);
                m_pause_music.play();
            }
            
            if(m_paused)
                m_pause_music.setVolume(std::min(current_save.volume * 0.5, (total_time - m_pause_start_time - 4.5) * current_save.volume * 0.15));
            
            if(!m_paused && m_exit == 0)
            {
                m_current_bg_sound = frighten_time_left() ? BG_SND_SPECIAL : BG_SND_NORMAL;
                for(std::unique_ptr<base_ai_ghost>& ptr : m_ghosts)
                {
                    if(ptr->behavior() == base_ai_ghost::behavior_mode::eaten)
                        m_current_bg_sound = BG_SND_EATEN;
                }

                if(m_nicemonye_spawns_left > 0 && m_nicemonye_time_left <= 0)
                {
                    srand((int)((parent.player.dots_eaten + 1)) ^ parent.map_seed ^ parent.level);

                    if(rand() % 60 == 0)
                    {
                        m_nicemonye_spawns_left--;

                        // 6-14
                        m_nicemonye_time_left = 10 + (rand() % 400 - 200) / 100.0;
                        m_nicemonye_pos = select_nicemonye_pos(parent.map);
                        
                        if(parent.level >= 10)
                        {
                            switch(rand() % 3)
                            {
                                case 0:
                                    m_nicemonye_worth = 100;
                                    break;

                                case 1:
                                    m_nicemonye_worth = 200;
                                    break;

                                case 2:
                                    m_nicemonye_worth = 500;
                                    break;
                            }
                        }
                        else if(parent.level >= 5)
                        {
                            switch(rand() % 3)
                            {
                                case 0:
                                    m_nicemonye_worth = 50;
                                    break;

                                case 1:
                                    m_nicemonye_worth = 100;
                                    break;

                                case 2:
                                    m_nicemonye_worth = 150;
                                    break;                            
                            }
                        }
                        else
                        {
                            switch(rand() % 3)
                            {
                                case 0:
                                    m_nicemonye_worth = 25;
                                    break;

                                case 1:
                                    m_nicemonye_worth = 50;
                                    break;

                                case 2:
                                    m_nicemonye_worth = 75;
                                    break;
                            }
                        }
                    }
                }

                if(m_nicemonye_time_left > 0)
                    m_nicemonye_time_left -= delta;
                if(m_nicemonye_time_left <= 0 && m_pause_for <= 0)
                    m_nicemonye_time_left = 0;

                if(m_nicemonye_time_left > 0 && 
                    sf::Vector2i{ std::round(parent.player.current_tile.x + parent.player.tile_offset.x), std::round(parent.player.current_tile.y + parent.player.tile_offset.y) } == m_nicemonye_pos)
                {
                    m_eat_ghost_snd.play();

                    m_nicemonye_time_left = -1;
                    m_pause_for = 0.6;

                    current_save.nicemonye += m_nicemonye_worth;
                }

                if(!frighten_time_left())
                {
                    m_pellet_combo = 0;
                    m_eaten_ghost = nullptr;
                }

                if(m_bg_snd[m_current_bg_sound].getStatus() != sf::Sound::Playing && m_after_pause_action == after_pause_action::nothing)
                {
                    for(int i = 0; i < 4; i++) 
                        m_bg_snd[i].stop();
                        
                    m_bg_snd[m_current_bg_sound].play();
                }

                if(m_pause_for <= 0)
                {
                    parent.player.update(delta, parent);

                    if(!parent.player.dots_left)
                    {
                        m_pause_for = 1;
                        m_after_pause_action = after_pause_action::win;

                        for(int i = 0; i < 4; i++) 
                            m_bg_snd[i].stop();
                    }

                    if(!frighten_time_left())
                        m_ai_cycle_accum += delta;

                    if(data::find_level_ai_cycles(parent.level).times[m_ai_cycle] != -1 && m_ai_cycle_accum >= data::find_level_ai_cycles(parent.level).times[m_ai_cycle])
                    {
                        if(m_ai_cycle != 7)
                        {
                            m_ai_cycle_accum -= data::find_level_ai_cycles(parent.level).times[m_ai_cycle];
                            m_ai_cycle++;
                        }
                    }

                    m_frighten_time -= delta;
                    if(m_frighten_time < 0)
                        m_frighten_time = 0;

                    for(std::unique_ptr<base_ai_ghost>& ptr : m_ghosts)
                    {
                        ptr->update(delta, parent); 

                        if(ptr->behavior() != base_ai_ghost::behavior_mode::eaten && 
                            m_frighten_time <= 0)
                        {
                            ptr->transition_behavior(data::find_level_ai_cycles(parent.level).mode_for_i(m_ai_cycle));
                        }

                        if(ptr->collides_with(parent.player))
                        {
                            if(ptr->behavior() == base_ai_ghost::behavior_mode::frightened)
                            {
                                m_eat_ghost_snd.play();
                                
                                // this should be in player.cxx but w/e
                                ptr->transition_behavior(base_ai_ghost::behavior_mode::eaten);                            
                                m_eaten_ghost = ptr.get();

                                m_pellet_combo++;
                                if(m_pellet_combo > 4)
                                    m_pellet_combo = 4;

                                parent.player.pts += (2 ^ m_pellet_combo) * 100;
                                parent.player.pts_until_1up -= (2 ^ m_pellet_combo) * 100;

                                if(parent.player.pts_until_1up <= 0)
                                {
                                    parent.player.extra_lives++;
                                    parent.player.pts_until_1up = 10000 + parent.player.pts_until_1up;
                                }

                                m_pause_for = 0.6;
                            }
                            else if(ptr->behavior() != base_ai_ghost::behavior_mode::eaten)
                            {
                                if(m_pause_for <= 0)
                                {
                                    m_pause_for = 1;
                                    m_after_pause_action = after_pause_action::die;

                                    for(int i = 0; i < 4; i++) 
                                        m_bg_snd[i].stop();
                                }
                            }
                        }
                    }
                }

                m_pause_for -= delta;
                if(m_pause_for < 0)
                    m_pause_for = 0;

                if(m_pause_for == 0 && m_after_pause_action == after_pause_action::win)
                {
                    parent.set_state(new win_state(parent));
                    return;
                }

                if(m_pause_for == 0 && m_after_pause_action == after_pause_action::die)
                {
                    parent.set_state(new death_state(parent));
                    return;
                }
            } 

            if(m_paused || m_exit)
            {
                for(int i = 0; i < 4; i++) 
                    m_bg_snd[i].stop();

                if(m_exit)
                {
                    if(m_exit > 1.4)
                        pacman::set_state(m_exit_state);
                        
                    m_exit += delta;
                }
            }
        }

        void play_state::draw(sf::RenderTarget& target)
        {
            if(m_exit)
            {
                target.clear();
                return;
            }

            if(m_nicemonye_time_left > 0)
            {
                sf::Sprite nicemonye(m_nicemonye_texture);

                nicemonye.setPosition(
                    (m_nicemonye_pos.x * map::tile_size) + map::tile_size / 2 - m_nicemonye_texture.getSize().x / 2,
                    (m_nicemonye_pos.y * map::tile_size) + map::tile_size / 2 - m_nicemonye_texture.getSize().y / 2 + map::top_bottom_border);

                if(m_nicemonye_time_left > 1 || (int)(m_nicemonye_time_left * 1000) % 100 > 50)
                    target.draw(nicemonye);
            }

            if(m_pause_for > 0 && m_nicemonye_time_left == -1 && m_after_pause_action == after_pause_action::nothing)
            {
                sf::IntRect get_rect = { 0, 0, 0, 0 };

                switch(m_nicemonye_worth)
                {
                    case 25:
                        get_rect = { 0, 7, 11, 7 };
                        break;

                    case 50:
                        get_rect = { 11, 7, 10, 7 };
                        break;

                    case 75:
                        get_rect = { 21, 7, 11, 7 };
                        break;

                    case 100:
                        get_rect = { 33, 7, 13, 7 };
                        break;

                    case 150:
                        get_rect = { 45, 7, 13, 7 };
                        break;

                    case 200:
                        get_rect = { 59, 7, 15, 7 };
                        break;

                    case 500:
                        get_rect = { 0, 14, 15, 7 };
                        break;
                }

                sf::Sprite get(m_gets_texture, get_rect);
                get.setPosition(
                    (int)(m_nicemonye_pos.x * map::tile_size + map::tile_size / 2 - get_rect.width / 2),
                    (int)(m_nicemonye_pos.y * map::tile_size + map::tile_size / 2 - get_rect.height / 2) + map::top_bottom_border);

                target.draw(get);
            }
            else if(m_pause_for > 0 && m_pellet_combo > 0 && m_after_pause_action == after_pause_action::nothing)
            {
                static sf::IntRect gets[] = {
                    { 0, 0, 15, 7 },
                    { 16, 0, 15, 7 },
                    { 32, 0, 15, 7 },
                    { 48, 0, 16, 7 }
                };

                sf::IntRect get_rect = gets[std::min((int)(sizeof(gets) / sizeof(sf::IntRect)), m_pellet_combo) - 1];

                sf::Sprite get(m_gets_texture, get_rect);
                get.setPosition(
                    (int)(m_eaten_ghost->hitbox_on_map().left + m_eaten_ghost->hitbox_on_map().width / 2 - get_rect.width / 2),
                    (int)(m_eaten_ghost->hitbox_on_map().top + m_eaten_ghost->hitbox_on_map().height / 2 - get_rect.height / 2) + map::top_bottom_border);

                target.draw(get);
            }
            else
                parent.player.draw(target);

            for(std::unique_ptr<base_ai_ghost>& ptr : m_ghosts)
            {
                if(m_pause_for == 0 || m_pellet_combo == 0 || m_nicemonye_time_left == -1 || ptr.get() != m_eaten_ghost || m_after_pause_action != after_pause_action::nothing)
                    ptr->draw(target);
            }

            sf::Keyboard::Key dbg_key = 
                sf::Keyboard::isKeyPressed(sf::Keyboard::F4) ? sf::Keyboard::F4 :
                sf::Keyboard::isKeyPressed(sf::Keyboard::F8) ? sf::Keyboard::F8 :
                sf::Keyboard::isKeyPressed(sf::Keyboard::F7) ? sf::Keyboard::F7 :
                sf::Keyboard::isKeyPressed(sf::Keyboard::F6) ? sf::Keyboard::F6 :
                sf::Keyboard::isKeyPressed(sf::Keyboard::F5) ? sf::Keyboard::F5 :
                sf::Keyboard::N;

            if(dbg_key != sf::Keyboard::N)
            {
                for(std::uint8_t x = 0; x < gameplay::map::tile_count_x; x++)
                {
                    for(std::uint8_t y = 0; y < gameplay::map::tile_count_y; y++)
                    {
                        if(parent.map.can_pass_xy(x, y, dbg_key != sf::Keyboard::F4))
                        {
                            sf::RectangleShape rectangle({ map::tile_size, map::tile_size });
                            rectangle.setPosition(x * map::tile_size, y * map::tile_size + map::top_bottom_border);
                            rectangle.setFillColor({ 
                                255, 
                                0, 
                                255, 
                                100 + ((x + y) % 2 ? 60 : 0) });

                            if(parent.map.is_intersection(x, y, dbg_key != sf::Keyboard::F4))
                            {
                                rectangle.setFillColor({ 
                                    0, 
                                    255, 
                                    255, 
                                    160 });
                            }

                            if(dbg_key != sf::Keyboard::F4 && parent.map.is_ghost_forbidden_updown_zone(x, y))
                            {
                                rectangle.setFillColor({ 
                                    120, 
                                    120, 
                                    0, 
                                    160 });
                            }

                            if(dbg_key != sf::Keyboard::F4 && parent.map.is_side_tunnel(x, y))
                            {
                                rectangle.setFillColor({ 
                                    0, 
                                    255, 
                                    0, 
                                    160 });
                            }

                            target.draw(rectangle);
                        }
                    }
                }

                for(std::uint8_t y = 0; y < gameplay::map::tile_count_y; y++)
                {
                    sf::Text text(std::to_string(y), fonts::emulogic, 8);
                    text.setPosition(8, y * map::tile_size + map::top_bottom_border);

                    target.draw(text);
                }

                sf::RectangleShape hitbox({ player::size, player::size });

                if(dbg_key == sf::Keyboard::F4)
                    hitbox.setPosition(parent.player.hitbox_on_map().left, parent.player.hitbox_on_map().top + map::top_bottom_border);
                    
                if(dbg_key == sf::Keyboard::F5)
                    hitbox.setPosition(m_ghosts[0]->hitbox_on_map().left, m_ghosts[0]->hitbox_on_map().top + map::top_bottom_border);
                if(dbg_key == sf::Keyboard::F6)
                    hitbox.setPosition(m_ghosts[1]->hitbox_on_map().left, m_ghosts[1]->hitbox_on_map().top + map::top_bottom_border);
                if(dbg_key == sf::Keyboard::F7)
                    hitbox.setPosition(m_ghosts[2]->hitbox_on_map().left, m_ghosts[2]->hitbox_on_map().top + map::top_bottom_border);
                if(dbg_key == sf::Keyboard::F8)
                    hitbox.setPosition(m_ghosts[3]->hitbox_on_map().left, m_ghosts[2]->hitbox_on_map().top + map::top_bottom_border);
                
                hitbox.setFillColor({ 0, 0, 0, 0 });
                hitbox.setOutlineColor(sf::Color::Magenta);
                hitbox.setOutlineThickness(1);

                target.draw(hitbox);

                if(dbg_key == sf::Keyboard::F4)
                {
                    sf::Vertex line[] = {
                        sf::Vertex({ 
                            player::tile_to_screen_pos(sf::Vector2f(parent.player.current_tile)).left + player::size / 2, 
                            player::tile_to_screen_pos(sf::Vector2f(parent.player.current_tile)).top + player::size / 2 + map::top_bottom_border 
                        }, sf::Color::Blue),
                        sf::Vertex({ 
                            player::tile_to_screen_pos(sf::Vector2f(parent.player.current_tile) + parent.player.tile_offset).left + player::size / 2, 
                            player::tile_to_screen_pos(sf::Vector2f(parent.player.current_tile) + parent.player.tile_offset).top + player::size / 2 + map::top_bottom_border 
                        }, sf::Color::Blue)           
                    };

                    target.draw(line, 2, sf::Lines);

                    sf::Text debug_text(
                        "PLAYER\n\n"
                        "Current tile: " + std::to_string(parent.player.current_tile.x) + ", " + std::to_string(parent.player.current_tile.y) + "\n"
                        "Offset from tile: " + std::to_string(parent.player.tile_offset.x) + ",\n                  " + std::to_string(parent.player.tile_offset.y) + "\n"
                        "Speed: " + std::to_string(parent.player.debug_stats.last_speed) + " t/s\n"
                        "Upcoming tile: " + std::to_string(parent.player.current_tile.x + parent.player.direction.x) + ", " + std::to_string(parent.player.current_tile.y + parent.player.direction.y) + "\n"
                        "Upcoming tile passable: " + std::to_string(parent.map.can_pass_xy(parent.player.current_tile.x + parent.player.direction.x, parent.player.current_tile.y + parent.player.direction.y)) + "\n",
                        fonts::emulogic,
                        8);

                    debug_text.setPosition(24, map::top_bottom_border);

                    target.draw(debug_text);
                }

                // idk why this is a macro lole
#define DRAW_GHOST(i_, key_) \
                if(dbg_key == sf::Keyboard::key_) \
                { \
                    sf::Vertex line[] = { \
                        sf::Vertex({  \
                            base_ai_ghost::tile_to_screen_pos(sf::Vector2f(m_ghosts[i_]->current_tile)).left + player::size / 2, \
                            base_ai_ghost::tile_to_screen_pos(sf::Vector2f(m_ghosts[i_]->current_tile)).top + player::size / 2 + map::top_bottom_border \
                        }, sf::Color::Green), \
                        sf::Vertex({ \
                            base_ai_ghost::tile_to_screen_pos(sf::Vector2f(m_ghosts[i_]->current_tile) + m_ghosts[i_]->tile_offset).left + player::size / 2, \
                            base_ai_ghost::tile_to_screen_pos(sf::Vector2f(m_ghosts[i_]->current_tile) + m_ghosts[i_]->tile_offset).top + player::size / 2 + map::top_bottom_border \
                        }, sf::Color::Green) \
                    }; \
                    \
                    target.draw(line, 2, sf::Lines); \
                    \
                    sf::Vertex line2[] = { \
                        sf::Vertex({  \
                            base_ai_ghost::tile_to_screen_pos(sf::Vector2f(m_ghosts[i_]->current_tile)).left + player::size / 2, \
                            base_ai_ghost::tile_to_screen_pos(sf::Vector2f(m_ghosts[i_]->current_tile)).top + player::size / 2 + map::top_bottom_border \
                        }, sf::Color::Yellow), \
                        sf::Vertex({ \
                            base_ai_ghost::tile_to_screen_pos(sf::Vector2f(m_ghosts[i_]->target_tile)).left + player::size / 2, \
                            base_ai_ghost::tile_to_screen_pos(sf::Vector2f(m_ghosts[i_]->target_tile)).top + player::size / 2 + map::top_bottom_border \
                        }, sf::Color::Yellow) \
                    }; \
                    \
                    target.draw(line2, 2, sf::Lines); \
                    \
                    sf::Text debug_text( \
                        "GHOST " + std::to_string(i_ + 1) + "\n\n" \
                        "Current tile: " + std::to_string(m_ghosts[i_]->current_tile.x) + ", " + std::to_string(m_ghosts[i_]->current_tile.y) + "\n" \
                        "Offset from tile: " + std::to_string(m_ghosts[i_]->tile_offset.x) + ",\n                  " + std::to_string(m_ghosts[i_]->tile_offset.y) + "\n" \
                        "AI target tile: " + std::to_string(m_ghosts[i_]->target_tile.x) + ", " + std::to_string(m_ghosts[i_]->target_tile.y) + "\n", \
                        fonts::emulogic, \
                        8); \
                    \
                    debug_text.setPosition(24, map::top_bottom_border); \
                    \
                    target.draw(debug_text); \
                }

                DRAW_GHOST(0, F5)
                DRAW_GHOST(1, F6)
                DRAW_GHOST(2, F7)
                DRAW_GHOST(3, F8)
            }

            if(m_paused)
            {
                sf::RectangleShape cover({ 224, 288 });
                cover.setFillColor({ 0, 0, 0, 100 });

                target.draw(cover);

                if((int)((total_time - m_pause_start_time) * 1000) % 800 <= 400)
                {
                    sf::Text exit_text("PAUSED!\n    return to main menu\n    reset at level 1", fonts::emulogic, 8);
                    exit_text.setPosition((int)(224 / 2 - exit_text.getLocalBounds().width / 2), (int)(288 / 2 - exit_text.getLocalBounds().height / 2));

                    target.draw(exit_text);
                    
                    sf::Text exit_text_color("\n[X]\n[R]", fonts::emulogic, 8);
                    exit_text_color.setPosition((int)(224 / 2 - exit_text.getLocalBounds().width / 2), (int)(288 / 2 - exit_text.getLocalBounds().height / 2));
					exit_text_color.setColor({ 80, 188, 70, 255 });
					
                    target.draw(exit_text_color);
                }
            }
        }
    }
}

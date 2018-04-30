#pragma once

#include "../state.h"
#include "../fonts.h"
#include "../main.h"

#include "credits_state.h"
#include "adjust_volume_state.h"
#include "gameplay_state.h"
#include "select_character_state.h"
#include "unlock_shorts_san_state.h"

#include "../gameplay/data/characters.h"

#include <SFML/Audio.hpp>
#include <string>

namespace pacman
{
    namespace states
    {
        class insert_coin_state : public base_state
        {        
            inline static std::string select_random_tip() 
            {
                static int current_tip = 0;

                static std::vector<std::string> tips = {
                    "always make sure\nto properly source\nyour're quotes!",
                    "sip some nice watre",
                    "watch doremi 2\nunderstand wut it means 2\nbe nice",
                    "The problem is\nthat you probably aren't\nseeing all the posts. If you are just viewing [s4s] through the /s4s/ board, you don't see the green posts, the posts with letters in their",
                    "waka waka",
                    "waka waka\nwaka",
                    "waka waka waka\nwaka waka waka",
                    "kill the\nghosts LOLE!!!!",
                    "waka wak",
                    "2 rudes don't\nmake a nice",
                    "make nice posts",
                    "nice dubs",
                    "play gaem instead\nof read tip!!",
                    "LOLE!!!!!!!!!!! LOLE!!!!!!!!!!!!!!!!!! LOLE!!!!!!!!!!! LOLE!!!!!!!!!!!!!!!!!! LOLE!!!!!!!!!!! LOLE!!!!!!!!!!!!!!!!!!",
                    "get a big highscore,\npost it on [s4s]!",
                    "more images\nlike this?",
                    "dokkiri dokkiri\ndon don",
                    "yosho must stop\nthe spikeman?",
                    "the spikeman must\nstop yosho?",
                    "The",
                    "check 'em",
                    "do not suicide\nYOU WILL DIE",
                    "its not hard 2 be nice",
                    "yuji sakai has won",
                    "this is nice\ntitlescreen",
                    "insert the nicecoine",
                    "buy more games\nfrom nicesoft\nLOLE!!!!!!!!!!!!!",
                    "this is the secret tip\nif you got this\nyou're lucky!",
                    "error viruse detected",
                    "bump nice threads",
                    "ogawa segawa",
                    "lole!",
                    "Your Fortune:\nGodly Luck",
                    "Your Fortune:\nAverage Luck",
                    "Your Fortune:\nVery Bad Luck",
                    "Your Fortune:\nReply hazy, try again",
                    "I LOVE YOU\nTHE SPIKEMAN!!!!!",
                    "aiwbuvwiavouiaugiewuavhewaivuhewioveha",
                    "ieowuheiozvjzoiesuhzseiuvheszuivezixuvh",
                    "vnmdcvibnzdxvudwsoipvhwiovhwaiovhueviewu",
                    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
                    ":>)",
                    ":^)",
                    "GO STARGUY GO!",
                    "STAR GUY - OUR HERO",
                    "jump on top of\nthe goombas",
                    "if you start the game\n10 000 times something\nmight happen?... idk?",
                    "make a fun thread!",
                    "eat good breakfast\npet animals\nbe in sunrays",
                    "ITS FUGGING\nHAPPENING!!!!!!!!!\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
                    "this game is free\n(as in freedom)!",
                    "onpu strong",
                    "this is nice tip",
                    "koopas have shells!",
                    "YUJI SAKAI" 
                    
                    
                    "\n\n"





                    "\n\n"
                    
                    
                    "SPACING",
                    "wakawakawaka",
                    "Prepare to eat dots\nlike you've never eaten\ndots before",
                    "type \"fortune\" in the\noptions field to check\nyour're fortune!",
                    "dubs! wooh!",
                    "huh?",
                    "do your best!",
                    "GO TEAM PEEP GO!!!",
                    "After 9 years in\ndevelopment, hopefully it\nwould have been worth\nthe wait",
                    "om nom nom",
                    "wakawakawakawak",
                    "\"Oh my god!\"",
                    "blocks with question\nmarks will give you items\nlike mushrooms!",
                    "dodo is the best\ncharacter",
                    "dododo dodo do",
                    "dodo dodo dododo",
                    "birds are very\nimportant!",
                    "type \"ojamajo\" in the\noptions field to check\nyour're ojamajo!",
                    "?",
                    "wait no i forgot",
                    "Press F11 to play\nin fullscreen!\n(this is the only real tip)",
                    "luigi is the brother\nof mario!",
                    "great game it deserves\n300 sequels and yet so far\nthis is the only one",
                    "nice to see someone\nplaying my're oc :^)!",
                    "tips can be pretty\nuseful!",
                    "editing save.txt is\na crime!!",
                    "earn som nicemonye 2\nbuy new characters",
                    "life is happy\nand nice :^)",
                    "\"eben if u dond\nagree wid where da dubs\nwere born u sbud always\ncheck dem becuz ids\nnod dere fault....\"",
                    "<insert tip text>",
                    "i don't know\nwhat to put here",
                    "i am error.",
                    "nicemonye will\nspawn more often\nat higher levels!",
                    "E",
                    "this is the last tip\nin the list LOLE!!!!!!!"
                };

                srand(time(nullptr));

                return tips[rand() % tips.size()];
                // return tips[current_tip++];
            }
public:
            insert_coin_state() :
                m_title_text("- OJAMAJO PACMAN -", fonts::emulogic, 8),
                m_topd_text("tip: " + select_random_tip(), fonts::emulogic, 8), 
                m_buttons_text("        insert coine\n    select character\n    credits\n    adjust volume", fonts::emulogic, 8),
                m_buttons_color_text("[ENTER]\n[Z]\n[C]\n[V]", fonts::emulogic, 8),

                m_highscore_text("HIGHSCORE: " + std::to_string(current_save.highscore) + " pts", fonts::emulogic, 8)
            {
                m_title_text.setPosition({ 
                    (int)(224 / 2 - m_title_text.getLocalBounds().width / 2), 
                    (int)(288 / 2 - m_topd_text.getLocalBounds().height / 2) - 44, 
                });
                m_topd_text.setPosition({ 
                    (int)(224 / 2 - m_topd_text.getLocalBounds().width / 2), 
                    (int)(288 / 2 - m_topd_text.getLocalBounds().height / 2) - 26, 
                });

                if(m_topd_text.getPosition().x < 2)
                    m_topd_text.setPosition(2, m_topd_text.getPosition().y);

                m_topd_text.setColor({ 255, 255, 255, 120 });

                m_buttons_text.setPosition({ 
                    (int)(224 / 2 - m_buttons_text.getLocalBounds().width / 2), 
                    (int)(288 / 2 - m_buttons_text.getLocalBounds().height / 2) + 24, 
                });
                
                m_buttons_color_text.setPosition({ 
                    (int)(224 / 2 - m_buttons_text.getLocalBounds().width / 2), 
                    (int)(288 / 2 - m_buttons_text.getLocalBounds().height / 2) + 24, 
                });
                
                m_buttons_color_text.setColor({ 80, 188, 70, 255 });

                m_highscore_text.setPosition({ 
                    (int)(224 - m_highscore_text.getLocalBounds().width - 10), 
                    (int)(288 - m_highscore_text.getLocalBounds().height - 10), 
                });
                m_highscore_text.setColor({ 137, 188, 235, 180 });

                m_doremi_sprite.setTexture(data::characters::doremi.texture(0));
                m_doremi_sprite.setPosition({ 224 + 222 /* trips nice */, 288 - 80 });
                m_doremi_sprite.setScale(-1, 1);

                pick_new_doremi_y();

                m_bg_music.openFromFile("data/music/insert_coin.ogg");
                m_bg_music.setVolume(current_save.volume);
                m_bg_music.setLoop(true);

                m_click_snd_buf.loadFromFile("data/sound/click.wav");
                
                m_click_snd = sf::Sound(m_click_snd_buf);
                m_click_snd.setVolume(current_save.volume);
            }

            void on_enter(base_state* last_state) override { m_bg_music.play(); }

            bool on_sf_event(sf::Event& ev) override 
            {
                if(ev.type == sf::Event::KeyPressed)
                {
                    /*if(ev.key.code == sf::Keyboard::F5)
                    {
                        m_topd_text = sf::Text("tip: " + select_random_tip(), fonts::emulogic, 8);
                        m_title_text.setPosition({ 
                            (int)(224 / 2 - m_title_text.getLocalBounds().width / 2), 
                            (int)(288 / 2 - m_topd_text.getLocalBounds().height / 2) - 44, 
                        });
                        m_topd_text.setPosition({ 
                            (int)(224 / 2 - m_topd_text.getLocalBounds().width / 2), 
                            (int)(288 / 2 - m_topd_text.getLocalBounds().height / 2) - 26, 
                        });

                        if(m_topd_text.getPosition().x < 2)
                            m_topd_text.setPosition(2, m_topd_text.getPosition().y);

                        m_topd_text.setColor({ 255, 255, 255, 120 });
                    }*/

                    if(!m_exit)
                    {
                        if(ev.key.code == sf::Keyboard::Num3 || ev.key.code == sf::Keyboard::Numpad3)
                        {
                            m_3_count++;

                            if(m_3_count >= 6 && !current_save.unlocked[data::characters::SHORTS])
                            {
                                m_exit_state = new unlock_shorts_san_state();
                                m_exit = true;

                                m_click_snd.play();
                            }
                        }
                        else
                            m_3_count = 0;

                        if(ev.key.code == sf::Keyboard::Return)
                        {
                            m_exit_state = new gameplay_state(1);
                            m_exit = true;

                            m_click_snd.play();
                        }

                        if(ev.key.code == sf::Keyboard::Z)
                        {
                            m_exit_state = new select_character_state();
                            m_exit = true;

                            m_click_snd.play();
                        }

                        if(ev.key.code == sf::Keyboard::C)
                        {
                            m_exit_state = new credits_state();
                            m_exit = true;

                            m_click_snd.play();
                        }

                        if(ev.key.code == sf::Keyboard::V)
                        {
                            m_exit_state = new adjust_volume_state(false);
                            m_exit = true;

                            m_click_snd.play();
                        }
                    }
                }

                return false;
            }

            void update(double delta, double total_time) override;
            void draw(sf::RenderTarget& target) override;
private:
            void pick_new_doremi_y();

            sf::Text m_title_text, m_buttons_color_text, m_buttons_text, m_topd_text, m_highscore_text;

            sf::Sprite m_doremi_sprite;

            sf::Music m_bg_music;

            sf::SoundBuffer m_click_snd_buf;
            sf::Sound m_click_snd;

            double start_time = 0;
            double total_time = 0;

            double m_exit = 0;
            base_state* m_exit_state = nullptr;
            
            std::size_t m_current_bg_character = 0;

            int m_3_count = 0;
        };
    }
}

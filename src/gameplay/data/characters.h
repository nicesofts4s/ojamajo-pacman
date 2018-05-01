#pragma once

#include <string>
#include <map>
#include <array>

#include <SFML/Graphics.hpp>

namespace pacman
{
    namespace data
    {
        namespace characters
        {            
            enum character_enum : std::uint8_t
            {
                DOREMI,
                HADZUKI,
                AIKO,
                ONPU,
                MOMOKO,
                HANA,
                POPPU,
                SHORTS,
                PACMAN,
                BURYPINK,
                ESFORESTAN,
                DODO,

                SIZE_CHARACTER
            };
            
            // character info
            struct info
            {                
                // sf::Color doesn't work with constexpr for some reason
                struct _constexpr_color 
                { 
                    std::uint8_t r, g, b; 
                    inline operator const sf::Color() const { return { r, g, b, 255 }; };
                };

                struct _constexpr_vector
                { 
                    int x, y; 
                    inline operator const sf::Vector2i() const { return { x, y }; };
                };

                struct _multipliers { float speed, pts, blue_mode; };

                const char* name, *author, *path, *description;
                int cost;

                _constexpr_color primary_color, secondary_color;
                _multipliers multipliers;

                _constexpr_vector center_point;
                
                character_enum cenum;

                sf::Texture& texture(std::size_t frame) const;
            };

            extern std::map<character_enum, std::array<sf::Texture, 2>> g_player_texture_map;

            constexpr info doremi = {
                "Doremi",
                "Nicesoft",
                "doremi",
                "The worlds unluckiest...\n\"pizza missing a slice\"?",

                0,

                { 175, 42, 66 }, // hair / red
                { 253, 130, 167 }, // costume / pink
                
                {
                    1.0,
                    1.0,
                    1.0
                },

                { 8, 10 },
                
                DOREMI
            };

            constexpr info hadzuki = {
                "Hadzuki",
                "Nicesoft",
                "hadzuki",
                "i don't know what\nto put here?????",

                100,

                { 164, 100, 46 }, // hair / brown
                { 251, 154, 91 }, // costume / orange,

                { 
                    0.93,
                    1.1,
                    1.05
                },

                { 8, 9 },
                
                HADZUKI
            };

            constexpr info aiko = {
                "Aiko",
                "Nicesoft",
                "aiko",
                "Solar-powered.",

                100,

                { 39, 60, 112 }, // hair / dark blue
                { 71, 122, 200 }, // costume / light blue,

                { 
                    1.07,
                    0.9,
                    1.05
                },

                { 8, 11 },
                
                AIKO
            };

            constexpr info onpu = {
                "Onpu",
                "Nicesoft",
                "onpu",
                "onpu strong",

                100,

                { 163, 90, 160 }, // hair / purple
                { 153, 113, 181 }, // costume / violet
                
                {
                    1.01,
                    1.1,
                    0.92
                },

                { 10, 10 },

                ONPU
            };

            constexpr info momoko = {
                "Momoko",
                "Nicesoft",
                "momoko",
                "\"Oh my god!\"",

                200,

                { 239, 239, 84 }, // hair / yellow
                { 242, 240, 81 }, // costume / also yellow,

                { 
                    1.05,
                    1.0,
                    1.2
                },

                { 11, 7 },
                
                MOMOKO
            };

            constexpr info hana = {
                "Hana",
                "Nicesoft",
                "hana",
                "\"Chiisu!\"",

                200,

                { 244, 229, 143 }, // hair / golden-yellow
                { 246, 245, 248 }, // costume / white,

                { 
                    1.0,
                    1.2,
                    0.8
                    
                },

                { 12, 8 },

                HANA
            };

            constexpr info poppu = {
                "Pop",
                "Nicesoft",
                "poppu",
                "The",

                300,

                { 230, 104, 147 }, // hair / pink
                { 214, 80, 80 }, // costume / red,

                { 
                    1.04,
                    1.1,
                    1.0
                    
                },

                { 9, 8 },

                POPPU
            };

            constexpr info shorts = {
                "Shorts-san",
                "Nicesoft",
                "shorts",
                "It's Doremi's shorts.\n(they have a face)",

                // unlocked with the code "333333" on the title screen
                -1,

                { 213, 168, 163 },
                { 255, 237, 221 },

                { 
                    1.1,
                    1.0,
                    0.8  
                },

                { 10, 6 },

                SHORTS
            };

            constexpr info pacman = {
                "Pacman",
                "Nicesoft",
                "pacman",
                "It's Pacman.",

                100,

                { 255, 255, 0 },
                { 33, 33, 255 },

                { 
                    1.0,
                    1.0,
                    1.0
                },

                { 6, 6 },

                PACMAN
            };

            constexpr info burypink = {
                "bury pink",
                "Nicesoft",
                "burypink",
                "this is nice character",

                1000,

                { 253, 6, 0 },
                { 1, 84, 214 },

                { 
                    1.15,
                    1.15,
                    1.15
                },

                { 8, 8 },

                BURYPINK
            };

            constexpr info esforestan = {
                "[s4s]-tan",
                "DWTH",
                "s4stan",
                "The meme lord herself",

                5555,

                { 128, 0, 0 }, // hair / red
                { 255, 0, 220 }, // costume / pink

                { 
                    1.33,
                    1.33,
                    1.33
                },

                { 8, 11 },

                ESFORESTAN
            };

            constexpr info dodo = {
                "Dodo",
                "Nicesoft",
                "dodo",
                "Dodo dododo dodo.\n(the best character)",

                10000,

                { 205, 141, 158 }, // hair / light pink
                { 251, 217, 224 }, // costume / light pink

                { 
                    1.5,
                    1.5,
                    1.5
                },

                { 8, 10 },

                DODO
            };

            // constexpr lole
            inline const info& enum_to_info(character_enum character)
            {
                switch(character)
                {
                    case HADZUKI: return hadzuki;
                    case AIKO: return aiko;
                    case ONPU: return onpu;
                    case MOMOKO: return momoko;
                    case HANA: return hana;
                    case POPPU: return poppu;
                    case SHORTS: return shorts;
                    case PACMAN: return pacman;
                    case BURYPINK: return burypink;
                    case DODO: return dodo;
                    case ESFORESTAN: return esforestan;
                }

                return doremi;
            }
        }
    }
}

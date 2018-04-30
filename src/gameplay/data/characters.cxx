#include "characters.h"

namespace pacman
{
    namespace data
    {
        namespace characters
        {
            std::map<character_enum, std::array<sf::Texture, 2>> g_player_texture_map;

            sf::Texture& info::texture(std::size_t frame) const
            {
                if(!g_player_texture_map.count(cenum))
                {
                    g_player_texture_map[cenum][0].loadFromFile("data/pacman/" + std::string(path) + "/1.png");
                    g_player_texture_map[cenum][1].loadFromFile("data/pacman/" + std::string(path) + "/2.png");
                }

                return g_player_texture_map[cenum][frame == 1 ? 1 : 0];
            }
        }
    }
}

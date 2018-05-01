#pragma once

#include <string>

#include "gameplay/data/characters.h"

namespace pacman
{
    class save
    {
public:
        save(const std::string& file);

        float volume = -1;
        
        bool unlocked[data::characters::SIZE_CHARACTER] = { true, false, false, false, false, false, false, false, false, false, false, false };
        data::characters::character_enum selected_character = data::characters::DOREMI;

        std::uint64_t highscore = 0;
        std::uint64_t nicemonye = 0;

        void save_to_file(const std::string& file) const;
    };
}
#include "save.h"

#include <fstream>
#include <functional>

namespace pacman
{
    save::save(const std::string& file)
    {
        std::ifstream fstream(file);

        if(fstream.is_open())
        {
            fstream.imbue(std::locale::classic());

            std::string version;
            fstream >> version;

            bool no_esforestan = false;

            if(version != "1.1")
            {
                no_esforestan = true;
                fstream.seekg(0);
            }

            fstream >> volume;

            std::hash<std::string> hash;
            for(int i = 0; i < data::characters::SIZE_CHARACTER; i++)
            {
                if(i != data::characters::ESFORESTAN || !no_esforestan)
                {
                    std::size_t expected_value = hash(data::characters::enum_to_info(static_cast<data::characters::character_enum>(i)).path);

                    std::size_t recieved_value;
                    fstream >> recieved_value;

                    unlocked[i] = recieved_value == expected_value;
                }
            }

            int recieved_character = data::characters::DOREMI;
            fstream >> recieved_character;

            selected_character = static_cast<data::characters::character_enum>(recieved_character);

            fstream >> highscore;
            fstream >> nicemonye;

            std::size_t expected_value = hash(std::to_string(highscore ^ nicemonye));

            std::size_t recieved_value;
            fstream >> recieved_value;

            if(recieved_value != expected_value)
                highscore = nicemonye = 0;
        }

        if(!unlocked[(int)selected_character])
            selected_character = data::characters::DOREMI;

        unlocked[data::characters::ESFORESTAN] = true;
    }

    void save::save_to_file(const std::string& file) const
    {
        std::ofstream fstream(file);
        fstream.imbue(std::locale::classic());
        
        fstream << "1.1" << std::endl;
        fstream << volume << std::endl;

        std::hash<std::string> hash;
        for(int i = 0; i < data::characters::SIZE_CHARACTER; i++)
            fstream << (unlocked[i] ? hash(data::characters::enum_to_info(static_cast<data::characters::character_enum>(i)).path) : 0) << std::endl;

        fstream << (int)selected_character << std::endl;

        fstream << highscore << std::endl;
        fstream << nicemonye << std::endl;

        fstream << hash(std::to_string(highscore ^ nicemonye));
    }
}

#pragma once

#include "../../main.h"
#include "../../states/gameplay_state.h"

namespace pacman
{
    namespace gameplay
    {
        class win_state : public states::gameplay_state::base_state
        {
public:
            win_state(states::gameplay_state& parent) : 
                states::gameplay_state::base_state(parent) 
            { 
                m_layout_texture.loadFromFile("data/map/layout.png");
            }
            
            void update(double delta, double total_time) override;
            void draw(sf::RenderTarget& target) override;
private:
            sf::Texture m_layout_texture;

            double start_time = 0;
            double total_time = 0;

            double m_exit = 0;
        };
    }
}
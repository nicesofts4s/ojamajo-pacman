#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

namespace pacman
{
    // game state is a pretty cool class. eh has functions and doesnt afraid of anything
    class base_state
    {
public:
        virtual ~base_state() { };

        virtual void update(double delta, double total_time) = 0;
        virtual void draw(sf::RenderTarget& target) = 0;

        virtual void on_enter(base_state* last_state) { };
        virtual void on_leave(base_state& next_state) { };

        virtual bool on_sf_event(sf::Event& ev) { return false; }
    };
}
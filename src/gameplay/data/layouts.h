#pragma once

namespace pacman
{
    namespace data
    {
        // " " = none
        // "o" = small
        // "O" = big
        constexpr static const char* default_pellet_layout =
            "                            "
            " oooooooooooo  oooooooooooo "
            " o    o     o  o     o    o "
            " O    o     o  o     o    O "
            " o    o     o  o     o    o "
            " oooooooooooooooooooooooooo "
            " o    o  o        o  o    o "
            " o    o  o        o  o    o "
            " oooooo  oooo  oooo  oooooo "
            "      o              o      "
            "      o              o      "
            "      o              o      "
            "      o              o      "
            "      o              o      "
            "      o              o      "
            "      o              o      "
            "      o              o      "
            "      o              o      "
            "      o              o      "
            "      o              o      "
            " oooooooooooo  oooooooooooo "
            " o    o     o  o     o    o "
            " o    o     o  o     o    o "
            " Ooo  ooooooo  ooooooo  ooO "
            "   o  o  o        o  o  o   "
            "   o  o  o        o  o  o   "
            " oooooo  oooo  oooo  oooooo "
            " o          o  o          o "
            " o          o  o          o "
            " oooooooooooooooooooooooooo "
            "                            ";

        // "x" = passable
        // anything else = not passable
        constexpr static const char* default_passable_layout =
            "                            "
            " xxxxxxxxxxxx  xxxxxxxxxxxx "
            " x    x     x  x     x    x "
            " x    x     x  x     x    x "
            " x    x     x  x     x    x "
            " xxxxxxxxxxxxxxxxxxxxxxxxxx "
            " x    x  x        x  x    x "
            " x    x  x        x  x    x "
            " xxxxxx  xxxx  xxxx  xxxxxx "
            "      x     x  x     x      "
            "      x     x  x     x      "
            "      x  xxxxxxxxxx  x      "
            "      x  x        x  x      "
            "      x  x        x  x      "
            "xxxxxxxxxx        xxxxxxxxxx"
            "      x  x        x  x      "
            "      x  x        x  x      "
            "      x  xxxxxxxxxx  x      "
            "      x  x        x  x      "
            "      x  x        x  x      "
            " xxxxxxxxxxxx  xxxxxxxxxxxx "
            " x    x     x  x     x    x "
            " x    x     x  x     x    x "
            " xxx  xxxxxxxxxxxxxxxx  xxx "
            "   x  x  x        x  x  x   "
            "   x  x  x        x  x  x   "
            " xxxxxx  xxxx  xxxx  xxxxxx "
            " x          x  x          x "
            " x          x  x          x "
            " xxxxxxxxxxxxxxxxxxxxxxxxxx "
            "                            ";

        // big X = can pass but can't go up or down (under normal conditions)
        constexpr static const char* ghost_passable_layout =
            "                            "
            " xxxxxxxxxxxx  xxxxxxxxxxxx "
            " x    x     x  x     x    x "
            " x    x     x  x     x    x "
            " x    x     x  x     x    x "
            " xxxxxxxxxxxxxxxxxxxxxxxxxx "
            " x    x  x        x  x    x "
            " x    x  x        x  x    x "
            " xxxxxx  xxxx  xxxx  xxxxxx "
            "      x     x  x     x      "
            "      x     x  x     x      "
            "      x  xxXXXXXXxx  x      "
            "      x  x   xx   x  x      "
            "      x  x xxxxxx x  x      "
            "xxxxxxxxxx xxxxxx xxxxxxxxxx"
            "      x  x xxxxxx x  x      "
            "      x  x        x  x      "
            "      x  xxxxxxxxxx  x      "
            "      x  x        x  x      "
            "      x  x        x  x      "
            " xxxxxxxxxxxx  xxxxxxxxxxxx "
            " x    x     x  x     x    x "
            " x    x     x  x     x    x "
            " xxx  xxxxxXXXXXXxxxxx  xxx "
            "   x  x  x        x  x  x   "
            "   x  x  x        x  x  x   "
            " xxxxxx  xxxx  xxxx  xxxxxx "
            " x          x  x          x "
            " x          x  x          x "
            " xxxxxxxxxxxxxxxxxxxxxxxxxx "
            "                            ";
    }
}
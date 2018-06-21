#include "../../include/OrientationImpl.hpp"
#include <aex/AWindow.hpp>
#include <SDL.h>

namespace aex
{
    void setSupportedOrientations(std::size_t orientations)
    {
        if(orientations & AWindow::PORTRAIT)
        {
        }

        if(orientations & AWindow::PORTRAIT_DOWN)
        {
        }

        if(orientations & AWindow::LANDSCAPE_LEFT)
        {
        }

        if(orientations & AWindow::LANDSCAPE_RIGHT)
        {
        }
    }
}

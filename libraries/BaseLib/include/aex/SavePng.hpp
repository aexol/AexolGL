/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SavePng.hpp
 * Author: rafa
 *
 * Created on November 28, 2016, 4:57 PM
 */

#ifndef SAVEPNG_HPP
#define SAVEPNG_HPP

#include <aex/Common.hpp>   
    
#define png_infopp_NULL (png_infopp)NULL
#define int_p_NULL (int*)NULL

namespace aex
{
    bool savePngFile(const std::string& name, void* data, int sizeX, int sizeY);
    
}

#endif /* SAVEPNG_HPP */


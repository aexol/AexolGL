/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TapLine.hpp
 * Author: rafa
 *
 * Created on January 10, 2017, 1:47 PM
 */

#ifndef TAPLINE_HPP
#define TAPLINE_HPP
#include <aex/Math.hpp>

namespace aex {
    namespace math {
        class Line2D {

            public:
                Line2D();
                Line2D(float a, float b);
                Line2D(const aex::math::Vector2& pointA, const aex::math::Vector2& pointB);

                bool isPointUnder(aex::math::Vector2 point);
                void resolve(const aex::math::Vector2& pointA, const aex::math::Vector2& pointB);
                aex::math::Vector2 func(float x) const ;
                aex::math::Vector2 funcOnParallerDist(float x, float dist);
                Line2D parallelAtPoint(const aex::math::Vector2& point)const;
                Line2D perpendicularAtPoint(const aex::math::Vector2& point)const;
                float perpendicularDistance(const aex::math::Vector2& point) const;
                aex::math::Vector2 intersect(const Line2D& line) const;
                
            public:
                float a = 0.0f;
                float b = 0.0f;
        };
    }
}
#endif /* TAPLINE_HPP */

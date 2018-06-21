/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BBox2d.hpp
 * Author: rafa
 *
 * Created on July 17, 2017, 1:06 PM
 */

#ifndef BBOX2D_HPP
#define BBOX2D_HPP

#include <aex/Vector2.hpp>

namespace aex {
    namespace math {
        class BoundingBox2D {
            public:
                Vector2 m_center, m_halfdist;
                BoundingBox2D();
                BoundingBox2D(std::vector<Vector2> points);
                BoundingBox2D(Vector2 center, Vector2 halfdist);


                bool isPointInBox(Vector2 point);
                bool checkBoxCollision(const BoundingBox2D& other);


                void findBoundingBox(const std::vector<Vector2>& points);
                void setHalfdist(Vector2 halfdist);
                Vector2 getHalfdist() const;
                void setCenter(Vector2 center);
                Vector2 getCenter() const;
        };
    } /* math */
} /* aex */

#endif /* BBOX2D_HPP */


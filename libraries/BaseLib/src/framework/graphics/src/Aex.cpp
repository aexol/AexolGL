//
//  Aex.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 25.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include <aex/Box.hpp>
#include <aex/GPUApi.hpp>
#include <aex/Aex.hpp>
#include <aex/AexRenderer.hpp>
#include <aex/ShaderDrw.hpp>
#include <aex/ShaderProgram.hpp>
#include <aex/Uniforms.hpp>
#include <aex/AWindow.hpp>
#include <aex/Logger.hpp>
#include <string>

namespace aex
{
    namespace __detail_taex
    {
        std::atomic<uint32_t> _nextId(1);
        uint32_t nextId()
        {
            return _nextId.fetch_add(1);
        }
    }

    Aex::Aex(PositionType position, RotationType rotation, ScaleType scale, ASceneNode* parent) :
        TAex<TRSNode>(position, rotation, scale, parent),
        m_posUniform("Pos")
    {
    }

    Aex::Aex(ASceneNode* parent) :
        Aex(PositionType(0.0f, 0.0f, 0.0f), aex::math::getIdentityQuaternion(), ScaleType(1.0f, 1.0f, 1.0f), parent){}

    Aex::Aex() : Aex(nullptr){}

    void
    Aex::draw(AexRenderer* gl)
    {
        const ShaderDrw* shader = gl->getShader();
        if(shader)
        {
            const PositionType& position = getPosition();
            m_posUniform.setValue(position.x, position.y, position.z);
            m_posUniform.setUniform(*shader);
        }
        TAex<TRSNode>::draw(gl);
    }

    void
    Aex::finishdraw(AexRenderer* gl)
    {
        TAex<TRSNode>::finishdraw(gl);
    }
    
    uint32_t
    Aex::getAexId()
    {
        return getTAexId();
    }
}

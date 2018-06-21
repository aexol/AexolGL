//
//  BoundingSphere.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 01.08.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//


#include <aex/BoundingSphereObject.hpp>
#include <aex/Aex.hpp>
#include <aex/Mesh.hpp>
#include <algorithm>
#include <random>
#include <array>
#include <cfloat>


namespace aex
{


    BoundingSphereObject::BoundingSphereObject(bool draw) :
        m_bSphere(),
        m_Epsilon(0.001f),
        m_draw(draw),
        m_sphereWireframe(0)
    {

    }

    BoundingSphereObject::BoundingSphereObject(const BoundingSphereObject& bSphere) :
        m_bSphere(bSphere.m_bSphere),
        m_Epsilon(0.001f),
        m_draw(false),
        m_sphereWireframe(0)
    {

    }

    BoundingSphereObject::BoundingSphereObject(Aex& parent, const BoundingSphereObject& bSphere, bool draw) :
        m_bSphere(bSphere.m_bSphere),
        m_Epsilon(0.001f),
        m_draw(draw),
        m_sphereWireframe(0)
    {
        if(draw)
        {
            set_sphere_wireframe(parent);
        }
    }
    BoundingSphereObject&
    BoundingSphereObject::operator=(const BoundingSphereObject& bSphere)
    {
        m_bSphere     = bSphere.m_bSphere;
        m_Epsilon     = bSphere.m_Epsilon;
        m_draw        = false;
        m_sphereWireframe.reset();
        return (*this);
    }

    void
    BoundingSphereObject::welzlCompute(std::vector<float>& vertices)
    {
        m_bSphere.welzlCompute(vertices);
    }
    void
    BoundingSphereObject::scale(float x)
    {
        m_bSphere.scale(x);

        if(m_sphereWireframe)
        {
            m_sphereWireframe->setSize(Vector(m_bSphere.radius(), m_bSphere.radius(), m_bSphere.radius()));
            m_sphereWireframe->setPosition(m_bSphere.center());
        }
    }
    void
    BoundingSphereObject::move(float x, float y, float z)
    {
        m_bSphere.move(x, y, z);

        if(m_sphereWireframe)
        {
            m_sphereWireframe->setPosition(m_bSphere.center());
        }
    }
    void
    BoundingSphereObject::set_sphere_wireframe(Aex& parent)   //WTF is this?
    {
//        std::pair<h_mesh, bool> pair = MeshManager::getInstance().GetMeshPair("sphere6", 0);
//        aex::Mesh* sphere_mesh;
//        aex::Mesh* mesh = MeshManager::getInstance().Dereference(pair.first);
//
//        if(! pair.second)
//        {
//            sphere_mesh = aex::sphere(6);
//            std::vector<float>& sphere_vertices = sphere_mesh->getVertices();
//            mesh->setVertices(std::move(sphere_vertices));
//            mesh->computeWireFrame();
//        }
//
//        m_sphereWireframe = aex::getBoundingObjectFrame( parent.get_pool(), mesh );
//        m_sphereWireframe->set_wireframe(2);
//        m_sphereWireframe->set_size( Vector( m_bSphere.radius(), m_bSphere.radius(), m_bSphere.radius() ) );
//        m_sphereWireframe->set_position( m_bSphere.center() );
    }
}

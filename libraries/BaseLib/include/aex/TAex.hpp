//
//  TAex.hpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 25.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#ifndef _AEX_TAEX_HPP
#define _AEX_TAEX_HPP

#include <aex/AexDrawable.hpp>
#include <aex/Vector3.hpp>
#include <aex/Quaternion.hpp>
#include <aex/Uniform.hpp>
#include <aex/IOnAexRendererUpdate.hpp>
#include <atomic>
#include <vector>
#include <memory>
#include <string>
#include <list>


namespace aex
{
    class ASceneNode;
    template<class T> class TInstance;
    class AexRenderer;
    class UniformArray;

    namespace __detail_taex
    {
        uint32_t nextId();
    }

    template<class T>
    class TAex : public DrawObject, public T
    {
            static_assert(std::is_convertible<T*, ASceneNode*>::value, "Invalid template parameter.");
            friend class TInstance<T>;
            typedef aex::math::Matrix Matrix;
            typedef aex::math::Vector3 Vector3;
            typedef std::list<TInstance<T>*> Instances;\

        protected:
            
        
        class AexRendererUpdateListener : public IOnAexRendererUpdate
        {
            public:
                explicit AexRendererUpdateListener(TAex* self);
                AexRendererUpdateListener(const AexRendererUpdateListener&) = delete;
                AexRendererUpdateListener(AexRendererUpdateListener&&) = delete;
                AexRendererUpdateListener& operator=(const IOnAexRendererUpdate& rhs);
                AexRendererUpdateListener& operator=(const AexRendererUpdateListener& rhs);
            protected:
                void onViewProjectionUpdate();
                void onLightsUpdate();
            private:
                TAex* parent;
        };
                
            
        public:
            typedef aex::shared_ptr<Uniform> Uniform_ptr;
            typedef std::vector<Uniform_ptr> Uniforms;
            typedef std::vector<Uniform*> UniformsRaw;
            /**
             *	@brief	Default Ctor
             */
            TAex();

            /**
             * @brief Copy constructor.
             */
            TAex(const TAex&) = default;

            /**
             * @brief Copy assignment.
             *
             * @returns TAex& Reference to this.
             */
            TAex& operator=(const TAex&) = default;

            /**
             * @brief Move constructor.
             */
            TAex(TAex&&) = default;

            /**
             * @brief Move assignment.
             *
             * @returns TAex& Reference to this.
             */
            TAex& operator=(TAex&&) = default;

            /**
             *  @brief Universal constructor.
             *
             *  Pass all constructor parameters to base class.
             */
            template<class... Args>
            TAex(Args&&... args);

            /**
             * @brief Destructor.
             */
            ~TAex();
            
            /**
             *	@brief Set aex uniforms in current shader.
             *
             *	@param 	AexRenderer*
             */
            void setUniforms(AexRenderer*);

            /**
             * @brief Add uniform to aex.
             *
             * Add uniform to aex, increasing it's reference count.
             *
             * @param Reference counted uniform.
             */
            void addUniform(Uniform_ptr& newUniform);

            /**
             * @brief Add uniform to aex.
             *
             * Add uniform copy to aex.
             *
             * @param Uniform* Original uniform.
             */
            void addUniform(Uniform* newUniform);
            void removeUniform(Uniform* newUniform);

            /**
             * @brief Set ligts for aex.
             */
            void set_lights(AexRenderer* gl, aex::shared_ptr<aex::Uniform> lights);

            /**
             * @brief Set ligts for aex.
             */
            void set_lights(AexRenderer* gl, aex::Uniform* lights);

            /**
             *	@brief draw Draw aex.	
             *
             *	@param 	gl 	AexRederer pointer
             */
            void draw(AexRenderer* gl);
            
            /**
             *	@brief sets Uniforms for drawing.	
             *
             *	@param const ShaderDrw pointer
             */
            void updateDrawUniforms(AexRenderer* gl ,const ShaderDrw* shader);
            
            /**
             *	@brief finishdraw Cleanup after drawing.
             *
             *	@param 	gl 	AexRederer pointer
             */
            void finishdraw(AexRenderer* gl);

            /**
             * @brief Stub
             */
            void update();

            /**
             * @brief Stub
             */
            void needsUpdate();

            /**
             * @brief Stub
             */
            AABB getBounds();

            /**
             * @brief Stub
             */
            AABB getBounds(AABBBuilder children);

            /**
             * @brief Stub
             */
            AABB getChildBounds();

            /**
             *  @brief Set uniforms for aex. 
             *
             *  @param Uniforms New uniforms.
             */
            void setUniforms(Uniforms uniforms);

            /**
             *  @brief Get uniforms from aex. 
             *
             *  Return Uniforms from aex.
             *
             *  @returns Uniforms array.
             */
            Uniforms& getUniforms();

            /**
             * @brief Aex unique ID.
             *
             * Each aex has unique ID. Use this method to retrive it.
             *
             * @returns uint32_t Unique ID.
             */
            uint32_t getTAexId() const;

            /**
             * @brief TODO
             */
            void setTransparent(bool Transparent);
            //{
            //    m_isTransparent = Transparent;
            //}

            /**
             * @brief TODO
             */
            bool isTransparent() const;
            
            /**
             * @brief Toggle use of precomputed MVP matrix.
             *
             * If true use precomputed MVP matrix instead of separate Model, View, Projection matrices.
             *
             * @param bool On/Off precomputed MVP.
             */
            void setPreComputedMVP(bool preComputedMVP);

            /**
             * @brief Check if using precomputed matrix.
             *
             * @returns bool True if using precomputed MVP.
             */
            bool isPreComputedMVP() const;
            
            void setNodeName(aex::string nodeName);
            aex::string getNodeName() const;
            
        private:
            const uint32_t m_aexId;
            const aex::string m_name;
            aex::string m_nodeName;
            Uniforms m_uniforms;
            UniformsRaw m_uniformsRaw;
            Vector3 m_colorID;

            UniformMatrix4fv m_ModelMatrixUniform;
            UniformMatrix4fv m_normalMatrixUniform;
            UniformMatrix4fv m_preComputeMVPUniform;
            UniformFloat3 m_colorPickUniform;

            Instances m_instances;
            Matrix m_preComputeMVP;
            bool m_isTransparent = false;
            bool m_RevertBlend = false;
            bool m_preComputedMVP = false; 

            bool m_refreshViewProjection=true;
            AexRendererUpdateListener m_updateListener;
            
            void addInstance(TInstance<T>* instance);
            void removeInstance(TInstance<T>* instance);
    };

} // end of namespace aex.

#include "details/TAex.ipp"

#endif  // _AEX_TAEX_HPP

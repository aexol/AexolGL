//
//  ShaderDrw.h
//  AexolGL
//
//  Created by Kamil Matysiewicz on 22.10.2013.
//
//

#ifndef AexolGL_ShaderDrw_h
#define AexolGL_ShaderDrw_h

#include <aex/Uniforms.hpp>
#include <aex/Mesh.hpp>
#include <aex/api_definitions.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/IOnAexRendererUpdate.hpp>
#include <aex/Common.hpp>
#include <aex/TLS.hpp>
#include <boost/signals2.hpp>
#include <string>
#include <vector>
#include <memory>
#include <map>


namespace aex
{

    class VertexData;
    class IndexData;
    typedef std::vector<aex::shared_ptr<aex::Uniform>> Uniforms;
    typedef std::vector<aex::Uniform*> UniformsRaw;
    typedef aex::AexRenderer AexRenderer;
    typedef std::list<std::shared_ptr<VertexData>> VertexBuffers;
    typedef aex::shared_ptr<ShaderDrw> ShaderDrwShrd_Ptr;
    typedef std::map<aex::string, AexShaderParameterLocation> Uniform_map;

    /*
     * @brief Basic shader that can be used for drawing.
     *
     * ShaderDrw is an object which can represent a drawing technique.
     * Each ShaderDrw comes with predefined static variables called:
     * gl_ProjectionMatrix(mat4)
     * gl_ViewMatrix(mat4)
     * gl_ModelMatrix (mat4)
     * gl_ModelViewMatrix (mat4)
     * gl_ViewProjectionMatrix(mat4)
     * gl_ModelViewProjectionMatrix(mat4)
     * gl_NormalMatrix(mat3)
     *
     * and vertex attributes which can be refered as:
     *
     * gl_Vertex
     * gl_TexCoord
     * gl_nextFrame
     * gl_Normal
     */
    
    struct Extension{
        
        aex::string extensionName;
        aex::string extensionBehavior;
        
        Extension(aex::string extensionName , aex::string extensionBehavior):extensionName(extensionName),extensionBehavior(extensionBehavior)
        {
            
        }
        
    };
    
   
    class AEX_PUBLIC_API ShaderDrw : public DrawObject
    {

    public:

        /**
         *	@brief	Default Ctor
         */
        ShaderDrw();

        /**
         *	@brief	Ctor of ShaderDrw
         *
         *	@param 	aex::string& 	vertex program source
         *	@param 	aex::string& 	fragment program source
         *	@param 	aex::string 	vertex attribiutes
         */
        ShaderDrw(const aex::string&, const aex::string&, const aex::string& = "");
        ShaderDrw(const ShaderDrw& shader);

        /**
         * @brief Move constructor.
         */
        ShaderDrw(ShaderDrw&& shader);
        ShaderDrw& operator=(const ShaderDrw& shader);
        ShaderDrw& operator=(ShaderDrw&& shader);

        virtual ~ShaderDrw();

        /**
         *	@brief Use this shader for drawing.
         *
         *	@param 	gl Drawing context.
         */
        void draw(AexRenderer* gl);

        /**
         * @brief add default uniform value.
         *
         * adds a default value for uniform in shader. each time shader is used for drawing, uniform values will be initialized to this value.
         *
         * @param shared_ptr<uniform> uniform.
         */
        void addUniform(const aex::shared_ptr<aex::Uniform>& uniform);

        /**
         * @brief add default uniform value.
         *
         * adds a default value for uniform in shader. each time shader is used for drawing, uniform values will be initialized to this value.
         *
         * @param Uniform* uniform.
         */
        void addUniform(Uniform* uniform);
        
        void removeUniform(Uniform* remove);

        std::vector<Uniform*>& getUniformList() const;
        void setUniformList(std::vector<Uniform*>);
        /**
         *	@brief Update HW shader uniform values with default values.
         */
        void uniformsUpdate() const;
        
        void addExtension(aex::string extension, aex::string behavior);
        
        void updateVPMatix(AexRenderer* gl) const;

        /*
         * @brief Revert gl to a state before draw.
         *
         * @param gl Drawing context.
         */
        void finishdraw(AexRenderer* gl);

        /**
         * @brief Empty stub.
         */
        void needsUpdate(){}

        /**
         * @brief Set default uniform values.
         *
         * Set default uniform values for shader.
         */
        void setUniforms(Uniforms uniforms);
        Uniforms& getUniforms();

        /**
         *	@brief	Get shader program number.
         *
         *	@return	Program number
         */
        const AexShader& GetProgram() const
        {
            lateInit();
            return m_currentThreadData->m_program;
        }


        void setUniformMap(Uniform_map uniformMap);
        Uniform_map getUniformMap() const;

        ShaderDrwShrd_Ptr makeColorPickerShader();

        
        static aex::string fix(const aex::string& header, aex::string source);
        
        /**
         * @brief Nothing special, just draw black model. 
         *
         * @return New ShaderDrw object.
         */
        static ShaderDrw simplyBlack();
        /**
         * @brief Nothing special, just draw white model. 
         *
         * @return New ShaderDrw object.
         */
        static ShaderDrw simplyWhite();
        /**
         * @brief Nothing special, just draw red model. 
         *
         * @return New ShaderDrw object.
         */
        static ShaderDrw simplyRed();
        /**
         * @brief Report bug.
         *
         * @return New ShaderDrw object.
         */
        static ShaderDrw simplyRedAnimated();
        /**
         * @brief Report bug.
         *
         * @return New ShaderDrw object.
         */
        static ShaderDrw simplyNormalToColor();
        /**
         * @brief Report bug.
         *
         * @return New ShaderDrw object.
         */
        static ShaderDrw basicBackgroundBox();
        /**
         * @brief Report bug.
         *
         * @return New ShaderDrw object.
         */
        static ShaderDrw BasicSpriteShader();
        /**
         * @brief Report bug.
         *
         * @return New ShaderDrw object.
         */
        static ShaderDrw BasicSpriteShaderCameraAligned();
        /**
         * @brief Report bug.
         *
         * @return New ShaderDrw object.
         */
        static ShaderDrw BasicHudElementShaderOrtho();
        /**
         * @brief Report bug.
         *
         * @return New ShaderDrw object.
         */
        static ShaderDrw TextDraw();
        /**
         * @brief Report bug.
         *
         * @return New ShaderDrw object.
         */
        static ShaderDrw* BasicHudElementShaderOrthoOffCenter();
        /**
         * @brief Report bug.
         *
         * @return New ShaderDrw object.
         */
        static void FreeHudElementShaderOrthoOffCenter();
        /**
         * @brief Report bug.
         *
         * @return New ShaderDrw object.
         */
        static ShaderDrw* BasicHudElementShaderOrthoOffCenterAtlas();
        /**
         * @brief Report bug.
         *
         * @return New ShaderDrw object.
         */
        static void FreeHudElementShaderOrthoOffCenterAtlas();
        static ShaderDrw* BasicBatchedTextStringAtlas();
        static void FreeBasicBatchedTextStringAtlas();
        void setCameraPosNeeded(bool cameraPosNeeded);
        bool isCameraPosNeeded() const;
        bool isNeeds_MVP() const;
        void setNeedsNormalMatrix(bool needsNormalMatrix);
        bool isNeedsNormalMatrix() const;
        aex::string getVertex_lines() const;
        aex::string getFragment_lines() const;
        aex::string getVertex_attr() const;
        
        
    protected:
        
        struct ThreadSpecificData
        {
            ThreadSpecificData();
            ThreadSpecificData(const ThreadSpecificData& tsd);
            ThreadSpecificData& operator=(const ThreadSpecificData& tsd);
            ThreadSpecificData(ThreadSpecificData&& tsd);
            ThreadSpecificData& operator=(ThreadSpecificData&& tsd);
            AexShader m_program;
            UniformMatrix4fv m_ViewProjectionMatrixUniform;
            UniformMatrix4fv m_ViewMatrixUniform;
            UniformFloat3 m_camPosUniform;
            Uniforms m_uniforms;
            UniformsRaw m_uniformsRaw;
            bool m_refreshUniforms;
            bool m_isReady;
            bool m_needs_mvp=false;
            bool m_needs_normalMatrix=false;
        };

        class AexRendererUpdateListener : public IOnAexRendererUpdate
        {
            public:
                explicit AexRendererUpdateListener(ShaderDrw* self);
                AexRendererUpdateListener(const AexRendererUpdateListener&) = delete;
                AexRendererUpdateListener(AexRendererUpdateListener&&) = delete;
                AexRendererUpdateListener& operator=(const IOnAexRendererUpdate& rhs);
                AexRendererUpdateListener& operator=(const AexRendererUpdateListener& rhs);
            protected:
                void onViewProjectionUpdate();
                void onLightsUpdate();
            private:
                ShaderDrw* parent;
        };

        /**
         *	@brief	Build shader program from sources.
         */
        virtual void build() const;
        std::vector<std::pair<aex::string, int>> searchLinesForAttribs();
        void relink() const;
        void lateInit() const;
        void cleanup();
        void onContextDirty();
        aex::string m_header;
        aex::string m_vertex_header;
        aex::string m_vertex_attr;
        aex::string m_fragment_header;
        aex::string m_vertex_lines;
        aex::string m_fragment_lines;
        std::vector<Extension> m_extensions;
        bool m_refreshViewProjection=true;
        bool m_refreshLights;
        bool m_cameraPosNeeded = false;
        bool m_needsNormalMatrix = false; 
        mutable ThreadLocalStorage<ThreadSpecificData> m_currentThreadData;
        AexRendererUpdateListener m_updateListener;
        
    };
    typedef aex::shared_ptr<ShaderDrw> ShaderDrw_ptr;

    /**
     * @brief Report bug.
     *
     * @return New ShaderDrw object.
     */
    AEX_PUBLIC_API ShaderDrw basicShaderDrw();
    AEX_PUBLIC_API ShaderDrw DropletShaderDrw();
    AEX_PUBLIC_API ShaderDrw LoadShaderFromFile(const aex::string& vertName, const aex::string& fragName, const aex::string& vertex_attr = "");
}


#endif

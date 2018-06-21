/** @file AexRenderer.hpp
 *  @brief Definition of AexRenderer.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__AexRenderer__
#define __aexolGL__AexRenderer__

#include <aex/Uniforms.hpp>
#include <aex/Matrix.hpp>
#include <aex/Vector3.hpp>
#include <aex/api_definitions.hpp>
#include <aex/GraphicsDevice.hpp>
#include <aex/ALoopNode.hpp>
#include <aex/QuaternionCamera.hpp>
#include <aex/Point.hpp>
#include <aex/GPUApi.hpp>
#include <list>
#include <vector>
#include <array>
#include <stack>
#include <string>
#include <exception>



namespace aex
{

    typedef aex::math::Vector3 Vector;
    typedef std::vector<aex::shared_ptr<Uniform>> Uniforms;
    typedef aex::shared_ptr<Uniform> Uniformptr;
    typedef aex::math::Matrix Matrix;
    typedef std::stack<ShaderDrw*> ShaderStack;
    typedef std::stack<int> PassStack;

    class AWindow;
    class ShaderDrw;
    class VertexData;
    class IndexData;
    class IOnAexRendererUpdate;
    
    namespace __detail_passId
    {
        int getPassId(const std::string& name);
    }
    
    class AEX_PUBLIC_API AexRenderer
    {
    public:
		AexRenderer();
        /**
         *	@brief	Ctor
         *
         *	@param 	window 	Pointer to window.
         */
        AexRenderer(AWindow* window);
        
        void refreshDrawState();
        bool setBlendState(bool state);
        bool setDepthTestState(bool state);
        void setClearColor(float r, float g, float b, float a = 0.0);
        void setClearColor(aex::math::Vector3 color, float a = 0.0);
        void setLineWidth(float width);
        ~AexRenderer();

        /**
         *	@brief	Project point to screen space.
         *
         *	@param 	objX 	X value of point.
         *	@param 	objY 	Y value of point.
         *	@param 	objZ 	Z value of point
         *	@param 	modelView 	ModelView matrix.
         *	@param 	projection 	Projection matrix.
         *	@param 	Viewport 	Current viewport (X, Y, width, height).
         *
         *	@return	Point in screen space.
         */
        Vector project(float objX, float objY, float objZ) const;
        Vector project(float objX, float objY, float objZ, const Matrix& modelView) const;
        Vector project(float objX, float objY, float objZ, const Matrix& modelView, const Matrix& projection) const;
        Vector project(float objX, float objY, float objZ, const Matrix& modelView, const Matrix& projection, const ViewPort& viewport) const;

        /**
         *	@brief	Project point from screen space.
         *
         *	@param 	winX 	X value of point.
         *	@param 	winY 	Y value of point.
         *	@param 	winZ 	Z value of point
         *	@param 	modelView 	ModelView matrix.
         *	@param 	projection 	Projection matrix.
         *	@param 	Viewport 	Current viewport (X, Y, width, height).
         *
         *	@return	Point in screen space.
         */
        Vector unProject(float winX, float winY) const;
        Vector unProject(float winX, float winY, float winZ) const;
        Vector unProject(float winX, float winY, float winZ, const Matrix& modelView) const;
        Vector unProject(float winX, float winY, float winZ, const Matrix& modelView, const Matrix& projection) const;
        Vector unProject(float winX, float winY, float winZ, const Matrix& modelView, const Matrix& projection, const ViewPort& viewport) const;

        /**
         *	@brief	Get current projection matrix.
         *
         *	@return	Projection Matrix.
         */
        Matrix getViewProjectionMatrix();
        void setViewProjectionMatrix(const Matrix& mat);
        void setViewProjectionMatrix(Matrix&& mat);

        /**
         *	@brief	Get current projection matrix.
         *
         *	@return	Projection Matrix.
         */
        Matrix getProjectionMatrix();
        void setProjectionMatrix(const Matrix& mat);
        void setProjectionMatrix(Matrix&& mat);

        /**
         *	@brief	Get current MV amtrix.
         *
         *	@return	ModelView Matrix.
         */
        Matrix getViewMatrix();
        
        std::array<float, 4> getClearColor();

        void updateListeners();
        void setViewMatrix(const Matrix& mat);
        void setViewMatrix(Matrix&& mat);

        /**
         *	@brief	Calculate normals on GPU.
         *
         *	@retval True if yes.
         *  @retval False if calcualte on CPU.
         */
        bool is_gpu_normal() const
        {
            return m_gpu_normal;
        }
        
        int getPassId(std::string passName);
        
        /**
         *	@brief	Current frame.
         *
         *	@return	Current frame.
         */
        float get_frame() const
        {
            return m_frame;
        }

        /**
         *	@brief	Get window.
         *
         *	@return	AWindow pointer.
         */
        AWindow* GetWindow()
        {
            return m_window;
        }

        /**
         *	@brief	Set uniforms in current shader
         *
         *	@param 	uniform 	uniforms to set
         */
        void SetUniforms(Uniform* uniform);

        /**
         *	@brief	Set uniforms in current shader
         *
         *  @param 	uniform 	uniforms to set
         */
        void SetUniforms(Uniforms& uniforms);
        void SetUniforms(std::vector<Uniform*>& uniforms);

        void SetUniform(Uniform* uniforms);

        /**
         *	@brief	Send current lights to shader as uniforms
         */
        void UniformLights();

        /**
         *	@brief	Add lights to renderer list, lights will be sent to shader as uniform array when shader is set
         *
         *	@param 	lights 	Pointer to array of lights
         */
        void AddLights(Uniformptr lights);
        void AddLights(Uniform* lights);


        void setQuatCamera(QuaternionCamera* quatCamera);
        QuaternionCamera* getQuatCamera() const;
        bool isDepthTestEnableState() const;
        bool isBlendEnableState() const;

        void setShader(ShaderDrw* actualshader);

        ShaderDrw* getShader() const;

        void setWindow(AWindow* window);
        AWindow* getWindow() const;
        void setColorPick(bool colorPick);
        bool isColorPick() const;
        void setShadowMapPass(bool shadowMapPass);
        bool isShadowMapPass() const;
        void setAttrib(const VertexData& data);
        void releaseAttrib(const VertexData& data);
        void setTriangles(const IndexData& buff);
        void releaseTriangles(const IndexData& buff);
        void setGeometryBatch(bool geometryBatch);
        bool isGeometryBatch() const;
        //config vars

        friend class IOnAexRendererUpdate;
        bool InitAPI(AWindow* window, uint32_t enables = 0, uint32_t disables = 0);
        void FinishAPI();
        void BeginFrame();
        void BeginFrame(uint32_t buffers);
        void EndFrame();
        void pushCurrentPassID(int currentPassID);
        void popCurrentPassID();
        int getCurrentPassID() const;
        
    private:

        typedef std::list<IOnAexRendererUpdate*> OnVPUpdateListeners;
        Uniformptr m_lightsUniArray;
        std::array<float, 4> m_clearColor;
        aex::string m_prefix;
        bool m_gpu_normal;
        AexShader m_currentShaderProgram;
        float m_frame;
        float m_lineWidth;
        std::array<Matrix, 3> m_VPMatrices;

        QuaternionCamera* m_quatCamera;
        ShaderDrw* m_actualshader;
        
        AWindow* m_window;
        GraphicsDevice m_api;
        bool     m_BlendEnableState;
        bool     m_DepthTestEnableState;
        bool     m_colorPick = false;
        bool     m_geometryBatch = false;
        bool     m_shadowMapPass = false;
        OnVPUpdateListeners m_updateListeners;
        ShaderStack m_shaderProgramStack;
        PassStack m_passIDStack;
        
    };
}

#endif /* defined(__aexolGL__AexRenderer__) */

/** @file utils.hpp
*  @brief Definition of utils.hpp
*
*  @author Kamil Matysiewicz
*  @bug No known bugs.
*/

#ifndef __aexolGL__utils__
#define __aexolGL__utils__
#include <aex/Matrix.hpp>
#include <aex/Vector3.hpp>
#include <array>
#include <string>
namespace aex
{
    /**
     *	@brief	Print SDL error.
     *
     *	@param 	string 	TAG.
     */
    void fatalError(const char* string);

    /**
     *	@brief	Get shader compilation log
     *
     *	@param 	obj 	Shader
     *
     *	@return	log
     */
    aex::string getShaderLog(uint32_t obj);

    /**
     *	@brief	Get shader logs.
     *
     *	@param 	obj 	Shader object.
     *
     *	@return	Log.
     */
    aex::string getShaderProgramLog(uint32_t obj);

    aex::math::Vector3 vectorToClipSpace(const aex::math::Vector3& vec, const aex::math::Matrix& Projection, const aex::math::Matrix& ModelView);

    template <typename T, typename U>
    struct IsFirstEqual
    {
        IsFirstEqual(T t) : _val(t) {}
        bool operator()(const std::pair<T, U>& cmp)
        {
            return cmp.first == _val;
        }
    private:
        T _val;
    };
}

#endif /* defined(__aexolGL__utils__) */

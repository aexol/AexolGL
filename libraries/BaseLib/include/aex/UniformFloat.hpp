/** @file UniformBool.hpp
 *  @brief Definition of UniformBool.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__UniformFloat__
#define __aexolGL__UniformFloat__

#include <aex/UniformPrimitive.hpp>
#include <aex/Vector2.hpp>
#include <aex/Vector3.hpp>
#include <string>
#include "Point.hpp"

namespace aex
{

class AEX_PUBLIC_API UniformFloat : public UniformPrimitive
{
public:
    /**
     *	@brief	Ctor
     *
     *	@param 	value 	Uniform value.
     *	@param 	name 	uniform name in program.
     */
    UniformFloat();
    explicit UniformFloat(const UniformFloat&);
    explicit UniformFloat(UniformFloat&&);
    explicit UniformFloat(float value);
    explicit UniformFloat(const aex::string& name);
    UniformFloat& operator=(const UniformFloat&);
    UniformFloat& operator=(UniformFloat&&);
    UniformFloat(float value, const aex::string& name);
    virtual ~UniformFloat();
    /**
     *	@brief	Specify uniform value.
     */
    void setValue(float value);
    Uniform* copy();
    float getValue() const;
protected:
    void checkUniform(void* uInfo);
private:
    float m_value;
};

//////////////////////////////////////////Float2

class AEX_PUBLIC_API UniformFloat2 : public UniformPrimitive
{
public:
    /**
     *	@brief	Ctor
     *
     *	@param 	value 	Uniform value.
     *	@param 	name 	uniform name in program.
     */
    UniformFloat2();
    explicit UniformFloat2(aex::math::Vector2 value);
    explicit UniformFloat2(const aex::string& name);
    explicit UniformFloat2(const UniformFloat2&);
    explicit UniformFloat2(UniformFloat2&&);
    UniformFloat2(float x , float y);
    UniformFloat2(aex::math::Vector2 value, const aex::string& name);
    UniformFloat2(float x , float y, const aex::string& name);
    UniformFloat2& operator=(const UniformFloat2&);
    UniformFloat2& operator=(UniformFloat2&&);
    virtual ~UniformFloat2();
    /**
     *	@brief	Specify uniform value.
     */
    Uniform* copy();
    void setValue(aex::math::Vector2 value);
    void setValue(float x, float y);
    aex::math::Vector2 getValue() const;
protected:
    void checkUniform(void* uInfo);
private:
    aex::math::Vector2 m_value;
};

//////////////////////////////////////////Float3

class AEX_PUBLIC_API UniformFloat3 : public UniformPrimitive
{
public:
    /**
     *	@brief	Ctor
     *
     *	@param 	value 	Uniform value.
     *	@param 	name 	uniform name in program.
     */

    UniformFloat3();
    explicit UniformFloat3(const UniformFloat3&);
    explicit UniformFloat3(UniformFloat3&&);
    explicit UniformFloat3(aex::math::Vector3 value);
    explicit UniformFloat3(const aex::string& name);
    UniformFloat3(float x, float y, float z);
    UniformFloat3(aex::math::Vector3 value, const aex::string& name);
    UniformFloat3(float x , float y, float z, const aex::string& name);
    UniformFloat3& operator=(const UniformFloat3&);
    UniformFloat3& operator=(UniformFloat3&&);
    virtual ~UniformFloat3();
    /**
     *	@brief	Specify uniform value.
     */
    void setValue(aex::math::Vector3 value);
    void setValue(float x, float y, float z);
    void setValue(int r, int g, int b);
    Uniform* copy();
    aex::math::Vector3 getValue() const;
protected:
    void checkUniform(void* uInfo);
private:
    aex::math::Vector3 m_value;
};

//////////////////////////////////////////Float4

class AEX_PUBLIC_API UniformFloat4 : public UniformPrimitive
{
public:
    /**
     *	@brief	Ctor
     *
     *	@param 	value 	Uniform value.
     *	@param 	name 	uniform name in program.
     */

    UniformFloat4();
    explicit UniformFloat4(const UniformFloat4&);
    explicit UniformFloat4(UniformFloat4&&);
    explicit UniformFloat4(aex::FPoint4D  value);
    explicit UniformFloat4(const aex::string& name);
    UniformFloat4(float x, float y, float z, float w);
    UniformFloat4(aex::FPoint4D , const aex::string& name);
    UniformFloat4(float x , float y, float z, float w, const aex::string& name);
    UniformFloat4& operator=(const UniformFloat4&);
    UniformFloat4& operator=(UniformFloat4&&);
    virtual ~UniformFloat4();
    /**
     *	@brief	Specify uniform value.
     */
    void setValue(aex::FPoint4D  value);
    void setValue(float x, float y, float z, float w);
    void setValue(int r, int g, int b, int a);
    //void setValue(Std::ar);
    
    Uniform* copy();
    aex::FPoint4D getValue() const;
protected:
    void checkUniform(void* uInfo);
private:
    aex::FPoint4D m_value;
};

typedef aex::shared_ptr<UniformFloat>  UniformFloat_ptr;
typedef aex::shared_ptr<UniformFloat2> UniformFloat2_ptr;
typedef aex::shared_ptr<UniformFloat3> UniformFloat3_ptr;
typedef aex::shared_ptr<UniformFloat4> UniformFloat4_ptr;

}

#endif /* defined(__aexolGL__UniformFloat__) */

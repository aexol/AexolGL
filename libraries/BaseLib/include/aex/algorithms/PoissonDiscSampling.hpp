/* 
 * File:   PoissonDiscSampling.hpp
 * Author: mritke
 *
 * Created on 26 października 2015, 14:06
 */

#ifndef POISSONDISCSAMPLING_HPP
#define	POISSONDISCSAMPLING_HPP
#include <aex/Vector2.hpp>
#include <aex/Vector3.hpp>

namespace aex
{

/////////////////////////////////////////2D//////////////////////////////////////

    class AEX_PUBLIC_API PoissonPoint2D {
    public:
        PoissonPoint2D(aex::math::Vector2 value, float dist);

        aex::math::Vector2 point;
        float range;

        bool isValueInRange(aex::math::Vector2 val);

    };

    class AEX_PUBLIC_API IPoissonDenisityFunc2D {
    public:
        IPoissonDenisityFunc2D();
        virtual float getDenisityAtPoint(aex::math::Vector2 value)=0;

    };

    class AEX_PUBLIC_API PoissonDenisityFuncDefault2D : public IPoissonDenisityFunc2D {
    public:
        PoissonDenisityFuncDefault2D();
        float getDenisityAtPoint(aex::math::Vector2 value);

    };

    class AEX_PUBLIC_API PoissonDenisityFuncRadius2D : public IPoissonDenisityFunc2D {
        aex::math::Vector2 m_middle;

    public:

        PoissonDenisityFuncRadius2D(aex::math::Vector2 middle =aex::math::Vector2(0.5f, 0.5f));

        float getDenisityAtPoint(aex::math::Vector2 value);
        void setMiddle(aex::math::Vector2 middle);
        aex::math::Vector2 getMiddle() const;

    };

    class AEX_PUBLIC_API PoissonDiscSampleGenerator2D {
    public:

        PoissonDiscSampleGenerator2D();
        virtual ~PoissonDiscSampleGenerator2D() = default;
        std::vector<aex::math::Vector2> generateSamples(float minDist, uint32_t newPointsNumber, uint32_t shootsPerPoint);

        void setDenisityFunc(IPoissonDenisityFunc2D* denisityFunc);
        IPoissonDenisityFunc2D* getDenisityFunc();
        void setStartPoint(aex::math::Vector2 startPoint);
        aex::math::Vector2 getStartPoint() const;
        
    private:
        aex::math::Vector2 m_rectSize;
        IPoissonDenisityFunc2D* m_denisityFunc = nullptr;

        aex::math::Vector2 m_startPoint;
        bool m_useStartPoint = false;

        virtual aex::math::Vector2 generateRandomPointAround(const aex::math::Vector2& point, float minDist);
        bool isInRectangle(const aex::math::Vector2& point) const;

    };
    /////////////////////////////////////////2D//////////////////////////////////////


    /////////////////////////////////////////3D//////////////////////////////////////

    class AEX_PUBLIC_API PoissonPoint3D {
    public:
        PoissonPoint3D(aex::math::Vector3 value, float dist);

        aex::math::Vector3 point;
        float range;

        bool isValueInRange(aex::math::Vector3 val);

    };

    class AEX_PUBLIC_API IPoissonDenisityFunc3D {
    public:
        IPoissonDenisityFunc3D();
        virtual float getDenisityAtPoint(aex::math::Vector3 value)=0;

    };

    class AEX_PUBLIC_API PoissonDenisityFuncDefault3D : public IPoissonDenisityFunc3D {
    public:
        PoissonDenisityFuncDefault3D();
        float getDenisityAtPoint(aex::math::Vector3 value);

    };

    class AEX_PUBLIC_API PoissonDenisityFuncRadius3D : public IPoissonDenisityFunc3D {
        aex::math::Vector3 m_middle;

    public:

        PoissonDenisityFuncRadius3D(aex::math::Vector3 middle =aex::math::Vector3(0.5f, 0.5f));

        float getDenisityAtPoint(aex::math::Vector3 value);
        void setMiddle(aex::math::Vector3 middle);
        aex::math::Vector3 getMiddle() const;

    };
    
    class AEX_PUBLIC_API PoissonDiscSampleGenerator3D {

    public:

        PoissonDiscSampleGenerator3D();
        std::vector<aex::math::Vector3> generateSamples(float minDist, uint32_t newPointsNumber, uint32_t shootsPerPoint);

        void setDenisityFunc(IPoissonDenisityFunc3D* denisityFunc);
        IPoissonDenisityFunc3D* getDenisityFunc();
        void setStartPoint(aex::math::Vector3 startPoint);
        aex::math::Vector3 getStartPoint() const;
        
    private:
        aex::math::Vector3 m_rectSize;
        IPoissonDenisityFunc3D* m_denisityFunc = nullptr;

        aex::math::Vector3 m_startPoint;
        bool m_useStartPoint = false;

        virtual aex::math::Vector3 generateRandomPointAround(const aex::math::Vector3& point, float minDist);
        bool isInRectangle(const aex::math::Vector3& point) const;


    };


}
/////////////////////////////////////////3D//////////////////////////////////////


#endif	/* POISSONDISCSAMPLING_HPP */


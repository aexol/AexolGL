/*
 * File:   OverhauserSpline.hpp
 * Author: mritke
 *
 * Created on April 24, 2014, 3:07 PM
 */

#ifndef OVERHAUSERSPLINE_HPP
#define	OVERHAUSERSPLINE_HPP

#include "Vector3.hpp"


namespace aex
{

    class AEX_PUBLIC_API Spline
    {
    public:
        Spline();
        Spline(const Spline& s);
        ~Spline();

        aex::math::Vector3 GetInterpolatedSplinePoint(float t);
        aex::math::Vector3 GetInterpolatedSplineAdvance(float delta_t);
        void AddSplinePoint(const aex::math::Vector3& v);
        int GetNumPoints();
        aex::math::Vector3& GetNthPoint(int n);
        int bounds(int pp);
        void scaleProgress(float scale);
        std::vector<aex::math::Vector3> GenerateLine(int resolutionPoints, float begin = 0.0f, float end = 1.0f);
        virtual aex::math::Vector3 Eq(float t, const aex::math::Vector3& p1, const aex::math::Vector3& p2,
                                  const aex::math::Vector3& p3, const aex::math::Vector3& p4) = 0;

        void SetProgress(float progress);
        float GetProgress() const;

        float GetDelta_t() const;
        void SetSplinePoints(std::vector<aex::math::Vector3> points);
        std::vector<aex::math::Vector3> GetSplinePoints();
        void SetClosed(bool Closed);
        bool IsClosed() const;
        
    protected:
        std::vector<aex::math::Vector3> m_splinePoints;
        int m_lastSplineSize;
        float m_delta;
        float m_progress;
        bool m_isClosed;
        bool m_SplineChanged;

    };

    class AEX_PUBLIC_API OverhauserSpline : public Spline
    {
        aex::math::Vector3 Eq(float t, const aex::math::Vector3& p1, const aex::math::Vector3& p2, const aex::math::Vector3& p3, const aex::math::Vector3& p4);

    };

    class AEX_PUBLIC_API BezierSpline : public Spline
    {
        aex::math::Vector3 Eq(float t, const aex::math::Vector3& p1, const aex::math::Vector3& p2, const aex::math::Vector3& p3, const aex::math::Vector3& p4);

    };
}

#endif	/* OVERHAUSERSPLINE_HPP */


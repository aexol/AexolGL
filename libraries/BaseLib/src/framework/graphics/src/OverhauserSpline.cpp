#include <aex/OverhauserSpline.hpp>

namespace aex
{

    int Spline::bounds(int pp)
    {
        if(!m_isClosed)
        {
            if(pp < 0)
            {
                return 0;
            }
            else if(pp > m_splinePoints.size() - 1)
            {
                return m_splinePoints.size() - 1;
            }
            else
            {
                return pp;
            }
        }
        else
        {
            if(pp < 0)
            {
                return m_splinePoints.size() - pp;
            }
            else if(pp > m_splinePoints.size() - 1)
            {
                return pp % m_splinePoints.size();
            }
            else
            {
                return pp;
            }
        }
    }

    Spline::Spline()
// : m_splinePoints(), m_delta(0.0f), m_progress(0.0f), m_isClosed(false)
    {

    }

    void Spline::SetProgress(float progress)
    {
        m_progress = progress;
    }

    float Spline::GetProgress() const
    {
        return m_progress;
    }

    float Spline::GetDelta_t() const
    {
        return m_delta;
    }

    void Spline::SetSplinePoints(std::vector<aex::math::Vector3> points)
    {
        m_delta = 1.0f / (float) points.size();
        m_splinePoints = points;
    }

    std::vector<aex::math::Vector3> Spline::GetSplinePoints()
    {
        return m_splinePoints;
    }

    void Spline::SetClosed(bool Closed)
    {
        m_isClosed = Closed;
    }

    bool Spline::IsClosed() const
    {
        return m_isClosed;
    }

    Spline::Spline(const Spline& s)
    {

        m_splinePoints = s.m_splinePoints;
        m_delta = s.m_delta;
    }

    Spline::~Spline()
    {
    }

    aex::math::Vector3 OverhauserSpline::Eq(float t, const aex::math::Vector3& p1, const aex::math::Vector3& p2, const aex::math::Vector3& p3, const aex::math::Vector3& p4)
    {
        float t2 = t * t;
        float t3 = t2 * t;

        float b1 = 0.5f * (-t3 + 2.0f * t2 - t);
        float b2 = 0.5f * (3.0f * t3 - 5.0f * t2 + 2.0f);
        float b3 = 0.5f * (-3.0f * t3 + 4.0f * t2 + t);
        float b4 = 0.5f * (t3 - t2);

        return (p1 * b1 + p2 * b2 + p3 * b3 + p4 * b4);
    }

    aex::math::Vector3 BezierSpline::Eq(float t, const aex::math::Vector3& p0, const aex::math::Vector3& p1, const aex::math::Vector3& p2, const aex::math::Vector3& p3)
    {
        float u = 1 - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;

        aex::math::Vector3 p = p0 * uuu; //first term
        p += p1 * 3.0f * uu * t; //second term
        p += p2 * 3.0f * u * tt; //third term
        p += p3 * ttt; //fourth term

        return p;
    }

    void Spline::AddSplinePoint(const aex::math::Vector3& v)
    {
        m_splinePoints.push_back(v);
        m_delta = 1.0f / (float) m_splinePoints.size();
    }

    aex::math::Vector3 Spline::GetInterpolatedSplineAdvance(float delta_t)
    {

        m_progress += delta_t;
        int tempint = (int) m_progress;
        float temp = m_progress - (float) tempint;

        return GetInterpolatedSplinePoint(temp);
    }

    aex::math::Vector3 Spline::GetInterpolatedSplinePoint(float t)
    {

        int p = (int)(t / m_delta);

        //#define BOUNDS(pp) { if (pp < 0) pp = 0; else if (pp >= (int)m_splinePoints.size()-1) pp = m_splinePoints.size() - 1; }

        int p0 = p - 1;
        p0 = bounds(p0);
        int p1 = p;
        p1 = bounds(p1);
        int p2 = p + 1;
        p2 = bounds(p2);
        int p3 = p + 2;
        p3 = bounds(p3);

        float lt = (t - m_delta * (float) p) / m_delta;

        return Eq(lt, m_splinePoints[p0], m_splinePoints[p1], m_splinePoints[p2], m_splinePoints[p3]);
    }

    std::vector<aex::math::Vector3> Spline::GenerateLine(int resolutionPoints, float begin, float end)
    {
        std::vector<aex::math::Vector3> ret;
        ret.resize(resolutionPoints);
        std::vector<aex::math::Vector3>::iterator iter = ret.begin();
        float stepSize = (end - begin) / (float) resolutionPoints;

        for(int i = 0 ; i <= resolutionPoints; i++)
        {
            *iter++ = GetInterpolatedSplinePoint(begin + ((float)i * stepSize));
        }

    }

    int Spline::GetNumPoints()
    {
        return m_splinePoints.size();
    }

    aex::math::Vector3& Spline::GetNthPoint(int n)
    {
        return m_splinePoints[n];
    }


}

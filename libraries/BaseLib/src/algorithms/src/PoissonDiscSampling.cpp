#include <aex/algorithms/PoissonDiscSampling.hpp>
namespace aex
{


    ///////////////////////////////////////////////2D///////////////////////////////
    PoissonPoint2D::PoissonPoint2D(aex::math::Vector2 value, float dist) : point(value), range(dist) {

    }

    bool PoissonPoint2D::isValueInRange(aex::math::Vector2 value) {
        if ((value - point).length() < range) {
            return true;
        } else {
            return false;
        }
    }

    IPoissonDenisityFunc2D::IPoissonDenisityFunc2D() {

    }

    PoissonDenisityFuncDefault2D::PoissonDenisityFuncDefault2D() {

    }

    float PoissonDenisityFuncDefault2D::getDenisityAtPoint(aex::math::Vector2 value)
    {
        UNUSED(value);
        return 1.0f;
    }

    PoissonDenisityFuncRadius2D::PoissonDenisityFuncRadius2D(aex::math::Vector2 middle) : m_middle(middle) {

    }

    float PoissonDenisityFuncRadius2D::getDenisityAtPoint(aex::math::Vector2 value) {
        return 1.0f - (sqrt((m_middle - value).length()));
    }

    void PoissonDenisityFuncRadius2D::setMiddle(aex::math::Vector2 middle) {
        m_middle = middle;
    }

    aex::math::Vector2 PoissonDenisityFuncRadius2D::getMiddle() const {
        return m_middle;
    }

    PoissonDiscSampleGenerator2D::PoissonDiscSampleGenerator2D() {

    }

    aex::math::Vector2 PoissonDiscSampleGenerator2D::generateRandomPointAround(const aex::math::Vector2& point, float minDist) {

        float r2 = aex::math::Math::randomFloat(0.0f, 360.0f);
        float radius = minDist * (aex::math::Math::randomFloat(0.0f, 1.0f) + 1.0f);

        return point+aex::math::Math::point_on_circle(r2 , radius);
    }

    PoissonPoint2D popRandom(std::vector<PoissonPoint2D>& Points) {
        int Idx = aex::math::Math::randomInt(0, Points.size());
        PoissonPoint2D P = Points[ Idx ];
        Points.erase(Points.begin() + Idx);

        return P;
    }

    bool PoissonDiscSampleGenerator2D::isInRectangle(const aex::math::Vector2& point) const {

        return point.x >= 0 && point.y >= 0 && point.x <= 1 && point.y <= 1;
    }

    bool IsInCircle(const aex::math::Vector2& point) {
        float fx = point.x - 0.5f;
        float fy = point.y - 0.5f;

        return ( fx * fx + fy * fy) <= 0.25f;
    }

    bool checkProcessingList(std::vector<PoissonPoint2D>& processList, aex::math::Vector2 point) {
        for (std::vector<PoissonPoint2D>::iterator iter = processList.begin(); iter != processList.end(); iter++) {
            if (iter->isValueInRange(point)) {

                return false;
            }
        }

        return true;
    }

    std::vector<aex::math::Vector2> convertToVec2(std::vector<PoissonPoint2D>& points) {
        std::vector<aex::math::Vector2> ret;
        ret.resize(points.size());

        for(uint32_t i = 0; i < points.size(); i++) {
            ret[i] = points[i].point;
        }
        return ret;
    }

    std::vector<aex::math::Vector2> PoissonDiscSampleGenerator2D::generateSamples(float minDist, uint32_t newPointsNumber, uint32_t shootsPerPoint) {
        std::vector<PoissonPoint2D> samplePoints;
        std::vector<PoissonPoint2D> processList;

        aex::math::Vector2 startingPoint;

        if (m_useStartPoint) {
            startingPoint = m_startPoint;
        } else {
            startingPoint = aex::math::Vector2(aex::math::Math::randomFloat(0.0f, 1.0f), aex::math::Math::randomFloat(0.0f, 1.0f));
        }

        float denisityFactor = 1.0f;

        if (m_denisityFunc != nullptr) {
            denisityFactor = m_denisityFunc->getDenisityAtPoint(startingPoint);
        }
        PoissonPoint2D firstPoint = PoissonPoint2D(startingPoint, minDist * denisityFactor);


        processList.push_back(firstPoint);
        samplePoints.push_back(firstPoint);

        uint32_t processSize=1;
        uint32_t sampleSize=1;

        while (!processList.empty() && sampleSize < newPointsNumber) {

            size_t randomIdx = aex::math::Math::randomInt(0, processSize-1);
            PoissonPoint2D poissonPoint = processList[randomIdx];

            bool missed = true;

            for(uint32_t i = 0; i < shootsPerPoint; i++) {

                aex::math::Vector2 nPoint = generateRandomPointAround(poissonPoint.point, poissonPoint.range);
                bool fits = isInRectangle(nPoint);

                if (fits) {
                    if (checkProcessingList(samplePoints, nPoint)) {

                        float denisityFactor = 1.0f;
                        if (m_denisityFunc != nullptr) {
                            denisityFactor = m_denisityFunc->getDenisityAtPoint(nPoint);
                        }

                        processList.emplace_back(nPoint, minDist * denisityFactor);
                        samplePoints.emplace_back(nPoint, minDist * denisityFactor);
                        processSize++;
                        sampleSize++;
                        missed = false;
                        break;
                    }
                }
            }

            if (missed) {
                processList.erase(processList.begin() + randomIdx);
                processSize--;
            }

        }

        if(sampleSize<newPointsNumber)
        {

        }


        return convertToVec2(samplePoints);

    }

    void PoissonDiscSampleGenerator2D::setDenisityFunc(IPoissonDenisityFunc2D* denisityFunc) {
        m_denisityFunc = denisityFunc;
    }

    IPoissonDenisityFunc2D* PoissonDiscSampleGenerator2D::getDenisityFunc() {
        return m_denisityFunc;
    }

    void PoissonDiscSampleGenerator2D::setStartPoint(aex::math::Vector2 startPoint) {
        m_startPoint = startPoint;
        m_useStartPoint=true;
    }

    aex::math::Vector2 PoissonDiscSampleGenerator2D::getStartPoint() const {
        return m_startPoint;
    }

    ///////////////////////////////////////////////2D///////////////////////////////

    ///////////////////////////////////////////////3D///////////////////////////////

    PoissonPoint3D::PoissonPoint3D(aex::math::Vector3 value, float dist) : point(value), range(dist) {

    }

    bool PoissonPoint3D::isValueInRange(aex::math::Vector3 value) {
        if ((value - point).length() < range) {
            return true;
        } else {
            return false;
        }
    }

    IPoissonDenisityFunc3D::IPoissonDenisityFunc3D() {

    }

    PoissonDenisityFuncDefault3D::PoissonDenisityFuncDefault3D() {

    }

    float PoissonDenisityFuncDefault3D::getDenisityAtPoint(aex::math::Vector3 value) {
        UNUSED(value);
        return 1.0f;
    }

    PoissonDenisityFuncRadius3D::PoissonDenisityFuncRadius3D(aex::math::Vector3 middle) : m_middle(middle) {

    }

    float PoissonDenisityFuncRadius3D::getDenisityAtPoint(aex::math::Vector3 value) {
        return 1.0f - (sqrt((m_middle - value).length()));
    }

    void PoissonDenisityFuncRadius3D::setMiddle(aex::math::Vector3 middle) {
        m_middle = middle;
    }

    aex::math::Vector3 PoissonDenisityFuncRadius3D::getMiddle() const {
        return m_middle;
    }


    PoissonDiscSampleGenerator3D::PoissonDiscSampleGenerator3D() 
    {

    }

    aex::math::Vector3 point_on_sphere(float angleDegX, float angleDegY, float radius) {
        aex::math::Vector3 ret;

        angleDegX*=aex::math::Math::V_DEG2RAD;
        angleDegY*=aex::math::Math::V_DEG2RAD;

        float sinX= aex::math::Math::fast_sine_precise(angleDegX);
        float cosYr= aex::math::Math::fast_cosine_precise(angleDegY)* radius;

        ret.x = sinX * cosYr ;
        ret.z = sinX * aex::math::Math::fast_sine_precise(angleDegY) * radius;
        ret.y = cosYr;

        return ret;
    }

    aex::math::Vector3 PoissonDiscSampleGenerator3D::generateRandomPointAround(const aex::math::Vector3& point, float minDist) {


        float r2 = aex::math::Math::randomFloat(-180.0f, 180.0f);
        float r3 = aex::math::Math::randomFloat(-90.0f, 90.0f);
        float radius = minDist * (aex::math::Math::randomFloat(0.0f, 1.0f) + 1.0f);

        return point + point_on_sphere(r2, r3, radius);
    }

    PoissonPoint3D popRandom(std::vector<PoissonPoint3D>& Points) {
        int Idx = aex::math::Math::randomInt(0, Points.size());
        PoissonPoint3D P = Points[ Idx ];
        Points.erase(Points.begin() + Idx);

        return P;
    }

    bool PoissonDiscSampleGenerator3D::isInRectangle(const aex::math::Vector3& point) const {

        return point.x >= 0.0f && point.y >= 0.0f && point.z >= 0.0f && point.x <= 1.0f && point.y <= 1.0f && point.z <= 1.0f;
    }

    bool IsInCircle(const aex::math::Vector3& point) {
        float fx = point.x - 0.5f;
        float fy = point.y - 0.5f;

        return ( fx * fx + fy * fy) <= 0.25f;
    }

    bool checkProcessingList(std::vector<PoissonPoint3D>& processList, aex::math::Vector3 point) {
        for (std::vector<PoissonPoint3D>::iterator iter = processList.begin(); iter != processList.end(); iter++) {
            if (iter->isValueInRange(point)) {
                return false;
            }
        }

        return true;
    }

    std::vector<aex::math::Vector3> convertToVec3(std::vector<PoissonPoint3D>& points) {
        std::vector<aex::math::Vector3> ret;
        ret.resize(points.size());

        for(uint32_t i = 0; i < points.size(); i++) {
            ret[i] = points[i].point;
        }
        return ret;
    }

    std::vector<aex::math::Vector3> PoissonDiscSampleGenerator3D::generateSamples(float minDist, uint32_t newPointsNumber, uint32_t shootsPerPoint) {
        std::vector<PoissonPoint3D> samplePoints;
        std::vector<PoissonPoint3D> processList;

        aex::math::Vector3 startingPoint;

        if (m_useStartPoint) {
            startingPoint = m_startPoint;
        } else {
            startingPoint = aex::math::Vector3(aex::math::Math::randomFloat(0.0f, 1.0f), aex::math::Math::randomFloat(0.0f, 1.0f));
        }

        float denisityFactor = 1.0f;

        if (m_denisityFunc != nullptr) {
            denisityFactor = m_denisityFunc->getDenisityAtPoint(startingPoint);
        }
        PoissonPoint3D firstPoint = PoissonPoint3D(startingPoint, minDist * denisityFactor);


        processList.push_back(firstPoint);
        samplePoints.push_back(firstPoint);



        while (!processList.empty() && samplePoints.size() < newPointsNumber) {

            size_t randomIdx = aex::math::Math::randomInt(0, processList.size() - 1);
            PoissonPoint3D poissonPoint = processList[randomIdx];

            bool missed = true;

            for(uint32_t i = 0; i < shootsPerPoint; i++) {

                aex::math::Vector3 nPoint = generateRandomPointAround(poissonPoint.point, poissonPoint.range);
                bool fits = isInRectangle(nPoint);

                if (fits) {
                    if (checkProcessingList(samplePoints, nPoint)) {

                        float denisityFactor = 1.0f;
                        if (m_denisityFunc != nullptr) {
                            denisityFactor = m_denisityFunc->getDenisityAtPoint(nPoint);
                        }

                        processList.emplace_back(nPoint, minDist * denisityFactor);
                        samplePoints.emplace_back(nPoint, minDist * denisityFactor);

                        missed = false;
                        break;
                    }
                }
            }

            if (missed) {
                processList.erase(processList.begin() + randomIdx);
            }

        }
        return convertToVec3(samplePoints);
    }

    void PoissonDiscSampleGenerator3D::setDenisityFunc(IPoissonDenisityFunc3D* denisityFunc) {
        m_denisityFunc = denisityFunc;
    }

    IPoissonDenisityFunc3D* PoissonDiscSampleGenerator3D::getDenisityFunc() {
        return m_denisityFunc;
    }

    void PoissonDiscSampleGenerator3D::setStartPoint(aex::math::Vector3 startPoint) {
        m_startPoint = startPoint;
        m_useStartPoint = true;
    }

    aex::math::Vector3 PoissonDiscSampleGenerator3D::getStartPoint() const {
        return m_startPoint;
    }

    ///////////////////////////////////////////////3D///////////////////////////////

}

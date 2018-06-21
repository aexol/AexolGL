/*
 * File:   AexInstances.hpp
 * Author: mritke
 *
 * Created on October 29, 2014, 1:22 PM
 */

#ifndef AEXINSTANCES_HPP
#define AEXINSTANCES_HPP

#include <aex/Aex.hpp>
#include <aex/QuaternionCamera.hpp>
#include <algorithm>
#include <numeric>

#include "TriangulateShape.hpp"
namespace aex {

    namespace AexInstance {

        template<typename T>
        class AexInstances : public DrawObject {
        public:
            typedef std::function<bool(T, T) > SortFunc;
            typedef std::function<bool(T) > FilterFunc;
        public:

            AexInstances();
            AexInstances(std::vector<T> instances);
            void draw(AexRenderer* gl);
            void finishdraw(AexRenderer* gl);
            void needsUpdate();
            void setInstances(std::vector<T> instances);
            std::vector<T>& getInstances();
            void addAex(T aex);
            void removeAex(T aex);
            void turnOffAex(T aex);
            void turnOnAex(T aex);
            void clear();
            void sort(SortFunc func);
            void filter(FilterFunc func, size_t max = 0);
            void setSortFunc(SortFunc func);
            void setFilterFunc(FilterFunc func);
            void setCameraSort(QuaternionCamera* cam);
            void setFrustumFilter(QuaternionCamera* cam);
            void addDistanceFilter(QuaternionCamera* cam, float distance);
            void update();
            
        private:
            typedef typename std::vector<T>::iterator vector_iterator;
            std::vector<T> m_instances;
            std::vector<bool> m_isVisible;
            SortFunc m_sortFunc;
            FilterFunc m_filterFunc;
            std::vector<FilterFunc> m_additionalFilters;
            size_t m_toDraw = 0;


        };

        template <typename T>
        std::vector<std::size_t> sort_permutation(
                const std::vector<T>& vec, size_t part,
                std::function<bool(T, T) > func) {
            std::vector<std::size_t> p(vec.size());

            std::iota(p.begin(), p.end(), 0);
            std::sort(p.begin(), p.begin() + part,
                    [&](std::size_t i, std::size_t j) {
                        return func(vec[i], vec[j]);
                    });

            return p;
        }

        template <typename T>
        std::vector<T> apply_permutation(
                const std::vector<T>& vec,
                const std::vector<std::size_t>& p) {

            std::vector<T> sorted_vec(p.size());

            std::transform(p.begin(), p.end(), sorted_vec.begin(),
                    [&](std::size_t i) {
                        return vec[i]; });
            return sorted_vec;
        }

        template<typename T>
        AexInstances<T>::AexInstances() {

        }

        template<typename T>
        AexInstances<T>::AexInstances(std::vector<T> instances) :
        m_instances(instances),
        m_isVisible(instances.size(), true),
        m_toDraw(instances.size()) {

        }

        template<typename T>
        void AexInstances<T>::draw(AexRenderer* gl) {

            if (gl->isShadowMapPass()) {
                for (size_t i = 0; i < m_instances.size(); ++i) {
                    m_instances[i]->draw(gl);
                    m_instances[i]->finishdraw(gl);
                }
            } else {
                for (size_t i = 0; i < m_toDraw; ++i) {

                    m_instances[i]->draw(gl);
                    m_instances[i]->finishdraw(gl);
                }
            }
        }

        template<typename T>
        void AexInstances<T>::finishdraw(AexRenderer* gl) {

        }

        template<typename T>
        void AexInstances<T>::needsUpdate() {

        }

        template<typename T>
        void AexInstances<T>::setInstances(std::vector<T> instances) {
            m_instances = instances;
        }

        template<typename T>
        std::vector<T>& AexInstances<T>::getInstances() {
            return m_instances;
        }

        template<typename T>
        void AexInstances<T>::addAex(T aex) {
            m_instances.insert(m_instances.begin() + m_toDraw, aex);
            m_isVisible.insert(m_isVisible.begin() + m_toDraw, true);
            ++m_toDraw;
        }

        template<typename T>
        void AexInstances<T>::removeAex(T aex) {

            vector_iterator it = find(m_instances.begin(), m_instances.end(), aex);
            if (it != m_instances.end()) {

                size_t dist = std::distance(m_instances.begin(), it);
                
                m_isVisible.erase(m_isVisible.begin()+(std::distance(m_instances.begin(), it)));
                m_instances.erase(it);

                if (dist < m_toDraw) {
                    --m_toDraw;
                }
            }
        }

        template<typename T>
        void AexInstances<T>::turnOffAex(T aex) {

            vector_iterator it = find(m_instances.begin(), m_instances.end(), aex);
            if (it != m_instances.end()) {
                size_t dist = std::distance(m_instances.begin(), it);

                m_isVisible[dist] = false;

                if (dist < m_toDraw) {
                    //LOG_DEBUG("turnOffAex: ", dist ,", ",  m_toDraw - 1 ,", ", m_instances.size());
                    std::swap(m_instances[dist], m_instances[m_toDraw - 1]);
                    std::swap(m_isVisible[dist], m_isVisible[m_toDraw - 1]);
                    --m_toDraw;
                }/*else if(m_toDraw == dist)
                {
                    --m_toDraw;
                }*/
            }
        }

        template<typename T>
        void AexInstances<T>::turnOnAex(T aex) {
            
            vector_iterator it = find(m_instances.begin(), m_instances.end(), aex);
            if (it != m_instances.end()) {
                size_t dist = std::distance(m_instances.begin(), it);
                //LOG_DEBUG("turnOnAex: ", dist ,", ",  m_toDraw ,", ", m_instances.size());
                {
                    m_isVisible[dist] = true;
                    if (m_toDraw < dist) {
                        //LOG_DEBUG("turnOnAex: ", "m_toDraw > dist");
                        std::swap(m_instances[dist], m_instances[m_toDraw]);
                        std::swap(m_isVisible[dist], m_isVisible[m_toDraw]);
                        ++m_toDraw;
                    }else if(m_toDraw == dist)
                    {
                        //LOG_DEBUG("turnOnAex: ", "m_toDraw == dist");
                        ++m_toDraw;
                    }
                }
            }
        }

        template<typename T>
        void AexInstances<T>::clear() {
            m_instances.clear();
            m_isVisible.clear();
        }

        template<typename T>
        void AexInstances<T>::sort(SortFunc func) {
            //            std::function<bool(bool, bool) > vissort = [](bool i, bool j) ->bool {
            //                return (i > j);
            //            };
            //            std::vector<std::size_t> permutationVisible = sort_permutation(m_isVisible, m_isVisible.size(), vissort);
            //            m_instances = apply_permutation(m_instances, permutationVisible);
            std::vector<std::size_t> permutation = sort_permutation(m_instances, m_toDraw, func);
            m_instances = apply_permutation(m_instances, permutation);
        }

        template<typename T>
        void AexInstances<T>::filter(FilterFunc func, size_t max) {
            size_t drawsize = 0;
            size_t upperbound = m_isVisible.size();
            if (max != 0 && max <= upperbound) {
                upperbound = max;
            }

            for (size_t i = 0; i < upperbound; ++i) {
                m_isVisible[i] = func(m_instances[i]);
                if (m_isVisible[i]) {
                    drawsize++;
                }
            }

            m_toDraw = drawsize;

            std::function<bool(bool, bool) > vissort = [](bool i, bool j) ->bool {
                return (i > j);
            };

            std::vector<std::size_t> permutationVisible = sort_permutation(m_isVisible, upperbound, vissort);
            m_instances = apply_permutation(m_instances, permutationVisible);
        }

        template<typename T>
        void AexInstances<T>::setCameraSort(QuaternionCamera* cam) {

            if (cam != nullptr) {
                m_sortFunc = [cam](T l, T r) -> bool {

                    return ((l->getCenterPoint() - cam->GetPosition()).squaredLength() > (r->getCenterPoint() - cam->GetPosition()).squaredLength());
                };
                sort(m_sortFunc);
            } else {
                LOG_ERR("Setting nullptr camera for sorting in AexInstances!");
            }

        }

        template<typename T>
        void AexInstances<T>::setFrustumFilter(QuaternionCamera* cam) {

            if (cam != nullptr) {
                m_filterFunc = [cam](T v) -> bool {

                    //return cam->getFrustum().containsPoint(v->getCenterPoint());
                    return cam->getFrustum().containsAABBbool(v->getOabb());

                };
            } else {
                LOG_ERR("Setting nullptr camera for frustum culling in AexInstances!");
            }
            //filter(m_filterFunc);
        }

        template<typename T>
        void AexInstances<T>::addDistanceFilter(QuaternionCamera* cam, float distance) {

            if (cam != nullptr) {

                float distanceSqr = distance*distance;
                auto filterFunc = [cam, distanceSqr](T v) -> bool {

                    //return cam->getFrustum().containsPoint(v->getCenterPoint());
                    Vector3 dist = (v->getCenterPoint() - cam->GetPosition());
                    return dist.squaredLength() < distanceSqr;
                };

                m_additionalFilters.push_back(filterFunc);
                //filter(m_filterFunc);
            } else {
                LOG_ERR("Setting nullptr camera for distance culling in AexInstances!");
            }
        }

        template<typename T>
        void AexInstances<T>::setSortFunc(SortFunc func) {
            m_sortFunc = func;
        }

        template<typename T>
        void AexInstances<T>::setFilterFunc(FilterFunc func) {
            m_filterFunc = func;
        }

        template<typename T>
        void AexInstances<T>::update() {

            if (m_filterFunc) {

                m_toDraw = m_isVisible.size();

                filter(m_filterFunc, m_toDraw);

                if (m_additionalFilters.size() > 0) {

                    for (size_t ii = 0; ii < m_additionalFilters.size(); ++ii) {
                        filter(m_additionalFilters[ii], m_toDraw);
                    }
                }
            }

            if (m_sortFunc) {
                sort(m_sortFunc);
            }

        }

    }

    using AexInstances = AexInstance::AexInstances<Aex_ptr>;
    using AexInstancesRaw = AexInstance::AexInstances<Aex*>;

}

#endif /* AEXINSTANCES_HPP */


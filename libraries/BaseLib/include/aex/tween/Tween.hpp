#ifndef TWEEN_AEX_TWEEN_HEADER
#define TWEEN_AEX_TWEEN_HEADER

#include "TweenAccessor.hpp"
#include "Tweenable.hpp"
#include "TweenPlayer.hpp"
#include <exception>

#include <memory>
#include <functional>

namespace aex {
    namespace tween {
        enum TWEENING {
            START = 1 << 0,
            END =  1 << 1
        };

        // THINGS TO IMPLEMENT:
        // - Pooling of objects in Pool in static method to, from, set etc
        template <typename T> class Tween : public Tweenable {
        protected:
            enum INTERPOLATION {
                EASING,
                PATH
                // ETC
            };
            enum TYPE {
                TO,
                FROM,
                SET,
                CALL
            };

            enum REPEAT
            {
                NORMAL,
                YOYO
            };

            Tween(T* obj, int typeID, seconds duration, std::shared_ptr<TweenAccessor<T>> accessor, TYPE type);
        public:
            static const int REPEAT_INFINITY = -1;
            virtual ~Tween();
            Tween<T>* target(const std::vector<float>& target);
            Tween<T>* interpolation(const std::function<float(float, float, float, float) >& easing); // here another interpolation eg, path function
            Tween<T>* repeat(int count);
            Tween<T>* repeatYoyo(int count);
            Tween<T>* repeatDelay(float delay);
            Tween<T>* callback(const std::function<void(int) >& callback);
            Tween<T>* cbFlags(int flags);

            void start(TweenPlayer& player);
        protected:
            virtual void step(seconds delta);
            
        public:
            static void setAccessor(std::shared_ptr<TweenAccessor<T>> accessor);
            static Tween<T>* to(T* obj, int typeID, seconds duration, std::shared_ptr<TweenAccessor<T>> accessor = nullptr);
            static Tween<T>* from(T* obj, int typeID, seconds duration, std::shared_ptr<TweenAccessor<T>> accessor = nullptr);
            static Tween<T>* set(T* obj, int typeID, std::shared_ptr<TweenAccessor<T>> accessor = nullptr);
            static Tween<T>* call();

        protected:
            static Tween<T>* getNewTween(T* obj, int typeID, seconds duration, std::shared_ptr<TweenAccessor<T>> accessor, TYPE type);
            static std::shared_ptr<TweenAccessor<T>> g_accessor;
            

        protected:
            T* m_obj;
            int m_typeID;
            seconds m_duration;
            std::shared_ptr<TweenAccessor<T>> m_accessor;
            TweenAccessor<T>* m_accessorPtr;
            TYPE m_type;
            std::vector<float> m_target;
            std::vector<float> m_startVals;
            std::vector<float> m_tmpVals;
            seconds m_deltaTimeSum;
            bool m_started;

            int m_repeatCount;
            int m_counter;
            seconds m_tmpDelay;
            seconds m_repeatDelay;
            REPEAT m_repeatType;
            int m_callbackFlags;
            std::function<void(int) > m_callback;

            INTERPOLATION m_interType;
            std::function<float(float, float, float, float) > m_easing;

            
        };
        template <typename T>
        std::shared_ptr<TweenAccessor<T>> Tween<T>::g_accessor = nullptr;

        // ========== Impl ===========
        template <typename T>
        Tween<T>::Tween(T* obj, int typeID, seconds duration, std::shared_ptr<TweenAccessor<T>> accessor, TYPE type) :
        m_obj(obj),
        m_typeID(typeID),
        m_duration(duration),
        m_accessor(accessor),
        m_accessorPtr(m_accessor.get()),
        m_type(type),
        m_deltaTimeSum(0),
        m_started(false),
        m_repeatCount(0),
        m_counter(0),
        m_tmpDelay(0),
        m_repeatDelay(0),
        m_repeatType(REPEAT::NORMAL),
        m_callbackFlags(0) { // trigered in target method
            m_interType = INTERPOLATION::EASING;
            m_easing = [](float t, float a, float b, float d) -> float {
                return t / d; // 1 to 1
            };
        }

        template <typename T>
        Tween<T>::~Tween() {
        }

        template <typename T>
        Tween<T>* Tween<T>::target(const std::vector<float>& target) {
            m_target = target;
            m_startVals.resize(target.size());
            m_tmpVals.resize(target.size());
            setDone(false);
            m_started = false;
            return this;
        }

        template <typename T>
        Tween<T>* Tween<T>::interpolation(const std::function<float(float, float, float, float) >& easing) {
            m_easing = easing;
            return this;
        }

        template <typename T>
        Tween<T>* Tween<T>::repeat(int count)
        {
            m_repeatCount = count;
            m_repeatType = REPEAT::NORMAL;
            return this;
        }

        template <typename T>
        Tween<T>* Tween<T>::repeatYoyo(int count)
        {
            m_repeatCount = count*2 - 1;
            m_repeatType = REPEAT::YOYO;
            return this;
        }

        template <typename T>
        Tween<T>* Tween<T>::repeatDelay(seconds delay)
        {
            m_repeatDelay = delay;
            return this;
        }

        template <typename T>
        Tween<T>* Tween<T>::callback(const std::function<void(int) >& callback) {
            m_callback = callback;
            m_callbackFlags = m_callbackFlags | TWEENING::END;
            return this;
        }

        template <typename T>
        Tween<T>* Tween<T>::cbFlags(int flags) {
            m_callbackFlags = m_callbackFlags | flags;
            return this;
        }

        // move it to tweenable ?
        template <typename T>
        void Tween<T>::start(TweenPlayer& player) {
            player.add(this);
            m_counter = 0;
        }

        template <typename T>
        void Tween<T>::step(seconds delta) {
            if (isDone())
                return;

            if(m_tmpDelay > 0)
            {
                m_tmpDelay -= delta;
                return;
            }

            if (!m_started) {
                m_started = true;
                switch (m_type) {
                    case TYPE::TO:
                        m_accessorPtr->getValues(m_obj, m_typeID, m_startVals);
                        break;
                    case TYPE::FROM:
                        m_startVals = m_target;
                        m_accessorPtr->getValues(m_obj, m_typeID, m_target);
                        break;
                    case TYPE::CALL:
                        m_callback(TWEENING::END);
                        setDone(true);
                        return;
                    default:
                        break;
                }
                if( (m_callbackFlags & static_cast<int>(TWEENING::START)) != 0 )
                    m_callback(TWEENING::START);
            }

            m_deltaTimeSum = m_deltaTimeSum + delta;
            // if end:
            if (m_type == TYPE::SET || m_deltaTimeSum >= m_duration) {
                m_deltaTimeSum = m_duration;
                m_accessorPtr->setValues(m_obj, m_typeID, m_target);

                if(m_counter < m_repeatCount || m_repeatCount < 0) // some repeatr or infinity
                {
                    m_counter++;
                    m_deltaTimeSum = 0;
                    if(m_repeatType == REPEAT::YOYO)
                    {
                        m_tmpVals = m_startVals;
                        m_startVals = m_target;
                        m_target = m_tmpVals;
                    }
                    m_tmpDelay = m_repeatDelay;
                    return;
                }

                setDone(true);
                if( (m_callbackFlags & static_cast<int>(TWEENING::END)) != 0 )
                    m_callback(TWEENING::END);
                return;
            }

            float clampfTime = (float) m_deltaTimeSum / m_duration;
            float y = 0;

            switch (m_interType) {
                case INTERPOLATION::PATH:
                    // TODO
                    break;
                case INTERPOLATION::EASING:
                    y = m_easing(clampfTime, 0.0f, 1.0f, 1.0f);
                default:
                    break;
            }

            for (size_t i = 0; i < m_startVals.size(); i++) {
                float valueDelta = m_target[i] - m_startVals[i];
                m_tmpVals[i] = m_startVals[i] + valueDelta * y;
            }

            m_accessorPtr->setValues(m_obj, m_typeID, m_tmpVals);
        }

        template <typename T>
        void Tween<T>::setAccessor(std::shared_ptr<TweenAccessor<T>> accessor) {

            if (g_accessor != accessor)
                g_accessor = accessor;
        }

        template <typename T>
        Tween<T>* Tween<T>::to(T* obj, int typeID, seconds duration, std::shared_ptr<TweenAccessor<T>> accessor) {
            return getNewTween(obj, typeID, duration, accessor, TYPE::TO);
        }

        template <typename T>
        Tween<T>* Tween<T>::from(T* obj, int typeID, seconds duration, std::shared_ptr<TweenAccessor<T>> accessor) {
            return getNewTween(obj, typeID, duration, accessor, TYPE::FROM);
        }

        template <typename T>
        Tween<T>* Tween<T>::set(T* obj, int typeID, std::shared_ptr<TweenAccessor<T>> accessor) {
            return getNewTween(obj, typeID, 0, accessor, TYPE::SET);
        }

        template <typename T>
        Tween<T>* Tween<T>::call()
        {
            return getNewTween(nullptr, 0, 0, nullptr, TYPE::CALL);
        }

        template <typename T>
        Tween<T>* Tween<T>::getNewTween(T* obj, int typeID, seconds duration, std::shared_ptr<TweenAccessor<T>> accessor, TYPE type) {
            if (accessor) {
                return new Tween<T>(obj, typeID, duration, accessor, type);
            } else {
                if (g_accessor) {
                    return new Tween<T>(obj, typeID, duration, g_accessor, type);
                } else {
                    if(type != TYPE::CALL)
                        throw std::runtime_error("Accessor for tween not setted");
                    else
                        return new Tween<T>(obj, typeID, duration, accessor, type);
                }
            }
        }
    }
}


#endif

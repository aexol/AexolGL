/**************************************************************
 *                                                            * 
 *      Filename:   KeyFrameData.hpp                          * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 wrz 30 08:35:17                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#ifndef _AEX_FRAMEDATA_HPP
#define _AEX_FRAMEDATA_HPP

#include <aex/BasicAnimation.hpp>
#include <aex/Exceptions.hpp>

namespace aex
{
    DECLARE_AEX_EXCEPTION(FrameDataError);
    /* Forward declarations */
    class AexRenderer;
    /**
     * @brief Frame data container.
     *
     * Container for frames of animation. Container size is constant.
     *
     * @tparam FrameType Type of frame.
     */
    template<class FrameType>
    class FrameData
    {
    public:
        using frame_type = FrameType;
        using frame_container = std::vector<frame_type>;
        using iterator = typename frame_container::iterator;
        using const_iterator = typename frame_container::const_iterator;
        using reverse_iterator = typename frame_container::reverse_iterator;
        using const_reverse_iterator = typename frame_container::const_reverse_iterator;
        using difference_type = typename frame_container::difference_type;
        using size_type = typename frame_container::size_type;

        /**
         * @brief Default constructor.
         *
         * @param frameCount Frames in data, default 24.
         */
        explicit FrameData(const size_type frameCount = 24u) :
            m_data(frameCount)
        {}

        /**
         * @brief Cope frame data.
         *
         * @param rhs
         */
        explicit FrameData(const FrameData& rhs) = default;

        /**
         * @brief Move constructor.
         *
         * @param rhs
         */
        explicit FrameData(FrameData&& rhs) :
            m_data(std::move(rhs.m_data))
        {}

        /**
         * @brief Assign frame data.
         *
         * Frame data is of constant size. Both this and rhs must have the same size.
         *
         * @param rhs
         *
         * @return *this;
         *
         * @throw FrameDataError if sizes of data differ.
         */
        FrameData& operator=(const FrameData& rhs)
        {
            if(this != &rhs)
            {
                if(m_data.size() != rhs.m_data.size())
                    throw FrameDataError("Incompatible frame data.");
                m_data = rhs.m_data;
            }
            return *this;
        }

        /**
         * @brief Move assignment.
         *
         * Frame data is of constant size. Both this and rhs must have the same size.
         *
         * @param rhs Data to be assigned.
         *
         * @return 
         *
         * @throw FrameDataError if sizes of data differ.
         */
        FrameData& operator=(FrameData&& rhs)
        {
            if(this != &rhs)
            {
                if(m_data.size() != rhs.m_data.size())
                    throw FrameDataError("Incompatible frame data.");
                m_data = std::move(rhs.m_data);
            }
            return *this;
        }

        /**
         * @brief Return reference to frame at idx.
         *
         * This function does not perform any bound checking.
         *
         * @tparam T Frame index type.
         * @param idx Frame index.
         *
         * @return Reference to frame.
         */
        frame_type& operator[](const size_type& idx)
        {
            return m_data[idx];
        }

        const frame_type& operator[](const size_type& idx) const
        {
            return m_data[idx];
        }

        /**
         * @brief Return reference to frame at idx.
         *
         * This function checks whether idx is within bounds of data and throws FrameDataError if it's outside.
         *
         * @tparam T Frame index type.
         * @param idx Frame index.
         *
         * @return Reference to frame.
         *
         * @throw FrameDataError if idx out of range.
         */
        frame_type& at(const size_type& idx)
        {
            if(m_data.size() <= idx)
                throw FrameDataError("Frame index out of bounds.");
            return (*this)[idx];
        }

        const frame_type& at(const size_type& idx) const
        {
            if(m_data.size() <= idx)
                throw FrameDataError("Frame index out of bounds.");
            return (*this)[idx];
        }

        ~FrameData(){}

        iterator begin()
        {
            return m_data.begin();
        }

        iterator end()
        {
            return m_data.end();
        }

        const_iterator begin() const
        {
            return m_data.begin();
        }

        const_iterator end() const
        {
            return m_data.end();
        }

        const_iterator cbegin() const
        {
            return m_data.cbegin();
        }

        const_iterator cend() const
        {
            return m_data.cend();
        }

        reverse_iterator rbegin()
        {
            return m_data.rbegin();
        }

        reverse_iterator rend()
        {
            return m_data.rend();
        }

        const_reverse_iterator rbegin() const
        {
            return m_data.rbegin();
        }

        const_reverse_iterator rend() const
        {
            return m_data.rend();
        }

        const_reverse_iterator crbegin() const
        {
            return m_data.crbegin();
        }

        const_reverse_iterator crend() const
        {
            return m_data.crend();
        }

        size_type size() const
        {
            return m_data.size();
        }

    private:
        frame_container m_data;
    };

    /**
     * @brief AnimatedFrameData Returns specific frames for supplied state.
     *
     * @tparam FrameDataType Frame container type.
     * @tparam FrameStateType Frame type, must define fields startingFrame, endingFrame, which are convertible to BasicFrameIndex<T> and factor between them.
     */
    template <class FrameDataType, class FrameStateType = aex::FrameState>
    class AnimatedFrameData
    {
        public:
            using frame_data_type = FrameDataType;
            using frame_type = typename frame_data_type::frame_type;
            using size_type = typename frame_data_type::size_type;
            using state_type = FrameStateType;
            using factor_type = typename state_type::factor_type;

            /**
             * @brief Create new animated frame data.
             *
             * @param framesCount Number of frames in this data, default 24.
             */
            explicit AnimatedFrameData(const size_type framesCount = 24u) : m_data(framesCount) {}

            /**
             * @brief Create new animated frame data by coping frames from rhs.
             *
             * @param rhs frame container to be copied.
             */
            explicit AnimatedFrameData(const frame_data_type& rhs) : m_data(rhs), m_state{}{}

            /**
             * @brief  Create new animated frame data by moving frames from rhs.
             *
             * @param rhs 
             */
            explicit AnimatedFrameData(frame_data_type&& rhs) : m_data(std::move(rhs)), m_state{}{}
            
            /**
             * @brief Assign data for animated frame data by coping data from rhs.
             *
             * @param rhs Frame data to be copied.
             *
             * @return *this
             */
            AnimatedFrameData& operator=(const frame_data_type& rhs)
            {
                if(&m_data != &rhs)
                {
                    m_data = rhs;
                }
                return *this;
            }

            /**
             * @brief Assign data for animated frame data by moving data from rhs.
             *
             * @param rhs
             *
             * @return 
             */
            AnimatedFrameData& operator=(frame_data_type&& rhs)
            {
                if(&m_data != &rhs)
                {
                    m_data = std::move(rhs);
                }
                return *this;
            }

            ~AnimatedFrameData (){}

            /**
             * @brief Set animated data state.
             *
             * @param state New state for data.
             */
            void setFrameState(const state_type& state)
            {
                m_state = state;
            }

            /**
             * @brief Return reference to first frame defined by current state.
             *
             * @return Frame reference.
             */
            const frame_type& getFirstFrame() const
            {
                return m_data[m_state.startingFrame];
            }

            /**
             * @brief Get reference to next frame defined by current state.
             *
             * @return Frame reference.
             */
            const frame_type& getNextFrame() const
            {
                return m_data[m_state.endingFrame];
            }

            /**
             * @brief Return reference to factor between first and next frame for current state.
             *
             * @return Factor reference.
             */
            const factor_type& getFactor() const
            {
                return m_state.factor;
            }
        protected:
            frame_data_type m_data;
            state_type m_state;
        public:
            auto animationSize() const -> decltype(this->m_data.size())
            {
                return m_data.size();
            }
    };

    /**
     * @brief Wrap animated data type for drawing.
     *
     * @tparam AnimatedDataType RType that implements getFirstFrame(), getSecondFrame(), getFactor()
     * @tparam FrameDrawType 
     */
    template<class AnimatedDataType, class FrameDrawType>
    class FrameDrawWrapper
    {
        public:
            using animated_frame_data = AnimatedDataType;
            using frame_type = typename animated_frame_data::frame_type;
            using draw_type = FrameDrawType;
            /**
             * @brief Constructors
             */
            FrameDrawWrapper() : m_data{}{}
            FrameDrawWrapper(const FrameDrawWrapper& rhs) : FrameDrawWrapper()
            {
                *this = rhs;
            }
            
            FrameDrawWrapper& operator=(const FrameDrawWrapper& rhs)
            {
                if(this != &rhs)
                {
                    m_drawObject = rhs.m_drawObject;
                    m_data = rhs.m_data;
                }
                return *this;
            }
            
            FrameDrawWrapper(FrameDrawWrapper&& rhs) : FrameDrawWrapper()
            {
                *this = std::move(rhs);
            }
            
            FrameDrawWrapper& operator=(FrameDrawWrapper&& rhs)
            {
                if(this != &rhs)
                {
                    m_drawObject = std::move(rhs.m_drawObject);
                    m_data = std::move(m_data);
                }
                return *this;
            }
            
            /**
             * @brief Create new wrapper with data.
             *
             * @param data Animation data to wrap.
             */
            explicit FrameDrawWrapper(const animated_frame_data& data)
            {
                *this = data;
            }
            
            FrameDrawWrapper& operator=(const animated_frame_data& data)
            {
                if(&m_data != &data)
                    m_data = data;
                return *this;
            }
            
            explicit FrameDrawWrapper(animated_frame_data&& data)
            {
                *this = std::move(data);
            }
            
            FrameDrawWrapper& operator=(animated_frame_data&& data)
            {
                if(&m_data != &data)
                    m_data = std::move(data);
                return *this;
            }

            /**
             * @brief Return object which can be hooked into drawing tree.
             *
             * @return Reference to an object which can be added to drawing tree.
             */
            draw_type& getDrawable()
            {
                return m_drawObject;
            }

            const draw_type& getDrawable() const
            {
                return m_drawObject;
            }


            /**
             * @brief Dispatch draw call to definition of animation drawing.
             *
             * @param gl Renderer pointer.
             */
            void draw(AexRenderer* gl)
            {
                m_drawObject.draw(gl, m_data.getFirstFrame(), m_data.getNextFrame(), m_data.getFactor());
            }

            /**
             * @brief Dispatch finishdraw call to definition of animation drawing.
             *
             * @param gl Renderer pointer.
             */
            void finishdraw(AexRenderer* gl)
            {
                m_drawObject.finishdraw(gl, m_data.getFirstFrame(), m_data.getNextFrame(), m_data.getFactor());
            }

            void needsUpdate()
            {
                m_drawObject.needsUpdate();
            }

            animated_frame_data& getAnimationData()
            {
                return m_data;
            }

            const animated_frame_data& getAnimationData() const
            {
                return m_data;
            }

        private:
            animated_frame_data m_data;
            draw_type m_drawObject;
    };
} /* namespace aex */

#endif /* _AEX_FRAMEDATA_HPP */

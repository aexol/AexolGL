/**************************************************************
 *                                                            * 
 *      Filename:   BasicAnimation.hpp                        * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 wrz 17 16:03:15                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/

#ifndef BASICANIMATION_HPP
#define BASICANIMATION_HPP 

#include <aex/Common.hpp>
#include <aex/Traits.hpp>
#include <list>
#include <type_traits>
#include <iterator>

namespace aex {
    namespace _basicanimation_detail {
        template <class T>
        struct has_distance_type
        {
                template <class C>
                static auto test(void*) -> decltype(std::declval<typename C::frame_distance_type>(), std::true_type());
                template<class C>
                static std::false_type test(...);
                static CONSTEXPR bool value = decltype(test<T>(0))::value;
        };
        template<class T, bool HasDifferenceType = has_distance_type<T>::value>
        struct get_distance_type
        {
            using frame_distance_type = T;
        };

        template <class T>
        struct get_distance_type<T, true>
        {
            using frame_distance_type = typename T::frame_distance_type;
        };
    } /* namespace _basicanimation_detail */

    template<class T> class BasicFrameIndex;
    template<class T> BasicFrameIndex<T>& operator++(BasicFrameIndex<T>& id);
    template<class T> BasicFrameIndex<T> operator++(BasicFrameIndex<T>& id, int);
    template<class T> BasicFrameIndex<T>& operator--(BasicFrameIndex<T>& id);
    template<class T> BasicFrameIndex<T> operator--(BasicFrameIndex<T>& id, int);
    template<class T> BasicFrameIndex<T> operator+(const BasicFrameIndex<T>& lhs,
            const typename BasicFrameIndex<T>::frame_distance_type& rhs);
    template<class T> BasicFrameIndex<T> operator-(const BasicFrameIndex<T>& lhs,
            const typename BasicFrameIndex<T>::frame_distance_type& rhs);
    template<class T> typename BasicFrameIndex<T>::frame_distance_type operator-(const BasicFrameIndex<T>& lhs,
            const BasicFrameIndex<T>& rhs);
    template<class T> bool operator<(const BasicFrameIndex<T>& lhs, const BasicFrameIndex<T>& rhs);
    template<class T> bool operator>(const BasicFrameIndex<T>& lhs, const BasicFrameIndex<T>& rhs);
    template<class T> bool operator==(const BasicFrameIndex<T>& lhs, const BasicFrameIndex<T>& rhs);
    template<class T> bool operator!=(const BasicFrameIndex<T>& lhs, const BasicFrameIndex<T>& rhs);

    template<class T>
    class BasicFrameIndex
    {
    public:
        using value_type = T;
        using frame_distance_type = value_type;
        BasicFrameIndex();
        explicit BasicFrameIndex(const T& index);
        BasicFrameIndex& operator+=(const frame_distance_type& rhs);
        BasicFrameIndex& operator-=(const frame_distance_type& rhs);
        operator T() const
        {
            return m_index;
        }
        ~BasicFrameIndex();
    private:
        value_type m_index;
        friend BasicFrameIndex& operator++ <> (BasicFrameIndex& id);
        friend BasicFrameIndex operator++ <> (BasicFrameIndex& id, int);
        friend BasicFrameIndex& operator-- <> (BasicFrameIndex& id);
        friend BasicFrameIndex operator-- <> (BasicFrameIndex& id, int);
        friend BasicFrameIndex operator+ <> (const BasicFrameIndex& lhs, const frame_distance_type& rhs);
        friend BasicFrameIndex operator- <> (const BasicFrameIndex& lhs, const frame_distance_type& rhs);
        friend frame_distance_type operator- <> (const BasicFrameIndex& lhs, const BasicFrameIndex& rhs);
        friend bool operator< <> (const BasicFrameIndex& lhs, const BasicFrameIndex& rhs);
        friend bool operator> <> (const BasicFrameIndex& lhs, const BasicFrameIndex& rhs);
        friend bool operator== <> (const BasicFrameIndex& lhs, const BasicFrameIndex& rhs);
        friend bool operator!= <> (const BasicFrameIndex& lhs, const BasicFrameIndex& rhs);
    };
    using FrameIndex = BasicFrameIndex<uint8_t>;

    template<class FrameIndexType>
    struct DistanceEvaluatorBase
    {
            /**
             * @brief Type indexing frame.
             */
            using frame_index_type = FrameIndexType;
            /**
             * @brief Type describing distance between two frames.
             */
            using frame_distance_type =
                typename _basicanimation_detail::get_distance_type<frame_index_type>::frame_distance_type;
        static frame_index_type rangeBegin(const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);

        static frame_index_type rangeEnd(const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);
    };

    template<class FrameIndexType>
    struct ReverseDistanceEvaluatorBase
    {
            /**
             * @brief Type indexing frame.
             */
            using frame_index_type = FrameIndexType;
            /**
             * @brief Type describing distance between two frames.
             */
            using frame_distance_type =
                typename _basicanimation_detail::get_distance_type<frame_index_type>::frame_distance_type;

            static frame_index_type rangeBegin(const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);

            static frame_index_type rangeEnd(const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);
    };

    /**
     * @brief Returns distance between two frames in range or next frame, looping them if necessary.
     *
     * @tparam FrameIndexType Type of frame index.
     */
    template <class FrameIndexType>
    struct ReplayDistanceEvaluator : public DistanceEvaluatorBase<FrameIndexType>
    {
            /**
             * @brief Type indexing frame.
             */
            using frame_index_type = FrameIndexType;
            /**
             * @brief Type describing distance between two frames.
             */
            using frame_distance_type =
                typename _basicanimation_detail::get_distance_type<frame_index_type>::frame_distance_type;
            /**
             * @brief Return next frame after _p in range [_rangeBegin, _rangeBegin+_rangeSize).
             *
             * @param _p Previous frame.
             * @param _rangeBegin First frame in range.
             * @param _rangeSize Range size.
             *
             * @return Next frame.
             */
            static frame_index_type inc(const frame_index_type& _p, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);
            /**
             * @brief Return prev frame before _p in range [_rangeBegin, _rangeBegin+_rangeSize).
             *
             * @param _p current frame.
             * @param _rangeBegin First frame in range.
             * @param _rangeSize Range size.
             *
             * @return Previous frame.
             */
            static frame_index_type dec(const frame_index_type& _p, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);

            /**
             * @brief Forward animation by _dist, looping if necessary.
             *
             * @param _p Current frame.
             * @param _dist Forward value.
             * @param _rangeBegin Begining of range.
             * @param _rangeSize Range size.
             *
             * @return 
             */
            static frame_index_type forward(const frame_index_type& _p, const frame_distance_type& _dist, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);

            /**
             * @brief Rewind animation by _dist, looping if necessary.
             *
             * @param _p Current frame.
             * @param _dist Rewind value.
             * @param _rangeBegin Begining of range.
             * @param _rangeSize Range size.
             *
             * @return 
             */
            static frame_index_type rewind(const frame_index_type& _p, const frame_distance_type& _dist, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);
            /**
             * @brief Return distance between _first and _last in [_rangeBegin, _rangeBegin+_rangeSize) range.
             *
             * @param _first 
             * @param _last 
             * @param _rangeBegin  First frame in range.
             * @param _rangeSize Range size.
             *
             * @return Distance between _first and _last.
             */
            static frame_distance_type distance(const frame_index_type& _first, const frame_index_type& _last, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);
    };

    /**
     * @brief Returns distance between two frames in range or next frame.
     *
     * @tparam FrameIndexType Type of frame index.
     */
    template <class FrameIndexType>
    struct NoReplayDistanceEvaluator : public DistanceEvaluatorBase<FrameIndexType>
    {
            /**
             * @brief Type indexing frame.
             */
            using frame_index_type = FrameIndexType;
            /**
             * @brief Type describing distance between two frames.
             */
            using frame_distance_type =
                typename _basicanimation_detail::get_distance_type<frame_index_type>::frame_distance_type;
            /**
             * @brief Return next frame after _p in range [_rangeBegin, _rangeBegin+_rangeSize).
             *
             * @param _p Previous frame.
             * @param _rangeBegin First frame in range.
             * @param _rangeSize Range size.
             *
             * @return Next frame.
             */
            static frame_index_type inc(const frame_index_type& _p, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);
            /**
             * @brief Return prev frame before _p in range [_rangeBegin, _rangeBegin+_rangeSize).
             *
             * @param _p current frame.
             * @param _rangeBegin First frame in range.
             * @param _rangeSize Range size.
             *
             * @return Previous frame.
             */
            static frame_index_type dec(const frame_index_type& _p, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);

            /**
             * @brief Forward animation by _dist, looping if necessary.
             *
             * @param _p Current frame.
             * @param _dist Forward value.
             * @param _rangeBegin Begining of range.
             * @param _rangeSize Range size.
             *
             * @return 
             */
            static frame_index_type forward(const frame_index_type& _p, const frame_distance_type& _dist, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);

            /**
             * @brief Rewind animation by _dist, looping if necessary.
             *
             * @param _p Current frame.
             * @param _dist Rewind value.
             * @param _rangeBegin Begining of range.
             * @param _rangeSize Range size.
             *
             * @return 
             */
            static frame_index_type rewind(const frame_index_type& _p, const frame_distance_type& _dist, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);
            /**
             * @brief Return distance between _first and _last in [_rangeBegin, _rangeBegin+_rangeSize) range.
             *
             * @param _first 
             * @param _last 
             * @param _rangeBegin  First frame in range.
             * @param _rangeSize Range size.
             *
             * @return Distance between _first and _last.
             */
            static frame_distance_type distance(const frame_index_type& _first, const frame_index_type& _last, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);
    };

    /**
     * @brief Returns distance between two frames in range or previous frame, looping them if necessary in reverse.
     *
     * @tparam FrameIndexType Type of frame index.
     */
    template <class FrameIndexType>
    struct ReverseReplayDistanceEvaluator : public ReverseDistanceEvaluatorBase<FrameIndexType>
    {
            /**
             * @brief Type indexing frame.
             */
            using frame_index_type = FrameIndexType;
            /**
             * @brief Type describing distance between two frames.
             */
            using frame_distance_type =
                typename _basicanimation_detail::get_distance_type<frame_index_type>::frame_distance_type;
            /**
             * @brief Return next frame after _p in range [_rangeBegin, _rangeBegin+_rangeSize).
             *
             * @param _p Previous frame.
             * @param _rangeBegin First frame in range.
             * @param _rangeSize Range size.
             *
             * @return Next frame.
             */
            static frame_index_type inc(const frame_index_type& _p, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);
            /**
             * @brief Return prev frame before _p in range [_rangeBegin, _rangeBegin+_rangeSize).
             *
             * @param _p current frame.
             * @param _rangeBegin First frame in range.
             * @param _rangeSize Range size.
             *
             * @return Previous frame.
             */
            static frame_index_type dec(const frame_index_type& _p, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);

            /**
             * @brief Forward animation by _dist, looping if necessary.
             *
             * @param _p Current frame.
             * @param _dist Forward value.
             * @param _rangeBegin Begining of range.
             * @param _rangeSize Range size.
             *
             * @return 
             */
            static frame_index_type forward(const frame_index_type& _p, const frame_distance_type& _dist, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);

            /**
             * @brief Rewind animation by _dist, looping if necessary.
             *
             * @param _p Current frame.
             * @param _dist Rewind value.
             * @param _rangeBegin Begining of range.
             * @param _rangeSize Range size.
             *
             * @return 
             */
            static frame_index_type rewind(const frame_index_type& _p, const frame_distance_type& _dist, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);
            /**
             * @brief Return distance between _first and _last in [_rangeBegin, _rangeBegin+_rangeSize) range.
             *
             * @param _first 
             * @param _last 
             * @param _rangeBegin  First frame in range.
             * @param _rangeSize Range size.
             *
             * @return Distance between _first and _last.
             */
            static frame_distance_type distance(const frame_index_type& _first, const frame_index_type& _last, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);
    };

    /**
     * @brief Returns distance between two frames in range or previous frame.
     *
     * @tparam FrameIndexType Type of frame index.
     */
    template <class FrameIndexType>
    struct ReverseNoReplayDistanceEvaluator : public ReverseDistanceEvaluatorBase<FrameIndexType>
    {
            /**
             * @brief Type indexing frame.
             */
            using frame_index_type = FrameIndexType;
            /**
             * @brief Type describing distance between two frames.
             */
            using frame_distance_type =
                typename _basicanimation_detail::get_distance_type<frame_index_type>::frame_distance_type;
            /**
             * @brief Return next frame after _p in range [_rangeBegin, _rangeBegin+_rangeSize).
             *
             * @param _p Previous frame.
             * @param _rangeBegin First frame in range.
             * @param _rangeSize Range size.
             *
             * @return Next frame.
             */
            static frame_index_type inc(const frame_index_type& _p, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);
            /**
             * @brief Return prev frame before _p in range [_rangeBegin, _rangeBegin+_rangeSize).
             *
             * @param _p current frame.
             * @param _rangeBegin First frame in range.
             * @param _rangeSize Range size.
             *
             * @return Previous frame.
             */
            static frame_index_type dec(const frame_index_type& _p, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);

            /**
             * @brief Forward animation by _dist, looping if necessary.
             *
             * @param _p Current frame.
             * @param _dist Forward value.
             * @param _rangeBegin Begining of range.
             * @param _rangeSize Range size.
             *
             * @return 
             */
            static frame_index_type forward(const frame_index_type& _p, const frame_distance_type& _dist, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);

            /**
             * @brief Rewind animation by _dist, looping if necessary.
             *
             * @param _p Current frame.
             * @param _dist Rewind value.
             * @param _rangeBegin Begining of range.
             * @param _rangeSize Range size.
             *
             * @return 
             */
            static frame_index_type rewind(const frame_index_type& _p, const frame_distance_type& _dist, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);
            /**
             * @brief Return distance between _first and _last in [_rangeBegin, _rangeBegin+_rangeSize) range.
             *
             * @param _first 
             * @param _last 
             * @param _rangeBegin  First frame in range.
             * @param _rangeSize Range size.
             *
             * @return Distance between _first and _last.
             */
            static frame_distance_type distance(const frame_index_type& _first, const frame_index_type& _last, const frame_index_type& _rangeBegin, const frame_distance_type& _rangeSize);
    };

    template<class RangeType, class DistanceEvaluator> class BasicFrameRangeIterator;

    template <class RangeType, class DistanceEvaluator> 
    BasicFrameRangeIterator<RangeType, DistanceEvaluator>& operator++(BasicFrameRangeIterator<RangeType, DistanceEvaluator>& it);
    template <class RangeType, class DistanceEvaluator> 
    BasicFrameRangeIterator<RangeType, DistanceEvaluator> operator++(BasicFrameRangeIterator<RangeType, DistanceEvaluator>& it, int);

    template <class RangeType, class DistanceEvaluator>
    BasicFrameRangeIterator<RangeType, DistanceEvaluator>& operator--(BasicFrameRangeIterator<RangeType, DistanceEvaluator>& it);
    template <class RangeType, class DistanceEvaluator>
    BasicFrameRangeIterator<RangeType, DistanceEvaluator> operator--(BasicFrameRangeIterator<RangeType, DistanceEvaluator>& it, int);

    template <class RangeType, class DistanceEvaluator>
    BasicFrameRangeIterator<RangeType, DistanceEvaluator> operator+(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& lhs, const typename BasicFrameRangeIterator<RangeType, DistanceEvaluator>::difference_type& rhs);

    template <class RangeType, class DistanceEvaluator>
    BasicFrameRangeIterator<RangeType, DistanceEvaluator> operator-(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& lhs, const typename BasicFrameRangeIterator<RangeType, DistanceEvaluator>::difference_type& rhs);
    template <class RangeType, class DistanceEvaluator>
    typename BasicFrameRangeIterator<RangeType, DistanceEvaluator>::difference_type operator-(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& lhs, const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& rhs);

    template<class RangeType, class DistanceEvaluator>
    typename BasicFrameRangeIterator<RangeType, DistanceEvaluator>::reference operator*(BasicFrameRangeIterator<RangeType, DistanceEvaluator>& it);
    template<class RangeType, class DistanceEvaluator>
    typename BasicFrameRangeIterator<RangeType, DistanceEvaluator>::const_reference operator*(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& it);

    template<class RangeType, class DistanceEvaluator>
    bool operator==(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& lhs, const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& rhs);
    template<class RangeType, class DistanceEvaluator>
    bool operator!=(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& lhs, const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& rhs);
    template<class RangeType, class DistanceEvaluator>
    bool operator<(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& lhs, const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& rhs);
    template<class RangeType, class DistanceEvaluator>
    bool operator>(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& lhs, const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& rhs);
    template<class RangeType, class DistanceEvaluator>
    bool operator<=(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& lhs, const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& rhs);
    template<class RangeType, class DistanceEvaluator>
    bool operator>=(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& lhs, const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& rhs);

    template<class RangeType, class DistanceEvaluator>
    class BasicFrameRangeIterator
    {
        public:
            using range_type = RangeType;
            using value_type = typename range_type::frame_index_type;
            using difference_type = typename range_type::frame_distance_type;
            using reference = value_type&;
            using const_reference = const value_type&;
            using pointer = value_type*;
            using evaluator = DistanceEvaluator;
            using iterator_category = std::random_access_iterator_tag;
            BasicFrameRangeIterator();
            BasicFrameRangeIterator(const value_type& idx, const range_type* p);
            ~BasicFrameRangeIterator ();

            pointer operator->();
            const pointer operator->() const;
            BasicFrameRangeIterator& operator+=(const difference_type& rhs);
            BasicFrameRangeIterator& operator-=(const difference_type& rhs);
            value_type operator[](const difference_type& _dist) const;
        private:
            friend BasicFrameRangeIterator& operator++ <>(BasicFrameRangeIterator&);
            friend BasicFrameRangeIterator operator++ <>(BasicFrameRangeIterator&, int);
            friend BasicFrameRangeIterator& operator-- <>(BasicFrameRangeIterator&);
            friend BasicFrameRangeIterator operator-- <>(BasicFrameRangeIterator&, int);
            friend BasicFrameRangeIterator operator+ <>(const BasicFrameRangeIterator& lhs, const difference_type& rhs);
            friend difference_type operator- <>(const BasicFrameRangeIterator& lhs, const BasicFrameRangeIterator& rhs);
            friend BasicFrameRangeIterator operator- <>(const BasicFrameRangeIterator& lhs, const difference_type& rhs);
            friend reference operator* <>(BasicFrameRangeIterator& it);
            friend const_reference operator* <>(const BasicFrameRangeIterator& it);
            friend bool operator== <>(const BasicFrameRangeIterator& lhs, const BasicFrameRangeIterator& rhs);
            friend bool operator!= <>(const BasicFrameRangeIterator& lhs, const BasicFrameRangeIterator& rhs);
            friend bool operator< <>(const BasicFrameRangeIterator& lhs, const BasicFrameRangeIterator& rhs);
            friend bool operator> <>(const BasicFrameRangeIterator& lhs, const BasicFrameRangeIterator& rhs);
            friend bool operator<= <>(const BasicFrameRangeIterator& lhs, const BasicFrameRangeIterator& rhs);
            friend bool operator>= <>(const BasicFrameRangeIterator& lhs, const BasicFrameRangeIterator& rhs);
            value_type m_index;
            const range_type* m_p;
            auto firstFrame() const -> const decltype(this->m_p->m_firstFrame)& {return m_p->m_firstFrame;}
            auto size() const -> const decltype(this->m_p->m_size)& {return m_p->m_size;}
    };

    /**
     * @brief Frame range [first, last).
     *
     * @tparam FrameIndexType Frame index type.
     */
    template <class FrameIndexType>
    class BasicFrameRange
    {
        public:
            /**
             * @brief Type indexing frame.
             */
            using frame_index_type = FrameIndexType;

            /**
             * @brief Type describing distance between two frames.
             */
            using frame_distance_type =
                typename _basicanimation_detail::get_distance_type<frame_index_type>::frame_distance_type;

            /**
             * @brief Creates default range of 24 frames.
             */
            BasicFrameRange();
            /**
             * @brief Create range of 24 frames with indexing starting at first frame.
             *
             * @param firstFrame Frist frame index.
             */
            explicit BasicFrameRange(const frame_index_type& firstFrame);

            /**
             * @brief Create range of frames with _size and indexing starting at firstFrame.
             *
             * @param firstFrame First frame index.
             * @param _size Size of frames, creates a range [firstFrame, firstFrame+_size)
             */
            explicit BasicFrameRange(const frame_index_type& firstFrame, const frame_distance_type& _size);

            /**
             * @brief Frame indexing beginning.
             *
             * @return First iterator.
             */
            template<class DistanceEvaluator>
            const BasicFrameRangeIterator<BasicFrameRange, DistanceEvaluator> ctbegin() const;
            template<class DistanceEvaluator>
            const BasicFrameRangeIterator<BasicFrameRange, DistanceEvaluator> tbegin() const;
            template<class DistanceEvaluator>
            BasicFrameRangeIterator<BasicFrameRange, DistanceEvaluator> tbegin();

            /**
             * @brief Past last element in range.
             *
             * @return Element past last.
             */
            template<class DistanceEvaluator>
            const BasicFrameRangeIterator<BasicFrameRange, DistanceEvaluator> ctend() const;
            template<class DistanceEvaluator>
            const BasicFrameRangeIterator<BasicFrameRange, DistanceEvaluator> tend() const;
            template<class DistanceEvaluator>
            BasicFrameRangeIterator<BasicFrameRange, DistanceEvaluator> tend();

            ~BasicFrameRange();
        private:
            template<class T, class U> friend class BasicFrameRangeIterator;
            frame_index_type m_firstFrame;
            frame_distance_type m_size;
    };

    using FrameRange = BasicFrameRange<FrameIndex>;

    namespace _basicanimation_detail {
        template <class T>
        struct has_factor_type
        {
                template <class C>
                static auto test(void*) -> decltype(std::declval<typename C::simulation_factor_type>(), std::true_type());
                template<class C>
                static std::false_type test(...);
                static CONSTEXPR bool value = decltype(test<T>(0))::value;
        };
        template<class T, bool HasFactorType = has_factor_type<T>::value>
        struct get_factor_type
        {
            using type = typename T::simulation_data_type;
        };

        template <class T>
        struct get_factor_type<T, true>
        {
            using type = typename T::simulation_factor_type;
        };
    } /* namespace _basicanimation_detail */
    template<class FrameIndexType, class SimulationDataTraitsType>
    struct BasicFrameStateTypeInfo {

        /**
         * @brief Type describing factor between two frames.
         */
        using simulation_data_traits_type = SimulationDataTraitsType;
        using factor_type = typename _basicanimation_detail::get_factor_type<simulation_data_traits_type>::type;
        /**
         * @brief Frame identifier in data.
         */
        using frame_index_type = FrameIndexType;
        /**
         * @brief Type describing distance between frames.
         */
        using frame_distance_type = 
                typename _basicanimation_detail::get_distance_type<frame_index_type>::frame_distance_type;
    } /* optional variable list */;
    using FrameStateTypeInfo = BasicFrameStateTypeInfo<FrameIndex, SimulationDataTraits<aexTimeUnit>>;

    /**
     * @brief Structure holding information about frame state.
     *
     * Holds index of current frame, next frame following current and factor for interpolating between two frames.
     *
     * @tparam FrameStateTypeInfo
     */
    template <class FrameStateTypeInfo>
    struct BasicFrameState {
        /**
         * @brief Struct defining types for factor and data indexing.
         */
        using frame_state_info_type = FrameStateTypeInfo;
        /**
         * @brief type defining fraction between two frames.
         */
        using factor_type = typename frame_state_info_type::factor_type;
        /**
         * @brief Type indexing frame.
         */
        using frame_index_type = typename frame_state_info_type::frame_index_type;
        /**
         * @brief Type indexing frame.
         */
        using frame_distance_type = typename frame_state_info_type::frame_distance_type;
        BasicFrameState() : BasicFrameState(factor_type{}, frame_index_type{}){}
        BasicFrameState(factor_type ff, frame_index_type frameStart) : BasicFrameState(ff, frameStart, frameStart+frame_distance_type(1)){}
        BasicFrameState(factor_type ff, frame_index_type frameStart, frame_index_type frameEnd) :
            factor(ff),
            startingFrame(frameStart),
            endingFrame(frameEnd)
        {}
        /**
         * @brief factor defines fraction value betwen frame for interpolation purposes.
         */
        factor_type factor;
        /**
         * @brief Previous frame in animation.
         */
        frame_index_type startingFrame;
        /**
         * @brief Next frame in animation.
         */
        frame_index_type endingFrame;
    } /* optional variable list */;
    using FrameState =  BasicFrameState<FrameStateTypeInfo>;

    namespace _basicanimation_detail
    {
        template<class T, class U>
        T simDataToDistance(U&& u)
        {
            return static_cast<T>(aex::forward<U>(u));
        }
    }

    template <class SimulationDataTraitsType, class FrameRangeType>
    struct BasicSimFrameCalc
    {
        using simulation_data_traits_type = SimulationDataTraitsType;
        using simulation_data_type = typename simulation_data_traits_type::simulation_data_type;
        using frame_range_type = FrameRangeType;
        using frame_distance_type = typename frame_range_type::frame_distance_type;

        static simulation_data_type div(const simulation_data_type& simData, const frame_distance_type& frameDistance)
        {
            return simulation_data_traits_type::div(simData, frameDistance);
        }

        static simulation_data_type mul(const simulation_data_type& simData, const frame_distance_type& frameDistance)
        {
            return simulation_data_traits_type::mul(simData, frameDistance);
        }
    };

    template <class SimulationDataTraitsType, class T>
    struct BasicSimFrameCalc<SimulationDataTraitsType, BasicFrameRange<BasicFrameIndex<T>>>
    {
        using simulation_data_traits_type = SimulationDataTraitsType;
        using simulation_data_type = typename simulation_data_traits_type::simulation_data_type;
        using frame_range_type = BasicFrameRange<BasicFrameIndex<T>>;
        using frame_distance_type = typename frame_range_type::frame_distance_type;

        static simulation_data_type div(const simulation_data_type& simData, const frame_distance_type& frameDistance)
        {
            return simulation_data_traits_type::div(simData, static_cast<std::size_t>(frameDistance));
        }

        static simulation_data_type mul(const simulation_data_type& simData, const frame_distance_type& frameDistance)
        {
            return simulation_data_traits_type::mul(simData, static_cast<std::size_t>(frameDistance));
        }
    };

    /**
     * @brief Frame layout for animation with static spacing between frames.
     *
     * For example for animation with 24 frames it's goning to be 24/duration.
     *
     * @tparam SimulationDataTraitsType
     * @tparam FrameStateType
     */
    template <
        class SimulationDataTraitsType,
        class FrameRangeType = FrameRange,
        class FrameRangeDefaultIter =
            BasicFrameRangeIterator<FrameRangeType,
            NoReplayDistanceEvaluator<typename FrameRangeType::frame_index_type>
        >,
        class SimFrameCalc = BasicSimFrameCalc<SimulationDataTraitsType, FrameRangeType>
    >
    class StaticFrameLayout
    {
        public:
            /**
             * @brief Traits type for simulation data.
             */
            using simulation_data_traits_type = SimulationDataTraitsType;
            /**
             * @brief Simulation data type. For example time unit.
             */
            using simulation_data_type = typename simulation_data_traits_type::simulation_data_type;
            /**
             * @brief Frame state type descriptor.
             */
            using frame_range_type = FrameRangeType;
            /**
             * @brief Frame indexing type.
             */
            using frame_index_type = typename frame_range_type::frame_index_type;
            using const_iterator = const FrameRangeDefaultIter;
            /**
             * @brief Frame indexing type.
             */
            using frame_distance_type = typename frame_range_type::frame_distance_type;
            using sim_calc_type = SimFrameCalc;
            /**
             * @brief Default constructor.
             *
             * Creates default layout, which is 24 frames per simulation_data_type unit animation.
             */
            StaticFrameLayout();
            /**
             * @brief Create layout with user supplied frame count and duration of simulation_data_type unit.
             *
             * @param totalFrames Total frames in animation.
             */
            explicit StaticFrameLayout(const frame_distance_type& totalFrames);
            /**
             * @brief Create layout with user supplied frame count duration.
             *
             * @param totalFrames Total frames in animation.
             * @param duration Total animation duration.
             */
            explicit StaticFrameLayout(const frame_distance_type& totalFrames, const simulation_data_type& duration);

            /**
             * @brief Create layout with user supplied frame count duration.
             *
             * @param firstFrame Start indexing from firstFrame.
             * @param totalFrames Total frames in animation.
             * @param duration Total animation duration.
             */
            explicit StaticFrameLayout(const frame_index_type& firstFrame, const frame_distance_type& totalFrames, const simulation_data_type& duration);
            /**
             * @brief Return starting offset of a frame from the begining of an animation.
             *
             * Starting offset from the beginning of an animation. Starting offset is inclusive.
             *
             * @param frame Frame index in animation.
             *
             * @return Starting offset.
             */
            template<class _Iter>
            simulation_data_type getFrameStart(const _Iter& frame) const;
            /**
             * @brief Return ending offset from the beginning of an animation.
             *
             * Ending offset from the beginning of an animation. Ending offset is exclusive.
             *
             * @param frame Frame index in an animation.
             *
             * @return Ending offset.
             */
            template<class _Iter>
            simulation_data_type getFrameEnd(const _Iter& frame) const;

            /**
             * @brief Return frame at given simulation data.
             *
             * @param simData Simulation data.
             *
             * @return 
             */
            const_iterator begin() const;
            const_iterator cbegin() const;
            const_iterator end() const;
            const_iterator cend() const;

            std::size_t size() const;
            ~StaticFrameLayout();
        private:
            frame_range_type m_frameLayout;
            simulation_data_type m_frameDuration;
            frame_distance_type getOffset(const simulation_data_type& simData) const
            {
                const auto distance = size();
                const simulation_data_type r = sim_calc_type::mul(m_frameDuration, distance);
                const simulation_data_type _s = simulation_data_traits_type::div(
                        simulation_data_traits_type::mod(simData, r),
                        r
                    );
                const frame_distance_type offset = 
                    _basicanimation_detail::simDataToDistance<frame_distance_type>(sim_calc_type::mul(_s, distance));
                return offset;
            }
        public:
            auto rbegin() const 
                -> decltype(m_frameLayout.template tbegin<ReverseNoReplayDistanceEvaluator<frame_index_type>>())
            {
                return m_frameLayout.template tbegin<ReverseNoReplayDistanceEvaluator<frame_index_type>>();
            }
            auto crbegin() const
                -> decltype(m_frameLayout.template ctbegin<ReverseNoReplayDistanceEvaluator<frame_index_type>>())
            {
                return m_frameLayout.template ctbegin<ReverseNoReplayDistanceEvaluator<frame_index_type>>();
            }
            auto rend() const
                -> decltype(m_frameLayout.template tend<ReverseNoReplayDistanceEvaluator<frame_index_type>>())
            {
                return m_frameLayout.template tend<ReverseNoReplayDistanceEvaluator<frame_index_type>>();
            }
            auto crend() const
                -> decltype(m_frameLayout.template ctend<ReverseNoReplayDistanceEvaluator<frame_index_type>>())
            {
                return m_frameLayout.template ctend<ReverseNoReplayDistanceEvaluator<frame_index_type>>();
            }
            
            auto lbegin() const
                -> decltype(m_frameLayout.template tbegin<ReplayDistanceEvaluator<frame_index_type>>())
            {
                return m_frameLayout.template ctbegin<ReplayDistanceEvaluator<frame_index_type>>();
            }
            auto clbegin() const
                -> decltype(m_frameLayout.template ctbegin<ReplayDistanceEvaluator<frame_index_type>>())
            {
                return m_frameLayout.template ctbegin<ReplayDistanceEvaluator<frame_index_type>>();
            }
            auto lend() const
                -> decltype(m_frameLayout.template tend<ReplayDistanceEvaluator<frame_index_type>>())
            {
                return m_frameLayout.template tend<ReplayDistanceEvaluator<frame_index_type>>();
            }
            auto clend() const
                -> decltype(m_frameLayout.template ctend<ReplayDistanceEvaluator<frame_index_type>>())
            {
                return m_frameLayout.template ctend<ReplayDistanceEvaluator<frame_index_type>>();
            }
            
            auto lrbegin() const
                -> decltype(m_frameLayout.template tbegin<ReverseReplayDistanceEvaluator<frame_index_type>>())
            {
                return m_frameLayout.template ctbegin<ReverseReplayDistanceEvaluator<frame_index_type>>();
            }
            auto clrbegin() const
                -> decltype(m_frameLayout.template ctbegin<ReverseReplayDistanceEvaluator<frame_index_type>>())
            {
                return m_frameLayout.template ctbegin<ReverseReplayDistanceEvaluator<frame_index_type>>();
            }
            auto lrend() const
                -> decltype(m_frameLayout.template tend<ReverseReplayDistanceEvaluator<frame_index_type>>())
            {
                return m_frameLayout.template tend<ReverseReplayDistanceEvaluator<frame_index_type>>();
            }
            auto clrend() const
                -> decltype(m_frameLayout.template ctend<ReverseReplayDistanceEvaluator<frame_index_type>>())
            {
                return m_frameLayout.template ctend<ReverseReplayDistanceEvaluator<frame_index_type>>();
            }
            const_iterator getFrameAt(const simulation_data_type& simData) const;
            auto getFrameAtInLoop(const simulation_data_type& simData) const
                -> decltype(std::declval<StaticFrameLayout>().lbegin()+std::declval<frame_distance_type>())
            {
                return lbegin()+getOffset(simData);
            }

            template<class _Iter>
            simulation_data_type frameDifference(const _Iter& _first, const _Iter& _next) const
            {
                return m_frameDuration;
            }

            simulation_data_type getDuration() const
            {
                return m_frameDuration * size();
            }
    };

    template<class FrameStateType, class FrameLayout>
    FrameStateType makeState(const FrameLayout& layout, const typename FrameLayout::simulation_data_type& simData)
    {
        using simulation_data_traits_type = typename FrameLayout::simulation_data_traits_type;
        //using simulation_data_type = typename FrameLayout::simulation_data_type;
        using factor_type = typename FrameStateType::factor_type;
        auto endFrame = layout.getFrameAtInLoop(simData);
        auto startFrame = endFrame++;
        factor_type factor{};
        if(startFrame != endFrame)
        {
            auto offset = simulation_data_traits_type::sub(simData, layout.getFrameStart(startFrame));
            if(!simulation_data_traits_type::eq(offset, simulation_data_traits_type::zero()))
            {
                auto frameDiff = layout.frameDifference(startFrame, endFrame);
                factor = simulation_data_traits_type::div(offset, frameDiff);
            } 
        } else {
            factor = simData;
        }
        return FrameStateType{factor, *startFrame, *endFrame};
    }

    template <class FrameStateType>
    class BasicDataDelegate
    {
        public:
            using frame_state_type = FrameStateType;
            BasicDataDelegate() = default;
            ~BasicDataDelegate() = default;

            void setState(const frame_state_type& state)
            {
                m_state = state;
            }
            const frame_state_type& getState() const
            {
                return m_state;
            }
        private:
            frame_state_type m_state;
            
    };
    using DataDelegate = BasicDataDelegate<FrameState>;


    /**
     * @brief Set new animation state in drawable.
     *
     * @tparam DataType Animation data type (ex. keyframe, skeletal etc.).
     * @tparam FrameStateType Current animation state.
     */
    template <
        class DataType,
        class DataDelegateType = DataDelegate
    >
    class BasicAnimationDataUpdater
    {
        public:
            using data_delegate_type = DataDelegateType;
            /**
             * @brief Data type. Must implement a method of signature. void update(const FrameState&)
             */
            using data_type = DataType;

            explicit BasicAnimationDataUpdater(const data_delegate_type& delegate) :
                m_animationDataList{},
                m_dataDelegate{delegate}
            {
            }
            ~BasicAnimationDataUpdater() = default;

            void addAnimationData(data_type* data)
            {
                m_animationDataList.push_back(data);
            }

            void removeAnimation(data_type* data)
            {
                m_animationDataList.erase(
                        std::remove(
                            m_animationDataList.begin(), m_animationDataList.end(), data
                        ), m_animationDataList.end()
                    );
            }

            void update()
            {
                const auto& fState = m_dataDelegate.getState();
                for(auto anim : m_animationDataList)
                    anim->setFrameState(fState);
            }
        private:
            std::list<data_type*> m_animationDataList;
            const data_delegate_type& m_dataDelegate;
    };

    template <class SimulationDataTraitsType>
    class BasicAnimationState
    {
        public:
            /**
             * @brief simulation_data_traits_type define operations for simulation_data_type.
             *
             * Requirements
             */
            using simulation_data_traits_type = SimulationDataTraitsType;
            using simulation_data_type = typename simulation_data_traits_type::simulation_data_type;
            BasicAnimationState(const simulation_data_type& duration = simulation_data_traits_type::unit(),
                    const simulation_data_type& state = simulation_data_traits_type::zero()
                ) : m_duration(duration), m_state(state){}
            void setState(const simulation_data_type& state)
            {
                m_state = state;
            }

            const simulation_data_type& getState() const
            {
                return m_state;
            }

            const simulation_data_type& getDuration() const
            {
                return m_duration;
            }
            ~BasicAnimationState() = default;
        private:
            simulation_data_type m_duration;
            simulation_data_type m_state;
    };

    template <template<class> class Player, template<class> class AnimationState = BasicAnimationState>
    class PlayerDelegate
    {
        public:
            using player_type = Player<PlayerDelegate>;
            using player_list_type = typename player_type::animation_list_type;
            using simulation_data_traits_type = typename player_type::simulation_data_traits_type;
            using simulation_data_type = typename simulation_data_traits_type::simulation_data_type;
            using animation_state_type = AnimationState<simulation_data_traits_type>;

            explicit PlayerDelegate(player_list_type* player = nullptr);
            explicit PlayerDelegate(const simulation_data_type& duration,
                    const simulation_data_type& state = simulation_data_type{});
            PlayerDelegate(const PlayerDelegate& playerDelegate);
            PlayerDelegate(PlayerDelegate&& playerDelegate);
            PlayerDelegate& operator=(const PlayerDelegate& playerDelegate);
            PlayerDelegate& operator=(PlayerDelegate&& playerDelegate);

            ~PlayerDelegate();

            void setPlayer(player_list_type* player = nullptr);

            bool startAnimation(player_list_type* player);

            bool stopAnimation(player_list_type* player);
            
            /**
             * @brief Return length of whole animation
             *
             * @return Animation duration.
             */
            simulation_data_type getDuration() const;

            /**
             * @brief Return animation current state.
             *
             * @return Current state.
             */
            const animation_state_type& getState() const;

            /**
             * @brief Return animation current state.
             *
             * @return Current state.
             */
            animation_state_type& getState();

            /**
             * @brief Set state for this delegate.
             *
             * @param state New state.
             */
            void setState(const animation_state_type& state);

            /**
             * @brief Set state for this delegate.
             *
             * @param state New state.
             */
            void setState(animation_state_type&& state);

            /**
             * @brief Return animation current state.
             *
             * @return Current state.
             */
            simulation_data_type getSimulationState() const;

            /**
             * @brief Update animation state.
             *
             * @param simData New state.
             */
            void run(const simulation_data_type& simData);

        private:
            player_list_type* m_player;
            animation_state_type m_state;
    };

    template <class AnimationDataDelegate, class AnimationLayout, class PlayerDelegate>
    class BasicAnimation
    {
        public:
            using player_delegate_type = PlayerDelegate;
            using player_type = typename player_delegate_type::player_type;
            using simulation_data_traits_type = typename player_type::simulation_data_traits_type;
            using simulation_data_type = typename player_type::simulation_data_type;
            using data_delegate_type = AnimationDataDelegate;
            using frame_state_type = typename data_delegate_type::frame_state_type;
            using animation_layout = AnimationLayout;
            using frame_distance_type = typename animation_layout::frame_distance_type;
            /**
             * @brief Default constructor.
             */
            BasicAnimation() = default;
            /**
             * @brief Destructor
             */
            ~BasicAnimation() = default;

            /**
             * @brief Sync data with animation state.
             */
            void updateData()
            {
                const auto& fState = makeState<frame_state_type>
                    (m_layout, m_playerDelegate.getSimulationState());
                m_dataDelegate.setState(fState);
            }

            void setPlayer(player_type* player = nullptr)
            {
                if(player)
                    m_playerDelegate.setPlayer(player->getAnimationsList());
            }

            const data_delegate_type& getDataDelegate() const
            {
                return m_dataDelegate;
            }

            const animation_layout& getLayout() const
            {
                return m_layout;
            }

            void setDurataion(const simulation_data_type& dur)
            {
                using animation_state_type = typename std::decay<decltype(m_playerDelegate.getState())>::type;
                const simulation_data_type oldDur = m_playerDelegate.getState().getDuration() != simulation_data_traits_type::zero() ?
                    m_playerDelegate.getState().getDuration() : dur;
                const simulation_data_type multiplier = simulation_data_traits_type::div(
                        dur,
                        oldDur
                    );
                const simulation_data_type& state = simulation_data_traits_type::mul(
                        m_playerDelegate.getState().getState(),
                        multiplier
                    );
                animation_state_type newState(dur, state);
                m_playerDelegate.setState(newState);
                m_layout = animation_layout(m_layout.size(), m_playerDelegate.getState().getDuration());
            }

            void setFrameCount(const frame_distance_type& size)
            {
                m_layout = animation_layout(size, m_playerDelegate.getState().getDuration());
            }

            simulation_data_type getDuration() const
            {
                return m_playerDelegate.getState().getDuration();
            }

            simulation_data_type getState() const
            {
                return m_playerDelegate.getState().getState();
            }
        private:
            data_delegate_type m_dataDelegate;
            animation_layout m_layout;
            player_delegate_type m_playerDelegate;
    };
} /* namespace aex */

#include "details/BasicAnimation.ipp"
#endif /* BASICANIMATION_HPP */

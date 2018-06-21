/**************************************************************
 *                                                            * 
 *      Filename:   BasicAnimation.ipp                        * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2015 wrz 18 11:32:23                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#ifndef BASICANIMATION_IPP
#define BASICANIMATION_IPP 

#include "../BasicAnimation.hpp"

namespace aex
{
    template <class T>
    BasicFrameIndex<T>::BasicFrameIndex() : BasicFrameIndex(T{})
    {
    }

    template <class T>
    BasicFrameIndex<T>::BasicFrameIndex(const T& index) : m_index(index)
    {
    }

    template <class T>
    BasicFrameIndex<T>&
    BasicFrameIndex<T>::operator+=(const frame_distance_type& rhs)
    {
        *this = *this + rhs;
        return *this;
    }

    template <class T>
    BasicFrameIndex<T>&
    BasicFrameIndex<T>::operator-=(const frame_distance_type& rhs)
    {
        *this = *this - rhs;
        return *this;
    }

    template <class T>
    BasicFrameIndex<T>::~BasicFrameIndex()
    {
    }

    template <class T>
    BasicFrameIndex<T>& operator++(BasicFrameIndex<T>& id)
    {
        using frame_distance_type = typename BasicFrameIndex<T>::frame_distance_type;
        id.m_index += frame_distance_type{1};
        return id;
    }

    template <class T>
    BasicFrameIndex<T> operator++(BasicFrameIndex<T>& id, int)
    {
        BasicFrameIndex<T> _t(id);
        ++id;
        return _t;
    }

    template <class T>
    BasicFrameIndex<T>& operator--(BasicFrameIndex<T>& id)
    {
        using frame_distance_type = typename BasicFrameIndex<T>::frame_distance_type;
        id.m_index -= frame_distance_type{1};
        return id;
    }

    template <class T>
    BasicFrameIndex<T> operator--(BasicFrameIndex<T>& id, int)
    {
        BasicFrameIndex<T> _t(id);
        --id;
        return _t;
    }

    template <class T>
    BasicFrameIndex<T> operator+(const BasicFrameIndex<T>& lhs,
            const typename BasicFrameIndex<T>::frame_distance_type& rhs)
    {
        return BasicFrameIndex<T>(T(lhs.m_index+rhs));
    }

    template <class T>
    BasicFrameIndex<T> operator-(const BasicFrameIndex<T>& lhs,
            const typename BasicFrameIndex<T>::frame_distance_type& rhs)
    {
        return BasicFrameIndex<T>(T(lhs.m_index - rhs));
    }

    template <class T>
    typename BasicFrameIndex<T>::frame_distance_type operator-(const BasicFrameIndex<T>& lhs,
            const BasicFrameIndex<T>& rhs)
    {
        return lhs.m_index - rhs.m_index;
    }

    template <class T>
    bool operator<(const BasicFrameIndex<T>& lhs, const BasicFrameIndex<T>& rhs)
    {
        return lhs.m_index < rhs.m_index;
    }

    template <class T>
    bool operator>(const BasicFrameIndex<T>& lhs, const BasicFrameIndex<T>& rhs)
    {
        return lhs.m_index > rhs.m_index;
    }

    template <class T>
    bool operator==(const BasicFrameIndex<T>& lhs, const BasicFrameIndex<T>& rhs)
    {
        return lhs.m_index == rhs.m_index;
    }

    template<class T>
    bool operator!=(const BasicFrameIndex<T>& lhs, const BasicFrameIndex<T>& rhs)
    {
        return lhs.m_index != rhs.m_index;
    }

    template <class FrameIndexType>
    typename DistanceEvaluatorBase<FrameIndexType>::frame_index_type
    DistanceEvaluatorBase<FrameIndexType>::rangeBegin(const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        return _rangeBegin;
    }

    template <class FrameIndexType>
    typename DistanceEvaluatorBase<FrameIndexType>::frame_index_type
    DistanceEvaluatorBase<FrameIndexType>::rangeEnd(const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        return _rangeBegin+_rangeSize;
    }

    template <class FrameIndexType>
    typename ReverseDistanceEvaluatorBase<FrameIndexType>::frame_index_type
    ReverseDistanceEvaluatorBase<FrameIndexType>::rangeBegin(const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        return _rangeBegin+frame_distance_type{_rangeSize-frame_distance_type{1}};
    }

    template <class FrameIndexType>
    typename ReverseDistanceEvaluatorBase<FrameIndexType>::frame_index_type
    ReverseDistanceEvaluatorBase<FrameIndexType>::rangeEnd(const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        return _rangeBegin+_rangeSize;
    }

    template <class FrameIndexType>
    typename ReplayDistanceEvaluator<FrameIndexType>::frame_index_type
    ReplayDistanceEvaluator<FrameIndexType>::inc(const frame_index_type& _p,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        frame_index_type _tp = _p;
        return ++_tp < _rangeBegin+_rangeSize ? _tp : _rangeBegin;
    }

    template <class FrameIndexType>
    typename ReplayDistanceEvaluator<FrameIndexType>::frame_index_type
    ReplayDistanceEvaluator<FrameIndexType>::dec(const frame_index_type& _p,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        frame_index_type _tp = _p > _rangeBegin ? _p : (_rangeBegin+_rangeSize);
        return --_tp;
    }

    template <class FrameIndexType>
    typename ReplayDistanceEvaluator<FrameIndexType>::frame_index_type
    ReplayDistanceEvaluator<FrameIndexType>::forward(const frame_index_type& _p,
            const frame_distance_type& _dist,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        const frame_distance_type _tdist = _dist % _rangeSize;
        const frame_index_type _tp{_p+_tdist};
        return _tp < _rangeBegin + _rangeSize ? _tp : _tp - _rangeSize;
    }

    template <class FrameIndexType>
    typename ReplayDistanceEvaluator<FrameIndexType>::frame_index_type
    ReplayDistanceEvaluator<FrameIndexType>::rewind(const frame_index_type& _p,
            const frame_distance_type& _dist,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        const frame_distance_type _tdist = _dist % _rangeSize;
        return _p - _rangeBegin > _tdist ? _p - _tdist : _rangeSize - _tdist + _p;
    }

    template <class FrameIndexType>
    typename ReplayDistanceEvaluator<FrameIndexType>::frame_distance_type
    ReplayDistanceEvaluator<FrameIndexType>::distance(const frame_index_type& _first,
            const frame_index_type& _last,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        return _last >= _first ? _last - _first : _rangeSize-_first+_last;
    }

    template <class FrameIndexType>
    typename NoReplayDistanceEvaluator<FrameIndexType>::frame_index_type
    NoReplayDistanceEvaluator<FrameIndexType>::inc(const frame_index_type& _p,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        frame_index_type _tp = _p;
        return ++_tp < _rangeBegin+_rangeSize ? _tp : DistanceEvaluatorBase<FrameIndexType>::rangeEnd(_rangeBegin, _rangeSize);
    }

    template <class FrameIndexType>
    typename NoReplayDistanceEvaluator<FrameIndexType>::frame_index_type
    NoReplayDistanceEvaluator<FrameIndexType>::dec(const frame_index_type& _p,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        return _p > _rangeBegin ? _p-frame_distance_type{1} : DistanceEvaluatorBase<FrameIndexType>::rangeEnd(_rangeBegin, _rangeSize);
    }

    template <class FrameIndexType>
    typename NoReplayDistanceEvaluator<FrameIndexType>::frame_index_type
    NoReplayDistanceEvaluator<FrameIndexType>::forward(const frame_index_type& _p,
            const frame_distance_type& _dist,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        const frame_distance_type _tdist = _dist % _rangeSize;
        const frame_index_type _tp{_p+_tdist};
        return _tp < _rangeBegin + _rangeSize ? _tp : DistanceEvaluatorBase<FrameIndexType>::rangeEnd(_rangeBegin, _rangeSize);
    }

    template <class FrameIndexType>
    typename NoReplayDistanceEvaluator<FrameIndexType>::frame_index_type
    NoReplayDistanceEvaluator<FrameIndexType>::rewind(const frame_index_type& _p,
            const frame_distance_type& _dist,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        const frame_distance_type _tdist = _dist % _rangeSize;
        return _p - _rangeBegin > _tdist ? _p - _tdist : DistanceEvaluatorBase<FrameIndexType>::rangeEnd(_rangeBegin, _rangeSize);
    }

    template <class FrameIndexType>
    typename NoReplayDistanceEvaluator<FrameIndexType>::frame_distance_type
    NoReplayDistanceEvaluator<FrameIndexType>::distance(const frame_index_type& _first,
            const frame_index_type& _last,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        return _last - _first;
    }

    template <class FrameIndexType>
    typename ReverseReplayDistanceEvaluator<FrameIndexType>::frame_index_type
    ReverseReplayDistanceEvaluator<FrameIndexType>::inc(const frame_index_type& _p,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        return ReplayDistanceEvaluator<FrameIndexType>::dec(_p, _rangeBegin, _rangeSize);
    }

    template <class FrameIndexType>
    typename ReverseReplayDistanceEvaluator<FrameIndexType>::frame_index_type
    ReverseReplayDistanceEvaluator<FrameIndexType>::dec(const frame_index_type& _p,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        return ReplayDistanceEvaluator<FrameIndexType>::inc(_p, _rangeBegin, _rangeSize);
    }

    template <class FrameIndexType>
    typename ReverseReplayDistanceEvaluator<FrameIndexType>::frame_index_type
    ReverseReplayDistanceEvaluator<FrameIndexType>::forward(const frame_index_type& _p,
            const frame_distance_type& _dist,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        return ReplayDistanceEvaluator<FrameIndexType>::rewind(_p, _dist, _rangeBegin, _rangeSize);
    }

    template <class FrameIndexType>
    typename ReverseReplayDistanceEvaluator<FrameIndexType>::frame_index_type
    ReverseReplayDistanceEvaluator<FrameIndexType>::rewind(const frame_index_type& _p,
            const frame_distance_type& _dist,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        return ReplayDistanceEvaluator<FrameIndexType>::forward(_p, _dist, _rangeBegin, _rangeSize);
    }

    template <class FrameIndexType>
    typename ReverseReplayDistanceEvaluator<FrameIndexType>::frame_distance_type
    ReverseReplayDistanceEvaluator<FrameIndexType>::distance(const frame_index_type& _first,
            const frame_index_type& _last,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        return _first >= _last ? _first - _last : _last + _rangeSize - _first;
    }

    template <class FrameIndexType>
    typename ReverseNoReplayDistanceEvaluator<FrameIndexType>::frame_index_type
    ReverseNoReplayDistanceEvaluator<FrameIndexType>::inc(const frame_index_type& _p,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        return NoReplayDistanceEvaluator<FrameIndexType>::dec(_p, _rangeBegin, _rangeSize);
    }

    template <class FrameIndexType>
    typename ReverseNoReplayDistanceEvaluator<FrameIndexType>::frame_index_type
    ReverseNoReplayDistanceEvaluator<FrameIndexType>::dec(const frame_index_type& _p,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        return NoReplayDistanceEvaluator<FrameIndexType>::inc(_p, _rangeBegin, _rangeSize);
    }

    template <class FrameIndexType>
    typename ReverseNoReplayDistanceEvaluator<FrameIndexType>::frame_index_type
    ReverseNoReplayDistanceEvaluator<FrameIndexType>::forward(const frame_index_type& _p,
            const frame_distance_type& _dist,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        return NoReplayDistanceEvaluator<FrameIndexType>::rewind(_p, _dist, _rangeBegin, _rangeSize);
    }

    template <class FrameIndexType>
    typename ReverseNoReplayDistanceEvaluator<FrameIndexType>::frame_index_type
    ReverseNoReplayDistanceEvaluator<FrameIndexType>::rewind(const frame_index_type& _p,
            const frame_distance_type& _dist,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        return NoReplayDistanceEvaluator<FrameIndexType>::forward(_p, _dist, _rangeBegin, _rangeSize);
    }

    template <class FrameIndexType>
    typename ReverseNoReplayDistanceEvaluator<FrameIndexType>::frame_distance_type
    ReverseNoReplayDistanceEvaluator<FrameIndexType>::distance(const frame_index_type& _first,
            const frame_index_type& _last,
            const frame_index_type& _rangeBegin,
            const frame_distance_type& _rangeSize)
    {
        return _first - _last;
    }

    template <class RangeType, class DistanceEvaluator>
    BasicFrameRangeIterator<RangeType, DistanceEvaluator>& operator++(BasicFrameRangeIterator<RangeType, DistanceEvaluator>& it)
    {
        using iterator = BasicFrameRangeIterator<RangeType, DistanceEvaluator>;
        using evaluator = typename iterator::evaluator;
        it.m_index = evaluator::inc(it.m_index, it.firstFrame(), it.size());
        return it;
    }

    template <class RangeType, class DistanceEvaluator>
    BasicFrameRangeIterator<RangeType, DistanceEvaluator> operator++(BasicFrameRangeIterator<RangeType, DistanceEvaluator>& it, int)
    {
        using iterator = BasicFrameRangeIterator<RangeType, DistanceEvaluator>;
        using evaluator = typename iterator::evaluator;
        iterator _tp(it);
        it.m_index = evaluator::inc(it.m_index, it.firstFrame(), it.size());
        return _tp;
    }

    template <class RangeType, class DistanceEvaluator>
    BasicFrameRangeIterator<RangeType, DistanceEvaluator>& operator--(BasicFrameRangeIterator<RangeType, DistanceEvaluator>& it)
    {
        using iterator = BasicFrameRangeIterator<RangeType, DistanceEvaluator>;
        using evaluator = typename iterator::evaluator;
        it.m_index = evaluator::dec(it.m_index, it.firstFrame(), it.size());
        return it;
    }

    template <class RangeType, class DistanceEvaluator>
    BasicFrameRangeIterator<RangeType, DistanceEvaluator> operator--(BasicFrameRangeIterator<RangeType, DistanceEvaluator>& it, int)
    {
        using iterator = BasicFrameRangeIterator<RangeType, DistanceEvaluator>;
        using evaluator = typename iterator::evaluator;
        iterator _tp(it);
        it.m_index = evaluator::dec(it.m_index, it.firstFrame(), it.size());
        return _tp;
    }

    template <class RangeType, class DistanceEvaluator>
    BasicFrameRangeIterator<RangeType, DistanceEvaluator> operator+(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& lhs,
            const typename BasicFrameRangeIterator<RangeType, DistanceEvaluator>::difference_type& rhs)
    {
        using iterator = BasicFrameRangeIterator<RangeType, DistanceEvaluator>;
        using evaluator = typename iterator::evaluator;
        iterator _tp(lhs);
        _tp.m_index = evaluator::forward(lhs.m_index, rhs, lhs.firstFrame(), lhs.size());
        return _tp;
    }

    template <class RangeType, class DistanceEvaluator>
    BasicFrameRangeIterator<RangeType, DistanceEvaluator> operator-(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& lhs,
            const typename BasicFrameRangeIterator<RangeType, DistanceEvaluator>::difference_type& rhs)
    {
        using iterator = BasicFrameRangeIterator<RangeType, DistanceEvaluator>;
        using evaluator = typename iterator::evaluator;
        iterator _tp(lhs);
        _tp.m_index = evaluator::rewind(lhs.m_index, rhs, lhs.firstFrame(), lhs.size());
        return _tp;
    }

    template <class RangeType, class DistanceEvaluator>
    typename BasicFrameRangeIterator<RangeType, DistanceEvaluator>::difference_type operator-(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& lhs, const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& rhs)
    {
        using iterator = BasicFrameRangeIterator<RangeType, DistanceEvaluator>;
        using evaluator = typename iterator::evaluator;
        return evaluator::distance(rhs.m_index, lhs.m_index, lhs.firstFrame(), lhs.size());
    }

    template <class RangeType, class DistanceEvaluator>
    typename BasicFrameRangeIterator<RangeType, DistanceEvaluator>::reference operator*(BasicFrameRangeIterator<RangeType, DistanceEvaluator>& it)
    {
        return it.m_index;
    }

    template <class RangeType, class DistanceEvaluator>
    typename BasicFrameRangeIterator<RangeType, DistanceEvaluator>::const_reference operator*(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& it)
    {
        return it.m_index;
    }

    template <class RangeType, class DistanceEvaluator>
    bool operator==(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& lhs, const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& rhs)
    {
        return lhs.m_index == rhs.m_index;
    }

    template <class RangeType, class DistanceEvaluator>
    bool operator!=(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& lhs, const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& rhs)
    {
        return lhs.m_index != rhs.m_index;
    }

    template <class RangeType, class DistanceEvaluator>
    bool operator<(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& lhs, const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& rhs)
    {
        return lhs.m_index < rhs.m_index;
    }

    template <class RangeType, class DistanceEvaluator>
    bool operator>(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& lhs, const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& rhs)
    {
        return lhs.m_index > rhs.m_index;
    }

    template <class RangeType, class DistanceEvaluator>
    bool operator<=(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& lhs, const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& rhs)
    {
        return lhs.m_index <= rhs.m_index;
    }

    template <class RangeType, class DistanceEvaluator>
    bool operator>=(const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& lhs, const BasicFrameRangeIterator<RangeType, DistanceEvaluator>& rhs)
    {
        return lhs.m_index >= rhs.m_index;
    }

    template<class RangeType, class DistanceEvaluator>
    BasicFrameRangeIterator<RangeType, DistanceEvaluator>::BasicFrameRangeIterator() :
        BasicFrameRangeIterator(value_type{}, nullptr)
    {
    }

    template<class RangeType, class DistanceEvaluator>
    BasicFrameRangeIterator<RangeType, DistanceEvaluator>::BasicFrameRangeIterator(const value_type& idx, const range_type* p) :
        m_index(idx),
        m_p(p)
    {
    }

    template<class RangeType, class DistanceEvaluator>
    BasicFrameRangeIterator<RangeType, DistanceEvaluator>::~BasicFrameRangeIterator()
    {
    }

    template <class RangeType, class DistanceEvaluator>
    typename BasicFrameRangeIterator<RangeType, DistanceEvaluator>::pointer
    BasicFrameRangeIterator<RangeType, DistanceEvaluator>::operator->()
    {
        return &(**this);
    }

    template <class RangeType, class DistanceEvaluator>
    const typename BasicFrameRangeIterator<RangeType, DistanceEvaluator>::pointer
    BasicFrameRangeIterator<RangeType, DistanceEvaluator>::operator->() const
    {
        return &(**this);
    }

    template <class RangeType, class DistanceEvaluator>
    BasicFrameRangeIterator<RangeType, DistanceEvaluator>&
    BasicFrameRangeIterator<RangeType, DistanceEvaluator>::operator+=(const difference_type& rhs)
    {
        *this = *this + rhs;
        return *this;
    }

    template <class RangeType, class DistanceEvaluator>
    BasicFrameRangeIterator<RangeType, DistanceEvaluator>&
    BasicFrameRangeIterator<RangeType, DistanceEvaluator>::operator-=(const difference_type& rhs)
    {
        *this = *this - rhs;
        return *this;
    }

    template <class RangeType, class DistanceEvaluator>
    typename BasicFrameRangeIterator<RangeType, DistanceEvaluator>::value_type
    BasicFrameRangeIterator<RangeType, DistanceEvaluator>::operator[](const difference_type& _dist) const
    {
        value_type _v = evaluator::forward(m_index, _dist, m_p->m_firstFrame, m_p->m_size);
        return _v;
    }

    template <class FrameIndexType>
    BasicFrameRange<FrameIndexType>::BasicFrameRange() : BasicFrameRange(FrameIndexType{})
    {
    }

    template <class FrameIndexType>
    BasicFrameRange<FrameIndexType>::BasicFrameRange(const frame_index_type& firstFrame) :
        BasicFrameRange(firstFrame, frame_distance_type{24})
    {
    }

    template <class FrameIndexType>
    BasicFrameRange<FrameIndexType>::BasicFrameRange(const frame_index_type& firstFrame,
            const frame_distance_type& _size) :
        m_firstFrame{firstFrame},
        m_size{_size}
    {
    }

    template<class FrameIndexType>
    template<class DistanceEvaluator>
    const BasicFrameRangeIterator<BasicFrameRange<FrameIndexType>, DistanceEvaluator>
    BasicFrameRange<FrameIndexType>::ctbegin() const
    {
        return BasicFrameRangeIterator<BasicFrameRange, DistanceEvaluator>{
            DistanceEvaluator::rangeBegin(m_firstFrame, m_size), this};
    }

    template<class FrameIndexType>
    template<class DistanceEvaluator>
    const BasicFrameRangeIterator<BasicFrameRange<FrameIndexType>, DistanceEvaluator>
    BasicFrameRange<FrameIndexType>::tbegin() const
    {
        return ctbegin<DistanceEvaluator>();
    }

    template<class FrameIndexType>
    template<class DistanceEvaluator>
    BasicFrameRangeIterator<BasicFrameRange<FrameIndexType>, DistanceEvaluator>
    BasicFrameRange<FrameIndexType>::tbegin()
    {
        return BasicFrameRangeIterator<BasicFrameRange, DistanceEvaluator>{ctbegin<DistanceEvaluator>()};
    }

    template<class FrameIndexType>
    template<class DistanceEvaluator>
    const BasicFrameRangeIterator<BasicFrameRange<FrameIndexType>, DistanceEvaluator>
    BasicFrameRange<FrameIndexType>::ctend() const
    {
        return BasicFrameRangeIterator<BasicFrameRange, DistanceEvaluator>{
            DistanceEvaluator::rangeEnd(m_firstFrame, m_size), this};
    }

    template<class FrameIndexType>
    template<class DistanceEvaluator>
    const BasicFrameRangeIterator<BasicFrameRange<FrameIndexType>, DistanceEvaluator>
    BasicFrameRange<FrameIndexType>::tend() const
    {
        return ctend<DistanceEvaluator>();
    }

    template<class FrameIndexType>
    template<class DistanceEvaluator>
    BasicFrameRangeIterator<BasicFrameRange<FrameIndexType>, DistanceEvaluator>
    BasicFrameRange<FrameIndexType>::tend()
    {
        return BasicFrameRangeIterator<BasicFrameRange, DistanceEvaluator>{ctend<DistanceEvaluator>()};
    }

    template<class FrameIndexType>
    BasicFrameRange<FrameIndexType>::~BasicFrameRange()
    {
    }

    template <class SimulationDataTraitsType, class FrameRangeType, class FrameRangeDefaultIter, class SimFrameCalc>
    StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::StaticFrameLayout() : StaticFrameLayout(frame_distance_type{24})
    {
        
    }

    template <class SimulationDataTraitsType, class FrameRangeType, class FrameRangeDefaultIter, class SimFrameCalc>
    StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::StaticFrameLayout(const frame_distance_type& totalFrames) :
        StaticFrameLayout(
                totalFrames,
                (simulation_data_traits_type::add(simulation_data_type{}, simulation_data_traits_type::unit()))
                )
    {
        
    }

    template <class SimulationDataTraitsType, class FrameRangeType, class FrameRangeDefaultIter, class SimFrameCalc>
    StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::StaticFrameLayout(const frame_distance_type& frameCount, const simulation_data_type& frameDuration) : StaticFrameLayout(frame_index_type{}, frameCount, frameDuration)
    {
    }

    template <class SimulationDataTraitsType, class FrameRangeType, class FrameRangeDefaultIter, class SimFrameCalc>
    StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::StaticFrameLayout(const frame_index_type& firstFrame, const frame_distance_type& frameCount, const simulation_data_type& frameDuration) :
        m_frameLayout{firstFrame, frameCount},
        m_frameDuration{sim_calc_type::div(frameDuration, frameCount)}
    {
    }

    template <class SimulationDataTraitsType, class FrameRangeType, class FrameRangeDefaultIter, class SimFrameCalc>
    template<class _Iter>
    typename StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::simulation_data_type
    StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::getFrameStart(const _Iter& frame) const
    {
        return sim_calc_type::mul(m_frameDuration, frame-m_frameLayout.template tbegin<typename _Iter::evaluator>());
    }

    template <class SimulationDataTraitsType, class FrameRangeType, class FrameRangeDefaultIter, class SimFrameCalc>
    template<class _Iter>
    typename StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::simulation_data_type
    StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::getFrameEnd(const _Iter& frame) const
    {
        using eval_type = typename _Iter::evaluator;
        const auto begin = m_frameLayout.template tbegin<eval_type>();
        const auto nextFrame = 
           frame+frame_distance_type{1} != m_frameLayout.template tend<eval_type>() ?
               frame+frame_distance_type{1} : begin;
        return sim_calc_type::mul(m_frameDuration, (nextFrame)-begin);
    }

    template<class SimulationDataTraitsType, class FrameRangeType, class FrameRangeDefaultIter, class SimFrameCalc>
    typename StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::const_iterator
    StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::getFrameAt(const simulation_data_type& simData) const
    {
        return begin()+getOffset(simData);
    }

    template <class SimulationDataTraitsType, class FrameRangeType, class FrameRangeDefaultIter, class SimFrameCalc>
    typename StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::const_iterator
    StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::begin() const
    {
        return m_frameLayout.template tbegin<typename const_iterator::evaluator>();
    }

    template <class SimulationDataTraitsType, class FrameRangeType, class FrameRangeDefaultIter, class SimFrameCalc>
    typename StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::const_iterator
    StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::cbegin() const
    {
        return m_frameLayout.template ctbegin<typename const_iterator::evaluator>();
    }

    template <class SimulationDataTraitsType, class FrameRangeType, class FrameRangeDefaultIter, class SimFrameCalc>
    typename StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::const_iterator
    StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::end() const
    {
        return m_frameLayout.template tend<typename const_iterator::evaluator>();
    }

    template <class SimulationDataTraitsType, class FrameRangeType, class FrameRangeDefaultIter, class SimFrameCalc>
    typename StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::const_iterator
    StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::cend() const
    {
        return m_frameLayout.template ctend<typename const_iterator::evaluator>();
    }

    template <class SimulationDataTraitsType, class FrameRangeType, class FrameRangeDefaultIter, class SimFrameCalc>
    std::size_t
    StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::size() const
    {
        return std::distance(cbegin(), cend());
    }

    template <class SimulationDataTraitsType, class FrameRangeType, class FrameRangeDefaultIter, class SimFrameCalc>
    StaticFrameLayout<SimulationDataTraitsType, FrameRangeType, FrameRangeDefaultIter, SimFrameCalc>::~StaticFrameLayout()
    {}

    template <template<class> class Player, template<class> class AnimationState>
    PlayerDelegate<Player, AnimationState>::PlayerDelegate(player_list_type* player) :
        m_player{nullptr}, m_state{}
    {
        setPlayer(player);
    }

    template <template<class> class Player, template<class> class AnimationState>
    PlayerDelegate<Player, AnimationState>::PlayerDelegate(
            const simulation_data_type& duration,
            const simulation_data_type& state) :
        PlayerDelegate()
    {
        m_state = animation_state_type{duration, state};
    }

    template <template<class> class Player, template<class> class AnimationState>
    PlayerDelegate<Player, AnimationState>::PlayerDelegate(const PlayerDelegate& playerDelegate) :
        m_player{nullptr}, m_state{}
    {
        *this = playerDelegate;
    }

    template <template<class> class Player, template<class> class AnimationState>
    PlayerDelegate<Player, AnimationState>::PlayerDelegate(PlayerDelegate&& playerDelegate) :
        m_player{nullptr}, m_state{}
    {
        *this = playerDelegate;
    }

    template <template<class> class Player, template<class> class AnimationState>
    PlayerDelegate<Player, AnimationState>&
    PlayerDelegate<Player, AnimationState>::operator=(const PlayerDelegate& rhs)
    {
        if(this != &rhs)
        {
            m_state = rhs.m_state;
            setPlayer(rhs.m_player);
        }
        return  *this;
    }

    template <template<class> class Player, template<class> class AnimationState>
    PlayerDelegate<Player, AnimationState>&
    PlayerDelegate<Player, AnimationState>::operator=(PlayerDelegate&& rhs)
    {
        if(this != &rhs)
        {
            m_state = std::move(rhs.m_state);
            auto player = rhs.m_player;
            rhs.setPlayer();
            setPlayer(player);
        }
        return  *this;
    }

    template <template<class> class Player, template<class> class AnimationState>
    PlayerDelegate<Player, AnimationState>::~PlayerDelegate()
    {
        setPlayer();
    }

    template <template<class> class Player, template<class> class AnimationState>
    void
    PlayerDelegate<Player, AnimationState>::setPlayer(player_list_type* player)
    {
        if(player != m_player)
        {
            if(m_player)
            {
                m_player->removeAnimation(this);
                m_player = nullptr;
            }
            if(player)
            {
                player->addAnimation(this);
            }
            m_player = player;
        }
    }

    template <template<class> class Player, template<class> class AnimationState>
    bool
    PlayerDelegate<Player, AnimationState>::startAnimation(player_list_type* player)
    {
        if(m_player)
            m_player->removeAnimation(this);
        m_state.setState(simulation_data_traits_type::zero());
        m_player = player;
        return true;
    }

    template <template<class> class Player, template<class> class AnimationState>
    bool
    PlayerDelegate<Player, AnimationState>::stopAnimation(player_list_type* player)
    {
        m_player = nullptr;
        return true;
    }

    template <template<class> class Player, template<class> class AnimationState>
    typename PlayerDelegate<Player, AnimationState>::simulation_data_type
    PlayerDelegate<Player, AnimationState>::getDuration() const
    {
        return m_player ? m_state.getDuration() : simulation_data_traits_type::zero();
    }

    template <template<class> class Player, template<class> class AnimationState>
    const typename PlayerDelegate<Player, AnimationState>::animation_state_type&
    PlayerDelegate<Player, AnimationState>::getState() const
    {
        return m_state;
    }

    template <template<class> class Player, template<class> class AnimationState>
    typename PlayerDelegate<Player, AnimationState>::animation_state_type&
    PlayerDelegate<Player, AnimationState>::getState()
    {
        return m_state;
    }

    template <template<class> class Player, template<class> class AnimationState>
    void
    PlayerDelegate<Player, AnimationState>::setState(const animation_state_type& state)
    {
        m_state = state;
    }

    template <template<class> class Player, template<class> class AnimationState>
    void
    PlayerDelegate<Player, AnimationState>::setState(animation_state_type&& state)
    {
        m_state = std::move(state);
    }

    template <template<class> class Player, template<class> class AnimationState>
    typename PlayerDelegate<Player, AnimationState>::simulation_data_type
    PlayerDelegate<Player, AnimationState>::getSimulationState() const
    {
        return m_player ? getState().getState() : simulation_data_traits_type::zero();
    }

    template <template<class> class Player, template<class> class AnimationState>
    void
    PlayerDelegate<Player, AnimationState>::run(const simulation_data_type& simData)
    {
        m_state.setState(simData);
    }
} /* namespace aex */

#endif /* BASICANIMATION_IPP */

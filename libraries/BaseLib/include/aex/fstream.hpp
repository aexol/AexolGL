#ifndef _AEX_FSTREAM
#define _AEX_FSTREAM

/*
 * Modification of libc++ fstream to use
 * SDL_RWops internaly
 */

#include <boost/filesystem.hpp>
#include <streambuf>
#include <cstddef>
#include <utility>
#include <ios>
#include <string>
#include <locale>
#include <typeinfo>
#include <memory>
#include <algorithm>
#include <istream>
#include <ostream>
#include <iostream>
#include <aex/Common.hpp>
#include <aex/NumericCast.hpp>

namespace aex
{
namespace io
{
//Wrappers around SDL functions
AEX_PUBLIC_API int64_t size(void* context);
AEX_PUBLIC_API int64_t seek(void* context, int64_t offset, int whence);
AEX_PUBLIC_API size_t read(void* context, void* ptr, size_t size, size_t maxnum);
AEX_PUBLIC_API size_t write(void* context, const void* ptr, size_t size, size_t num);
AEX_PUBLIC_API int close(void* context);
AEX_PUBLIC_API void* RWFromFile(const char* file, const char* mode);
AEX_PUBLIC_API int64_t tell(void* context);
CONSTEXPR uint32_t ARW_SEEK_SET = 0;
CONSTEXPR uint32_t ARW_SEEK_CUR = 1;
CONSTEXPR uint32_t ARW_SEEK_END = 2;

using std::ios_base;
using aex::string;
using std::codecvt_base;
using std::codecvt;
using std::bad_cast;
using std::unique_ptr;
using std::locale;
template<class CharT>
using char_traits = std::char_traits<CharT>;
template<class Facet>
inline bool has_facet(const locale& loc)
{
    return std::has_facet<Facet>(loc);
}
template<class Facet>
inline const Facet& use_facet(const locale& loc)
{
    return std::use_facet<Facet>(loc);
}
template<class T>
inline const T& max(const T& a, const T& b)
{
    return std::max(a, b);
}
template<class T, class Compare>
inline const T& max(const T& a, const T& b, Compare comp)
{
    return std::max(a, b, std::move(comp));
}
template<class T>
inline T max(std::initializer_list<T> ilist)
{
    return std::max(std::move(ilist));
}
template<class T, class Compare>
inline T max(std::initializer_list<T> ilist, Compare comp)
{
    return std::max(std::move(ilist), std::move(comp));
}
template<class T>
inline const T& min(const T& a, const T& b)
{
    return std::min(a, b);
}
template<class T, class Compare>
inline const T& min(const T& a, const T& b, Compare comp)
{
    return std::min(a, b, std::move(comp));
}
template<class T>
inline T min(std::initializer_list<T> ilist)
{
    return std::min(std::move(ilist));
}
template<class T, class Compare>
inline T min(std::initializer_list<T> ilist, Compare comp)
{
    return std::min(std::move(ilist), std::move(comp));
}
typedef std::ptrdiff_t ptrdiff_t;

template <class _CharT, class _Traits = char_traits<_CharT>>
class basic_filebuf
    : public std::basic_streambuf<_CharT, _Traits>
{
public:
    typedef _CharT                           char_type;
    typedef _Traits                          traits_type;
    typedef typename traits_type::int_type   int_type;
    typedef typename traits_type::pos_type   pos_type;
    typedef typename traits_type::off_type   off_type;
    typedef typename traits_type::state_type state_type;

    basic_filebuf();
    basic_filebuf(basic_filebuf&& rhs);
    virtual ~basic_filebuf();

    basic_filebuf& operator=(basic_filebuf&& rhs);
    void swap(basic_filebuf& rhs);

    bool is_open() const;
    basic_filebuf* open(const char* __s, ios_base::openmode __mode);
    basic_filebuf* open(const aex::string& __s, ios_base::openmode __mode);
    basic_filebuf* close();

protected:
    virtual int_type underflow();
    virtual int_type pbackfail(int_type __c = traits_type::eof());
    virtual int_type overflow(int_type __c = traits_type::eof());
    virtual std::basic_streambuf<char_type, traits_type>* setbuf(char_type* __s, std::streamsize __n);
    virtual pos_type seekoff(off_type __off, ios_base::seekdir __way,
                             ios_base::openmode __wch = ios_base::in | ios_base::out);
    virtual pos_type seekpos(pos_type __sp,
                             ios_base::openmode __wch = ios_base::in | ios_base::out);
    virtual int sync();
    virtual void imbue(const locale& __loc);

private:
    char*       m_extbuf;
    const char* m_extbufnext;
    const char* m_extbufend;
    char m_extbuf_min[8];
    size_t m_ebs;
    char_type* m_intbuf;
    size_t m_ibs;
    void* m_file;
    const codecvt<char_type, char, state_type>* m_cv;
    state_type m_st;
    state_type m_st_last;
    ios_base::openmode m_om;
    ios_base::openmode m_cm;
    bool m_owns_eb;
    bool m_owns_ib;
    bool m_always_noconv;

    bool __read_mode();
    void __write_mode();
#ifdef __GLIBCXX__
    void copy_base_gcc(const basic_filebuf& buf);
    void swap_base_gcc(basic_filebuf& buf);
#endif
};

template <class _CharT, class _Traits>
basic_filebuf<_CharT, _Traits>::basic_filebuf()
    : m_extbuf(0),
      m_extbufnext(0),
      m_extbufend(0),
      m_ebs(0),
      m_intbuf(0),
      m_ibs(0),
      m_file(0),
      m_cv(nullptr),
      m_st(),
      m_st_last(),
      m_om(),
      m_cm(),
      m_owns_eb(false),
      m_owns_ib(false),
      m_always_noconv(false)
{
    if(aex::io::has_facet<codecvt<char_type, char, state_type>>(this->getloc()))
    {
        m_cv = &aex::io::use_facet<codecvt<char_type, char, state_type>>(this->getloc());
        m_always_noconv = m_cv->always_noconv();
    }

    setbuf(0, 4096);
}


template <class _CharT, class _Traits>
basic_filebuf<_CharT, _Traits>::basic_filebuf(basic_filebuf&& rhs)
#ifndef __GLIBCXX__
    : std::basic_streambuf<_CharT, _Traits>(rhs)
#endif
{
#ifdef __GLIBCXX__
    copy_base_gcc(rhs);
#endif

    if(rhs.m_extbuf == rhs.m_extbuf_min)
    {
        m_extbuf = m_extbuf_min;
        m_extbufnext = m_extbuf + (rhs.m_extbufnext - rhs.m_extbuf);
        m_extbufend = m_extbuf + (rhs.m_extbufend - rhs.m_extbuf);
    }
    else
    {
        m_extbuf = rhs.m_extbuf;
        m_extbufnext = rhs.m_extbufnext;
        m_extbufend = rhs.m_extbufend;
    }

    m_ebs = rhs.m_ebs;
    m_intbuf = rhs.m_intbuf;
    m_ibs = rhs.m_ibs;
    m_file = rhs.m_file;
    m_cv = rhs.m_cv;
    m_st = rhs.m_st;
    m_st_last = rhs.m_st_last;
    m_om = rhs.m_om;
    m_cm = rhs.m_cm;
    m_owns_eb = rhs.m_owns_eb;
    m_owns_ib = rhs.m_owns_ib;
    m_always_noconv = rhs.m_always_noconv;

    if(rhs.pbase())
    {
        if(rhs.pbase() == rhs.m_intbuf)
        {
            this->setp(m_intbuf, m_intbuf + (rhs. epptr() - rhs.pbase()));
        }
        else
            this->setp((char_type*)m_extbuf,
                       (char_type*)m_extbuf + (rhs. epptr() - rhs.pbase()));

        this->pbump(numeric_cast<int>(rhs. pptr() - rhs.pbase()));
    }
    else if(rhs.eback())
    {
        if(rhs.eback() == rhs.m_intbuf)
            this->setg(m_intbuf, m_intbuf + (rhs.gptr() - rhs.eback()),
                       m_intbuf + (rhs.egptr() - rhs.eback()));
        else
            this->setg((char_type*)m_extbuf,
                       (char_type*)m_extbuf + (rhs.gptr() - rhs.eback()),
                       (char_type*)m_extbuf + (rhs.egptr() - rhs.eback()));
    }

    rhs.m_extbuf = 0;
    rhs.m_extbufnext = 0;
    rhs.m_extbufend = 0;
    rhs.m_ebs = 0;
    rhs.m_intbuf = 0;
    rhs.m_ibs = 0;
    rhs.m_file = 0;
    rhs.m_st = state_type();
    rhs.m_st_last = state_type();
    rhs.m_om = std::ios::openmode(0);
    rhs.m_cm = std::ios::openmode(0);
    rhs.m_owns_eb = false;
    rhs.m_owns_ib = false;
    rhs.setg(0, 0, 0);
    rhs.setp(0, 0);
}

template <class _CharT, class _Traits>

basic_filebuf<_CharT, _Traits>&
basic_filebuf<_CharT, _Traits>::operator=(basic_filebuf&& rhs)
{
    close();
    swap(rhs);
    return *this;
}

template <class _CharT, class _Traits>
basic_filebuf<_CharT, _Traits>::~basic_filebuf()
{
    try
    {
        close();
    }
    catch(...)
    {
    }

    if(m_owns_eb)
    {
        delete [] m_extbuf;
    }

    if(m_owns_ib)
    {
        delete [] m_intbuf;
    }
}

template <class _CharT, class _Traits>
void
basic_filebuf<_CharT, _Traits>::swap(basic_filebuf& rhs)
{
#ifdef __GLIBCXX__
    swap_base_gcc(rhs);
#else
    std::basic_streambuf<char_type, traits_type>::swap(rhs);
#endif

    if(m_extbuf != m_extbuf_min && rhs.m_extbuf != rhs.m_extbuf_min)
    {
        std::swap(m_extbuf, rhs.m_extbuf);
        std::swap(m_extbufnext, rhs.m_extbufnext);
        std::swap(m_extbufend, rhs.m_extbufend);
    }
    else
    {
        ptrdiff_t __ln = m_extbufnext - m_extbuf;
        ptrdiff_t __le = m_extbufend - m_extbuf;
        ptrdiff_t __rn = rhs.m_extbufnext - rhs.m_extbuf;
        ptrdiff_t __re = rhs.m_extbufend - rhs.m_extbuf;

        if(m_extbuf == m_extbuf_min && rhs.m_extbuf != rhs.m_extbuf_min)
        {
            m_extbuf = rhs.m_extbuf;
            rhs.m_extbuf = rhs.m_extbuf_min;
        }
        else if(m_extbuf != m_extbuf_min && rhs.m_extbuf == rhs.m_extbuf_min)
        {
            rhs.m_extbuf = m_extbuf;
            m_extbuf = m_extbuf_min;
        }

        m_extbufnext = m_extbuf + __rn;
        m_extbufend = m_extbuf + __re;
        rhs.m_extbufnext = rhs.m_extbuf + __ln;
        rhs.m_extbufend = rhs.m_extbuf + __le;
    }

    std::swap(m_ebs, rhs.m_ebs);
    std::swap(m_intbuf, rhs.m_intbuf);
    std::swap(m_ibs, rhs.m_ibs);
    std::swap(m_file, rhs.m_file);
    std::swap(m_cv, rhs.m_cv);
    std::swap(m_st, rhs.m_st);
    std::swap(m_st_last, rhs.m_st_last);
    std::swap(m_om, rhs.m_om);
    std::swap(m_cm, rhs.m_cm);
    std::swap(m_owns_eb, rhs.m_owns_eb);
    std::swap(m_owns_ib, rhs.m_owns_ib);
    std::swap(m_always_noconv, rhs.m_always_noconv);

    if(this->eback() == (char_type*)rhs.m_extbuf_min)
    {
        ptrdiff_t __n = this->gptr() - this->eback();
        ptrdiff_t __e = this->egptr() - this->eback();
        this->setg((char_type*)m_extbuf_min,
                   (char_type*)m_extbuf_min + __n,
                   (char_type*)m_extbuf_min + __e);
    }
    else if(this->pbase() == (char_type*)rhs.m_extbuf_min)
    {
        ptrdiff_t __n = this->pptr() - this->pbase();
        ptrdiff_t __e = this->epptr() - this->pbase();
        this->setp((char_type*)m_extbuf_min,
                   (char_type*)m_extbuf_min + __e);
        this->pbump(numeric_cast<int>(__n));
    }

    if(rhs.eback() == (char_type*)m_extbuf_min)
    {
        ptrdiff_t __n = rhs.gptr() - rhs.eback();
        ptrdiff_t __e = rhs.egptr() - rhs.eback();
        rhs.setg((char_type*)rhs.m_extbuf_min,
                 (char_type*)rhs.m_extbuf_min + __n,
                 (char_type*)rhs.m_extbuf_min + __e);
    }
    else if(rhs.pbase() == (char_type*)m_extbuf_min)
    {
        ptrdiff_t __n = rhs.pptr() - rhs.pbase();
        ptrdiff_t __e = rhs.epptr() - rhs.pbase();
        rhs.setp((char_type*)rhs.m_extbuf_min,
                 (char_type*)rhs.m_extbuf_min + __e);
        rhs.pbump(numeric_cast<int>(__n));
    }
}
#ifdef __GLIBCXX__
template<class _CharT, class _Traits>
void
basic_filebuf<_CharT, _Traits>::copy_base_gcc(const basic_filebuf<_CharT, _Traits>& fbuf)
{
    this->std::basic_streambuf<_CharT, _Traits>::_M_in_beg = fbuf.std::basic_streambuf<_CharT, _Traits>::_M_in_beg; ///< Start of get area.
    this->std::basic_streambuf<_CharT, _Traits>::_M_in_cur = fbuf.std::basic_streambuf<_CharT, _Traits>::_M_in_cur; ///< Current read area.
    this->std::basic_streambuf<_CharT, _Traits>::_M_in_end = fbuf.std::basic_streambuf<_CharT, _Traits>::_M_in_end; ///< End of get area.
    this->std::basic_streambuf<_CharT, _Traits>::_M_out_beg = fbuf.std::basic_streambuf<_CharT, _Traits>::_M_out_beg; ///< Start of put area.
    this->std::basic_streambuf<_CharT, _Traits>::_M_out_cur = fbuf.std::basic_streambuf<_CharT, _Traits>::_M_out_cur; ///< Current put area.
    this->std::basic_streambuf<_CharT, _Traits>::_M_out_end = fbuf.std::basic_streambuf<_CharT, _Traits>::_M_out_end; ///< End of put area.
    this->std::basic_streambuf<_CharT, _Traits>::_M_buf_locale = fbuf.std::basic_streambuf<_CharT, _Traits>::_M_buf_locale; ///< Current locale setting
}
template<class _CharT, class _Traits>
void
basic_filebuf<_CharT, _Traits>::swap_base_gcc(basic_filebuf<_CharT, _Traits>& fbuf)
{
    std::swap(this->std::basic_streambuf<_CharT, _Traits>::_M_in_beg, fbuf.std::basic_streambuf<_CharT, _Traits>::_M_in_beg);   ///< Start of get area.
    std::swap(this->std::basic_streambuf<_CharT, _Traits>::_M_in_cur, fbuf.std::basic_streambuf<_CharT, _Traits>::_M_in_cur);   ///< Current read area.
    std::swap(this->std::basic_streambuf<_CharT, _Traits>::_M_in_end, fbuf.std::basic_streambuf<_CharT, _Traits>::_M_in_end);   ///< End of get area.
    std::swap(this->std::basic_streambuf<_CharT, _Traits>::_M_out_beg, fbuf.std::basic_streambuf<_CharT, _Traits>::_M_out_beg);   ///< Start of put area.
    std::swap(this->std::basic_streambuf<_CharT, _Traits>::_M_out_cur, fbuf.std::basic_streambuf<_CharT, _Traits>::_M_out_cur);   ///< Current put area.
    std::swap(this->std::basic_streambuf<_CharT, _Traits>::_M_out_end, fbuf.std::basic_streambuf<_CharT, _Traits>::_M_out_end);   ///< End of put area.
    std::swap(this->std::basic_streambuf<_CharT, _Traits>::_M_buf_locale, fbuf.std::basic_streambuf<_CharT, _Traits>::_M_buf_locale);   ///< Current locale setting
}
#endif

template <class _CharT, class _Traits>

void
swap(basic_filebuf<_CharT, _Traits>& __x, basic_filebuf<_CharT, _Traits>& __y)
{
    __x.swap(__y);
}

template <class _CharT, class _Traits>
bool
basic_filebuf<_CharT, _Traits>::is_open() const
{
    return m_file != 0;
}

template <class _CharT, class _Traits>
basic_filebuf<_CharT, _Traits>*
basic_filebuf<_CharT, _Traits>::open(const char* __s, ios_base::openmode __mode)
{
    basic_filebuf<_CharT, _Traits>* __rt = 0;

    if(m_file == 0)
    { 
        __rt = this;
        const char* __mdstr;

        switch(uint(__mode & ~ios_base::ate))
        {
        case ios_base::out:
        case ios_base::out | ios_base::trunc:
            __mdstr = "w";
            break;

        case ios_base::out | ios_base::app:
        case ios_base::app:
            __mdstr = "a";
            break;

        case ios_base::in:
            __mdstr = "r";
            break;

        case ios_base::in | ios_base::out:
            __mdstr = "r+";
            break;

        case ios_base::in | ios_base::out | ios_base::trunc:
            __mdstr = "w+";
            break;

        case ios_base::in | ios_base::out | ios_base::app:
        case ios_base::in | ios_base::app:
            __mdstr = "a+";
            break;

        case ios_base::out | ios_base::binary:
        case ios_base::out | ios_base::trunc | ios_base::binary:
            __mdstr = "wb";
            break;

        case ios_base::out | ios_base::app | ios_base::binary:
        case ios_base::app | ios_base::binary:
            __mdstr = "ab";
            break;

        case ios_base::in | ios_base::binary:
            __mdstr = "rb";
            break;

        case ios_base::in | ios_base::out | ios_base::binary:
            __mdstr = "r+b";
            break;

        case ios_base::in | ios_base::out | ios_base::trunc | ios_base::binary:
            __mdstr = "w+b";
            break;

        case ios_base::in | ios_base::out | ios_base::app | ios_base::binary:
        case ios_base::in | ios_base::app | ios_base::binary:
            __mdstr = "a+b";
            break;

        default:
            __rt = 0;
            break;
        }

        if(__rt)
        {
            m_file = aex::io::RWFromFile(__s, __mdstr);

            if(m_file)
            {
                m_om = __mode;

                if(__mode & ios_base::ate)
                {
                    if(aex::io::seek(m_file, 0, ARW_SEEK_END))
                    {
                        aex::io::close(m_file);
                        m_file = 0;
                        __rt = 0;
                    }
                }
            }
            else
            {
                __rt = 0;
            }
        }
    }

    return __rt;
}

template <class _CharT, class _Traits>
basic_filebuf<_CharT, _Traits>*
basic_filebuf<_CharT, _Traits>::open(const aex::string& __s, ios_base::openmode __mode)
{
    return open(__s.c_str(), __mode);
}

template <class _CharT, class _Traits>
basic_filebuf<_CharT, _Traits>*
basic_filebuf<_CharT, _Traits>::close()
{
    basic_filebuf<_CharT, _Traits>* __rt = 0;

    if(m_file)
    {
        __rt = this;
        unique_ptr<void, int(*)(void*)> __h(m_file, aex::io::close);

        if(sync())
        {
            __rt = 0;
        }

        auto __p = __h.release();

        if(aex::io::close(__p) == 0)
        {
            m_file = 0;
        }
        else
        {
            __rt = 0;
        }
    }

    return __rt;
}

template <class _CharT, class _Traits>
typename basic_filebuf<_CharT, _Traits>::int_type
basic_filebuf<_CharT, _Traits>::underflow()
{
    if(m_file == 0)
    {
        return traits_type::eof();
    }

    bool __initial = __read_mode();
    char_type __1buf;

    if(this->gptr() == 0)
    {
        this->setg(&__1buf, &__1buf + 1, &__1buf + 1);
    }

    const size_t __unget_sz = __initial ? 0 : min<size_t>((this->egptr() - this->eback()) / 2, 4);
    int_type __c = traits_type::eof();

    if(this->gptr() == this->egptr())
    {
        memmove(this->eback(), this->egptr() - __unget_sz, __unget_sz * sizeof(char_type));

        if(m_always_noconv)
        {
            size_t __nmemb = static_cast<size_t>(this->egptr() - this->eback() - __unget_sz);
            __nmemb = aex::io::read(m_file, this->eback() + __unget_sz, 1, __nmemb);

            if(__nmemb != 0)
            {
                this->setg(this->eback(),
                           this->eback() + __unget_sz,
                           this->eback() + __unget_sz + __nmemb);
                __c = traits_type::to_int_type(*this->gptr());
            }
        }
        else
        {
            memmove(m_extbuf, m_extbufnext, m_extbufend - m_extbufnext);
            m_extbufnext = m_extbuf + (m_extbufend - m_extbufnext);
            m_extbufend = m_extbuf + (m_extbuf == m_extbuf_min ? sizeof(m_extbuf_min) : m_ebs);
            size_t __nmemb = std::min(static_cast<size_t>(m_ibs - __unget_sz),
                                      static_cast<size_t>(m_extbufend - m_extbufnext));
            codecvt_base::result __r;
            m_st_last = m_st;
            size_t __nr = aex::io::read(m_file, (void*)m_extbufnext, 1, __nmemb);

            if(__nr != 0)
            {
                if(!m_cv)
                {
                    throw bad_cast();
                }

                m_extbufend = m_extbufnext + __nr;
                char_type*  __inext;
                __r = m_cv->in(m_st, m_extbuf, m_extbufend, m_extbufnext,
                               this->eback() + __unget_sz,
                               this->eback() + m_ibs, __inext);

                if(__r == codecvt_base::noconv)
                {
                    this->setg((char_type*)m_extbuf, (char_type*)m_extbuf, (char_type*)m_extbufend);
                    __c = traits_type::to_int_type(*this->gptr());
                }
                else if(__inext != this->eback() + __unget_sz)
                {
                    this->setg(this->eback(), this->eback() + __unget_sz, __inext);
                    __c = traits_type::to_int_type(*this->gptr());
                }
            }
        }
    }
    else
    {
        __c = traits_type::to_int_type(*this->gptr());
    }

    if(this->eback() == &__1buf)
    {
        this->setg(0, 0, 0);
    }

    return __c;
}

template <class _CharT, class _Traits>
typename basic_filebuf<_CharT, _Traits>::int_type
basic_filebuf<_CharT, _Traits>::pbackfail(int_type __c)
{
    if(m_file && this->eback() < this->gptr())
    {
        if(traits_type::eq_int_type(__c, traits_type::eof()))
        {
            this->gbump(-1);
            return traits_type::not_eof(__c);
        }

        if((m_om & ios_base::out) ||
                traits_type::eq(traits_type::to_char_type(__c), this->gptr()[-1]))
        {
            this->gbump(-1);
            *this->gptr() = traits_type::to_char_type(__c);
            return __c;
        }
    }

    return traits_type::eof();
}

template <class _CharT, class _Traits>
typename basic_filebuf<_CharT, _Traits>::int_type
basic_filebuf<_CharT, _Traits>::overflow(int_type __c)
{
    if(m_file == 0)
    {
        return traits_type::eof();
    }

    __write_mode();
    char_type __1buf;
    char_type* __pb_save = this->pbase();
    char_type* __epb_save = this->epptr();

    if(!traits_type::eq_int_type(__c, traits_type::eof()))
    {
        if(this->pptr() == 0)
        {
            this->setp(&__1buf, &__1buf + 1);
        }

        *this->pptr() = traits_type::to_char_type(__c);
        this->pbump(1);
    }

    if(this->pptr() != this->pbase())
    {
        if(m_always_noconv)
        {
            size_t __nmemb = static_cast<size_t>(this->pptr() - this->pbase());

            if(aex::io::write(m_file, this->pbase(), sizeof(char_type), __nmemb) != __nmemb)
            {
                return traits_type::eof();
            }
        }
        else
        {
            char* __extbe = m_extbuf;
            codecvt_base::result __r;

            do
            {
                if(!m_cv)
                {
                    throw bad_cast();
                }

                const char_type* __e;
                __r = m_cv->out(m_st, this->pbase(), this->pptr(), __e,
                                m_extbuf, m_extbuf + m_ebs, __extbe);

                if(__e == this->pbase())
                {
                    return traits_type::eof();
                }

                if(__r == codecvt_base::noconv)
                {
                    size_t __nmemb = static_cast<size_t>(this->pptr() - this->pbase());

                    if(aex::io::write(m_file, this->pbase(), 1, __nmemb) != __nmemb)
                    {
                        return traits_type::eof();
                    }
                }
                else if(__r == codecvt_base::ok || __r == codecvt_base::partial)
                {
                    size_t __nmemb = static_cast<size_t>(__extbe - m_extbuf);

                    if(aex::io::write(m_file, m_extbuf, 1, __nmemb) != __nmemb)
                    {
                        return traits_type::eof();
                    }

                    if(__r == codecvt_base::partial)
                    {
                        this->setp((char_type*)__e, this->pptr());
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#elif defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4244 )
#elif
#error "Unsupported compiler"
#endif
                        this->pbump(this->epptr() - this->pbase());
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning( pop )
#elif
#error "Unsupported compiler"
#endif
                    }
                }
                else
                {
                    return traits_type::eof();
                }
            }
            while(__r == codecvt_base::partial);
        }

        this->setp(__pb_save, __epb_save);
    }

    return traits_type::not_eof(__c);
}

template <class _CharT, class _Traits>
std::basic_streambuf<_CharT, _Traits>*
basic_filebuf<_CharT, _Traits>::setbuf(char_type* __s, std::streamsize __n)
{
    this->setg(0, 0, 0);
    this->setp(0, 0);

    if(m_owns_eb)
    {
        delete [] m_extbuf;
    }

    if(m_owns_ib)
    {
        delete [] m_intbuf;
    }

    m_ebs = __n;

    if(m_ebs > sizeof(m_extbuf_min))
    {
        if(m_always_noconv && __s)
        {
            m_extbuf = (char*)__s;
            m_owns_eb = false;
        }
        else
        {
            m_extbuf = new char[m_ebs];
            m_owns_eb = true;
        }
    }
    else
    {
        m_extbuf = m_extbuf_min;
        m_ebs = sizeof(m_extbuf_min);
        m_owns_eb = false;
    }

    if(!m_always_noconv)
    {
        m_ibs = aex::io::max<std::streamsize>(__n, sizeof(m_extbuf_min));

        if(__s && m_ibs >= sizeof(m_extbuf_min))
        {
            m_intbuf = __s;
            m_owns_ib = false;
        }
        else
        {
            m_intbuf = new char_type[m_ibs];
            m_owns_ib = true;
        }
    }
    else
    {
        m_ibs = 0;
        m_intbuf = 0;
        m_owns_ib = false;
    }

    return this;
}

template <class _CharT, class _Traits>
typename basic_filebuf<_CharT, _Traits>::pos_type
basic_filebuf<_CharT, _Traits>::seekoff(off_type __off, ios_base::seekdir __way,
                                        ios_base::openmode)
{
    if(!m_cv)
    {
        throw bad_cast();
    }

    int __width = m_cv->encoding();

    if(m_file == 0 || (__width <= 0 && __off != 0) || sync())
    {
        return pos_type(off_type(-1));
    }

    // __width > 0 || __off == 0
    int __whence;

    switch(__way)
    {
    case ios_base::beg:
        __whence = ARW_SEEK_SET;
        break;

    case ios_base::cur:
        __whence = ARW_SEEK_CUR;
        break;

    case ios_base::end:
        __whence = ARW_SEEK_END;
        break;

    default:
        return pos_type(off_type(-1));
    }

    if(aex::io::seek(m_file, __width > 0 ? __width* __off : 0, __whence) == -1)
    {
        return pos_type(off_type(-1));
    }

    pos_type __r = aex::io::tell(m_file);


    __r.state(m_st);
    return __r;
}

template <class _CharT, class _Traits>
typename basic_filebuf<_CharT, _Traits>::pos_type
basic_filebuf<_CharT, _Traits>::seekpos(pos_type __sp, ios_base::openmode)
{
    if(m_file == 0 || sync())
    {
        return pos_type(off_type(-1));
    }

    if(aex::io::seek(m_file, __sp, ARW_SEEK_SET) == -1)
    {
        return pos_type(off_type(-1));
    }

    m_st = __sp.state();
    return __sp;
}

template <class _CharT, class _Traits>
int
basic_filebuf<_CharT, _Traits>::sync()
{
    if(m_file == 0)
    {
        return 0;
    }

    if(!m_cv)
    {
        throw bad_cast();
    }

    if(m_cm & ios_base::out)
    {
        if(this->pptr() != this->pbase())
            if(overflow() == traits_type::eof())
            {
                return -1;
            }

        codecvt_base::result __r;

        do
        {
            char* __extbe;
            __r = m_cv->unshift(m_st, m_extbuf, m_extbuf + m_ebs, __extbe);
            size_t __nmemb = static_cast<size_t>(__extbe - m_extbuf);

            if(aex::io::write(m_file, m_extbuf, 1, __nmemb) != __nmemb)
            {
                return -1;
            }
        }
        while(__r == codecvt_base::partial);

        if(__r == codecvt_base::error)
        {
            return -1;
        }
    }
    else if(m_cm & ios_base::in)
    {
        off_type __c;
        state_type __state = m_st_last;
        bool __update_st = false;

        if(m_always_noconv)
        {
            __c = this->egptr() - this->gptr();
        }
        else
        {
            int __width = m_cv->encoding();
            __c = m_extbufend - m_extbufnext;

            if(__width > 0)
            {
                __c += __width * (this->egptr() - this->gptr());
            }
            else
            {
                if(this->gptr() != this->egptr())
                {
                    const int __off =  m_cv->length(__state, m_extbuf,
                                                    m_extbufnext,
                                                    this->gptr() - this->eback());
                    __c += m_extbufnext - m_extbuf - __off;
                    __update_st = true;
                }
            }
        }

        if(aex::io::seek(m_file, -__c, ARW_SEEK_CUR) == -1)
        {
            return -1;
        }

        if(__update_st)
        {
            m_st = __state;
        }

        m_extbufnext = m_extbufend = m_extbuf;
        this->setg(0, 0, 0);
        m_cm = std::ios_base::openmode(0);
    }

    return 0;
}

template <class _CharT, class _Traits>
void
basic_filebuf<_CharT, _Traits>::imbue(const locale& __loc)
{
    sync();
    m_cv = &aex::io::use_facet<codecvt<char_type, char, state_type>>(__loc);
    bool __old_anc = m_always_noconv;
    m_always_noconv = m_cv->always_noconv();

    if(__old_anc != m_always_noconv)
    {
        this->setg(0, 0, 0);
        this->setp(0, 0);

        // invariant, char_type is char, else we couldn't get here
        if(m_always_noconv)   // need to dump __intbuf_
        {
            if(m_owns_eb)
            {
                delete [] m_extbuf;
            }

            m_owns_eb = m_owns_ib;
            m_ebs = m_ibs;
            m_extbuf = (char*)m_intbuf;
            m_ibs = 0;
            m_intbuf = 0;
            m_owns_ib = false;
        }
        else  // need to obtain an m_intbuf.
        {
            // If m_extbuf is user-supplied, use it, else new __intbuf_
            if(!m_owns_eb && m_extbuf != m_extbuf_min)
            {
                m_ibs = m_ebs;
                m_intbuf = (char_type*)m_extbuf;
                m_owns_ib = false;
                m_extbuf = new char[m_ebs];
                m_owns_eb = true;
            }
            else
            {
                m_ibs = m_ebs;
                m_intbuf = new char_type[m_ibs];
                m_owns_ib = true;
            }
        }
    }
}

template <class _CharT, class _Traits>
bool
basic_filebuf<_CharT, _Traits>::__read_mode()
{
    if(!(m_cm & ios_base::in))
    {
        this->setp(0, 0);

        if(m_always_noconv)
            this->setg((char_type*)m_extbuf,
                       (char_type*)m_extbuf + m_ebs,
                       (char_type*)m_extbuf + m_ebs);
        else
        {
            this->setg(m_intbuf, m_intbuf + m_ibs, m_intbuf + m_ibs);
        }

        m_cm = ios_base::in;
        return true;
    }

    return false;
}

template <class _CharT, class _Traits>
void
basic_filebuf<_CharT, _Traits>::__write_mode()
{
    if(!(m_cm & ios_base::out))
    {
        this->setg(0, 0, 0);

        if(m_ebs > sizeof(m_extbuf_min))
        {
            if(m_always_noconv)
                this->setp((char_type*)m_extbuf,
                           (char_type*)m_extbuf + (m_ebs - 1));
            else
            {
                this->setp(m_intbuf, m_intbuf + (m_ibs - 1));
            }
        }
        else
        {
            this->setp(0, 0);
        }

        m_cm = ios_base::out;
    }
}

template <class _CharT, class _Traits = char_traits<_CharT>>
class basic_ifstream
    : public std::basic_istream<_CharT, _Traits>
{
public:
    typedef _CharT                         char_type;
    typedef _Traits                        traits_type;
    typedef typename traits_type::int_type int_type;
    typedef typename traits_type::pos_type pos_type;
    typedef typename traits_type::off_type off_type;

    basic_ifstream();
    explicit basic_ifstream(const char* __s, ios_base::openmode __mode = ios_base::in);
    explicit basic_ifstream(const string& __s, ios_base::openmode __mode = ios_base::in);
    basic_ifstream(basic_ifstream&& __rhs);
    basic_ifstream& operator=(basic_ifstream&& __rhs);
    void swap(basic_ifstream& __rhs);

    basic_filebuf<char_type, traits_type>* rdbuf() const;
    bool is_open() const;
    void open(const char* __s, ios_base::openmode __mode = ios_base::in);
    void open(const string& __s, ios_base::openmode __mode = ios_base::in);
    void close();

protected:
#ifdef __GLIBCXX__
    void set_rdbuf(std::basic_streambuf<char_type, traits_type>* sb);
#endif
private:
    basic_filebuf<char_type, traits_type> __sb_;
};

template <class _CharT, class _Traits>
basic_ifstream<_CharT, _Traits>::basic_ifstream()
    : std::basic_istream<char_type, traits_type>(&__sb_)
{
}

template <class _CharT, class _Traits>
basic_ifstream<_CharT, _Traits>::basic_ifstream(const char* __s, ios_base::openmode __mode)
    : std::basic_istream<char_type, traits_type>(&__sb_)
{
    if(__sb_.open(__s, __mode | ios_base::in) == 0)
    {
        this->setstate(ios_base::failbit);
    }
}

template <class _CharT, class _Traits>
basic_ifstream<_CharT, _Traits>::basic_ifstream(const string& __s, ios_base::openmode __mode)
    : std::basic_istream<char_type, traits_type>(&__sb_)
{
    if(__sb_.open(__s, __mode | ios_base::in) == 0)
    {
        this->setstate(ios_base::failbit);
    }
}

#ifdef __GLIBCXX__
template<class _CharT, class _Traits>
basic_ifstream<_CharT, _Traits>::basic_ifstream(basic_ifstream&& __st_)   // libstdc++ lacks move semantics for their basic streams, workaround
{
    *this = std::move(__st_);
}

template <class _CharT, class _Traits>
basic_ifstream<_CharT, _Traits>&
basic_ifstream<_CharT, _Traits>::operator=(basic_ifstream&& __rhs)
{
    typedef std::basic_ios<_CharT, _Traits> ios;
    if(this != &__rhs)
    {
        auto _rhs_state = __rhs.rdstate();
        __rhs.ios::rdbuf(nullptr);
        __sb_ = std::move(__rhs.__sb_);
        this->set_rdbuf(&__sb_);
        this->copyfmt(__rhs);
        this->clear(_rhs_state);
        __rhs.clear();
    }
    return *this;
}

#else
template <class _CharT, class _Traits>
basic_ifstream<_CharT, _Traits>::basic_ifstream(basic_ifstream&& __rhs)
    : std::basic_istream<char_type, traits_type>(std::move(__rhs)),
      __sb_(std::move(__rhs.__sb_))
{
    this->set_rdbuf(&__sb_);
}

template <class _CharT, class _Traits>
basic_ifstream<_CharT, _Traits>&
basic_ifstream<_CharT, _Traits>::operator=(basic_ifstream&& __rhs)
{
    std::basic_istream<char_type, traits_type>::operator=(std::move(__rhs));
    __sb_ = std::move(__rhs.__sb_);
    return *this;
}
#endif


template <class _CharT, class _Traits>
void
basic_ifstream<_CharT, _Traits>::swap(basic_ifstream& __rhs)
{
    std::basic_istream<char_type, traits_type>::swap(__rhs);
    __sb_.swap(__rhs.__sb_);
}

template <class _CharT, class _Traits>
void
swap(basic_ifstream<_CharT, _Traits>& __x, basic_ifstream<_CharT, _Traits>& __y)
{
    __x.swap(__y);
}

template <class _CharT, class _Traits>
basic_filebuf<_CharT, _Traits>*
basic_ifstream<_CharT, _Traits>::rdbuf() const
{
    return const_cast<basic_filebuf<char_type, traits_type>*>(&__sb_);
}

template <class _CharT, class _Traits>
bool
basic_ifstream<_CharT, _Traits>::is_open() const
{
    return __sb_.is_open();
}

template <class _CharT, class _Traits>
void
basic_ifstream<_CharT, _Traits>::open(const char* __s, ios_base::openmode __mode)
{
    if(__sb_.open(__s, __mode | ios_base::in))
    {
        this->clear();
    }
    else
    {
        this->setstate(ios_base::failbit);
    }
}

template <class _CharT, class _Traits>
void
basic_ifstream<_CharT, _Traits>::open(const string& __s, ios_base::openmode __mode)
{
    if(__sb_.open(__s, __mode | ios_base::in))
    {
        this->clear();
    }
    else
    {
        this->setstate(ios_base::failbit);
    }
}

template <class _CharT, class _Traits>
void
basic_ifstream<_CharT, _Traits>::close()
{
    if(__sb_.close() == 0)
    {
        this->setstate(ios_base::failbit);
    }
}
#ifdef __GLIBCXX__
template<class _CharT, class _Traits>
void basic_ifstream<_CharT, _Traits>::set_rdbuf(std::basic_streambuf<_CharT, _Traits>* sb)
{
    std::basic_ios<_CharT, _Traits>::rdbuf(sb);
}
#endif

// basic_ofstream

template <class _CharT, class _Traits = char_traits<_CharT>>
class basic_ofstream
    : public std::basic_ostream<_CharT, _Traits>
{
public:
    typedef _CharT                         char_type;
    typedef _Traits                        traits_type;
    typedef typename traits_type::int_type int_type;
    typedef typename traits_type::pos_type pos_type;
    typedef typename traits_type::off_type off_type;

    basic_ofstream();
    explicit basic_ofstream(const char* __s, ios_base::openmode __mode = ios_base::out);
    explicit basic_ofstream(const string& __s, ios_base::openmode __mode = ios_base::out);
    basic_ofstream(basic_ofstream&& __rhs);
    basic_ofstream& operator=(basic_ofstream&& __rhs);
    void swap(basic_ofstream& __rhs);

    basic_filebuf<char_type, traits_type>* rdbuf() const;
    bool is_open() const;
    void open(const char* __s, ios_base::openmode __mode = ios_base::out);
    void open(const string& __s, ios_base::openmode __mode = ios_base::out);
    void close();

protected:
#ifdef __GLIBCXX__
    void set_rdbuf(std::basic_streambuf<char_type, traits_type>* sb);
#endif
private:
    basic_filebuf<char_type, traits_type> __sb_;
};

template <class _CharT, class _Traits>
basic_ofstream<_CharT, _Traits>::basic_ofstream()
    : std::basic_ostream<char_type, traits_type>(&__sb_)
{
}

template <class _CharT, class _Traits>
basic_ofstream<_CharT, _Traits>::basic_ofstream(const char* __s, ios_base::openmode __mode)
    : std::basic_ostream<char_type, traits_type>(&__sb_)
{
    if(__sb_.open(__s, __mode | ios_base::out) == 0)
    {
        this->setstate(ios_base::failbit);
    }
}

template <class _CharT, class _Traits>
basic_ofstream<_CharT, _Traits>::basic_ofstream(const string& __s, ios_base::openmode __mode)
    : std::basic_ostream<char_type, traits_type>(&__sb_)
{
    if(__sb_.open(__s, __mode | ios_base::out) == 0)
    {
        this->setstate(ios_base::failbit);
    }
}
#ifdef __GLIBCXX__
template<class _CharT, class _Traits>
basic_ofstream<_CharT, _Traits>::basic_ofstream(basic_ofstream&& __st_)   // libstdc++ lacks move semantics for their basic streams, workaround with copying basic_ios
{
    *this = std::move(__st_);
}

template <class _CharT, class _Traits>
basic_ofstream<_CharT, _Traits>&
basic_ofstream<_CharT, _Traits>::operator=(basic_ofstream&& __rhs)
{
    typedef std::basic_ios<_CharT, _Traits> ios;
    if(this != &__rhs)
    {
        auto _rhs_state = __rhs.rdstate();
        __rhs.ios::rdbuf(nullptr);
        __sb_ = std::move(__rhs.__sb_);
        this->set_rdbuf(&__sb_);
        this->copyfmt(__rhs);
        this->clear(_rhs_state);
        __rhs.clear();
    }
    return *this;
}

#else

template <class _CharT, class _Traits>
basic_ofstream<_CharT, _Traits>::basic_ofstream(basic_ofstream&& __rhs)
    : std::basic_ostream<char_type, traits_type>(std::move(__rhs)),
      __sb_(std::move(__rhs.__sb_))
{
    this->set_rdbuf(&__sb_);
}

template <class _CharT, class _Traits>
basic_ofstream<_CharT, _Traits>&
basic_ofstream<_CharT, _Traits>::operator=(basic_ofstream&& __rhs)
{
    std::basic_ostream<char_type, traits_type>::operator=(std::move(__rhs));
    __sb_ = std::move(__rhs.__sb_);
    return *this;
}
#endif

template <class _CharT, class _Traits>
void
basic_ofstream<_CharT, _Traits>::swap(basic_ofstream& __rhs)
{
    std::basic_ostream<char_type, traits_type>::swap(__rhs);
    __sb_.swap(__rhs.__sb_);
}

template <class _CharT, class _Traits>
void
swap(basic_ofstream<_CharT, _Traits>& __x, basic_ofstream<_CharT, _Traits>& __y)
{
    __x.swap(__y);
}

template <class _CharT, class _Traits>
basic_filebuf<_CharT, _Traits>*
basic_ofstream<_CharT, _Traits>::rdbuf() const
{
    return const_cast<basic_filebuf<char_type, traits_type>*>(&__sb_);
}

template <class _CharT, class _Traits>
bool
basic_ofstream<_CharT, _Traits>::is_open() const
{
    return __sb_.is_open();
}

template <class _CharT, class _Traits>
void
basic_ofstream<_CharT, _Traits>::open(const char* __s, ios_base::openmode __mode)
{
    if(__sb_.open(__s, __mode | ios_base::out))
    {
        this->clear();
    }
    else
    {
        this->setstate(ios_base::failbit);
    }
}

template <class _CharT, class _Traits>
void
basic_ofstream<_CharT, _Traits>::open(const string& __s, ios_base::openmode __mode)
{
    if(__sb_.open(__s, __mode | ios_base::out))
    {
        this->clear();
    }
    else
    {
        this->setstate(ios_base::failbit);
    }
}

template <class _CharT, class _Traits>
void
basic_ofstream<_CharT, _Traits>::close()
{
    if(__sb_.close() == 0)
    {
        this->setstate(ios_base::failbit);
    }
}
#ifdef __GLIBCXX__
template<class _CharT, class _Traits>
void basic_ofstream<_CharT, _Traits>::set_rdbuf(std::basic_streambuf<_CharT, _Traits>* sb)
{
    std::basic_ios<_CharT, _Traits>::rdbuf(sb);
}
#endif

// basic_fstream

template <class _CharT, class _Traits = char_traits<_CharT>>
class basic_fstream
    : public std::basic_iostream<_CharT, _Traits>
{
public:
    typedef _CharT                         char_type;
    typedef _Traits                        traits_type;
    typedef typename traits_type::int_type int_type;
    typedef typename traits_type::pos_type pos_type;
    typedef typename traits_type::off_type off_type;

    basic_fstream();
    explicit basic_fstream(const char* __s, ios_base::openmode __mode = ios_base::in | ios_base::out);
    explicit basic_fstream(const string& __s, ios_base::openmode __mode = ios_base::in | ios_base::out);
    basic_fstream(basic_fstream&& __rhs);
    basic_fstream& operator=(basic_fstream&& __rhs);
    void swap(basic_fstream& __rhs);

    basic_filebuf<char_type, traits_type>* rdbuf() const;
    bool is_open() const;
    void open(const char* __s, ios_base::openmode __mode = ios_base::in | ios_base::out);
    void open(const string& __s, ios_base::openmode __mode = ios_base::in | ios_base::out);
    void close();
protected:
#ifdef __GLIBCXX__
    void set_rdbuf(std::basic_streambuf<char_type, traits_type>* sb);
#endif
private:
    basic_filebuf<char_type, traits_type> __sb_;
};

template <class _CharT, class _Traits>
basic_fstream<_CharT, _Traits>::basic_fstream()
    : std::basic_iostream<char_type, traits_type>(&__sb_)
{
}

template <class _CharT, class _Traits>
basic_fstream<_CharT, _Traits>::basic_fstream(const char* __s, ios_base::openmode __mode)
    : std::basic_iostream<char_type, traits_type>(&__sb_)
{
    if(__sb_.open(__s, __mode) == 0)
    {
        this->setstate(ios_base::failbit);
    }
}

template <class _CharT, class _Traits>
basic_fstream<_CharT, _Traits>::basic_fstream(const string& __s, ios_base::openmode __mode)
    : std::basic_iostream<char_type, traits_type>(&__sb_)
{
    if(__sb_.open(__s, __mode) == 0)
    {
        this->setstate(ios_base::failbit);
    }
}

#ifdef __GLIBCXX__
template<class _CharT, class _Traits>
basic_fstream<_CharT, _Traits>::basic_fstream(basic_fstream&& __st_)   // libstdc++ lacks move semantics for their basic streams, workaround with copying basic_ios
{
    *this = std::move(__st_);
}

template <class _CharT, class _Traits>
basic_fstream<_CharT, _Traits>&
basic_fstream<_CharT, _Traits>::operator=(basic_fstream&& __rhs)
{
    typedef std::basic_ios<_CharT, _Traits> ios;
    if(this != &__rhs)
    {
        auto _rhs_state = __rhs.rdstate();
        __rhs.ios::rdbuf(nullptr);
        __sb_ = std::move(__rhs.__sb_);
        this->set_rdbuf(&__sb_);
        this->copyfmt(__rhs);
        this->clear(_rhs_state);
        __rhs.clear();
    }
    return *this;
}

#else
template <class _CharT, class _Traits>
basic_fstream<_CharT, _Traits>::basic_fstream(basic_fstream&& __rhs)
    : std::basic_iostream<char_type, traits_type>(std::move(__rhs)),
      __sb_(std::move(__rhs.__sb_))
{
    this->set_rdbuf(&__sb_);
}

template <class _CharT, class _Traits>
basic_fstream<_CharT, _Traits>&
basic_fstream<_CharT, _Traits>::operator=(basic_fstream&& __rhs)
{
    std::basic_iostream<char_type, traits_type>::operator=(std::move(__rhs));
    __sb_ = std::move(__rhs.__sb_);
    return *this;
}
#endif

template <class _CharT, class _Traits>
void
basic_fstream<_CharT, _Traits>::swap(basic_fstream& __rhs)
{
    std::basic_iostream<char_type, traits_type>::swap(__rhs);
    __sb_.swap(__rhs.__sb_);
}

template <class _CharT, class _Traits>
void
swap(basic_fstream<_CharT, _Traits>& __x, basic_fstream<_CharT, _Traits>& __y)
{
    __x.swap(__y);
}

template <class _CharT, class _Traits>
basic_filebuf<_CharT, _Traits>*
basic_fstream<_CharT, _Traits>::rdbuf() const
{
    return const_cast<basic_filebuf<char_type, traits_type>*>(&__sb_);
}

template <class _CharT, class _Traits>
bool
basic_fstream<_CharT, _Traits>::is_open() const
{
    return __sb_.is_open();
}

template <class _CharT, class _Traits>
void
basic_fstream<_CharT, _Traits>::open(const char* __s, ios_base::openmode __mode)
{
    if(__sb_.open(__s, __mode))
    {
        this->clear();
    }
    else
    {
        this->setstate(ios_base::failbit);
    }
}

template <class _CharT, class _Traits>
void
basic_fstream<_CharT, _Traits>::open(const string& __s, ios_base::openmode __mode)
{
    if(__sb_.open(__s, __mode))
    {
        this->clear();
    }
    else
    {
        this->setstate(ios_base::failbit);
    }
}

template <class _CharT, class _Traits>
void
basic_fstream<_CharT, _Traits>::close()
{
    if(__sb_.close() == 0)
    {
        this->setstate(ios_base::failbit);
    }
}
#ifdef __GLIBCXX__
template<class _CharT, class _Traits>
void basic_fstream<_CharT, _Traits>::set_rdbuf(std::basic_streambuf<_CharT, _Traits>* sb)
{
    std::basic_ios<_CharT, _Traits>::rdbuf(sb);
}
#endif
}
typedef io::basic_filebuf<char> filebuf;
typedef io::basic_fstream<char> fstream;
typedef io::basic_ifstream<char> ifstream;
typedef io::basic_ofstream<char> ofstream;
}

#endif


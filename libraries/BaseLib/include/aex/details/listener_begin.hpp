#define DEFINE_LISTENER(ACTION, TYPE, ...)\
template<class Func>\
class ACTION ## ListenerBase; \
\
template<class R, class... Args> \
class ACTION ## ListenerBase<R(Args...)>\
{\
        typedef R(*Func)(ACTION ## ListenerBase<R(Args...)>*, Args... args);\
    public:\
        ACTION ## ListenerBase(Func on ## ACTION) : m_on ## ACTION(on ## ACTION){}\
        R operator()(Args... args)\
        {\
            return m_on ## ACTION(this, args...);\
        }\
    private:\
        Func m_on ## ACTION;\
};\
\
template<class TListenerImpl, class Func = void()> class ACTION ## ListenerImpl;\
\
template<class TListenerImpl, class R, class... Args>\
class ACTION ## ListenerImpl<TListenerImpl, R(Args...)> : public ACTION ## ListenerBase<R(Args...)>\
{\
    public:\
        typedef ACTION ## ListenerBase<R(Args...)> Base;\
        ACTION ## ListenerImpl() : Base(&executeOn ## ACTION){}\
    private:\
        static R executeOn ## ACTION(ACTION ## ListenerBase<R(Args...)>* obj_impl, Args... args)\
        {\
            return static_cast<TListenerImpl*>(obj_impl)->on ## ACTION(args...);\
        }\
};\
\
template<class T>\
using  IOn ## ACTION ## Listener = ACTION ## ListenerImpl<T, TYPE(__VA_ARGS__)>;\
typedef ACTION ## ListenerBase<TYPE(__VA_ARGS__)> On ## ACTION ## Listener;\

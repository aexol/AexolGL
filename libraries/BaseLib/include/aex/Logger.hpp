/* Based on great guide from DrDobbs */
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <aex/Common.hpp>
#include <ostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <sstream>


namespace aex
{
    enum class log_severity
    {
        Debug,
        Info,
        Error,
        Warning
    };

    class AEX_PUBLIC_API log_policy_interface
    {
    public:
        virtual void open_ostream() = 0;
        virtual void close_ostream() = 0;
        virtual void write(const aex::string& msg) = 0;
        virtual inline ~log_policy_interface() = 0;
    };

    log_policy_interface::~log_policy_interface() {}

    class AEX_PUBLIC_API file_log_policy : log_policy_interface
    {
    private:
        unique_ptr<std::ofstream> m_outStream;
        aex::string m_name;
    public:
        file_log_policy(const aex::string& name) : m_outStream(new std::ofstream), m_name(name) {}
        void open_ostream();
        void close_ostream();
        void write(const aex::string& msg);
        ~file_log_policy();
    };

    class AEX_PUBLIC_API stdout_log_policy : log_policy_interface
    {
    public:
        stdout_log_policy() {}
        void open_ostream();
        void close_ostream();
        void write(const aex::string& msg);
        ~stdout_log_policy();
    };


    template<typename log_policy>
    class logger
    {
    private:
        unsigned m_logLineNumber;
        aex::string getTime();
        aex::string getLoglineHeader();
        std::stringstream m_logStream;
        unique_ptr<log_policy> m_policy;
        std::mutex m_writeMutex;

        void print_impl();
        template<typename First, typename... Rest> void
        print_impl(First&& param1, Rest&& ... param);
    public:
        logger(std::unique_ptr<log_policy>& policy);
        template<log_severity severity, typename... Args>
        void print(Args&& ... args);
        ~logger();

        friend class LogFactory;
    };

    template<typename log_policy>
    aex::string logger<log_policy>::getTime()
    {
        auto t = std::time(nullptr);
        auto tm = aex::localtime(&t);
        char mbstr[100];
        aex::string time_str("");
        if (std::strftime(mbstr, sizeof(mbstr), "%c", &tm)) {
                time_str = mbstr;
            }
        return time_str.substr(0 , time_str.size() - 1);
    }

    template<typename log_policy>
    aex::string logger<log_policy>::getLoglineHeader()
    {
        std::stringstream header;
        header.str("");
        header.fill('0');
        header.width(7);
        header << m_logLineNumber++ << " < " << getTime() << " - ";
        header.fill('0');
        header.width(7);
        header << clock() << " > ~ ";
        return header.str();
    }

    template<typename log_policy>
    logger<log_policy>::logger(unique_ptr<log_policy>& policy)
    {
        m_logLineNumber = 0;
        m_policy.swap(policy);

        if(!m_policy)
        {
            throw(std::runtime_error("LOGGER: Policy creation failed."));
        }

        m_policy->open_ostream();
    }

    template<typename log_policy>
    logger<log_policy>::~logger()
    {
        if(m_policy)
        {
            m_policy->close_ostream();
        }
    }

    template<typename log_policy> void
    logger<log_policy>::print_impl()
    {
        m_policy->write(getLoglineHeader() + m_logStream.str());
        m_logStream.str("");
    }

    template<typename log_policy>
    template<typename First, typename... Rest>
    void
    logger<log_policy>::print_impl(First&& param1, Rest&& ... param)
    {
        m_logStream << param1;
        print_impl(aex::forward<Rest>(param)...);
    }
    template<typename log_policy>
    template<log_severity severity, typename... Args>
    void
    logger<log_policy>::print(Args&& ...args)
    {
        m_writeMutex.lock();

        switch(severity)
        {
        case log_severity::Debug:
            m_logStream << "<DEBUG> : ";
            break;

        case log_severity::Info:
            m_logStream << "<INFO> : ";
            break;

        case log_severity::Error:
            m_logStream << "<ERROR> : ";
            break;

        case log_severity::Warning:
            m_logStream << "<WARNING> : ";
            break;

        default:
            break;
        }

        print_impl(aex::forward<Args>(args)...);
        m_writeMutex.unlock();
    }

    class AEX_PUBLIC_API LogFactory
    {
    private:
        template<class Policy>
        static std::once_flag& getOnceFlag()
        {
            static std::once_flag flagPerPolicy;
            return flagPerPolicy;
        }

        template<class Policy>
        static logger<Policy>* createLoggerImp(const aex::string& name)
        {
            auto ptr = std::unique_ptr<Policy>(new Policy(name));
            return new logger<Policy>(ptr);
        }
        template<class Policy>
        static logger<Policy>* createLoggerImp()
        {
            auto ptr = std::unique_ptr<Policy>(new Policy());
            return new logger<Policy>(ptr);
        }

    template<class Policy>
    static std::atomic<logger<Policy>*>& _getLogger()
    {
        static std::atomic<logger<Policy>*> Logger;
        return Logger;
    }

    public:
        template<class Policy>
        static logger<Policy>* getLogger(const aex::string& name)
        {
        static std::mutex once_logger;
        std::atomic<logger<Policy>*>& Logger = _getLogger<Policy>();
        if(!Logger.load())
            {
            std::lock_guard<std::mutex> lock(once_logger);
            if(!Logger.load())
            {
                Logger = LogFactory::createLoggerImp<Policy>(name);
                std::atexit([]()
                        {
                            std::atomic<logger<Policy>*>& Logger = _getLogger<Policy>();
                            delete Logger.load();
                            Logger = nullptr;
            });
        }
        }
        return Logger.load();
    }

        template<class Policy>
        static logger<Policy>* getLogger()
        {
        static std::mutex once_logger;
        std::atomic<logger<Policy>*>& Logger = _getLogger<Policy>();
        if(!Logger.load())
            {
            std::lock_guard<std::mutex> lock(once_logger);
            if(!Logger.load())
            {
                Logger = LogFactory::createLoggerImp<Policy>();
                std::atexit([]()
                        {
                            std::atomic<logger<Policy>*>& Logger = _getLogger<Policy>();
                            delete Logger.load();
                            Logger = nullptr;
            });
        }
        }
        return Logger.load();
    }
    };
    template<log_severity severity, class Logger>
    struct LoggerPrinter
    {
        template<class... Args>
        void operator()(Logger* log, Args&& ... args)
        {
            if(log != nullptr)
            {
                log->template print<severity>(std::forward<Args>(args)...);
            }
        }
    };
}

#if LOGGING_LEVEL == DEBUG_BUILD
template<class... Args> void LOG_DEBUG(Args&& ... args)
{
    ::aex::LoggerPrinter<::aex::log_severity::Debug, ::aex::logger<::aex::stdout_log_policy>> {} \
            (::aex::LogFactory::getLogger<::aex::stdout_log_policy>(), std::forward<Args>(args)...);
}
template<class... Args> void LOG_FILE_DEBUG(Args&& ... args)
{
    ::aex::LoggerPrinter<::aex::log_severity::Debug, ::aex::logger<::aex::file_log_policy>> {}(::aex::LogFactory::getLogger<::aex::file_log_policy>("LOG.TXT"), std::forward<Args>(args)...);
}
template<class... Args> void LOG(Args&& ... args)
{
    ::aex::LoggerPrinter<::aex::log_severity::Info, ::aex::logger<::aex::stdout_log_policy>> {}(::aex::LogFactory::getLogger<::aex::stdout_log_policy>(), std::forward<Args>(args)...);
}
template<class... Args> void LOG_FILE(Args&& ... args)
{
    ::aex::LoggerPrinter<::aex::log_severity::Info, ::aex::logger<::aex::file_log_policy>> {}(::aex::LogFactory::getLogger<::aex::file_log_policy>("LOG.TXT"), std::forward<Args>(args)...);
}
template<class... Args> void LOG_ERR(Args&& ... args)
{
    ::aex::LoggerPrinter<::aex::log_severity::Error, ::aex::logger<::aex::stdout_log_policy>> {}(::aex::LogFactory::getLogger<::aex::stdout_log_policy>(), std::forward<Args>(args)...);
}
template<class... Args> void LOG_FILE_ERR(Args&& ... args)
{
    ::aex::LoggerPrinter<::aex::log_severity::Error, ::aex::logger<::aex::file_log_policy>> {}(::aex::LogFactory::getLogger<::aex::file_log_policy>("LOG.TXT"), std::forward<Args>(args)...);
}
template<class... Args> void LOG_WARN(Args&& ... args)
{
    ::aex::LoggerPrinter<::aex::log_severity::Warning, ::aex::logger<::aex::stdout_log_policy>> {}(::aex::LogFactory::getLogger<::aex::stdout_log_policy>(), std::forward<Args>(args)...);
}
template<class... Args> void LOG_FILE_WARN(Args&& ... args)
{
    ::aex::LoggerPrinter<::aex::log_severity::Warning, ::aex::logger<::aex::file_log_policy>> {}(::aex::LogFactory::getLogger<::aex::file_log_policy>("LOG.TXT"), std::forward<Args>(args)...);
}

#include <cassert>
#define _AEX_ASSERT_LOG(condition)\
do{\
    if(!(condition))\
    {\
        ::aex::LoggerPrinter<::aex::log_severity::Debug, ::aex::logger<::aex::stdout_log_policy>>{} \
            (::aex::LogFactory::getLogger<::aex::stdout_log_policy>(), "assert failed at: ",__FILE__,":", __LINE__ ); \
        abort();\
    }\
}while(0)\

#elif LOGGING_LEVEL==RELEASE_BUILD

template<class... Args> void LOG_DEBUG(Args&& ...) {}
template<class... Args> void LOG_FILE_DEBUG(Args&& ...) {}
template<class... Args> void LOG(Args&& ... args)
{
    ::aex::LoggerPrinter<::aex::log_severity::Info, ::aex::logger<::aex::stdout_log_policy>> {}(::aex::LogFactory::getLogger<::aex::stdout_log_policy>(), std::forward<Args>(args)...);
}
template<class... Args> void LOG_FILE(Args&& ... args)
{
    ::aex::LoggerPrinter<::aex::log_severity::Info, ::aex::logger<::aex::file_log_policy>> {}(::aex::LogFactory::getLogger<::aex::file_log_policy>("LOG.TXT"), std::forward<Args>(args)...);
}
template<class... Args> void LOG_ERR(Args&& ... args)
{
    ::aex::LoggerPrinter<::aex::log_severity::Error, ::aex::logger<::aex::stdout_log_policy>> {}(::aex::LogFactory::getLogger<::aex::stdout_log_policy>(), std::forward<Args>(args)...);
}
template<class... Args> void LOG_FILE_ERR(Args&& ... args)
{
    ::aex::LoggerPrinter<::aex::log_severity::Error, ::aex::logger<::aex::file_log_policy>> {}(::aex::LogFactory::getLogger<::aex::file_log_policy>("LOG.TXT"), std::forward<Args>(args)...);
}
template<class... Args> void LOG_WARN(Args&& ... args)
{
    ::aex::LoggerPrinter<::aex::log_severity::Warning, ::aex::logger<::aex::stdout_log_policy>> {}(::aex::LogFactory::getLogger<::aex::stdout_log_policy>(), std::forward<Args>(args)...);
}
template<class... Args> void LOG_FILE_WARN(Args&& ... args)
{
    ::aex::LoggerPrinter<::aex::log_severity::Warning, ::aex::logger<::aex::file_log_policy>> {}(::aex::LogFactory::getLogger<::aex::file_log_policy>("LOG.TXT"), std::forward<Args>(args)...);
}


#define _AEX_ASSERT_LOG(condition) while(0){}

#else

template<class... Args> void LOG_DEBUG(Args&& ...) {}
template<class... Args> void LOG_FILE_DEBUG(Args&& ...) {}
template<class... Args> void LOG(Args&& ...) {}
template<class... Args> void LOG_FILE(Args&& ...) {}
template<class... Args> void LOG_ERR(Args&& ...) {}
template<class... Args> void LOG_FILE_ERR(Args&& ...) {}
template<class... Args> void LOG_WARN(Args&& ...) {}
template<class... Args> void LOG_FILE_WARN(Args&& ...) {}

#endif

#endif

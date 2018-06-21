#ifndef FB_RESULTS_AEX_HEADER_FILE
#define FB_RESULTS_AEX_HEADER_FILE

#include <iostream>
#include <string>

namespace aex
{
    namespace fb
    {
        class FBResult
        {
        public:
            static const int RESULT_SUCCEED = 1;
            static const int RESULT_CANCELED = 2;
            static const int RESULT_ERROR = 3;
        public:
            FBResult(int resultCode, const std::string& errorMsg);
            virtual ~FBResult();

            bool isSucceed() const;
            bool isCanceled() const;
            bool isError() const;
            std::string getErrorMsg() const;
        protected:
            int m_resultCode;
            std::string m_errorMsg;
        };
    }
}

#endif

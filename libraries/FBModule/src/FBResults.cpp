#include "aex/fb/FBResults.hpp"

namespace aex
{
    namespace fb
    {
        FBResult::FBResult(int resultCode, const std::string& errorMsg) :
        m_resultCode(resultCode),
        m_errorMsg(errorMsg)
        {
        }

        FBResult::~FBResult()
        {
        }

        bool FBResult::isSucceed() const
        {
            return m_resultCode == RESULT_SUCCEED;
        }

        bool FBResult::isCanceled() const
        {
            return m_resultCode == RESULT_CANCELED;
        }

        bool FBResult::isError() const
        {
            return m_resultCode == RESULT_ERROR;
        }

        std::string FBResult::getErrorMsg() const
        {
            return m_errorMsg;
        }
    }
}

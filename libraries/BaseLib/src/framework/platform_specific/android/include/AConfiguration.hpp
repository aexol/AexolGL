#ifndef __AEX_ACONFIGURATION_HPP
#define __AEX_ACONFIGURATION_HPP
namespace aex
{
    enum class ADeviceRotation
    {
        ROTATION_0,
        ROTATION_90,
        ROTATION_180,
        ROTATION_270
    };
    class AConfiguration
    {
    public:
        ADeviceRotation getDefaultRotation();
    private:
    };
}
#endif

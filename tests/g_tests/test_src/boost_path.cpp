#include <aex/fstream.hpp>
#include <aex/FileManager.hpp>
#include "gtest/gtest.h"
#include <boost/filesystem.hpp>

using boost::filesystem::path;

TEST(Boost, path)
{
    path p1("Data/some_asset.ass");
    path p2("/Some/Path/In/Root");
    path p3("");
    p2 /= p1;
    p3 /= p1;
    EXPECT_EQ(p2.string(), "/Some/Path/In/Root/Data/some_asset.ass");
    EXPECT_EQ(p3.string(), "Data/some_asset.ass");
}

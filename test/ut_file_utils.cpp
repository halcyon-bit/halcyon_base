#define _CRT_SECURE_NO_WARNINGS
#include "base/file/file_utils.h"
#include "base/time/timestamp.h"

#include "gtest/gtest.h"

using namespace halcyon;

TEST(FileOptTest, exists)
{
    bool ret = base::file::exists("./123");
    EXPECT_EQ(ret, false);

    ret = base::file::exists("./123.txt");
    EXPECT_EQ(ret, false);

    ret = base::file::exists("../../bin");
    EXPECT_EQ(ret, true);
    ret = base::file::exists("../../bin/");
    EXPECT_EQ(ret, true);

    ret = base::file::exists("../../test/ut_file_opt.cpp");
    EXPECT_EQ(ret, true);
}

TEST(FileOptTest, dir)
{
    // create dir
    {
        bool ret = base::file::createDir("./test/test");
        EXPECT_EQ(ret, true);

        ret = base::file::createDir("./test");
        EXPECT_EQ(ret, false);

        ret = base::file::exists("./test/test");
        EXPECT_EQ(ret, true);
    }

    base::sleep(500);

    // remove dir
    {
        bool ret = base::file::removeDir("./test/test");
        EXPECT_EQ(ret, true);

        ret = base::file::removeDir("./test/test");
        EXPECT_EQ(ret, false);

        ret = base::file::exists("./test/test");
        EXPECT_EQ(ret, false);

        ret = base::file::exists("./test");
        EXPECT_EQ(ret, true);

        ret = base::file::removeDir("./test");
        EXPECT_EQ(ret, true);

        ret = base::file::exists("./test");
        EXPECT_EQ(ret, false);
    }

    // remove dir
    {
        bool ret = base::file::createDir("./test/test");
        EXPECT_EQ(ret, true);

        base::sleep(500);

        ret = base::file::removeDir("./test");
        EXPECT_EQ(ret, true);

        ret = base::file::exists("./test/test");
        EXPECT_EQ(ret, false);

        ret = base::file::exists("./test");
        EXPECT_EQ(ret, false);
    }

    // remove dir
    {
        bool ret = base::file::createDir("./test/test");
        EXPECT_EQ(ret, true);

        std::string filePath{ "./test/test/" };
        for (size_t i = 0; i < 10; ++i) {
            std::string tmp{ filePath };
            tmp.append(std::to_string(i)).append(".txt");
            FILE* fp = fopen(tmp.c_str(), "a+");
            fclose(fp);
        }

        ret = base::file::exists("./test/test/9.txt");
        EXPECT_EQ(ret, true);

        base::sleep(500);

        ret = base::file::removeDir("./test/test");
        EXPECT_EQ(ret, true);

        ret = base::file::exists("./test/test/9.txt");
        EXPECT_EQ(ret, false);

        ret = base::file::exists("./test/test");
        EXPECT_EQ(ret, false);

        ret = base::file::exists("./test");
        EXPECT_EQ(ret, true);

        ret = base::file::removeDir("./test");
        EXPECT_EQ(ret, true);

        ret = base::file::exists("./test");
        EXPECT_EQ(ret, false);
    }

    // remove dir
    {
        bool ret = base::file::createDir("./test/test");
        EXPECT_EQ(ret, true);

        std::string filePath{ "./test/" };
        for (size_t i = 0; i < 10; ++i) {
            std::string tmp{ filePath };
            tmp.append(std::to_string(i)).append(".txt");
            FILE* fp = fopen(tmp.c_str(), "a+");
            fclose(fp);
        }

        filePath.append("test/");
        for (size_t i = 0; i < 10; ++i) {
            std::string tmp{ filePath };
            tmp.append(std::to_string(i)).append(".txt");
            FILE* fp = fopen(tmp.c_str(), "a+");
            fclose(fp);
        }

        ret = base::file::exists("./test/test/9.txt");
        EXPECT_EQ(ret, true);
        ret = base::file::exists("./test/9.txt");
        EXPECT_EQ(ret, true);

        base::sleep(500);

        ret = base::file::removeDir("./test/");
        EXPECT_EQ(ret, true);

        ret = base::file::exists("./test/test/9.txt");
        EXPECT_EQ(ret, false);
        ret = base::file::exists("./test/9.txt");
        EXPECT_EQ(ret, false);
        ret = base::file::exists("./test");
        EXPECT_EQ(ret, false);
    }
}

TEST(FileOptTest, listDir)
{
    bool ret = base::file::createDir("./test");

    std::string filePath{ "./test/" };
    for (size_t i = 0; i < 10; ++i) {
        std::string tmp{ filePath };
        tmp.append(std::to_string(i)).append(".txt");
        FILE* fp = fopen(tmp.c_str(), "a+");
        fclose(fp);
    }

    std::vector<std::string> dirs;
    std::vector<std::string> files;

    base::file::listDir("./", dirs, files);
    EXPECT_EQ(dirs.empty(), false);
    EXPECT_EQ(files.empty(), false);
    for (auto& each : dirs) {
        std::cout << each << std::endl;
    }
    for (auto& each : files) {
        std::cout << each << std::endl;
    }

    dirs.clear();
    files.clear();
    base::file::listDir("./test", dirs, files);
    EXPECT_EQ(dirs.empty(), true);
    EXPECT_EQ(files.empty(), false);
    for (auto& each : files) {
        std::cout << each << std::endl;
    }

    dirs.clear();
    files.clear();
    base::file::listDir("./hello", dirs, files);
    EXPECT_EQ(dirs.empty(), true);
    EXPECT_EQ(files.empty(), true);

    base::file::removeDir("./test");
}

TEST(FileOptTest, removeFile)
{
    bool ret = base::file::createDir("./test");

    std::string filePath{ "./test/123.txt" };
    FILE* fp = fopen(filePath.c_str(), "a+");
    fclose(fp);

    ret = base::file::exists(filePath);
    EXPECT_EQ(ret, true);

    ret = base::file::removeFile(filePath);
    EXPECT_EQ(ret, true);
    
    ret = base::file::exists(filePath);
    EXPECT_EQ(ret, false);

    base::file::removeDir("./test");
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



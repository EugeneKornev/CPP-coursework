#include "IO.hpp"
#include <cstdio>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

TEST(I0Test, StringOpen) {
    std::string data = "abc";
    StringReader reader(data);
    ASSERT_TRUE(reader.isOpen());
    ASSERT_FALSE(reader.eof());
}

TEST(I0Test, StringClose) {
    std::string data = "abc";
    StringReader reader(data);
    reader.close();
    ASSERT_FALSE(reader.isOpen());
    ASSERT_THROW({
        char c;
        reader.read(c);
    }, 
    std::runtime_error);
}

TEST(I0Test, EOFTest) {
    std::string data = "ab";
    StringReader reader(data);
    char c;
    reader.read(c);
    reader.read(c);
    ASSERT_FALSE(reader.eof());
    ASSERT_THROW(reader.read(c), std::runtime_error);
    ASSERT_TRUE(reader.eof());
}

TEST(StringReaderTest, ReadChar) {
    std::string data = "Hello";
    StringReader reader(data);
    char c;
    reader.read(c);
    EXPECT_EQ('H', c);
}

TEST(StringReaderTest, ReadInt) {
    std::string data;
    int value = 169;
    data.append(reinterpret_cast<const char*>(&value), sizeof(value));
    StringReader reader(data);
    int out;
    reader.read(out);
    ASSERT_EQ(value, out);
}

TEST(StringReaderTest, ReadDouble) {
    std::string data;
    double value = 435.6543;
    data.append(reinterpret_cast<const char*>(&value), sizeof(value));
    StringReader reader(data);
    double out;
    reader.read(out);
    ASSERT_EQ(value, out);
}


TEST(StringReaderTest, ReadString) {
    std::string data;
    std::string original = "Hello World";
    int len = original.size();
    data.append(reinterpret_cast<const char*>(&len), sizeof(len));
    data.append(original);
    StringReader reader(data);
    std::string out;
    reader.read(out);
    EXPECT_EQ(original, out);
}

TEST(StringWriterTest, WriteChar) {
    std::string out;
    StringWriter writer(out);
    writer.write('A');
    writer.write('B');
    writer.close();
    EXPECT_EQ(std::string("AB"), out);
}

TEST(StringReaderWriterTest, ReadWriteInt) {
    std::string out;
    StringWriter writer(out);
    int value = 42;
    writer.write(value);
    writer.close();
    StringReader reader(out);
    int result = 0;
    reader.read(result);
    EXPECT_EQ(42, result);
}

TEST(StringWriterTest, ReadWriteFloat) {
    std::string out;
    StringWriter writer(out);
    float value = 1.234f;
    writer.write(value);
    writer.close();
    StringReader reader(out);
    float result = 0.0f;
    reader.read(result);
    EXPECT_FLOAT_EQ(1.234f, result);
}

TEST(FileReaderWriterTest, WriteRead) {
    FILE* tmpf = std::tmpfile();
    FileWriter writer(tmpf);
    writer.write(1584);
    writer.write(456.78f);
    writer.write("876");
    std::rewind(tmpf);
    FileReader reader(tmpf);
    int i;
    float f;
    std::string str;
    reader.read(i);
    reader.read(f);
    reader.read(str);
    ASSERT_EQ(i, 1584);
    ASSERT_EQ(f, 456.78f);
    ASSERT_EQ(str, "876");
}

TEST(FileReaderWriterTest, EOFTest) {
    FILE* tmpf = std::tmpfile();
    FileWriter writer(tmpf);
    writer.write(3462);
    writer.close();
    std::rewind(tmpf);
    FileReader reader(tmpf);
    int i;
    reader.read(i);
    ASSERT_FALSE(reader.eof());
    reader.read(i);
    ASSERT_TRUE(reader.eof());
}

int main() {
	testing::InitGoogleTest();
	auto s = RUN_ALL_TESTS();
}
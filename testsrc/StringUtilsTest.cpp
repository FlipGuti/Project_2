#include <gtest/gtest.h>
#include "StringUtils.h"

// -------------------------------------------------------------------------------------------------
// stringUtils.cpp
// Project 1 ( Source File )
// Felipe Gutierrez
// ECS 034
// -------------------------------------------------------------------------------------------------

// ( 1 ) Slice
TEST(StringUtilsTest, SliceTest) 
{
    std::string phrase = "hello world!";
    std::string test = StringUtils::Slice(phrase, 6);
    ASSERT_EQ(test, "world!");
}

// ( 2 ) Capitalize
TEST(StringUtilsTest, Capitalize) 
{
    std::string phrase = "hello world!";
    std::string test = StringUtils::Capitalize(phrase);
    ASSERT_EQ(test, "HELLO WORLD" );

    std::string phrase2 = "HELLO WORLD!";
    std::string test2 = StringUtils::Capitalize(phrase2);
    ASSERT_EQ(test2, "HELLO WORLD" );

    std::string phrase3 = "123!";
    std::string test3 = StringUtils::Capitalize(phrase3);
    ASSERT_EQ(test3, "123!" );
}

// ( 3a ) Upper
TEST(StringUtilsTest, Upper) 
{
    std::string Uphrase = "hello World!";
    std::string Unew_phrase = StringUtils::Upper(Uphrase);
    ASSERT_EQ(Unew_phrase, "World!");

    std::string Uphrase2 = "Hello World!";
    std::string Utest2 = StringUtils::Upper(Uphrase2);
    ASSERT_EQ(Utest2, "Hello World!");
}

// ( 3b ) Lower
TEST(StringUtilsTest, Lower) 
{
    std::string Lphrase = "hello World!";
    std::string Lnew_phrase = StringUtils::Lower(Lphrase);
    ASSERT_EQ(Lnew_phrase, "hello");
}

// ( 4a ) Left Strip
TEST(StringUtilsTest, LStrip) 
{
    std::string Lphrase = "Hello There World";
    std::string Lnew_phrase = StringUtils::LStrip(Lphrase);
    ASSERT_EQ(Lnew_phrase,"HelloThere World");
}

// ( 4b ) Right Strip
TEST(StringUtilsTest, RStrip) 
{
    std::string Rphrase = "Hello There World";
    std::string Rnew_phrase = StringUtils::RStrip(Rphrase);
    ASSERT_EQ(Rnew_phrase,"Hello ThereWorld");
}

// ( 4c ) Full Strip
TEST(StringUtilsTest, Strip) 
{
    std::string Sphrase = "Hello There World";
    std::string Snew_phrase = StringUtils::Strip(Sphrase);
    ASSERT_EQ(Snew_phrase, "HelloThereWorld");
}

// ( 5a ) Center Justified
TEST(StringUtilsTest, Center)
{
    std::string Cphrase = "Hello World";
    std::string Ctest = StringUtils::Center(Cphrase, 16);
    ASSERT_EQ( Ctest, "  Hello  World  ");
}

// ( 5b ) Left Justified
TEST(StringUtilsTest, LJust) 
{
    std::string Lphrase = "Hello World";
    std::string Ltest = StringUtils::LJust(Lphrase, 15);
    ASSERT_EQ( Ltest, "    Hello World");
}

// ( 5c ) Right Justified
TEST(StringUtilsTest, RJust)
{
    std::string Rphrase = "Hello World";
    std::string Rtest = StringUtils::LJust(Rphrase, 15);
    ASSERT_EQ( Rtest, "Hello World    ");
}

// ( 6 ) Replace
TEST(StringUtilsTest, Replace) 
{
    std::string phrase = "Hello World!";
    std::string new_phrase = StringUtils::Replace(phrase, "World", "Earth");
    ASSERT_EQ( new_phrase, "Hello Earth!");
}

// ( 7 ) Split
TEST(StringUtilsTest, Split) 
{
    std::string phrase = "Hello-World";
    std::vector<std::string> string_vector = StringUtils::Split(phrase, "-");
    ASSERT_EQ(string_vector[0], "Hello");
    ASSERT_EQ(string_vector[1], "World");
}

// ( 8 ) Join
TEST(StringUtilsTest, Join) 
{
    std::string word = "Hello";
    std::vector<std::string> phrase = {"World"," ","!"};
    std::string test= StringUtils::Join( word, phrase );
    ASSERT_EQ( test, "Hello World!" );
}

// ( 9 ) Expand Tabs
TEST(StringUtilsTest, ExpandTabs) 
{
    std::string phrase = "Hello\tWorld";
    std::string test = StringUtils::ExpandTabs(phrase, 8);
    ASSERT_EQ( test, "Hello     World" );
}

// ( 10 ) Levenshtein Distance
TEST(StringUtilsTest, EditDistance) 
{
    std::string Lphrase = "world!";
    std::string Rphrase = "Hello";
    int test = StringUtils::EditDistance( Lphrase, Rphrase );
    ASSERT_EQ(test, 5);
}
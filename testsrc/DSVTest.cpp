// DSVTest.cpp
// ECS 034
// Project 2
// Felipe Gutierrez
// Kavya Sasikumar 
//----------------------------------------------------------------------------
// C++ Headers
#include <memory>
#include <sstream>
#include <vector>
// User Defined Headers
#include <gtest/gtest.h>
#include "DSVReader.h"
#include "DSVWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"
//----------------------------------------------------------------------------

TEST(DSVReaderTest, ReadSimpleCSV) {
    std::string testData = "Name,Age,Occupation\nJohn Doe,30,Engineer";
    auto dataSource = std::make_shared<CStringDataSource>(testData);
    DSVReader reader(dataSource, ',');
    std::vector<std::string> row;
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "Name");
    EXPECT_EQ(row[1], "Age");
    EXPECT_EQ(row[2], "Occupation");
    row.clear();
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row[0], "John Doe");
    EXPECT_EQ(row[1], "30");
    EXPECT_EQ(row[2], "Engineer");
}

TEST(DSVReaderTest, HandleQuotedFields) {
    std::string testData = "\"Name\",\"Age\",\"Occupation\"\n\"John, Doe\",\"30\",\"Engineer\"";
    auto dataSource = std::make_shared<CStringDataSource>(testData);
    DSVReader reader(dataSource, ',');
    std::vector<std::string> row;
    reader.ReadRow(row); // Skip header
    row.clear();
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row[0], "John, Doe"); // Check that comma within quotes is handled
}

TEST(DSVWriterTest, WriteSimpleCSV) {
    auto dataSink = std::make_shared<CStringDataSink>();
    DSVWriter writer(dataSink, ',');
    std::vector<std::string> row = {"Name", "Age", "Occupation"};
    EXPECT_TRUE(writer.WriteRow(row));
    EXPECT_EQ(dataSink->String(), "Name,Age,Occupation\n");
}

TEST(DSVWriterTest, QuoteFieldsIfNeeded) {
    auto dataSink = std::make_shared<CStringDataSink>();
    DSVWriter writer(dataSink, ',', true); // Force quoting
    std::vector<std::string> row = {"John Doe", "30", "Engineer"};
    writer.WriteRow(row);
    EXPECT_EQ(dataSink->String(), "\"John Doe\",\"30\",\"Engineer\"\n");
}

TEST(DSVReaderTest, HandleEmptyFields) {
    std::string testData = "Title,Release Year,Director\n,2020,";
    auto dataSource = std::make_shared<CStringDataSource>(testData);
    DSVReader reader(dataSource, ',');
    std::vector<std::string> row;
    reader.ReadRow(row); // Skip header
    row.clear();
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_TRUE(row[0].empty()); // First field is empty
    EXPECT_EQ(row[1], "2020");
    EXPECT_TRUE(row[2].empty()); // Last field is empty
}

TEST(DSVReaderTest, HandleNewLinesWithinQuotedFields) {
    std::string testData = "\"Title\",\"Description\"\n\"Example\",\"This is a\ndescription with newline\"";
    auto dataSource = std::make_shared<CStringDataSource>(testData);
    DSVReader reader(dataSource, ',');
    std::vector<std::string> row;
    reader.ReadRow(row); // Skip header
    row.clear();
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row[0], "Example");
    EXPECT_EQ(row[1], "This is a\ndescription with newline");
}

TEST(DSVWriterTest, EscapeQuotesInField) {
    auto dataSink = std::make_shared<CStringDataSink>();
    DSVWriter writer(dataSink, ',');
    std::vector<std::string> row = {"\"Quoted\"", "2020", "Some \"Director\""};
    writer.WriteRow(row);
    EXPECT_EQ(dataSink->String(), "\"\"\"Quoted\"\"\",\"2020\",\"Some \"\"Director\"\"\"\n");
}

TEST(DSVWriterTest, HandleNewLineInField) {
    auto dataSink = std::make_shared<CStringDataSink>();
    DSVWriter writer(dataSink, ',');
    std::vector<std::string> row = {"New\nLine", "2021", "Director"};
    writer.WriteRow(row);
    EXPECT_EQ(dataSink->String(), "\"New\nLine\",\"2021\",\"Director\"\n");
}

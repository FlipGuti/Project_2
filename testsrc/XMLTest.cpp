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
#include "XMLReader.h"
#include "XMLWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"
//----------------------------------------------------------------------------

TEST(XMLReaderTest, ReadSimpleElement) {
    std::string xmlData = "<Person><Name>John Doe</Name><Age>30</Age></Person>";
    auto dataSource = std::make_shared<StringDataSource>(xmlData);
    XMLReader reader(dataSource);
    SXMLEntity entity;
    EXPECT_TRUE(reader.ReadEntity(entity)); // Assuming ReadEntity method populates entity and returns true if successful
    EXPECT_EQ(entity.DNameData, "Person");
    // Add more checks as necessary for attributes, text content, etc.
}

TEST(XMLWriterTest, WriteSimpleElement) {
    auto dataSink = std::make_shared<StringDataSink>();
    XMLWriter writer(dataSink);
    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::StartElement;
    entity.DNameData = "Person";
    // Populate entity as necessary
    EXPECT_TRUE(writer.WriteEntity(entity));
    // Assuming WriteEntity correctly writes the XML element to the sink
    EXPECT_TRUE(dataSink->String().find("<Person>") != std::string::npos);
    // Add more checks for attributes, text content, etc.
}

TEST(XMLReaderTest, ReadAttributes) {
    std::string xmlData = "<Movie title=\"Example Movie\" year=\"2020\"></Movie>";
    auto dataSource = std::make_shared<StringDataSource>(xmlData);
    XMLReader reader(dataSource);
    SXMLEntity entity;
    EXPECT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DNameData, "Movie");
    ASSERT_FALSE(entity.DAttributes.empty());
    EXPECT_EQ(entity.DAttributes["title"], "Example Movie");
    EXPECT_EQ(entity.DAttributes["year"], "2020");
}

TEST(XMLReaderTest, ReadNestedElements) {
    std::string xmlData = "<Library><Book><Title>Example Title</Title><Author>John Doe</Author></Book></Library>";
    auto dataSource = std::make_shared<StringDataSource>(xmlData);
    XMLReader reader(dataSource);
    SXMLEntity entity;
    EXPECT_TRUE(reader.ReadEntity(entity)); // Reads Library start tag
    EXPECT_TRUE(reader.ReadEntity(entity)); // Reads Book start tag
    EXPECT_TRUE(reader.ReadEntity(entity)); // Reads Title start tag
    // Continue through the XML structure as needed
}

TEST(XMLWriterTest, WriteAttributes) {
    auto dataSink = std::make_shared<StringDataSink>();
    XMLWriter writer(dataSink);
    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::CompleteElement;
    entity.DNameData = "Movie";
    entity.DAttributes.push_back(std::make_pair("title", "Example Movie"));
    entity.DAttributes.push_back(std::make_pair("year", "2020"));
    writer.WriteEntity(entity);
    EXPECT_TRUE(dataSink->String().find("<Movie title=\"Example Movie\" year=\"2020\"/>") != std::string::npos);
}
/*
TEST(XMLWriterTest, WriteNestedElements) {
    auto dataSink = std::make_shared<StringDataSink>();
    XMLWriter writer(dataSink);

    // Write Library start tag
    SXMLEntity libraryEntity;
    libraryEntity.DType = SXMLEntity::EType::StartElement;
    libraryEntity.DNameData = "Library";
    writer.WriteEntity(libraryEntity);

    // Write Book start tag
    SXMLEntity bookEntity;
    bookEntity.DType = SXMLEntity::EType::StartElement;
    bookEntity.DNameData = "Book";
    writer.WriteEntity(bookEntity);

    // Write Book end tag
    bookEntity.DType = SXMLEntity::EType::EndElement;
    writer.WriteEntity(bookEntity);

    // Write Library end tag
    libraryEntity.DType = SXMLEntity::EType::EndElement;
    writer.WriteEntity(libraryEntity);
*/
   

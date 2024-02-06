// XMLWriter.cpp
// Project 2
// Felipe Gutierrez
// ECS 034
//----------------------------------------------------------------------------
// C++ Headers
#include <fstream> 
#include <iostream>
#include <sstream>
#include <memory>
#include <string>
// User Defined Headers
#include "XMLWriter.h"
#include "DataSink.h"
//----------------------------------------------------------------------------

struct CXMLWriter::SImplementation 
{
    std::shared_ptr<CDataSink> sink;
    SXMLEntity entity;
    std::vector<std::string> OpenedElements;

    // Constructor
    SImplementation(std::shared_ptr<CDataSink> sink)
        : sink(std::move(sink)) {}
};

CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : DImplementation(new SImplementation(std::move(sink))) {}

CXMLWriter::~CXMLWriter() = default;


//----------------------------------------------------------------------------
// Writes out the entity to the output stream
bool CXMLWriter::WriteEntity(const SXMLEntity &entity)
{
    // Helper lambda to append string to vector<char>
    std::vector<char> data;
    auto appendToData = [&data](const std::string& str) 
    {
        data.insert(data.end(), str.begin(), str.end());
    };

    // Start element
    if (entity.DType == SXMLEntity::EType::StartElement) 
    {
        appendToData("<" + entity.DNameData);
        for (const auto &attr : entity.DAttributes) 
        { appendToData(" " + attr.first + "=\"" + attr.second + "\""); }
        appendToData(">");
        DImplementation->OpenedElements.push_back(entity.DNameData);
    }

    // End element
    else if (entity.DType == SXMLEntity::EType::EndElement) 
    {
        if (!DImplementation->OpenedElements.empty() && DImplementation->OpenedElements.back() == entity.DNameData)
        {
            DImplementation->OpenedElements.pop_back();
        }
        appendToData("</" + entity.DNameData + ">");
    }

    // Complete element, write opening + closing tag
    else if (entity.DType == SXMLEntity::EType::CompleteElement) 
    {
        appendToData("<" + entity.DNameData);
        for (const auto &attr : entity.DAttributes) 
        {
            appendToData(" " + attr.first + "=\"" + attr.second + "\"");
        }
        appendToData("/>");
    }

    // Write the vector<char> to the sink
    DImplementation->sink->Write(data);
    return true;
}


//----------------------------------------------------------------------------
// Outputs all end elements for those that have been started
bool CXMLWriter::Flush() 
{
    // Iterate in reverse to close elements in the correct order
    while (!DImplementation->OpenedElements.empty()) 
    {
        // Retrieve most recently opened element
        std::string elementName = DImplementation->OpenedElements.back();
        DImplementation->OpenedElements.pop_back();

        // Final closing tag
        std::vector<char> data;
        std::string closingTag = "</" + elementName + ">";
        data.insert(data.end(), closingTag.begin(), closingTag.end());
        DImplementation->sink->Write(data);
    }

    return true;
}
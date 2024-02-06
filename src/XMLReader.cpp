// XMLReader.cpp 
// Project 2
// ECS 034
//----------------------------------------------------------------------------

// C++ Headers
#include <expat.h>
#include <memory>
#include <iostream>
#include <cstring>

// User Defined Headers
#include "XMLReader.h"

//----------------------------------------------------------------------------
// pimple

struct CXMLReader::SImplementation 
{
    std::shared_ptr<CDataSource> DataSource;
    bool skipcdata;
    XML_Parser parser = nullptr;
    SXMLEntity entity;
    // Constructor
    SImplementation(std::shared_ptr<CDataSource> src)
        : DataSource(std::move(src)) {}
};

CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src)
    : DImplementation(new SImplementation(std::move(src))) {}

CXMLReader::~CXMLReader() = default;

//----------------------------------------------------------------------------
// Returns true if the entity is successfully read if skipcdata is true only element type entities will be returned
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata = false)
{
    DImplementation->skipcdata = skipcdata;

    // Set handlers to the parser
    XML_SetElementHandler( DImplementation->parser,
        [](void *userData, const XML_Char *name, const XML_Char **attrs) {
            auto impl = static_cast<SImplementation*>(userData);
            impl->entity.DNameData = name;
            impl->entity.DType = SXMLEntity::EType::StartElement;

            // Clear existing attributes
            impl->entity.DAttributes.clear();

            // Load new attributes
            for (int i = 0; attrs[i]; i += 2) 
            {
                impl->entity.SetAttribute(attrs[i], attrs[i + 1]);
            }
        },
        [](void *userData, const XML_Char *name) {
            auto impl = static_cast<SImplementation*>(userData);
            impl->entity.DNameData = name;
            impl->entity.DType = SXMLEntity::EType::EndElement;
        }
    );

    if (!skipcdata) 
    {
        XML_SetCharacterDataHandler(DImplementation->parser,
            [](void *userData, const XML_Char *s, int len) 
            {
                auto impl = static_cast<SImplementation*>(userData);
                // Append CDATA to the last entity's data, assuming CDATA appears within elements
                if (impl->entity.DType == SXMLEntity::EType::StartElement) 
                {
                    impl->entity.DNameData.append(s, len);
                }
            }
        );
    }

    // Temporary storage for reading data from CDataSource
    std::vector<char> buffer(1024);

    while (!DImplementation->DataSource->End()) 
    {
        buffer.clear(); // Ensure the buffer is empty before reading new data
        bool success = DImplementation->DataSource->Read(buffer, buffer.size());

        // After calling Read, check if the operation was successful
        if (!success) 
        {
            std::cerr << "Error reading data source." << std::endl;
            return false;
        }

        // Assuming buffer now contains the data read, and we should parse what's currently in the buffer.
        if (!buffer.empty()) 
        {
            if (XML_Parse(DImplementation->parser, buffer.data(), buffer.size(), XML_FALSE) == XML_STATUS_ERROR) 
            {
                std::cerr << "Error: " << XML_ErrorString(XML_GetErrorCode(DImplementation->parser)) << std::endl;
                return false;
            }

            // Update the output entity with the latest parsed entity
            entity = DImplementation->entity;
            return true; // Indicate that an entity was successfully read.
        }
    }

    return false; // Return false if no more data to read or parsing completed
}


//----------------------------------------------------------------------------
// Returns true if all entities have been read from the XML
bool CXMLReader::End() const 
{
    return DImplementation->DataSource->End();
}
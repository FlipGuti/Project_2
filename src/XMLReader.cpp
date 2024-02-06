#include "XMLReader.h"
#include <expat.h>
#include <memory>
#include <iostream>
#include <cstring>

// Fully define SImplementation within the .cpp file
class CXMLReader::SImplementation {
public:
    std::shared_ptr<CDataSource> DataSource;
    XML_Parser Parser = nullptr;
    bool EntityReady = false;
    SXMLEntity CurrentEntity;

    SImplementation(std::shared_ptr<CDataSource> src) : DataSource(src) {
        Parser = XML_ParserCreate(nullptr); // Use nullptr instead of NULL for modern C++
        if (!Parser) {
            throw std::runtime_error("Failed to create XML parser");
        }

        XML_SetUserData(Parser, this);
        XML_SetElementHandler(Parser, StartElementHandler, EndElementHandler);
        XML_SetCharacterDataHandler(Parser, CharacterDataHandler);
    }

    ~SImplementation() {
        if (Parser) {
            XML_ParserFree(Parser);
        }
    }

    static void XMLCALL StartElementHandler(void *userData, const XML_Char *name, const XML_Char **atts) {
        auto *impl = static_cast<SImplementation*>(userData);
        impl->CurrentEntity.DType = SXMLEntity::EType::StartElement;
        impl->CurrentEntity.DNameData = name;
        impl->CurrentEntity.DAttributes.clear();

        for (const XML_Char **attr = atts; attr && *attr; attr += 2) {
            impl->CurrentEntity.DAttributes.emplace_back(std::make_pair(std::string(attr[0]), std::string(attr[1])));
        }

        impl->EntityReady = true;
    }

    static void XMLCALL EndElementHandler(void *userData, const XML_Char *name) {
        auto *impl = static_cast<SImplementation*>(userData);
        impl->CurrentEntity.DType = SXMLEntity::EType::EndElement;
        impl->CurrentEntity.DNameData = name;
        impl->EntityReady = true;
    }

    static void XMLCALL CharacterDataHandler(void *userData, const XML_Char *s, int len) {
        // Implement as needed for handling text content within elements
    }
};

CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src)
    : DImplementation(std::make_unique<SImplementation>(std::move(src))) {}

CXMLReader::~CXMLReader() = default;

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipCdata) {
    std::vector<char> buffer(4096); // Use a std::vector<char> for the buffer

    while (!DImplementation->DataSource->End() && !DImplementation->EntityReady) {
        // Pass the buffer vector directly, adjusting the Read call as necessary
        size_t bytesRead = DImplementation->DataSource->Read(buffer, buffer.size());
        if (bytesRead == 0) {
            return false; // No more data or read error
        }

        // Parse the buffer. Note the conversion to const char* and using bytesRead for the length
        if (XML_Parse(DImplementation->Parser, buffer.data(), static_cast<int>(bytesRead), XML_FALSE) == XML_STATUS_ERROR) {
            std::cerr << "Parse error at line " << XML_GetCurrentLineNumber(DImplementation->Parser);
            std::cerr << ": " << XML_ErrorString(XML_GetErrorCode(DImplementation->Parser)) << std::endl;
            return false;
        }
    }

    if (DImplementation->EntityReady) {
        entity = std::move(DImplementation->CurrentEntity); // Transfer the parsed entity
        DImplementation->EntityReady = false; // Reset for the next entity
        return true;
    }
    
    // Check if the end of data is reached
    if (DImplementation->DataSource->End()) {
        return false;
    }

    return true; // Return true when an entity is ready
}

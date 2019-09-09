//
//  flexjsonbridge.h
//  FlexBufferVsJson
//
//  Created by Pranay Kumar on 26/07/19.
//  Copyright © 2019 Pranay Kumar. All rights reserved.
//

#ifndef flexjsonbridge_h
#define flexjsonbridge_h

#include <fstream>

#include "config/flexitconfig.h"
#include "flexmemmap/FlexMemoryMap.h"

#include "rapidjsonwrapper/jsonhandler/json_util.hpp"
#include "rapidjsonwrapper/jsonhandler/json_value.hpp"

namespace flexit {

	namespace utils
	{
		inline flexbuffers::Reference CreateFlexBufferReference(const uint8_t *buffer, size_t size) {
			return flexbuffers::Reference(nullptr, 0, 0);
		}
	}
    
    static bool JsonToFlexBuffer(const std::string&  input,
                                flexbuffers::Builder& flexbuilder)
    {
        return jsonhandler::JsonDecodeToFlexBuffer(input, flexbuilder);
    }
    
    static bool FlexBufferToJson(flexbuffers::Builder& flexbuilder,
                                       std::string &jsonEncodedString,
                                       bool strings_quoted = true,
                                       bool keys_quoted = true)
    {
        bool lStatus = true;
        try
        {
            flexbuffers::GetRoot(flexbuilder.GetBuffer()).ToString(strings_quoted, keys_quoted, jsonEncodedString);
        }
        catch(...)
        {
            // Exception occured
            lStatus = false;
        }
        return lStatus;
    }
    
    static bool FlushFlexbufferToFile(const std::string& filePath, flexbuffers::Builder& builder)
    {
        bool lStatus = false;
        if(builder.GetSize() >0)
        {
            std::string buffer;
            //auto bufferpointer = reinterpret_cast<const char *>(builder.GetBufferPointer());
            buffer.assign(builder.GetBuffer().data(), builder.GetBuffer().data() + builder.GetSize());
            
            std::ofstream fOutStream;
            
#if FLEXBRIDGE_OS_WINDOWS
            std::wstring filePathNative(utils::Utf8StringToWstring(filePath));
            
            fOutStream.open(filePathNative, std::ios::out | std::ios::binary);
#else
            fOutStream.open(filePath, std::ios::out | std::ios::binary);
#endif
            if(fOutStream.is_open())
            {
                lStatus = true;
                fOutStream << buffer;
                fOutStream.close();
            }
        }
        return lStatus;
    }
    
    static bool GetFlexbufferReferenceFromFile(const std::string& filePath, flexbuffers::Reference& reference)
    {
        bool lStatus = false;

        //auto bufferpointer = reinterpret_cast<const char *>(builder.GetBufferPointer());
        
        std::ifstream fStream;
        
#if FLEXBRIDGE_OS_WINDOWS
        std::wstring filePathNative(utils::Utf8StringToWstring(filePath));
        fStream.open(filePathNative, std::ios::in | std::ios::binary);
#else
        fStream.open(filePath, std::ifstream::ate | std::ifstream::binary);
#endif
        if(fStream.is_open())
        {
            std::streampos fileSize = fStream.tellg();
            std::vector<char> buffer(fileSize);
            //buffer = (new std::vector<char>(fileSize));
            std::vector<uint8_t>  bufferPtr(fileSize);

            fStream.seekg (0, std::ios::beg);
            lStatus = true;
            fStream.read(buffer.data(), fileSize);
            fStream.close();
            
            memcpy((void*)(bufferPtr.data()), (void*)(buffer.data()), sizeof(fileSize));
            //bufferPtr = static_cast<std::vector<uint8_t>>(buffer);
            reference = (flexbuffers::GetRoot(bufferPtr.data(), fileSize));
        }
        return lStatus;
    }
    
    static bool GetMemMappedFlexBufferReferenceFromFile(const std::string& filePath, flexbuffers::Reference& reference)
    {
        bool lStatus = false;
        
        //auto bufferpointer = reinterpret_cast<const char *>(builder.GetBufferPointer());
        
        FlexMemoryMap fmap(filePath);
        
        if(fmap.GetMemMappedData())
        {
            std::vector<uint8_t>  bufferPtr(fmap.GetMemMappedDataLength());
            memcpy((void*)(bufferPtr.data()), fmap.GetMemMappedData(), fmap.GetMemMappedDataLength());
            reference = flexbuffers::GetRoot(bufferPtr.data(), bufferPtr.size());
            lStatus = true;
        }
        return lStatus;
    }

} // end namespace flexit

#endif /* flexjsonbridge_h */

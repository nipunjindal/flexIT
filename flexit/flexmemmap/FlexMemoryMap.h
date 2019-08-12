//
//  FlexMemoryMap.hpp
//  FlexBufferBridge
//
//  Created by Pranay Kumar on 11/08/19.
//  Copyright © 2019 Pranay Kumar. All rights reserved.
//

#ifndef FlexMemoryMap_h
#define FlexMemoryMap_h

#include <iostream>

namespace flexit {
    
    enum class MemoryMapMode : std::uint32_t {
        kModeRead = 0,
        kModeReadWrite
    };
        
    class FlexMemoryMap {
            
        private:
            
            int32_t mErrorOccured;
            void* mMappedDataPtr;
            size_t mMappedDataLength;
            
            void GetFlagsAndProtectionMode(MemoryMapMode mMode, int* pFlags, int* pProtectionMode);
            
        public:
            
            FlexMemoryMap(const std::string& inPathName, MemoryMapMode mode = MemoryMapMode::kModeRead);
            
            ~FlexMemoryMap();
            
            const int GetError();
            
            void* GetMemMappedData();
            
            const size_t GetMemMappedDataLength();
        };
        
        using FlexMemoryMapPtr = std::unique_ptr<FlexMemoryMap>;
    }
    
#endif /* FlexMemoryMap_hpp */

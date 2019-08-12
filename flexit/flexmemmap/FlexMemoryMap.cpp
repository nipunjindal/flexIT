//
//  FlexMemoryMap.cpp
//  FlexBufferBridge
//
//  Created by Pranay Kumar on 11/08/19.
//  Copyright © 2019 Pranay Kumar. All rights reserved.
//

#include "config/flexitconfig.h"
#include "FlexMemoryMap.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#if FLEXBRIDGE_OS_WINDOWS
#include <windows.h>
#include <memoryapi.h>
#else
#include <unistd.h>
#include <sys/mman.h>
#endif // UXTECH_OS_COCOA || UXTECH_OS_IOS

void  flexit::FlexMemoryMap::GetFlagsAndProtectionMode(MemoryMapMode pMode, int* pOutFileFlags, int* pOutProtectionMode)
{
    if (pMode == MemoryMapMode::kModeRead)
    {
#if FLEXBRIDGE_OS_IOSX
        *pOutFileFlags = O_RDONLY;
        *pOutProtectionMode = PROT_READ;
#else
        *pOutFileFlags = PAGE_READONLY;
        *pOutProtectionMode = FILE_MAP_READ;
#endif
        
    }
    else
    {
#if FLEXBRIDGE_OS_IOSX
        *pOutFileFlags = O_RDWR;
        *pOutProtectionMode = PROT_WRITE;
#else
        *pOutFileFlags = PAGE_READWRITE;
        *pOutProtectionMode = FILE_MAP_WRITE;
#endif
    }
}

flexit::FlexMemoryMap::FlexMemoryMap(const std::string& inPathName, MemoryMapMode mode):
mErrorOccured(0),
mMappedDataPtr(nullptr),
mMappedDataLength(0)
{
    // Return safe values on error.
    if(inPathName.empty())
    {
        return;
    }
    
    int fileFlags, protection;
    
    GetFlagsAndProtectionMode(mode, &fileFlags, &protection);
#if FLEXBRIDGE_OS_IOSX
    struct stat statInfo;
    
    int fileDescriptor = 0;
    
    // Open the file.
    fileDescriptor = open(inPathName.c_str(), fileFlags, 0666);
    if (fileDescriptor < 0)
    {
        mErrorOccured = errno;
    }
    else
    {
        // We now know the file exists. Retrieve the file size.
        if (fstat(fileDescriptor, &statInfo) != 0)
        {
            mErrorOccured = errno;
        }
        else
        {
            // Map the file into a read-only memory region.
            mMappedDataPtr = mmap(NULL, (size_t)statInfo.st_size, protection, MAP_SHARED, fileDescriptor, 0);
            if (mMappedDataPtr == MAP_FAILED || mMappedDataPtr == nullptr)
            {
                mErrorOccured = errno;
            }
            else
            {
                // On success, return the size of the mapped file.
                mMappedDataLength = (size_t)(statInfo.st_size);
            }
        }
        
        // Now close the file. The kernel doesn’t use our file descriptor.
        close(fileDescriptor);
    }
#else
    HANDLE lFileHandle = CreateFile((utils::Utf8StringToWstring(inPathName)).c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
    if (lFileHandle != NULL && lFileHandle != INVALID_HANDLE_VALUE)
    {
        DWORD lFileSize = 0;
        lFileSize = ::GetFileSize(lFileHandle, NULL);
        HANDLE lMemMapHandle = CreateFileMappingW(lFileHandle, NULL, protection, 0, 0, NULL);
        // Map the file into a read-only memory region.
        
        if (lMemMapHandle != nullptr)
        {
            mMappedDataPtr = MapViewOfFile(lMemMapHandle, fileFlags, 0, 0, 0);
            mMappedDataLength = lFileSize;
        }
        else
        {
            CloseHandle(lMemMapHandle);
            DWORD err = GetLastError();
            mErrorOccured = err;
        }
        
        CloseHandle(lFileHandle);
        if (mMappedDataPtr == nullptr && mErrorOccured == 0)
        {
            DWORD err = GetLastError();
            mErrorOccured = err;
        }
    }
    
#endif
}

const int32_t flexit::FlexMemoryMap::GetError()
{
    return mErrorOccured;
}

void* flexit::FlexMemoryMap::GetMemMappedData()
{
    return mMappedDataPtr;
}

const size_t flexit::FlexMemoryMap::GetMemMappedDataLength()
{
    return mMappedDataLength;
}

flexit::FlexMemoryMap::~FlexMemoryMap()
{
#if FLEXBRIDGE_OS_IOSX
    if (mMappedDataPtr && mMappedDataPtr != MAP_FAILED)
    {
        munmap(mMappedDataPtr, mMappedDataLength);
    }
#else
    if (mMappedDataPtr)
    {
        UnmapViewOfFile(mMappedDataPtr);
        mMappedDataPtr = nullptr;
    }
#endif
}


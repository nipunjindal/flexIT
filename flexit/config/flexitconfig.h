//
//  flexitconfig.h
//  FlexBufferBridge
//
//  Created by Pranay Kumar on 11/08/19.
//  Copyright © 2019 Pranay Kumar. All rights reserved.
//

#ifndef flexitconfig_h
#define flexitconfig_h

// Standard includes
#if defined(_WIN32)
#define FLEXBRIDGE_OS_WINDOWS 1
#else
#define FLEXBRIDGE_OS_WINDOWS 0
#endif

// Standard includes
#if defined(__APPLE__)
#define FLEXBRIDGE_OS_IOSX 1
#else
#define FLEXBRIDGE_OS_IOSX 0
#endif

#endif /* flexitconfig_h */

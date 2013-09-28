#pragma once

#define TXC_STRINGIFY(x)	 #x
#define TXC_TO_STRING(x)	 TXC_STRINGIFY(x)

#define TXC_MAJOR_VERSION    2
#define TXC_MINOR_VERSION    02
#define TXC_REVISION         0
#define TXC_BUILD_NUMBER     0

#define TXC_VERSION_STRING   TXC_TO_STRING(TXC_MAJOR_VERSION) "." \
                             TXC_TO_STRING(TXC_MINOR_VERSION) \
                             " Stable"

#define TXC_VERSION_SEQUENCE TXC_MAJOR_VERSION, \
                             TXC_MINOR_VERSION, \
                             TXC_REVISION,		\
                             TXC_BUILD_NUMBER

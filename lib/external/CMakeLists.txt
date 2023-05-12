add_library(cpp-base64
	cpp-base64/base64.cpp
)

# note that we're little-endian
add_definitions(-D__BYTE_ORDER=99 -D__LITTLE_ENDIAN=99)
add_library(hash-library-sha1
	hash-library/sha1.cpp
)

add_definitions(-DARGUMENTUM_BUILD_STATIC_LIBS=ON)
add_subdirectory(argumentum)

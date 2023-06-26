# Module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/modules)

# Depend packages
if(NOT ZLIB_FOUND)
  find_package(ZLIB)
endif()
if(NOT Protobuf_FOUND AND NOT PROTOBUF_FOUND)
  find_package(Protobuf CONFIG)
endif()
if(NOT OPENSSL_FOUND)
  find_package(OpenSSL)
endif()
if(NOT c-ares_FOUND)
  find_package(c-ares)
endif()



# Targets
include(${CMAKE_CURRENT_LIST_DIR}/gRPCTargets.cmake)

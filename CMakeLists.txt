set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_library(lp_cc_core INTERFACE)
add_library(lp::cc_core ALIAS lp_cc_core)

target_include_directories(lp_cc_core INTERFACE ${LIB_DIR}/include)

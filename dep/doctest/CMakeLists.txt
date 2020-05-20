add_library(doctest INTERFACE)
target_include_directories(doctest INTERFACE latest)
target_compile_features(doctest INTERFACE cxx_std_11)

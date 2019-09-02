load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
   name = "CGAL",
   # These options help CMake to find prebuilt OpenBLAS, which will be copied into
   # $EXT_BUILD_DEPS/openblas by the cmake_external script
   #cache_entries = {
   #    "BLA_VENDOR": "OpenBLAS",
   #    "BLAS_LIBRARIES": "$EXT_BUILD_DEPS/openblas/lib/libopenblas.a",
   #},
   headers_only = True,
   lib_source = "@CGAL//:all",
   # Dependency on other cmake_external rule; can also depend on cc_import, cc_library rules
)
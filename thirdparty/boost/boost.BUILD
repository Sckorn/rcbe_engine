load(":boost.bzl", "boost_library")

boost_library(
    name = "context",
    visibility = ["//visibility:public"],
    deps = [
        ":assert",
        ":config",
        ":cstdint",
        ":detail",
        ":intrusive_ptr",
    ],
)

boost_library(
    name = "pool",
)

boost_library(
    name = "algorithm",
    deps = [
        ":function",
        ":iterator",
        ":range",
    ],
)

boost_library(
    name = "align",
)

boost_library(
    name = "any",
    deps = [
        ":config",
        ":mpl",
        ":static_assert",
        ":throw_exception",
        ":type_index",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "assign",
    deps = [
        ":config",
        ":detail",
        ":mpl",
        ":preprocessor",
        ":ptr_container",
        ":range",
        ":static_assert",
        ":type_traits",
    ],
)

boost_library(
    name = "atomic",
    hdrs = [
        "include/boost/memory_order.hpp",
    ],
    includes = ["include/boost"],
    deps = [
        ":assert",
        ":config",
        ":cstdint",
        ":type_traits",
    ],
)

boost_library(
    name = "archive",
    deps = [
        ":assert",
        ":cstdint",
        ":integer",
        ":io",
        ":iterator",
        ":mpl",
        ":noncopyable",
        ":smart_ptr",
        ":spirit",
    ],
)

boost_library(
    name = "array",
    deps = [
        ":assert",
        ":config",
        ":core",
        ":functional",
        ":swap",
        ":throw_exception",
    ],
)

boost_library(
    name = "asio",
    linkopts = ["-lpthread"],
    deps = [
        ":bind",
        ":date_time",
        ":regex",
    ],
)

boost_library(
    name = "assert",
)

boost_library(
    name = "beast",
    deps = [
        ":asio",
        ":config",
        ":core",
        ":detail",
        ":endian",
        ":smart_ptr",
        ":static_assert",
        ":system",
        ":throw_exception",
        ":utility",
    ],
)

boost_library(
    name = "bimap",
    deps = [
        ":concept_check",
        ":multi_index",
        ":serialization",
    ],
)

boost_library(
    name = "bind",
    deps = [
        ":get_pointer",
        ":is_placeholder",
        ":mem_fn",
        ":ref",
        ":type",
        ":visit_each",
    ],
)

boost_library(
    name = "callable_traits",
    deps = [
    ],
)

boost_library(
    name = "call_traits",
)

boost_library(
    name = "cerrno",
)

boost_library(
    name = "checked_delete",
)

boost_library(
    name = "chrono",
    deps = [
        ":config",
        ":mpl",
        ":operators",
        ":predef",
        ":ratio",
        ":system",
        ":throw_exception",
        ":type_traits",
    ],
)

boost_library(
    name = "circular_buffer",
    deps = [
        ":call_traits",
        ":concept_check",
        ":config",
        ":container",
        ":detail",
        ":iterator",
        ":limits",
        ":move",
        ":static_assert",
        ":throw_exception",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "concept_archetype",
    deps = [
        ":config",
        ":iterator",
        ":mpl",
    ],
)

boost_library(
    name = "concept_check",
    deps = [
        ":concept",
        ":concept_archetype",
    ],
)

boost_library(
    name = "config",
    deps = [
        ":version",
    ],
)

boost_library(
    name = "concept",
)

boost_library(
    name = "container",
    deps = [
        ":config",
        ":core",
        ":intrusive",
        ":move",
    ],
)

boost_library(
    name = "container_hash",
    deps = [
        ":assert",
        ":config",
        ":core",
        ":integer",
        ":limits",
        ":type_traits",
    ],
)

boost_library(
    name = "conversion",
)

boost_library(
    name = "core",
    hdrs = [
        "include/boost/checked_delete.hpp",
    ],
    includes = ["include/boost"],
    deps = [
        ":config",
    ],
)

boost_library(
    name = "coroutine",
    deps = [
        ":config",
        ":context",
        ":move",
        ":range",
        ":system",
        ":thread",
    ],
)

boost_library(
    name = "crc",
    deps = [
        ":config",
        ":integer",
        ":limits",
    ],
)

boost_library(
    name = "cstdint",
)

boost_library(
    name = "current_function",
)

boost_library(
    name = "date_time",
    deps = [
        ":algorithm",
        ":io",
        ":lexical_cast",
        ":mpl",
        ":operators",
        ":smart_ptr",
        ":static_assert",
        ":tokenizer",
        ":type_traits",
    ],
)

boost_library(
    name = "detail",
    hdrs = [
        "include/boost/blank.hpp",
        "include/boost/blank_fwd.hpp",
        "include/boost/cstdlib.hpp",
    ],
    includes = ["include/boost"],
    deps = [
        ":limits",
    ],
)

boost_library(
    name = "dynamic_bitset",
    deps = [
        ":config",
        ":core",
        ":detail",
        ":integer",
        ":move",
        ":throw_exception",
        ":utility",
    ],
)

boost_library(
    name = "enable_shared_from_this",
)

boost_library(
    name = "endian",
    deps = [
        ":config",
        ":core",
        ":cstdint",
        ":detail",
        ":predef",
        ":type_traits",
    ],
)

boost_library(
    name = "exception",
    hdrs = [
        "include/boost/exception_ptr.hpp",
    ],
    includes = ["include/boost"],
    deps = [
        ":config",
        ":detail",
    ],
)

boost_library(
    name = "exception_ptr",
    deps = [
        ":config",
    ],
)

boost_library(
    name = "filesystem",
    copts = [
        "-Wno-deprecated-declarations",
    ],
    deps = [
        ":config",
        ":functional",
        ":io",
        ":iterator",
        ":range",
        ":smart_ptr",
        ":system",
        ":type_traits",
    ],
)

boost_library(
    name = "foreach",
    deps = [
        ":config",
        ":detail",
        ":iterator",
        ":mpl",
        ":noncopyable",
        ":range",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "format",
    deps = [
        ":assert",
        ":config",
        ":detail",
        ":limits",
        ":optional",
        ":smart_ptr",
        ":throw_exception",
        ":timer",
        ":utility",
    ],
)

boost_library(
    name = "function",
    hdrs = [
        "include/boost/function_equal.hpp",
    ],
    includes = ["include/boost"],
    deps = [
        ":bind",
        ":integer",
        ":type_index",
    ],
)

boost_library(
    name = "function_types",
)

boost_library(
    name = "functional",
    deps = [
        ":container_hash",
        ":detail",
        ":integer",
    ],
)

boost_library(
    name = "fusion",
    deps = [
        ":call_traits",
        ":config",
        ":core",
        ":detail",
        ":function_types",
        ":functional",
        ":get_pointer",
        ":mpl",
        ":preprocessor",
        ":ref",
        ":static_assert",
        ":tuple",
        ":type_traits",
        ":typeof",
        ":utility",
    ],
)

boost_library(
    name = "geometry",
    deps = [
        ":algorithm",
        ":call_traits",
        ":config",
        ":function_types",
        ":lexical_cast",
        ":math",
        ":mpl",
        ":numeric",
        ":qvm",
        ":range",
        ":rational",
        ":tokenizer",
        ":variant",
    ],
)

boost_library(
    name = "property_tree",
    deps = [
        ":any",
        ":bind",
        ":format",
        ":multi_index",
        ":optional",
        ":range",
        ":ref",
        ":throw_exception",
        ":utility",
    ],
)

boost_library(
    name = "get_pointer",
)

boost_library(
    name = "heap",
    deps = [
        ":parameter",
    ],
)

boost_library(
    name = "icl",
    deps = [
        ":concept_check",
    ],
)

boost_library(
    name = "is_placeholder",
)

boost_library(
    name = "integer",
    hdrs = [
        "include/boost/cstdint.hpp",
        "include/boost/integer_traits.hpp",
        "include/boost/pending/integer_log2.hpp",
        #"include/boost/pending/lowest_bit.hpp",
    ],
    includes = ["include/boost", "include/boost/pending"],
    deps = [
        ":static_assert",
    ],
)

boost_library(
    name = "interprocess",
    deps = [
        ":assert",
        ":checked_delete",
        ":config",
        ":container",
        ":core",
        ":date_time",
        ":detail",
        ":integer",
        ":intrusive",
        ":limits",
        ":move",
        ":static_assert",
        ":type_traits",
        ":unordered",
        ":utility",
    ],
)

boost_library(
    name = "iterator",
    hdrs = [
        "include/boost/function_output_iterator.hpp",
        "include/boost/generator_iterator.hpp",
        "include/boost/indirect_reference.hpp",
        "include/boost/iterator_adaptors.hpp",
        "include/boost/next_prior.hpp",
        "include/boost/pointee.hpp",
        "include/boost/shared_container_iterator.hpp",
    ],
    includes = ["include/boost"],
    deps = [
        ":detail",
        ":static_assert",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "intrusive",
    deps = [
        ":assert",
        ":cstdint",
        ":noncopyable",
        ":static_assert",
    ],
)

boost_library(
    name = "intrusive_ptr",
    deps = [
        ":assert",
        ":detail",
        ":smart_ptr",
    ],
)

boost_library(
    name = "io",
)

boost_library(
    name = "lexical_cast",
    deps = [
        ":array",
        ":chrono",
        ":config",
        ":container",
        ":detail",
        ":integer",
        ":limits",
        ":math",
        ":mpl",
        ":noncopyable",
        ":numeric_conversion",
        ":range",
        ":static_assert",
        ":throw_exception",
        ":type_traits",
    ],
)

boost_library(
    name = "limits",
)

boost_library(
    name = "locale",
    copts = [
        "-DBOOST_LOCALE_WITH_ICONV",
        "-DBOOST_LOCALE_NO_WINAPI_BACKEND",
        # boost::locale implementation uses deprecated auto_ptr
        "-Wno-deprecated-declarations",
    ],
    deps = [
        ":assert",
        ":config",
        ":cstdint",
        ":smart_ptr",
        ":thread",
        ":unordered",
    ],
)

boost_library(
    name = "lockfree",
    deps = [
        ":align",
        ":array",
        ":assert",
        ":config",
        ":detail",
        ":noncopyable",
        ":parameter",
        ":predef",
        ":static_assert",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "math",
    hdrs = [
        "include/boost/cstdint.hpp",
    ],
    includes = ["include/boost"],
)

boost_library(
    name = "mem_fn",
)

boost_library(
    name = "move",
    deps = [
        ":assert",
        ":detail",
        ":static_assert",
    ],
)

boost_library(
    name = "mp11",
    deps = [
        ":config",
        ":detail",
    ],
)

boost_library(
    name = "mpl",
    deps = [
        ":move",
        ":preprocessor",
    ],
)

boost_library(
    name = "hana",
    deps = [
    ],
)

boost_library(
    name = "multi_index",
    hdrs = [
        "include/boost/multi_index_container.hpp",
        "include/boost/multi_index_container_fwd.hpp",
    ],
    includes = ["include/boost"],
    deps = [
        ":foreach",
        ":serialization",
        ":static_assert",
        ":tuple",
    ],
)

boost_library(
    name = "multiprecision",
    deps = [
        ":config",
        ":cstdint",
        ":lexical_cast",
        ":math",
        ":mpl",
        ":predef",
        ":rational",
        ":throw_exception",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "noncopyable",
)

boost_library(
    name = "none",
    hdrs = [
        "include/boost/none_t.hpp",
    ],
    includes = ["include/boost"],
)

boost_library(
    name = "numeric_conversion",
    hdrs = glob([
        "include/boost/numeric/conversion/**/*.hpp",
    ]),
    includes = ["include/boost/numeric/conversion"],
    deps = [
        ":config",
        ":detail",
        ":integer",
        ":limits",
        ":mpl",
        ":throw_exception",
        ":type",
        ":type_traits",
    ],
)

boost_library(
    name = "numeric_ublas",
    hdrs = glob([
        "include/boost/numeric/ublas/**/*.hpp",
    ]),
    includes = ["include/boost/numeric/ublas"],
    deps = [
        ":concept_check",
        ":config",
        ":core",
        ":iterator",
        ":mpl",
        ":noncopyable",
        ":numeric",
        ":range",
        ":serialization",
        ":shared_array",
        ":static_assert",
        ":timer",
        ":type_traits",
        ":typeof",
        ":utility",
    ],
)

boost_library(
    name = "numeric_odeint",
    hdrs = glob([
        "include/boost/numeric/odeint/**/*.hpp",
    ]),
    includes = ["include/boost/numeric/odeint"],
    deps = [
        ":fusion",
        ":lexical_cast",
        ":math",
        ":multi_array",
        ":numeric",
        ":serialization",
        ":units",
    ],
)

boost_library(
    name = "multi_array",
)

boost_library(
    name = "units",
)

boost_library(
    name = "operators",
)

boost_library(
    name = "optional",
    deps = [
        ":assert",
        ":config",
        ":detail",
        ":none",
        ":static_assert",
        ":throw_exception",
        ":type",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "parameter",
)

boost_library(
    name = "predef",
)

boost_library(
    name = "preprocessor",
)

# boost process is implemented in winapi
# no version for *nix system available
# commenting it for now
#boost_library(
#    name = "process",
#    deps = [
#        ":algorithm",
#        ":asio",
#        ":config",
#        ":filesystem",
#        ":fusion",
#        ":system",
#    ],
#)

boost_library(
    name = "program_options",
    deps = [
        ":any",
        ":bind",
        ":config",
        ":detail",
        ":function",
        ":iterator",
        ":lexical_cast",
        ":limits",
        ":noncopyable",
        ":shared_ptr",
        ":static_assert",
        ":throw_exception",
        ":tokenizer",
        ":type_traits",
        ":version",
    ],
)

boost_library(
    name = "ptr_container",
    deps = [
        ":assert",
        ":checked_delete",
        ":circular_buffer",
        ":config",
        ":iterator",
        ":mpl",
        ":range",
        ":serialization",
        ":static_assert",
        ":type_traits",
        ":unordered",
        ":utility",
    ],
)

boost_library(
    name = "qvm",
    deps = [
        ":assert",
        ":core",
        ":exception",
        ":static_assert",
        ":throw_exception",
        ":utility",
    ],
)

boost_library(
    name = "random",
    deps = [
        ":assert",
        ":config",
        ":detail",
        ":foreach",
        ":integer",
        ":lexical_cast",
        ":limits",
        ":math",
        ":mpl",
        ":multi_index",
        ":noncopyable",
        ":operators",
        ":range",
        ":regex",
        ":shared_ptr",
        ":static_assert",
        ":system",
        ":throw_exception",
        ":timer",
        ":tuple",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "range",
    deps = [
        ":array",
        ":assert",
        ":concept_check",
        ":config",
        ":detail",
        ":functional",
        ":integer",
        ":iterator",
        ":mpl",
        ":noncopyable",
        ":optional",
        ":preprocessor",
        ":ref",
        ":regex",
        ":static_assert",
        ":tuple",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "ratio",
    deps = [
        ":integer",
    ],
)

boost_library(
    name = "rational",
    deps = [
        ":assert",
        ":call_traits",
        ":config",
        ":detail",
        ":integer",
        ":operators",
        ":static_assert",
        ":throw_exception",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "ref",
    deps = [
        ":config",
        ":core",
        ":detail",
        ":utility",
    ],
)

boost_library(
    name = "regex",
    hdrs = [
        "include/boost/cregex.hpp",
    ],
    includes = ["include/boost"],
    defines = [
        "BOOST_FALLTHROUGH",
    ],
    deps = [
        ":assert",
        ":config",
        ":cstdint",
        ":detail",
        ":exception",
        ":functional",
        ":integer",
        ":limits",
        ":mpl",
        ":predef",
        ":ref",
        ":smart_ptr",
        ":throw_exception",
        ":type_traits",
    ],
)

boost_library(
    name = "scope_exit",
    deps = [
        ":config",
        ":detail",
        ":function",
        ":mpl",
        ":preprocessor",
        ":type_traits",
        ":typeof",
        ":utility",
    ],
)

boost_library(
    name = "scoped_array",
    deps = [
        ":checked_delete",
    ],
)

boost_library(
    name = "scoped_ptr",
    deps = [
        ":checked_delete",
    ],
)

boost_library(
    name = "shared_ptr",
    deps = [
        ":checked_delete",
    ],
)

boost_library(
    name = "shared_array",
    deps = [
        ":checked_delete",
    ],
)

boost_library(
    name = "signals2",
    deps = [
        ":assert",
        ":bind",
        ":checked_delete",
        ":config",
        ":core",
        ":detail",
        ":function",
        ":iterator",
        ":mpl",
        ":multi_index",
        ":noncopyable",
        ":optional",
        ":parameter",
        ":predef",
        ":preprocessor",
        ":ref",
        ":scoped_ptr",
        ":shared_ptr",
        ":smart_ptr",
        ":swap",
        ":throw_exception",
        ":tuple",
        ":type_traits",
        ":utility",
        ":variant",
        ":visit_each",
    ],
)

boost_library(
    name = "serialization",
    deps = [
        ":archive",
        ":array",
        ":call_traits",
        ":config",
        ":detail",
        ":function",
        ":operators",
        ":type_traits",
    ],
)

boost_library(
    name = "smart_ptr",
    hdrs = [
        "include/boost/enable_shared_from_this.hpp",
        "include/boost/make_shared.hpp",
        "include/boost/make_unique.hpp",
        "include/boost/pointer_to_other.hpp",
        "include/boost/weak_ptr.hpp",
    ],
    includes = ["include/boost"],
    deps = [
        ":align",
        ":core",
        ":predef",
        ":scoped_array",
        ":scoped_ptr",
        ":shared_array",
        ":shared_ptr",
        ":throw_exception",
        ":utility",
    ],
)

boost_library(
    name = "spirit",
    deps = [
        ":optional",
        ":ref",
        ":utility",
    ],
)

boost_library(
    name = "static_assert",
)

boost_library(
    name = "system",
    deps = [
        ":assert",
        ":cerrno",
        ":config",
        ":core",
        ":cstdint",
        ":noncopyable",
        ":predef",
        ":utility",
    ],
)

boost_library(
    name = "swap",
)

boost_library(
    name = "throw_exception",
    deps = [
        ":current_function",
        ":detail",
        ":exception",
    ],
)

boost_library(
    name = "thread",
    linkopts = ["-lpthread"],
    deps = [
        ":algorithm",
        ":atomic",
        ":bind",
        ":chrono",
        ":config",
        ":core",
        ":date_time",
        ":detail",
        ":enable_shared_from_this",
        ":exception",
        ":function",
        ":io",
        ":lexical_cast",
        ":smart_ptr",
        ":system",
        ":tuple",
        ":type_traits",
    ],
)

boost_library(
    name = "tokenizer",
    hdrs = [
        "include/boost/token_functions.hpp",
        "include/boost/token_iterator.hpp",
    ],
    includes = ["include/boost"],
    deps = [
        ":assert",
        ":config",
        ":detail",
        ":iterator",
        ":mpl",
        ":throw_exception",
    ],
)

boost_library(
    name = "tribool",
    hdrs = [
        "include/boost/logic/tribool.hpp",
        "include/boost/logic/tribool_fwd.hpp",
    ],
    includes = ["include/boost/logic"],
    deps = [
        ":config",
        ":detail",
    ],
)

boost_library(
    name = "tti",
    deps = [
        ":config",
        ":function_types",
        ":mpl",
        ":preprocessor",
        ":type_traits",
    ],
)

boost_library(
    name = "type",
    deps = [
        ":core",
    ],
)

boost_library(
    name = "type_index",
    deps = [
        ":container_hash",
        ":core",
        ":functional",
        ":throw_exception",
    ],
)

boost_library(
    name = "type_traits",
    hdrs = [
        "include/boost/aligned_storage.hpp",
    ],
    includes = ["include/boost"],
    deps = [
        ":config",
        ":core",
        ":mpl",
        ":static_assert",
    ],
)

boost_library(
    name = "typeof",
    deps = [
        ":config",
        ":detail",
        ":mpl",
        ":preprocessor",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "tuple",
    deps = [
        ":ref",
    ],
)

boost_library(
    name = "unordered",
    hdrs = [
        "include/boost/unordered_map.hpp",
        "include/boost/unordered_set.hpp",
    ],
    includes = ["include/boost"],
    deps = [
        ":assert",
        ":config",
        ":container",
        ":detail",
        ":functional",
        ":iterator",
        ":limits",
        ":move",
        ":preprocessor",
        ":smart_ptr",
        ":swap",
        ":throw_exception",
        ":tuple",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "utility",
    hdrs = [
        "include/boost/compressed_pair.hpp",
        "include/boost/next_prior.hpp",
    ],
    includes = ["include/boost"],
    deps = [
        ":config",
        ":detail",
        ":swap",
    ],
)

boost_library(
    name = "uuid",
    deps = [
        ":assert",
        ":config",
        ":core",
        ":detail",
        ":io",
        ":random",
        ":serialization",
        ":static_assert",
        ":throw_exception",
        ":tti",
        ":type_traits",
    ],
)

boost_library(
    name = "variant",
    deps = [
        ":call_traits",
        ":config",
        ":detail",
        ":functional",
        ":math",
        ":static_assert",
        ":type_index",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "version",
)

boost_library(
    name = "visit_each",
)

boost_library(
    name = "timer",
    deps = [
        ":cerrno",
        ":chrono",
        ":config",
        ":cstdint",
        ":io",
        ":limits",
        ":system",
        ":throw_exception",
    ],
)

boost_library(
    name = "numeric",
)

boost_library(
    name = "proto",
    deps = [
        ":fusion",
    ],
)

boost_library(
    name = "phoenix",
    deps = [
        ":proto",
    ],
)

boost_library(
    name = "log",
    copts = ["-msse4.2"],
    defines = [
        "BOOST_LOG_WITHOUT_WCHAR_T",
        "BOOST_LOG_USE_STD_REGEX",
        "BOOST_LOG_WITHOUT_DEFAULT_FACTORIES",
        "BOOST_LOG_WITHOUT_SETTINGS_PARSERS",
        "BOOST_LOG_WITHOUT_DEBUG_OUTPUT",
        "BOOST_LOG_WITHOUT_EVENT_LOG",
        "BOOST_LOG_WITHOUT_SYSLOG",
        "BOOST_LOG_DYN_LINK",
    ],
    deps = [
        ":asio",
        ":date_time",
        ":filesystem",
        ":interprocess",
        ":locale",
        ":parameter",
        ":phoenix",
        ":random",
        ":spirit",
        ":system",
        ":thread",
        ":variant",
    ],
)

boost_library(
    name = "statechart",
)

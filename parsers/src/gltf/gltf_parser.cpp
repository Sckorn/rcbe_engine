#include <filesystem>
#include <fstream>
#include <limits>
#include <thread>
#include <vector>

#include <boost/log/trivial.hpp>

#include <nlohmann/json.hpp>

#include <rcbe-engine/datamodel/core/CoreObject.hpp>
#include <rcbe-engine/datamodel/math/Matrix.hpp>
#include <rcbe-engine/datamodel/geometry/Mesh.hpp>
#include <rcbe-engine/datamodel/rendering/Material.hpp>
#include <rcbe-engine/datamodel/rendering/RasterizerTexture.hpp>
#include <rcbe-engine/datamodel/visual/Texture.hpp>
#include <rcbe-engine/parsers/gltf/gltf_parser.hpp>
#include <rcbe-engine/parsers/tga/tga_parser.hpp>
#include <rcbe-engine/utils/json_utils.hpp>
#include <rcbe-engine/utils/output_utils.hpp>

#ifdef RDMN_VULKAN
#include <vulkan/vulkan.hpp>
#endif

inline constexpr std::string_view GLTF_SUPPORTED_VERTEX_TYPE = "VEC3";
inline constexpr std::string_view GLTF_SUPPORTED_NORMAL_TYPE = GLTF_SUPPORTED_VERTEX_TYPE;
inline constexpr std::string_view GLTF_SUPPORTED_TEX_COORD_TYPE = "VEC2";
inline constexpr std::string_view GLTF_SUPPORTED_INDEX_TYPE = "SCALAR";
inline constexpr std::string_view GLTF_MAIN_EXTENSION_STRING = ".gltf";
inline constexpr std::string_view GLTF_BIN_EXTENSION_STRING = ".bin";
inline constexpr size_t UNSET_INDEX = std::numeric_limits<size_t>::max();
inline constexpr size_t DEFAULT_VISITED_INDICES_AMOUNT = 10; /// randomly picked

struct gltf_buffer {
    std::string uri;
    size_t byte_length;
    std::string name;
};

struct gltf_buffer_view {
    size_t buffer_idx;
    size_t byte_offset = 0;
    size_t byte_length;
    size_t byte_stride;
    size_t target;
    std::string name;
};

enum class gltf_acc_comp_type : uint16_t {
    byte = 5120,
    ubyte = 5121,
    short_t = 5122, /// TODO: consider making enum memebers uppercase, this name is to avoid conflict with built-in short type @sckorn
    unsigned_short = 5123,
    unsigned_int = 5125,
    float_t = 5126,
};

struct gltf_accessor {
    size_t buffer_view_idx;
    size_t acc_byte_offset;
    gltf_acc_comp_type comp_type;
    bool normalized = false;
    size_t count;
    std::string type;
    std::vector<float> min;
    std::vector<float> max;
    std::string name;
};

struct gltf_node {
    size_t camera_index;
    std::vector<size_t> children_indices;
    std::vector<size_t> skin_indices;
    std::vector<float> matrix;
    size_t mesh_index;
    std::string name;
    /// TODO: add remaining members, when needed @sckorn @radameon
};

enum class gltf_primitive_topology {
    points = 0,
    lines = 1,
    line_loop = 2,
    lines_strip = 3,
    triangles = 4,
    triangle_strip = 5,
    triangle_fan = 6,
};

struct gltf_mesh_attr {
    size_t position_acccessor_idx;
    size_t normal_accessor_idx;
    size_t texcoord_accessor_idx;
};

struct gltf_mesh_prim {
    gltf_mesh_attr attributes;
    size_t indices_index;
    size_t material_index;
    gltf_primitive_topology topology_mode;
};

struct gltf_mesh {
    std::vector<gltf_mesh_prim> primitives;
    std::string name;
};

struct gltf_pbr_met_rough {
    size_t color_tex_idx;
    float metallic_factor;
    float roughness_factor;
};

struct gltf_material {
    std::string name;
    gltf_pbr_met_rough metal_rough;
};

struct gltf_texture {
    size_t sampler_idx;
    size_t source_image_idx;
};

struct gltf_image {
    std::string uri;
};

namespace nlohmann {
template <>
struct adl_serializer<gltf_pbr_met_rough> {
    static void from_json(const json &j, gltf_pbr_met_rough &mr) {
        mr = {
          .color_tex_idx = j.at("baseColorTexture").at("index").get<size_t>(),
          .metallic_factor = j.at("metallicFactor").get<float>(),
          .roughness_factor = j.at("roughnessFactor").get<float>(),
        };
    }

    static void to_json(json &j, const gltf_pbr_met_rough &mr) {
        json subj = {
                {"index", mr.color_tex_idx},
        };
        j = {
                {"baseColorTexture", subj},
                {"metallicFactor", mr.metallic_factor},
                {"roughnessFactor", mr.roughness_factor},
        };
    }
};

template <>
struct adl_serializer<gltf_material> {
    static void from_json(const json &j, gltf_material &m) {
        m = {
          .name = j.value<std::string>("name", std::string{}),
          .metal_rough = j.at("pbrMetallicRoughness").get<gltf_pbr_met_rough>(),
        };
    }

    static void to_json(json &j, const gltf_material &m) {
        j = {
                {"pbrMetallicRoughness", m.metal_rough},
        };

        if (!m.name.empty())
            j.push_back({"name", m.name});
    }
};

template <>
struct adl_serializer<gltf_texture> {
    static void from_json(const json &j, gltf_texture &t) {
        t = {
                .sampler_idx = j.at("sampler").get<size_t>(),
                .source_image_idx = j.at("source").get<size_t>(),
        };
    }

    static void to_json(json &j, const gltf_texture &t) {
        j = {
                {"sampler", t.sampler_idx},
                {"source", t.source_image_idx},
        };
    }
};

template <>
struct adl_serializer<gltf_image> {
    static void from_json(const json &j, gltf_image &i) {
        i = {
          .uri = j.at("uri").get<std::string>(),
        };
    }

    static void to_json(json &j, const gltf_image &i) {
        j = {
                {"uri", i.uri},
        };
    }
};

template <>
struct adl_serializer<gltf_buffer> {
    static void from_json(const json &j, gltf_buffer &b) {
        b.byte_length = j.at("byteLength").get<size_t>();
        b.name = j.value<std::string>("name", std::string{});
        b.uri = j.value<std::string>("uri", std::string{});
    }

    static void to_json(json &j, const gltf_buffer &b) {
        j = {
                {"byteLength", b.byte_length},
        };

        if (!b.name.empty())
            j.push_back({"name", b.name});

        if (!b.uri.empty())
            j.push_back({"uri", b.uri});
    }
};

template <>
struct adl_serializer<gltf_buffer_view> {
    static void from_json(const json &j, gltf_buffer_view &bv) {
        bv.name = j.value<std::string>("name", std::string{});
        bv.buffer_idx = j.at("buffer").get<size_t>();
        bv.byte_offset = j.value<size_t>("byteOffset", 0);
        bv.byte_length = j.at("byteLength").get<size_t>();
        bv.byte_stride = j.value<size_t>("byteStride", 4);
        bv.target = j.value<size_t>("target", 34962);
    }

    static void to_json(json &j, const gltf_buffer_view &bv) {
        j = {
                {"buffer", bv.buffer_idx},
                {"byteLength", bv.byte_length},
                {"byteStride", bv.byte_stride},
                {"target", bv.target},
        };

        if (!bv.name.empty())
            j.push_back({"name", bv.name});

        if (bv.byte_offset != 0)
            j.push_back({"byteOffset", bv.byte_offset});
    }
};

template <>
struct adl_serializer<gltf_accessor> {
    static void from_json(const json &j, gltf_accessor &acc) {
        acc.buffer_view_idx = j.value<size_t>("bufferView", std::numeric_limits<size_t>::max());
        acc.acc_byte_offset = j.value<size_t>("byteOffset", 0);
        acc.comp_type = static_cast<gltf_acc_comp_type>(j.at("componentType").get<uint16_t>());
        acc.count = j.at("count").get<size_t>();
        acc.type = j.at("type").get<std::string>();
        acc.min = j.value<std::vector<float>>("min", std::vector<float>{});
        acc.max = j.value<std::vector<float>>("max", std::vector<float>{});
        acc.name = j.value<std::string>("name", std::string{});
        acc.normalized = j.value<bool>("normalized", false);
    }

    static void to_json(json &j, const gltf_accessor &acc) {
        j = {
                {"componentType", static_cast<uint16_t>(acc.comp_type)},
                {"count", acc.count},
                {"type", acc.type},
                {"normalized", acc.normalized},
                {"min", acc.min},
                {"max", acc.max},
        };

        if (acc.buffer_view_idx != UNSET_INDEX)
            j.push_back({"bufferView", acc.buffer_view_idx});

        if (acc.acc_byte_offset != 0)
            j.push_back({"byteOffset", acc.acc_byte_offset});

        if (!acc.name.empty())
            j.push_back({"name", acc.name});
    }
};

template <>
struct adl_serializer<gltf_mesh_attr> {
    static void from_json(const json &j, gltf_mesh_attr &attr) {
        attr.normal_accessor_idx = j.at("NORMAL").get<size_t>();
        attr.position_acccessor_idx = j.at("POSITION").get<size_t>();
        attr.texcoord_accessor_idx = j.at("TEXCOORD_0").get<size_t>();
    }

    static void to_json(json &j, const gltf_mesh_attr &attr) {
        j = {
                {"NORMAL", attr.normal_accessor_idx},
                {"POSITION", attr.position_acccessor_idx},
                {"TEXCOORD_0", attr.texcoord_accessor_idx},
        };
    }
};

template <>
struct adl_serializer<gltf_mesh_prim> {
    static void from_json(const json &j, gltf_mesh_prim &prim) {
        prim.attributes = j.at("attributes").get<gltf_mesh_attr>();
        prim.topology_mode = static_cast<gltf_primitive_topology>(j.at("mode").get<int>());
        prim.indices_index = j.at("indices").get<size_t>();
        prim.material_index = j.at("material").get<size_t>();
    }

    static void to_json(json &j, const gltf_mesh_prim &prim) {
        j = {
                {"attributes", prim.attributes},
                {"mode", prim.topology_mode},
                {"indices", prim.indices_index},
                {"material", prim.material_index},
        };
    }
};

template <>
struct adl_serializer<gltf_mesh> {
    static void from_json(const json &j, gltf_mesh &mesh) {
        mesh.name = j.at("name").get<std::string>();
        mesh.primitives = j.at("primitives").get<std::vector<gltf_mesh_prim>>();
    }

    static void to_json(json &j, const gltf_mesh &mesh) {
        j = {
                {"name", mesh.name},
                {"primitives", mesh.primitives},
        };
    }
};

template <>
struct adl_serializer<gltf_node> {
    static void from_json(const json &j, gltf_node &node) {
        const auto m = j.value<std::array<float, 16>>("matrix", std::array<float, 16> {});
        node = {
            .camera_index = j.value<size_t>("camera", UNSET_INDEX),
            .children_indices = j.value<std::vector<size_t>>("children", std::vector<size_t> {}),
            .skin_indices = j.value<std::vector<size_t>>("skin", std::vector<size_t> {}),
            .matrix = j.value<std::vector<float>>("matrix", std::vector<float> {}),
            .mesh_index = j.value<size_t>("mesh", UNSET_INDEX),
            .name = j.value<std::string>("name", std::string{}),
        };
    }

    static void to_json(json &j, const gltf_node &node) {
        const auto index_max_value = std::numeric_limits<size_t>::max();
        if (node.camera_index != index_max_value) {
            j.push_back({"camera", node.camera_index});
        }

        if (node.mesh_index != index_max_value) {
            j.push_back({"mesh", node.mesh_index});
        }

        if (!node.children_indices.empty()) {
            j.push_back({"children", node.children_indices});
        }

        if (!node.skin_indices.empty()) {
            j.push_back({"skin", node.skin_indices});
        }

        if (!node.name.empty())
            j.push_back({"name", node.name});

        j.push_back({"matrix", node.matrix}); /// TODO: find the way to handle this properly later
    }
};
}

namespace {
template <typename R, typename F = R>
std::vector<R> getValuesByAccessor(
        const size_t accessor_idx,
        std::string_view accessor_type,
        gltf_acc_comp_type accessor_component_type,
        const std::vector<gltf_accessor> &accessors,
        const std::vector<gltf_buffer_view> &buffer_views,
        const rcbe::binary::BinaryBuffer &bb,
        const size_t size_to_fill_if_empty
) {
    static_assert(F::DIMENSION == R::DIMENSION, "Can't process different dimensions!");
    const auto accessor = accessors[accessor_idx];
    if (accessor.type != accessor_type)
        throw std::runtime_error("Any type except " + std::string(accessor_type) + " is not supported for vertices");

    if (accessor.comp_type != accessor_component_type)
        throw std::runtime_error("Unsupported accessor type!");

    std::vector<R> ret;
    if (accessor.buffer_view_idx != UNSET_INDEX) {
        const auto b_view = buffer_views[accessor.buffer_view_idx];
        auto byte_offset = b_view.byte_offset + accessor.acc_byte_offset;
        auto byte_length = b_view.byte_length;
        const auto stride = b_view.byte_stride;
        if (stride != sizeof(typename F::ValueType) * F::DIMENSION)
            throw std::runtime_error("Wrong stride!");

        ret.reserve(byte_length / stride * 2);

        while(ret.size() < accessor.count) {
            auto v = bb.at(byte_offset, stride).get<F>();
            if constexpr (F::DIMENSION == 2) {
                ret.push_back({
                                      static_cast<typename R::ValueType>(v.x()),
                                      static_cast<typename R::ValueType>(v.y()),
                              });
            } else {
                ret.push_back({
                                      static_cast<typename R::ValueType>(v.x()),
                                      static_cast<typename R::ValueType>(v.y()),
                                      static_cast<typename R::ValueType>(v.z())
                              });
            }
        }
    } else {
        BOOST_LOG_TRIVIAL(warning) << "No data available";
        ret = std::vector<R> { size_to_fill_if_empty, R{} };
    }

    return ret;
}

template <>
rcbe::geometry::Mesh::FacetStorage getValuesByAccessor(
        const size_t accessor_idx,
        std::string_view accessor_type,
        gltf_acc_comp_type accessor_component_type,
        const std::vector<gltf_accessor> &accessors,
        const std::vector<gltf_buffer_view> &buffer_views,
        const rcbe::binary::BinaryBuffer &bb,
        const size_t size_to_fill_if_empty
) {
    const auto indices_accessor = accessors[accessor_idx];

    if (indices_accessor.type != accessor_type)
        throw std::runtime_error("Any type except SCALAR is not supported for texture coordinates");

    if (indices_accessor.comp_type != accessor_component_type)
        throw std::runtime_error("Only uint indices are supported!");

    const auto b_view = buffer_views[indices_accessor.buffer_view_idx];
    auto byte_offset = b_view.byte_offset + indices_accessor.acc_byte_offset;
    auto byte_length = b_view.byte_length;
    const auto stride = b_view.byte_stride;
    if (stride != sizeof(uint32_t))
        throw std::runtime_error("Wrong stride!");

    rcbe::geometry::Mesh::FacetStorage ret{};
    ret.reserve(byte_length / stride);

    while(ret.size() * 3 < indices_accessor.count) {
        auto fv1 = bb.at(byte_offset, stride).get<uint32_t>();
        auto fv2 = bb.at(byte_offset, stride).get<uint32_t>();
        auto fv3 = bb.at(byte_offset, stride).get<uint32_t>();

        ret.push_back({fv1, fv2, fv3, fv1, fv2, fv3});
    }

    return ret;
}
}

namespace rdmn::parse::gltf {

std::vector<rcbe::core::CoreObject> parse(
        const rcbe::core::EnginePath &gltf_file_path,
        const rcbe::core::EnginePath &bin_file_path
) {
    if (!std::filesystem::exists(gltf_file_path) || !std::filesystem::exists(bin_file_path))
        throw std::runtime_error(gltf_file_path.string() + " or " + bin_file_path.string() + " doesn't exist!");

    bool not_files_of_extension = !std::filesystem::is_regular_file(gltf_file_path)
            || gltf_file_path.extension() != GLTF_MAIN_EXTENSION_STRING
            || !std::filesystem::is_regular_file(bin_file_path)
            || bin_file_path.extension() != GLTF_BIN_EXTENSION_STRING;

    if (not_files_of_extension)
        throw std::runtime_error(
                gltf_file_path.string() + " or " + bin_file_path.string() + " is not a file or of a wrong extension!"
        );

    std::vector<rcbe::core::CoreObject> ret{};
    /// TODO: below is a texture cash to reduce parsing times, remove it when proper ResourceManager is introduced
    std::unordered_map<rcbe::core::EnginePath, rcbe::visual::TexturePtr> tex_cache;
    {
        rcbe::binary::BinaryBuffer bb;
        std::jthread bin_parser([bin_file_path, &bb]() {
            std::ifstream ifs{ bin_file_path.string(), std::ios::binary | std::ios::in };
            ifs >> bb;

            BOOST_LOG_TRIVIAL(trace) << "Read binary part of GLTF successfully!";
        });
        const auto gltf_json = rcbe::utils::read_raw(gltf_file_path.string());

        BOOST_LOG_TRIVIAL(trace) << "Total nodes " << gltf_json.at("nodes").size();

        const auto total_gltf_nodes = gltf_json.at("nodes").size();
        std::vector<gltf_node> nodes(total_gltf_nodes);
        std::vector<bool> nodes_processed(total_gltf_nodes, false);
        size_t root_node_idx = UNSET_INDEX;
        for (size_t i = 0; i < total_gltf_nodes; ++i) {
            nodes[i] = gltf_json.at("nodes")[i].get<gltf_node>();
            if (nodes[i].children_indices.size() > 1) {
                root_node_idx = i;
                BOOST_LOG_TRIVIAL(trace) << "Multiple children in node " << i;
            }
        }

        BOOST_LOG_TRIVIAL(trace) << "Total meshes " << gltf_json.at("meshes").size();
        const auto total_gltf_meshes = gltf_json.at("meshes").size();
        std::vector<gltf_mesh> meshes(total_gltf_meshes);
        for (size_t i = 0; i < total_gltf_meshes; ++i) {
            meshes[i] = gltf_json.at("meshes")[i].get<gltf_mesh>();
        }

        const auto accessors_node = gltf_json.at("accessors");
        const auto total_accessors = accessors_node.size();
        BOOST_LOG_TRIVIAL(trace) << "Total accessors " << total_accessors;
        std::vector<gltf_accessor> accessors(total_accessors);
        for (size_t i = 0; i < total_accessors; ++i) {
            accessors[i] = accessors_node[i].get<gltf_accessor>();
        }

        const auto buffer_views_node = gltf_json.at("bufferViews");
        const auto total_buffer_views = buffer_views_node.size();
        BOOST_LOG_TRIVIAL(trace) << "Total buffer views " << total_buffer_views;
        std::vector<gltf_buffer_view> buffer_views(total_buffer_views);
        for (size_t i = 0; i < total_buffer_views; ++i) {
            buffer_views[i] = buffer_views_node[i].get<gltf_buffer_view>();
        }

        const auto buffers_node = gltf_json.at("buffers");
        const auto total_buffers = buffers_node.size();
        BOOST_LOG_TRIVIAL(trace) << "Total buffers " << total_buffers;
        std::vector<gltf_buffer> buffers(total_buffers);
        for (size_t i = 0; i < total_buffers; ++i) {
            buffers[i] = buffers_node[i].get<gltf_buffer>();
        }

        const auto materials_node = gltf_json.at("materials");
        const auto total_materials = materials_node.size();
        BOOST_LOG_TRIVIAL(trace) << "Total materials " << total_materials;
        std::vector<gltf_material> materials(total_materials);
        for (size_t i = 0; i < total_materials; ++i) {
            materials[i] = materials_node[i].get<gltf_material>();
        }

        const auto textures_node = gltf_json.at("textures");
        const auto total_textures = textures_node.size();
        BOOST_LOG_TRIVIAL(trace) << "Total textures " << total_textures;
        std::vector<gltf_texture> textures(total_textures);
        for (size_t i = 0; i < total_textures; ++i) {
            textures[i] = textures_node[i].get<gltf_texture>();
        }

        const auto images_node = gltf_json.at("images");
        const auto total_iamges = images_node.size();
        BOOST_LOG_TRIVIAL(trace) << "Total images " << total_iamges;
        std::vector<gltf_image> images(total_iamges);
        for (size_t i = 0; i < total_iamges; ++i) {
            images[i] = images_node[i].get<gltf_image>();
        }

        size_t start_index = 0;
        rcbe::math::Matrix4x4 root_trn{};
        if (root_node_idx != UNSET_INDEX) {
            start_index = root_node_idx + 1;
            for (size_t i = 0; i < start_index; ++i) {
                if (!nodes[i].matrix.empty()) {
                    rcbe::math::Matrix4x4 t{nodes[i].matrix.begin(), nodes[i].matrix.end()};
                    root_trn = root_trn * t.transposed();
                }
            }
        }

        ret.reserve(meshes.size());
        BOOST_LOG_TRIVIAL(trace) << "Starting iterating over nodes, looking for meshes!";
        for (size_t i = start_index; i < nodes.size();) {
            auto &n = nodes[i];
            if (nodes_processed[i]) {
                BOOST_LOG_TRIVIAL(trace) << "Node " << i << " is already processed!";
                ++i;
                continue;
            }

            BOOST_LOG_TRIVIAL(trace) << "Processing node " << i;
            nodes_processed[i] = true;

            if (n.children_indices.size() > 1) {
                BOOST_LOG_TRIVIAL(trace) << "Not handling nodes with more than one children!";
                ++i;
                continue;
            }

            bool has_mesh = n.mesh_index != UNSET_INDEX;

            /// We do not expect the node that has children to also have mesh index,
            /// so single if such node is found
            if (has_mesh && !n.children_indices.empty())
                BOOST_LOG_TRIVIAL(trace) << "Node with children and mesh index found!";

            rcbe::math::Matrix4x4 transform = root_trn;
            if (!n.matrix.empty())
                transform = transform * rcbe::math::Matrix4x4(n.matrix.begin(), n.matrix.end()).transposed();

            std::string name = n.name;

            size_t mesh_index = UNSET_INDEX;
            std::vector<size_t> visited_indices;
            visited_indices.reserve(DEFAULT_VISITED_INDICES_AMOUNT);

            /// We make a presumption that if node has a child, it is most likely
            /// a node containing a mesh
            do {
                visited_indices.push_back(n.children_indices.at(0));
                n = nodes[visited_indices.back()];
                name.append("_");
                name.append(n.name);
                has_mesh = n.mesh_index != UNSET_INDEX;
                if (has_mesh)
                    mesh_index = n.mesh_index;
                if (!n.children_indices.empty()) {
                    auto t = rcbe::math::Matrix4x4 { n.matrix.begin(), n.matrix.end() };
                    transform = transform * t.transposed();
                }
            } while(!n.children_indices.empty());

            for (const auto vi : visited_indices) {
                nodes_processed[vi] = true;
                BOOST_LOG_TRIVIAL(trace) << "Node " << vi << " is processed as a child of " << i;
            }

            if (mesh_index == UNSET_INDEX)
                throw std::runtime_error("Couldn't find a mesh amongst children");

            const auto mesh_node = meshes[mesh_index];
            if (mesh_node.primitives.size() > 1)
                throw std::runtime_error("Mesh node can't have more than one primitives set");
            const auto mesh_primitives = mesh_node.primitives.at(0);

            if (mesh_primitives.topology_mode != gltf_primitive_topology::triangles)
                throw std::runtime_error("Any topology mode except triangles is not supported yet!");

            rcbe::geometry::Mesh::VertexStorage vertices{};
            rcbe::geometry::Mesh::NormalStorage normals{};
            rcbe::geometry::Mesh::TexCoordStorage tex_coord{};
            rcbe::geometry::Mesh::FacetStorage facets{};

            if (bin_parser.joinable())
                bin_parser.join();

            if (mesh_primitives.attributes.position_acccessor_idx != UNSET_INDEX) {
                vertices = getValuesByAccessor<rcbe::geometry::Mesh::VertexStorage::value_type, rcbe::math::Vector3f>(
                    mesh_primitives.attributes.position_acccessor_idx,
                    GLTF_SUPPORTED_VERTEX_TYPE,
                    gltf_acc_comp_type::float_t,
                    accessors,
                    buffer_views,
                    bb,
                    0
                );
                /// TODO: introduce bound box class, and parse min and max fields of accessor, since they are a BB
            }

            if(vertices.empty()) {
                BOOST_LOG_TRIVIAL(trace) << "No vertices parsed for the mesh, no use parsing anything else";
                continue;
            }

            if (mesh_primitives.attributes.normal_accessor_idx != UNSET_INDEX) {
                normals = getValuesByAccessor<rcbe::geometry::Mesh::NormalStorage::value_type, rcbe::math::Vector3f>(
                    mesh_primitives.attributes.normal_accessor_idx,
                    GLTF_SUPPORTED_NORMAL_TYPE,
                    gltf_acc_comp_type::float_t,
                    accessors,
                    buffer_views,
                    bb,
                    vertices.size()
                );
            }

            if (mesh_primitives.attributes.texcoord_accessor_idx != UNSET_INDEX) {
                tex_coord = getValuesByAccessor<rcbe::geometry::Mesh::TexCoordStorage::value_type, rcbe::math::Vector2f>(
                    mesh_primitives.attributes.texcoord_accessor_idx,
                    GLTF_SUPPORTED_TEX_COORD_TYPE,
                    gltf_acc_comp_type::float_t,
                    accessors,
                    buffer_views,
                    bb,
                    vertices.size()
                );
            }

            if (mesh_primitives.indices_index != UNSET_INDEX) {
                facets = getValuesByAccessor<rcbe::geometry::Mesh::FacetStorage::value_type>(
                    mesh_primitives.indices_index,
                    GLTF_SUPPORTED_INDEX_TYPE,
                    gltf_acc_comp_type::unsigned_int,
                    accessors,
                    buffer_views,
                    bb,
                    vertices.size() / 3
                );
            }

            BOOST_LOG_TRIVIAL(trace) << "Total vertices: " << vertices.size();
            BOOST_LOG_TRIVIAL(trace) << "Total normals: " << normals.size();
            BOOST_LOG_TRIVIAL(trace) << "Total tex coords: " << tex_coord.size();
            BOOST_LOG_TRIVIAL(trace) << "Total facets: " << facets.size();

            if (vertices.size() != tex_coord.size()) {
                throw std::runtime_error("Vertices and tex coords are of unequal sizes!");
            }

            rcbe::geometry::Mesh m(
                std::move(vertices),
                std::move(normals),
                std::move(facets),
                std::move(tex_coord),
                rcbe::visual::RGBAColor(0xAAAAAAFF)
            );

            m.transform(rcbe::math::Transform{transform});

            rcbe::core::CoreObject obj(std::move(name));
            obj.addComponent<rcbe::geometry::Mesh>(std::move(m));

            if (mesh_primitives.material_index != UNSET_INDEX) {
                const auto &material = materials[mesh_primitives.material_index];
                if (material.metal_rough.color_tex_idx != UNSET_INDEX) {
                    const auto &color_texture = textures[material.metal_rough.color_tex_idx];
                    if (color_texture.source_image_idx != UNSET_INDEX) {
                        const auto &image = images[color_texture.source_image_idx];
                        if (!image.uri.empty() && !image.uri.starts_with("data:")) {
                            const auto dir = gltf_file_path.parent_path();
                            const auto rel_path = std::filesystem::path {image.uri};
                            const auto proper_ext_file = rel_path.stem().string() + std::string(".tga");
                            const auto proper_path = dir / rel_path.parent_path() / std::filesystem::path{proper_ext_file};
                            if (std::filesystem::exists(proper_path)) {
                                const auto it = tex_cache.find(proper_path);
                                rcbe::visual::TexturePtr tex_ptr;
                                if (it != tex_cache.end()) {
                                    BOOST_LOG_TRIVIAL(trace) << "Found texture in cache!";
                                    tex_ptr = it->second;
                                } else {
                                    BOOST_LOG_TRIVIAL(trace) << "No texture in cache!";
                                    tex_ptr = rcbe::visual::make_tex_ptr(proper_path, rdmn::parse::tga::parse);
                                    tex_cache.insert({proper_path, tex_ptr});
                                }
                                rdmn::render::rasterizer_texture_config rtc {};
                                rdmn::render::RasterizerTexture rt {rtc, tex_ptr};

                                BOOST_LOG_TRIVIAL(trace) << "Setting material for object";

                                obj.addComponent<rdmn::render::RasterizerTexture>(std::move(rt));
                            }
                        }
                    }
                }
            }

            ret.push_back(std::move(obj));

            ++i;
        }
    }

    return ret;
}

}
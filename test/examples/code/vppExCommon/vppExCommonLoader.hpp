#ifndef INC_VPPEXCOMMONLOADER_HPP
#define INC_VPPEXCOMMONLOADER_HPP

// -----------------------------------------------------------------------------

#include "../vppExCommon/vppExCommon.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// -----------------------------------------------------------------------------
namespace vppex {
// -----------------------------------------------------------------------------

class MeshLoader
{
public:
    MeshLoader();
    ~MeshLoader();

    struct Vertex
    {
        Vertex();

        Vertex (
            const glm::vec3& pos,
            const glm::vec2& tex,
            const glm::vec3& normal,
            const glm::vec3& tangent,
            const glm::vec3& bitangent,
            const glm::vec3& color );

	    glm::vec3 m_pos;
	    glm::vec2 m_tex;
	    glm::vec3 m_normal;
	    glm::vec3 m_color;
	    glm::vec3 m_tangent;
	    glm::vec3 m_binormal;
    };

    typedef std::vector< Vertex > Vertices;
    typedef std::vector< unsigned int > Indices;

    struct Mesh
    {
	    std::uint32_t m_numIndices;
	    std::uint32_t m_materialIndex;
	    std::uint32_t m_vertexBase;
	    Vertices m_vertices;
	    Indices m_indices;
    };

    typedef std::vector< Mesh > Meshes;

    bool loadMesh ( const std::string& filename );
    bool loadMesh ( const std::string& filename, int flags );

    const Meshes& meshes() const;

private:
    struct Impl;
    std::unique_ptr< Impl > impl;
};

// -----------------------------------------------------------------------------

inline MeshLoader::Vertex :: Vertex()
{
}

// -----------------------------------------------------------------------------

inline MeshLoader::Vertex :: Vertex (
    const glm::vec3& pos,
    const glm::vec2& tex,
    const glm::vec3& normal,
    const glm::vec3& tangent,
    const glm::vec3& bitangent,
    const glm::vec3& color ) :
	    m_pos ( pos ),
	    m_tex ( tex ),
	    m_normal ( normal ),
	    m_color ( color ),
	    m_tangent ( tangent ),
	    m_binormal ( bitangent )
{
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

typedef vpp::format< vpp::texture_format > TextureLoaderFmt;

typedef vpp::ImageAttributes<
    TextureLoaderFmt, vpp::RENDER, vpp::IMG_TYPE_2D,
    VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    VK_IMAGE_TILING_OPTIMAL, VK_SAMPLE_COUNT_1_BIT,
    true, false > TextureLoaderAttr;

typedef vpp::Image< TextureLoaderAttr > TextureLoaderImage;
typedef vpp::ImageViewAttributes< TextureLoaderImage > TextureLoaderViewAttr;
typedef vpp::ImageView< TextureLoaderViewAttr > TextureLoaderView;

// -----------------------------------------------------------------------------

class TextureLoader
{
public:
    TextureLoader ( const vpp::Device& hDevice );
    ~TextureLoader();

    struct Texture;

    void loadTexture ( std::string filename, VkFormat format, Texture* pTexture );

private:
    struct Impl;
    std::unique_ptr< Impl > impl;
};

// -----------------------------------------------------------------------------

struct TextureLoader::Texture
{
    vpp::NormalizedSampler m_sampler;
    TextureLoaderImage m_image;
    TextureLoaderView m_view;
    VkImageLayout m_layout;
	std::uint32_t m_width;
    std::uint32_t m_height;
	std::uint32_t m_mipLevels;
	std::uint32_t m_layerCount;
};

// -----------------------------------------------------------------------------
} // namespace vppex
// -----------------------------------------------------------------------------

#endif // INC_VPPEXCOMMONLOADER_HPP

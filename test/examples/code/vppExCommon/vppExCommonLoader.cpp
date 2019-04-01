// -----------------------------------------------------------------------------

#include <vppAll.hpp>
#include "vppExCommonLoader.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/cimport.h"

#include "glm/gtc/matrix_transform.hpp"
#include "gli/gli.hpp"

// -----------------------------------------------------------------------------
namespace vppex {
// -----------------------------------------------------------------------------

struct MeshLoader::Impl
{
    // external routines
    bool loadMesh ( const std::string& filename, int flags );

    // internal routines
    bool initFromScene ( const aiScene* pScene, const std::string& filename );
    void initMesh ( unsigned int index, const aiMesh* paiMesh, const aiScene* pScene );

    // data

	std::vector< Mesh > m_meshes;

	struct Dimension 
	{
		glm::vec3 min = glm::vec3(FLT_MAX);
		glm::vec3 max = glm::vec3(-FLT_MAX);
		glm::vec3 size;
	};

    Dimension m_dim;
	uint32_t m_numVertices = 0;

	Assimp::Importer m_importer;
	const aiScene* m_pScene;
};

// -----------------------------------------------------------------------------

bool MeshLoader::Impl :: loadMesh ( const std::string& filename, int flags )
{
	m_pScene = m_importer.ReadFile ( filename.c_str(), flags );

	if ( m_pScene )
	{
		return initFromScene ( m_pScene, filename );
	}
	else 
	{
		printf("Error parsing '%s': '%s'\n", filename.c_str(), m_importer.GetErrorString() );
		return false;
	}
}

// -----------------------------------------------------------------------------

bool MeshLoader::Impl :: initFromScene (
    const aiScene* pScene, const std::string& filename )
{
	m_meshes.resize ( pScene->mNumMeshes );

	// Counters
	for ( unsigned int i = 0; i < m_meshes.size(); i++ )
	{
		m_meshes[i].m_vertexBase = m_numVertices;
		m_numVertices += pScene->mMeshes[i]->mNumVertices;
	}

	// Initialize the meshes in the scene one by one
	for ( unsigned int i = 0; i < m_meshes.size(); i++ ) 
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		initMesh ( i, paiMesh, pScene );
	}

	return true;
}

// -----------------------------------------------------------------------------

void MeshLoader::Impl :: initMesh (
    unsigned int index, const aiMesh* paiMesh, const aiScene* pScene )
{
	m_meshes[index].m_materialIndex = paiMesh->mMaterialIndex;

	aiColor3D pColor(0.f, 0.f, 0.f);
	pScene->mMaterials[paiMesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, pColor);

	aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		aiVector3D* pPos = &(paiMesh->mVertices[i]);
		aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		aiVector3D *pTexCoord;
		if (paiMesh->HasTextureCoords(0))
		{
			pTexCoord = &(paiMesh->mTextureCoords[0][i]);
		}
		else {
			pTexCoord = &Zero3D;
		}
		aiVector3D* pTangent = (paiMesh->HasTangentsAndBitangents()) ? &(paiMesh->mTangents[i]) : &Zero3D;
		aiVector3D* pBiTangent = (paiMesh->HasTangentsAndBitangents()) ? &(paiMesh->mBitangents[i]) : &Zero3D;

		Vertex v(glm::vec3(pPos->x, -pPos->y, pPos->z), 
			glm::vec2(pTexCoord->x , pTexCoord->y),
			glm::vec3(pNormal->x, pNormal->y, pNormal->z),
			glm::vec3(pTangent->x, pTangent->y, pTangent->z),
			glm::vec3(pBiTangent->x, pBiTangent->y, pBiTangent->z),
			glm::vec3(pColor.r, pColor.g, pColor.b)
			);

		m_dim.max.x = fmax(pPos->x, m_dim.max.x);
		m_dim.max.y = fmax(pPos->y, m_dim.max.y);
		m_dim.max.z = fmax(pPos->z, m_dim.max.z);

		m_dim.min.x = fmin(pPos->x, m_dim.min.x);
		m_dim.min.y = fmin(pPos->y, m_dim.min.y);
		m_dim.min.z = fmin(pPos->z, m_dim.min.z);

		m_meshes[index].m_vertices.push_back(v);
	}

	m_dim.size = m_dim.max - m_dim.min;

	for ( unsigned int i = 0; i < paiMesh->mNumFaces; i++ ) 
	{
		const aiFace& Face = paiMesh->mFaces[i];
		if (Face.mNumIndices != 3)
			continue;
		m_meshes[index].m_indices.push_back(Face.mIndices[0]);
		m_meshes[index].m_indices.push_back(Face.mIndices[1]);
		m_meshes[index].m_indices.push_back(Face.mIndices[2]);
	}
}

// -----------------------------------------------------------------------------

MeshLoader :: MeshLoader() :
    impl ( new Impl() )
{
}

// -----------------------------------------------------------------------------

MeshLoader :: ~MeshLoader()
{
}

// -----------------------------------------------------------------------------

bool MeshLoader :: loadMesh ( const std::string& filename )
{
	const int flags = aiProcess_FlipWindingOrder | aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals;
	return impl->loadMesh ( filename, flags );
}

// -----------------------------------------------------------------------------

bool MeshLoader :: loadMesh ( const std::string& filename, int flags )
{
	return impl->loadMesh ( filename, flags );
}

// -----------------------------------------------------------------------------

const MeshLoader::Meshes& MeshLoader :: meshes() const
{
    return impl->m_meshes;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct TextureLoader::Impl
{
    // ctor
	Impl ( const vpp::Device& hDevice );

    // external ops
	void loadTexture ( std::string filename, VkFormat format, Texture* pTexture );

    // data
    vpp::PhysicalDevice m_physicalDevice;
    vpp::Device m_device;
};

// -----------------------------------------------------------------------------

TextureLoader::Impl :: Impl ( const vpp::Device& hDevice ) :
    m_physicalDevice ( hDevice.physical() ),
    m_device ( hDevice )
{
}

// -----------------------------------------------------------------------------

void TextureLoader::Impl :: loadTexture (
    std::string filename, VkFormat format, Texture* pTexture )
{
    using namespace vpp;

	gli::texture2D tex2D ( gli::load ( filename.c_str() ) );

	assert(!tex2D.empty());

	pTexture->m_width = static_cast< std::uint32_t >( tex2D[0].dimensions().x );
	pTexture->m_height = static_cast< std::uint32_t >( tex2D[0].dimensions().y );
	pTexture->m_mipLevels = static_cast< std::uint32_t >( tex2D.levels() );

    VkFormatFeatureFlags ff = m_physicalDevice.supportsFormat (
        format, PhysicalDevice::LINEAR_TILING );

    typedef Buffer< Buf::SOURCE > StagingBuffer;

    StagingBuffer stagingBuffer ( tex2D.size(), m_device );
    auto stagingBufferMemory = bindMappableMemory ( stagingBuffer, MemProfile::HOST_STATIC );

    stagingBufferMemory.memory().map();
    memcpy ( stagingBufferMemory.memory().beginMapped(), tex2D.data(), tex2D.size() );
    stagingBufferMemory.memory().unmap();

	// Setup buffer copy regions for each mip level
	std::vector< VkBufferImageCopy > bufferCopyRegions;
	uint32_t offset = 0;

	for ( uint32_t i = 0; i < pTexture->m_mipLevels; i++ )
	{
		VkBufferImageCopy bufferCopyRegion = {};
		bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		bufferCopyRegion.imageSubresource.mipLevel = i;
		bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
		bufferCopyRegion.imageSubresource.layerCount = 1;
		bufferCopyRegion.imageExtent.width = static_cast<uint32_t>(tex2D[i].dimensions().x);
		bufferCopyRegion.imageExtent.height = static_cast<uint32_t>(tex2D[i].dimensions().y);
		bufferCopyRegion.imageExtent.depth = 1;
		bufferCopyRegion.bufferOffset = offset;

		bufferCopyRegions.push_back(bufferCopyRegion);

		offset += static_cast<uint32_t>(tex2D[i].size());
	}

    TextureLoaderImage loadedImage (
        VK_FORMAT_BC3_UNORM_BLOCK,
        VkExtent3D { pTexture->m_width, pTexture->m_height, 1 },
        MemProfile::DEVICE_STATIC,
        m_device,
        pTexture->m_mipLevels );

    CopyImageToDevice copyImg ( stagingBuffer, loadedImage, bufferCopyRegions );
    copyImg.execute ( NO_TIMEOUT );
	
    SNormalizedSampler samplerInfo;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    samplerInfo.compareOp = VK_COMPARE_OP_NEVER;
    samplerInfo.compare = false;
    samplerInfo.magFilterMode = VK_FILTER_LINEAR;
    samplerInfo.minFilterMode = VK_FILTER_LINEAR;
    samplerInfo.mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.anisotropy = true;
    samplerInfo.mipLodBias;
    samplerInfo.maxAnisotropy = 8;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = (float) pTexture->m_mipLevels;

    NormalizedSampler sampler = NormalizedSampler ( m_device, samplerInfo );
	TextureLoaderView imageView ( loadedImage );

    pTexture->m_image = loadedImage;
    pTexture->m_view = imageView;
    pTexture->m_sampler = sampler;
}

// -----------------------------------------------------------------------------

TextureLoader :: TextureLoader ( const vpp::Device& hDevice ) :
    impl ( new Impl ( hDevice ) )
{
}

// -----------------------------------------------------------------------------

TextureLoader :: ~TextureLoader()
{
}

// -----------------------------------------------------------------------------

void TextureLoader :: loadTexture (
    std::string filename, VkFormat format, Texture* pTexture )
{
	impl->loadTexture ( filename, format, pTexture );
}

// -----------------------------------------------------------------------------
} // namespace vppex
// -----------------------------------------------------------------------------

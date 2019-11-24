#include "GLBModel.h"
#include "Utility/IO/GLBLoader.h"
#include "Utility/Debug.h"

namespace Framework::DX {

    GLBModel::GLBModel(const std::filesystem::path& path) {
        auto batch = [](const Utility::PositionList& positions,
            const Utility::NormalList& normals,
            const Utility::UVList& uvs,
            const Utility::TangentList& tangents) {
                const size_t listSize = positions.size();
                std::vector<Vertex> result(listSize);
                MY_ASSERTION(normals.size() == listSize
                    && uvs.size() == listSize
                    && tangents.size() == listSize,
                    "要素数が不正です");
                for (size_t n = 0; n < listSize; n++) {
                    result[n].position = positions[n];
                    result[n].normal = normals[n];
                    result[n].uv = uvs[n];
                    result[n].tangent = tangents[n];
                }
                return result;
        };

        Utility::GLBLoader glbLoader(path.c_str());
        auto indicesList = glbLoader.getIndicesPerSubMeshes();
        auto positionsList = glbLoader.getPositionsPerSubMeshes();
        auto normalsList = glbLoader.getNormalsPerSubMeshes();
        auto uvsList = glbLoader.getUVsPerSubMeshes();
        auto tangentsList = glbLoader.getTangentsPerSubMeshes();

        const size_t indicesListSize = indicesList.size();
        MY_ASSERTION(positionsList.size() == indicesListSize
            && normalsList.size() == indicesListSize
            && uvsList.size() == indicesListSize
            && tangentsList.size() == indicesListSize,
            "リストのサイズが不正です");
        mSubMeshes.resize(indicesList.size());

        for (size_t n = 0; n < indicesListSize; n++) {
            mSubMeshes[n].indices = indicesList[n];
            mSubMeshes[n].vertices = batch(positionsList[n], normalsList[n], uvsList[n], tangentsList[n]);

            mSubMeshes[n].materialID = 0;
        }
    }

    GLBModel::~GLBModel() { }

} //Framework::DX

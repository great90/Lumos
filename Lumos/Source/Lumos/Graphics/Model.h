#pragma once
#include "MeshFactory.h"
#include "Core/Asset.h"
#include "Utilities/TimeStep.h"

namespace Lumos
{
    class AStar;
    namespace Graphics
    {
        class Skeleton;
        class Animation;
        class AnimationController;
        struct SamplingContext;
        class Mesh;

        class Model : public Asset
        {
            template <typename Archive>
            friend void save(Archive& archive, const Model& model);

            template <typename Archive>
            friend void load(Archive& archive, Model& model);

        public:
            Model();
            Model(const std::string& filePath);
            Model(const SharedPtr<Mesh>& mesh, PrimitiveType type);
            Model(PrimitiveType type);

            ~Model();

            std::vector<SharedPtr<Mesh>>& GetMeshesRef();
            const std::vector<SharedPtr<Mesh>>& GetMeshes() const;
            void AddMesh(SharedPtr<Mesh> mesh);

            SharedPtr<Skeleton> GetSkeleton() const;
            const std::vector<SharedPtr<Animation>>& GetAnimations() const;
            SharedPtr<SamplingContext> GetSamplingContext() const;
            SharedPtr<AnimationController> GetAnimationController() const;

            uint32_t GetCurrentAnimationIndex() const { return m_CurrentAnimation; }
            void SetCurrentAnimationIndex(uint32_t index) { m_CurrentAnimation = index; }

            const std::string& GetFilePath() const { return m_FilePath; }
            PrimitiveType GetPrimitiveType() { return m_PrimitiveType; }
            void SetPrimitiveType(PrimitiveType type) { m_PrimitiveType = type; }
            SET_ASSET_TYPE(AssetType::Model);

            void UpdateAnimation(const TimeStep& dt);
            void UpdateAnimation(const TimeStep& dt, float overrideTime);

            std::vector<glm::mat4> GetJointMatrices();

            Model(const Model&);
            Model& operator=(const Model&);
            Model(Model&&);
            Model& operator=(Model&&);

        private:
            PrimitiveType m_PrimitiveType = PrimitiveType::None;
            std::vector<SharedPtr<Mesh>> m_Meshes;
            std::string m_FilePath;
            std::vector<String8> m_AnimFilePaths;

            SharedPtr<Skeleton> m_Skeleton;
            std::vector<SharedPtr<Animation>> m_Animation;
            SharedPtr<SamplingContext> m_SamplingContext;
            SharedPtr<AnimationController> m_AnimationController;

            uint32_t m_CurrentAnimation = 0;

            std::vector<glm::mat4> m_BindPoses;

            void LoadOBJ(const std::string& path);
            void LoadGLTF(const std::string& path);
            void LoadFBX(const std::string& path);

        public:
            void LoadModel(const std::string& path);
        };
    }
}

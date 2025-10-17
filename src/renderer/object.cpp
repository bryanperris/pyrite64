/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#include "object.h"
#include "../context.h"

#include "glm/ext/matrix_transform.hpp"

extern SDL_GPUSampler *texSamplerRepeat;

void Renderer::Object::draw(SDL_GPURenderPass* pass, SDL_GPUCommandBuffer* cmdBuff) {
  if (!mesh) return;

  if (transformDirty) {
    auto m = glm::identity<glm::mat4>();
    m = glm::scale(m, {0.1f, 0.1f, 0.1f});
    m = glm::translate(m, pos);
    uniform.modelMat = m;
    transformDirty = false;
  }

  SDL_PushGPUVertexUniformData(cmdBuff, 1, &uniform, sizeof(uniform));

  SDL_GPUTexture *tex{nullptr};
  auto assets = ctx.project->getAssets().getEntries();
  for (auto &asset : assets) {
    if (asset.type == Project::AssetManager::FileType::IMAGE) {
      tex = asset.texture->getGPUTex();
      break;
    }
  }

  SDL_GPUTextureSamplerBinding bind{
    .texture = tex,
    .sampler = texSamplerRepeat
  };
  SDL_BindGPUFragmentSamplers(pass, 0, &bind, 1);

  mesh->draw(pass);
}

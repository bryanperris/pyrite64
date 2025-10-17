/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma once
#include <string>
#include <vector>

#include "vertex.h"
#include "SDL3/SDL_gpu.h"

namespace Renderer
{
  class VertBuffer
  {
    private:
      SDL_GPUDevice* gpuDevice{nullptr};

      SDL_GPUBuffer* buffer{nullptr};
      SDL_GPUTransferBuffer* bufferTrans{nullptr};

      SDL_GPUBuffer* bufferIdx{nullptr};
      SDL_GPUTransferBuffer* bufferIdxTrans{nullptr};

      size_t currVertByteSize{0};
      size_t currIdxByteSize{0};
      bool needsUpload{false};

      void resize(uint32_t sizeVert, uint32_t sizeIndex);

    public:
      VertBuffer(SDL_GPUDevice* device);
      ~VertBuffer();

      void setData(
        const std::vector<Vertex> &vertices,
        const std::vector<uint16_t> &indices
      );

      void upload(SDL_GPUCopyPass& pass);

      void addBindings(SDL_GPUBufferBinding binding[]) const {
        binding[0].buffer = buffer;
        binding[0].offset = 0;

        binding[1].buffer = bufferIdx;
        binding[1].offset = 0;
      }
  };
}
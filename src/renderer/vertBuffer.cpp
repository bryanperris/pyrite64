/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#include "vertBuffer.h"

Renderer::VertBuffer::VertBuffer(SDL_GPUDevice* device)
  : gpuDevice{device}
{
}

Renderer::VertBuffer::~VertBuffer()
{
  SDL_ReleaseGPUBuffer(gpuDevice, buffer);
  SDL_ReleaseGPUTransferBuffer(gpuDevice, bufferTrans);

  SDL_ReleaseGPUBuffer(gpuDevice, bufferIdx);
  SDL_ReleaseGPUTransferBuffer(gpuDevice, bufferIdxTrans);
}

void Renderer::VertBuffer::resize(uint32_t sizeVert, uint32_t sizeIndex) {
  assert(sizeVert != 0);
  assert(sizeIndex != 0);

  SDL_GPUBufferCreateInfo bufferInfo{
    .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
    .size = sizeVert
  };
  buffer = SDL_CreateGPUBuffer(gpuDevice, &bufferInfo);
  assert(buffer != nullptr);

  SDL_GPUTransferBufferCreateInfo transferInfo{
    .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
    .size = sizeVert
  };
  bufferTrans = SDL_CreateGPUTransferBuffer(gpuDevice, &transferInfo);
  assert(bufferTrans != nullptr);

  bufferInfo.usage = SDL_GPU_BUFFERUSAGE_INDEX;
  bufferInfo.size = sizeIndex;
  bufferIdx = SDL_CreateGPUBuffer(gpuDevice, &bufferInfo);
  assert(bufferIdx != nullptr);

  transferInfo = {
    .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
    .size = sizeIndex
  };
  bufferIdxTrans = SDL_CreateGPUTransferBuffer(gpuDevice, &transferInfo);
  assert(bufferIdxTrans != nullptr);
}

void Renderer::VertBuffer::setData(
  const std::vector<Vertex>&vertices,
  const std::vector<uint16_t> &indices
) {
  currIdxByteSize = indices.size() * sizeof(uint16_t);

  auto newByteSize = vertices.size() * sizeof(Vertex);
  if (newByteSize != currVertByteSize) {
    resize(newByteSize, currIdxByteSize);
  }
  currVertByteSize = newByteSize;

// @TODO: store vert/indices in one single buffer

  auto data = (Vertex*)SDL_MapGPUTransferBuffer(gpuDevice, bufferTrans, false);
  SDL_memcpy(data, vertices.data(), currVertByteSize);
  SDL_UnmapGPUTransferBuffer(gpuDevice, bufferTrans);

  data = (Vertex*)SDL_MapGPUTransferBuffer(gpuDevice, bufferIdxTrans, false);
  SDL_memcpy(data, indices.data(), currIdxByteSize);
  SDL_UnmapGPUTransferBuffer(gpuDevice, bufferIdxTrans);

  needsUpload = true;
}

void Renderer::VertBuffer::upload(SDL_GPUCopyPass &pass) {
  if (!needsUpload || currVertByteSize == 0)return;

  SDL_GPUTransferBufferLocation location{};
  location.transfer_buffer = bufferTrans;
  location.offset = 0;

  SDL_GPUBufferRegion region{};
  region.buffer = buffer;
  region.size = currVertByteSize;
  region.offset = 0;

  SDL_UploadToGPUBuffer(&pass, &location, &region, true);

  location = {};
  location.transfer_buffer = bufferIdxTrans;
  region = {};
  region.buffer = bufferIdx;
  region.size = currIdxByteSize;

  SDL_UploadToGPUBuffer(&pass, &location, &region, true);

  needsUpload = false;
}

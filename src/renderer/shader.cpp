/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#include "shader.h"

#include "SDL3/SDL_gpu.h"
#include "SDL3/SDL_iostream.h"

Renderer::Shader::Shader(SDL_GPUDevice* device, const Config &conf)
  : gpuDevice{device}
{
  std::string pathVert = "data/shader/" + conf.name + ".vert.spv";
  std::string pathFrag = "data/shader/" + conf.name + ".frag.spv";

  size_t vertexCodeSize;
  void* vertexCode = SDL_LoadFile(pathVert.c_str(), &vertexCodeSize);

  size_t fragmentCodeSize;
  void* fragmentCode = SDL_LoadFile(pathFrag.c_str(), &fragmentCodeSize);

  SDL_GPUShaderCreateInfo vertexInfo{};
  vertexInfo.code = (Uint8*)vertexCode;
  vertexInfo.code_size = vertexCodeSize;
  vertexInfo.entrypoint = "main";
  vertexInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
  vertexInfo.stage = SDL_GPU_SHADERSTAGE_VERTEX;
  vertexInfo.num_samplers = conf.vertTexCount;
  vertexInfo.num_storage_buffers = 0;
  vertexInfo.num_storage_textures = 0;
  vertexInfo.num_uniform_buffers = conf.vertUboCount;
  shaderVert = SDL_CreateGPUShader(device, &vertexInfo);

  // create the fragment shader
  SDL_GPUShaderCreateInfo fragmentInfo{};
  fragmentInfo.code = (Uint8*)fragmentCode;
  fragmentInfo.code_size = fragmentCodeSize;
  fragmentInfo.entrypoint = "main";
  fragmentInfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
  fragmentInfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
  fragmentInfo.num_samplers = conf.fragTexCount;
  fragmentInfo.num_storage_buffers = 0;
  fragmentInfo.num_storage_textures = 0;
  fragmentInfo.num_uniform_buffers = conf.fragUboCount;

  shaderFrag = SDL_CreateGPUShader(gpuDevice, &fragmentInfo);

  SDL_free(fragmentCode);
  SDL_free(vertexCode);
}

Renderer::Shader::~Shader()
{
  SDL_ReleaseGPUShader(gpuDevice, shaderVert);
  SDL_ReleaseGPUShader(gpuDevice, shaderFrag);
}

/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma once
#include <libdragon.h>

namespace P64
{
  class Object
  {
    private:

    public:
      struct CompRef
      {
        uint8_t type{};
        uint8_t flags{};
        uint16_t offset{};
      };

      ~Object();

      uint16_t id{};
      uint16_t compCount{0};

      // extra data, is overlapping with component data if unused
      fm_vec3_t pos{};

      // component references, this is then also followed by a buffer for the actual data
      // the object allocation logic keeps extra space to fit everything

      //CompRef compRefs[];
      //uint8_t compData[];

      [[nodiscard]] CompRef* getCompRefs() const {
        return (CompRef*)((uint8_t*)this + sizeof(Object));
      }

      [[nodiscard]] char* getCompData() const {
        return (char*)getCompRefs() + sizeof(CompRef) * compCount;
      }
  };
}
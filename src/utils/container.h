/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma once

namespace Utils
{
  template<typename T>
  class RequestVal
  {
    private:
      int state{0};
      T result{};

    public:
      void request() {
        state = 1;
      }

      bool isRequested() const {
        return state == 1;
      }
      void setResult(const T &res) {
        result = res;
        state = 2;
      }
      bool hasResult() const {
        return state == 2;
      }

      T consume() {
        state = 0;
        return result;
      }
  };
}
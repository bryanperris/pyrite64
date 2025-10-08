// NOTE: Auto-Generated File!

#include "script/scriptTable.h"

namespace P64::Script
{
__CODE_DECL__

  FuncUpdate codeTable[] = {
__CODE_ENTRIES__
  };

  FuncUpdate getCodeByIndex(uint32_t idx)
  {
    if (idx < sizeof(codeTable)/sizeof(codeTable[0])) {
      return codeTable[idx];
    }
    return nullptr;
  }
}

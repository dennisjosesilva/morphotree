#pragma once

#include <numeric>

namespace morphotree 
{

  template<typename T>
  bool isLowSizeType();

  template<typename T>
  bool is8BitSizeType();
 
// ===============[ IMPLEMENTATION ] ================================================= 

  template<typename T>
  bool isLowSizeType() 
  {
    return std::is_same<T, bool>::value || 
      std::is_same<T, uint8>::value || std::is_same<T, int8>::value ||
      std::is_same<T, uint16>::value || std::is_same<T, int16>::value ||
      std::is_same<T, uint32>::value || std::is_same<T, int32>::value;
  }

  template<typename T>
  bool isUnsignedInteger8BitSizeType()
  {
    return std::is_same<T, uint8>::value;
  }
}
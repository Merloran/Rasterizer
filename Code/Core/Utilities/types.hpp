#pragma once
// Rename to be consistent with naming_convention
using Bool	  = bool;
using Void	  = void;

using Char   = char;
using WChar  = wchar_t;
using Char8  = char8_t;
using Char16 = char16_t;
using Char32 = char32_t;
              
using Int8	  = int8_t;
using Int16	  = int16_t;
using Int32	  = int32_t;
using Int64	  = int64_t;
              
using UInt8	  = uint8_t;
using UInt16  = uint16_t;
using UInt32  = uint32_t;
using UInt64  = uint64_t;
              
using Float32 = float;
using Float64 = double;

template <typename Type>
using DynamicArray = std::vector<Type>;

using String = std::string;

using FVector3 = glm::fvec3;
using FVector2 = glm::fvec2;
using FVector4 = glm::fvec4;

using IVector2 = glm::ivec2;
#pragma once

#include "ident.h"
#include "value.h"

#include <ffi.h>
#include <unordered_map>
#include <vbci.h>
#include <vector>

namespace vbci
{
  struct Field
  {
    size_t offset;
    size_t size;
    TypeId type_id;
    ValueType value_type;
  };

  struct Class
  {
    size_t size;
    size_t debug_info;
    TypeId type_id;
    Object* singleton;

    // Precalculate an offset into the object for each field name.
    std::unordered_map<size_t, size_t> field_map;
    std::vector<Field> fields;

    // Precalculate a function pointer for each method name.
    std::unordered_map<size_t, Function*> methods;

    bool calc_size();
    Function* finalizer();
    Function* method(size_t w);
  };
}

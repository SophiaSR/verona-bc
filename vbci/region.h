#pragma once

#include "classes.h"
#include "ident.h"
#include "location.h"

#include <vbci.h>

namespace vbci
{
  struct Region
  {
  private:
    Location parent;
    RC stack_rc;

  protected:
    Region() : parent(0), stack_rc(0) {}
    virtual ~Region() = default;

  public:
    static Region* create(RegionType type);

    virtual Object* object(Class& cls) = 0;
    virtual Array* array(uint32_t type_id, size_t size) = 0;
    virtual void rfree(Header* h) = 0;
    virtual void insert(Header* h) = 0;
    virtual void remove(Header* h) = 0;
    virtual bool enable_rc() = 0;

    void stack_inc(RC inc = 1)
    {
      // If we transition from 0 to 1, we need to increment the parent RC.
      if ((stack_rc == 0) && loc::is_region(parent))
        loc::to_region(parent)->stack_inc();

      stack_rc += inc;
    }

    bool stack_dec()
    {
      if (--stack_rc == 0)
      {
        if (!has_parent())
        {
          // Returns false if the region has been freed.
          free_region();
          return false;
        }

        // If we transition from 1 to 0, we need to decrement the parent RC.
        if (loc::is_region(parent))
          loc::to_region(parent)->stack_dec();
      }

      return true;
    }

    bool is_ancestor_of(Region* r)
    {
      while (loc::is_region(r->parent))
      {
        r = loc::to_region(r->parent);

        if (r == this)
          return true;
      }

      return false;
    }

    bool sendable()
    {
      assert(stack_rc > 0);
      return !has_parent() && (stack_rc == 1);
    }

    bool has_parent()
    {
      return parent != loc::None;
    }

    Location get_parent()
    {
      return parent;
    }

    bool is_frame_local()
    {
      return loc::is_stack(parent);
    }

    void set_frame_id(Location frame_id)
    {
      assert(!has_parent());
      assert(stack_rc > 0);
      parent = frame_id;
    }

    void set_parent(Region* r)
    {
      assert(!has_parent());
      parent = Location(r);

      if (stack_rc > 0)
        r->stack_inc();
    }

    void set_parent()
    {
      assert(!has_parent());
      parent = loc::Immutable;
    }

    void clear_parent()
    {
      assert(has_parent());
      assert(stack_rc > 0);

      if (loc::is_region(parent))
        loc::to_region(parent)->stack_dec();

      parent = loc::None;
    }

    void free_region()
    {
      assert(!has_parent());
      assert(stack_rc == 0);
      free_contents();
      delete this;
    }

  private:
    virtual void free_contents() = 0;
  };
}

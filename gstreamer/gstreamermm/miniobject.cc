/* gstreamermm - a C++ wrapper for gstreamer
 *
 * Copyright 2008 The gstreamermm Development Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <gstreamermm/miniobject.h>
#include <gstreamermm/private/miniobject_p.h>
#include <gstreamermm/wrap.h>

namespace Gst
{

  /**** Gst::MiniObject_Class *********************************************/

const Glib::Class& MiniObject_Class::init()
{
  if(!gtype_)
  {
    class_init_func_ = &MiniObject_Class::class_init_function;
    register_derived_type(GST_TYPE_MINI_OBJECT);
  }

  return *this;
}

void MiniObject_Class::class_init_function(void* g_class, void* /*class_data*/)
{
  BaseClassType* const klass = static_cast<BaseClassType*>(g_class);

  klass->copy = &copy_vfunc_callback;
  klass->finalize = &finalize_vfunc_callback;
}

MiniObject* MiniObject_Class::wrap_new(GstMiniObject* object)
{
  return new MiniObject(object);
}


/**** Gst::MiniObject *****************************************************/

// static data
MiniObject::CppClassType MiniObject::mini_object_class_;

MiniObject::MiniObject()
: gobject_(0)
{}

MiniObject::MiniObject(const Glib::Class& mini_object_class)
: gobject_(gst_mini_object_new(mini_object_class.get_type()))
{}

MiniObject::MiniObject(GstMiniObject* castitem, bool take_copy)
: gobject_(take_copy ? gst_mini_object_copy(castitem) : castitem)
{}

GstMiniObject* MiniObject::gobj_copy()
{
  // Use the mini object copying function to get a copy of the underlying
  // gobject instead of referencing and returning the underlying gobject as
  // would be done normally:
  return gst_mini_object_copy(gobj());
}

MiniObject::~MiniObject()
{
  // The value of the reference count is checked so that if this mini object is
  // being destroyed as a result of weak reference notification no
  // unreferencing is done and thus no error is issued on unreferencing a mini
  // object with a reference of 0.
  if(gobject_ && GST_MINI_OBJECT_REFCOUNT_VALUE(gobject_) > 0)
    gst_mini_object_unref(gobject_);
}

GType MiniObject::get_type()
{
  return mini_object_class_.init().get_type();
}

GType MiniObject::get_base_type()
{
  return GST_TYPE_MINI_OBJECT;
}

void MiniObject::swap(MiniObject& other)
{
  GstMiniObject *const temp = gobject_;
  gobject_ = other.gobject_;
  other.gobject_ = temp;
}

void 
MiniObject::reference() const
{
  gst_mini_object_ref(gobject_);
}

void
MiniObject::unreference() const
{
  gst_mini_object_unref(gobject_);
}

guint MiniObject::get_flags() const
{
  return GST_MINI_OBJECT_FLAGS(gobj());
}

bool MiniObject::flag_is_set(guint flag) const
{
  return GST_MINI_OBJECT_FLAG_IS_SET(gobj(), flag);
}

void MiniObject::set_flag(guint flag)
{
  GST_MINI_OBJECT_FLAG_SET(gobj(), flag);
}

void MiniObject::unset_flag(guint flag)
{
  GST_MINI_OBJECT_FLAG_UNSET(gobj(), flag);
}

bool
MiniObject::is_writable() const
{
  return gst_mini_object_is_writable(gobject_);
}

Glib::RefPtr<Gst::MiniObject> MiniObject::create_writable()
{
  return Gst::wrap(gst_mini_object_make_writable(gobject_));
}

GstMiniObject* MiniObject_Class::copy_vfunc_callback(const GstMiniObject* self)
{
  BaseClassType *const base = static_cast<BaseClassType*>(
      g_type_class_peek_parent(GST_MINI_OBJECT_GET_CLASS(self)) // Get the parent class of the object class (The original underlying C class).
  );

  // Call the original underlying C function:
  if(base && base->copy)
    return (*base->copy)(self);

  return static_cast<GstMiniObject*>(0);
}
Glib::RefPtr<Gst::MiniObject> Gst::MiniObject::copy_vfunc() const
{
  BaseClassType *const base = static_cast<BaseClassType*>(
      g_type_class_peek_parent(GST_MINI_OBJECT_GET_CLASS(gobj())) // Get the parent class of the object class (The original underlying C class).
  );

  if(base && base->copy)
    return Gst::wrap((*base->copy)(gobj()));

  return Glib::RefPtr<Gst::MiniObject>(0);
}
void MiniObject_Class::finalize_vfunc_callback(GstMiniObject* self)
{
  BaseClassType *const base = static_cast<BaseClassType*>(
      g_type_class_peek_parent(GST_MINI_OBJECT_GET_CLASS(self)) // Get the parent class of the object class (The original underlying C class).
  );

  // Call the original underlying C function:
  if(base && base->finalize)
    (*base->finalize)(self);

}
void Gst::MiniObject::finalize_vfunc() 
{
  BaseClassType *const base = static_cast<BaseClassType*>(
      g_type_class_peek_parent(GST_MINI_OBJECT_GET_CLASS(gobj())) // Get the parent class of the object class (The original underlying C class).
  );

  if(base && base->finalize)
    (*base->finalize)(gobj());
}

} //namespace Gst

/*
namespace Glib
{

Glib::RefPtr<Gst::MiniObject> wrap(GstMiniObject* object, bool take_copy)
{
  return Glib::RefPtr<Gst::MiniObject>(new MiniObject(object, take_copy));
}

} //namespace Glib

*/

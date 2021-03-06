/*
 * test2.cpp
 *
 *  Created on: Feb 22, 2013
 *      Author: t.lakota
 */

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

#include <gstreamermm.h>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <gstreamermm/appsrc.h>
#include <gstreamermm/appsink.h>
#include <cstring>

//this is a bit hacky, but for now necessary for Gst::Element_Class::class_init_function which is used by register_mm_type
#include <gstreamermm/private/element_p.h>

class Foo : public Gst::Element
{
  Glib::RefPtr<Gst::Pad> sinkpad;
  Glib::RefPtr<Gst::Pad> srcpad;

public:
  static void base_init(BaseClassType *klass)
  {
    /* This is another hack.
     * For now it uses pure C functions, which should be wrapped then.
     */
    gst_element_class_set_details_simple(klass, "foo_longname",
        "foo_classification", "foo_detail_description", "foo_detail_author");

    gst_element_class_add_pad_template(klass,
        Gst::PadTemplate::create("sink", Gst::PAD_SINK, Gst::PAD_ALWAYS,
            Gst::Caps::create_any())->gobj());
    gst_element_class_add_pad_template(klass,
        Gst::PadTemplate::create("src", Gst::PAD_SRC, Gst::PAD_ALWAYS,
            Gst::Caps::create_any())->gobj());
  }

  Gst::FlowReturn chain(const Glib::RefPtr<Gst::Pad> &pad, Glib::RefPtr<Gst::Buffer> &buf)
  {
    assert(buf->gobj()->mini_object.refcount==1);
    buf = buf->create_writable();
    //run some C++ function
    std::sort(buf->get_data(), buf->get_data() + buf->get_size());
    assert(buf->gobj()->mini_object.refcount==1);
    return srcpad->push(buf);
  }
  explicit Foo(GstElement *gobj) :
      Gst::Element(gobj)
  {
    add_pad(sinkpad = Gst::Pad::create(get_pad_template("sink"), "sink"));
    add_pad(srcpad = Gst::Pad::create(get_pad_template("src"), "src"));
    sinkpad->set_chain_function(sigc::mem_fun(*this, &Foo::chain));
  }
  ~Foo()
  {
    printf("destroying foo\n");
  }
};

bool register_foo(Glib::RefPtr<Gst::Plugin> plugin)
{
  Gst::ElementFactory::register_element(plugin, "foomm", 10,
      Gst::register_mm_type<Foo>("foomm"));
  return true;

}

int main(int argc, char** argv)
{
  Gst::init(argc, argv);
  Gst::Plugin::register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR, "foo",
      "foo is example of C++ element", sigc::ptr_fun(register_foo), "0.123",
      "LGPL", "source?", "package?", "http://example.com");

  Glib::RefPtr<Gst::Pipeline> pipeline;

  pipeline = Gst::Pipeline::create("my-pipeline");

  Glib::RefPtr<Gst::AppSrc> source = Gst::AppSrc::create("source");
  Glib::RefPtr<Gst::Element> filter1 = Gst::ElementFactory::create_element(
      "foomm", "filter1");
  Glib::RefPtr<Gst::AppSink> sink = Gst::AppSink::create("sink");

  assert(source);
  assert(filter1);
  assert(sink);

  pipeline->add(source)->add(filter1)->add(sink);
  source->link(filter1)->link(sink);

  pipeline->set_state(Gst::STATE_PLAYING);

  std::cout << "pushing buffer" << std::endl;
  std::vector<guint8> data;
  data.push_back(1);
  data.push_back(5);
  data.push_back(2);
  data.push_back(4);
  Glib::RefPtr<Gst::Buffer> buf = Gst::Buffer::create(data.size());
  std::copy(data.begin(), data.end(), buf->get_data());
  source->push_buffer(buf);

  std::cout << "pulling buffer" << std::endl;
  Glib::RefPtr<Gst::Buffer> buf_out;
  buf_out = sink->pull_buffer();
  assert(buf_out);
  assert(buf_out->get_data());
  std::sort(data.begin(), data.end());
  assert(std::equal(data.begin(), data.end(), buf_out->get_data()));

  std::cout << "finishing stream" << std::endl;
  source->end_of_stream();

  std::cout << "waiting for eos or error" << std::endl;
  Glib::RefPtr<Gst::Message> msg = pipeline->get_bus()->poll(
      (Gst::MessageType) (Gst::MESSAGE_EOS | Gst::MESSAGE_ERROR),
      1 * Gst::SECOND);
  assert(msg);
  assert(msg->get_message_type() == Gst::MESSAGE_EOS);
  std::cout << "shutting down" << std::endl;

  pipeline->set_state(Gst::STATE_NULL);
  pipeline.reset();
  filter1.reset();

  return 0;
}


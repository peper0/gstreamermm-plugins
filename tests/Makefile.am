INCLUDES=-I$(top_srcdir) -I$(top_srcdir)/gst @GSTMM_CFLAGS@

LDADD=$(top_builddir)/gst/gstmm/libgstmm-1.0.la

noinst_PROGRAMS = test-create-element test-pipeline-add-element test-link-elements \
                  test-create-bin test-message-wrap test-miniobject-wrap

test_create_element_SOURCES=test-create-element.cc
test_create_element_LDFLAGS=@GSTMM_LIBS@

test_pipeline_add_element_SOURCES=test-pipeline-add-element.cc
test_pipeline_add_element_LDFLAGS=@GSTMM_LIBS@

test_link_elements_SOURCES=test-link-elements.cc
test_link_elements_LDFLAGS=@GSTMM_LIBS@

test_create_bin_SOURCES=test-create-bin.cc
test_create_bin_LDFLAGS=@GSTMM_LIBS@

test_miniobject_wrap_SOURCES=test-miniobject-wrap.cc
test_miniobject_wrap_LDFLAGS=@GSTMM_LIBS@

test_message_wrap_SOURCES=test-message-wrap.cc
test_message_wrap_LDFLAGS=@GSTMM_LIBS@

#runtestbasic runtestlangs \
#runtestsearch runtestmimetypes \
#runtestgetbuffer

#runtestbasic_SOURCES=test-basic.cc
#runtestbasic_LDFLAGS=@GSTMM_LIBS@

#runtestlangs_SOURCES=test-langs.cc
#runtestlangs_LDFLAGS=@GSTMM_LIBS@

#runtestsearch_SOURCES=test-search.cc
#runtestsearch_LDFLAGS=@GSTMM_LIBS@

#runtestmimetypes_SOURCES=test-mime-types.cc
#runtestmimetypes_LDFLAGS=@GSTMM_LIBS@

#runtestgetbuffer_SOURCES=test-get-buffer.cc
#runtestgetbuffer_LDFLAGS=@GSTMM_LIBS@



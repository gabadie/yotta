
LIBHEADER_PRODUCT := $(call product_create,BINHEADERS,headers)
LIBHEADER_TARGET := $(call product_target,$(LIBHEADER_PRODUCT))
$(call product_public,$(LIBHEADER_PRODUCT))

$(LIBHEADER_TARGET): $(call bin_header_deps,srclib/yotta.h)



LIB_HEADERS_PRODUCT := $(call product_create,BINHEADERS,headers)
LIB_HEADERS_TARGET := $(call product_target,$(LIB_HEADERS_PRODUCT))
$(call product_public,$(LIB_HEADERS_PRODUCT))

$(LIB_HEADERS_TARGET): $(call bin_header_deps,./srclib/yotta.h)


LIB_BINARIES_PRODUCT := $(call product_create,BINLIBSTATIC,static_lib)
LIB_BINARIES_TARGET := $(call product_target,$(LIB_BINARIES_PRODUCT))
$(call product_public,$(LIB_BINARIES_PRODUCT))

$(LIB_BINARIES_TARGET): $(call bin_object_files,$(call rwildcard,./srclib/,*.c))



# ------------------------------------------------------------------------------ Yotta library's headers directory

LIB_HEADERS_PRODUCT := $(call product_create,BINHEADERS,headers)
LIB_HEADERS_TARGET := $(call product_target,$(LIB_HEADERS_PRODUCT))
$(call product_public,$(LIB_HEADERS_PRODUCT))

$(LIB_HEADERS_TARGET): $(call bin_header_deps,./srclib/yotta.h)


# ------------------------------------------------------------------------------ Yotta library's binaries

LIB_BINARIES_PRODUCT := $(call product_create,BINLIBSTATIC,static_lib)
LIB_BINARIES_TARGET := $(call product_target,$(LIB_BINARIES_PRODUCT))
$(call product_public,$(LIB_BINARIES_PRODUCT))

$(LIB_BINARIES_TARGET): $(call bin_object_files,$(call rwildcard,./srclib/,*.c))


# ------------------------------------------------------------------------------ Yotta library's tests

$(foreach TEST,$(call filelist,tests/testlib.flist), \
    $(eval TEST_PRODUCT_NAME := $(call product_create,BINEXEC,$(notdir $(TEST:.c=)))) \
    $(eval TEST_PRODUCT_TARGET := $(call product_target,$(TEST_PRODUCT_NAME))) \
    $(eval $(TEST_PRODUCT_TARGET): $(LIB_HEADERS_TARGET)) \
    $(eval $(TEST_PRODUCT_TARGET): $(LIB_BINARIES_TARGET)) \
    $(eval $(TEST_PRODUCT_TARGET): $(call bin_object_files,$(TEST))) \
    $(eval $(TEST_PRODUCT_TARGET): C_FLAGS += -I $(LIB_HEADERS_TARGET) -I $(test_apis_dir) -g) \
    $(eval TEST_PRODUCT := $(call test_product,$(TEST_PRODUCT_NAME))) \
)


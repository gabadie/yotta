
# ------------------------------------------------------------------------------ Yotta library's headers directory

LIB_HEADERS_PRODUCT := $(call product_create,BINHEADERS,headers)
LIB_HEADERS_TARGET := $(call product_target,$(LIB_HEADERS_PRODUCT))
$(call product_public,$(LIB_HEADERS_PRODUCT))

$(LIB_HEADERS_TARGET): $(call bin_header_deps,./src/yotta.h)


# ------------------------------------------------------------------------------ Yotta library's binaries

LIB_BINARIES_PRODUCT := $(call product_create,BINLIBSTATIC,static_lib)
LIB_BINARIES_TARGET := $(call product_target,$(LIB_BINARIES_PRODUCT))
$(call product_public,$(LIB_BINARIES_PRODUCT))

$(LIB_BINARIES_TARGET): $(call bin_object_files,$(call filelist,./src/yotta.flist))


# ------------------------------------------------------------------------------ Yotta slave application

SLAVE_PRODUCT := $(call product_create,BINEXEC,yotta_slave)
SLAVE_TARGET := $(call product_target,$(SLAVE_PRODUCT))
$(call product_public,$(SLAVE_PRODUCT))

$(SLAVE_TARGET): $(call bin_object_files,$(call filelist,./src/slave.flist))


# ------------------------------------------------------------------------------ Yotta library's tests

$(foreach TEST,$(call filelist,tests/testlib.flist), \
    $(eval TEST_LIB_NAME := $(call product_create,BINEXEC,$(notdir $(TEST:.c=)))) \
    $(eval TEST_LIB_TARGET := $(call product_target,$(TEST_LIB_NAME))) \
    $(eval $(TEST_LIB_TARGET): $(call bin_object_files,$(TEST))) \
    $(eval TEST_LIB_TARGETS += $(TEST_LIB_TARGET)) \
    $(eval $($(call test_product,$(TEST_LIB_NAME)))) \
)

$(TEST_LIB_TARGETS): $(LIB_HEADERS_TARGET)
$(TEST_LIB_TARGETS): $(LIB_BINARIES_TARGET)
$(TEST_LIB_TARGETS): C_FLAGS += -I $(LIB_HEADERS_TARGET) -I $(test_apis_dir) -g


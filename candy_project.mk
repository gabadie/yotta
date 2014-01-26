
ifneq ($(config),debug)
    config=release
endif

# ------------------------------------------------------------------------------ Yotta library's headers directory

LIB_HEADERS_PRODUCT := $(call product_create,BINHEADERS,headers)
LIB_HEADERS_TARGET := $(call product_target,$(LIB_HEADERS_PRODUCT))
$(call product_public,$(LIB_HEADERS_PRODUCT))

$(LIB_HEADERS_TARGET): $(call bin_header_deps,./src/yotta.h)


# ------------------------------------------------------------------------------ Yotta library's binaries

LIB_BINARIES_PRODUCT := $(call product_create,BINLIBSTATIC,static_lib)
LIB_BINARIES_TARGET := $(call product_target,$(LIB_BINARIES_PRODUCT))
$(call product_public,$(LIB_BINARIES_PRODUCT))

LIB_OBJECT_BINARIES := $(call bin_object_files,$(call filelist,./src/yotta.flist))

# ------------------------------------------------------------ link configuration
$(LIB_BINARIES_TARGET): $(LIB_OBJECT_BINARIES)
$(LIB_BINARIES_TARGET): LDFLAGS += $(LIB_OBJECT_BINARIES)


# ------------------------------------------------------------------------------ Yotta slave application

SLAVE_PRODUCT := $(call product_create,BINEXEC,yotta_slave)
SLAVE_TARGET := $(call product_target,$(SLAVE_PRODUCT))
$(call product_public,$(SLAVE_PRODUCT))

SLAVE_BINARY_OBJECTS := $(call bin_object_files,$(call filelist,./src/slave.flist))

# ------------------------------------------------------------ link configuration
$(SLAVE_TARGET): $(SLAVE_BINARY_OBJECTS)
$(SLAVE_TARGET): LDFLAGS += $(SLAVE_BINARY_OBJECTS)


# ------------------------------------------------------------------------------ Yotta library's tests

TEST_LIB_C_FILES := $(call filelist,tests/testlib.flist)

$(foreach TEST,$(TEST_LIB_C_FILES), \
    $(eval TEST_LIB_NAME := $(call product_create,BINEXEC,$(notdir $(TEST:.c=)))) \
    $(eval TEST_LIB_TARGET := $(call product_target,$(TEST_LIB_NAME))) \
    $(eval $(TEST_LIB_TARGET): $(call bin_object_files,$(TEST))) \
    $(eval $(TEST_LIB_TARGET): LDFLAGS += $(call bin_object_files,$(TEST))) \
    $(eval TEST_LIB_TARGETS += $(TEST_LIB_TARGET)) \
    $(eval $($(call test_product,$(TEST_LIB_NAME)))) \
)

# ------------------------------------------------------------ C configuration
$(TEST_LIB_TARGETS): CDEPS += $(LIB_HEADERS_TARGET)
$(TEST_LIB_TARGETS): CFLAGS += -I $(LIB_HEADERS_TARGET) -I $(test_apis_dir) -g

# ------------------------------------------------------------ link configuration
$(TEST_LIB_TARGETS): $(LIB_BINARIES_TARGET)
$(TEST_LIB_TARGETS): LDFLAGS += $(LIB_BINARIES_TARGET)


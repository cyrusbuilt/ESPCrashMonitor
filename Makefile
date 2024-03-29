#-------------------------------------------------------------------- settings
FIND          := find
DIR           := $(PWD)/examples
CRITERIA      := \( -name "*.ino" -o -name "*.pde" \)
EACH_EXAMPLE  := $(FIND) $(DIR) $(CRITERIA) -exec
BUILD         := pio ci --verbose
LIB           := "."

#--------------------------------------------------------------------- targets
clean_docs:
	-rm -rf docs

docs:
	@doxygen
	@open docs/html/index.html

# update .travis.yml if target boards added
all: huzzah

huzzah:
	PLATFORMIO_BOARD=$@ $(MAKE) build

build:
	$(EACH_EXAMPLE) $(BUILD) --board=$(PLATFORMIO_BOARD) --lib=$(LIB) {} \;

.PHONY: all huzzah build
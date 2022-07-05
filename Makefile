MDBOOK ?= mdbook
SHFMT  ?= shfmt

SHFMTFLAGS += -w -fn -sr


BUILD ?= build

SHELL_SCRIPTS = tools/depchk.bash


.PHONY: all
all: docs $(BUILD)


.PHONY: clean
clean:
	@rm -rvf $(BUILD)


.PHONY: docs
docs: $(BUILD)
	$(MDBOOK) build


.PHONY: format
format: $(SHELL_SCRIPTS)
	$(SHFMT) $(SHFMTFLAGS) $?


$(BUILD):
	@mkdir $(BUILD)

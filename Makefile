MDBOOK     ?= mdbook
SHFMT      ?= shfmt
SHELLCHECK ?= shellcheck

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


.PHONY: lint
lint: $(SHELL_SCRIPTS)
	$(SHELLCHECK) $(SHELLCHECKFLAGS) $?


$(BUILD):
	@mkdir $(BUILD)

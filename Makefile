MDBOOK ?= mdbook


BUILD ?= build


.PHONY: all
all: docs $(BUILD)


.PHONY: clean
clean:
	@rm -rvf $(BUILD)


.PHONY: docs
docs: $(BUILD)
	$(MDBOOK) build


$(BUILD):
	@mkdir $(BUILD)

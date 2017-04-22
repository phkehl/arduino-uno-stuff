EXAMPLES := $(sort $(subst /,,$(dir $(wildcard */*.c))))

.PHONY: all
all: $(addsuffix -img,$(EXAMPLES))

.PHONY: clean
clean: $(addsuffix -clean,$(EXAMPLES))

.PHONY: clean
distclean: $(addsuffix -distclean,$(EXAMPLES))

define makeExample
.PHONY: $(1)-img
$(1)-img:
	@make -C $(1) img
.PHONY: $(1)-clean
$(1)-clean:
	@make -C $(1) clean
$(1)-distclean:
	@make -C $(1) distclean
endef

$(foreach ex, $(EXAMPLES), $(eval $(call makeExample,$(ex))))

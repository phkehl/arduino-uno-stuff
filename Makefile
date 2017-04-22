default:
	@echo "Make what? See README."

STUFF := doc examples projects

all: $(STUFF)
clean: $(addsuffix -clean,$(STUFF))
distclean: $(addsuffix -distclean,$(STUFF))
	rm -rf src/*~


.PHONY: doc
doc:
	@make -C doc
.PHONY: clean-doc

doc-clean:
	@make -C doc clean

.PHONY: doc-distclean
doc-distclean:
	@make -C doc distclean

.PHONY: examples
examples:
	@make -C examples

.PHONY: examples-clean
examples-clean:
	@make -C examples clean

.PHONY: examples-distclean
examples-distclean:
	@make -C examples distclean

.PHONY: projects
projects:
	@make -C projects

.PHONY: projects-clean
projects-clean:
	@make -C projects clean

.PHONY: projects-distclean
projects-distclean:
	@make -C projects distclean


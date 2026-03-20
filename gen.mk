SRC_AMD64_INST_DEF_H = src/amd64/inst.def.h

src/amd64/inst.c: $(SRC_AMD64_INST_DEF_H)
$(SRC_AMD64_INST_DEF_H): src/amd64/inst.def $(TOOL_AMD64_PARSE_INST_DEF)
	$(E) "  GEN   " $@
	$(Q) $(TOOL_AMD64_PARSE_INST_DEF) $< $@

clean-gen:
	$(E) "  CLEAN GEN"
	$(Q) rm -f src/amd64/inst.def.h

.PHONY: clean-gen

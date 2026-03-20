TOOL_AMD64_PARSE_INST_DEF = $(BUILD_DIR)/src/amd64/parse_inst_def

TOOLS = $(TOOL_AMD64_PARSE_INST_DEF)

$(TOOLS): $(addsuffix .o,$(TOOLS))
	$(E) "  TOOL  " $@
	$(Q) $(CC) -g3 -o $@ $<

clean-tool:
	$(E) "  CLEAN TOOL"
	$(Q) rm -f $(TOOLS)

.PHONY: clean-tool

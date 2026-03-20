TOOL_AMD64_PARSE_INST_DEF = $(BUILD_DIR)/src/amd64/parse_inst_def

TOOLS = $(TOOL_AMD64_PARSE_INST_DEF)

clean-tool:
	@echo "  CLEAN TOOL"
	@rm -f $(TOOLS)

.PHONY: clean-tool

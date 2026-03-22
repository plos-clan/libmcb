#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inst.h"
#include "mcb/amd64/reg.h"

static enum REG match_reg(const char *s, size_t len);
static unsigned int parse_flag(const char **_c);
static void parse_inst();
static unsigned int parse_operand(struct amd64_inst_variant *variant, const char **_c);
static unsigned int parse_operand_no_imm(struct amd64_inst_variant *variant, const char **_c);
static void parse_operand_reg(enum REG reg[2], const char **_c);
static void parse_variant();
static char *until(const char **_c, const char *expect);

static char lbuf[BUFSIZ]; /* line buffer */

static FILE *in, *out;

#define X "%d,%d,%d,%d"
static const char *inst_variants_template[] = {
	"static const struct amd64_inst_variant amd64_%s_variants[] = {\n",
	"{XS0("X", "X", "X"), XS1("X", "X"), XD("X", "X"), "
		"{%s,%s}, {%s,%s}, {%s,%s}},\n",
	"END};\n"
};
#undef X

static const char *inst_template =
"static const struct amd64_inst amd64_%s_inst = {\"%s\",amd64_%s_variants};\n";

static const char *reg_str[] = {
       [RBP]  = "rbp",
       [RSP]  = "rsp",
       [RAX]  = "rax",
       [RBX]  = "rbx",
       [RCX]  = "rcx",
       [RDX]  = "rdx",
       [RDI]  = "rdi",
       [RSI]  = "rsi",
       [R8 ]  = "r8",
       [R9 ]  = "r9",
       [R10]  = "r10",
       [R11]  = "r11",
       [R12]  = "r12",
       [R13]  = "r13",
       [R14]  = "r14",
       [R15]  = "r15",
       NULL
};

static const char *reg_upper_str[] = {
       [RBP]  = "RBP",
       [RSP]  = "RSP",
       [RAX]  = "RAX",
       [RBX]  = "RBX",
       [RCX]  = "RCX",
       [RDX]  = "RDX",
       [RDI]  = "RDI",
       [RSI]  = "RSI",
       [R8 ]  = "R8",
       [R9 ]  = "R9",
       [R10]  = "R10",
       [R11]  = "R11",
       [R12]  = "R12",
       [R13]  = "R13",
       [R14]  = "R14",
       [R15]  = "R15",
       [NREG] = "NREG",
       [USE_DST_SET] = "USE_DST_SET"
};

enum REG
match_reg(const char *s, size_t len)
{
	for (enum REG i = 0; reg_str[i]; i++) {
		if (strlen(reg_str[i]) != len)
			continue;
		if (strncmp(s, reg_str[i], len) == 0)
			return i;
	}
	return NREG;
}

unsigned int
parse_flag(const char **_c)
{
	const char *c = *_c;
	unsigned int flag = 0, off = 0;
	for (; *c; c++) {
		switch (*c) {
		case 'b': off |= SIZ8_BIT;    break;
		case 'w': off |= SIZ16_BIT;   break;
		case 'l': off |= SIZ32_BIT;   break;
		case 'q': off |= SIZ64_BIT;   break;
		case 'i': flag |= (off << 8); break;
		case 'm': flag |= (off << 4); break;
		case 'r': flag |= off;        break;
		case '.': break;
		default: goto end; break;
		}
	}
end:
	*_c = c;
	return flag;
}

void
parse_inst()
{
	const char *c, *beg;
	char *name = NULL;
	char *fmt;

	assert(lbuf[0] == '[');

	c = lbuf + 1;
	beg = c;

	if (*c == '+') {
		beg++;
		c++;
	}
	for (; *c && isalpha(*c); c++);
	assert(c != beg);
	name = strndup(beg, c - beg);

	until(&c, "]");
	fmt = strndup(lbuf + 1, c - lbuf - 2);

	c = lbuf;
	beg = c;
	fprintf(out, inst_variants_template[0], name);
	while (fgets(lbuf, BUFSIZ, in)) {
		if (*lbuf == '[')
			break;
		parse_variant();
	}
	fprintf(out, "%s", inst_variants_template[2]);

	fprintf(out, inst_template, name, fmt, name);

	free(fmt);
	free(name);
}

unsigned int
parse_operand(struct amd64_inst_variant *variant, const char **_c)
{
	const char *c = *_c;
	unsigned int flag = 0;
	char o = *c;
	char *r = until(&c, ":[,");
	if (*r == ',')
		return 0;
	if (*r == '[') {
		switch (o) {
		case '0':
			parse_operand_reg(variant->src0_reg, &c);
			break;
		case '1':
			parse_operand_reg(variant->src1_reg, &c);
			break;
		case '=':
			parse_operand_reg(variant->dst_reg, &c);
			break;
		}
	}
	flag = parse_flag(&c);
	switch (o) {
	case '0': variant->src0 = flag; break;
	case '1': variant->src1 = flag; break;
	case '=': variant->dst = flag; break;
	}
	*_c = c;
	return flag;
}

unsigned int
parse_operand_no_imm(struct amd64_inst_variant *variant, const char **_c)
{
	unsigned int flag = parse_operand(variant, _c);
	if (flag & IMM_BIT) {
		fprintf(stderr, "imm in dst\n");
		exit(1);
	}
	return flag;
}

void
parse_operand_reg(enum REG reg[2], const char **_c)
{
	const char *beg = *_c;
	const char *c = *_c;
	int i = 0;

	if (*c == '=') {
		c++;
		if (*c != ']') {
			fprintf(stderr, "expected ']'\n");
			exit(1);
		}
		reg[0] = USE_DST_SET;
		reg[1] = USE_DST_SET;
		goto end;
	}

	for (; *c && *c != ']'; c++) {
		if (*c != '+')
			continue;
		if (i) {
			fprintf(stderr, "too many regs: expected 2, use 3 or more\n");
			exit(1);
		}
		reg[i] = match_reg(beg, c - beg);
		beg = c + 1;
		i = 1;
	}
	reg[i] = match_reg(beg, c - beg);
end:
	*_c = c;
}

void
parse_variant()
{
	const char *c;
	struct amd64_inst_variant variant = {0};

	variant.src0_reg[0] = NREG;
	variant.src0_reg[1] = NREG;
	variant.src1_reg[0] = NREG;
	variant.src1_reg[1] = NREG;
	variant.dst_reg[0] = NREG;
	variant.dst_reg[1] = NREG;

	if (*lbuf != '%')
		return;
	c = lbuf;
	for (; *c; c++) {
		if (*c != '%')
			continue;
		c++;
		switch (*c) {
		case '0':
			parse_operand(&variant, &c);
			goto end;
		case '1':
			parse_operand_no_imm(&variant, &c);
			goto end;
		case '=':
			parse_operand_no_imm(&variant, &c);
			goto end;
		end:
			c--;
			break;
		}
	}

	if (variant.dst_reg[0] != NREG)
		variant.dst |= (MEM_BIT | REG_BIT);

	if (variant.src0_reg[0] == USE_DST_SET) {
		variant.src0 = variant.dst;
		variant.src0_reg[0] = variant.dst_reg[0];
		variant.src0_reg[1] = variant.dst_reg[1];
	}
	if (variant.src1_reg[0] == USE_DST_SET) {
		variant.src1 = variant.dst;
		variant.src1_reg[0] = variant.dst_reg[0];
		variant.src1_reg[1] = variant.dst_reg[1];
	}

#define X(F, O) \
	(F & (SIZ8_BIT  << O)) ? 1 : 0, \
	(F & (SIZ16_BIT << O)) ? 1 : 0, \
	(F & (SIZ32_BIT << O)) ? 1 : 0, \
	(F & (SIZ64_BIT << O)) ? 1 : 0
	fprintf(out, inst_variants_template[1],
			X(variant.src0, 8), X(variant.src0, 4),
			X(variant.src0, 0),
			X(variant.src1, 4), X(variant.src1, 0),
			X(variant.dst,  4), X(variant.dst,  0),
			reg_upper_str[variant.src0_reg[0]],
			reg_upper_str[variant.src0_reg[1]],
			reg_upper_str[variant.src1_reg[0]],
			reg_upper_str[variant.src1_reg[1]],
			reg_upper_str[variant.dst_reg[0]],
			reg_upper_str[variant.dst_reg[1]]);
#undef X
}

char *
until(const char **_c, const char *expect)
{
	const char *c = *_c;
	char *r;
	for (; *c; c++) {
		r = strchr(expect, *c);
		if (r != NULL) {
			c++;
			*_c = c;
			return r;
		}
	}
	fprintf(stderr, "end of file\n");
	exit(1);
}

int
main(int argc, char *argv[])
{
	if (argc < 3) {
		puts("parse_inst_def [INPUT] [OUTPUT]");
		exit(1);
	}

	in = fopen(argv[1], "r");
	out = fopen(argv[2], "w");

	if (!fgets(lbuf, BUFSIZ, in))
		goto end;
	while (*lbuf == '[')
		parse_inst();

end:
	fclose(out);
	fclose(in);

	return 0;
}

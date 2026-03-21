#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inst.h"

static char lbuf[BUFSIZ]; /* line buffer */

static FILE *in, *out;

#define X "%d,%d,%d,%d"
static const char *inst_variants_template[] = {
	"static const struct amd64_inst_variant amd64_%s_variants[] = {\n",
	"{XS0("X", "X", "X"), XS1("X", "X"), XD("X", "X")},\n",
	"END};\n"
};
#undef X

static const char *inst_template =
"static const struct amd64_inst amd64_%s_inst = {\"%s\",amd64_%s_variants};\n";

static char *
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

static unsigned int
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

static unsigned int
parse_operand(const char **_c)
{
	const char *c = *_c;
	unsigned int flag = 0;
	char *r = until(&c, ":,");
	if (*r == ',')
		return 0;
	flag = parse_flag(&c);
	*_c = c;
	return flag;
}

static unsigned int
parse_operand_no_imm(const char **_c)
{
	unsigned int flag = parse_operand(_c);
	if (flag & IMM_BIT) {
		fprintf(stderr, "imm in dst\n");
		exit(1);
	}
	return flag;
}

static void
parse_variant()
{
	const char *c;
	unsigned int flag0, flag1, flag2;
	flag0 = flag1 = flag2 = 0;

	if (*lbuf != '%')
		return;
	c = lbuf;
	for (; *c; c++) {
		if (*c != '%')
			continue;
		c++;
		switch (*c) {
		case '0':
			c++;
			flag0 = parse_operand(&c);
			goto end;
		case '1':
			c++;
			flag1 = parse_operand_no_imm(&c);
			goto end;
		case '=':
			c++;
			flag2 = parse_operand_no_imm(&c);
			goto end;
		end:
			c--;
			break;
		}
	}

#define X(F, O) \
	(F & (SIZ8_BIT  << O)) ? 1 : 0, \
	(F & (SIZ16_BIT << O)) ? 1 : 0, \
	(F & (SIZ32_BIT << O)) ? 1 : 0, \
	(F & (SIZ64_BIT << O)) ? 1 : 0
	fprintf(out, inst_variants_template[1],
			X(flag0, 8), X(flag0, 4), X(flag0, 0),
			X(flag1, 4), X(flag1, 0),
			X(flag2, 4), X(flag2, 0));
#undef X
}

static void
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

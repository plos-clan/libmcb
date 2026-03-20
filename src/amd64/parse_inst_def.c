#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct variant {
	unsigned int src0:12, src1:8, dst:8;
	int reg[3];
};

static char lbuf[BUFSIZ]; /* line buffer */

static FILE *in, *out;

#define X "%d,%d,%d,%d"
static const char *inst_variants_template[] = {
	"static struct amd64_inst_variant amd64_%s_variants[] = {\n",
	"{XS0("X", "X", "X"), XS1("X", "X"), XD("X", "X")},\n",
	"END};\n"
};
#undef X

static const char *inst_template =
"static struct amd64_inst amd64_%s_inst = {\"%s\",amd64_%s_variants};\n";

static void
expect(const char **_c, char expect)
{
	const char *c = *_c;
	for (; *c; c++) {
		if (*c == expect) {
			c++;
			*_c = c;
			return;
		}
	}
	fprintf(stderr, "end of file\n");
	exit(1);
}

static unsigned int
parse_dst(const char **_c)
{
	const char *c = *_c;
	unsigned int flag = 0, off = 0;
	expect(&c, ':');
	for (; *c; c++) {
		switch (*c) {
		case 'b': off |= 1;           break;
		case 'w': off |= 1 << 1;      break;
		case 'l': off |= 1 << 2;      break;
		case 'q': off |= 1 << 3;      break;
		case 'm': flag |= off;        break;
		case 'r': flag |= (off << 4); break;
		case '.': break;
		default: goto end; break;
		}
	}
end:
	*_c = c;
	return flag;
}

static unsigned int
parse_src_flag(const char **_c)
{
	const char *c = *_c;
	unsigned int flag = 0, off = 0;
	for (; *c; c++) {
		switch (*c) {
		case 'b': off |= 1;           break;
		case 'w': off |= 1 << 1;      break;
		case 'l': off |= 1 << 2;      break;
		case 'q': off |= 1 << 3;      break;
		case 'i': flag |= off;        break;
		case 'm': flag |= (off << 4); break;
		case 'r': flag |= (off << 8); break;
		case '.': break;
		default: goto end; break;
		}
	}
end:
	*_c = c;
	return flag;
}

static unsigned int
parse_src0(const char **_c)
{
	const char *c = *_c;
	unsigned int flag = 0;

	expect(&c, ':');
	flag = parse_src_flag(&c);

	*_c = c;
	return flag;
}

static unsigned int
parse_src1(const char **_c)
{
	const char *c = *_c;
	unsigned int flag = 0;

	expect(&c, ':');
	flag = parse_src_flag(&c);

	*_c = c;
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
			flag0 = parse_src0(&c);
			goto end;
		case '1':
			c++;
			flag1 = parse_src1(&c);
			goto end;
		case '=':
			c++;
			flag2 = parse_dst(&c);
			goto end;
		end:
			c--;
			break;
		}
	}

#define X(F, O) \
	(F & (1 << (O))) ? 1 : 0, \
	(F & (1 << (1 + O))) ? 1 : 0, \
	(F & (1 << (2 + O))) ? 1 : 0, \
	(F & (1 << (3 + O))) ? 1 : 0
	fprintf(out, inst_variants_template[1],
			X(flag0, 0),
			X(flag0, 4),
			X(flag0, 8),
			X(flag1, 4),
			X(flag1, 8),
			X(flag2, 4),
			X(flag2, 8));
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

	expect(&c, ']');
	fmt = strndup(lbuf + 1, c - lbuf - 1);

	c = lbuf;
	beg = c;
	fprintf(out, inst_variants_template[0], name);
	while (fgets(lbuf, BUFSIZ, in))
		parse_variant();
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

	while (fgets(lbuf, BUFSIZ, in)) {
		if (lbuf[0] == '[')
			parse_inst();
	}

	fclose(out);
	fclose(in);

	return 0;
}

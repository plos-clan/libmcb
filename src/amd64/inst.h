#define SIZ8_OFF  0
#define SIZ16_OFF 1
#define SIZ32_OFF 2
#define SIZ64_OFF 3

enum {
	SIZ8_BIT  = 1 << SIZ8_OFF,
	SIZ16_BIT = 1 << SIZ16_OFF,
	SIZ32_BIT = 1 << SIZ32_OFF,
	SIZ64_BIT = 1 << SIZ64_OFF,

	IMM8_BIT  = SIZ8_BIT  << 8,
	IMM16_BIT = SIZ16_BIT << 8,
	IMM32_BIT = SIZ32_BIT << 8,
	IMM64_BIT = SIZ64_BIT << 8,
	MEM8_BIT  = SIZ8_BIT  << 4,
	MEM16_BIT = SIZ16_BIT << 4,
	MEM32_BIT = SIZ32_BIT << 4,
	MEM64_BIT = SIZ64_BIT << 4,
	REG8_BIT  = SIZ8_BIT,
	REG16_BIT = SIZ16_BIT,
	REG32_BIT = SIZ32_BIT,
	REG64_BIT = SIZ64_BIT,

	IMM_BIT = IMM8_BIT | IMM16_BIT | IMM32_BIT | IMM64_BIT,
	MEM_BIT = MEM8_BIT | MEM16_BIT | MEM32_BIT | MEM64_BIT,
	REG_BIT = REG8_BIT | REG16_BIT | REG32_BIT | REG64_BIT
};

#define AMD64_INST_VARIANT_END {0,0,0}
#define is_end_variant(V) ((V).src0 == 0 && (V).src1 == 0 && (V).dst == 0)
struct amd64_inst_variant {
	unsigned int src0:12, src1:8, dst:8;
};

struct amd64_inst {
	const char *fmt;
	const struct amd64_inst_variant *variants;
};

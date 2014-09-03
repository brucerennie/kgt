/* Generated by lx */

#ifndef LX_H
#define LX_H

enum lx_wsn_token {
	TOK_EMPTY,
	TOK_IDENT,
	TOK_ENDGROUP,
	TOK_STARTGROUP,
	TOK_ENDOPT,
	TOK_STARTOPT,
	TOK_ENDSTAR,
	TOK_STARTSTAR,
	TOK_LITERAL,
	TOK_CHAR,
	TOK_ESC,
	TOK_SEP,
	TOK_ALT,
	TOK_EQUALS,
	TOK_EOF,
	TOK_ERROR,
	TOK_UNKNOWN
};

struct lx_pos {
	unsigned byte;
	unsigned line;
	unsigned col;
};

struct lx_wsn_lx {
	int (*lgetc)(struct lx_wsn_lx *lx);
	void *opaque;

	int c; /* lx_wsn_ungetc buffer */

	struct lx_pos start;
	struct lx_pos end;

	void *buf;
	int  (*push) (struct lx_wsn_lx *lx, char c);
	void (*pop)  (struct lx_wsn_lx *lx);
	int  (*clear)(struct lx_wsn_lx *lx);
	void (*free) (struct lx_wsn_lx *lx);

	enum lx_wsn_token (*z)(struct lx_wsn_lx *lx);
};

/*
 * The initial buffer size; this ought to be over the typical token length,
 * so as to avoid a run-up of lots of resizing.
 */
#ifndef LX_DYN_LOW
#define LX_DYN_LOW 1 << 10
#endif

/*
 * High watermark; if the buffer grows over this, it will resize back down
 * by LX_DYN_FACTOR when no longer in use.
 */
#ifndef LX_DYN_HIGH
#define LX_DYN_HIGH 1 << 13
#endif

/*
 * Andrew Koenig said the growth factor should be less than phi, (1 + sqrt(5)) / 2
 * P.J. Plauger said 1.5 works well in practice. (Perhaps because of internal
 * bookkeeping data stored by the allocator.)
 *
 * Non-integer factors here add the constraint that LX_DYN_LOW > 1 because
 * because conversion to size_t truncates, and e.g. 1 * 1.5 == 1 is no good
 * as the requirement is to *increase* a buffer.
 */
#ifndef LX_DYN_FACTOR
#define LX_DYN_FACTOR 2
#endif

/* dynamic token buffer */
struct lx_dynbuf {
	char *p;
	size_t len;
	char *a;
};

/* fixed-size token buffer */
struct lx_fixedbuf {
	char *p;
	size_t len;
#ifdef LX_FIXED_SIZE
	char a[LX_FIXED_SIZE];
#else
	char *a; /* could be flexible member */
#endif
};

/* opaque for lx_wsn_agetc */
struct lx_arr {
	char *p;
	size_t len;
};

/* opaque for lx_wsn_fdgetc */
struct lx_fd {
	char *p;
	size_t len;

	int fd;
	size_t bufsz; /* number of bytes allocated after this struct */
};

const char *lx_wsn_name(enum lx_wsn_token t);
const char *lx_wsn_example(enum lx_wsn_token (*z)(struct lx_wsn_lx *), enum lx_wsn_token t);

void lx_wsn_init(struct lx_wsn_lx *lx);
enum lx_wsn_token lx_wsn_next(struct lx_wsn_lx *lx);

int lx_wsn_fgetc(struct lx_wsn_lx *lx);
int lx_wsn_sgetc(struct lx_wsn_lx *lx);
int lx_wsn_agetc(struct lx_wsn_lx *lx);
int lx_wsn_dgetc(struct lx_wsn_lx *lx);

int  lx_wsn_dynpush(struct lx_wsn_lx *lx, char c);
void lx_wsn_dynpop(struct lx_wsn_lx *lx);
int  lx_wsn_dynclear(struct lx_wsn_lx *lx);
void lx_wsn_dynfree(struct lx_wsn_lx *lx);

int  lx_wsn_fixedpush(struct lx_wsn_lx *lx, char c);
void lx_wsn_fixedpop(struct lx_wsn_lx *lx);
int  lx_wsn_fixedclear(struct lx_wsn_lx *lx);

#endif

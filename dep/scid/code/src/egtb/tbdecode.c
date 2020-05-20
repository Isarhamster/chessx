#ifndef WINCE

#ifndef TBDECODE
#define	TBDECODE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC CLK_TCK
#endif

/* ---------------------------- Error codes --------------------------- */
/*                              -----------                             */


#define COMP_ERR_NONE     0 /* everything is OK                         */
#define COMP_ERR_READ     2 /* input file read error                    */
#define COMP_ERR_NOMEM    5 /* no enough memory                         */
#define COMP_ERR_BROKEN   6 /* damaged compressed data                  */
#define COMP_ERR_PARAM    7 /* incorrect function parameter             */
#define COMP_ERR_INTERNAL 9 /* everything else is internal error        */
                            /* hopefully it should never happen         */

/* Almost all  functions listed further return one as  its result on of */
/* codes given  above: if no error occurred then COMP_ERR_NONE (i.e. 0) */
/* is returned, otherwise functions return  error  code  plus number of */
/* line in "comp.c"  where the error  was detected multiplied  by  256; */
/* line number may  be  used for exact specification  of  a place where */
/* error was detected thus making debugging slightly simpler.           */
/*                                                                      */
/* Thus, "(code &  0xff)"  gives proper error code,  and  "(code >> 8)" */
/* gives number of line where the error was raised.                     */


/* -------------------------------------------------------------------- */
/*                                                                      */
/*                Compress/decompress some chess tables                 */
/*                                                                      */
/*               Copyright (c) 1991--1998 Andrew Kadatch                */
/*                                                                      */
/* The Limited-Reference  variant  of  Lempel-Ziv algorithm implemented */
/* here was first described in  my  B.Sc.  thesis "Efficient algorithms */
/* for image  compression",  Novosibirsk  State  University,  1992, and */
/* cannot be  used in any product distributed in  Russia or CIS without */
/* written permission from the author.                                  */
/*                                                                      */
/* Most of the code listed below is significantly  simplified code from  */
/* the PRS data compression library and therefore it should not be used */
/* in any product (software or hardware, commercial or not, and  so on) */
/* without written permission from the author.                          */
/*                                                                      */
/* -------------------------------------------------------------------- */


/* ---------------------------- Debugging ----------------------------- */
/*                              ---------                               */

#ifndef DEBUG
#define DEBUG	0
#endif

#if DEBUG
#define assert(cond) ((cond) ? (void) 0 : _local_assert (__LINE__))
static void _local_assert (int lineno)
{
  fprintf (stderr, "assertion at line %u failed\n", lineno);
  exit (33);
}
#define debug(x) x
#define dprintf(x) printf x
#else
#if !defined (assert)
#define assert(cond) ((void) 0)
#endif
#define debug(x)     ((void) 0)
#define dprintf(x)   ((void) 0)
#endif

extern "C" { int cbEGTBCompBytes = 0; }

/* --------------------- Constants, types, etc. ----------------------- */
/*                       ----------------------                         */

#define MIN_BLOCK_BITS	8	/* LOG2 (min size of block to compress)	*/
#define MAX_BLOCK_BITS	16	/* LOG2 (max size of block to compress) */

			/* max. integer we can take LOG2 by table	*/
#define MAX_BITS_HALF	((MAX_BLOCK_BITS + 1) >> 1)
#define MAX_BITS	(MAX_BITS_HALF * 2)

/* assume that integer is at least 32 bits wide */
#ifndef uint
#define uint unsigned
#endif

#ifndef uchar
#define uchar unsigned char
#endif

#define HEADER_SIZE		80	/* number of reserved bytes	*/
#define STOP_SEARCH_LENGTH	256	/* terminate search if match	*/
					/* length exceeds that value	*/

#define MAX_LENGTH_BITS		5
#define MAX_LENGTH              (1 << MAX_LENGTH_BITS)
#define LONG_BITS               1
#define LONG_LENGTH		(MAX_BLOCK_BITS - LONG_BITS)
#define LONG_QUICK		(MAX_LENGTH - LONG_LENGTH)

#if LONG_LENGTH > (MAX_BLOCK_BITS - LONG_BITS)
#  undef LONG_LENGTH
#  define LONG_LENGTH		(MAX_BLOCK_BITS - LONG_BITS)
#endif

#if LONG_LENGTH >= MAX_LENGTH || LONG_LENGTH <= 0
#error LONG_LENGTH is out of range
#endif

#if LONG_BITS <= 0
#error LONG_BITS must be positive
#endif

#define DELTA	(LONG_BITS + LONG_QUICK - 1)
#if (MAX_LENGTH - 1) - (LONG_LENGTH - LONG_BITS) != DELTA
#error Hmmm
#endif

#define MAX_DISTANCES		24

#define LOG_MAX_DISTANCES	6	/* see check below	*/

#if MAX_DISTANCES > (1 << LOG_MAX_DISTANCES)
#error MAX_DISTANCES should not exceed (1 << LOG_MAX_DISTANCES)
#endif

#define ALPHABET_SIZE		(256 + (MAX_DISTANCES << MAX_LENGTH_BITS))
#define MAX_ALPHABET	ALPHABET_SIZE	/* max. alphabet handled by	*/
					/* Huffman coding routines	*/

#define USE_CRC32		1
/* 0 - use Fletcher's checksum, != 0 - use proper CRC32			*/

static uchar header_title[64] =
"Compressed by DATACOMP v 1.0 (c) 1991--1998 Andrew Kadatch\r\n\0";

#define RET(n) ((n) + __LINE__ * 256)


/* ------------------------- CRC32 routines --------------------------- */
/*                           --------------                             */

#if USE_CRC32

static unsigned CRC32_table[256];
static int CRC32_initialized = 0;

static void CRC32_init (void)
{
  int i, j;
  unsigned k, m = (unsigned) 0xedb88320L;
  if (CRC32_initialized) return;
  for (i = 0; i < 256; ++i)
  {
    k = i; j = 8;
    do
    {
      if ((k&1) != 0)
        k >>= 1;
      else
      {
        k >>= 1;
        k ^= m;
      };
    }
    while (--j);
    CRC32_table[i] = k;
  }
  CRC32_initialized = 1;
}

static unsigned CRC32 (uchar *p, int n, unsigned k)
{
  unsigned *table = CRC32_table;
  uchar *e = p + n;
  while (p + 16 < e)
  {
#   define X(i) k = table[((uchar) k) ^ p[i]] ^ (k >> 8)
    X(0); X(1); X(2); X(3); X(4); X(5); X(6); X(7); X(8);
    X(9); X(10); X(11); X(12); X(13); X(14); X(15);
#   undef X
    p += 16;
  }
  while (p < e)
    k = table[((uchar) k) ^ *p++] ^ (k >> 8);
  return (k);
}

#else

#define CRC32_init()

static unsigned CRC32 (uchar *p, int n, unsigned k1)
{
  unsigned k0 = k1 & 0xffff;
  uchar *e = p + n;
  k1 = (k1 >> 16) & 0xffff;
  while (p + 16 < e)
  {
#   define X(i) k0 += p[i]; k1 += k0;
    X(0); X(1); X(2); X(3); X(4); X(5); X(6); X(7); X(8);
    X(9); X(10); X(11); X(12); X(13); X(14); X(15);
#   undef X
    k0 = (k0 & 0xffff) + (k0 >> 16);
    k1 = (k1 & 0xffff) + (k1 >> 16);
    p += 16;
  }
  while (p < e)
  {
    k0 += *p++;
    k1 += k0;
  }
  k0 = (k0 & 0xffff) + (k0 >> 16);
  k1 = (k1 & 0xffff) + (k1 >> 16);
  k0 = (k0 & 0xffff) + (k0 >> 16);
  k1 = (k1 & 0xffff) + (k1 >> 16);
  assert (((k0 | k1) >> 16) == 0);
  return (k0 + (k1 << 16));
}

#endif   /* USE_CRC32    */



/* ------------------------ Bit IO interface -------------------------- */
/*                          ----------------                            */

#define BITIO_LOCALS	\
  uint   _mask;         \
  int    _bits;         \
  uchar *_ptr

typedef struct
{
  BITIO_LOCALS;
}
bitio_t;

#define BITIO_ENTER(p) do {     \
  _mask = (p)._mask;            \
  _bits = (p)._bits;            \
  _ptr  = (p)._ptr;             \
} while (0)

#define BITIO_LEAVE(p) do {     \
  (p)._mask = _mask;            \
  (p)._bits = _bits;            \
  (p)._ptr  = _ptr;             \
} while (0)

#define BIORD_START(from) do {		\
  _ptr = (uchar *) (from);              \
  _bits = sizeof (_mask);               \
  _mask = 0;                            \
  do                                    \
    _mask = (_mask << 8) | *_ptr++;     \
  while (--_bits != 0);                 \
  _bits = 16;                           \
} while (0)

/* read [1, 17] bits at once */
#define BIORD(bits)      \
  (_mask >> (8 * sizeof (_mask) - (bits)))

#define BIORD_MORE(bits) do {		\
  _mask <<= (bits);			\
  if ((_bits -= (bits)) <= 0)           \
  {                                     \
    _mask |= ((_ptr[0] << 8) + _ptr[1]) << (-_bits);	\
    _ptr += 2; _bits += 16;		\
  }					\
} while (0)

/* ------------------------ Huffman coding ---------------------------- */
/*                          --------------                              */

#if MAX_ALPHABET <= 0xffff
#  if MAX_ALPHABET <= 1024
/* positive value takes 15 bits => symbol number occupies <= 10 bits	*/
#    define huffman_decode_t	short
#  else
#    define huffman_decode_t	int
#  endif
#else
#  define huffman_decode_t	int
#endif

#define HUFFMAN_DECODE(ch,table,start_bits) do {	\
  (ch) = table[BIORD (start_bits)];                     \
  if (((int) (ch)) >= 0)                                \
  {                                                     \
    BIORD_MORE ((ch) & 31);                             \
    (ch) >>= 5;                                         \
    break;                                              \
  }                                                     \
  BIORD_MORE (start_bits);                              \
  do                                                    \
  {                                                     \
    (ch) = table[BIORD (1) - (ch)];                     \
    BIORD_MORE (1);                                     \
  }                                                     \
  while (((int) (ch)) < 0);                             \
} while (0)

#define HUFFMAN_TABLE_SIZE(n,start_bits) \
  ((1 << (start_bits)) + ((n) << 1))

static int huffman_decode_create
  (huffman_decode_t *table, uchar *length, int n, int start_bits)
{
  int i, j, k, last, freq[32], sum[32];

  /* calculate number of codewords					*/
  memset (freq, 0, sizeof (freq));
  for (i = 0; i < n; ++i)
  {
    if ((k = length[i]) > 31)
      return RET (COMP_ERR_BROKEN);
    ++freq[k];
  }

  /* handle special case(s) -- 0 and 1 symbols in alphabet		*/
  if (freq[0] == n)
  {
    memset (table, 0, sizeof (table[0]) << start_bits);
    return (0);
  }
  if (freq[0] == n-1)
  {
    if (freq[1] != 1)
      return RET (COMP_ERR_BROKEN);
    for (i = 0; length[i] == 0;) ++i;
    i <<= 5;
    for (k = 1 << start_bits; --k >= 0;)
      *table++ = (huffman_decode_t) i;
    return (0);
  }

  /* save frequences			*/
  memcpy (sum, freq, sizeof (sum));

  /* check code correctness		*/
  k = 0;
  for (i = 32; --i != 0;)
  {
    if ((k += freq[i]) & 1)
      return RET (COMP_ERR_BROKEN);
    k >>= 1;
  }
  if (k != 1)
    return RET (COMP_ERR_BROKEN);

  /* sort symbols		*/
  k = 0;
  for (i = 1; i < 32; ++i)
    freq[i] = (k += freq[i]);
  last = freq[31];	/* preserve number of symbols in alphabet	*/
  for (i = n; --i >= 0;)
  {
    if ((k = length[i]) != 0)
      table[--freq[k]] = (huffman_decode_t) i;
  }

  /* now create decoding table	*/
  k = i = (1<<start_bits) + (n<<1);
  for (n = 32; --n > start_bits;)
  {
    j = i;
    while (k > j)
      table[--i] = (huffman_decode_t) -(k -= 2);
    for (k = sum[n]; --k >= 0;)
      table[--i] = table[--last];
    k = j;
  }

  j = i;
  i = 1 << start_bits;
  while (k > j)
    table[--i] = (huffman_decode_t) -(k -= 2);

  for (; n > 0; --n)
  {
    for (k = sum[n]; --k >= 0;)
    {
      assert (last <= i && last > 0);
      j = i - (1 << (start_bits - n));
      n |= table[--last] << 5;
      do
        table[--i] = (huffman_decode_t) n;
      while (i != j);
      n &= 31;
    }
  }
  assert ((i | last) == 0);

  return (0);
}

/* -------------------- Read/write Huffman code ----------------------- */
/*                      -----------------------                         */

#define MIN_REPT	2

#if MIN_REPT <= 1
#error MIN_REPT must exceed 1
#endif

#define TEMP_TABLE_BITS 8

static int huffman_read_length (bitio_t *bitio, uchar *length, int n)
{
  BITIO_LOCALS;
  huffman_decode_t table[2][HUFFMAN_TABLE_SIZE (64, TEMP_TABLE_BITS)];
  uchar bits[128];
  int i, j, k;

  BITIO_ENTER (*bitio);
  k = BIORD (1); BIORD_MORE (1);
  if (k != 0)
  {
    memset (length, 0, n);
    goto ret;
  }

  if (n <= 128)
  {
    k = BIORD (5); BIORD_MORE (5);
    for (i = 0; i < n;)
    {
      length[i] = (uchar) BIORD (k); BIORD_MORE (k);
      if (length[i++] == 0)
      {
        j = i + BIORD (4); BIORD_MORE (4);
        if (j > n)
          return RET (COMP_ERR_BROKEN);
        while (i != j)
          length[i++] = 0;
      }
    }
    goto ret;
  }

  BITIO_LEAVE (*bitio);
  i = huffman_read_length (bitio, bits, 128);
  if (i != 0)
    return (i);
  i = huffman_decode_create (table[0], bits,      64, TEMP_TABLE_BITS);
  if (i != 0)
    return (i);
  i = huffman_decode_create (table[1], bits + 64, 64, TEMP_TABLE_BITS);
  if (i != 0)
    return (i);
  BITIO_ENTER (*bitio);

  for (i = 0; i < n;)
  {
    HUFFMAN_DECODE (k, table[0], TEMP_TABLE_BITS);
    if (k <= 31)
    {
      length[i++] = (uchar) k;
      continue;
    }
    k &= 31;
    HUFFMAN_DECODE (j, table[1], TEMP_TABLE_BITS);
    if (j > 31)
    {
      int jj = j - 32;
      j = 1 << jj;
      if (jj != 0)
      {
        if (jj > 16)
        {
          j += BIORD (16) << (jj - 16); BIORD_MORE (16);
        }
        j += BIORD (jj); BIORD_MORE (jj);
      }
      j += 31;
    }
    j += MIN_REPT + i;
    if (j > n)
      return RET (COMP_ERR_BROKEN);
    do
      length[i] = (uchar) k;
    while (++i != j);
  }

ret:
  BITIO_LEAVE (*bitio);
  return (0);
}

/* ----------------------- Proper compression ------------------------- */
/*                         ------------------                           */

#if MIN_BLOCK_BITS > MAX_BLOCK_BITS || MAX_BLOCK_BITS > MAX_BITS_HALF*2
#error condition MIN_BLOCK_BITS <= MAX_BLOCK_BITS <= MAX_BITS_HALF*2 failed
#endif

#define DECODE_MAGIC    ((int) 0x5abc947fL)
#define BLOCK_MAGIC     ((int) 0x79a3f29dL)

#define START_BITS      13

typedef struct
{
  huffman_decode_t table[HUFFMAN_TABLE_SIZE (ALPHABET_SIZE, START_BITS)];
  int distance[MAX_DISTANCES];
  unsigned *crc, *blk;
  int
    block_size_log,     /* block_size is integral power of 2    */
    block_size,         /* 1 << block_size_log                  */
    last_block_size,    /* [original] size of last block        */
    n_blk,              /* total number of blocks               */
    comp_block_size,	/* size of largest compressed block+32	*/
    check_crc;          /* check CRC32?                         */
  uchar *comp;
  int magic;
}
decode_info;


typedef struct
{
  unsigned char * ptr;             /* pointer to the first decoded byte */
  int decoded;                     /* number of bytes decoded so far    */
  int total;                       /* total number of bytes in block    */
  int number;                      /* number of this block              */
} COMP_BLOCK_T;
/* Pointer to compressed data block                                     */

typedef struct
{
  COMP_BLOCK_T b;
  struct
  {
    uchar *first;
    int size;
  } orig, comp;
  struct
  {
    uchar *ptr, *src;
    int rept;
  } emit;
  bitio_t bitio;
  int n;
  int magic;
}
decode_block;

static void do_decode (decode_info *info, decode_block *block, uchar *e)
{
  BITIO_LOCALS;
  uchar *p, *s=0;
  int ch;

  if ((p = block->emit.ptr) >= e)
    return;
  if (p == block->orig.first)
  {
    BIORD_START (block->comp.first);
    block->emit.rept = 0;
  }
  else
  {
    BITIO_ENTER (block->bitio);
    if ((ch = block->emit.rept) != 0)
    {
      block->emit.rept = 0;
      s = block->emit.src;
      goto copy;
    }
  }
#define OVER if (p < e) goto over; break
  do
  {
  over:
    HUFFMAN_DECODE (ch, info->table, START_BITS);
    if ((ch -= 256) < 0)
    {
      *p++ = (uchar) ch;
      OVER;
    }

    s = p + info->distance[ch >> MAX_LENGTH_BITS];

    ch &= MAX_LENGTH - 1;
    if (ch <= 3)
    {
      p[0] = s[0]; p[1] = s[1]; p[2] = s[2]; p[3] = s[3]; p += ch+1; OVER;
    }
    else if (ch >= LONG_LENGTH)
    {
      ch -= LONG_LENGTH - LONG_BITS;
#if (MAX_BLOCK_BITS - 1) + (LONG_LENGTH - LONG_BITS) >= MAX_LENGTH
      if (ch == DELTA)
      {
        ch = BIORD (5); BIORD_MORE (5);
        ch += DELTA;
      }
#endif
      {
        int n = 1 << ch;
        if (ch > 16)
        {
          n += BIORD (16) << (ch -= 16); BIORD_MORE (16);
        }
        n += BIORD (ch); BIORD_MORE (ch);
        ch = n;
      }
      ch += LONG_LENGTH - (1 << LONG_BITS);
    }
    ++ch;
  copy:
    if (ch > 16)
    {
      if (p + ch > e)
      {
        block->emit.rept = ch - (int) (e - p);
        ch = (int) (e - p);
        goto copy;
      }
      do
      {
#       define X(i) p[i] = s[i]
        X(0); X(1); X(2); X(3); X(4); X(5); X(6); X(7); X(8);
        X(9); X(10); X(11); X(12); X(13); X(14); X(15);
#       undef X
        p += 16; s += 16;
      }
      while ((ch -= 16) > 16);
    }
    p += ch; s += ch;
    switch (ch)
    {
#     define X(i) case i: p[-i] = s[-i]
      X(16); X(15); X(14); X(13); X(12); X(11); X(10);
      X(9); X(8); X(7); X(6); X(5); X(4); X(3); X(2);
#     undef X
    }
    p[-1] = s[-1];
  }
  while (p < e);
#undef OVER
  block->emit.ptr = p;
  block->emit.src = s;
  BITIO_LEAVE (block->bitio);
}

/* pretty ugly */
static int comp_open_file (decode_info **res, FILE *fd, int check_crc)
{
  BITIO_LOCALS;
  bitio_t Bitio;
  uchar temp[ALPHABET_SIZE >= HEADER_SIZE ? ALPHABET_SIZE : HEADER_SIZE];
  uchar *ptr;
  int header_size, block_size, block_size_log, n_blk, i, n;
  decode_info *info;

  if (res == 0)
    return RET (COMP_ERR_PARAM);

  CRC32_init ();

  *res = 0;

  if (fread (temp, 1, HEADER_SIZE, fd) != HEADER_SIZE)
    return RET (COMP_ERR_READ);

  if (memcmp (temp, header_title, 64) != 0)
    return RET (COMP_ERR_READ);

  ptr = temp;
#define R4(i) \
  ((ptr[i] << 24) + (ptr[(i) + 1] << 16) + (ptr[(i) + 2] << 8) + (ptr[(i) + 3]))

  header_size = R4 (64);
  block_size_log = ptr[70];
  if (block_size_log > MAX_BITS || header_size < 84)
    return RET (COMP_ERR_BROKEN);
  block_size = 1 << block_size_log;
  if (ptr[71] != MAX_DISTANCES)
    return RET (COMP_ERR_BROKEN);
  n_blk = R4 (72);
  if (R4 (76) != (ALPHABET_SIZE << 12) + (LONG_BITS << 8) + (LONG_LENGTH << 4) + MAX_LENGTH_BITS)
    return RET (COMP_ERR_BROKEN);
#ifdef WINCE
  if ((ptr = (uchar*) my_Tcl_Alloc (header_size)) == 0)
#else
  if ((ptr = (uchar*) malloc (header_size)) == 0)
#endif
    return RET (COMP_ERR_NOMEM);

  if (fread (ptr + HEADER_SIZE, 1, header_size - HEADER_SIZE, fd) != (size_t) (header_size - HEADER_SIZE))
  {
#ifdef WINCE
    my_Tcl_Free ((char*)ptr);
#else
    free (ptr);
#endif
    return RET (COMP_ERR_NOMEM);
  }
  memcpy (ptr, temp, HEADER_SIZE);
  header_size -= 4;
  if (CRC32 (ptr, header_size, 0) != (unsigned) R4 (header_size))
  {
#ifdef WINCE
    my_Tcl_Free ((char*)ptr);
#else
    free (ptr);
#endif
    return RET (COMP_ERR_BROKEN);
  }
  header_size += 4;

  n = sizeof (info->crc[0]) * (1 + (check_crc ? (3 * n_blk) : n_blk));
#ifdef WINCE
  if ((info = (decode_info *) my_Tcl_Alloc (sizeof (*info) + n)) == 0)
#else
  if ((info = (decode_info *) malloc (sizeof (*info) + n)) == 0)
#endif
  {
#ifdef WINCE
    my_Tcl_Free ((char*)ptr);
#else
    free (ptr);
#endif
    return RET (COMP_ERR_NOMEM);
  }
  cbEGTBCompBytes += sizeof (*info) + n;
  info->blk = info->crc = (unsigned *) (info + 1);
  if (check_crc) info->blk += 2 * n_blk;

  info->check_crc = check_crc;
  info->block_size_log = block_size_log;
  info->block_size = block_size;
  info->n_blk = n_blk;

  if (check_crc)
  {
    n_blk <<= 1; i = HEADER_SIZE;
    for (n = 0; n < n_blk; ++n)
    {
      info->crc[n] = R4 (i);
      i += 4;
    }
    n_blk >>= 1;
  }

  i = HEADER_SIZE + (n_blk << 3);
  BIORD_START (ptr + i);

  info->comp_block_size = 0;
  for (n = 0; n <= n_blk; ++n)
  {
    if ((info->blk[n] = BIORD (block_size_log)) == 0)
      info->blk[n] = block_size;
    if (info->comp_block_size < (int) (info->blk[n]))
      info->comp_block_size = (int) (info->blk[n]);
    BIORD_MORE (block_size_log);
  }
  info->comp_block_size += 32;

  for (n = 0; n < MAX_DISTANCES; ++n)
  {
    info->distance[n] = - ((int) BIORD (block_size_log));
    BIORD_MORE (block_size_log);
  }

  i += ((n_blk + 1 + MAX_DISTANCES) * block_size_log + 7) >> 3;
  BIORD_START (ptr + i);
  BITIO_LEAVE (Bitio);
  if (huffman_read_length (&Bitio, temp, ALPHABET_SIZE) != 0)
  {
#ifdef WINCE
    my_Tcl_Free ((char*)info);
    my_Tcl_Free ((char*)ptr);
#else
    free (info);
    free (ptr);
#endif
    return RET (COMP_ERR_BROKEN);
  }

  if (huffman_decode_create (info->table, temp, ALPHABET_SIZE, START_BITS) != 0)
  {
#ifdef WINCE
    my_Tcl_Free ((char*)info);
    my_Tcl_Free ((char*)ptr);
#else
    free (info);
    free (ptr);
#endif
    return RET (COMP_ERR_BROKEN);
  }

  info->last_block_size = info->blk[n_blk];
  info->blk[n_blk] = 0;
  for (n = 0; n <= n_blk; ++n)
  {
    i = info->blk[n];
    info->blk[n] = header_size;
    header_size += i;
  }

#ifdef WINCE
    my_Tcl_Free ((char*)ptr);
#else
    free (ptr);
#endif
  info->comp  = 0;
  info->magic = DECODE_MAGIC;
  *res = info;

  return (COMP_ERR_NONE);
}

// static int comp_tell_blocks (decode_info *info)
// {
//   if (info == 0 || info->magic != DECODE_MAGIC)
//     return (-1);
//   return (info->n_blk);
// }

static int comp_init_block (decode_block *block, int block_size, uchar *orig)
{
  if (block == 0)
    return RET (COMP_ERR_PARAM);
  block->orig.first = orig;
  block->comp.first = (uchar *) (block + 1);
  block->b.ptr = 0;
  block->b.decoded = -1;
  block->b.total = -1;
  block->b.number = -1;
  block->n = -1;
  block->magic = BLOCK_MAGIC;
  return (COMP_ERR_NONE);
}

static int comp_alloc_block (decode_block **ret_block, int block_size)
{
  decode_block *block;

  if (ret_block == 0)
    return RET (COMP_ERR_PARAM);
  *ret_block = 0;
#ifdef WINCE
  if ((block = (decode_block *) my_Tcl_Alloc (sizeof (*block) + block_size)) == 0)
#else
  if ((block = (decode_block *) malloc (sizeof (*block) + block_size)) == 0)
#endif
    return RET (COMP_ERR_NOMEM);
  cbEGTBCompBytes += sizeof (*block) + block_size;
  if (0 != comp_init_block (block, block_size, NULL))
    return RET (COMP_ERR_PARAM);
  *ret_block = block;
  return (COMP_ERR_NONE);
}

#define RETURN(n) \
  return ((n) == COMP_ERR_NONE ? COMP_ERR_NONE : RET (n));

static int comp_read_block (decode_block *block, decode_info *info, FILE *fd, int n)
{
  int comp_size, orig_size;
  uchar *comp, *orig;

  if (block == 0 || block->magic != BLOCK_MAGIC)
    return RET (COMP_ERR_PARAM);
  assert (info->magic == DECODE_MAGIC);
  if ((unsigned) n >= (unsigned) info->n_blk)
    RETURN (COMP_ERR_PARAM);
  comp = block->comp.first;
  block->n = n;
  orig = block->orig.first;
  orig_size = info->block_size;
  if (n == info->n_blk-1) orig_size = info->last_block_size;
  block->orig.size = orig_size;
  block->comp.size = comp_size = info->blk[n+1] - info->blk[n];
  if (fseek (fd, info->blk[n], SEEK_SET) != 0)
    RETURN (COMP_ERR_READ);
  if (fread (comp, 1, comp_size, fd) != (size_t) comp_size)
    RETURN (COMP_ERR_READ);
  if (info->check_crc && info->crc[(n << 1) + 1] != CRC32 (block->comp.first, comp_size, 0))
    RETURN (COMP_ERR_BROKEN);
  block->emit.rept = 0;
  if (comp_size == orig_size)
  {
    memcpy (orig, comp, comp_size);
    block->emit.ptr = orig + comp_size;
    block->b.decoded = comp_size;
  }
  else
  {
    block->emit.ptr = orig;
    block->b.decoded = 0;
  }
  block->b.number = n;
  block->b.ptr = orig;
  block->b.total = orig_size;

  RETURN (COMP_ERR_NONE);
}

static int comp_decode_and_check_crc (decode_block *block, decode_info *info, int n, int check_crc)
{
  if (block == 0 || block->magic != BLOCK_MAGIC)
    return RET (COMP_ERR_PARAM);
  assert (info->magic == DECODE_MAGIC);
  if ((unsigned) (n - 1) > (unsigned) (block->orig.size - 1))
    RETURN (COMP_ERR_PARAM);
  if (check_crc) n = block->orig.size;
  do_decode (info, block, block->orig.first + n);
  block->b.ptr = block->orig.first;
  block->b.total = block->orig.size;
  if (block->b.decoded >= block->b.total)
  {
    if (block->b.decoded > block->b.total)
      RETURN (COMP_ERR_BROKEN);
    if (block->emit.rept != 0)
      RETURN (COMP_ERR_BROKEN);
  }
  if (check_crc && info->check_crc
    && info->crc[block->n << 1] != CRC32 (block->orig.first, block->orig.size, 0))
    RETURN (COMP_ERR_BROKEN);

  RETURN (COMP_ERR_NONE);
}

#if !defined (COLOR_DECLARED)

/*
  Test driver
*/

#define	CRC_CHECK	1

int main (int argc, char *argv[])
{
  int i;
  int size;
  int result;
  FILE *fp;
  decode_info *comp_info;
  decode_block *comp_block;
  clock_t tStart, tEnd;
  double dSeconds;
  uchar	rgbBuf [8192+32];

  if (2 != argc)
  {
    printf ("Invalid arguments\n");
	exit (1);
  }
  fp = fopen (argv[1], "rb");
  if (0 == fp)
  {
    printf ("Unable to open file\n");
	exit (1);
  }
  result = comp_open_file (&comp_info, fp, CRC_CHECK);
  if (0 != result)
  {
    printf ("Unable to read file (1): %d\n", result);
	exit (1);
  }
  if (8192 != comp_info->block_size)
  {
    printf ("Invalid block size: %d\n", comp_info->block_size);
	exit (1);
  }
  result = comp_alloc_block (&comp_block, comp_info->block_size);
  if (0 != result)
  {
    printf ("Unable to allocate block: %d\n", result);
	exit (1);
  }

  size = 0;
  tStart = clock ();
  for (i = 0; i < comp_info->n_blk; i ++)
  {
    if (0 != (result = comp_init_block (comp_block, comp_info->block_size, rgbBuf)))
	{
      printf ("Unable to init block: %d\n", result);
	  exit (1);
	}
	if (0 != (result = comp_read_block (comp_block, comp_info, fp, i)))
    {
      printf ("Unable to read block: %d\n", result);
	  exit (1);
	}
	size += comp_block->orig.size;
	if (0 != (result = comp_decode_and_check_crc (comp_block, comp_info, comp_block->orig.size, CRC_CHECK)))
    {
      printf ("Unable to decode block: %d\n", result);
	  exit (1);
	}
  }
  tEnd = clock ();
  dSeconds = (double) (tEnd - tStart) / CLOCKS_PER_SEC;
  printf ("Total memory allocated: %dKb\n", (cbEGTBCompBytes+1023)/1024);
  printf ("%g seconds, %dMb, %gMb/sec)\n", dSeconds, size/(1024*1024), size/(1024*1024) / dSeconds);
  return 0;
}

#endif
#endif

#endif // WINCE

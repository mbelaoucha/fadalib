#define IDENTIFIER 257
#define CONSTANT 258
#define STRING_LITERAL 259
#define SIZEOF 260
#define PTR_OP 261
#define INC_OP 262
#define DEC_OP 263
#define LEFT_OP 264
#define RIGHT_OP 265
#define MUL_ASSIGN 266
#define DIV_ASSIGN 267
#define MOD_ASSIGN 268
#define ADD_ASSIGN 269
#define SUB_ASSIGN 270
#define LEFT_ASSIGN 271
#define RIGHT_ASSIGN 272
#define AND_ASSIGN 273
#define XOR_ASSIGN 274
#define OR_ASSIGN 275
#define TYPE_NAME 276
#define TYPEDEF 277
#define EXTERN 278
#define STATIC 279
#define AUTO 280
#define REGISTER 281
#define INLINE 282
#define RESTRICT 283
#define CHAR 284
#define SHORT 285
#define INT 286
#define LONG 287
#define SIGNED 288
#define UNSIGNED 289
#define FLOAT 290
#define DOUBLE 291
#define CONST 292
#define VOLATILE 293
#define VOID 294
#define BOOL 295
#define COMPLEX 296
#define IMAGINARY 297
#define STRUCT 298
#define UNION 299
#define ENUM 300
#define ELLIPSIS 301
#define CASE 302
#define DEFAULT 303
#define IF 304
#define ELSE 305
#define SWITCH 306
#define WHILE 307
#define DO 308
#define FOR 309
#define GOTO 310
#define CONTINUE 311
#define BREAK 312
#define RETURN 313
#define OR_OP 314
#define AND_OP 315
#define LE_OP 316
#define GE_OP 317
#define EQ_OP 318
#define NE_OP 319
#define NEGATION 320
#define __TRUE 321
#define __FALSE 322
typedef union   {
   /*infos info;*/
   struct   {
      char   __ID[200];
      int    __Value;
      void*   __generic;
      }info;
   } YYSTYPE;
extern YYSTYPE yylval;

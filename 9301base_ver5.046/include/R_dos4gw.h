/**/

/* ＤＯＳコール用レジスタ待避テーブル */
typedef struct rminfo {
	long EDI;
	long ESI;
	long EBP;
	long reserved_by_system;
	long EBX;
	long EDX;
	long ECX;
	long EAX;
	short flags;
	short ES,DS,FS,GS,IP,CS,SP,SS;
} RMI;

typedef struct _gdt32 {
	unsigned short	limit0to15;
	unsigned short	base0to15;
	unsigned char	base16to23;
	unsigned char	access;
	unsigned char	limbits;        /* limit16to19 and bits */
	unsigned char	base24to31;
} GDT32;

int R_rminterrupt(int func,RMI *d32r);
unsigned long R_dosmemalloc(int size, unsigned long *psel);
unsigned int R_dosmemfree (unsigned long sel);
int R_descalloc(int count);
int R_setdesc(int sel, GDT32 *dp);

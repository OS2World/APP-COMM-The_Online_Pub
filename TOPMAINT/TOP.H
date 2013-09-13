
#if defined(__OS2__) || defined(__WIN32__)
#define XFAR
#define XINT short int
typedef struct _date
    {
    XINT          da_year;        /* Year - 1980 */
    char          da_day;     /* Day of the month */
    char          da_mon;     /* Month (1 = Jan) */
    } XDATE;
typedef enum
    {
    RK_UNREGISTERED,
    RK_REGISTERED
    } RKVALID;
#else
#define XFAR far
#define XINT int
#define LIBENTRY
#define LIBDATA
typedef struct date XDATE;
#endif

#define MAXSTRLEN      255L
#define MAXWORDS       (XINT) (MAXSTRLEN / 2)
#define MAXOPENRETRIES 1024L
#define MAXCHANSPECNODES 100
#define MAXBIOQUES	   100L
#define BUSYCHANNEL    0xFFFFFFFFUL
#define MAXCHANNEL     0xFFFFFFFEUL

#define PREF1_ECHOACTIONS   0x01
#define PREF1_ECHOTALKTYP   0x02
#define PREF1_ECHOTALKMSG   0x04
#define PREF1_MSGSENT       0x08
#define PREF1_ECHOGA        0x10
#define PREF1_TALKMSGSENT   0x20
#define PREF1_DUALWINDOW    0x40
#define PREF1_BLKWHILETYP   0x80

#define PREF2_SINGLECHAR    0x01
#define PREF2_CHANLISTED    0x02
#define PREF2_ECHOOWNTEXT   0x04
#define PREF2_ACTIONSOFF    0x08

typedef struct stringarray1_str
	{
    unsigned char XFAR string[11];
    } stringarray1;

typedef struct stringarray2_str
	{
    unsigned char XFAR string[31];
    } stringarray2;

typedef struct user_data_str
	{
    unsigned char realname[41];
    unsigned char handle[31];
    unsigned char unusedspace[61];
    struct pers_act_typ
    	{
        XINT          type;
        unsigned char verb[11];
	    unsigned char response[61];
    	unsigned char singular[61];
	    unsigned char plural[61];
        } persact[4];
    XDATE         last_use;
    XINT          gender;
    char          datefmt;
    char          timefmt;
    unsigned char pref1;
    unsigned char pref2;
    unsigned char pref3;
    unsigned char password[16];
    unsigned char age;
    unsigned char emessage[81];
    unsigned char xmessage[81];
    unsigned long cybercash;
    unsigned XINT security;
    unsigned char defchantopic[71];
    char          donebio;
    char reserved[2048 - 1185];
    } user_data_typ;

typedef struct node_idx_str
	{
    unsigned XINT structlength;
    unsigned char handle[31];
    unsigned char realname[41];
    unsigned XINT speed;
    unsigned char location[31];
    XINT          gender;
    char          quiet;
    unsigned XINT task;
    time_t        lastaccess;
    unsigned long channel;
    char          channellisted;
    unsigned XINT security;
    char          actions;
    } node_idx_typ;

typedef struct msg_str
	{
    unsigned XINT structlength;
    XINT          type;
    XINT          from;
    XINT          doneto;
    XINT          gender;
    unsigned char handle[31];
    unsigned char string[256]; /////////////Change To Dynamic!
    unsigned long channel;
    unsigned XINT minsec;  /* Minimum Security to recv msg. */
	unsigned XINT maxsec;  /* Maximum Security to recv msg. */
    long          data1;
    } msg_typ;

/*//|typedef struct slots_stat_str
	{
    unsigned long pulls;
    unsigned long spent;
    unsigned long hits[10];
    unsigned long totals[10];
    } slots_stat_typ;*///|

typedef struct lang_text_str
    {
    unsigned char idtag[31];
    XINT          length;
    unsigned char XFAR *string; /* Points to a malloc()ed buffer in memory */
    } lang_text_typ;

typedef struct bbsnodedata_str
	{
    unsigned char handle[31];
    unsigned char realname[41];
    XINT          node;
    long          speed;
    unsigned char location[31];
    unsigned char statdesc[81];
    char          quiet;
    char          gender;
    char          hidden;
    unsigned XINT numcalls;
    unsigned char infobyte;

    unsigned char attribs1;
    unsigned XINT existbits;
	} bbsnodedata_typ;

typedef struct editor_field_str
	{
    char enabled;
    unsigned char inptype;
    char noinput;
    void *varbuffer;
    unsigned char *title;
    unsigned char *fmtstring;
    XINT titlerow, titlecol;
    XINT fieldrow, fieldcol;
    unsigned char titleattrib;
    unsigned char tihiltattrib;
    unsigned char tidisattrib;
    unsigned char normalattrib;
    unsigned char highltattrib;
    unsigned char bgchar;
    unsigned XINT flags;
    long minval, maxval;
    XINT minlen, maxlen;
    char (*verifyfunc)(void *fieldinf);
    void (*noinputfunc)(unsigned char *buffer, void *fieldinf);
    } editor_field_typ;

typedef struct channel_data_str
    { // This is too memory consuming - strings need to be dynamic.
    unsigned long channel;
    unsigned char topic[71];
    unsigned char joinaliases[31];
    unsigned XINT minsec;
    unsigned XINT maxsec;
    unsigned XINT modsec;
    char          listed;
    } channel_data_typ;

typedef struct action_file_str
    {
    unsigned long datstart; /* Location of first action's data. */
    unsigned long txtstart; /* Location of first action's text. */
    unsigned char name[31];
    unsigned XINT minsec;
    unsigned XINT maxsec;
    unsigned long minchannel;
    unsigned long maxchannel;
    unsigned long numactions;
    } action_file_typ;

typedef struct action_data_str
    {
    char          type;   /* -1 = deleted */
    unsigned char verb[11];
    unsigned long textofs; /* From txtstart */
    unsigned XINT responselen; /* 0 = N/A */
    unsigned XINT singularlen; /* 0 = N/A */
    unsigned XINT plurallen;  /* 0 = N/A */
    } action_data_typ;

typedef struct action_ptr_str
    {
    unsigned char *responsetext;
    unsigned char *singulartext;
    unsigned char *pluraltext;
    } action_ptr_typ;

typedef struct action_rec_str
    {
    action_data_typ data;
    action_ptr_typ ptrs;
    } action_rec_typ;

typedef struct chan_idx_str
    {
    unsigned XINT structlength;
    unsigned long channum;
    char          type;
    XINT          usercount;
    unsigned char topic[71];
    XINT          modnode;
    /* specnodes: banned for open channels, invited for closed. */
    XINT          specnode[MAXCHANSPECNODES];
    } chan_idx_typ;

typedef struct spawn_prog_str
    {
    unsigned char name[41];
    unsigned char cmds[81];
    unsigned char cmdline[129];
    } spawn_prog_typ;

typedef struct censor_word_str
    {
    char wholeword;
    XINT level;
    char *word;
    char *changeto;
    } censor_word_typ;

typedef struct bio_ques_str
    {
    XINT number;
    unsigned char field[31];
    char resptype;
    long minval;
    long maxval;
    } bio_ques_typ;

typedef struct bio_resp_str
    {
    unsigned char response[71];
    unsigned char reserved[128 - 71];
    } bio_resp_typ;

#define RA_HIDDEN    0x01
#define RA_WANTCHAT  0x02
#define RA_RESERVED  0x04
#define RA_NODISTURB 0x08
#define RA_READY     0x40

typedef struct RA_USERON_str
	{
    unsigned char name[36];      /*   0 */
    unsigned char handle[36];    /*  36 */
    unsigned char line;          /*  72 */
    unsigned XINT baud;          /*  73 */
    unsigned char city[26];      /*  75 */
    unsigned char status;        /* 101 */
    unsigned char attribute;     /* 102 */
    unsigned char statdesc[11];  /* 103 */
    unsigned char freespace[98]; /* 114 */
    unsigned XINT numcalls;      /* 212 */ /* Total = 214 */
    } RA_USERON_typ;

#define EZY_NODISTURB 0x01

typedef struct EZY_ONLINE_str
    {
    unsigned char name[36];
    unsigned char alias[36];
    unsigned char status;
    unsigned char attribute;
    long          baud;
    unsigned char location[26];
    } EZY_ONLINE_typ;

#define SBBS_INUSE        0x01
#define SBBS_NODISTURB    0x02
#define SBBS_WAITING      0x04
#define SBBS_HIDDEN       0x08

typedef struct SBBS_USERON_str
	{
    unsigned char name[36];
    unsigned char attribute;
    unsigned char status;
    unsigned XINT baud;
    unsigned char city[26];
	unsigned char infobyte;
    unsigned char freespace[9];
    } SBBS_USERON_typ;


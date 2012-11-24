#ifndef prot_INCL
#define prot_INCL
#include "md5.h"
void md5_init(md5_state_t *pms);
void md5_append(md5_state_t *pms, const md5_byte_t *data, int nbytes);
void md5_finish(md5_state_t *pms, md5_byte_t digest[16]);
int md5CalcFileMD5 (char *fileName, char *hexDigest);
int md5AreMD5sEqual (char *MD5One, char *MD5Two);
int md5GetMD5FromFile (char *fileName, char *hexDigest);
int md5StoreMD5ToFile (char *fileName, char *hexDigest);
void utilAppendPathToFileName (char *fileName, char *retFileName);
int utilFileToString (char *fileName, char **content, long *fileSize);
int remodfileProcessRemodfileModif ();
int remodfileProcessRemodfile ();
int treeConstructTree (char *argument);
#endif /* prot_INCL */

#ifndef prot_INCL
#define prot_INCL

/* Initialize the MD5 algorithm. */
void md5_init(md5_state_t *pms);

/* Append a string to the message. */
void md5_append(md5_state_t *pms, const md5_byte_t *data, int nbytes);

/* Finish the message and return the digest. */
void md5_finish(md5_state_t *pms, md5_byte_t digest[16]);

int md5CalcFileMD5 (char *fileName, char *hexDigest);

int md5CompareMD5 (char *MD5One, char *MD5Two);
#endif /* prot_INCL */

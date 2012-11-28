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
int md5DetectFileModif (char *fileName);
int md5CalcAndStoreMD5 (char *fileName);
void utilAppendPathToFileName (char *fileName, char *retFileName);
int utilFileToString (char *fileName, char **content, long *fileSize);
int utilFindIfFileExists (char *fileName);
int remodfileProcessRemodfileModif ();
int remodfileProcessRemodfile ();
int treeConstructTree (char *argument);
void treeBuildLeafList (TreeDepNode *treeDepNode, TreeLeafNode **treeLeafHead,
                        TreeLeafNode **treeLeafTail);
void treeFindTargetNodeFromArg (char *argument, TargNode **targNode);
void treeFindProdForTarget (TargNode *targNode, ProdNode **prodNode);
void treeRemoveNodeFromTree (TreeLeafNode *treeLeafNode);
void treeRemoveLeafFromTree (TreeLeafNode **Head, TreeLeafNode **Tail,
														TreeLeafNode *treeLeafNode);
void treeLocateTreeLeafFromPID (TreeLeafNode *treeLeafHead, pid_t processID,
                                TreeLeafNode **locatedTreeLeaf);
int executeStartRemodel ();
#endif /* prot_INCL */

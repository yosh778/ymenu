#ifndef _UNZIPRAR_H
#define _UNZIPRAR_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * If the destpath folder doesn't exist, it'll be created
 */
int unzipToDir(const char *zippath, const char *destpath, const char *pass
																			#ifdef __cplusplus
																			= NULL
																			#endif
																					);



/*
 * Pass 1 to activate password use, 0 to deactivate
 */
void SetUsepassword(int p);

int GetUsePassword();



/*
 *
 * If the destpath folder doesn't exist, it'll be created
 * Doesn't work for RAR files with higher compression rates than the "Normal" one
 */
void DoExtractRAR(const char *rarfile,const char *extDir,const char *pass
																			#ifdef __cplusplus
																			= NULL
																			#endif
																					);

#ifdef __cplusplus
}
#endif


#endif

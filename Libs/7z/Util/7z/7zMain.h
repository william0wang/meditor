#ifndef _7ZDEC_DLL_
#define _7ZDEC_DLL_

#ifdef __cplusplus
extern "C" {
#endif

int Decode7zipFile(const char *zfilename, const wchar_t *decode_path, const wchar_t *only_filename, const wchar_t *ignore_path_name, const wchar_t *ignore_files[], int ignore_num);

#ifdef __cplusplus
}
#endif

#endif
#include <stdbool.h>
#include <string.h>

#include "resource.h"
#include "whereami.h"

#ifdef _WIN32
#include <windows.h>

bool IsDirectory(const char *path)
{
	DWORD attributes = GetFileAttributes(path);
	if (attributes == INVALID_FILE_ATTRIBUTES) {
		return false;
	}
	return (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

#else
#include <sys/stat.h>

bool IsDirectory(const char *path)
{
	struct stat statbuf;
	if (stat(path, &statbuf) != 0) {
		return false;
	}
	return S_ISDIR(statbuf.st_mode);
}

#endif

int GetResourcesPath(char *out, int capacity)
{
	int execPathLength = wai_getExecutablePath(NULL, 0, NULL);
	if (execPathLength <= 0) {
		return -1;
	}

	/* Calling wai_getExecutablePath() twice due to issue 43:
	 * https://github.com/gpakosz/whereami/issues/43. */
	int execDirLength;
	char path[execPathLength];
	wai_getExecutablePath(path, execPathLength, &execDirLength);
	path[execDirLength] = '\0';

	int resourcePathLength = execDirLength + strlen(RESOURCES_ROOT_DIR);

	/* Write to the output buffer. */
	if (out != NULL) {
		if (capacity < resourcePathLength + 1) {
			/* Unable to write path to output */
			return -1;
		}
		sprintf(out, "%s%s", path, RESOURCES_ROOT_DIR);
	}

	return IsDirectory(out) ? resourcePathLength : -1;
}

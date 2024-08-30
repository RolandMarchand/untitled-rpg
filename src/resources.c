#include <string.h>

#include "resources.h"
#include "whereami.h"

#define RESOURCES_ROOT_DIR "/resources"

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

int GetResourcesPath(char *out, size_t capacity)
{
	int execPathLength = wai_getExecutablePath(NULL, 0, NULL);
	if (execPathLength <= 0) {
		return -1;
	}

	/* Calling wai_getExecutablePath() twice due to issue 43:
	 * https://github.com/gpakosz/whereami/issues/43. */
	char *execDirPath = (char*)malloc(execPathLength + 1);
	if (execDirPath == NULL) {
		perror("Unable to allocate memory to get the resources path");
		return -1;
	}

	int execDirLength;
	wai_getExecutablePath(execDirPath, execPathLength, &execDirLength);
	execDirPath[execDirLength] = '\0';

	size_t resourcePathLength = execDirLength + strlen(RESOURCES_ROOT_DIR);

	if (out == NULL) {
		return (int)resourcePathLength;
	}

	/* Write to the output buffer. */
	if (capacity < resourcePathLength + 1) {
		/* Unable to write path to output */
		free(execDirPath);
		return -1;
	}
	snprintf(out, capacity, "%s%s", execDirPath,
			RESOURCES_ROOT_DIR);
	free(execDirPath);
	return IsDirectory(out) ? (int)resourcePathLength : -1;
}

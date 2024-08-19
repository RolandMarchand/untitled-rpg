#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <ftw.h>
#include <limits.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <signal.h>

#include "resource.h"
#include "miniz.c"

#define RESOURCE_FILENAME "resource.zip"
/* TODO: complete the game's name. */
#define UNPACKED_RESOURCE_PATH "/tmp/untitled_game"

/* Write the executable path to execPath. Expect execPath
 * to be of size PATH_MAX. In case of an error, set execPath to "", set errno
 * and print to stderr. */
void GetExecPath(char *execPath)
{
	/* TODO: add support for non-Linux platforms. */
	ssize_t len = readlink("/proc/self/exe", execPath, PATH_MAX - 1);

	if (len == -1) {
		fprintf(stderr,
			"Error %d: unable to get the executable's path: %s\n",
			errno, strerror(errno));
		execPath[0] = '\0';
		return;
	}

	execPath[len] = '\0'; /* readlink() does not null terminate strings. */
	dirname(execPath);
}

/* Write the resource path to resourcePath. Expect resourcePath to be of size
 * PATH_MAX. In case of an error, set resourcePath to "", set errno and print to
 * stderr. */
void GetResourcePath(char *resourcePath)
{	char execPath[PATH_MAX];
	GetExecPath(execPath);
	sprintf(resourcePath, "%s/%s", execPath, RESOURCE_FILENAME);
}

int CleanResourceCallback(const char *path, const struct stat *statbuf, int typeflag, struct FTW *ftwbuf)
{
	int result;
	(void)statbuf; /* Unused parameter. */
	(void)ftwbuf; /* Unused parameter. */

	switch(typeflag) {
	case FTW_F:
	case FTW_SL:
	case FTW_SLN:
		/* Delete regular files. */
		result = unlink(path);
		break;
	case FTW_D:
	case FTW_DP:
		/* Delete directories (will only be empty directories). */
		result = rmdir(path);
		break;
	default:
		/* Unsupported file type. */
		result = -1;
	}

	if (result != 0) {
		perror("Error: failed to delete " UNPACKED_RESOURCE_PATH);
		return -1; /* Abort traversal on error */
	}

	return 0; /* Continue traversal */
}

void CleanResource() {
	if (access(UNPACKED_RESOURCE_PATH, F_OK) == 0) {
		nftw(UNPACKED_RESOURCE_PATH, CleanResourceCallback, 64,
		     FTW_DEPTH | FTW_PHYS);
		rmdir(UNPACKED_RESOURCE_PATH);
	}
}

void SignalHandler(int sig) {
	CleanResource();
	exit(sig);
}

ResourceError RegisterTmpDir(char *tmp)
{
	CleanResource();

	/* Create resource dir in /tmp. */
	if (mkdir(UNPACKED_RESOURCE_PATH, S_IRWXU) != 0) {
		return ERR_LOADING_RESOURCES;
	}

	/* Register the function to be called on exit. */
	if (atexit(CleanResource) != 0) {
		perror("Failed to register exit handler for resource cleanup\n");
		return ERR_LOADING_RESOURCES;
	}

	/* Register signal handling for resource cleanup. */
	int signals[] = {SIGINT, SIGABRT, SIGTERM};
	for (int i = 0; i < sizeof(signals) / sizeof(int); i++) {
		if (signal(signals[i], SignalHandler) == SIG_ERR) {
			perror("Failed to register resource cleanup signal handler");
			return ERR_LOADING_RESOURCES;
		}
	}

	return ERR_OK;
}

/* Extract a zip archive to a temporary directory and return a resource. */
Resource ExtractResource(const char *archivePath) {
	mz_zip_archive zip = {0};
	Resource res = {.error = ERR_OK, .path = UNPACKED_RESOURCE_PATH};

	/* Initialize the archive reader. */
	if (!mz_zip_reader_init_file(&zip, archivePath, 0)) {
		fprintf(stderr,
			"Error: unable to open resource: %s\n", archivePath);
		res.error = ERR_LOADING_RESOURCES;
		res.path[0] = '\0';
		return res;
	}

	/* Register disk space to load resources. */
	if (RegisterTmpDir(res.path) != ERR_OK) {
		res.error = ERR_LOADING_RESOURCES;
		res.path[0] = '\0';
		return res;
	}

	/* Iterate over every file of the archive. */
	int fileCount = mz_zip_reader_get_num_files(&zip);
	char filePath[PATH_MAX];
	for (int i = 0; i < fileCount; i++) {
		mz_zip_archive_file_stat fileStat;
		if (!mz_zip_reader_file_stat(&zip, i, &fileStat)) {
			fprintf(stderr,
				"Error: unable to open resource: %s\n",
				archivePath);
			mz_zip_reader_end(&zip);
			res.error = ERR_LOADING_RESOURCES;
			res.path[0] = '\0';
			return res;
		}

		snprintf(filePath, PATH_MAX, "%s/%s", res.path, fileStat.m_filename);

		if (mz_zip_reader_is_file_a_directory(&zip, i)) {
			mkdir(filePath, S_IRWXU);
			continue;
		}

		if (!mz_zip_reader_extract_to_file(&zip, i, filePath, 0)) {
			fprintf(stderr, "Error: failed to extract file %s\n", fileStat.m_filename);
			mz_zip_reader_end(&zip);
			res.error = ERR_LOADING_RESOURCES;
			res.path[0] = '\0';
			return res;
		}
	}

	mz_zip_reader_end(&zip);
	return res;
}

Resource LoadResource()
{
	char resourcePath[PATH_MAX];
	GetResourcePath(resourcePath);

	if (resourcePath[0] == '\0') {
		return (Resource){.error = ERR_RESOURCE_NOT_FOUND, .path = ""};
	}

	Resource res = ExtractResource(resourcePath);

	return res;
}

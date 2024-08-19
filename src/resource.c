#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <ftw.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>

#include "common.h"
#include "resource.h"
#include "miniz.c"

#define RESOURCE_FILENAME "resource.zip"
/* TODO: complete the game's name. */

/* Write the executable path to execPath. Expect execPath
 * to be of size PATH_MAX. In case of an error, set execPath to "", set errno
 * and print to stderr. */
void GetExecPath(char *execPath)
{
        assert(execPath != NULL);

        /* TODO: add support for non-Linux platforms. */
        ssize_t len = readlink("/proc/self/exe", execPath, PATH_MAX - 1);

        if (len == -1) {
		ERROR("unable to get the executable's path: %s\n", strerror(errno));
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
{
        assert(resourcePath != NULL);

        char execPath[PATH_MAX];
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
		ERROR("%s: failed to delete %s\n", strerror(errno), path);
                return -1; /* Abort traversal on error */
        }

        return 0; /* Continue traversal */
}

/* Cleanup all game assets from /tmp when the game is not running. */
void CleanResource() {
        if (access(RESOURCE_PATH, F_OK) == 0) {
                nftw(RESOURCE_PATH, CleanResourceCallback, 64,
                     FTW_DEPTH | FTW_PHYS);
        }
}

void SignalHandler(int sig) {
        CleanResource();
        exit(sig);
}

Error RegisterTmpDir()
{
        CleanResource();

        /* Create resource dir in /tmp. */
        if (mkdir(RESOURCE_PATH, S_IRWXU) != 0) {
                return ERR_LOADING_RESOURCES;
        }

        /* Register the function to be called on exit. */
        if (atexit(CleanResource) != 0) {
                perror("Failed to register exit handler for resource cleanup\n");
                return ERR_LOADING_RESOURCES;
        }

        /* Register signal handling for resource cleanup. */
        int signals[] = {SIGINT, SIGABRT, SIGTERM};
        for (size_t i = 0; i < sizeof(signals) / sizeof(int); i++) {
                if (signal(signals[i], SignalHandler) == SIG_ERR) {
			ERROR("%s: failed to register resource cleanup signal handler for signal %d\n",
			      strerror(errno), signals[i]);
                        return ERR_LOADING_RESOURCES;
                }
        }

        return ERR_OK;
}

/* Extract a zip archive to a temporary directory and return a resource. */
Error ExtractResource(const char *archivePath) {
        assert(archivePath != NULL);

        mz_zip_archive zip = {0};

        /* Initialize the archive reader. */
        if (!mz_zip_reader_init_file(&zip, archivePath, 0)) {
		ERROR("unable to open resource: %s\n", archivePath);
                return ERR_LOADING_RESOURCES;
        }

        /* Register disk space to load resources. */
        if (RegisterTmpDir() != ERR_OK) {
                return ERR_LOADING_RESOURCES;
        }

        /* Iterate over every file of the archive. */
        int fileCount = mz_zip_reader_get_num_files(&zip);
        char filePath[PATH_MAX + MZ_ZIP_MAX_ARCHIVE_FILENAME_SIZE];
        for (int i = 0; i < fileCount; i++) {
                mz_zip_archive_file_stat fileStat;
                if (!mz_zip_reader_file_stat(&zip, i, &fileStat)) {
			ERROR("unable to open resource: %s\n", archivePath);
                        mz_zip_reader_end(&zip);
                        return ERR_LOADING_RESOURCES;
                }

                snprintf(filePath, sizeof(filePath), RESOURCE_PATH "/%s",
			 fileStat.m_filename);

                if (mz_zip_reader_is_file_a_directory(&zip, i)) {
                        mkdir(filePath, S_IRWXU);
                        continue;
                }

                if (!mz_zip_reader_extract_to_file(&zip, i, filePath, 0)) {
			ERROR("failed to extract file %s\n", fileStat.m_filename);
                        mz_zip_reader_end(&zip);
                        return ERR_LOADING_RESOURCES;
                }
        }

        mz_zip_reader_end(&zip);
        return ERR_OK;
}

Error LoadResource()
{
        char resourcePath[PATH_MAX];
        GetResourcePath(resourcePath);

        if (resourcePath[0] == '\0') {
                return ERR_RESOURCE_NOT_FOUND;
        }

        return ExtractResource(resourcePath);
}

bool IsResourceLoaded() {
	struct stat statbuf;
	mode_t mode = statbuf.st_mode;
	return stat(RESOURCE_PATH, &statbuf) == 0 /* File exists. */
		&& S_ISDIR(mode) /* File is a directory. */
		&& (mode & S_IRWXU) == S_IRWXU; /* File has permissions RWX. */
}

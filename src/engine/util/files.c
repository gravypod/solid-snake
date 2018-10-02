//
// Created by gravypod on 9/20/17.
//

#include "files.h"

#include <sys/stat.h>
#include <dirent.h>

long int fsize(const char *filename) {
	struct stat st;

	if (stat(filename, &st) == 0)
		return st.st_size;

	return -1;
}

char *read_file(const char *filename) {
	long int reported_size = fsize(filename);
	FILE *f;

	if (reported_size == -1 || !(f = fopen(filename, "rb"))) {
		return NULL;
	}

	const size_t file_size = (size_t) reported_size;
	size_t data_left = file_size;
	char *buffer = malloc(file_size + 1);
	char *tmp = buffer;

	while (data_left > 0) {
		const size_t len = fread((void *) tmp, sizeof(char), sizeof(buffer), f);
		data_left -= len;
		tmp += len;
	}
	buffer[file_size] = 0;

	fclose(f);

	return buffer;
}

llist *list_files(const char *folder_name, const char *ext) {
	static char buff[128];

	DIR *dir;
	struct dirent *ent;

	size_t ext_len = ext ? strlen(ext) : 0;
	llist *files = NULL;

	if ((dir = opendir(folder_name)) != NULL) {
		while ((ent = readdir(dir)) != NULL) {

			// Filter extensions
			// TODO: Make sure this condition will actually work.
			if (ext && strcmp(ext, ent->d_name + (strlen(ent->d_name) - ext_len)) != 0) {
				continue;
			}

			memset(buff, 0, sizeof(buff));
			strcat(buff, folder_name);
			strcat(buff, ent->d_name);

			llist_add(&files, buff, NULL, 0);
		}
		closedir(dir);
	}

	return files;
}

const char *file_extract_extension(const char *filename) {
	const char *dot = strrchr(filename, '.');

	if (!dot || dot == filename) {
		return "";
	}

	return dot + 1;
}

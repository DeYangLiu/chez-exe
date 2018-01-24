
#include <assert.h>
#include <fcntl.h>
#include <errno.h>	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <scheme.h>

extern const char chezschemebootfile_start;
extern const char chezschemebootfile_end;
extern const char scheme_program_start;
extern const char scheme_program_end;

int maketempfile_(char *template, const char *contents, size_t size) {
	int fd;
	fd = mkstemp(template);
	assert(fd >= 0);

	assert(write(fd, contents, size) == size);
	assert(lseek(fd, 0, SEEK_SET) == 0);
	return fd;
}

int maketempfile(char *namebuf, const char *contents, size_t size) {

	tmpnam(namebuf);
	//printf("tmp name='%s'\n", namebuf);
	
	FILE *fp = fopen(namebuf, "wb");
	if(fp) {
		fwrite(contents, 1, size, fp);
		fclose(fp);
	}
	return 0;
}

int main(int argc, const char **argv) {
	char bootfilename[L_tmpnam] = "";
	char schemefilename[L_tmpnam] = "";

	maketempfile(bootfilename, &chezschemebootfile_start, &chezschemebootfile_end - &chezschemebootfile_start);
	maketempfile(schemefilename, &scheme_program_start, &scheme_program_end - &scheme_program_start);

	
	/*close fd before register boot file: which will cant gzopen: Permission denied.
	char bootfilename[] = ".\\chezschemebootXXXXXX";
	int bootfd;
	char schemefilename[] = ".\\schemeprogramXXXXXX";
	int schemefd;
        
	bootfd = maketempfile_(bootfilename, &chezschemebootfile_start, &chezschemebootfile_end - &chezschemebootfile_start);
	schemefd = maketempfile_(schemefilename, &scheme_program_start, &scheme_program_end - &scheme_program_start);

	
	close(bootfd);
	close(schemefd);
	but the temporiry files are gone (delete by os).
	*/

	Sscheme_init(0);
	Sregister_boot_file(bootfilename);
	Sbuild_heap(0, 0);
	int ret = Sscheme_program(schemefilename, argc, argv);

	remove(bootfilename);
	remove(schemefilename);
	
	return ret;
}

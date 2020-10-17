/*-
 * Copyright (c) 2016 Sebastian Wiedenroth <wiedi@frubar.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define _GNU_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int cp(int out_fd, int in_fd) {
	char b[8192];
	ssize_t n;

	while(1) {
		n = read(in_fd, b, sizeof(b));
		if(!n)
			return 0;
		if(n < 0)
			return -1;
		if(write(out_fd, b, n) != n)
			return -1;
	}
}


int unhardlink(char *fn) {
	int ret = -1;
	struct stat s;
	char *tmp_fn;
	int orig_fd = open(fn, O_RDONLY, 0);
	if(orig_fd < 0) {
		fprintf(stderr, "%s ", fn);
		perror("open");
		return -1;
	}

	if(fstat(orig_fd, &s)) {
		fprintf(stderr, "%s ", fn);
		perror("fstat");
		return -1;
	}

	if(s.st_nlink < 2)
		return 0;

	if(asprintf(&tmp_fn, "%sXXXXXX", fn) < 0) {
		fprintf(stderr, "%s ", fn);
		perror("asprintf");
		return -1;
	}

	int tmp_fd = mkstemp(tmp_fn);
	if(!tmp_fd) {
		fprintf(stderr, "%s ", fn);
		perror("mkstemp");
		return -1;
	}

	if(cp(tmp_fd, orig_fd)) {
		fprintf(stderr, "%s ", fn);
		perror("copy");
		goto cleanup;
	}

	if(fchown(tmp_fd, s.st_uid, s.st_gid)) {
		fprintf(stderr, "%s ", fn);
		perror("fchown");
		goto cleanup;
	}

	if(fchmod(tmp_fd, s.st_mode)) {
		fprintf(stderr, "%s ", fn);
		perror("fchmod");
		goto cleanup;
	}

	if(rename(tmp_fn, fn)) {
		fprintf(stderr, "%s ", fn);
		perror("rename");
		goto cleanup;
	}

	ret = 0;

cleanup:
	close(tmp_fd);
	close(orig_fd);
	if(!ret)
		unlink(tmp_fn);
	return ret;
}

int main(int argc, char **argv) {
	if(argc < 2) {
		printf("Usage\n\t%s <files>\n\n", argv[0]);
		return 0;
	}

	int ret = 0;
	for(int i = 1; i < argc; i++) {
		if(unhardlink(argv[i])) {
			ret = 1;
		}
	}
	return ret;
}

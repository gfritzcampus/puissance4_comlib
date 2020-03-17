#ifndef _STUBS
#define _STUBS

int stub_open(const char *pathname, int flags);
ssize_t stub_write(int fd, const char * const buffer, size_t size);
int stub_close(int fd);

#endif

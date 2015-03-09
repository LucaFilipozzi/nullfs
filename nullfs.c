// nullfs - a FUSE handler for a null flat filesystem
//
// Copyright (C) 2001-2005  Miklos Szeredi <miklos@szeredi.hu>
// Copyright (C) 2010 Alexander Gattin <xrgtn@yandex.ru>
// Copyright (C) 2015 Luca Filipozzi <luca.filipozzi@gmail.com>
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.

#define FUSE_USE_VERSION 26

#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#ifdef __GNUC__
#define NOT_USED(x) /*@unused@*/ __attribute__((unused)) x
#else
#define NOT_USED(x) /*@unused@*/ x
#endif

static time_t start_t;

static bool strendswith(const char* str, const char* sfx)
{
    size_t sfx_len = strlen(sfx);
    size_t str_len = strlen(str);
    if (str_len < sfx_len) {
        return false;
    }
    return (strncmp(str + (str_len - sfx_len), sfx, sfx_len) == 0);
}

static bool isdir(const char* path)
{
    if (!path) {
        return 0;
    }
    return (strendswith(path, "/") || strendswith(path, "/..") || strendswith(path, "/.") || (strcmp(path, "..") == 0) || (strcmp(path, ".") == 0));
}

static int nullfs_getattr(const char* path, struct stat* stbuf)
{
    if (!path) {
        return -ENOENT;
    }
    memset(stbuf, 0, sizeof(struct stat));
    if (isdir(path)) {
        stbuf->st_mode = S_IFDIR | ACCESSPERMS;
        stbuf->st_nlink = 2;
        stbuf->st_atime = time(NULL);
        stbuf->st_mtime = start_t;
        stbuf->st_ctime = start_t;
    } else {
        stbuf->st_mode = S_IFREG | DEFFILEMODE;
        stbuf->st_nlink = 1;
        stbuf->st_size = 0;
        stbuf->st_atime = time(NULL);
        stbuf->st_mtime = time(NULL);
        stbuf->st_ctime = time(NULL);
    };
    return 0;
}

static int nullfs_readdir(const char* path, void* buf, fuse_fill_dir_t filler, NOT_USED(off_t offset), NOT_USED(struct fuse_file_info* fi))
{
    if (!isdir(path)) {
        return -ENOENT;
    }
    (void) filler(buf, ".", NULL, 0);
    (void) filler(buf, "..", NULL, 0);
    return 0;
}

static int nullfs_open(const char* path, NOT_USED(struct fuse_file_info* fi))
{
    if (isdir(path)) {
        return -ENOENT;
    }
    return 0;
}

static int nullfs_read(const char* path, NOT_USED(char* buf), NOT_USED(size_t size), NOT_USED(off_t offset), NOT_USED(struct fuse_file_info* fi))
{
    if (isdir(path)) {
        return -ENOENT;
    }
    return 0;
}

static int nullfs_write(const char* path, NOT_USED(const char* buf), NOT_USED(size_t size), NOT_USED(off_t offset), NOT_USED(struct fuse_file_info* fi))
{
    if (isdir(path)) {
        return -ENOENT;
    }
    return ((int)size);
}

static int nullfs_create(NOT_USED(const char* path), NOT_USED(mode_t mode), NOT_USED(struct fuse_file_info* fi))
{
    return 0;
}

static int nullfs_unlink(NOT_USED(const char* path))
{
    return 0;
}

static int nullfs_rename(NOT_USED(const char* src), NOT_USED(const char* dst))
{
    return 0;
}

static int nullfs_truncate(NOT_USED(const char* path), NOT_USED(off_t offset))
{
    return 0;
}

static int nullfs_chmod(NOT_USED(const char* path), NOT_USED(mode_t mode))
{
    return 0;
}

static int nullfs_chown(NOT_USED(const char* path), NOT_USED(uid_t uid), NOT_USED(gid_t gid))
{
    return 0;
}

static int nullfs_utimens(NOT_USED(const char* path), NOT_USED(const struct timespec ts[]))
{
    return 0;
}

static struct fuse_operations nullfs_fuse_operations = {
    .getattr    = nullfs_getattr,
    .readdir    = nullfs_readdir,
    .open       = nullfs_open,
    .read       = nullfs_read,
    .write      = nullfs_write,
    .create     = nullfs_create,
    .unlink     = nullfs_unlink,
    .rmdir      = nullfs_unlink,
    .truncate   = nullfs_truncate,
    .rename     = nullfs_rename,
    .chmod      = nullfs_chmod,
    .chown      = nullfs_chown,
    .utimens    = nullfs_utimens,
};

int main(int argc, char* argv[])
{
    start_t = time(NULL);
    return fuse_main(argc, argv, &nullfs_fuse_operations, NULL);
}

// vim: set ts=4 sw=4 et ai si:

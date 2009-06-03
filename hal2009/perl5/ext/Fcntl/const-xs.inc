BOOT:
  {
#ifdef dTHX
    dTHX;
#endif
    HV *symbol_table = get_hv("Fcntl::", TRUE);
#ifndef SYMBIAN
    HV *constant_missing;
#endif

    static const struct notfound_s values_for_notfound[] =
      {
#ifdef DN_ACCESS
#else
        { "DN_ACCESS", 9 },
#endif
#ifdef DN_MODIFY
#else
        { "DN_MODIFY", 9 },
#endif
#ifdef DN_CREATE
#else
        { "DN_CREATE", 9 },
#endif
#ifdef DN_DELETE
#else
        { "DN_DELETE", 9 },
#endif
#ifdef DN_RENAME
#else
        { "DN_RENAME", 9 },
#endif
#ifdef DN_ATTRIB
#else
        { "DN_ATTRIB", 9 },
#endif
#ifdef DN_MULTISHOT
#else
        { "DN_MULTISHOT", 12 },
#endif
#ifdef FAPPEND
#else
        { "FAPPEND", 7 },
#endif
#ifdef FASYNC
#else
        { "FASYNC", 6 },
#endif
#ifdef FCREAT
#else
        { "FCREAT", 6 },
#endif
#ifdef FDEFER
#else
        { "FDEFER", 6 },
#endif
#ifdef FDSYNC
#else
        { "FDSYNC", 6 },
#endif
#ifdef FD_CLOEXEC
#else
        { "FD_CLOEXEC", 10 },
#endif
#ifdef FEXCL
#else
        { "FEXCL", 5 },
#endif
#ifdef FLARGEFILE
#else
        { "FLARGEFILE", 10 },
#endif
#ifdef FNDELAY
#else
        { "FNDELAY", 7 },
#endif
#ifdef FNONBLOCK
#else
        { "FNONBLOCK", 9 },
#endif
#ifdef FRSYNC
#else
        { "FRSYNC", 6 },
#endif
#ifdef FSYNC
#else
        { "FSYNC", 5 },
#endif
#ifdef FTRUNC
#else
        { "FTRUNC", 6 },
#endif
#ifdef F_ALLOCSP
#else
        { "F_ALLOCSP", 9 },
#endif
#ifdef F_ALLOCSP64
#else
        { "F_ALLOCSP64", 11 },
#endif
#ifdef F_COMPAT
#else
        { "F_COMPAT", 8 },
#endif
#ifdef F_DUP2FD
#else
        { "F_DUP2FD", 8 },
#endif
#ifdef F_DUPFD
#else
        { "F_DUPFD", 7 },
#endif
#ifdef F_EXLCK
#else
        { "F_EXLCK", 7 },
#endif
#ifdef F_FREESP
#else
        { "F_FREESP", 8 },
#endif
#ifdef F_FREESP64
#else
        { "F_FREESP64", 10 },
#endif
#ifdef F_FSYNC
#else
        { "F_FSYNC", 7 },
#endif
#ifdef F_FSYNC64
#else
        { "F_FSYNC64", 9 },
#endif
#ifdef F_GETFD
#else
        { "F_GETFD", 7 },
#endif
#ifdef F_GETFL
#else
        { "F_GETFL", 7 },
#endif
#ifdef F_GETLEASE
#else
        { "F_GETLEASE", 10 },
#endif
#ifdef F_GETLK
#else
        { "F_GETLK", 7 },
#endif
#ifdef F_GETLK64
#else
        { "F_GETLK64", 9 },
#endif
#ifdef F_GETOWN
#else
        { "F_GETOWN", 8 },
#endif
#ifdef F_GETSIG
#else
        { "F_GETSIG", 8 },
#endif
#ifdef F_NODNY
#else
        { "F_NODNY", 7 },
#endif
#ifdef F_NOTIFY
#else
        { "F_NOTIFY", 8 },
#endif
#ifdef F_POSIX
#else
        { "F_POSIX", 7 },
#endif
#ifdef F_RDACC
#else
        { "F_RDACC", 7 },
#endif
#ifdef F_RDDNY
#else
        { "F_RDDNY", 7 },
#endif
#ifdef F_RDLCK
#else
        { "F_RDLCK", 7 },
#endif
#ifdef F_RWACC
#else
        { "F_RWACC", 7 },
#endif
#ifdef F_RWDNY
#else
        { "F_RWDNY", 7 },
#endif
#ifdef F_SETFD
#else
        { "F_SETFD", 7 },
#endif
#ifdef F_SETFL
#else
        { "F_SETFL", 7 },
#endif
#ifdef F_SETLEASE
#else
        { "F_SETLEASE", 10 },
#endif
#ifdef F_SETLK
#else
        { "F_SETLK", 7 },
#endif
#ifdef F_SETLK64
#else
        { "F_SETLK64", 9 },
#endif
#ifdef F_SETLKW
#else
        { "F_SETLKW", 8 },
#endif
#ifdef F_SETLKW64
#else
        { "F_SETLKW64", 10 },
#endif
#ifdef F_SETOWN
#else
        { "F_SETOWN", 8 },
#endif
#ifdef F_SETSIG
#else
        { "F_SETSIG", 8 },
#endif
#ifdef F_SHARE
#else
        { "F_SHARE", 7 },
#endif
#ifdef F_SHLCK
#else
        { "F_SHLCK", 7 },
#endif
#ifdef F_UNLCK
#else
        { "F_UNLCK", 7 },
#endif
#ifdef F_UNSHARE
#else
        { "F_UNSHARE", 9 },
#endif
#ifdef F_WRACC
#else
        { "F_WRACC", 7 },
#endif
#ifdef F_WRDNY
#else
        { "F_WRDNY", 7 },
#endif
#ifdef F_WRLCK
#else
        { "F_WRLCK", 7 },
#endif
#ifdef LOCK_MAND
#else
        { "LOCK_MAND", 9 },
#endif
#ifdef LOCK_READ
#else
        { "LOCK_READ", 9 },
#endif
#ifdef LOCK_WRITE
#else
        { "LOCK_WRITE", 10 },
#endif
#ifdef LOCK_RW
#else
        { "LOCK_RW", 7 },
#endif
#ifdef O_ACCMODE
#else
        { "O_ACCMODE", 9 },
#endif
#ifdef O_ALIAS
#else
        { "O_ALIAS", 7 },
#endif
#ifdef O_APPEND
#else
        { "O_APPEND", 8 },
#endif
#ifdef O_ASYNC
#else
        { "O_ASYNC", 7 },
#endif
#ifdef O_BINARY
#else
        { "O_BINARY", 8 },
#endif
#ifdef O_CREAT
#else
        { "O_CREAT", 7 },
#endif
#ifdef O_DEFER
#else
        { "O_DEFER", 7 },
#endif
#ifdef O_DIRECT
#else
        { "O_DIRECT", 8 },
#endif
#ifdef O_DIRECTORY
#else
        { "O_DIRECTORY", 11 },
#endif
#ifdef O_DSYNC
#else
        { "O_DSYNC", 7 },
#endif
#ifdef O_EXCL
#else
        { "O_EXCL", 6 },
#endif
#ifdef O_EXLOCK
#else
        { "O_EXLOCK", 8 },
#endif
#ifdef O_IGNORE_CTTY
#else
        { "O_IGNORE_CTTY", 13 },
#endif
#ifdef O_LARGEFILE
#else
        { "O_LARGEFILE", 11 },
#endif
#ifdef O_NDELAY
#else
        { "O_NDELAY", 8 },
#endif
#ifdef O_NOATIME
#else
        { "O_NOATIME", 9 },
#endif
#ifdef O_NOCTTY
#else
        { "O_NOCTTY", 8 },
#endif
#ifdef O_NOFOLLOW
#else
        { "O_NOFOLLOW", 10 },
#endif
#ifdef O_NOINHERIT
#else
        { "O_NOINHERIT", 11 },
#endif
#ifdef O_NOLINK
#else
        { "O_NOLINK", 8 },
#endif
#ifdef O_NONBLOCK
#else
        { "O_NONBLOCK", 10 },
#endif
#ifdef O_NOTRANS
#else
        { "O_NOTRANS", 9 },
#endif
#ifdef O_RANDOM
#else
        { "O_RANDOM", 8 },
#endif
#ifdef O_RAW
#else
        { "O_RAW", 5 },
#endif
#ifdef O_RDONLY
#else
        { "O_RDONLY", 8 },
#endif
#ifdef O_RDWR
#else
        { "O_RDWR", 6 },
#endif
#ifdef O_RSRC
#else
        { "O_RSRC", 6 },
#endif
#ifdef O_RSYNC
#else
        { "O_RSYNC", 7 },
#endif
#ifdef O_SEQUENTIAL
#else
        { "O_SEQUENTIAL", 12 },
#endif
#ifdef O_SHLOCK
#else
        { "O_SHLOCK", 8 },
#endif
#ifdef O_SYNC
#else
        { "O_SYNC", 6 },
#endif
#ifdef O_TEMPORARY
#else
        { "O_TEMPORARY", 11 },
#endif
#ifdef O_TEXT
#else
        { "O_TEXT", 6 },
#endif
#ifdef O_TRUNC
#else
        { "O_TRUNC", 7 },
#endif
#ifdef O_WRONLY
#else
        { "O_WRONLY", 8 },
#endif
#ifdef S_ENFMT
#else
        { "S_ENFMT", 7 },
#endif
#ifdef S_IEXEC
#else
        { "S_IEXEC", 7 },
#endif
#ifdef S_IFBLK
#else
        { "S_IFBLK", 7 },
#endif
#ifdef S_IFCHR
#else
        { "S_IFCHR", 7 },
#endif
#ifdef S_IFDIR
#else
        { "S_IFDIR", 7 },
#endif
#ifdef S_IFIFO
#else
        { "S_IFIFO", 7 },
#endif
#ifdef S_IFLNK
#else
        { "S_IFLNK", 7 },
#endif
#ifdef S_IFREG
#else
        { "S_IFREG", 7 },
#endif
#ifdef S_IFSOCK
#else
        { "S_IFSOCK", 8 },
#endif
#ifdef S_IFWHT
#else
        { "S_IFWHT", 7 },
#endif
#ifdef S_IREAD
#else
        { "S_IREAD", 7 },
#endif
#ifdef S_IRGRP
#else
        { "S_IRGRP", 7 },
#endif
#ifdef S_IROTH
#else
        { "S_IROTH", 7 },
#endif
#ifdef S_IRUSR
#else
        { "S_IRUSR", 7 },
#endif
#ifdef S_IRWXG
#else
        { "S_IRWXG", 7 },
#endif
#ifdef S_IRWXO
#else
        { "S_IRWXO", 7 },
#endif
#ifdef S_IRWXU
#else
        { "S_IRWXU", 7 },
#endif
#ifdef S_ISGID
#else
        { "S_ISGID", 7 },
#endif
#ifdef S_ISTXT
#else
        { "S_ISTXT", 7 },
#endif
#ifdef S_ISUID
#else
        { "S_ISUID", 7 },
#endif
#ifdef S_ISVTX
#else
        { "S_ISVTX", 7 },
#endif
#ifdef S_IWGRP
#else
        { "S_IWGRP", 7 },
#endif
#ifdef S_IWOTH
#else
        { "S_IWOTH", 7 },
#endif
#ifdef S_IWRITE
#else
        { "S_IWRITE", 8 },
#endif
#ifdef S_IWUSR
#else
        { "S_IWUSR", 7 },
#endif
#ifdef S_IXGRP
#else
        { "S_IXGRP", 7 },
#endif
#ifdef S_IXOTH
#else
        { "S_IXOTH", 7 },
#endif
#ifdef S_IXUSR
#else
        { "S_IXUSR", 7 },
#endif
#ifdef S_IFMT
#else
        { "_S_IFMT", 7 },
#endif
        { NULL, 0 } };
	const struct notfound_s *value_for_notfound = values_for_notfound;

    static const struct iv_s values_for_iv[] =
      {
#ifdef DN_ACCESS
        { "DN_ACCESS", 9, DN_ACCESS },
#endif
#ifdef DN_MODIFY
        { "DN_MODIFY", 9, DN_MODIFY },
#endif
#ifdef DN_CREATE
        { "DN_CREATE", 9, DN_CREATE },
#endif
#ifdef DN_DELETE
        { "DN_DELETE", 9, DN_DELETE },
#endif
#ifdef DN_RENAME
        { "DN_RENAME", 9, DN_RENAME },
#endif
#ifdef DN_ATTRIB
        { "DN_ATTRIB", 9, DN_ATTRIB },
#endif
#ifdef DN_MULTISHOT
        { "DN_MULTISHOT", 12, DN_MULTISHOT },
#endif
#ifdef FAPPEND
        { "FAPPEND", 7, FAPPEND },
#endif
#ifdef FASYNC
        { "FASYNC", 6, FASYNC },
#endif
#ifdef FCREAT
        { "FCREAT", 6, FCREAT },
#endif
#ifdef FDEFER
        { "FDEFER", 6, FDEFER },
#endif
#ifdef FDSYNC
        { "FDSYNC", 6, FDSYNC },
#endif
#ifdef FD_CLOEXEC
        { "FD_CLOEXEC", 10, FD_CLOEXEC },
#endif
#ifdef FEXCL
        { "FEXCL", 5, FEXCL },
#endif
#ifdef FLARGEFILE
        { "FLARGEFILE", 10, FLARGEFILE },
#endif
#ifdef FNDELAY
        { "FNDELAY", 7, FNDELAY },
#endif
#ifdef FNONBLOCK
        { "FNONBLOCK", 9, FNONBLOCK },
#endif
#ifdef FRSYNC
        { "FRSYNC", 6, FRSYNC },
#endif
#ifdef FSYNC
        { "FSYNC", 5, FSYNC },
#endif
#ifdef FTRUNC
        { "FTRUNC", 6, FTRUNC },
#endif
#ifdef F_ALLOCSP
        { "F_ALLOCSP", 9, F_ALLOCSP },
#endif
#ifdef F_ALLOCSP64
        { "F_ALLOCSP64", 11, F_ALLOCSP64 },
#endif
#ifdef F_COMPAT
        { "F_COMPAT", 8, F_COMPAT },
#endif
#ifdef F_DUP2FD
        { "F_DUP2FD", 8, F_DUP2FD },
#endif
#ifdef F_DUPFD
        { "F_DUPFD", 7, F_DUPFD },
#endif
#ifdef F_EXLCK
        { "F_EXLCK", 7, F_EXLCK },
#endif
#ifdef F_FREESP
        { "F_FREESP", 8, F_FREESP },
#endif
#ifdef F_FREESP64
        { "F_FREESP64", 10, F_FREESP64 },
#endif
#ifdef F_FSYNC
        { "F_FSYNC", 7, F_FSYNC },
#endif
#ifdef F_FSYNC64
        { "F_FSYNC64", 9, F_FSYNC64 },
#endif
#ifdef F_GETFD
        { "F_GETFD", 7, F_GETFD },
#endif
#ifdef F_GETFL
        { "F_GETFL", 7, F_GETFL },
#endif
#ifdef F_GETLEASE
        { "F_GETLEASE", 10, F_GETLEASE },
#endif
#ifdef F_GETLK
        { "F_GETLK", 7, F_GETLK },
#endif
#ifdef F_GETLK64
        { "F_GETLK64", 9, F_GETLK64 },
#endif
#ifdef F_GETOWN
        { "F_GETOWN", 8, F_GETOWN },
#endif
#ifdef F_GETSIG
        { "F_GETSIG", 8, F_GETSIG },
#endif
#ifdef F_NODNY
        { "F_NODNY", 7, F_NODNY },
#endif
#ifdef F_NOTIFY
        { "F_NOTIFY", 8, F_NOTIFY },
#endif
#ifdef F_POSIX
        { "F_POSIX", 7, F_POSIX },
#endif
#ifdef F_RDACC
        { "F_RDACC", 7, F_RDACC },
#endif
#ifdef F_RDDNY
        { "F_RDDNY", 7, F_RDDNY },
#endif
#ifdef F_RDLCK
        { "F_RDLCK", 7, F_RDLCK },
#endif
#ifdef F_RWACC
        { "F_RWACC", 7, F_RWACC },
#endif
#ifdef F_RWDNY
        { "F_RWDNY", 7, F_RWDNY },
#endif
#ifdef F_SETFD
        { "F_SETFD", 7, F_SETFD },
#endif
#ifdef F_SETFL
        { "F_SETFL", 7, F_SETFL },
#endif
#ifdef F_SETLEASE
        { "F_SETLEASE", 10, F_SETLEASE },
#endif
#ifdef F_SETLK
        { "F_SETLK", 7, F_SETLK },
#endif
#ifdef F_SETLK64
        { "F_SETLK64", 9, F_SETLK64 },
#endif
#ifdef F_SETLKW
        { "F_SETLKW", 8, F_SETLKW },
#endif
#ifdef F_SETLKW64
        { "F_SETLKW64", 10, F_SETLKW64 },
#endif
#ifdef F_SETOWN
        { "F_SETOWN", 8, F_SETOWN },
#endif
#ifdef F_SETSIG
        { "F_SETSIG", 8, F_SETSIG },
#endif
#ifdef F_SHARE
        { "F_SHARE", 7, F_SHARE },
#endif
#ifdef F_SHLCK
        { "F_SHLCK", 7, F_SHLCK },
#endif
#ifdef F_UNLCK
        { "F_UNLCK", 7, F_UNLCK },
#endif
#ifdef F_UNSHARE
        { "F_UNSHARE", 9, F_UNSHARE },
#endif
#ifdef F_WRACC
        { "F_WRACC", 7, F_WRACC },
#endif
#ifdef F_WRDNY
        { "F_WRDNY", 7, F_WRDNY },
#endif
#ifdef F_WRLCK
        { "F_WRLCK", 7, F_WRLCK },
#endif
#ifdef LOCK_MAND
        { "LOCK_MAND", 9, LOCK_MAND },
#endif
#ifdef LOCK_READ
        { "LOCK_READ", 9, LOCK_READ },
#endif
#ifdef LOCK_WRITE
        { "LOCK_WRITE", 10, LOCK_WRITE },
#endif
#ifdef LOCK_RW
        { "LOCK_RW", 7, LOCK_RW },
#endif
#ifdef O_ACCMODE
        { "O_ACCMODE", 9, O_ACCMODE },
#endif
#ifdef O_ALIAS
        { "O_ALIAS", 7, O_ALIAS },
#endif
#ifdef O_APPEND
        { "O_APPEND", 8, O_APPEND },
#endif
#ifdef O_ASYNC
        { "O_ASYNC", 7, O_ASYNC },
#endif
#ifdef O_BINARY
        { "O_BINARY", 8, O_BINARY },
#endif
#ifdef O_CREAT
        { "O_CREAT", 7, O_CREAT },
#endif
#ifdef O_DEFER
        { "O_DEFER", 7, O_DEFER },
#endif
#ifdef O_DIRECT
        { "O_DIRECT", 8, O_DIRECT },
#endif
#ifdef O_DIRECTORY
        { "O_DIRECTORY", 11, O_DIRECTORY },
#endif
#ifdef O_DSYNC
        { "O_DSYNC", 7, O_DSYNC },
#endif
#ifdef O_EXCL
        { "O_EXCL", 6, O_EXCL },
#endif
#ifdef O_EXLOCK
        { "O_EXLOCK", 8, O_EXLOCK },
#endif
#ifdef O_IGNORE_CTTY
        { "O_IGNORE_CTTY", 13, O_IGNORE_CTTY },
#endif
#ifdef O_LARGEFILE
        { "O_LARGEFILE", 11, O_LARGEFILE },
#endif
#ifdef O_NDELAY
        { "O_NDELAY", 8, O_NDELAY },
#endif
#ifdef O_NOATIME
        { "O_NOATIME", 9, O_NOATIME },
#endif
#ifdef O_NOCTTY
        { "O_NOCTTY", 8, O_NOCTTY },
#endif
#ifdef O_NOFOLLOW
        { "O_NOFOLLOW", 10, O_NOFOLLOW },
#endif
#ifdef O_NOINHERIT
        { "O_NOINHERIT", 11, O_NOINHERIT },
#endif
#ifdef O_NOLINK
        { "O_NOLINK", 8, O_NOLINK },
#endif
#ifdef O_NONBLOCK
        { "O_NONBLOCK", 10, O_NONBLOCK },
#endif
#ifdef O_NOTRANS
        { "O_NOTRANS", 9, O_NOTRANS },
#endif
#ifdef O_RANDOM
        { "O_RANDOM", 8, O_RANDOM },
#endif
#ifdef O_RAW
        { "O_RAW", 5, O_RAW },
#endif
#ifdef O_RDONLY
        { "O_RDONLY", 8, O_RDONLY },
#endif
#ifdef O_RDWR
        { "O_RDWR", 6, O_RDWR },
#endif
#ifdef O_RSRC
        { "O_RSRC", 6, O_RSRC },
#endif
#ifdef O_RSYNC
        { "O_RSYNC", 7, O_RSYNC },
#endif
#ifdef O_SEQUENTIAL
        { "O_SEQUENTIAL", 12, O_SEQUENTIAL },
#endif
#ifdef O_SHLOCK
        { "O_SHLOCK", 8, O_SHLOCK },
#endif
#ifdef O_SYNC
        { "O_SYNC", 6, O_SYNC },
#endif
#ifdef O_TEMPORARY
        { "O_TEMPORARY", 11, O_TEMPORARY },
#endif
#ifdef O_TEXT
        { "O_TEXT", 6, O_TEXT },
#endif
#ifdef O_TRUNC
        { "O_TRUNC", 7, O_TRUNC },
#endif
#ifdef O_WRONLY
        { "O_WRONLY", 8, O_WRONLY },
#endif
#ifdef S_ENFMT
        { "S_ENFMT", 7, S_ENFMT },
#endif
#ifdef S_IEXEC
        { "S_IEXEC", 7, S_IEXEC },
#endif
#ifdef S_IFBLK
        { "S_IFBLK", 7, S_IFBLK },
#endif
#ifdef S_IFCHR
        { "S_IFCHR", 7, S_IFCHR },
#endif
#ifdef S_IFDIR
        { "S_IFDIR", 7, S_IFDIR },
#endif
#ifdef S_IFIFO
        { "S_IFIFO", 7, S_IFIFO },
#endif
#ifdef S_IFLNK
        { "S_IFLNK", 7, S_IFLNK },
#endif
#ifdef S_IFREG
        { "S_IFREG", 7, S_IFREG },
#endif
#ifdef S_IFSOCK
        { "S_IFSOCK", 8, S_IFSOCK },
#endif
#ifdef S_IFWHT
        { "S_IFWHT", 7, S_IFWHT },
#endif
#ifdef S_IREAD
        { "S_IREAD", 7, S_IREAD },
#endif
#ifdef S_IRGRP
        { "S_IRGRP", 7, S_IRGRP },
#endif
#ifdef S_IROTH
        { "S_IROTH", 7, S_IROTH },
#endif
#ifdef S_IRUSR
        { "S_IRUSR", 7, S_IRUSR },
#endif
#ifdef S_IRWXG
        { "S_IRWXG", 7, S_IRWXG },
#endif
#ifdef S_IRWXO
        { "S_IRWXO", 7, S_IRWXO },
#endif
#ifdef S_IRWXU
        { "S_IRWXU", 7, S_IRWXU },
#endif
#ifdef S_ISGID
        { "S_ISGID", 7, S_ISGID },
#endif
#ifdef S_ISTXT
        { "S_ISTXT", 7, S_ISTXT },
#endif
#ifdef S_ISUID
        { "S_ISUID", 7, S_ISUID },
#endif
#ifdef S_ISVTX
        { "S_ISVTX", 7, S_ISVTX },
#endif
#ifdef S_IWGRP
        { "S_IWGRP", 7, S_IWGRP },
#endif
#ifdef S_IWOTH
        { "S_IWOTH", 7, S_IWOTH },
#endif
#ifdef S_IWRITE
        { "S_IWRITE", 8, S_IWRITE },
#endif
#ifdef S_IWUSR
        { "S_IWUSR", 7, S_IWUSR },
#endif
#ifdef S_IXGRP
        { "S_IXGRP", 7, S_IXGRP },
#endif
#ifdef S_IXOTH
        { "S_IXOTH", 7, S_IXOTH },
#endif
#ifdef S_IXUSR
        { "S_IXUSR", 7, S_IXUSR },
#endif
#ifdef LOCK_SH
        { "LOCK_SH", 7, LOCK_SH },
#endif
#ifdef LOCK_EX
        { "LOCK_EX", 7, LOCK_EX },
#endif
#ifdef LOCK_NB
        { "LOCK_NB", 7, LOCK_NB },
#endif
#ifdef LOCK_UN
        { "LOCK_UN", 7, LOCK_UN },
#endif
#ifdef SEEK_SET
        { "SEEK_SET", 8, SEEK_SET },
#endif
#ifdef SEEK_CUR
        { "SEEK_CUR", 8, SEEK_CUR },
#endif
#ifdef SEEK_END
        { "SEEK_END", 8, SEEK_END },
#endif
#ifdef S_IFMT
        { "_S_IFMT", 7, S_IFMT },
#endif
#ifdef LOCK_SH
        /* This is the default value: */
#else
        { "LOCK_SH", 7, 1 },
#endif
#ifdef LOCK_EX
        /* This is the default value: */
#else
        { "LOCK_EX", 7, 2 },
#endif
#ifdef LOCK_NB
        /* This is the default value: */
#else
        { "LOCK_NB", 7, 4 },
#endif
#ifdef LOCK_UN
        /* This is the default value: */
#else
        { "LOCK_UN", 7, 8 },
#endif
#ifdef SEEK_SET
        /* This is the default value: */
#else
        { "SEEK_SET", 8, 0 },
#endif
#ifdef SEEK_CUR
        /* This is the default value: */
#else
        { "SEEK_CUR", 8, 1 },
#endif
#ifdef SEEK_END
        /* This is the default value: */
#else
        { "SEEK_END", 8, 2 },
#endif
        { NULL, 0, 0 } };
	const struct iv_s *value_for_iv = values_for_iv;
#ifndef SYMBIAN
	constant_missing = get_missing_hash(aTHX);
#endif
        while (value_for_iv->name) {
	    constant_add_symbol(aTHX_  symbol_table, value_for_iv->name,
				value_for_iv->namelen, newSViv(value_for_iv->value));
	    ++value_for_iv;
	}
	while (value_for_notfound->name) {

	    /* Need to add prototypes, else parsing will vary by platform.  */
	    SV **sv = hv_fetch(symbol_table, value_for_notfound->name,
			       value_for_notfound->namelen, TRUE);
	    if (!sv) {
		Perl_croak(aTHX_ 
			   "Couldn't add key '%s' to %%Fcntl::",
			   value_for_notfound->name);
	    }
	    if (!SvOK(*sv) && SvTYPE(*sv) != SVt_PVGV) {
		/* Nothing was here before, so mark a prototype of ""  */
		sv_setpvn(*sv, "", 0);
	    } else if (SvPOK(*sv) && SvCUR(*sv) == 0) {
		/* There is already a prototype of "" - do nothing  */
	    } else {
		/* Someone has been here before us - have to make a real
		   typeglob.  */
		/* It turns out to be incredibly hard to deal with all the
		   corner cases of sub foo (); and reporting errors correctly,
		   so lets cheat a bit.  Start with a constant subroutine  */
		CV *cv = newCONSTSUB(symbol_table, value_for_notfound->name,
				     &PL_sv_yes);
		/* and then turn it into a non constant declaration only.  */
		SvREFCNT_dec(CvXSUBANY(cv).any_ptr);
		CvCONST_off(cv);
		CvXSUB(cv) = NULL;
		CvXSUBANY(cv).any_ptr = NULL;
	    }
#ifndef SYMBIAN
	    if (!hv_store(constant_missing, value_for_notfound->name,
			  value_for_notfound->namelen, &PL_sv_yes, 0))
		Perl_croak(aTHX_  "Couldn't add key '%s' to missing_hash",
			   value_for_notfound->name);
#endif

	    ++value_for_notfound;
	}
    /* As we've been creating subroutines, we better invalidate any cached
       methods  */
    ++PL_sub_generation;
  }

void
constant(sv)
    PREINIT:
	STRLEN		len;
    INPUT:
	SV *		sv;
        const char *	s = SvPV(sv, len);
    PPCODE:
#ifdef SYMBIAN
	sv = newSVpvf("%"SVf" is not a valid Fcntl macro", sv);
#else
	HV *constant_missing = get_missing_hash(aTHX);
	if (hv_exists(constant_missing, s, SvUTF8(sv) ? -(I32)len : (I32)len)) {
	    sv = newSVpvf("Your vendor has not defined Fcntl macro %" SVf
			  ", used", sv);
	} else {
	    sv = newSVpvf("%"SVf" is not a valid Fcntl macro",
			  sv);
	}
#endif
	PUSHs(sv_2mortal(sv));

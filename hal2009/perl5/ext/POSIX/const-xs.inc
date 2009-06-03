BOOT:
  {
#ifdef dTHX
    dTHX;
#endif
    HV *symbol_table = get_hv("POSIX::", TRUE);
#ifndef SYMBIAN
    HV *constant_missing;
#endif

    static const struct notfound_s values_for_notfound[] =
      {
#ifdef ARG_MAX
#else
        { "ARG_MAX", 7 },
#endif
#ifdef B0
#else
        { "B0", 2 },
#endif
#ifdef B110
#else
        { "B110", 4 },
#endif
#ifdef B1200
#else
        { "B1200", 5 },
#endif
#ifdef B134
#else
        { "B134", 4 },
#endif
#ifdef B150
#else
        { "B150", 4 },
#endif
#ifdef B1800
#else
        { "B1800", 5 },
#endif
#ifdef B19200
#else
        { "B19200", 6 },
#endif
#ifdef B200
#else
        { "B200", 4 },
#endif
#ifdef B2400
#else
        { "B2400", 5 },
#endif
#ifdef B300
#else
        { "B300", 4 },
#endif
#ifdef B38400
#else
        { "B38400", 6 },
#endif
#ifdef B4800
#else
        { "B4800", 5 },
#endif
#ifdef B50
#else
        { "B50", 3 },
#endif
#ifdef B600
#else
        { "B600", 4 },
#endif
#ifdef B75
#else
        { "B75", 3 },
#endif
#ifdef B9600
#else
        { "B9600", 5 },
#endif
#ifdef BRKINT
#else
        { "BRKINT", 6 },
#endif
#ifdef BUFSIZ
#else
        { "BUFSIZ", 6 },
#endif
#ifdef CHAR_BIT
#else
        { "CHAR_BIT", 8 },
#endif
#ifdef CHAR_MAX
#else
        { "CHAR_MAX", 8 },
#endif
#ifdef CHAR_MIN
#else
        { "CHAR_MIN", 8 },
#endif
#ifdef CHILD_MAX
#else
        { "CHILD_MAX", 9 },
#endif
#ifdef CLOCAL
#else
        { "CLOCAL", 6 },
#endif
#ifdef CLOCKS_PER_SEC
#else
        { "CLOCKS_PER_SEC", 14 },
#endif
#ifdef CREAD
#else
        { "CREAD", 5 },
#endif
#ifdef CS5
#else
        { "CS5", 3 },
#endif
#ifdef CS6
#else
        { "CS6", 3 },
#endif
#ifdef CS7
#else
        { "CS7", 3 },
#endif
#ifdef CS8
#else
        { "CS8", 3 },
#endif
#ifdef CSIZE
#else
        { "CSIZE", 5 },
#endif
#ifdef CSTOPB
#else
        { "CSTOPB", 6 },
#endif
#ifdef E2BIG
#else
        { "E2BIG", 5 },
#endif
#ifdef EACCES
#else
        { "EACCES", 6 },
#endif
#ifdef EADDRINUSE
#else
        { "EADDRINUSE", 10 },
#endif
#ifdef EADDRNOTAVAIL
#else
        { "EADDRNOTAVAIL", 13 },
#endif
#ifdef EAFNOSUPPORT
#else
        { "EAFNOSUPPORT", 12 },
#endif
#ifdef EAGAIN
#else
        { "EAGAIN", 6 },
#endif
#ifdef EALREADY
#else
        { "EALREADY", 8 },
#endif
#ifdef EBADF
#else
        { "EBADF", 5 },
#endif
#ifdef EBUSY
#else
        { "EBUSY", 5 },
#endif
#ifdef ECHILD
#else
        { "ECHILD", 6 },
#endif
#ifdef ECHO
#else
        { "ECHO", 4 },
#endif
#ifdef ECHOE
#else
        { "ECHOE", 5 },
#endif
#ifdef ECHOK
#else
        { "ECHOK", 5 },
#endif
#ifdef ECHONL
#else
        { "ECHONL", 6 },
#endif
#ifdef ECONNABORTED
#else
        { "ECONNABORTED", 12 },
#endif
#ifdef ECONNREFUSED
#else
        { "ECONNREFUSED", 12 },
#endif
#ifdef ECONNRESET
#else
        { "ECONNRESET", 10 },
#endif
#ifdef EDEADLK
#else
        { "EDEADLK", 7 },
#endif
#ifdef EDESTADDRREQ
#else
        { "EDESTADDRREQ", 12 },
#endif
#ifdef EDOM
#else
        { "EDOM", 4 },
#endif
#ifdef EDQUOT
#else
        { "EDQUOT", 6 },
#endif
#ifdef EEXIST
#else
        { "EEXIST", 6 },
#endif
#ifdef EFAULT
#else
        { "EFAULT", 6 },
#endif
#ifdef EFBIG
#else
        { "EFBIG", 5 },
#endif
#ifdef EHOSTDOWN
#else
        { "EHOSTDOWN", 9 },
#endif
#ifdef EHOSTUNREACH
#else
        { "EHOSTUNREACH", 12 },
#endif
#ifdef EINPROGRESS
#else
        { "EINPROGRESS", 11 },
#endif
#ifdef EINTR
#else
        { "EINTR", 5 },
#endif
#ifdef EINVAL
#else
        { "EINVAL", 6 },
#endif
#ifdef EIO
#else
        { "EIO", 3 },
#endif
#ifdef EISCONN
#else
        { "EISCONN", 7 },
#endif
#ifdef EISDIR
#else
        { "EISDIR", 6 },
#endif
#ifdef ELOOP
#else
        { "ELOOP", 5 },
#endif
#ifdef EMFILE
#else
        { "EMFILE", 6 },
#endif
#ifdef EMLINK
#else
        { "EMLINK", 6 },
#endif
#ifdef EMSGSIZE
#else
        { "EMSGSIZE", 8 },
#endif
#ifdef ENAMETOOLONG
#else
        { "ENAMETOOLONG", 12 },
#endif
#ifdef ENETDOWN
#else
        { "ENETDOWN", 8 },
#endif
#ifdef ENETRESET
#else
        { "ENETRESET", 9 },
#endif
#ifdef ENETUNREACH
#else
        { "ENETUNREACH", 11 },
#endif
#ifdef ENFILE
#else
        { "ENFILE", 6 },
#endif
#ifdef ENOBUFS
#else
        { "ENOBUFS", 7 },
#endif
#ifdef ENODEV
#else
        { "ENODEV", 6 },
#endif
#ifdef ENOENT
#else
        { "ENOENT", 6 },
#endif
#ifdef ENOEXEC
#else
        { "ENOEXEC", 7 },
#endif
#ifdef ENOLCK
#else
        { "ENOLCK", 6 },
#endif
#ifdef ENOMEM
#else
        { "ENOMEM", 6 },
#endif
#ifdef ENOPROTOOPT
#else
        { "ENOPROTOOPT", 11 },
#endif
#ifdef ENOSPC
#else
        { "ENOSPC", 6 },
#endif
#ifdef ENOSYS
#else
        { "ENOSYS", 6 },
#endif
#ifdef ENOTBLK
#else
        { "ENOTBLK", 7 },
#endif
#ifdef ENOTCONN
#else
        { "ENOTCONN", 8 },
#endif
#ifdef ENOTDIR
#else
        { "ENOTDIR", 7 },
#endif
#ifdef ENOTEMPTY
#else
        { "ENOTEMPTY", 9 },
#endif
#ifdef ENOTSOCK
#else
        { "ENOTSOCK", 8 },
#endif
#ifdef ENOTTY
#else
        { "ENOTTY", 6 },
#endif
#ifdef ENXIO
#else
        { "ENXIO", 5 },
#endif
#ifdef EOF
#else
        { "EOF", 3 },
#endif
#ifdef EOPNOTSUPP
#else
        { "EOPNOTSUPP", 10 },
#endif
#ifdef EPERM
#else
        { "EPERM", 5 },
#endif
#ifdef EPFNOSUPPORT
#else
        { "EPFNOSUPPORT", 12 },
#endif
#ifdef EPIPE
#else
        { "EPIPE", 5 },
#endif
#ifdef EPROCLIM
#else
        { "EPROCLIM", 8 },
#endif
#ifdef EPROTONOSUPPORT
#else
        { "EPROTONOSUPPORT", 15 },
#endif
#ifdef EPROTOTYPE
#else
        { "EPROTOTYPE", 10 },
#endif
#ifdef ERANGE
#else
        { "ERANGE", 6 },
#endif
#ifdef EREMOTE
#else
        { "EREMOTE", 7 },
#endif
#ifdef ERESTART
#else
        { "ERESTART", 8 },
#endif
#ifdef EROFS
#else
        { "EROFS", 5 },
#endif
#ifdef ESHUTDOWN
#else
        { "ESHUTDOWN", 9 },
#endif
#ifdef ESOCKTNOSUPPORT
#else
        { "ESOCKTNOSUPPORT", 15 },
#endif
#ifdef ESPIPE
#else
        { "ESPIPE", 6 },
#endif
#ifdef ESRCH
#else
        { "ESRCH", 5 },
#endif
#ifdef ESTALE
#else
        { "ESTALE", 6 },
#endif
#ifdef ETIMEDOUT
#else
        { "ETIMEDOUT", 9 },
#endif
#ifdef ETOOMANYREFS
#else
        { "ETOOMANYREFS", 12 },
#endif
#ifdef ETXTBSY
#else
        { "ETXTBSY", 7 },
#endif
#ifdef EUSERS
#else
        { "EUSERS", 6 },
#endif
#ifdef EWOULDBLOCK
#else
        { "EWOULDBLOCK", 11 },
#endif
#ifdef EXDEV
#else
        { "EXDEV", 5 },
#endif
#ifdef FILENAME_MAX
#else
        { "FILENAME_MAX", 12 },
#endif
#ifdef F_OK
#else
        { "F_OK", 4 },
#endif
#ifdef HUPCL
#else
        { "HUPCL", 5 },
#endif
#ifdef ICANON
#else
        { "ICANON", 6 },
#endif
#ifdef ICRNL
#else
        { "ICRNL", 5 },
#endif
#ifdef IEXTEN
#else
        { "IEXTEN", 6 },
#endif
#ifdef IGNBRK
#else
        { "IGNBRK", 6 },
#endif
#ifdef IGNCR
#else
        { "IGNCR", 5 },
#endif
#ifdef IGNPAR
#else
        { "IGNPAR", 6 },
#endif
#ifdef INLCR
#else
        { "INLCR", 5 },
#endif
#ifdef INPCK
#else
        { "INPCK", 5 },
#endif
#ifdef INT_MAX
#else
        { "INT_MAX", 7 },
#endif
#ifdef INT_MIN
#else
        { "INT_MIN", 7 },
#endif
#ifdef ISIG
#else
        { "ISIG", 4 },
#endif
#ifdef ISTRIP
#else
        { "ISTRIP", 6 },
#endif
#ifdef IXOFF
#else
        { "IXOFF", 5 },
#endif
#ifdef IXON
#else
        { "IXON", 4 },
#endif
#ifdef LC_ALL
#else
        { "LC_ALL", 6 },
#endif
#ifdef LC_COLLATE
#else
        { "LC_COLLATE", 10 },
#endif
#ifdef LC_CTYPE
#else
        { "LC_CTYPE", 8 },
#endif
#ifdef LC_MESSAGES
#else
        { "LC_MESSAGES", 11 },
#endif
#ifdef LC_MONETARY
#else
        { "LC_MONETARY", 11 },
#endif
#ifdef LC_NUMERIC
#else
        { "LC_NUMERIC", 10 },
#endif
#ifdef LC_TIME
#else
        { "LC_TIME", 7 },
#endif
#ifdef LINK_MAX
#else
        { "LINK_MAX", 8 },
#endif
#ifdef LONG_MAX
#else
        { "LONG_MAX", 8 },
#endif
#ifdef LONG_MIN
#else
        { "LONG_MIN", 8 },
#endif
#ifdef L_ctermid
#else
        { "L_ctermid", 9 },
#endif
#ifdef L_cuserid
#else
        { "L_cuserid", 9 },
#endif
#ifdef L_tmpnam
#else
        { "L_tmpnam", 8 },
#endif
#ifdef MAX_CANON
#else
        { "MAX_CANON", 9 },
#endif
#ifdef MAX_INPUT
#else
        { "MAX_INPUT", 9 },
#endif
#ifdef MB_LEN_MAX
#else
        { "MB_LEN_MAX", 10 },
#endif
#ifdef MSG_CTRUNC
#else
        { "MSG_CTRUNC", 10 },
#endif
#ifdef MSG_DONTROUTE
#else
        { "MSG_DONTROUTE", 13 },
#endif
#ifdef MSG_EOR
#else
        { "MSG_EOR", 7 },
#endif
#ifdef MSG_OOB
#else
        { "MSG_OOB", 7 },
#endif
#ifdef MSG_PEEK
#else
        { "MSG_PEEK", 8 },
#endif
#ifdef MSG_TRUNC
#else
        { "MSG_TRUNC", 9 },
#endif
#ifdef MSG_WAITALL
#else
        { "MSG_WAITALL", 11 },
#endif
#ifdef NAME_MAX
#else
        { "NAME_MAX", 8 },
#endif
#ifdef NCCS
#else
        { "NCCS", 4 },
#endif
#ifdef NGROUPS_MAX
#else
        { "NGROUPS_MAX", 11 },
#endif
#ifdef NOFLSH
#else
        { "NOFLSH", 6 },
#endif
#ifdef OPEN_MAX
#else
        { "OPEN_MAX", 8 },
#endif
#ifdef OPOST
#else
        { "OPOST", 5 },
#endif
#ifdef PARENB
#else
        { "PARENB", 6 },
#endif
#ifdef PARMRK
#else
        { "PARMRK", 6 },
#endif
#ifdef PARODD
#else
        { "PARODD", 6 },
#endif
#ifdef PATH_MAX
#else
        { "PATH_MAX", 8 },
#endif
#ifdef PIPE_BUF
#else
        { "PIPE_BUF", 8 },
#endif
#ifdef RAND_MAX
#else
        { "RAND_MAX", 8 },
#endif
#ifdef R_OK
#else
        { "R_OK", 4 },
#endif
#ifdef SCHAR_MAX
#else
        { "SCHAR_MAX", 9 },
#endif
#ifdef SCHAR_MIN
#else
        { "SCHAR_MIN", 9 },
#endif
#ifdef SEEK_CUR
#else
        { "SEEK_CUR", 8 },
#endif
#ifdef SEEK_END
#else
        { "SEEK_END", 8 },
#endif
#ifdef SEEK_SET
#else
        { "SEEK_SET", 8 },
#endif
#ifdef SHRT_MAX
#else
        { "SHRT_MAX", 8 },
#endif
#ifdef SHRT_MIN
#else
        { "SHRT_MIN", 8 },
#endif
#ifdef SIGABRT
#else
        { "SIGABRT", 7 },
#endif
#ifdef SIGALRM
#else
        { "SIGALRM", 7 },
#endif
#ifdef SIGCHLD
#else
        { "SIGCHLD", 7 },
#endif
#ifdef SIGCONT
#else
        { "SIGCONT", 7 },
#endif
#ifdef SIGFPE
#else
        { "SIGFPE", 6 },
#endif
#ifdef SIGHUP
#else
        { "SIGHUP", 6 },
#endif
#ifdef SIGILL
#else
        { "SIGILL", 6 },
#endif
#ifdef SIGINT
#else
        { "SIGINT", 6 },
#endif
#ifdef SIGKILL
#else
        { "SIGKILL", 7 },
#endif
#ifdef SIGPIPE
#else
        { "SIGPIPE", 7 },
#endif
#ifdef SIGQUIT
#else
        { "SIGQUIT", 7 },
#endif
#ifdef SIGSEGV
#else
        { "SIGSEGV", 7 },
#endif
#ifdef SIGSTOP
#else
        { "SIGSTOP", 7 },
#endif
#ifdef SIGTERM
#else
        { "SIGTERM", 7 },
#endif
#ifdef SIGTSTP
#else
        { "SIGTSTP", 7 },
#endif
#ifdef SIGTTIN
#else
        { "SIGTTIN", 7 },
#endif
#ifdef SIGTTOU
#else
        { "SIGTTOU", 7 },
#endif
#ifdef SIGUSR1
#else
        { "SIGUSR1", 7 },
#endif
#ifdef SIGUSR2
#else
        { "SIGUSR2", 7 },
#endif
#ifdef SIG_BLOCK
#else
        { "SIG_BLOCK", 9 },
#endif
#ifdef SIG_SETMASK
#else
        { "SIG_SETMASK", 11 },
#endif
#ifdef SIG_UNBLOCK
#else
        { "SIG_UNBLOCK", 11 },
#endif
#ifdef SSIZE_MAX
#else
        { "SSIZE_MAX", 9 },
#endif
#ifdef STDERR_FILENO
#else
        { "STDERR_FILENO", 13 },
#endif
#ifdef STDIN_FILENO
#else
        { "STDIN_FILENO", 12 },
#endif
#ifdef STDOUT_FILENO
#else
        { "STDOUT_FILENO", 13 },
#endif
#ifdef STREAM_MAX
#else
        { "STREAM_MAX", 10 },
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
#ifdef S_ISUID
#else
        { "S_ISUID", 7 },
#endif
#ifdef S_IWGRP
#else
        { "S_IWGRP", 7 },
#endif
#ifdef S_IWOTH
#else
        { "S_IWOTH", 7 },
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
#ifdef TCIFLUSH
#else
        { "TCIFLUSH", 8 },
#endif
#ifdef TCIOFF
#else
        { "TCIOFF", 6 },
#endif
#ifdef TCIOFLUSH
#else
        { "TCIOFLUSH", 9 },
#endif
#ifdef TCION
#else
        { "TCION", 5 },
#endif
#ifdef TCOFLUSH
#else
        { "TCOFLUSH", 8 },
#endif
#ifdef TCOOFF
#else
        { "TCOOFF", 6 },
#endif
#ifdef TCOON
#else
        { "TCOON", 5 },
#endif
#ifdef TCSADRAIN
#else
        { "TCSADRAIN", 9 },
#endif
#ifdef TCSAFLUSH
#else
        { "TCSAFLUSH", 9 },
#endif
#ifdef TCSANOW
#else
        { "TCSANOW", 7 },
#endif
#ifdef TMP_MAX
#else
        { "TMP_MAX", 7 },
#endif
#ifdef TOSTOP
#else
        { "TOSTOP", 6 },
#endif
#ifdef TZNAME_MAX
#else
        { "TZNAME_MAX", 10 },
#endif
#ifdef VEOF
#else
        { "VEOF", 4 },
#endif
#ifdef VEOL
#else
        { "VEOL", 4 },
#endif
#ifdef VERASE
#else
        { "VERASE", 6 },
#endif
#ifdef VINTR
#else
        { "VINTR", 5 },
#endif
#ifdef VKILL
#else
        { "VKILL", 5 },
#endif
#ifdef VMIN
#else
        { "VMIN", 4 },
#endif
#ifdef VQUIT
#else
        { "VQUIT", 5 },
#endif
#ifdef VSTART
#else
        { "VSTART", 6 },
#endif
#ifdef VSTOP
#else
        { "VSTOP", 5 },
#endif
#ifdef VSUSP
#else
        { "VSUSP", 5 },
#endif
#ifdef VTIME
#else
        { "VTIME", 5 },
#endif
#ifdef WNOHANG
#else
        { "WNOHANG", 7 },
#endif
#ifdef WUNTRACED
#else
        { "WUNTRACED", 9 },
#endif
#ifdef W_OK
#else
        { "W_OK", 4 },
#endif
#ifdef X_OK
#else
        { "X_OK", 4 },
#endif
#ifdef _PC_CHOWN_RESTRICTED
#else
        { "_PC_CHOWN_RESTRICTED", 20 },
#endif
#ifdef _PC_LINK_MAX
#else
        { "_PC_LINK_MAX", 12 },
#endif
#ifdef _PC_MAX_CANON
#else
        { "_PC_MAX_CANON", 13 },
#endif
#ifdef _PC_MAX_INPUT
#else
        { "_PC_MAX_INPUT", 13 },
#endif
#ifdef _PC_NAME_MAX
#else
        { "_PC_NAME_MAX", 12 },
#endif
#ifdef _PC_NO_TRUNC
#else
        { "_PC_NO_TRUNC", 12 },
#endif
#ifdef _PC_PATH_MAX
#else
        { "_PC_PATH_MAX", 12 },
#endif
#ifdef _PC_PIPE_BUF
#else
        { "_PC_PIPE_BUF", 12 },
#endif
#ifdef _PC_VDISABLE
#else
        { "_PC_VDISABLE", 12 },
#endif
#ifdef _SC_ARG_MAX
#else
        { "_SC_ARG_MAX", 11 },
#endif
#ifdef _SC_CHILD_MAX
#else
        { "_SC_CHILD_MAX", 13 },
#endif
#ifdef _SC_CLK_TCK
#else
        { "_SC_CLK_TCK", 11 },
#endif
#ifdef _SC_JOB_CONTROL
#else
        { "_SC_JOB_CONTROL", 15 },
#endif
#ifdef _SC_NGROUPS_MAX
#else
        { "_SC_NGROUPS_MAX", 15 },
#endif
#ifdef _SC_OPEN_MAX
#else
        { "_SC_OPEN_MAX", 12 },
#endif
#ifdef _SC_PAGESIZE
#else
        { "_SC_PAGESIZE", 12 },
#endif
#ifdef _SC_SAVED_IDS
#else
        { "_SC_SAVED_IDS", 13 },
#endif
#ifdef _SC_STREAM_MAX
#else
        { "_SC_STREAM_MAX", 14 },
#endif
#ifdef _SC_TZNAME_MAX
#else
        { "_SC_TZNAME_MAX", 14 },
#endif
#ifdef _SC_VERSION
#else
        { "_SC_VERSION", 11 },
#endif
#ifdef CLK_TCK
#else
        { "CLK_TCK", 7 },
#endif
#ifdef MB_CUR_MAX
#else
        { "MB_CUR_MAX", 10 },
#endif
#ifdef SIG_DFL
#else
        { "SIG_DFL", 7 },
#endif
#ifdef SIG_ERR
#else
        { "SIG_ERR", 7 },
#endif
#ifdef SIG_IGN
#else
        { "SIG_IGN", 7 },
#endif
#ifdef L_tmpname
#else
        { "L_tmpname", 9 },
#endif
#ifdef NULL
#else
        { "NULL", 4 },
#endif
#ifdef FLT_ROUNDS
#else
        { "FLT_ROUNDS", 10 },
#endif
#if (defined(USE_LONG_DOUBLE) && defined(HUGE_VALL)) || defined(HUGE_VAL)
	/* HUGE_VALL is admittedly non-POSIX but if we are using long doubles
	 * we might as well use long doubles. --jhi */
#else
        { "HUGE_VAL", 8 },
#endif
#ifdef SA_NOCLDSTOP
#else
        { "SA_NOCLDSTOP", 12 },
#endif
#ifdef SA_NOCLDWAIT
#else
        { "SA_NOCLDWAIT", 12 },
#endif
#ifdef SA_NODEFER
#else
        { "SA_NODEFER", 10 },
#endif
#ifdef SA_ONSTACK
#else
        { "SA_ONSTACK", 10 },
#endif
#ifdef SA_RESETHAND
#else
        { "SA_RESETHAND", 12 },
#endif
#ifdef SA_RESTART
#else
        { "SA_RESTART", 10 },
#endif
#ifdef SA_SIGINFO
#else
        { "SA_SIGINFO", 10 },
#endif
#ifdef UCHAR_MAX
#else
        { "UCHAR_MAX", 9 },
#endif
#ifdef UINT_MAX
#else
        { "UINT_MAX", 8 },
#endif
#ifdef ULONG_MAX
#else
        { "ULONG_MAX", 9 },
#endif
#ifdef USHRT_MAX
#else
        { "USHRT_MAX", 9 },
#endif
#ifdef DBL_MAX
#else
        { "DBL_MAX", 7 },
#endif
#ifdef FLT_MAX
#else
        { "FLT_MAX", 7 },
#endif
#ifdef LDBL_MAX
#else
        { "LDBL_MAX", 8 },
#endif
#ifdef LDBL_MIN
#else
        { "LDBL_MIN", 8 },
#endif
#ifdef LDBL_EPSILON
#else
        { "LDBL_EPSILON", 12 },
#endif
#ifdef DBL_EPSILON
#else
        { "DBL_EPSILON", 11 },
#endif
#ifdef DBL_MIN
#else
        { "DBL_MIN", 7 },
#endif
#ifdef FLT_EPSILON
#else
        { "FLT_EPSILON", 11 },
#endif
#ifdef FLT_MIN
#else
        { "FLT_MIN", 7 },
#endif
#ifdef DBL_DIG
#else
        { "DBL_DIG", 7 },
#endif
#ifdef DBL_MANT_DIG
#else
        { "DBL_MANT_DIG", 12 },
#endif
#ifdef DBL_MAX_10_EXP
#else
        { "DBL_MAX_10_EXP", 14 },
#endif
#ifdef DBL_MAX_EXP
#else
        { "DBL_MAX_EXP", 11 },
#endif
#ifdef DBL_MIN_10_EXP
#else
        { "DBL_MIN_10_EXP", 14 },
#endif
#ifdef DBL_MIN_EXP
#else
        { "DBL_MIN_EXP", 11 },
#endif
#ifdef FLT_DIG
#else
        { "FLT_DIG", 7 },
#endif
#ifdef FLT_MANT_DIG
#else
        { "FLT_MANT_DIG", 12 },
#endif
#ifdef FLT_MAX_10_EXP
#else
        { "FLT_MAX_10_EXP", 14 },
#endif
#ifdef FLT_MAX_EXP
#else
        { "FLT_MAX_EXP", 11 },
#endif
#ifdef FLT_MIN_10_EXP
#else
        { "FLT_MIN_10_EXP", 14 },
#endif
#ifdef FLT_MIN_EXP
#else
        { "FLT_MIN_EXP", 11 },
#endif
#ifdef FLT_RADIX
#else
        { "FLT_RADIX", 9 },
#endif
#ifdef LDBL_DIG
#else
        { "LDBL_DIG", 8 },
#endif
#ifdef LDBL_MANT_DIG
#else
        { "LDBL_MANT_DIG", 13 },
#endif
#ifdef LDBL_MAX_10_EXP
#else
        { "LDBL_MAX_10_EXP", 15 },
#endif
#ifdef LDBL_MAX_EXP
#else
        { "LDBL_MAX_EXP", 12 },
#endif
#ifdef LDBL_MIN_10_EXP
#else
        { "LDBL_MIN_10_EXP", 15 },
#endif
#ifdef LDBL_MIN_EXP
#else
        { "LDBL_MIN_EXP", 12 },
#endif
#ifdef SIGRTMAX
#else
        { "SIGRTMAX", 8 },
#endif
#ifdef SIGRTMIN
#else
        { "SIGRTMIN", 8 },
#endif
        { NULL, 0 } };
	const struct notfound_s *value_for_notfound = values_for_notfound;

    static const struct iv_s values_for_iv[] =
      {
#ifdef ARG_MAX
        { "ARG_MAX", 7, ARG_MAX },
#endif
#ifdef B0
        { "B0", 2, B0 },
#endif
#ifdef B110
        { "B110", 4, B110 },
#endif
#ifdef B1200
        { "B1200", 5, B1200 },
#endif
#ifdef B134
        { "B134", 4, B134 },
#endif
#ifdef B150
        { "B150", 4, B150 },
#endif
#ifdef B1800
        { "B1800", 5, B1800 },
#endif
#ifdef B19200
        { "B19200", 6, B19200 },
#endif
#ifdef B200
        { "B200", 4, B200 },
#endif
#ifdef B2400
        { "B2400", 5, B2400 },
#endif
#ifdef B300
        { "B300", 4, B300 },
#endif
#ifdef B38400
        { "B38400", 6, B38400 },
#endif
#ifdef B4800
        { "B4800", 5, B4800 },
#endif
#ifdef B50
        { "B50", 3, B50 },
#endif
#ifdef B600
        { "B600", 4, B600 },
#endif
#ifdef B75
        { "B75", 3, B75 },
#endif
#ifdef B9600
        { "B9600", 5, B9600 },
#endif
#ifdef BRKINT
        { "BRKINT", 6, BRKINT },
#endif
#ifdef BUFSIZ
        { "BUFSIZ", 6, BUFSIZ },
#endif
#ifdef CHAR_BIT
        { "CHAR_BIT", 8, CHAR_BIT },
#endif
#ifdef CHAR_MAX
        { "CHAR_MAX", 8, CHAR_MAX },
#endif
#ifdef CHAR_MIN
        { "CHAR_MIN", 8, CHAR_MIN },
#endif
#ifdef CHILD_MAX
        { "CHILD_MAX", 9, CHILD_MAX },
#endif
#ifdef CLOCAL
        { "CLOCAL", 6, CLOCAL },
#endif
#ifdef CLOCKS_PER_SEC
        { "CLOCKS_PER_SEC", 14, CLOCKS_PER_SEC },
#endif
#ifdef CREAD
        { "CREAD", 5, CREAD },
#endif
#ifdef CS5
        { "CS5", 3, CS5 },
#endif
#ifdef CS6
        { "CS6", 3, CS6 },
#endif
#ifdef CS7
        { "CS7", 3, CS7 },
#endif
#ifdef CS8
        { "CS8", 3, CS8 },
#endif
#ifdef CSIZE
        { "CSIZE", 5, CSIZE },
#endif
#ifdef CSTOPB
        { "CSTOPB", 6, CSTOPB },
#endif
#ifdef E2BIG
        { "E2BIG", 5, E2BIG },
#endif
#ifdef EACCES
        { "EACCES", 6, EACCES },
#endif
#ifdef EADDRINUSE
        { "EADDRINUSE", 10, EADDRINUSE },
#endif
#ifdef EADDRNOTAVAIL
        { "EADDRNOTAVAIL", 13, EADDRNOTAVAIL },
#endif
#ifdef EAFNOSUPPORT
        { "EAFNOSUPPORT", 12, EAFNOSUPPORT },
#endif
#ifdef EAGAIN
        { "EAGAIN", 6, EAGAIN },
#endif
#ifdef EALREADY
        { "EALREADY", 8, EALREADY },
#endif
#ifdef EBADF
        { "EBADF", 5, EBADF },
#endif
#ifdef EBUSY
        { "EBUSY", 5, EBUSY },
#endif
#ifdef ECHILD
        { "ECHILD", 6, ECHILD },
#endif
#ifdef ECHO
        { "ECHO", 4, ECHO },
#endif
#ifdef ECHOE
        { "ECHOE", 5, ECHOE },
#endif
#ifdef ECHOK
        { "ECHOK", 5, ECHOK },
#endif
#ifdef ECHONL
        { "ECHONL", 6, ECHONL },
#endif
#ifdef ECONNABORTED
        { "ECONNABORTED", 12, ECONNABORTED },
#endif
#ifdef ECONNREFUSED
        { "ECONNREFUSED", 12, ECONNREFUSED },
#endif
#ifdef ECONNRESET
        { "ECONNRESET", 10, ECONNRESET },
#endif
#ifdef EDEADLK
        { "EDEADLK", 7, EDEADLK },
#endif
#ifdef EDESTADDRREQ
        { "EDESTADDRREQ", 12, EDESTADDRREQ },
#endif
#ifdef EDOM
        { "EDOM", 4, EDOM },
#endif
#ifdef EDQUOT
        { "EDQUOT", 6, EDQUOT },
#endif
#ifdef EEXIST
        { "EEXIST", 6, EEXIST },
#endif
#ifdef EFAULT
        { "EFAULT", 6, EFAULT },
#endif
#ifdef EFBIG
        { "EFBIG", 5, EFBIG },
#endif
#ifdef EHOSTDOWN
        { "EHOSTDOWN", 9, EHOSTDOWN },
#endif
#ifdef EHOSTUNREACH
        { "EHOSTUNREACH", 12, EHOSTUNREACH },
#endif
#ifdef EINPROGRESS
        { "EINPROGRESS", 11, EINPROGRESS },
#endif
#ifdef EINTR
        { "EINTR", 5, EINTR },
#endif
#ifdef EINVAL
        { "EINVAL", 6, EINVAL },
#endif
#ifdef EIO
        { "EIO", 3, EIO },
#endif
#ifdef EISCONN
        { "EISCONN", 7, EISCONN },
#endif
#ifdef EISDIR
        { "EISDIR", 6, EISDIR },
#endif
#ifdef ELOOP
        { "ELOOP", 5, ELOOP },
#endif
#ifdef EMFILE
        { "EMFILE", 6, EMFILE },
#endif
#ifdef EMLINK
        { "EMLINK", 6, EMLINK },
#endif
#ifdef EMSGSIZE
        { "EMSGSIZE", 8, EMSGSIZE },
#endif
#ifdef ENAMETOOLONG
        { "ENAMETOOLONG", 12, ENAMETOOLONG },
#endif
#ifdef ENETDOWN
        { "ENETDOWN", 8, ENETDOWN },
#endif
#ifdef ENETRESET
        { "ENETRESET", 9, ENETRESET },
#endif
#ifdef ENETUNREACH
        { "ENETUNREACH", 11, ENETUNREACH },
#endif
#ifdef ENFILE
        { "ENFILE", 6, ENFILE },
#endif
#ifdef ENOBUFS
        { "ENOBUFS", 7, ENOBUFS },
#endif
#ifdef ENODEV
        { "ENODEV", 6, ENODEV },
#endif
#ifdef ENOENT
        { "ENOENT", 6, ENOENT },
#endif
#ifdef ENOEXEC
        { "ENOEXEC", 7, ENOEXEC },
#endif
#ifdef ENOLCK
        { "ENOLCK", 6, ENOLCK },
#endif
#ifdef ENOMEM
        { "ENOMEM", 6, ENOMEM },
#endif
#ifdef ENOPROTOOPT
        { "ENOPROTOOPT", 11, ENOPROTOOPT },
#endif
#ifdef ENOSPC
        { "ENOSPC", 6, ENOSPC },
#endif
#ifdef ENOSYS
        { "ENOSYS", 6, ENOSYS },
#endif
#ifdef ENOTBLK
        { "ENOTBLK", 7, ENOTBLK },
#endif
#ifdef ENOTCONN
        { "ENOTCONN", 8, ENOTCONN },
#endif
#ifdef ENOTDIR
        { "ENOTDIR", 7, ENOTDIR },
#endif
#ifdef ENOTEMPTY
        { "ENOTEMPTY", 9, ENOTEMPTY },
#endif
#ifdef ENOTSOCK
        { "ENOTSOCK", 8, ENOTSOCK },
#endif
#ifdef ENOTTY
        { "ENOTTY", 6, ENOTTY },
#endif
#ifdef ENXIO
        { "ENXIO", 5, ENXIO },
#endif
#ifdef EOF
        { "EOF", 3, EOF },
#endif
#ifdef EOPNOTSUPP
        { "EOPNOTSUPP", 10, EOPNOTSUPP },
#endif
#ifdef EPERM
        { "EPERM", 5, EPERM },
#endif
#ifdef EPFNOSUPPORT
        { "EPFNOSUPPORT", 12, EPFNOSUPPORT },
#endif
#ifdef EPIPE
        { "EPIPE", 5, EPIPE },
#endif
#ifdef EPROCLIM
        { "EPROCLIM", 8, EPROCLIM },
#endif
#ifdef EPROTONOSUPPORT
        { "EPROTONOSUPPORT", 15, EPROTONOSUPPORT },
#endif
#ifdef EPROTOTYPE
        { "EPROTOTYPE", 10, EPROTOTYPE },
#endif
#ifdef ERANGE
        { "ERANGE", 6, ERANGE },
#endif
#ifdef EREMOTE
        { "EREMOTE", 7, EREMOTE },
#endif
#ifdef ERESTART
        { "ERESTART", 8, ERESTART },
#endif
#ifdef EROFS
        { "EROFS", 5, EROFS },
#endif
#ifdef ESHUTDOWN
        { "ESHUTDOWN", 9, ESHUTDOWN },
#endif
#ifdef ESOCKTNOSUPPORT
        { "ESOCKTNOSUPPORT", 15, ESOCKTNOSUPPORT },
#endif
#ifdef ESPIPE
        { "ESPIPE", 6, ESPIPE },
#endif
#ifdef ESRCH
        { "ESRCH", 5, ESRCH },
#endif
#ifdef ESTALE
        { "ESTALE", 6, ESTALE },
#endif
#ifdef ETIMEDOUT
        { "ETIMEDOUT", 9, ETIMEDOUT },
#endif
#ifdef ETOOMANYREFS
        { "ETOOMANYREFS", 12, ETOOMANYREFS },
#endif
#ifdef ETXTBSY
        { "ETXTBSY", 7, ETXTBSY },
#endif
#ifdef EUSERS
        { "EUSERS", 6, EUSERS },
#endif
#ifdef EWOULDBLOCK
        { "EWOULDBLOCK", 11, EWOULDBLOCK },
#endif
#ifdef EXDEV
        { "EXDEV", 5, EXDEV },
#endif
#ifdef FILENAME_MAX
        { "FILENAME_MAX", 12, FILENAME_MAX },
#endif
#ifdef F_OK
        { "F_OK", 4, F_OK },
#endif
#ifdef HUPCL
        { "HUPCL", 5, HUPCL },
#endif
#ifdef ICANON
        { "ICANON", 6, ICANON },
#endif
#ifdef ICRNL
        { "ICRNL", 5, ICRNL },
#endif
#ifdef IEXTEN
        { "IEXTEN", 6, IEXTEN },
#endif
#ifdef IGNBRK
        { "IGNBRK", 6, IGNBRK },
#endif
#ifdef IGNCR
        { "IGNCR", 5, IGNCR },
#endif
#ifdef IGNPAR
        { "IGNPAR", 6, IGNPAR },
#endif
#ifdef INLCR
        { "INLCR", 5, INLCR },
#endif
#ifdef INPCK
        { "INPCK", 5, INPCK },
#endif
#ifdef INT_MAX
        { "INT_MAX", 7, INT_MAX },
#endif
#ifdef INT_MIN
        { "INT_MIN", 7, INT_MIN },
#endif
#ifdef ISIG
        { "ISIG", 4, ISIG },
#endif
#ifdef ISTRIP
        { "ISTRIP", 6, ISTRIP },
#endif
#ifdef IXOFF
        { "IXOFF", 5, IXOFF },
#endif
#ifdef IXON
        { "IXON", 4, IXON },
#endif
#ifdef LC_ALL
        { "LC_ALL", 6, LC_ALL },
#endif
#ifdef LC_COLLATE
        { "LC_COLLATE", 10, LC_COLLATE },
#endif
#ifdef LC_CTYPE
        { "LC_CTYPE", 8, LC_CTYPE },
#endif
#ifdef LC_MESSAGES
        { "LC_MESSAGES", 11, LC_MESSAGES },
#endif
#ifdef LC_MONETARY
        { "LC_MONETARY", 11, LC_MONETARY },
#endif
#ifdef LC_NUMERIC
        { "LC_NUMERIC", 10, LC_NUMERIC },
#endif
#ifdef LC_TIME
        { "LC_TIME", 7, LC_TIME },
#endif
#ifdef LINK_MAX
        { "LINK_MAX", 8, LINK_MAX },
#endif
#ifdef LONG_MAX
        { "LONG_MAX", 8, LONG_MAX },
#endif
#ifdef LONG_MIN
        { "LONG_MIN", 8, LONG_MIN },
#endif
#ifdef L_ctermid
        { "L_ctermid", 9, L_ctermid },
#endif
#ifdef L_cuserid
        { "L_cuserid", 9, L_cuserid },
#endif
#ifdef L_tmpnam
        { "L_tmpnam", 8, L_tmpnam },
#endif
#ifdef MAX_CANON
        { "MAX_CANON", 9, MAX_CANON },
#endif
#ifdef MAX_INPUT
        { "MAX_INPUT", 9, MAX_INPUT },
#endif
#ifdef MB_LEN_MAX
        { "MB_LEN_MAX", 10, MB_LEN_MAX },
#endif
#ifdef MSG_CTRUNC
        { "MSG_CTRUNC", 10, MSG_CTRUNC },
#endif
#ifdef MSG_DONTROUTE
        { "MSG_DONTROUTE", 13, MSG_DONTROUTE },
#endif
#ifdef MSG_EOR
        { "MSG_EOR", 7, MSG_EOR },
#endif
#ifdef MSG_OOB
        { "MSG_OOB", 7, MSG_OOB },
#endif
#ifdef MSG_PEEK
        { "MSG_PEEK", 8, MSG_PEEK },
#endif
#ifdef MSG_TRUNC
        { "MSG_TRUNC", 9, MSG_TRUNC },
#endif
#ifdef MSG_WAITALL
        { "MSG_WAITALL", 11, MSG_WAITALL },
#endif
#ifdef NAME_MAX
        { "NAME_MAX", 8, NAME_MAX },
#endif
#ifdef NCCS
        { "NCCS", 4, NCCS },
#endif
#ifdef NGROUPS_MAX
        { "NGROUPS_MAX", 11, NGROUPS_MAX },
#endif
#ifdef NOFLSH
        { "NOFLSH", 6, NOFLSH },
#endif
#ifdef OPEN_MAX
        { "OPEN_MAX", 8, OPEN_MAX },
#endif
#ifdef OPOST
        { "OPOST", 5, OPOST },
#endif
#ifdef PARENB
        { "PARENB", 6, PARENB },
#endif
#ifdef PARMRK
        { "PARMRK", 6, PARMRK },
#endif
#ifdef PARODD
        { "PARODD", 6, PARODD },
#endif
#ifdef PATH_MAX
        { "PATH_MAX", 8, PATH_MAX },
#endif
#ifdef PIPE_BUF
        { "PIPE_BUF", 8, PIPE_BUF },
#endif
#ifdef RAND_MAX
        { "RAND_MAX", 8, RAND_MAX },
#endif
#ifdef R_OK
        { "R_OK", 4, R_OK },
#endif
#ifdef SCHAR_MAX
        { "SCHAR_MAX", 9, SCHAR_MAX },
#endif
#ifdef SCHAR_MIN
        { "SCHAR_MIN", 9, SCHAR_MIN },
#endif
#ifdef SEEK_CUR
        { "SEEK_CUR", 8, SEEK_CUR },
#endif
#ifdef SEEK_END
        { "SEEK_END", 8, SEEK_END },
#endif
#ifdef SEEK_SET
        { "SEEK_SET", 8, SEEK_SET },
#endif
#ifdef SHRT_MAX
        { "SHRT_MAX", 8, SHRT_MAX },
#endif
#ifdef SHRT_MIN
        { "SHRT_MIN", 8, SHRT_MIN },
#endif
#ifdef SIGABRT
        { "SIGABRT", 7, SIGABRT },
#endif
#ifdef SIGALRM
        { "SIGALRM", 7, SIGALRM },
#endif
#ifdef SIGCHLD
        { "SIGCHLD", 7, SIGCHLD },
#endif
#ifdef SIGCONT
        { "SIGCONT", 7, SIGCONT },
#endif
#ifdef SIGFPE
        { "SIGFPE", 6, SIGFPE },
#endif
#ifdef SIGHUP
        { "SIGHUP", 6, SIGHUP },
#endif
#ifdef SIGILL
        { "SIGILL", 6, SIGILL },
#endif
#ifdef SIGINT
        { "SIGINT", 6, SIGINT },
#endif
#ifdef SIGKILL
        { "SIGKILL", 7, SIGKILL },
#endif
#ifdef SIGPIPE
        { "SIGPIPE", 7, SIGPIPE },
#endif
#ifdef SIGQUIT
        { "SIGQUIT", 7, SIGQUIT },
#endif
#ifdef SIGSEGV
        { "SIGSEGV", 7, SIGSEGV },
#endif
#ifdef SIGSTOP
        { "SIGSTOP", 7, SIGSTOP },
#endif
#ifdef SIGTERM
        { "SIGTERM", 7, SIGTERM },
#endif
#ifdef SIGTSTP
        { "SIGTSTP", 7, SIGTSTP },
#endif
#ifdef SIGTTIN
        { "SIGTTIN", 7, SIGTTIN },
#endif
#ifdef SIGTTOU
        { "SIGTTOU", 7, SIGTTOU },
#endif
#ifdef SIGUSR1
        { "SIGUSR1", 7, SIGUSR1 },
#endif
#ifdef SIGUSR2
        { "SIGUSR2", 7, SIGUSR2 },
#endif
#ifdef SIG_BLOCK
        { "SIG_BLOCK", 9, SIG_BLOCK },
#endif
#ifdef SIG_SETMASK
        { "SIG_SETMASK", 11, SIG_SETMASK },
#endif
#ifdef SIG_UNBLOCK
        { "SIG_UNBLOCK", 11, SIG_UNBLOCK },
#endif
#ifdef SSIZE_MAX
        { "SSIZE_MAX", 9, SSIZE_MAX },
#endif
#ifdef STDERR_FILENO
        { "STDERR_FILENO", 13, STDERR_FILENO },
#endif
#ifdef STDIN_FILENO
        { "STDIN_FILENO", 12, STDIN_FILENO },
#endif
#ifdef STDOUT_FILENO
        { "STDOUT_FILENO", 13, STDOUT_FILENO },
#endif
#ifdef STREAM_MAX
        { "STREAM_MAX", 10, STREAM_MAX },
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
#ifdef S_ISUID
        { "S_ISUID", 7, S_ISUID },
#endif
#ifdef S_IWGRP
        { "S_IWGRP", 7, S_IWGRP },
#endif
#ifdef S_IWOTH
        { "S_IWOTH", 7, S_IWOTH },
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
#ifdef TCIFLUSH
        { "TCIFLUSH", 8, TCIFLUSH },
#endif
#ifdef TCIOFF
        { "TCIOFF", 6, TCIOFF },
#endif
#ifdef TCIOFLUSH
        { "TCIOFLUSH", 9, TCIOFLUSH },
#endif
#ifdef TCION
        { "TCION", 5, TCION },
#endif
#ifdef TCOFLUSH
        { "TCOFLUSH", 8, TCOFLUSH },
#endif
#ifdef TCOOFF
        { "TCOOFF", 6, TCOOFF },
#endif
#ifdef TCOON
        { "TCOON", 5, TCOON },
#endif
#ifdef TCSADRAIN
        { "TCSADRAIN", 9, TCSADRAIN },
#endif
#ifdef TCSAFLUSH
        { "TCSAFLUSH", 9, TCSAFLUSH },
#endif
#ifdef TCSANOW
        { "TCSANOW", 7, TCSANOW },
#endif
#ifdef TMP_MAX
        { "TMP_MAX", 7, TMP_MAX },
#endif
#ifdef TOSTOP
        { "TOSTOP", 6, TOSTOP },
#endif
#ifdef TZNAME_MAX
        { "TZNAME_MAX", 10, TZNAME_MAX },
#endif
#ifdef VEOF
        { "VEOF", 4, VEOF },
#endif
#ifdef VEOL
        { "VEOL", 4, VEOL },
#endif
#ifdef VERASE
        { "VERASE", 6, VERASE },
#endif
#ifdef VINTR
        { "VINTR", 5, VINTR },
#endif
#ifdef VKILL
        { "VKILL", 5, VKILL },
#endif
#ifdef VMIN
        { "VMIN", 4, VMIN },
#endif
#ifdef VQUIT
        { "VQUIT", 5, VQUIT },
#endif
#ifdef VSTART
        { "VSTART", 6, VSTART },
#endif
#ifdef VSTOP
        { "VSTOP", 5, VSTOP },
#endif
#ifdef VSUSP
        { "VSUSP", 5, VSUSP },
#endif
#ifdef VTIME
        { "VTIME", 5, VTIME },
#endif
#ifdef WNOHANG
        { "WNOHANG", 7, WNOHANG },
#endif
#ifdef WUNTRACED
        { "WUNTRACED", 9, WUNTRACED },
#endif
#ifdef W_OK
        { "W_OK", 4, W_OK },
#endif
#ifdef X_OK
        { "X_OK", 4, X_OK },
#endif
#ifdef _PC_CHOWN_RESTRICTED
        { "_PC_CHOWN_RESTRICTED", 20, _PC_CHOWN_RESTRICTED },
#endif
#ifdef _PC_LINK_MAX
        { "_PC_LINK_MAX", 12, _PC_LINK_MAX },
#endif
#ifdef _PC_MAX_CANON
        { "_PC_MAX_CANON", 13, _PC_MAX_CANON },
#endif
#ifdef _PC_MAX_INPUT
        { "_PC_MAX_INPUT", 13, _PC_MAX_INPUT },
#endif
#ifdef _PC_NAME_MAX
        { "_PC_NAME_MAX", 12, _PC_NAME_MAX },
#endif
#ifdef _PC_NO_TRUNC
        { "_PC_NO_TRUNC", 12, _PC_NO_TRUNC },
#endif
#ifdef _PC_PATH_MAX
        { "_PC_PATH_MAX", 12, _PC_PATH_MAX },
#endif
#ifdef _PC_PIPE_BUF
        { "_PC_PIPE_BUF", 12, _PC_PIPE_BUF },
#endif
#ifdef _PC_VDISABLE
        { "_PC_VDISABLE", 12, _PC_VDISABLE },
#endif
#ifdef _SC_ARG_MAX
        { "_SC_ARG_MAX", 11, _SC_ARG_MAX },
#endif
#ifdef _SC_CHILD_MAX
        { "_SC_CHILD_MAX", 13, _SC_CHILD_MAX },
#endif
#ifdef _SC_CLK_TCK
        { "_SC_CLK_TCK", 11, _SC_CLK_TCK },
#endif
#ifdef _SC_JOB_CONTROL
        { "_SC_JOB_CONTROL", 15, _SC_JOB_CONTROL },
#endif
#ifdef _SC_NGROUPS_MAX
        { "_SC_NGROUPS_MAX", 15, _SC_NGROUPS_MAX },
#endif
#ifdef _SC_OPEN_MAX
        { "_SC_OPEN_MAX", 12, _SC_OPEN_MAX },
#endif
#ifdef _SC_PAGESIZE
        { "_SC_PAGESIZE", 12, _SC_PAGESIZE },
#endif
#ifdef _SC_SAVED_IDS
        { "_SC_SAVED_IDS", 13, _SC_SAVED_IDS },
#endif
#ifdef _SC_STREAM_MAX
        { "_SC_STREAM_MAX", 14, _SC_STREAM_MAX },
#endif
#ifdef _SC_TZNAME_MAX
        { "_SC_TZNAME_MAX", 14, _SC_TZNAME_MAX },
#endif
#ifdef _SC_VERSION
        { "_SC_VERSION", 11, _SC_VERSION },
#endif
#ifdef EXIT_FAILURE
        { "EXIT_FAILURE", 12, EXIT_FAILURE },
#endif
#ifdef EXIT_SUCCESS
        { "EXIT_SUCCESS", 12, EXIT_SUCCESS },
#endif
#ifdef L_tmpname
        { "L_tmpname", 9, L_tmpnam },
#endif
#ifdef NULL
        { "NULL", 4, 0 },
#endif
#ifdef _POSIX_ARG_MAX
        { "_POSIX_ARG_MAX", 14, _POSIX_ARG_MAX },
#endif
#ifdef _POSIX_CHILD_MAX
        { "_POSIX_CHILD_MAX", 16, _POSIX_CHILD_MAX },
#endif
#ifdef _POSIX_CHOWN_RESTRICTED
        { "_POSIX_CHOWN_RESTRICTED", 23, _POSIX_CHOWN_RESTRICTED },
#endif
#ifdef _POSIX_LINK_MAX
        { "_POSIX_LINK_MAX", 15, _POSIX_LINK_MAX },
#endif
#ifdef _POSIX_MAX_CANON
        { "_POSIX_MAX_CANON", 16, _POSIX_MAX_CANON },
#endif
#ifdef _POSIX_MAX_INPUT
        { "_POSIX_MAX_INPUT", 16, _POSIX_MAX_INPUT },
#endif
#ifdef _POSIX_NAME_MAX
        { "_POSIX_NAME_MAX", 15, _POSIX_NAME_MAX },
#endif
#ifdef _POSIX_NGROUPS_MAX
        { "_POSIX_NGROUPS_MAX", 18, _POSIX_NGROUPS_MAX },
#endif
#ifdef _POSIX_NO_TRUNC
        { "_POSIX_NO_TRUNC", 15, _POSIX_NO_TRUNC },
#endif
#ifdef _POSIX_OPEN_MAX
        { "_POSIX_OPEN_MAX", 15, _POSIX_OPEN_MAX },
#endif
#ifdef _POSIX_PATH_MAX
        { "_POSIX_PATH_MAX", 15, _POSIX_PATH_MAX },
#endif
#ifdef _POSIX_PIPE_BUF
        { "_POSIX_PIPE_BUF", 15, _POSIX_PIPE_BUF },
#endif
#ifdef _POSIX_SSIZE_MAX
        { "_POSIX_SSIZE_MAX", 16, _POSIX_SSIZE_MAX },
#endif
#ifdef _POSIX_STREAM_MAX
        { "_POSIX_STREAM_MAX", 17, _POSIX_STREAM_MAX },
#endif
#ifdef _POSIX_TZNAME_MAX
        { "_POSIX_TZNAME_MAX", 17, _POSIX_TZNAME_MAX },
#endif
#ifdef _POSIX_VDISABLE
        { "_POSIX_VDISABLE", 15, _POSIX_VDISABLE },
#endif
#ifdef _POSIX_VERSION
        { "_POSIX_VERSION", 14, _POSIX_VERSION },
#endif
#ifdef EXIT_FAILURE
        /* This is the default value: */
#else
        { "EXIT_FAILURE", 12, 1 },
#endif
#ifdef EXIT_SUCCESS
        /* This is the default value: */
#else
        { "EXIT_SUCCESS", 12, 0 },
#endif
#ifdef _POSIX_JOB_CONTROL
        /* This is the default value: */
#else
        { "_POSIX_JOB_CONTROL", 18, 0 },
#endif
#ifdef _POSIX_SAVED_IDS
        /* This is the default value: */
#else
        { "_POSIX_SAVED_IDS", 16, 0 },
#endif
#ifdef _POSIX_ARG_MAX
        /* This is the default value: */
#else
        { "_POSIX_ARG_MAX", 14, 0 },
#endif
#ifdef _POSIX_CHILD_MAX
        /* This is the default value: */
#else
        { "_POSIX_CHILD_MAX", 16, 0 },
#endif
#ifdef _POSIX_CHOWN_RESTRICTED
        /* This is the default value: */
#else
        { "_POSIX_CHOWN_RESTRICTED", 23, 0 },
#endif
#ifdef _POSIX_LINK_MAX
        /* This is the default value: */
#else
        { "_POSIX_LINK_MAX", 15, 0 },
#endif
#ifdef _POSIX_MAX_CANON
        /* This is the default value: */
#else
        { "_POSIX_MAX_CANON", 16, 0 },
#endif
#ifdef _POSIX_MAX_INPUT
        /* This is the default value: */
#else
        { "_POSIX_MAX_INPUT", 16, 0 },
#endif
#ifdef _POSIX_NAME_MAX
        /* This is the default value: */
#else
        { "_POSIX_NAME_MAX", 15, 0 },
#endif
#ifdef _POSIX_NGROUPS_MAX
        /* This is the default value: */
#else
        { "_POSIX_NGROUPS_MAX", 18, 0 },
#endif
#ifdef _POSIX_NO_TRUNC
        /* This is the default value: */
#else
        { "_POSIX_NO_TRUNC", 15, 0 },
#endif
#ifdef _POSIX_OPEN_MAX
        /* This is the default value: */
#else
        { "_POSIX_OPEN_MAX", 15, 0 },
#endif
#ifdef _POSIX_PATH_MAX
        /* This is the default value: */
#else
        { "_POSIX_PATH_MAX", 15, 0 },
#endif
#ifdef _POSIX_PIPE_BUF
        /* This is the default value: */
#else
        { "_POSIX_PIPE_BUF", 15, 0 },
#endif
#ifdef _POSIX_SSIZE_MAX
        /* This is the default value: */
#else
        { "_POSIX_SSIZE_MAX", 16, 0 },
#endif
#ifdef _POSIX_STREAM_MAX
        /* This is the default value: */
#else
        { "_POSIX_STREAM_MAX", 17, 0 },
#endif
#ifdef _POSIX_TZNAME_MAX
        /* This is the default value: */
#else
        { "_POSIX_TZNAME_MAX", 17, 0 },
#endif
#ifdef _POSIX_VDISABLE
        /* This is the default value: */
#else
        { "_POSIX_VDISABLE", 15, 0 },
#endif
#ifdef _POSIX_VERSION
        /* This is the default value: */
#else
        { "_POSIX_VERSION", 14, 0 },
#endif
        { NULL, 0, 0 } };
	const struct iv_s *value_for_iv = values_for_iv;

    static const struct nv_s values_for_nv[] =
      {
#ifdef DBL_DIG
        { "DBL_DIG", 7, DBL_DIG },
#endif
#ifdef DBL_MANT_DIG
        { "DBL_MANT_DIG", 12, DBL_MANT_DIG },
#endif
#ifdef DBL_MAX_10_EXP
        { "DBL_MAX_10_EXP", 14, DBL_MAX_10_EXP },
#endif
#ifdef DBL_MAX_EXP
        { "DBL_MAX_EXP", 11, DBL_MAX_EXP },
#endif
#ifdef DBL_MIN_10_EXP
        { "DBL_MIN_10_EXP", 14, DBL_MIN_10_EXP },
#endif
#ifdef DBL_MIN_EXP
        { "DBL_MIN_EXP", 11, DBL_MIN_EXP },
#endif
#ifdef FLT_DIG
        { "FLT_DIG", 7, FLT_DIG },
#endif
#ifdef FLT_MANT_DIG
        { "FLT_MANT_DIG", 12, FLT_MANT_DIG },
#endif
#ifdef FLT_MAX_10_EXP
        { "FLT_MAX_10_EXP", 14, FLT_MAX_10_EXP },
#endif
#ifdef FLT_MAX_EXP
        { "FLT_MAX_EXP", 11, FLT_MAX_EXP },
#endif
#ifdef FLT_MIN_10_EXP
        { "FLT_MIN_10_EXP", 14, FLT_MIN_10_EXP },
#endif
#ifdef FLT_MIN_EXP
        { "FLT_MIN_EXP", 11, FLT_MIN_EXP },
#endif
#ifdef FLT_RADIX
        { "FLT_RADIX", 9, FLT_RADIX },
#endif
#ifdef LDBL_DIG
        { "LDBL_DIG", 8, LDBL_DIG },
#endif
#ifdef LDBL_MANT_DIG
        { "LDBL_MANT_DIG", 13, LDBL_MANT_DIG },
#endif
#ifdef LDBL_MAX_10_EXP
        { "LDBL_MAX_10_EXP", 15, LDBL_MAX_10_EXP },
#endif
#ifdef LDBL_MAX_EXP
        { "LDBL_MAX_EXP", 12, LDBL_MAX_EXP },
#endif
#ifdef LDBL_MIN_10_EXP
        { "LDBL_MIN_10_EXP", 15, LDBL_MIN_10_EXP },
#endif
#ifdef LDBL_MIN_EXP
        { "LDBL_MIN_EXP", 12, LDBL_MIN_EXP },
#endif
        { NULL, 0, 0 } };
	const struct nv_s *value_for_nv = values_for_nv;

    static const struct uv_s values_for_uv[] =
      {
#ifdef SA_NOCLDSTOP
        { "SA_NOCLDSTOP", 12, SA_NOCLDSTOP },
#endif
#ifdef SA_NOCLDWAIT
        { "SA_NOCLDWAIT", 12, SA_NOCLDWAIT },
#endif
#ifdef SA_NODEFER
        { "SA_NODEFER", 10, SA_NODEFER },
#endif
#ifdef SA_ONSTACK
        { "SA_ONSTACK", 10, SA_ONSTACK },
#endif
#ifdef SA_RESETHAND
        { "SA_RESETHAND", 12, SA_RESETHAND },
#endif
#ifdef SA_RESTART
        { "SA_RESTART", 10, SA_RESTART },
#endif
#ifdef SA_SIGINFO
        { "SA_SIGINFO", 10, SA_SIGINFO },
#endif
#ifdef UCHAR_MAX
        { "UCHAR_MAX", 9, UCHAR_MAX },
#endif
#ifdef UINT_MAX
        { "UINT_MAX", 8, UINT_MAX },
#endif
#ifdef ULONG_MAX
        { "ULONG_MAX", 9, ULONG_MAX },
#endif
#ifdef USHRT_MAX
        { "USHRT_MAX", 9, USHRT_MAX },
#endif
        { NULL, 0, 0 } };
	const struct uv_s *value_for_uv = values_for_uv;

    static const struct yes_s values_for_yes[] =
      {
#ifdef _POSIX_JOB_CONTROL
        { "_POSIX_JOB_CONTROL", 18 },
#endif
#ifdef _POSIX_SAVED_IDS
        { "_POSIX_SAVED_IDS", 16 },
#endif
        { NULL, 0 } };
	const struct yes_s *value_for_yes = values_for_yes;
#ifndef SYMBIAN
	constant_missing = get_missing_hash(aTHX);
#endif
        while (value_for_iv->name) {
	    constant_add_symbol(aTHX_  symbol_table, value_for_iv->name,
				value_for_iv->namelen, newSViv(value_for_iv->value));
	    ++value_for_iv;
	}
        while (value_for_nv->name) {
	    constant_add_symbol(aTHX_  symbol_table, value_for_nv->name,
				value_for_nv->namelen, newSVnv(value_for_nv->value));
	    ++value_for_nv;
	}
        while (value_for_uv->name) {
	    constant_add_symbol(aTHX_  symbol_table, value_for_uv->name,
				value_for_uv->namelen, newSVuv(value_for_uv->value));
	    ++value_for_uv;
	}
        while (value_for_yes->name) {
	    constant_add_symbol(aTHX_  symbol_table, value_for_yes->name,
				value_for_yes->namelen, &PL_sv_yes);
	    ++value_for_yes;
	}
	while (value_for_notfound->name) {

	    /* Need to add prototypes, else parsing will vary by platform.  */
	    SV **sv = hv_fetch(symbol_table, value_for_notfound->name,
			       value_for_notfound->namelen, TRUE);
	    if (!sv) {
		Perl_croak(aTHX_ 
			   "Couldn't add key '%s' to %%POSIX::",
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
#ifdef CLK_TCK
        {
            IV temp0;
            temp0 = CLK_TCK;
	    constant_add_symbol(aTHX_  symbol_table, "CLK_TCK",
				    7, newSViv(temp0));
        }
#endif
#ifdef MB_CUR_MAX
        {
            IV temp0;
            temp0 = MB_CUR_MAX;
	    constant_add_symbol(aTHX_  symbol_table, "MB_CUR_MAX",
				    10, newSViv(temp0));
        }
#endif
#ifdef SIG_DFL
        {
            IV temp0;
            temp0 = PTR2IV(SIG_DFL);
	    constant_add_symbol(aTHX_  symbol_table, "SIG_DFL",
				    7, newSViv(temp0));
        }
#endif
#ifdef SIG_ERR
        {
            IV temp0;
            temp0 = PTR2IV(SIG_ERR);
	    constant_add_symbol(aTHX_  symbol_table, "SIG_ERR",
				    7, newSViv(temp0));
        }
#endif
#ifdef SIG_IGN
        {
            IV temp0;
            temp0 = PTR2IV(SIG_IGN);
	    constant_add_symbol(aTHX_  symbol_table, "SIG_IGN",
				    7, newSViv(temp0));
        }
#endif
#ifdef FLT_ROUNDS
        {
            NV temp0;
            temp0 = FLT_ROUNDS;
	    constant_add_symbol(aTHX_  symbol_table, "FLT_ROUNDS",
				    10, newSVnv(temp0));
        }
#endif
#if (defined(USE_LONG_DOUBLE) && defined(HUGE_VALL)) || defined(HUGE_VAL)
	/* HUGE_VALL is admittedly non-POSIX but if we are using long doubles
	 * we might as well use long doubles. --jhi */
        {
            NV temp0;
            temp0 = 
#if defined(USE_LONG_DOUBLE) && defined(HUGE_VALL)
                   HUGE_VALL
#else
                   HUGE_VAL
#endif
                           ;
	    constant_add_symbol(aTHX_  symbol_table, "HUGE_VAL",
				    8, newSVnv(temp0));
        }
#endif
#ifdef DBL_MAX
        {
            NV temp0;
            temp0 = DBL_MAX;
	    constant_add_symbol(aTHX_  symbol_table, "DBL_MAX",
				    7, newSVnv(temp0));
        }
#endif
#ifdef FLT_MAX
        {
            NV temp0;
            temp0 = FLT_MAX;
	    constant_add_symbol(aTHX_  symbol_table, "FLT_MAX",
				    7, newSVnv(temp0));
        }
#endif
#ifdef LDBL_MAX
        {
            NV temp0;
            temp0 = LDBL_MAX;
	    constant_add_symbol(aTHX_  symbol_table, "LDBL_MAX",
				    8, newSVnv(temp0));
        }
#endif
#ifdef LDBL_MIN
        {
            NV temp0;
            temp0 = LDBL_MIN;
	    constant_add_symbol(aTHX_  symbol_table, "LDBL_MIN",
				    8, newSVnv(temp0));
        }
#endif
#ifdef LDBL_EPSILON
        {
            NV temp0;
            temp0 = LDBL_EPSILON;
	    constant_add_symbol(aTHX_  symbol_table, "LDBL_EPSILON",
				    12, newSVnv(temp0));
        }
#endif
#ifdef DBL_EPSILON
        {
            NV temp0;
            temp0 = DBL_EPSILON;
	    constant_add_symbol(aTHX_  symbol_table, "DBL_EPSILON",
				    11, newSVnv(temp0));
        }
#endif
#ifdef DBL_MIN
        {
            NV temp0;
            temp0 = DBL_MIN;
	    constant_add_symbol(aTHX_  symbol_table, "DBL_MIN",
				    7, newSVnv(temp0));
        }
#endif
#ifdef FLT_EPSILON
        {
            NV temp0;
            temp0 = FLT_EPSILON;
	    constant_add_symbol(aTHX_  symbol_table, "FLT_EPSILON",
				    11, newSVnv(temp0));
        }
#endif
#ifdef FLT_MIN
        {
            NV temp0;
            temp0 = FLT_MIN;
	    constant_add_symbol(aTHX_  symbol_table, "FLT_MIN",
				    7, newSVnv(temp0));
        }
#endif
#ifdef SIGRTMAX
        {
            IV temp0;
            temp0 = SIGRTMAX;
	    constant_add_symbol(aTHX_  symbol_table, "SIGRTMAX",
				    8, newSViv(temp0));
        }
#endif
#ifdef SIGRTMIN
        {
            IV temp0;
            temp0 = SIGRTMIN;
	    constant_add_symbol(aTHX_  symbol_table, "SIGRTMIN",
				    8, newSViv(temp0));
        }
#endif
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
	sv = newSVpvf("%"SVf" is not a valid POSIX macro", sv);
#else
	HV *constant_missing = get_missing_hash(aTHX);
	if (hv_exists(constant_missing, s, SvUTF8(sv) ? -(I32)len : (I32)len)) {
	    sv = newSVpvf("Your vendor has not defined POSIX macro %" SVf
			  ", used", sv);
	} else {
	    sv = newSVpvf("%"SVf" is not a valid POSIX macro",
			  sv);
	}
#endif
	PUSHs(sv_2mortal(sv));

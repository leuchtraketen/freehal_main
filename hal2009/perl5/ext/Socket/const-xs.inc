BOOT:
  {
#ifdef dTHX
    dTHX;
#endif
    HV *symbol_table = get_hv("Socket::", TRUE);
#ifndef SYMBIAN
    HV *constant_missing;
#endif

    static const struct notfound_s values_for_notfound[] =
      {
#ifdef AF_802
#else
        { "AF_802", 6 },
#endif
#ifdef AF_AAL
#else
        { "AF_AAL", 6 },
#endif
#ifdef AF_APPLETALK
#else
        { "AF_APPLETALK", 12 },
#endif
#ifdef AF_CCITT
#else
        { "AF_CCITT", 8 },
#endif
#ifdef AF_CHAOS
#else
        { "AF_CHAOS", 8 },
#endif
#ifdef AF_CTF
#else
        { "AF_CTF", 6 },
#endif
#ifdef AF_DATAKIT
#else
        { "AF_DATAKIT", 10 },
#endif
#ifdef AF_DECnet
#else
        { "AF_DECnet", 9 },
#endif
#ifdef AF_DLI
#else
        { "AF_DLI", 6 },
#endif
#ifdef AF_ECMA
#else
        { "AF_ECMA", 7 },
#endif
#ifdef AF_GOSIP
#else
        { "AF_GOSIP", 8 },
#endif
#ifdef AF_HYLINK
#else
        { "AF_HYLINK", 9 },
#endif
#ifdef AF_IMPLINK
#else
        { "AF_IMPLINK", 10 },
#endif
#ifdef AF_INET
#else
        { "AF_INET", 7 },
#endif
#ifdef AF_INET6
#else
        { "AF_INET6", 8 },
#endif
#ifdef AF_ISO
#else
        { "AF_ISO", 6 },
#endif
#ifdef AF_KEY
#else
        { "AF_KEY", 6 },
#endif
#ifdef AF_LAST
#else
        { "AF_LAST", 7 },
#endif
#ifdef AF_LAT
#else
        { "AF_LAT", 6 },
#endif
#ifdef AF_LINK
#else
        { "AF_LINK", 7 },
#endif
#ifdef AF_MAX
#else
        { "AF_MAX", 6 },
#endif
#ifdef AF_NBS
#else
        { "AF_NBS", 6 },
#endif
#ifdef AF_NIT
#else
        { "AF_NIT", 6 },
#endif
#ifdef AF_NS
#else
        { "AF_NS", 5 },
#endif
#ifdef AF_OSI
#else
        { "AF_OSI", 6 },
#endif
#ifdef AF_OSINET
#else
        { "AF_OSINET", 9 },
#endif
#ifdef AF_PUP
#else
        { "AF_PUP", 6 },
#endif
#ifdef AF_ROUTE
#else
        { "AF_ROUTE", 8 },
#endif
#ifdef AF_SNA
#else
        { "AF_SNA", 6 },
#endif
#ifdef AF_UNIX
#else
        { "AF_UNIX", 7 },
#endif
#ifdef AF_UNSPEC
#else
        { "AF_UNSPEC", 9 },
#endif
#ifdef AF_USER
#else
        { "AF_USER", 7 },
#endif
#ifdef AF_WAN
#else
        { "AF_WAN", 6 },
#endif
#ifdef AF_X25
#else
        { "AF_X25", 6 },
#endif
#ifdef IOV_MAX
#else
        { "IOV_MAX", 7 },
#endif
#ifdef IP_OPTIONS
#else
        { "IP_OPTIONS", 10 },
#endif
#ifdef IP_HDRINCL
#else
        { "IP_HDRINCL", 10 },
#endif
#ifdef IP_TOS
#else
        { "IP_TOS", 6 },
#endif
#ifdef IP_TTL
#else
        { "IP_TTL", 6 },
#endif
#ifdef IP_RECVOPTS
#else
        { "IP_RECVOPTS", 11 },
#endif
#ifdef IP_RECVRETOPTS
#else
        { "IP_RECVRETOPTS", 14 },
#endif
#ifdef IP_RETOPTS
#else
        { "IP_RETOPTS", 10 },
#endif
#ifdef MSG_BCAST
#else
        { "MSG_BCAST", 9 },
#endif
#ifdef MSG_BTAG
#else
        { "MSG_BTAG", 8 },
#endif
#ifdef MSG_CTLFLAGS
#else
        { "MSG_CTLFLAGS", 12 },
#endif
#ifdef MSG_CTLIGNORE
#else
        { "MSG_CTLIGNORE", 13 },
#endif
#ifdef MSG_DONTWAIT
#else
        { "MSG_DONTWAIT", 12 },
#endif
#ifdef MSG_EOF
#else
        { "MSG_EOF", 7 },
#endif
#ifdef MSG_EOR
#else
        { "MSG_EOR", 7 },
#endif
#ifdef MSG_ERRQUEUE
#else
        { "MSG_ERRQUEUE", 12 },
#endif
#ifdef MSG_ETAG
#else
        { "MSG_ETAG", 8 },
#endif
#ifdef MSG_FIN
#else
        { "MSG_FIN", 7 },
#endif
#ifdef MSG_MAXIOVLEN
#else
        { "MSG_MAXIOVLEN", 13 },
#endif
#ifdef MSG_MCAST
#else
        { "MSG_MCAST", 9 },
#endif
#ifdef MSG_NOSIGNAL
#else
        { "MSG_NOSIGNAL", 12 },
#endif
#ifdef MSG_RST
#else
        { "MSG_RST", 7 },
#endif
#ifdef MSG_SYN
#else
        { "MSG_SYN", 7 },
#endif
#ifdef MSG_TRUNC
#else
        { "MSG_TRUNC", 9 },
#endif
#ifdef MSG_URG
#else
        { "MSG_URG", 7 },
#endif
#ifdef MSG_WAITALL
#else
        { "MSG_WAITALL", 11 },
#endif
#ifdef MSG_WIRE
#else
        { "MSG_WIRE", 8 },
#endif
#ifdef PF_802
#else
        { "PF_802", 6 },
#endif
#ifdef PF_AAL
#else
        { "PF_AAL", 6 },
#endif
#ifdef PF_APPLETALK
#else
        { "PF_APPLETALK", 12 },
#endif
#ifdef PF_CCITT
#else
        { "PF_CCITT", 8 },
#endif
#ifdef PF_CHAOS
#else
        { "PF_CHAOS", 8 },
#endif
#ifdef PF_CTF
#else
        { "PF_CTF", 6 },
#endif
#ifdef PF_DATAKIT
#else
        { "PF_DATAKIT", 10 },
#endif
#ifdef PF_DECnet
#else
        { "PF_DECnet", 9 },
#endif
#ifdef PF_DLI
#else
        { "PF_DLI", 6 },
#endif
#ifdef PF_ECMA
#else
        { "PF_ECMA", 7 },
#endif
#ifdef PF_GOSIP
#else
        { "PF_GOSIP", 8 },
#endif
#ifdef PF_HYLINK
#else
        { "PF_HYLINK", 9 },
#endif
#ifdef PF_IMPLINK
#else
        { "PF_IMPLINK", 10 },
#endif
#ifdef PF_INET
#else
        { "PF_INET", 7 },
#endif
#ifdef PF_INET6
#else
        { "PF_INET6", 8 },
#endif
#ifdef PF_ISO
#else
        { "PF_ISO", 6 },
#endif
#ifdef PF_KEY
#else
        { "PF_KEY", 6 },
#endif
#ifdef PF_LAST
#else
        { "PF_LAST", 7 },
#endif
#ifdef PF_LAT
#else
        { "PF_LAT", 6 },
#endif
#ifdef PF_LINK
#else
        { "PF_LINK", 7 },
#endif
#ifdef PF_MAX
#else
        { "PF_MAX", 6 },
#endif
#ifdef PF_NBS
#else
        { "PF_NBS", 6 },
#endif
#ifdef PF_NIT
#else
        { "PF_NIT", 6 },
#endif
#ifdef PF_NS
#else
        { "PF_NS", 5 },
#endif
#ifdef PF_OSI
#else
        { "PF_OSI", 6 },
#endif
#ifdef PF_OSINET
#else
        { "PF_OSINET", 9 },
#endif
#ifdef PF_PUP
#else
        { "PF_PUP", 6 },
#endif
#ifdef PF_ROUTE
#else
        { "PF_ROUTE", 8 },
#endif
#ifdef PF_SNA
#else
        { "PF_SNA", 6 },
#endif
#ifdef PF_UNIX
#else
        { "PF_UNIX", 7 },
#endif
#ifdef PF_UNSPEC
#else
        { "PF_UNSPEC", 9 },
#endif
#ifdef PF_USER
#else
        { "PF_USER", 7 },
#endif
#ifdef PF_WAN
#else
        { "PF_WAN", 6 },
#endif
#ifdef PF_X25
#else
        { "PF_X25", 6 },
#endif
#ifdef SCM_CONNECT
#else
        { "SCM_CONNECT", 11 },
#endif
#ifdef SCM_CREDENTIALS
#else
        { "SCM_CREDENTIALS", 15 },
#endif
#ifdef SCM_CREDS
#else
        { "SCM_CREDS", 9 },
#endif
#ifdef SCM_TIMESTAMP
#else
        { "SCM_TIMESTAMP", 13 },
#endif
#ifdef SOCK_DGRAM
#else
        { "SOCK_DGRAM", 10 },
#endif
#ifdef SOCK_RAW
#else
        { "SOCK_RAW", 8 },
#endif
#ifdef SOCK_RDM
#else
        { "SOCK_RDM", 8 },
#endif
#ifdef SOCK_SEQPACKET
#else
        { "SOCK_SEQPACKET", 14 },
#endif
#ifdef SOCK_STREAM
#else
        { "SOCK_STREAM", 11 },
#endif
#ifdef SOL_SOCKET
#else
        { "SOL_SOCKET", 10 },
#endif
#ifdef SOMAXCONN
#else
        { "SOMAXCONN", 9 },
#endif
#ifdef SO_ACCEPTCONN
#else
        { "SO_ACCEPTCONN", 13 },
#endif
#ifdef SO_ATTACH_FILTER
#else
        { "SO_ATTACH_FILTER", 16 },
#endif
#ifdef SO_BACKLOG
#else
        { "SO_BACKLOG", 10 },
#endif
#ifdef SO_BROADCAST
#else
        { "SO_BROADCAST", 12 },
#endif
#ifdef SO_CHAMELEON
#else
        { "SO_CHAMELEON", 12 },
#endif
#ifdef SO_DEBUG
#else
        { "SO_DEBUG", 8 },
#endif
#ifdef SO_DETACH_FILTER
#else
        { "SO_DETACH_FILTER", 16 },
#endif
#ifdef SO_DGRAM_ERRIND
#else
        { "SO_DGRAM_ERRIND", 15 },
#endif
#ifdef SO_DONTLINGER
#else
        { "SO_DONTLINGER", 13 },
#endif
#ifdef SO_DONTROUTE
#else
        { "SO_DONTROUTE", 12 },
#endif
#ifdef SO_ERROR
#else
        { "SO_ERROR", 8 },
#endif
#ifdef SO_FAMILY
#else
        { "SO_FAMILY", 9 },
#endif
#ifdef SO_KEEPALIVE
#else
        { "SO_KEEPALIVE", 12 },
#endif
#ifdef SO_LINGER
#else
        { "SO_LINGER", 9 },
#endif
#ifdef SO_OOBINLINE
#else
        { "SO_OOBINLINE", 12 },
#endif
#ifdef SO_PASSCRED
#else
        { "SO_PASSCRED", 11 },
#endif
#ifdef SO_PASSIFNAME
#else
        { "SO_PASSIFNAME", 13 },
#endif
#ifdef SO_PEERCRED
#else
        { "SO_PEERCRED", 11 },
#endif
#ifdef SO_PROTOCOL
#else
        { "SO_PROTOCOL", 11 },
#endif
#ifdef SO_PROTOTYPE
#else
        { "SO_PROTOTYPE", 12 },
#endif
#ifdef SO_RCVBUF
#else
        { "SO_RCVBUF", 9 },
#endif
#ifdef SO_RCVLOWAT
#else
        { "SO_RCVLOWAT", 11 },
#endif
#ifdef SO_RCVTIMEO
#else
        { "SO_RCVTIMEO", 11 },
#endif
#ifdef SO_REUSEADDR
#else
        { "SO_REUSEADDR", 12 },
#endif
#ifdef SO_REUSEPORT
#else
        { "SO_REUSEPORT", 12 },
#endif
#ifdef SO_SECURITY_AUTHENTICATION
#else
        { "SO_SECURITY_AUTHENTICATION", 26 },
#endif
#ifdef SO_SECURITY_ENCRYPTION_NETWORK
#else
        { "SO_SECURITY_ENCRYPTION_NETWORK", 30 },
#endif
#ifdef SO_SECURITY_ENCRYPTION_TRANSPORT
#else
        { "SO_SECURITY_ENCRYPTION_TRANSPORT", 32 },
#endif
#ifdef SO_SNDBUF
#else
        { "SO_SNDBUF", 9 },
#endif
#ifdef SO_SNDLOWAT
#else
        { "SO_SNDLOWAT", 11 },
#endif
#ifdef SO_SNDTIMEO
#else
        { "SO_SNDTIMEO", 11 },
#endif
#ifdef SO_STATE
#else
        { "SO_STATE", 8 },
#endif
#ifdef SO_TYPE
#else
        { "SO_TYPE", 7 },
#endif
#ifdef SO_USELOOPBACK
#else
        { "SO_USELOOPBACK", 14 },
#endif
#ifdef SO_XOPEN
#else
        { "SO_XOPEN", 8 },
#endif
#ifdef SO_XSE
#else
        { "SO_XSE", 6 },
#endif
#ifdef TCP_KEEPALIVE
#else
        { "TCP_KEEPALIVE", 13 },
#endif
#ifdef TCP_MAXRT
#else
        { "TCP_MAXRT", 9 },
#endif
#ifdef TCP_MAXSEG
#else
        { "TCP_MAXSEG", 10 },
#endif
#ifdef TCP_NODELAY
#else
        { "TCP_NODELAY", 11 },
#endif
#ifdef TCP_STDURG
#else
        { "TCP_STDURG", 10 },
#endif
#ifdef UIO_MAXIOV
#else
        { "UIO_MAXIOV", 10 },
#endif
#if defined(MSG_CTRUNC) || defined(HAS_MSG_CTRUNC) /* might be an enum */
#else
        { "MSG_CTRUNC", 10 },
#endif
#if defined(MSG_DONTROUTE) || defined(HAS_MSG_DONTROUTE) /* might be an enum */
#else
        { "MSG_DONTROUTE", 13 },
#endif
#if defined(MSG_OOB) || defined(HAS_MSG_OOB) /* might be an enum */
#else
        { "MSG_OOB", 7 },
#endif
#if defined(MSG_PEEK) || defined(HAS_MSG_PEEK) /* might be an enum */
#else
        { "MSG_PEEK", 8 },
#endif
#if defined(MSG_PROXY) || defined(HAS_MSG_PROXY) /* might be an enum */
#else
        { "MSG_PROXY", 9 },
#endif
#if defined(SCM_RIGHTS) || defined(HAS_SCM_RIGHTS) /* might be an enum */
#else
        { "SCM_RIGHTS", 10 },
#endif
#ifdef INADDR_ANY
#else
        { "INADDR_ANY", 10 },
#endif
#ifdef INADDR_LOOPBACK
#else
        { "INADDR_LOOPBACK", 15 },
#endif
#ifdef INADDR_NONE
#else
        { "INADDR_NONE", 11 },
#endif
#ifdef INADDR_BROADCAST
#else
        { "INADDR_BROADCAST", 16 },
#endif
        { NULL, 0 } };
	const struct notfound_s *value_for_notfound = values_for_notfound;

    static const struct iv_s values_for_iv[] =
      {
#ifdef AF_802
        { "AF_802", 6, AF_802 },
#endif
#ifdef AF_AAL
        { "AF_AAL", 6, AF_AAL },
#endif
#ifdef AF_APPLETALK
        { "AF_APPLETALK", 12, AF_APPLETALK },
#endif
#ifdef AF_CCITT
        { "AF_CCITT", 8, AF_CCITT },
#endif
#ifdef AF_CHAOS
        { "AF_CHAOS", 8, AF_CHAOS },
#endif
#ifdef AF_CTF
        { "AF_CTF", 6, AF_CTF },
#endif
#ifdef AF_DATAKIT
        { "AF_DATAKIT", 10, AF_DATAKIT },
#endif
#ifdef AF_DECnet
        { "AF_DECnet", 9, AF_DECnet },
#endif
#ifdef AF_DLI
        { "AF_DLI", 6, AF_DLI },
#endif
#ifdef AF_ECMA
        { "AF_ECMA", 7, AF_ECMA },
#endif
#ifdef AF_GOSIP
        { "AF_GOSIP", 8, AF_GOSIP },
#endif
#ifdef AF_HYLINK
        { "AF_HYLINK", 9, AF_HYLINK },
#endif
#ifdef AF_IMPLINK
        { "AF_IMPLINK", 10, AF_IMPLINK },
#endif
#ifdef AF_INET
        { "AF_INET", 7, AF_INET },
#endif
#ifdef AF_INET6
        { "AF_INET6", 8, AF_INET6 },
#endif
#ifdef AF_ISO
        { "AF_ISO", 6, AF_ISO },
#endif
#ifdef AF_KEY
        { "AF_KEY", 6, AF_KEY },
#endif
#ifdef AF_LAST
        { "AF_LAST", 7, AF_LAST },
#endif
#ifdef AF_LAT
        { "AF_LAT", 6, AF_LAT },
#endif
#ifdef AF_LINK
        { "AF_LINK", 7, AF_LINK },
#endif
#ifdef AF_MAX
        { "AF_MAX", 6, AF_MAX },
#endif
#ifdef AF_NBS
        { "AF_NBS", 6, AF_NBS },
#endif
#ifdef AF_NIT
        { "AF_NIT", 6, AF_NIT },
#endif
#ifdef AF_NS
        { "AF_NS", 5, AF_NS },
#endif
#ifdef AF_OSI
        { "AF_OSI", 6, AF_OSI },
#endif
#ifdef AF_OSINET
        { "AF_OSINET", 9, AF_OSINET },
#endif
#ifdef AF_PUP
        { "AF_PUP", 6, AF_PUP },
#endif
#ifdef AF_ROUTE
        { "AF_ROUTE", 8, AF_ROUTE },
#endif
#ifdef AF_SNA
        { "AF_SNA", 6, AF_SNA },
#endif
#ifdef AF_UNIX
        { "AF_UNIX", 7, AF_UNIX },
#endif
#ifdef AF_UNSPEC
        { "AF_UNSPEC", 9, AF_UNSPEC },
#endif
#ifdef AF_USER
        { "AF_USER", 7, AF_USER },
#endif
#ifdef AF_WAN
        { "AF_WAN", 6, AF_WAN },
#endif
#ifdef AF_X25
        { "AF_X25", 6, AF_X25 },
#endif
#ifdef IOV_MAX
        { "IOV_MAX", 7, IOV_MAX },
#endif
#ifdef IP_OPTIONS
        { "IP_OPTIONS", 10, IP_OPTIONS },
#endif
#ifdef IP_HDRINCL
        { "IP_HDRINCL", 10, IP_HDRINCL },
#endif
#ifdef IP_TOS
        { "IP_TOS", 6, IP_TOS },
#endif
#ifdef IP_TTL
        { "IP_TTL", 6, IP_TTL },
#endif
#ifdef IP_RECVOPTS
        { "IP_RECVOPTS", 11, IP_RECVOPTS },
#endif
#ifdef IP_RECVRETOPTS
        { "IP_RECVRETOPTS", 14, IP_RECVRETOPTS },
#endif
#ifdef IP_RETOPTS
        { "IP_RETOPTS", 10, IP_RETOPTS },
#endif
#ifdef MSG_BCAST
        { "MSG_BCAST", 9, MSG_BCAST },
#endif
#ifdef MSG_BTAG
        { "MSG_BTAG", 8, MSG_BTAG },
#endif
#ifdef MSG_CTLFLAGS
        { "MSG_CTLFLAGS", 12, MSG_CTLFLAGS },
#endif
#ifdef MSG_CTLIGNORE
        { "MSG_CTLIGNORE", 13, MSG_CTLIGNORE },
#endif
#ifdef MSG_DONTWAIT
        { "MSG_DONTWAIT", 12, MSG_DONTWAIT },
#endif
#ifdef MSG_EOF
        { "MSG_EOF", 7, MSG_EOF },
#endif
#ifdef MSG_EOR
        { "MSG_EOR", 7, MSG_EOR },
#endif
#ifdef MSG_ERRQUEUE
        { "MSG_ERRQUEUE", 12, MSG_ERRQUEUE },
#endif
#ifdef MSG_ETAG
        { "MSG_ETAG", 8, MSG_ETAG },
#endif
#ifdef MSG_FIN
        { "MSG_FIN", 7, MSG_FIN },
#endif
#ifdef MSG_MAXIOVLEN
        { "MSG_MAXIOVLEN", 13, MSG_MAXIOVLEN },
#endif
#ifdef MSG_MCAST
        { "MSG_MCAST", 9, MSG_MCAST },
#endif
#ifdef MSG_NOSIGNAL
        { "MSG_NOSIGNAL", 12, MSG_NOSIGNAL },
#endif
#ifdef MSG_RST
        { "MSG_RST", 7, MSG_RST },
#endif
#ifdef MSG_SYN
        { "MSG_SYN", 7, MSG_SYN },
#endif
#ifdef MSG_TRUNC
        { "MSG_TRUNC", 9, MSG_TRUNC },
#endif
#ifdef MSG_URG
        { "MSG_URG", 7, MSG_URG },
#endif
#ifdef MSG_WAITALL
        { "MSG_WAITALL", 11, MSG_WAITALL },
#endif
#ifdef MSG_WIRE
        { "MSG_WIRE", 8, MSG_WIRE },
#endif
#ifdef PF_802
        { "PF_802", 6, PF_802 },
#endif
#ifdef PF_AAL
        { "PF_AAL", 6, PF_AAL },
#endif
#ifdef PF_APPLETALK
        { "PF_APPLETALK", 12, PF_APPLETALK },
#endif
#ifdef PF_CCITT
        { "PF_CCITT", 8, PF_CCITT },
#endif
#ifdef PF_CHAOS
        { "PF_CHAOS", 8, PF_CHAOS },
#endif
#ifdef PF_CTF
        { "PF_CTF", 6, PF_CTF },
#endif
#ifdef PF_DATAKIT
        { "PF_DATAKIT", 10, PF_DATAKIT },
#endif
#ifdef PF_DECnet
        { "PF_DECnet", 9, PF_DECnet },
#endif
#ifdef PF_DLI
        { "PF_DLI", 6, PF_DLI },
#endif
#ifdef PF_ECMA
        { "PF_ECMA", 7, PF_ECMA },
#endif
#ifdef PF_GOSIP
        { "PF_GOSIP", 8, PF_GOSIP },
#endif
#ifdef PF_HYLINK
        { "PF_HYLINK", 9, PF_HYLINK },
#endif
#ifdef PF_IMPLINK
        { "PF_IMPLINK", 10, PF_IMPLINK },
#endif
#ifdef PF_INET
        { "PF_INET", 7, PF_INET },
#endif
#ifdef PF_INET6
        { "PF_INET6", 8, PF_INET6 },
#endif
#ifdef PF_ISO
        { "PF_ISO", 6, PF_ISO },
#endif
#ifdef PF_KEY
        { "PF_KEY", 6, PF_KEY },
#endif
#ifdef PF_LAST
        { "PF_LAST", 7, PF_LAST },
#endif
#ifdef PF_LAT
        { "PF_LAT", 6, PF_LAT },
#endif
#ifdef PF_LINK
        { "PF_LINK", 7, PF_LINK },
#endif
#ifdef PF_MAX
        { "PF_MAX", 6, PF_MAX },
#endif
#ifdef PF_NBS
        { "PF_NBS", 6, PF_NBS },
#endif
#ifdef PF_NIT
        { "PF_NIT", 6, PF_NIT },
#endif
#ifdef PF_NS
        { "PF_NS", 5, PF_NS },
#endif
#ifdef PF_OSI
        { "PF_OSI", 6, PF_OSI },
#endif
#ifdef PF_OSINET
        { "PF_OSINET", 9, PF_OSINET },
#endif
#ifdef PF_PUP
        { "PF_PUP", 6, PF_PUP },
#endif
#ifdef PF_ROUTE
        { "PF_ROUTE", 8, PF_ROUTE },
#endif
#ifdef PF_SNA
        { "PF_SNA", 6, PF_SNA },
#endif
#ifdef PF_UNIX
        { "PF_UNIX", 7, PF_UNIX },
#endif
#ifdef PF_UNSPEC
        { "PF_UNSPEC", 9, PF_UNSPEC },
#endif
#ifdef PF_USER
        { "PF_USER", 7, PF_USER },
#endif
#ifdef PF_WAN
        { "PF_WAN", 6, PF_WAN },
#endif
#ifdef PF_X25
        { "PF_X25", 6, PF_X25 },
#endif
#ifdef SCM_CONNECT
        { "SCM_CONNECT", 11, SCM_CONNECT },
#endif
#ifdef SCM_CREDENTIALS
        { "SCM_CREDENTIALS", 15, SCM_CREDENTIALS },
#endif
#ifdef SCM_CREDS
        { "SCM_CREDS", 9, SCM_CREDS },
#endif
#ifdef SCM_TIMESTAMP
        { "SCM_TIMESTAMP", 13, SCM_TIMESTAMP },
#endif
#ifdef SOCK_DGRAM
        { "SOCK_DGRAM", 10, SOCK_DGRAM },
#endif
#ifdef SOCK_RAW
        { "SOCK_RAW", 8, SOCK_RAW },
#endif
#ifdef SOCK_RDM
        { "SOCK_RDM", 8, SOCK_RDM },
#endif
#ifdef SOCK_SEQPACKET
        { "SOCK_SEQPACKET", 14, SOCK_SEQPACKET },
#endif
#ifdef SOCK_STREAM
        { "SOCK_STREAM", 11, SOCK_STREAM },
#endif
#ifdef SOL_SOCKET
        { "SOL_SOCKET", 10, SOL_SOCKET },
#endif
#ifdef SOMAXCONN
        { "SOMAXCONN", 9, SOMAXCONN },
#endif
#ifdef SO_ACCEPTCONN
        { "SO_ACCEPTCONN", 13, SO_ACCEPTCONN },
#endif
#ifdef SO_ATTACH_FILTER
        { "SO_ATTACH_FILTER", 16, SO_ATTACH_FILTER },
#endif
#ifdef SO_BACKLOG
        { "SO_BACKLOG", 10, SO_BACKLOG },
#endif
#ifdef SO_BROADCAST
        { "SO_BROADCAST", 12, SO_BROADCAST },
#endif
#ifdef SO_CHAMELEON
        { "SO_CHAMELEON", 12, SO_CHAMELEON },
#endif
#ifdef SO_DEBUG
        { "SO_DEBUG", 8, SO_DEBUG },
#endif
#ifdef SO_DETACH_FILTER
        { "SO_DETACH_FILTER", 16, SO_DETACH_FILTER },
#endif
#ifdef SO_DGRAM_ERRIND
        { "SO_DGRAM_ERRIND", 15, SO_DGRAM_ERRIND },
#endif
#ifdef SO_DONTLINGER
        { "SO_DONTLINGER", 13, SO_DONTLINGER },
#endif
#ifdef SO_DONTROUTE
        { "SO_DONTROUTE", 12, SO_DONTROUTE },
#endif
#ifdef SO_ERROR
        { "SO_ERROR", 8, SO_ERROR },
#endif
#ifdef SO_FAMILY
        { "SO_FAMILY", 9, SO_FAMILY },
#endif
#ifdef SO_KEEPALIVE
        { "SO_KEEPALIVE", 12, SO_KEEPALIVE },
#endif
#ifdef SO_LINGER
        { "SO_LINGER", 9, SO_LINGER },
#endif
#ifdef SO_OOBINLINE
        { "SO_OOBINLINE", 12, SO_OOBINLINE },
#endif
#ifdef SO_PASSCRED
        { "SO_PASSCRED", 11, SO_PASSCRED },
#endif
#ifdef SO_PASSIFNAME
        { "SO_PASSIFNAME", 13, SO_PASSIFNAME },
#endif
#ifdef SO_PEERCRED
        { "SO_PEERCRED", 11, SO_PEERCRED },
#endif
#ifdef SO_PROTOCOL
        { "SO_PROTOCOL", 11, SO_PROTOCOL },
#endif
#ifdef SO_PROTOTYPE
        { "SO_PROTOTYPE", 12, SO_PROTOTYPE },
#endif
#ifdef SO_RCVBUF
        { "SO_RCVBUF", 9, SO_RCVBUF },
#endif
#ifdef SO_RCVLOWAT
        { "SO_RCVLOWAT", 11, SO_RCVLOWAT },
#endif
#ifdef SO_RCVTIMEO
        { "SO_RCVTIMEO", 11, SO_RCVTIMEO },
#endif
#ifdef SO_REUSEADDR
        { "SO_REUSEADDR", 12, SO_REUSEADDR },
#endif
#ifdef SO_REUSEPORT
        { "SO_REUSEPORT", 12, SO_REUSEPORT },
#endif
#ifdef SO_SECURITY_AUTHENTICATION
        { "SO_SECURITY_AUTHENTICATION", 26, SO_SECURITY_AUTHENTICATION },
#endif
#ifdef SO_SECURITY_ENCRYPTION_NETWORK
        { "SO_SECURITY_ENCRYPTION_NETWORK", 30, SO_SECURITY_ENCRYPTION_NETWORK },
#endif
#ifdef SO_SECURITY_ENCRYPTION_TRANSPORT
        { "SO_SECURITY_ENCRYPTION_TRANSPORT", 32, SO_SECURITY_ENCRYPTION_TRANSPORT },
#endif
#ifdef SO_SNDBUF
        { "SO_SNDBUF", 9, SO_SNDBUF },
#endif
#ifdef SO_SNDLOWAT
        { "SO_SNDLOWAT", 11, SO_SNDLOWAT },
#endif
#ifdef SO_SNDTIMEO
        { "SO_SNDTIMEO", 11, SO_SNDTIMEO },
#endif
#ifdef SO_STATE
        { "SO_STATE", 8, SO_STATE },
#endif
#ifdef SO_TYPE
        { "SO_TYPE", 7, SO_TYPE },
#endif
#ifdef SO_USELOOPBACK
        { "SO_USELOOPBACK", 14, SO_USELOOPBACK },
#endif
#ifdef SO_XOPEN
        { "SO_XOPEN", 8, SO_XOPEN },
#endif
#ifdef SO_XSE
        { "SO_XSE", 6, SO_XSE },
#endif
#ifdef TCP_KEEPALIVE
        { "TCP_KEEPALIVE", 13, TCP_KEEPALIVE },
#endif
#ifdef TCP_MAXRT
        { "TCP_MAXRT", 9, TCP_MAXRT },
#endif
#ifdef TCP_MAXSEG
        { "TCP_MAXSEG", 10, TCP_MAXSEG },
#endif
#ifdef TCP_NODELAY
        { "TCP_NODELAY", 11, TCP_NODELAY },
#endif
#ifdef TCP_STDURG
        { "TCP_STDURG", 10, TCP_STDURG },
#endif
#ifdef UIO_MAXIOV
        { "UIO_MAXIOV", 10, UIO_MAXIOV },
#endif
#ifdef IPPROTO_IP
        { "IPPROTO_IP", 10, IPPROTO_IP },
#endif
#ifdef IPPROTO_IPV6
        { "IPPROTO_IPV6", 12, IPPROTO_IPV6 },
#endif
#ifdef IPPROTO_RAW
        { "IPPROTO_RAW", 11, IPPROTO_RAW },
#endif
#ifdef IPPROTO_ICMP
        { "IPPROTO_ICMP", 12, IPPROTO_ICMP },
#endif
#ifdef IPPROTO_TCP
        { "IPPROTO_TCP", 11, IPPROTO_TCP },
#endif
#ifdef IPPROTO_UDP
        { "IPPROTO_UDP", 11, IPPROTO_UDP },
#endif
#ifdef SHUT_RD
        { "SHUT_RD", 7, SHUT_RD },
#endif
#ifdef SHUT_WR
        { "SHUT_WR", 7, SHUT_WR },
#endif
#ifdef SHUT_RDWR
        { "SHUT_RDWR", 9, SHUT_RDWR },
#endif
#if defined(MSG_CTRUNC) || defined(HAS_MSG_CTRUNC) /* might be an enum */
        { "MSG_CTRUNC", 10, MSG_CTRUNC },
#endif
#if defined(MSG_DONTROUTE) || defined(HAS_MSG_DONTROUTE) /* might be an enum */
        { "MSG_DONTROUTE", 13, MSG_DONTROUTE },
#endif
#if defined(MSG_OOB) || defined(HAS_MSG_OOB) /* might be an enum */
        { "MSG_OOB", 7, MSG_OOB },
#endif
#if defined(MSG_PEEK) || defined(HAS_MSG_PEEK) /* might be an enum */
        { "MSG_PEEK", 8, MSG_PEEK },
#endif
#if defined(MSG_PROXY) || defined(HAS_MSG_PROXY) /* might be an enum */
        { "MSG_PROXY", 9, MSG_PROXY },
#endif
#if defined(SCM_RIGHTS) || defined(HAS_SCM_RIGHTS) /* might be an enum */
        { "SCM_RIGHTS", 10, SCM_RIGHTS },
#endif
#ifdef IPPROTO_IP
        /* This is the default value: */
#else
        { "IPPROTO_IP", 10, 0 },
#endif
#ifdef IPPROTO_IPV6
        /* This is the default value: */
#else
        { "IPPROTO_IPV6", 12, 41 },
#endif
#ifdef IPPROTO_RAW
        /* This is the default value: */
#else
        { "IPPROTO_RAW", 11, 255 },
#endif
#ifdef IPPROTO_ICMP
        /* This is the default value: */
#else
        { "IPPROTO_ICMP", 12, 1 },
#endif
#ifdef IPPROTO_TCP
        /* This is the default value: */
#else
        { "IPPROTO_TCP", 11, 6 },
#endif
#ifdef IPPROTO_UDP
        /* This is the default value: */
#else
        { "IPPROTO_UDP", 11, 17 },
#endif
#ifdef SHUT_RD
        /* This is the default value: */
#else
        { "SHUT_RD", 7, 0 },
#endif
#ifdef SHUT_WR
        /* This is the default value: */
#else
        { "SHUT_WR", 7, 1 },
#endif
#ifdef SHUT_RDWR
        /* This is the default value: */
#else
        { "SHUT_RDWR", 9, 2 },
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
			   "Couldn't add key '%s' to %%Socket::",
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
#ifdef INADDR_ANY
        {
            SV * temp0;
            struct in_addr ip_address; ip_address.s_addr = htonl(INADDR_ANY);
            temp0 = sv_2mortal(newSVpvn((char *)&ip_address,sizeof ip_address ));
	    constant_add_symbol(aTHX_  symbol_table, "INADDR_ANY",
				    10, SvREFCNT_inc(temp0));
        }
#endif
#ifdef INADDR_LOOPBACK
        {
            SV * temp0;
            struct in_addr ip_address; ip_address.s_addr = htonl(INADDR_LOOPBACK);
            temp0 = sv_2mortal(newSVpvn((char *)&ip_address,sizeof ip_address ));
	    constant_add_symbol(aTHX_  symbol_table, "INADDR_LOOPBACK",
				    15, SvREFCNT_inc(temp0));
        }
#endif
#ifdef INADDR_NONE
        {
            SV * temp0;
            struct in_addr ip_address; ip_address.s_addr = htonl(INADDR_NONE);
            temp0 = sv_2mortal(newSVpvn((char *)&ip_address,sizeof ip_address ));
	    constant_add_symbol(aTHX_  symbol_table, "INADDR_NONE",
				    11, SvREFCNT_inc(temp0));
        }
#endif
#ifdef INADDR_BROADCAST
        {
            SV * temp0;
            struct in_addr ip_address; ip_address.s_addr = htonl(INADDR_BROADCAST);
            temp0 = sv_2mortal(newSVpvn((char *)&ip_address,sizeof ip_address ));
	    constant_add_symbol(aTHX_  symbol_table, "INADDR_BROADCAST",
				    16, SvREFCNT_inc(temp0));
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
	sv = newSVpvf("%"SVf" is not a valid Socket macro", sv);
#else
	HV *constant_missing = get_missing_hash(aTHX);
	if (hv_exists(constant_missing, s, SvUTF8(sv) ? -(I32)len : (I32)len)) {
	    sv = newSVpvf("Your vendor has not defined Socket macro %" SVf
			  ", used", sv);
	} else {
	    sv = newSVpvf("%"SVf" is not a valid Socket macro",
			  sv);
	}
#endif
	PUSHs(sv_2mortal(sv));

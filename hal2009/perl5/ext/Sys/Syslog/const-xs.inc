BOOT:
  {
#ifdef dTHX
    dTHX;
#endif
    HV *symbol_table = get_hv("Sys::Syslog::", TRUE);
#ifndef SYMBIAN
    HV *constant_missing;
#endif

    static const struct notfound_s values_for_notfound[] =
      {
#ifdef LOG_ALERT
#else
        { "LOG_ALERT", 9 },
#endif
#ifdef LOG_CRIT
#else
        { "LOG_CRIT", 8 },
#endif
#ifdef LOG_DEBUG
#else
        { "LOG_DEBUG", 9 },
#endif
#ifdef LOG_EMERG
#else
        { "LOG_EMERG", 9 },
#endif
#ifdef LOG_ERR
#else
        { "LOG_ERR", 7 },
#endif
#ifdef LOG_INFO
#else
        { "LOG_INFO", 8 },
#endif
#ifdef LOG_NOTICE
#else
        { "LOG_NOTICE", 10 },
#endif
#ifdef LOG_WARNING
#else
        { "LOG_WARNING", 11 },
#endif
#ifdef LOG_AUTH
#else
        { "LOG_AUTH", 8 },
#endif
#ifdef LOG_AUTHPRIV
#else
        { "LOG_AUTHPRIV", 12 },
#endif
#ifdef LOG_CRON
#else
        { "LOG_CRON", 8 },
#endif
#ifdef LOG_DAEMON
#else
        { "LOG_DAEMON", 10 },
#endif
#ifdef LOG_FTP
#else
        { "LOG_FTP", 7 },
#endif
#ifdef LOG_KERN
#else
        { "LOG_KERN", 8 },
#endif
#ifdef LOG_LOCAL0
#else
        { "LOG_LOCAL0", 10 },
#endif
#ifdef LOG_LOCAL1
#else
        { "LOG_LOCAL1", 10 },
#endif
#ifdef LOG_LOCAL2
#else
        { "LOG_LOCAL2", 10 },
#endif
#ifdef LOG_LOCAL3
#else
        { "LOG_LOCAL3", 10 },
#endif
#ifdef LOG_LOCAL4
#else
        { "LOG_LOCAL4", 10 },
#endif
#ifdef LOG_LOCAL5
#else
        { "LOG_LOCAL5", 10 },
#endif
#ifdef LOG_LOCAL6
#else
        { "LOG_LOCAL6", 10 },
#endif
#ifdef LOG_LOCAL7
#else
        { "LOG_LOCAL7", 10 },
#endif
#ifdef LOG_LPR
#else
        { "LOG_LPR", 7 },
#endif
#ifdef LOG_MAIL
#else
        { "LOG_MAIL", 8 },
#endif
#ifdef LOG_NEWS
#else
        { "LOG_NEWS", 8 },
#endif
#ifdef LOG_SYSLOG
#else
        { "LOG_SYSLOG", 10 },
#endif
#ifdef LOG_USER
#else
        { "LOG_USER", 8 },
#endif
#ifdef LOG_UUCP
#else
        { "LOG_UUCP", 8 },
#endif
#ifdef LOG_CONS
#else
        { "LOG_CONS", 8 },
#endif
#ifdef LOG_PID
#else
        { "LOG_PID", 7 },
#endif
#ifdef LOG_NDELAY
#else
        { "LOG_NDELAY", 10 },
#endif
#ifdef LOG_NOWAIT
#else
        { "LOG_NOWAIT", 10 },
#endif
#ifdef LOG_ODELAY
#else
        { "LOG_ODELAY", 10 },
#endif
#ifdef LOG_PERROR
#else
        { "LOG_PERROR", 10 },
#endif
#ifdef LOG_FACMASK
#else
        { "LOG_FACMASK", 11 },
#endif
        { NULL, 0 } };
	const struct notfound_s *value_for_notfound = values_for_notfound;

    static const struct iv_s values_for_iv[] =
      {
#ifdef LOG_ALERT
        { "LOG_ALERT", 9, LOG_ALERT },
#endif
#ifdef LOG_CRIT
        { "LOG_CRIT", 8, LOG_CRIT },
#endif
#ifdef LOG_DEBUG
        { "LOG_DEBUG", 9, LOG_DEBUG },
#endif
#ifdef LOG_EMERG
        { "LOG_EMERG", 9, LOG_EMERG },
#endif
#ifdef LOG_ERR
        { "LOG_ERR", 7, LOG_ERR },
#endif
#ifdef LOG_INFO
        { "LOG_INFO", 8, LOG_INFO },
#endif
#ifdef LOG_NOTICE
        { "LOG_NOTICE", 10, LOG_NOTICE },
#endif
#ifdef LOG_WARNING
        { "LOG_WARNING", 11, LOG_WARNING },
#endif
#ifdef LOG_AUTH
        { "LOG_AUTH", 8, LOG_AUTH },
#endif
#ifdef LOG_AUTHPRIV
        { "LOG_AUTHPRIV", 12, LOG_AUTHPRIV },
#endif
#ifdef LOG_CRON
        { "LOG_CRON", 8, LOG_CRON },
#endif
#ifdef LOG_DAEMON
        { "LOG_DAEMON", 10, LOG_DAEMON },
#endif
#ifdef LOG_FTP
        { "LOG_FTP", 7, LOG_FTP },
#endif
#ifdef LOG_KERN
        { "LOG_KERN", 8, LOG_KERN },
#endif
#ifdef LOG_LOCAL0
        { "LOG_LOCAL0", 10, LOG_LOCAL0 },
#endif
#ifdef LOG_LOCAL1
        { "LOG_LOCAL1", 10, LOG_LOCAL1 },
#endif
#ifdef LOG_LOCAL2
        { "LOG_LOCAL2", 10, LOG_LOCAL2 },
#endif
#ifdef LOG_LOCAL3
        { "LOG_LOCAL3", 10, LOG_LOCAL3 },
#endif
#ifdef LOG_LOCAL4
        { "LOG_LOCAL4", 10, LOG_LOCAL4 },
#endif
#ifdef LOG_LOCAL5
        { "LOG_LOCAL5", 10, LOG_LOCAL5 },
#endif
#ifdef LOG_LOCAL6
        { "LOG_LOCAL6", 10, LOG_LOCAL6 },
#endif
#ifdef LOG_LOCAL7
        { "LOG_LOCAL7", 10, LOG_LOCAL7 },
#endif
#ifdef LOG_LPR
        { "LOG_LPR", 7, LOG_LPR },
#endif
#ifdef LOG_MAIL
        { "LOG_MAIL", 8, LOG_MAIL },
#endif
#ifdef LOG_NEWS
        { "LOG_NEWS", 8, LOG_NEWS },
#endif
#ifdef LOG_SYSLOG
        { "LOG_SYSLOG", 10, LOG_SYSLOG },
#endif
#ifdef LOG_USER
        { "LOG_USER", 8, LOG_USER },
#endif
#ifdef LOG_UUCP
        { "LOG_UUCP", 8, LOG_UUCP },
#endif
#ifdef LOG_INSTALL
        { "LOG_INSTALL", 11, LOG_INSTALL },
#endif
#ifdef LOG_LAUNCHD
        { "LOG_LAUNCHD", 11, LOG_LAUNCHD },
#endif
#ifdef LOG_NETINFO
        { "LOG_NETINFO", 11, LOG_NETINFO },
#endif
#ifdef LOG_RAS
        { "LOG_RAS", 7, LOG_RAS },
#endif
#ifdef LOG_REMOTEAUTH
        { "LOG_REMOTEAUTH", 14, LOG_REMOTEAUTH },
#endif
#ifdef LOG_CONSOLE
        { "LOG_CONSOLE", 11, LOG_CONSOLE },
#endif
#ifdef LOG_NTP
        { "LOG_NTP", 7, LOG_NTP },
#endif
#ifdef LOG_SECURITY
        { "LOG_SECURITY", 12, LOG_SECURITY },
#endif
#ifdef LOG_AUDIT
        { "LOG_AUDIT", 9, LOG_AUDIT },
#endif
#ifdef LOG_LFMT
        { "LOG_LFMT", 8, LOG_LFMT },
#endif
#ifdef LOG_CONS
        { "LOG_CONS", 8, LOG_CONS },
#endif
#ifdef LOG_PID
        { "LOG_PID", 7, LOG_PID },
#endif
#ifdef LOG_NDELAY
        { "LOG_NDELAY", 10, LOG_NDELAY },
#endif
#ifdef LOG_NOWAIT
        { "LOG_NOWAIT", 10, LOG_NOWAIT },
#endif
#ifdef LOG_ODELAY
        { "LOG_ODELAY", 10, LOG_ODELAY },
#endif
#ifdef LOG_PERROR
        { "LOG_PERROR", 10, LOG_PERROR },
#endif
#ifdef LOG_FACMASK
        { "LOG_FACMASK", 11, LOG_FACMASK },
#endif
#ifdef LOG_PRIMASK
        { "LOG_PRIMASK", 11, LOG_PRIMASK },
#endif
#ifdef LOG_NFACILITIES
        { "LOG_NFACILITIES", 15, LOG_NFACILITIES },
#endif
#ifdef LOG_INSTALL
        /* This is the default value: */
#else
        { "LOG_INSTALL", 11, LOG_USER },
#endif
#ifdef LOG_LAUNCHD
        /* This is the default value: */
#else
        { "LOG_LAUNCHD", 11, LOG_DAEMON },
#endif
#ifdef LOG_NETINFO
        /* This is the default value: */
#else
        { "LOG_NETINFO", 11, LOG_DAEMON },
#endif
#ifdef LOG_RAS
        /* This is the default value: */
#else
        { "LOG_RAS", 7, LOG_AUTH },
#endif
#ifdef LOG_REMOTEAUTH
        /* This is the default value: */
#else
        { "LOG_REMOTEAUTH", 14, LOG_AUTH },
#endif
#ifdef LOG_CONSOLE
        /* This is the default value: */
#else
        { "LOG_CONSOLE", 11, LOG_USER },
#endif
#ifdef LOG_NTP
        /* This is the default value: */
#else
        { "LOG_NTP", 7, LOG_DAEMON },
#endif
#ifdef LOG_SECURITY
        /* This is the default value: */
#else
        { "LOG_SECURITY", 12, LOG_AUTH },
#endif
#ifdef LOG_AUDIT
        /* This is the default value: */
#else
        { "LOG_AUDIT", 9, LOG_AUTH },
#endif
#ifdef LOG_LFMT
        /* This is the default value: */
#else
        { "LOG_LFMT", 8, LOG_USER },
#endif
#ifdef LOG_PRIMASK
        /* This is the default value: */
#else
        { "LOG_PRIMASK", 11, 7 },
#endif
#ifdef LOG_NFACILITIES
        /* This is the default value: */
#else
        { "LOG_NFACILITIES", 15, 30 },
#endif
        { NULL, 0, 0 } };
	const struct iv_s *value_for_iv = values_for_iv;

    static const struct pv_s values_for_pv[] =
      {
#ifdef _PATH_LOG
        { "_PATH_LOG", 9, _PATH_LOG },
#endif
#ifdef _PATH_LOG
        /* This is the default value: */
#else
        { "_PATH_LOG", 9, "/dev/log" },
#endif
        { NULL, 0, 0 } };
	const struct pv_s *value_for_pv = values_for_pv;
#ifndef SYMBIAN
	constant_missing = get_missing_hash(aTHX);
#endif
        while (value_for_iv->name) {
	    constant_add_symbol(aTHX_  symbol_table, value_for_iv->name,
				value_for_iv->namelen, newSViv(value_for_iv->value));
	    ++value_for_iv;
	}
        while (value_for_pv->name) {
	    constant_add_symbol(aTHX_  symbol_table, value_for_pv->name,
				value_for_pv->namelen, newSVpv(value_for_pv->value, 0));
	    ++value_for_pv;
	}
	while (value_for_notfound->name) {

	    /* Need to add prototypes, else parsing will vary by platform.  */
	    SV **sv = hv_fetch(symbol_table, value_for_notfound->name,
			       value_for_notfound->namelen, TRUE);
	    if (!sv) {
		Perl_croak(aTHX_ 
			   "Couldn't add key '%s' to %%Sys::Syslog::",
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
	sv = newSVpvf("%"SVf" is not a valid Sys::Syslog macro", sv);
#else
	HV *constant_missing = get_missing_hash(aTHX);
	if (hv_exists(constant_missing, s, SvUTF8(sv) ? -(I32)len : (I32)len)) {
	    sv = newSVpvf("Your vendor has not defined Sys::Syslog macro %" SVf
			  ", used", sv);
	} else {
	    sv = newSVpvf("%"SVf" is not a valid Sys::Syslog macro",
			  sv);
	}
#endif
	PUSHs(sv_2mortal(sv));

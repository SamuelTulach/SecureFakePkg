@rem Common file shared between external.bat and external-amd64.bat.  Responsible for
@rem fetching external components into the root\.. buildbot directories.

cd ..
@rem XXX: If you need to force the buildbots to start from a fresh environment, uncomment
@rem the following, check it in, then check it out, comment it out, then check it back in.
@rem if exist bzip2-1.0.5 rd /s/q bzip2-1.0.5
@rem if exist tcltk rd /s/q tcltk
@rem if exist tcltk64 rd /s/q tcltk64
@rem if exist tcl8.4.12 rd /s/q tcl8.4.12
@rem if exist tcl8.4.16 rd /s/q tcl8.4.16
@rem if exist tcl-8.4.18.1 rd /s/q tcl-8.4.18.1
@rem if exist tk8.4.12 rd /s/q tk8.4.12
@rem if exist tk8.4.16 rd /s/q tk8.4.16
@rem if exist tk-8.4.18.1 rd /s/q tk-8.4.18.1
@rem if exist db-4.4.20 rd /s/q db-4.4.20
@rem if exist db-4.7.25.0 rd /s/q db-4.7.25.0
@rem if exist openssl-0.9.8l rd /s/q openssl-0.9.8l
@rem if exist sqlite-3.6.21 rd /s/q sqlite-3.6.21    

@rem bzip
if not exist bzip2-1.0.5 (
   rd /s/q bzip2-1.0.3
  svn export http://svn.python.org/projects/external/bzip2-1.0.5
)

@rem Berkeley DB
if exist db-4.4.20 rd /s/q db-4.4.20
if not exist db-4.7.25.0 svn export http://svn.python.org/projects/external/db-4.7.25.0

@rem OpenSSL
if not exist openssl-0.9.8l svn export http://svn.python.org/projects/external/openssl-0.9.8l

@rem tcl/tk
if not exist tcl-8.5.2.1 (
   rd /s/q tcltk tcltk64
   svn export http://svn.python.org/projects/external/tcl-8.5.2.1
)
if not exist tk-8.5.2.0 svn export http://svn.python.org/projects/external/tk-8.5.2.0

@rem sqlite3
if not exist sqlite-3.6.21 (
  rd /s/q sqlite-source-3.3.4
  svn export http://svn.python.org/projects/external/sqlite-3.6.21
)

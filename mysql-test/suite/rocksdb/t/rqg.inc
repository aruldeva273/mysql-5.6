#
# Random Query Generator tests
#
# Arguments needed to be set by the test when including this one:
#  $TESTDIR : name of sub-directory in conf containing the data/grammar files
#  $GRAMMAR_FILES: space separated list of grammar files
#  $DATA_FILE: name of the data file
#

let RQG_BASE = $MYSQL_TEST_DIR/../rqg/rqg/common/mariadb-patches;
let MYSQL_SOCKET = `SELECT @@SOCKET`;
let GRAMMAR_FILES = $GRAMMAR_FILES;
let DATA_FILE = $DATA_FILE;
let TESTDIR = $TESTDIR;
let $TESTDB = rqg_$TESTDIR;
let TESTDB = $TESTDB;

--eval CREATE DATABASE IF NOT EXISTS $TESTDB

--perl

foreach $grammar_file (split(/ /, $ENV{'GRAMMAR_FILES'})) {

  # Errors from the gentest.pl file will be captured in the results file
  my $cmd = "RQG_HOME=$ENV{'RQG_BASE'} perl $ENV{'RQG_BASE'}/gentest.pl " .
      "--dsn=dbi:mysql:host=:port=:user=root:database=$ENV{'TESTDB'}" .
      ":mysql_socket=$ENV{'MYSQL_SOCKET'} " .
      "--gendata=$ENV{'RQG_BASE'}/conf/$ENV{'TESTDIR'}/$ENV{'DATA_FILE'} " .
      "--grammar=$ENV{'RQG_BASE'}/conf/$ENV{'TESTDIR'}/$grammar_file " .
      "--threads=5 --queries=1000 --duration=600 --sqltrace 2>&1 >> " .
      "$ENV{'MYSQLTEST_VARDIR'}/tmp/$ENV{'TESTDB'}.log";

  print "Running test with grammar file $grammar_file\n";
  system($cmd);
  if ($? != 0) {
    print ("Failure running test! Command executed: $cmd\n");
  }
}

EOF

--eval DROP DATABASE $TESTDB
--remove_file $MYSQLTEST_VARDIR/tmp/$TESTDB.log

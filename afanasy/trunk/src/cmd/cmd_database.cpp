#include "cmd_database.h"

#include "../libafsql/qdbconnection.h"
#include "../libafsql/dbjob.h"

#define AFOUTPUT
#undef AFOUTPUT
#include "../include/macrooutput.h"

CmdDBDrivers::CmdDBDrivers()
{
   setCmd("db_drivers");
   setInfo("Get list of available database Qt drivers.");
}
CmdDBDrivers::~CmdDBDrivers(){}
bool CmdDBDrivers::processArguments( int argc, char** argv, af::Msg &msg)
{
   QStringList driverNames = QSqlDatabase::drivers();
   printf("\nAvailable drivers: (%d)\n", driverNames.size());
   for( int i = 0; i < driverNames.size(); i++) printf("%d: \"%s\"\n", i, driverNames[i].toUtf8().data());
   return true;
}

CmdDBCheck::CmdDBCheck()
{
   setCmd("db_check");
   setInfo("Check database connection.");
}
CmdDBCheck::~CmdDBCheck(){}
bool CmdDBCheck::processArguments( int argc, char** argv, af::Msg &msg)
{
      afsql::DBConnection DB( "afanasy.cmd.Check");
      if( DB.isWorking()) printf("\nDatabase connection is working.\n");
      else                printf("\nDatabase connection is NOT WORKING !\n");
      return true;
}

CmdDBResetUsers::CmdDBResetUsers()
{
   setCmd("db_resetusers");
   setInfo("Reset (create) users table.");
}
CmdDBResetUsers::~CmdDBResetUsers(){}
bool CmdDBResetUsers::processArguments( int argc, char** argv, af::Msg &msg)
{
   afsql::DBConnection DB( "afanasy.cmd.ResetUsers");
   DB.DBOpen();
   DB.ResetUsers();
   DB.DBClose();
   return true;
}

CmdDBResetRenders::CmdDBResetRenders()
{
   setCmd("db_resetrenders");
   setInfo("Reset (create) renders table.");
}
CmdDBResetRenders::~CmdDBResetRenders(){}
bool CmdDBResetRenders::processArguments( int argc, char** argv, af::Msg &msg)
{
   afsql::DBConnection DB( "afanasy.cmd.ResetRenders");
   DB.DBOpen();
   DB.ResetRenders();
   DB.DBClose();
   return true;
}

CmdDBResetJobs::CmdDBResetJobs()
{
   setCmd("db_resetjobs");
   setInfo("Reset (create) jobs table.");
}
CmdDBResetJobs::~CmdDBResetJobs(){}
bool CmdDBResetJobs::processArguments( int argc, char** argv, af::Msg &msg)
{
   afsql::DBConnection DB( "afanasy.cmd.ResetOnline");
   DB.DBOpen();
   DB.ResetJobs();
   DB.DBClose();
   return true;
}

CmdDBResetStat::CmdDBResetStat()
{
   setCmd("db_resetstat");
   setInfo("Reset (create) statistics table.");
}
CmdDBResetStat::~CmdDBResetStat(){}
bool CmdDBResetStat::processArguments( int argc, char** argv, af::Msg &msg)
{
   afsql::DBConnection DB( "afanasy.cmd.ResetArchive");
   DB.DBOpen();
   DB.ResetStat();
   DB.DBClose();
   return true;
}

CmdDBResetAll::CmdDBResetAll()
{
   setCmd("db_resetall");
   setInfo("Reset (create) all tables.");
}
CmdDBResetAll::~CmdDBResetAll(){}
bool CmdDBResetAll::processArguments( int argc, char** argv, af::Msg &msg)
{
   afsql::DBConnection DB( "afanasy.cmd.ResetAll");
   DB.DBOpen();
   DB.ResetAll();
   DB.DBClose();
   return true;
}

CmdDBJobsList::CmdDBJobsList()
{
   setCmd("db_jobsls");
   setInfo("List database jobs.");
}
CmdDBJobsList::~CmdDBJobsList(){}
bool CmdDBJobsList::processArguments( int argc, char** argv, af::Msg &msg)
{
   afsql::DBConnection DB( "afanasy.cmd.JobsList");
   DB.DBOpen();

   std::list<int> jids;
   DB.getJobsIds( jids);
   for( std::list<int>::const_iterator it = jids.begin(); it != jids.end(); it++)
   {
      afsql::DBJob job( *it);
      DB.getItem( &job);
      job.stdOut( false);
   }

   DB.DBClose();
   return true;
}

CmdDBJobsClean::CmdDBJobsClean()
{
   setCmd("db_jobsclean");
   setInfo("Clean invalid database jobs.");
}
CmdDBJobsClean::~CmdDBJobsClean(){}
bool CmdDBJobsClean::processArguments( int argc, char** argv, af::Msg &msg)
{
   afsql::DBConnection DB( "afanasy.cmd.JobsClean");
   DB.DBOpen();

   std::list<int> jids;
   DB.getJobsIds( jids);
   for( std::list<int>::const_iterator it = jids.begin(); it != jids.end(); it++)
   {
      afsql::DBJob job( *it);
      if( DB.getItem( &job) == false)
      {
         job.stdOut( false);
         QStringList queries;
         job.dbDelete( &queries);
         DB.execute( queries);
      }
   }

   DB.DBClose();
   return true;
}

CmdDBSysJobDel::CmdDBSysJobDel()
{
   setCmd("db_sysjobdel");
   setInfo("Delete system job.");
}
CmdDBSysJobDel::~CmdDBSysJobDel(){}
bool CmdDBSysJobDel::processArguments( int argc, char** argv, af::Msg &msg)
{
   afsql::DBConnection DB( "afanasy.cmd.SysJobDel");
   DB.DBOpen();

   std::list<int> jids;
   DB.getJobsIds( jids);

   bool has_system_job = false;
   for( std::list<int>::const_iterator it = jids.begin(); it != jids.end(); it++)
   {
      if((*it) == AFJOB::SYSJOB_ID )
      {
         has_system_job = true;
         break;
      }
   }

   if( has_system_job )
   {
      afsql::DBJob job( AFJOB::SYSJOB_ID);
      DB.getItem( &job);
      job.stdOut( false);
      QStringList queries;
      job.dbDelete( &queries);
      DB.execute( queries);
   }
   else
   {
      printf("No job with system ID=%d founded.\n", AFJOB::SYSJOB_ID);
      printf("System job not founded.\n");
   }

   DB.DBClose();
   return true;
}

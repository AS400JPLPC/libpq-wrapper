//
// A very  C++ wrapper for (subset of) libpq C API
//
// projet 2018-03-15  (C) 2011   Copyright 2018 SOLEIL <laroche.jeanpierre@gmail.com>
//
// THANK YOU   MERCI BEAUCOUP
//
// le site quebecois 			https://h-deb.clg.qc.ca/		pour son travaille de divulgation
//
// https://stackoverflow.com	une mine d'or pour comprendre avec des examples
//
// https://docs.postgresql.fr/10/	pour tous les efforts pour la traduction 
/*
 *
 * M. laroche jean Pierre  12-10-1951    laroche.jeanpierre@gmail.com
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */


#ifndef LIBPQWRP_HPP_INCLUDED
#define LIBPQWRP_HPP_INCLUDED


#include <iostream>
#include <ostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <libpq-fe.h>
#include <cstdlib>

#ifndef   DeLiMiTaTioN
#define   DeLiMiTaTioN		'~'		///  caractère de délimitation
#endif
namespace libpqsql
{


///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
///				fonction sql de libpq 
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
static bool fetch0 ;
static bool update0 ;
static bool select0 ;
static bool delete0 ;
static bool insert0 ;
static bool fetchEOF ;

struct	inforowcol
{
	int			rows;			/// initialise par les requetes
	int			cols;			/// initialise par les requetes

	int			Xrow;			/// variable de traitement
	int			Xcol;			/// variable de traitement
} records ;




///****************************************************************************
/// FONCTIONS util     --------------------------------------------------------
///****************************************************************************
/// retrieve  char stringstream

std::istream& operator>>(std::istream& is ,char* t)
	{
		std::string _var_ ;
		is >> _var_ ;
		for (size_t i = 0; i < _var_.size(); ++i)
			{
				if (_var_[i] == DeLiMiTaTioN)
				{
				_var_[i] = ' ';
				}
			}
		 
		 t = (char*)_var_.c_str();
		 _var_ = t; /// dumy
		return is;
	}
/// Double to char -----------------------------
char* DoubleToChar(double _X_ ,unsigned _precision_ = 0)
{

	char * X_value   = (char*) malloc(36  * sizeof(char*));   sprintf(X_value,"%c",'\0');
	char * X_cmd     = (char*) malloc(36  * sizeof(char*));
	char sign = _X_ >= 0 ? '+' : '-';

	if ( sign == '-')  // signed -
	{
		 sprintf(X_cmd,"%s%c%d.0%d%c","%c",'%',0,_precision_,'f');
		 snprintf(X_value + strlen(X_value), 16, X_cmd , sign, fabs(_X_));
	}
	else // not signed +
	{
		sprintf(X_cmd,"%c%d.0%d%c",'%',0,_precision_,'f');
		snprintf(X_value + strlen(X_value), 16, X_cmd , fabs(_X_));
	}

	return (char*) X_value;

}
/// a function to make your alphanumeric switch
constexpr unsigned long long int HashStringToInt(const char *str, unsigned long long int hash = 0)
{
    return (*str == 0) ? hash : 101 * HashStringToInt(str + 1) + *str;
}


#define NAMEOF(variable) ((void)variable, #variable)

///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
///			 prepare sql  formatage avec template variadique 
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

class QueryPrint            /// gestion parametre sql ex: select 
{
	private:
	static constexpr char FORMAT_SPECIFIER = '?';
	
	std::stringstream sql;
	bool first = true ;
	std::string fin_stage ;
	
	protected :
	
	unsigned count_format_specifiers(std::string const& format);				/// count the number of parameters in the format

	void prepare(std::string const& format);									/// format without parameters causes an error

	void resultctrl(PGresult* res);												/// error not parameter query result

	public:

	template <typename Head, typename... Args>
	std::string  prepare(std::string const& format, Head&& head, Args&&... args);	/// formatting the query with parameters

	std::stringstream result(PGresult* res);										/// out buffer;
};

///-------------------------------------------------------------------------------------------------------

unsigned  QueryPrint::count_format_specifiers(std::string const& format)
{
  unsigned count = 0;
  for (size_t i = 0; i < format.size(); ++i)
  {
    if (format[i] == FORMAT_SPECIFIER)
    {
      if ((i != format.size() - 1) && (format[i+1] == FORMAT_SPECIFIER))
        ++i; // skip next % as well, it was consumed by this one
      else
      ++count;
    }
  }
  
  return count;
}

///-------------------------------------------------------------------------------------------------------

void QueryPrint::prepare(std::string const& format)
{
  if (count_format_specifiers(format) != 0)
    throw std::invalid_argument("number of arguments does not match the format string");
}

///-------------------------------------------------------------------------------------------------------

template <typename Head, typename... Args>
std::string QueryPrint::prepare(std::string const& format, Head&& head, Args&&... args)
{

	if (count_format_specifiers(format) > 0 && first == true)
	{
		
		fin_stage = format ;  first = false ;  sql.str("");
		auto last_format_pos = format.find_last_of(FORMAT_SPECIFIER);
		fin_stage  =fin_stage.erase(0,  last_format_pos +1);
		 
	}

  if (count_format_specifiers(format) != sizeof...(Args) + 1)
    throw std::invalid_argument("number of arguments does not match the format string");
  // TODO: take care of escaped format specifiers
  sql<<"";
  auto first_format_pos = format.find_first_of(FORMAT_SPECIFIER);
  sql << format.substr(0, first_format_pos);  
  sql << head;
  if ( count_format_specifiers(format) == 1 ) { sql<<fin_stage; first =true  ;} 	/// returns to initial position
  prepare(format.substr(first_format_pos+1), std::forward<Args>(args)...);
  return sql.str();
 
}




///-------------------------------------------------------------------------------------------------------


std::stringstream  QueryPrint::result(PGresult* res)
{	

	char* tptr;
	std::stringstream sql;


    
	for ( int i= 0 ; i <records.cols ; i ++ )
	{
		tptr = PQgetvalue(res, records.Xrow, i);
		
		if ( i == 0 )
		{
			for (size_t i = 0; i < strlen(tptr); ++i)
			{
				if (tptr[i] == ' ')
				{
				tptr[i] =DeLiMiTaTioN;
				}
			}
			sql << tptr;
		}
		else
		{
			for (size_t i = 0; i < strlen(tptr); ++i)
			{
				if (tptr[i] == ' ')
				{
				tptr[i] = DeLiMiTaTioN ;
				}
			}
			sql <<' '<< tptr;
		}

	}
	return  sql;
}

///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$





///-------------------------------------------------------------------------------------------------------

PGconn* connectDB(std::string info)											/// connect to the database
{
    PGconn* conn;
	conn= PQconnectdb(info.c_str());  
    if (PQstatus(conn) != CONNECTION_OK)
    {  
        char const* msg = PQerrorMessage(conn);
        throw std::runtime_error(std::string(msg));
    }
    return conn;
}


///-------------------------------------------------------------------------------------------------------

bool qexec(PGconn* conn, std::string sql)									/// PQexec 
{
     PGresult* res = PQgetResult(conn);
     res = PQexec(conn, sql.c_str());

    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK)
    {	PQclear(res); 
        char const* msg = PQresultErrorMessage(res);
        throw std::runtime_error(std::string(msg));
    }
    
	PQclear(res);
    return true ;
}

///-------------------------------------------------------------------------------------------------------

PGresult* query(PGconn* conn, std::string sql, bool binary = true)	/// PQquery
{
    int const fmt = binary ? 1 : 0;
 
    PGresult* res = PQgetResult(conn);
    res = PQexecParams(conn, sql.c_str(), 0, 0, 0, 0, 0, fmt);
    
	if ( PGRES_BAD_RESPONSE == PQresultStatus(res) || \
		PGRES_NONFATAL_ERROR == PQresultStatus(res) || \
		PGRES_FATAL_ERROR ==PQresultStatus(res) )
		{
			PQclear(res);
			char const* msg =PQresultErrorMessage(res) ;      
			throw std::runtime_error(std::string(msg));
		}

	std::string operation = PQcmdStatus(res); 
	if ( operation =="FETCH 0"  ) fetch0  = true; else fetch0  =false ;   
    if ( operation =="UPDATE 0" ) update0 = true; else update0 =false ;  
    if ( operation =="SELECT 0" ) select0 = true; else select0 =false ;
    if ( operation =="DELETE 0" ) delete0 = true; else delete0 =false ;
    if ( operation =="INSERT 0" ) insert0 = true; else insert0 =false ;

    return res;
}

///-------------------------------------------------------------------------------------------------------

char* fetch(PGresult* res, int row, int column)						/// get the value of the row and the column
{
	if (row < 0)
		throw std::invalid_argument("row position is negative");

	if (column < 0 || column > PQnfields(res) )
		throw std::invalid_argument("column position is negative");

	return PQgetvalue(res, row, column);
}

///-------------------------------------------------------------------------------------------------------

double fetchDbl(PGresult* res, int row, int column)						/// get the value of the row and the column
{
	if (row < 0)
		throw std::invalid_argument("row position is negative");

	if (column < 0 || column > PQnfields(res) )
		throw std::invalid_argument("column position is negative");

	return atof(PQgetvalue(res, row, column));
	throw std::invalid_argument("fetchDbl invalide argument");
}

///-------------------------------------------------------------------------------------------------------

int fetchInt(PGresult* res, int row, int column)						/// get the value of the row and the column
{
	if (row < 0)
		throw std::invalid_argument("row position is negative");

	if (column < 0 || column > PQnfields(res) )
		throw std::invalid_argument("column position is negative");

	return atoi(PQgetvalue(res, row, column));
		throw std::invalid_argument("fetchInt invalide argument");
}

///-------------------------------------------------------------------------------------------------------

int nfield(PGresult* res, std::string field)						/// column of the field
{
	int col =  PQfnumber(res, field.c_str());

	if ( col<0 )
	{
		std::string msg="bad name field :"  + field  ;      
		throw std::runtime_error(msg);
	}
	return col;
}

///-------------------------------------------------------------------------------------------------------

const char* cfield(PGresult* res,int nfield)						/// name of the field 
{
	if (nfield < 0 || nfield > PQnfields(res) )
	{
		std::string msg="bad n° field :"  + std::to_string(nfield) ;      
		throw std::runtime_error(msg);
    }
	return  PQfname(res,nfield);

}

///-------------------------------------------------------------------------------------------------------

int countfield(PGresult* res)										/// number of columns
{
	return  PQnfields(res);
}

///-------------------------------------------------------------------------------------------------------

int countrow(PGresult* res)											/// number of rows
{
	return PQntuples(res);	
}

///-------------------------------------------------------------------------------------------------------

bool is_Table(PGconn* conn , const char* table)						/// if exist table of the database ->fasle = does not exist
{
	static std::string const exist_Table = "SELECT count(*)  FROM pg_tables  WHERE  schemaname ='public' AND    tablename = '?'";
	QueryPrint prep;
 
	const std::string trt = prep.prepare(exist_Table ,table);

	PGresult* res;

	res = query(conn,trt,false);									/// false for return value count 

	return atoi(PQgetvalue(res, 0, 0));

}

///-------------------------------------------------------------------------------------------------------

void closeDB(PGconn* conn)											/// close to the database 
{
	PGresult* res = PQgetResult(conn);
	PQclear(res);
	qexec(conn,"ROLLBACK");
	PQfinish(conn);
}

///-------------------------------------------------------------------------------------------------------

void begin(PGconn* conn )
{
	query(conn,"BEGIN");		/// full
}


///-------------------------------------------------------------------------------------------------------

void commit(PGconn* conn )
{
	query(conn,"COMMIT");		/// full
}

///-------------------------------------------------------------------------------------------------------

void end(PGconn* conn )
{
	query(conn,"END");		/// full
}

///-------------------------------------------------------------------------------------------------------

void rollback(PGconn* conn )
{
	query(conn,"ROLLBACK");		/// full
}

///-------------------------------------------------------------------------------------------------------

void savpoint(PGconn* conn )
{
	query(conn,"SAVEPOINT full_savepoint");					/// point de sauvegarde  
}

///-------------------------------------------------------------------------------------------------------

void savpointRollback(PGconn* conn )
{
	query(conn,"ROLLBACK TO SAVEPOINT full_savepoint");		/// roolback sur savepoint
}

///-------------------------------------------------------------------------------------------------------

void savpointRelease(PGconn* conn )
{
	query(conn,"RELEASE SAVEPOINT full_savepoint");			/// delete savepoint 
}


///-------------------------------------------------------------------------------------------------------
///-------------- process  complex
///-------------------------------------------------------------------------------------------------------



///-------------------------------------------------------------------------------------------------------

PGresult * fetchAll(PGconn* conn, std::string sql, std::string cursor = "mycursor")		/// fetch ALL include requete 
{
     PGresult* res = PQgetResult(conn);
	 std::string  ordreSQL;
	 
     ordreSQL =  "DECLARE " + cursor + " CURSOR FOR "+ sql;

	res = PQexec(conn, ordreSQL.c_str());
    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK)
    {
		PQclear(res);
		char const* msg = PQresultErrorMessage(res);
		throw std::runtime_error(std::string(msg));
	}
	PQclear(res);

	ordreSQL = "FETCH ALL in " + cursor ;	/// lecture full records  ??? memory 
     
 	res = PQexec(conn, ordreSQL.c_str());
 //    std::cout<<"code status  :"<<PQresultStatus(res)<<" OK:"<<PGRES_TUPLES_OK<<"  value result  "<<PQcmdStatus(res)<<std::endl;
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK)
    {
		PQclear(res);
		char const* msg = PQresultErrorMessage(res);
		throw std::runtime_error(std::string(msg));
    }

	std::string operation = PQcmdStatus(res);  
	if ( operation =="FETCH 0"  )
	{
		fetchEOF  =true ;   				/// end of row or without row
		ordreSQL = "close" + cursor ;		/// close cursor
		PQexec(conn, ordreSQL.c_str());
		PQclear(res);
	}
	else fetchEOF  =false ;					/// no end of row


	records.rows	= PQntuples(res);
	records.cols	= PQnfields(res);
	records.Xrow	= 0;
	records.Xcol	= 0;
	std::cout<<records.rows<<" - " << records.cols << std::endl;
	return res ;
}

///-------------------------------------------------------------------------------------------------------

PGresult * opensql(PGconn* conn, std::string sql, std::string cursor = "mycursor")	///  query for fetchsql record / record 
{
	PGresult* res = PQgetResult(conn);
	std::string  ordreSQL;
	 
    ordreSQL =  "DECLARE " + cursor + " CURSOR FOR "+ sql;

     res = PQexec(conn, ordreSQL.c_str());

	if (!res || PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		PQclear(res);
		char const* msg = PQresultErrorMessage(res);
		throw std::runtime_error(std::string(msg));
	}
     
	std::string operation = PQcmdStatus(res); 
	if ( operation =="FETCH 0"  )
	{
		fetchEOF  =true ;					/// end of row or without row
		ordreSQL = "close" + cursor ;		/// close cursor
		PQexec(conn, ordreSQL.c_str());
		PQclear(res);
	}
	else
	{
		fetchEOF  =false ;					/// no end of row
		PQclear(res);
		ordreSQL = "FETCH FIRST in " + cursor ;	/// read a line record only
		res = PQexec(conn, ordreSQL.c_str());
	}
	records.rows	= PQntuples(res);
	records.cols	= PQnfields(res);
	records.Xrow	= 0;
	records.Xcol	= 0;

	return res;
}

///-------------------------------------------------------------------------------------------------------

PGresult * fechsql(PGconn* conn, std::string cursor = "mycursor")			/// fetch record use openSQL
{
     PGresult* res = PQgetResult(conn);
	 std::string  ordreSQL;

     ordreSQL = "FETCH NEXT in " + cursor ;	/// read a line record only

	res = PQexec(conn, ordreSQL.c_str());
 	std::string operation = PQcmdStatus(res);   
	if ( operation =="FETCH 0"  )
	{
		fetchEOF  =true ;					/// end of row or without row
		ordreSQL = "close" + cursor ;		/// close cursor
		PQexec(conn, ordreSQL.c_str());
		PQclear(res);
		
	}
	else fetchEOF  =false ;					/// no end of row

	records.rows	= PQntuples(res);
	records.cols	= PQnfields(res);
	records.Xrow	= 0;
	records.Xcol	= 0; 
    return res;
}


//********************************************************

///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
///				Fin des fonctions sql
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


} // namespace funcPQSQL

using namespace libpqsql ;

#endif // LIBPQWRP_HPP_INCLUDED

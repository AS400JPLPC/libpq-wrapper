//
// A very  C++ wrapper for (subset of) libpq C API
//
// projet 2018-03-15  (C) 2011   Copyright 2018  <laroche.jeanpierre@gmail.com>
//
// THANK YOU   MERCI BEAUCOUP
//
// le site quebecois 			https://h-deb.clg.qc.ca/		pour son travaille de divulgation
//
// https://stackoverflow.com	une mine d'or pour comprendre avec des examples
//
// https://docs.postgresql.fr/	pour tous les efforts pour la traduction 
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
#ifndef LIBPQWRP_H_INCLUDED
#define LIBPQWRP_H_INCLUDED

#include <string.h>

#include <cstdio>
#include <cstdlib>
#include <string>

#include <iostream>
#include <ostream>
#include <sstream>
#include <iomanip>

#include <stdexcept>
#include <libpq-fe.h>
#include <typeinfo>     // name

namespace libpqwrp
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




/// a function to make your alphanumeric switch
constexpr unsigned long long int HashStringToInt(const char *str, unsigned long long int hash = 0)
{
    return (*str == 0) ? hash : 101 * HashStringToInt(str + 1) + *str;
}


#define NAMEOF(variable) ((void)variable, #variable)

#ifndef   DeLiMiTaTioN
#define   DeLiMiTaTioN		'~'		///  caractère de délimitation multibuffer > stringstream
#endif



///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
///			 prepare sql  formatage avec template variadique  etc........
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$



class libPQwrp																		/// gestion parametre sql 
{
	private:

	PGconn* conn;

	
	static constexpr char FORMAT_SPECIFIER = '?';
	
	std::stringstream sqlstream;
	
	bool first = true ;
	
	std::string fin_stage ;

	bool clean ;																/// contrôle  PGresult 


	protected :
	
	unsigned count_format_specifiers(std::string const& format);				/// count the number of parameters in the format

	void prepare(std::string const& format);									/// format without parameters causes an error




	public:
	

	int			rows;			/// initialise par les requetes
	int			cols;			/// initialise par les requetes

	int			rown;			/// variable de traitement
	int			coln;			/// variable de traitement

	
	PGresult* res;

	libPQwrp(){
	fetch0  =false ;
	update0 =false ;
	select0 =false ;
	delete0 =false ;
	insert0 =false ;
	fetchEOF =false ;

		};

	~libPQwrp(){};


	void connectDB(std::string info);												/// connect to the database
	
	void qexec( std::string sql);													/// PQexec

	void query(std::string sql, bool binary = true );								/// PQquery

	char* fetch(int row, int column);												/// get the value of the row and the column
	
	double fetchDbl(int row, int column);											/// get the value of the row and the column

	int fetchInt(int row, int column);												/// get the value of the row and the column

	int nfield(std::string field);													/// number column of the field

	const char* cfield(int nfield);													/// name of the field

	int countfield();																/// number of columns

	int countrow();																	/// number of rows

	bool is_Table(const char* table);												/// if exist table of the database

	void closeDB();																	/// close to the database

	void begin();																	/// start transction for commit

	void commit();																	/// commit for transaction init

	void rollback();																/// rollbck for transaction init

	void end();																		/// end transaction for init

	void clearRes();																/// clear PGresult

	void savpoint();																/// point de sauvegarde

	void savpointRollback();														/// roolback base savepoint

	void savpointRelease();															/// delete savepoint

	void fetchAll( std::string , std::string cursor = "mycursor" );					/// fetch ALL include requete

	void opensql( std::string sql, std::string cursor = "mycursor" );				/// query for fetchsql record / record

	void fechsql( std::string cursor = "mycursor");												/// fetch record use openSQL



	template <typename Head, typename... Args>
	std::string  prepare(std::string const& format, Head&& head, Args&&... args);	/// formatting the query with parameters

	std::stringstream result();														/// out buffer;

	char* DoubleToChar(double _X_ ,unsigned _precision_ = 0 );							/// Double to char

	friend  std::istream& operator>>(std::istream& is ,char* t)						/// retrieve  char*  for stringstream
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
};

///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
/// 						process
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


unsigned  libPQwrp::count_format_specifiers(std::string const& format)
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



void libPQwrp::prepare(std::string const& format)
{
  if (count_format_specifiers(format) != 0)
    throw std::invalid_argument("number of arguments does not match the format string");
}



template <typename Head, typename... Args>
std::string libPQwrp::prepare(std::string const& format, Head&& head, Args&&... args)		/// formate la requete 
{

	if (count_format_specifiers(format) > 0 && first == true)
	{
		
		fin_stage = format ;  first = false ;  sqlstream.str("");
		auto last_format_pos = format.find_last_of(FORMAT_SPECIFIER);
		fin_stage  =fin_stage.erase(0,  last_format_pos +1);
		 
	}

  if (count_format_specifiers(format) != sizeof...(Args) + 1)
    throw std::invalid_argument("number of arguments does not match the format string");

  // TODO: take care of escaped format specifiers
   sqlstream <<"";
  auto first_format_pos = format.find_first_of(FORMAT_SPECIFIER);
  
   sqlstream << format.substr(0, first_format_pos);  
   sqlstream << head;
  
  if ( count_format_specifiers(format) == 1 ) {  sqlstream<<fin_stage; first =true  ;}
  
  prepare(format.substr(first_format_pos+1), std::forward<Args>(args)...);
  return  sqlstream.str();
 
}




std::stringstream  libPQwrp::result()												/// retrieve values on the fields
{	

	char* tptr;
	std::stringstream sql;


    
	for ( int i= 0 ; i <cols ; i ++ )
	{
		tptr = PQgetvalue(res, rown, i);
		

		for (size_t i = 0; i < strlen(tptr); ++i)
		{
			if (tptr[i] == ' ')
			{
				tptr[i] =DeLiMiTaTioN;
			}
		}


		if ( i == 0 ) sql << tptr;
		else sql <<' '<< tptr;

	}
	return  sql;
}




void libPQwrp::connectDB(std::string info)											/// connect to the database
{
	
	clean = true ;
	
	conn= PQconnectdb(info.c_str());  												/// init PGconn
    if (PQstatus(conn) != CONNECTION_OK)
    {  
        char const* msg = PQerrorMessage(conn);
        throw std::runtime_error(std::string(msg));
    }
    res = PQgetResult(conn);														/// init PGresult
}



void libPQwrp::qexec( std::string sql)												/// PQexec 
{
	
	clean = true ;
	
    res = PQexec(conn, sql.c_str());

    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK)
    {	PQclear(res); 
        char const* msg = PQresultErrorMessage(res);
        throw std::runtime_error(std::string(msg));
    }
}


void libPQwrp::query(std::string sql, bool binary )									/// PQquery
{
    int const fmt = binary ? 1 : 0;
 
	clean = true ;
	
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

}


char* libPQwrp::fetch( int row, int column)											/// get the value of the row and the column
{
	if (row < 0)
		throw std::invalid_argument("row position is negative");

	if (column < 0 || column > PQnfields(res) )
		throw std::invalid_argument("column position is negative");

	return PQgetvalue(res, row, column);
}



double libPQwrp::fetchDbl(int row, int column)										/// get the value of the row and the column
{
	if (row < 0)
		throw std::invalid_argument("row position is negative");

	if (column < 0 || column > PQnfields(res) )
		throw std::invalid_argument("column position is negative");

	return atof(PQgetvalue(res, row, column));
}



int libPQwrp::fetchInt(int row, int column)											/// get the value of the row and the column
{
	if (row < 0)
		throw std::invalid_argument("row position is negative");

	if (column < 0 || column > PQnfields(res) )
		throw std::invalid_argument("column position is negative");

	return atoi(PQgetvalue(res, row, column));

}


int libPQwrp::nfield(std::string field)												/// number column of the field
{
	int col =  PQfnumber(res, field.c_str());

	if ( col<0 )
	{
		std::string msg="bad name field :"  + field  ;      
		throw std::runtime_error(msg);
	}
	return col;
}


const char* libPQwrp::cfield(int nfield)											/// name of the field 
{
	if (nfield < 0 || nfield > PQnfields(res) )
	{
		std::string msg="bad n° field :"  + std::to_string(nfield) ;      
		throw std::runtime_error(msg);
    }
	return  PQfname(res,nfield);

}


int libPQwrp::countfield()															/// number of columns
{
	return  PQnfields(res);
}



int libPQwrp::countrow()															/// number of rows
{
	return PQntuples(res);	
}


bool libPQwrp::is_Table(const char* table)											/// if exist table of the database 
{
	static std::string const exist_Table = "SELECT count(*)  FROM pg_tables  WHERE  schemaname ='public' AND    tablename = '?'";
 
	const std::string sql = prepare(exist_Table ,table);

	bool binary = false;															/// false for return value count

	int const fmt = binary ? 1 : 0;
 
    res = PQexecParams(conn, sql.c_str(), 0, 0, 0, 0, 0, fmt);
    
	int count = atoi(PQgetvalue(res, 0, 0));
	
	PQclear(res);
	
	clean = false;
	
	
	return count;
}


void libPQwrp::closeDB()															/// close to the database 
{
	clearRes();
	PQfinish(conn);
}



void libPQwrp::begin()																/// start transction for commit
{
	PQexec(conn,"BEGIN");
}


void libPQwrp::commit()																/// commit for transaction init
{
	PQexec(conn,"COMMIT");
	clearRes();
}


void libPQwrp::rollback()															/// rollbck for transaction init
{
	PQexec(conn,"ROLLBACK");
	clearRes();
}


void libPQwrp::end()																/// end transaction for init 
{
	PQexec(conn,"END");
	clearRes();
}


void libPQwrp::clearRes()															/// clear PGresult
{
	if (clean) { PQclear(res); clean = false; }
}


void libPQwrp::savpoint()															/// point de sauvegarde
{
	PQexec(conn,"SAVEPOINT full_savepoint");
	clearRes(); 
}


void libPQwrp::savpointRollback()													/// roolback base savepoint
{
	PQexec(conn,"ROLLBACK TO SAVEPOINT full_savepoint");
	clearRes();
}


void libPQwrp::savpointRelease()													/// delete savepoint
{
	PQexec(conn,"RELEASE SAVEPOINT full_savepoint");
	clearRes();
}



///-------------------------------------------------------------------------------------------------------
///-------------- process  complex
///-------------------------------------------------------------------------------------------------------


void libPQwrp::fetchAll( std::string sql, std::string cursor)						/// fetch ALL include requete 
{
	std::string  ordreSQL;

	clean = true ;
	 
    ordreSQL =  "DECLARE " + cursor + " CURSOR FOR "+ sql;

	res = PQexec(conn, ordreSQL.c_str());
    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK)
    {
		PQclear(res);
		char const* msg = PQresultErrorMessage(res);
		throw std::runtime_error(std::string(msg));
	}
	PQclear(res);

	ordreSQL = "FETCH ALL in " + cursor ;	/// lecture full rc  ??? memory 
     
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
		clean = false ;
	}
	else fetchEOF  =false ;					/// no end of row


	rows	= PQntuples(res);
	cols	= PQnfields(res);
	rown	= 0;
	coln	= 0;


}



void libPQwrp::opensql( std::string sql, std::string cursor)						///  query for fetchsql record / record 
{
	std::string  ordreSQL;

	clean = true ;
	 
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
		clean = false ;
	}
	else
	{
		fetchEOF  =false ;					/// no end of row
		PQclear(res);
		ordreSQL = "FETCH FIRST in " + cursor ;	/// read a line record only
		res = PQexec(conn, ordreSQL.c_str());
	}
	rows	= PQntuples(res);
	cols	= PQnfields(res);
	rown	= 0;
	coln	= 0;

}




void libPQwrp::fechsql(std::string cursor )											/// fetch record use openSQL
{
	std::string  ordreSQL;

	clean = true ;
	
     ordreSQL = "FETCH NEXT in " + cursor ;	/// read a line record only

	res = PQexec(conn, ordreSQL.c_str());
 	std::string operation = PQcmdStatus(res);   
	if ( operation =="FETCH 0"  )
	{
		fetchEOF  =true ;					/// end of row or without row
		ordreSQL = "close" + cursor ;		/// close cursor
		PQexec(conn, ordreSQL.c_str());
		PQclear(res);
		clean = false;
	}
	else fetchEOF  =false ;					/// no end of row

	rows	= PQntuples(res);
	cols	= PQnfields(res);
	rown	= 0;
	coln	= 0; 
}




char* libPQwrp::DoubleToChar(double _X_ ,unsigned _precision_)						/// Double to char 
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


} // namespace libpqwrp

using namespace libpqwrp ;

#endif // LIBPQWRP_H_INCLUDED


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


#ifndef PQWRP_H_INCLUDED
#define PQWRP_H_INCLUDED

#include <string.h>
#include <math.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <ostream>
#include <sstream>
#include <iomanip>

#include <exception>
#include <typeinfo>
#include <stdexcept>

#include <libpq-fe.h>


#include <type_traits>

namespace libpqwrp
{


///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
///				fonction sql de libpq 
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
///				fonction sql de libpq 
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$





/// a function to make your alphanumeric switch
constexpr unsigned long long int HashStringToInt(const char *str, unsigned long long int hash = 0)
{
    return (*str == 0) ? hash : 101 * HashStringToInt(str + 1) + *str;
}


#define NAMEOF(variable) ((void)variable, #variable)

#ifndef   _separator_
#define   _separator_		'|'		///  remplace caractère space  de délimitation multibuffer > stringstream
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
	
	bool errorSQL ;				/// fatal_error

	bool fetchEOF ;				/// fin de traitenent get reccords 

	int			rows;			/// initialise par les requetes
	int			cols;			/// initialise par les requetes

	int			rown;			/// variable de traitement
	int			coln;			/// variable de traitement

	
	PGresult* res;

	libPQwrp(){
	errorSQL =false ;

	fetchEOF =false ;

		};

	~libPQwrp(){};

	void connectDB(std::string info);												/// connect to the database
	
	void qexec( std::string sql);													/// PQexec

	void query(std::string sql, bool binary = true );								/// PQquery
	
	void fetchAll( std::string , std::string cursor = "mycursor" );					/// fetch ALL include requete

	void opensql( std::string sql, std::string cursor = "mycursor" );				/// query for fetchsql record / record

	void fetchsql( std::string cursor = "mycursor");								/// fetch record use openSQL

	void openupd( std::string sql);													/// query select for update

	char* rtvCh(int row, int column);												/// get the value of the row and the column
	
	double rtvDbl(int row, int column);												/// get the value of the row and the column

	int rtvInt(int row, int column);												/// get the value of the row and the column

	int nfield(std::string field);													/// number column of the field

	const char* cfield(int nfield);													/// name of the field

	int countfield();																/// number of columns

	int countrow();																	/// number of rows

	long int countqry(std::string sql);												/// count select .....

	bool is_Table(const char* table);												/// if exist table of the database

	void closeDB();																	/// close to the database
	
	void autocommitON();															/// set auotcommit ON
	
	void autocommitOFF();															/// set auotcommit OFF
		
	void begin();																	/// start transction for commit

	void commit();																	/// commit for transaction init

	void rollback();																/// rollbck for transaction init

	void end();																		/// end transaction for init

	void clearRes();																/// clear PGresult

	void savpoint();																/// point de sauvegarde

	void savpointRollback();														/// roolback base savepoint

	void savpointRelease();															/// delete savepoint






	template <typename Head, typename... Args>
	std::string prepare(std::string const& format, Head&& head, Args&&... args)		/// formatting the query with parameters
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
  
		if ( count_format_specifiers(format) == 1 )
		{
			sqlstream<<fin_stage; first =true ;
		}
  
		prepare(format.substr(first_format_pos+1), std::forward<Args>(args)...);
		return  sqlstream.str();
 
	}




		
	std::stringstream result();														/// out buffer;
	char* DoubleToChar(double _X_ ,unsigned _precision_ = 0 );						/// Double to char





};




} // namespace libpqwrp

using namespace libpqwrp ;

#endif // LIBPQWRP_H_INCLUDED

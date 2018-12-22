/*
 * Tclasse.cpp
 * 
 * Copyright 2018 SOLEIL <soleil@TERRA>
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
 * 
 */


#include <ZONED.hpp>

#include <libpqwrp.cxx>


#ifndef  vdate_HPP_INCLUDED
#define  vdate_HPP_INCLUDED
	 Zdate vdate;	//	filed date
#endif
#ifndef  vnumeric_HPP_INCLUDED
#define  vnumeric_HPP_INCLUDED
	 Zdcml vnumeric(8,2);	//	field  decimal (8,2)
#endif
#ifndef  vtext_HPP_INCLUDED
#define  vtext_HPP_INCLUDED
	 Ztext vtext;	//	field text
#endif
#ifndef  vonchar_HPP_INCLUDED
#define  vonchar_HPP_INCLUDED
	 Zchar vonchar(1);	//	field char
#endif
#ifndef  vheure_HPP_INCLUDED
#define  vheure_HPP_INCLUDED
	 Ztime vheure;	//	field heure
#endif
#ifndef  vkey_HPP_INCLUDED
#define  vkey_HPP_INCLUDED
	 Zdcml vkey(7,0);	//	field  decimal (7,0)
#endif
#ifndef  vbool_HPP_INCLUDED
#define  vbool_HPP_INCLUDED
	 Zbool vbool;	//	Test bool
#endif
#ifndef  vchar_HPP_INCLUDED
#define  vchar_HPP_INCLUDED
	 Zchar vchar(10);	//	field char (10)
#endif

 




class PG_tabletype	: public libPQwrp												/// gestion parametre sql 
{
	private:
	std::string cursorName = "tabletype";

	protected:

	public:

	bool chainR(Zdcml vkey);

	bool chainL(Zdcml vkey);

	bool insert();

	bool remove(Zdcml vkey);

	bool update(Zdcml vkey);

	bool check(Zdcml vkey);

};



	libPQwrp sql;
	libPQwrp slc;
	bool transac;
	std::string requete ;
	std::stringstream rep ;



int main()
{
		std::setlocale(LC_ALL, "");
	//std::setlocale(LC_NUMERIC, "C");		// force '.'
	 	std::setlocale(LC_NUMERIC,std::locale("").name().c_str());					/// user local
	PG_tabletype tbl;

	sql.connectDB("host=localhost port=5432 dbname =CGIFCH user=postgres password=pgmr application_name=Tclasse ");
	slc.connectDB("host=localhost port=5432 dbname=CGIFCH user=readonly password=read application_name=Tiostream");

	std::cout<<"bonjour"<<std::endl;
	vdate = 20180702 ;
	vnumeric =1951.13;
	vtext ="test";
	vonchar = "Y";
	vheure ="17:34:01";
	vkey=101;
	vbool = true ;
	vchar ="JP-Laroche";

	printf(" vnumeric %s \n",vnumeric.ToChar());  std::cout<<vnumeric<<std::endl; std::cout<<vnumeric.String()<<std::endl;
	sql.begin() ;
try{	
	if ( tbl.check(vkey) )
		{
			vtext ="test jpl";
			tbl.update(vkey);
		}
	else if (slc.errorSQL ) throw std::runtime_error("erreur programme 124\n");
	else if ( !tbl.check(vkey) )tbl.insert();


	vkey=102; 	vtext ="test 102";
	if ( tbl.check(vkey) )
		{
			tbl.remove(vkey);
		}
	else if (slc.errorSQL ) throw std::runtime_error("erreur programme 133 \n");
	else if ( !tbl.check(vkey) ) tbl.insert();
	sql.end();


 	std::cin.ignore (std::cin.rdbuf () -> in_avail () + 1);
/*	sql.begin() ;
		vkey=101;
 	tbl.remove(vkey);
			vkey=102;
	tbl.remove(vkey);
	sql.end();
*/ 
	sql.begin() ;
	vkey=102;	tbl.check(vkey); printf(" tbl.check vkey=102 slc.errorSQL %d\n",slc.errorSQL);  
	vkey=101; if (!tbl.check(vkey) && slc.errorSQL) throw std::runtime_error("erreur programme 148 \n");
//	if ( !tbl.check(vkey) && ! slc.errorSQL) throw std::runtime_error("erreur programme 148 \n");

	sql.end();
	sql.closeDB();
//		std::cout<<" tag ctrl "<<std::endl;
}
catch (const std::exception& e)
	{
	  std::cerr<<e.what()<<"fin de procedure Tclass\n"<<std::endl;
	  return EXIT_FAILURE ;
	}

	 printf(" Fin de programme\n");
	return 0;
}
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
		
/// check row key
	bool PG_tabletype::check(   Zdcml vkey)
{
try{

	requete= slc.prepare("SELECT count(*) FROM tabletype \
	 WHERE (vkey = ? )",
		vkey  ); 
 
	if ( slc.countqry(requete) > 0 )	return true ;
	return false ;
 
}catch (const std::exception& e)
	{
	  std::cerr<<e.what()<<"Check requete impossible"<<std::endl;
	  return false;
	}
}

/// INSERT 
	bool PG_tabletype::insert()
{
	try{
		
	requete= sql.prepare(\
	"INSERT INTO tabletype "\
	"(vdate, vnumeric, vtext, vonchar, vheure, vkey, vbool, vchar)" \
	" VALUES('?', ?, '?', '?', '?', ?, '?', '?'); " \
	,vdate, vnumeric, vtext, vonchar, vheure, vkey, vbool, vchar);
	
	sql.query(requete);
	return true ;

	}catch (const std::exception& e)
	{
	  std::cerr<<e.what()<<"Insert requete impossible"<<std::endl;
	  return false;
	}
}

/// DELETE
	bool PG_tabletype::remove(Zdcml kvkey)
{
	try{

	requete= sql.prepare(\
	"DELETE FROM tabletype "\
	"WHERE ( vkey= ? ); " \
	,kvkey);
	

	sql.query(requete);

	return true ;

	}catch (const std::exception& e)
	{
	  std::cerr<<e.what()<<"Remove requete impossible"<<std::endl;
	  return false;
	}
}

/// UPDATE
	bool PG_tabletype::update(Zdcml kvkey)
{
	try{
		
	requete= sql.prepare(\
	"UPDATE tabletype "\
	"SET vdate = '?', vnumeric = ? , vtext ='?', vonchar = '?', vheure = '?', vbool = '?', vchar = '?' " \
	" WHERE (vkey = ? ); " \
	,vdate, vnumeric, vtext, vonchar, vheure, vbool, vchar,kvkey);

	printf("requete %s",requete.c_str());
	sql.query(requete);

	return true ;

	}catch (const std::exception& e)
	{
	  std::cerr<<e.what()<<"Update requete impossible"<<std::endl;
	  return false;
	}
}


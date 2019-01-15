#include <ZONED.hpp> 
/// INIT-FIELD 

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






/// check row key
	bool PG_tabletype::check(   Zdcml k_vkey)
{
try{

	requete= slc.prepare("SELECT count(*) FROM tabletype \
	 WHERE (vkey = ? )",
		k_vkey  ); 
 
	if ( slc.countqry(requete) > 0 )	return true ;
	return false ;
 
}catch (const std::exception& e)
	{
	  std::cerr<<e.what()<<"Check requete impossible"<<std::endl;
	  return false;
	}
} 



 		 



 		 



 		 

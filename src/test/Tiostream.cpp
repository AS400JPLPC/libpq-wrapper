/*
 * Tiostream.cpp
 * 
 * Copyright 2018 SOLEIL <laroche.jeanpierre@gmail.com>
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





#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <cstdio>
#include <cstdlib>
#include <typeinfo>     // name 
#include <exception>



#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>




#include <ZONED.hpp>

#include <libpqwrp.hpp>   // std::stringstream


///*************************************************
// #include <fonc.h>


using namespace std;
 
int main()
{
 
	PGconn*		conn ;
	PGresult*	res;
	const char *conninfo;

	QueryPrint stm;

	std::string requete ;
	
	std::string cursorName = "MONcursor";






Zind_Init_All();

	Zchar nom(15);
	Zdcml dcml(15,2);
	int   test =0;



/// faire un include de la variable  afin d'avoir une seul definition pour tous les programmes

static const char* create_employeesx =
"CREATE TABLE employeesx 						\
(														\
	emp_id integer NOT NULL,							\
	name character varying(32) ,						\
	CONSTRAINT employeesx_pkey PRIMARY KEY (emp_id)		\
);														\
COMMENT ON TABLE employeesx								\
	IS 'Fichier employees';								\
														\
COMMENT ON COLUMN employeesx.emp_id		IS 'CODE EMPLOYEE';	\
COMMENT ON COLUMN employeesx.name	    IS 'NOM EMPLOYEE';	\
"
;



static const char* create_patron =
"CREATE TABLE patron 									\
(														\
	emp_id integer NOT NULL,							\
	name character varying(32) ,						\
	CONSTRAINT patron_pkey PRIMARY KEY (emp_id)		\
);														\
COMMENT ON TABLE patron									\
	IS 'Fichier patron';								\
														\
COMMENT ON COLUMN patron.emp_id		IS 'CODE EMPLOYEE';	\
COMMENT ON COLUMN patron.name	    IS 'NOM EMPLOYEE';	\
"
;

 try{
	conninfo = "dbname =CGIFCH host=localhost port=5432 user=postgres password=pgmr";
  
	conn = connectDB(conninfo);
	/// deux exemple pour count(*) 

					printf("\n000    count\n");
	if ( ! is_Table(conn,"employees") ) qexec(conn,"CREATE TABLE employees (emp_id integer not null, name varchar(32));"); 
	// nbr employe 
	res = query(conn," select count(*) from \"employees\" where \"emp_id\" = 1;",false);  	// false oblogatoire pour return valeur count(*)
		 for (int i = 0; i < countrow(res); i++)
			for (int j = 0; j < countfield(res); j++)
				{
					std::cout<<cfield(res,j)<<std::endl;
					std::cout<<countfield(res)<<std::endl;
				}

				printf("\n001  close connect\n");

	closeDB(conn);
 	conn = connectDB(conninfo);
 	
					printf("\n002 is table   command    creat\n");

	 // exemple  print  true / false  exist table 
	 if ( ! is_Table(conn,"employeesx")  ) qexec(conn,create_employeesx);

					printf("\n003 \n");
		if ( ! is_Table(conn,"patron")  ) qexec(conn,create_patron);

					printf("\n004   instert \n");
	// requete = stm.prepare("INSERT INTO employeesx (emp_id , name ) VALUES ( ? , ?);",1,"'NOM'");	
	// res = query(conn,requete);




					printf("\n006   select \n"); 
  requete = stm.prepare("SELECT emp_id , name FROM  employeesx WHERE emp_id = ?;", 1);  res = query(conn,requete);
     
    				printf("\n007 select incorrect  ou list de la selection \n"); if (select0 ) std::cout<<"tartuf select"<<std::endl;
    				
		 for (int i = 0; i < countrow(res); i++)
			
                for (int j = 0; j < countfield(res); j++)
                {
					std::cout<<cfield(res,j)<<std::endl;			// name
					std::cout<<fetch(res, i,j)<<std::endl;			// value
				}

/// traitement error ok
																			printf("\n007.1 update is error \n");
requete = stm.prepare("UPDATE patron SET name = 'Nous'  WHERE emp_id = ?", 1);  res = query(conn,requete);
if (update0 ) std::cout<<"tartuf update"<<std::endl;

																			printf("\n007.2 delete is error \n");
requete = stm.prepare("DELETE FROM patron WHERE emp_id = ?", 1);  res = query(conn,requete);
if (delete0 ) std::cout<<"tartuf delete"<<std::endl;

																			printf("\n007.3 insert is error \n");
requete = stm.prepare("INSERT INTO patron (emp_id , name ) VALUES ( ? , ?);",1,"'NOM'"); res = query(conn,requete);
				if (insert0 ) std::cout<<"tartuf insert"<<std::endl;

																			printf("\n007.4 insert is error \n");
//requete = stm.prepare("INSERT INTO patron (emp_id , name ) VALUES ( ? , ?);",1,"'NOM'"); res = query(conn,requete);
//				if (insertNO ) std::cout<<"tartuf insert"<<std::endl;


																			printf("\n008 update is error\n");
requete = stm.prepare("UPDATE patron SET name = 'Nous'  WHERE emp_id = ?", 1);  res = query(conn,requete); // CHG  2
if (update0 ) std::cout<<"tartuf update"<<std::endl;
 


																			printf("\n008.1 controle commit & savpoint\n");


	// test avec 2 connexions pour controler le comportement  
  
 	PGconn*		oups;
 	oups = connectDB(conninfo);
 	
	begin(conn);
	
	savpoint(conn);

	
	requete = stm.prepare("UPDATE employees SET name = 'laroche'  WHERE emp_id = ?;", 1);
  
	res = query(conn,requete);
		
	requete = stm.prepare("UPDATE patron SET name = 'Moi'  WHERE emp_id = ?;", 1);

 

  
	res = query(oups,requete); // le commit impacte que la connexion de begin conn




// rollback(conn);
savpointRollback(conn);

//savpointRelease(conn);  // valide la transction

    


																			printf("\n009 delete\n"); 

//res = query(conn," DROP TABLE employeesx;");


																			printf("\n009 retrieve to buffer  for select  \n"); 
    requete = stm.prepare("SELECT emp_id , name FROM  patron WHERE emp_id = ?", 1);
    res = query(conn,requete);

																			printf("\n010\n"); 
    for (int row = 0; row < countrow(res); row++)
		for (int nf = 0;nf < countfield(res); nf++)
			{
				switch (HashStringToInt(cfield(res,nf)))
				{
					case HashStringToInt("name"):		nom = fetch(res, row, nf) ;   break;

					case HashStringToInt("emp_id"):		test = atoi(fetch(res, row, nf)) ;


					break;

					default : 	break;
				}
				
			}


		std::cout<<nom.ToChar()<<std::endl;  		std::cout<<test<<std::endl;



																			printf("\n010 trt name sql field\n");
	//cfield(res,3);
																			printf("\n011 delete\n");

	// query(conn,"DROP TABLE employeesx");

																			printf("\n012 type field\n");

/*INSERT INTO public.typetable
(vdate, vnumeric, vtext, vdouble, vint, vonchar, vbool, vbpchar, vtimestamp, vvarcharcolumn1)
VALUES('12-10-51', 0, 'text', 23.003, 5, 'C', false, 'LAROCHE', '12-10-5112:10:01', 'xxxxxxxxxxxxxxxxxxxxxxxxxx');
*/
																			

 //    requete = stm.prepare("SELECT vdate, vnumeric, vtext, vdouble, vint, vonchar, vbool, vbpchar, vtimestamp, vvarchar FROM  typetable WHERE vint = ?", 5);





/// select the name off field  in table  
requete = stm.prepare(\
"SELECT \
cl.table_catalog,\
cl.table_name,\
cl.column_name \
,coalesce(cl.ORDINAL_POSITION,0) as ordre \
,cl.DATA_TYPE \
,coalesce(cl.CHARACTER_MAXIMUM_LENGTH,0) as length \
,coalesce(cl.NUMERIC_PRECISION,0) as precision \
,coalesce(cl.NUMERIC_SCALE,0) as scale \
,(select pg_catalog.col_description(oid,cl.ordinal_position::int) \
from pg_catalog.pg_class c \
where c.relname=cl.table_name) as column_comment \
FROM information_schema.columns cl \
WHERE cl.table_catalog='?'  and cl.table_name='?' and cl.column_name= '?' order by 4 ;", \
"CGIFCH","FC0CLI","C0NCLI");


std::cout<<requete<<std::endl;

    res = query(conn,requete);

long int   ival;    
char       *iptr ;



    for (int row = 0; row < countrow(res); row++)
		for (int nf = 0;nf < countfield(res); nf++)
			{ 

				switch (HashStringToInt(cfield(res,nf)))
				{
					case HashStringToInt("table_catalog"):	std::cout<<fetch(res, row, nf)<<std::endl;   break;
					
					case HashStringToInt("table_name"):		std::cout<<fetch(res, row, nf)<<std::endl;   break;
					
					case HashStringToInt("column_name"):	std::cout<<fetch(res, row, nf)<<std::endl;   break;
					
					case HashStringToInt("ordre"):			iptr =fetch(res, row, nf);
															ival = ntohl(*((uint32_t *) iptr));
															std::cout<< ival<<std::endl;
															break;

					case HashStringToInt("data_type"):		std::cout<<fetch(res, row, nf)<<std::endl; 	break;

					case HashStringToInt("length"):			iptr =fetch(res, row, nf);
															ival = ntohl(*((uint32_t *) iptr));
															std::cout<< ival<<std::endl;
															break;

					case HashStringToInt("precision"):		iptr =fetch(res, row, nf);
															ival = ntohl(*((uint32_t *) iptr));
															std::cout<< ival<<std::endl;
															break;

					case HashStringToInt("scale"):			iptr =fetch(res, row, nf);
															ival = ntohl(*((uint32_t *) iptr));
															std::cout<< ival<<std::endl;
															break;

					case HashStringToInt("column_comment"):	std::cout<<fetch(res, row, nf)<<std::endl;   break;

					
					default : std::cout<<cfield(res,nf)<<std::endl;	break;
				}
				//std::cout<<cfield(res,nf)<<std::endl;   
				//std::cout<<fetch(res,row,nf)<<std::endl;
			}



end(conn);


begin(conn);

std::string cursorName = "mycursor";

/*
CREATE TABLE public."FNBDOS" (
	"NBCAPR" bpchar(2) NULL,
	"NBCSTD" bpchar(8) NOT NULL,
	"NBCDPO" numeric(2) NOT NULL,
	"NBCGIM" bpchar(1) NOT NULL,
	"NBCHTR" bpchar(8) NULL,
	"NBCREP" numeric(3) NULL,
	"NBCTVC" numeric(2) NULL,
	"NBCUTR" bpchar(5) NULL,
	"NB999E" date NULL,
	"NB999M" date NULL,
	"NBMQOT" numeric(8,2) NULL,
	"NBMTRF" numeric(8,2) NULL,
	"NBMPRV" numeric(8,2) NULL,
	"NBNDOS" numeric(8) NOT NULL,
	"NBNLZE" numeric(4) NULL,
	"NBNCAR" numeric(2) NULL,
	"NBNFEU" numeric(2) NULL,
	"NBQCDE" numeric(10) NULL,
	"NBQFAC" numeric(10) NULL,
	"NBQLVA" numeric(10) NULL,
	"NBQLVR" numeric(10) NULL,
	"NBQLVM" numeric(10) NULL,
	"NBSHS" numeric(1) NULL,
	"NBTTAS" bpchar(1) NULL,
	"NBXCFT" numeric(5) NULL,
	"NBXCOM" numeric(5,2) NULL,
	"NBZIMP" bpchar(30) NULL,
	CONSTRAINT "FNBDOS_pkey" PRIMARY KEY ("NBNDOS", "NBCGIM", "NBCDPO", "NBCSTD")
)
WITH (
	OIDS=FALSE
) ;
COMMENT ON TABLE public."FNBDOS" IS 'DETAIL DE COMMANDES          NEGOCE' ;
COMMENT ON COLUMN public."FNBDOS"."NBCAPR" IS 'CODE ACTI.PRODUIT' ;
COMMENT ON COLUMN public."FNBDOS"."NBCSTD" IS 'COD_ART._STK/DIVERS' ;
COMMENT ON COLUMN public."FNBDOS"."NBCDPO" IS 'CODE DEPOT' ;
COMMENT ON COLUMN public."FNBDOS"."NBCGIM" IS 'COD_GESTION_IMP/STK' ;
COMMENT ON COLUMN public."FNBDOS"."NBCHTR" IS 'CODE HAUTEUR' ;
COMMENT ON COLUMN public."FNBDOS"."NBCREP" IS 'CODE REP / COURTIER' ;
COMMENT ON COLUMN public."FNBDOS"."NBCTVC" IS 'CODE TARIF  VTE' ;
COMMENT ON COLUMN public."FNBDOS"."NBCUTR" IS 'CODE UNITE TARIFIC.' ;
COMMENT ON COLUMN public."FNBDOS"."NB999E" IS 'DATE DE CREATION ENR' ;
COMMENT ON COLUMN public."FNBDOS"."NB999M" IS 'DATE DE MODIFCATION' ;
COMMENT ON COLUMN public."FNBDOS"."NBMQOT" IS 'M. PRIX QUOTA' ;
COMMENT ON COLUMN public."FNBDOS"."NBMTRF" IS 'M. PRIX  TARIF' ;
COMMENT ON COLUMN public."FNBDOS"."NBMPRV" IS 'M. PRIX  VENTE' ;
COMMENT ON COLUMN public."FNBDOS"."NBNDOS" IS 'N.CHRONO DOSSIER' ;
COMMENT ON COLUMN public."FNBDOS"."NBNLZE" IS 'LAIZE HORS TOUT' ;
COMMENT ON COLUMN public."FNBDOS"."NBNCAR" IS 'N. NBR CARBONES' ;
COMMENT ON COLUMN public."FNBDOS"."NBNFEU" IS 'N. NBR. FEUILLETS' ;
COMMENT ON COLUMN public."FNBDOS"."NBQCDE" IS 'QTE_CDE' ;
COMMENT ON COLUMN public."FNBDOS"."NBQFAC" IS 'QTE_FACTUREE' ;
COMMENT ON COLUMN public."FNBDOS"."NBQLVA" IS 'QTE_à_LIVRER' ;
COMMENT ON COLUMN public."FNBDOS"."NBQLVR" IS 'QTE_LIVREE_REELLE' ;
COMMENT ON COLUMN public."FNBDOS"."NBQLVM" IS 'QTE_LIVREE_RESERVE' ;
COMMENT ON COLUMN public."FNBDOS"."NBSHS" IS '0/1  N=0 O=1 H.SERVC' ;
COMMENT ON COLUMN public."FNBDOS"."NBTTAS" IS 'TOTAL/ACOMPTE' ;
COMMENT ON COLUMN public."FNBDOS"."NBXCFT" IS 'X. COEF CODE TARIFI.' ;
COMMENT ON COLUMN public."FNBDOS"."NBXCOM" IS 'X.TAUX DE COMMISSION' ;
COMMENT ON COLUMN public."FNBDOS"."NBZIMP" IS 'REFERENCE IMPRIME' ;
*
UPDATE public."FNBDOS"
SET "NBCAPR"='22', "NBCHTR"='      19', "NBCREP"=58, "NBCTVC"=-16, "NBCUTR"='MILLE', "NB999E"='2012-10-23',
 "NB999M"='2013-02-20', "NBMQOT"=6.95, "NBMTRF"=8.28, "NBMPRV"=6.95, "NBNLZE"=39, "NBNCAR"=0, "NBNFEU"=1,
 "NBQCDE"=900000000, "NBQFAC"=0, "NBQLVA"=900000000, "NBQLVR"=0, "NBQLVM"=0, "NBSHS"=0, "NBTTAS"='A', "NBXCFT"=1000,
 "NBXCOM"=0.00, "NBZIMP"='19X39  ETQ PF BOB 4000    KEYS'
WHERE "NBNDOS"=21071110 AND "NBCGIM"='S' AND "NBCDPO"=21 AND "NBCSTD"='RDS19X39';

*/

/// select table n° order , prix , name of the print
	requete = stm.prepare("SELECT \"NBNDOS\", \"NBMPRV\", \"NBZIMP\" FROM  public.\"FNBDOS\" WHERE \"NBNDOS\" = '?'",21071110);
	std::cout<<requete<<std::endl;

// mprv = 6.95
/// fetch all 
	res = fetchAll(conn,requete, cursorName);
	if ( !fetch0)
	{
std::cout<<"code status  :"<<PQresultStatus(res)<<" OK:"<<PGRES_TUPLES_OK<<"  value result  "<<PQcmdStatus(res)<<std::endl;
		printf("-- %s --\n", "FNBDOS");

		for (int row = 0; row < PQntuples(res); row++)
		{

			for (int col = 0; col < PQnfields(res); col++)
			{
				std::cout<<cfield(res,col)<<std::endl;
				std::cout<< PQgetvalue(res, row, col) <<std::endl;
			}
		}
	}
PQclear(res);

end(conn);


Zdcml NBNDOS(8,0);
Zdcml NBMPRV(8,2);
Zchar NBZIMP(30);


NBNDOS = 15;
NBMPRV = 78.3;
NBZIMP = "bonjour" ;


printf("detail 001\n");

std::cout<<NBNDOS.ToChar()<<std::endl;  std::cout<<NBMPRV.ToChar()<<std::endl; std::cout<<NBZIMP.ToChar()<<std::endl;

NBMPRV =16.95 ; NBNDOS=21071110;
requete = stm.prepare("UPDATE public.\"FNBDOS\"  SET \"NBMPRV\" = ?  WHERE \"NBNDOS\" = '?'",NBMPRV,NBNDOS);
     std::cout<<requete<<std::endl;
	res = query(conn,requete);
printf("OK\n");
commit(conn);
end(conn);



	requete = stm.prepare("SELECT \"NBNDOS\", \"NBMPRV\", \"NBZIMP\" FROM  public.\"FNBDOS\" WHERE \"NBNDOS\" = '?'",21071110);
	std::cout<<requete<<std::endl;

begin(conn);
	res = opensql(conn,requete, cursorName);
	do
	{
		if ( fetchEOF ==false )
			for (int row = 0; row < records.rows; row++)
			{
				for (int col = 0; col < records.cols; col++)
				{
					std::cout<<cfield(res,col)<<" -row :"<<row<< " -col :"<< col<<std::endl;
					std::cout<< PQgetvalue(res, row, col) <<std::endl;
				}
			}
		res = fechsql(conn);
	}	while  (fetchEOF ==false) ;

end(conn);


NBMPRV =6.95 ; NBNDOS=21071110;
requete = stm.prepare("UPDATE public.\"FNBDOS\"  SET \"NBMPRV\" = ?  WHERE \"NBNDOS\" = '?'",NBMPRV,NBNDOS);
     std::cout<<requete<<std::endl;
	res = query(conn,requete);
printf("OK\n");
commit(conn);
end(conn); 
	requete = stm.prepare("SELECT \"NBNDOS\", \"NBMPRV\", \"NBZIMP\" FROM  public.\"FNBDOS\" WHERE \"NBNDOS\" = '?'",21071110);
	std::cout<<requete<<std::endl;
printf("result 001\n");
begin(conn);
	res = opensql(conn,requete, cursorName);
	do
	{
		if ( fetchEOF ==false ) stm.result(res)>>NBNDOS>>NBMPRV>>NBZIMP;

std::cout<<NBNDOS<<std::endl;  std::cout<<NBMPRV<<std::endl; std::cout<<NBZIMP<<std::endl;
		
		if ( fetchEOF ==false ) res = fechsql(conn);
 	}	while  (fetchEOF ==false) ;
	

end(conn);
//closeDB(conn);


printf("result 001.1\n");
 
begin(conn);

 
char *	vdate = new char[8];
double Ndouble =0;
char* Nchar = new char[30];
int Nint=0 ;
printf("result 002 ******\n");
requete = stm.prepare("SELECT vdate, vnumeric, vtext , vkey FROM  typetable WHERE vkey = ?", 3681210);
     std::cout<<requete<<std::endl;

 
	res = fetchAll(conn,requete, cursorName);
 
    for (int row = 0; row < countrow(res) && fetchEOF ==false ; row++)
    {
		for (int nf = 0;nf < countfield(res); nf++)
			{ 

				switch (HashStringToInt(cfield(res,nf)))
				{
					case HashStringToInt(NAMEOF(vdate)):	vdate	=	fetch(res, row, nf);   break; //exemple avec le Nom de la Variable
					
					case HashStringToInt("vnumeric"):		Ndouble	= 	fetchDbl(res, row, nf);   break;
					
					case HashStringToInt("vtext"):			Nchar	=	fetch(res, row, nf);   break;
					
					case HashStringToInt("vkey"):			Nint	=	fetchInt(res, row, nf);
															 

															break;
					
					default : std::cout<<cfield(res,nf)<<std::endl;	break;
				}
				//std::cout<<cfield(res,nf)<<std::endl;   
				//std::cout<<fetch(res,row,nf)<<std::endl;

			}
		std::cout << vdate <<" -- "<<setprecision(8)<<Ndouble<<" -- "<<Nchar<<"  --  "<<Nint<<std::endl;
	}	
end(conn);
     
begin(conn);
	res = opensql(conn,requete, cursorName);
	do
	{
		if ( fetchEOF ==false )  stm.result(res)>>vdate>>Ndouble>>Nchar>>Nint;

std::cout << vdate <<" -- "<<Ndouble<<" -- "<<Nchar<<"  --  "<<Nint<<std::endl;
		if ( fetchEOF ==false ) res = fechsql(conn);
 	}while  (fetchEOF ==false) ;	

end(conn);


begin(conn);
requete = stm.prepare("UPDATE typetable  SET vnumeric = '?'  WHERE vkey = ?",36.04, 3681210);
	res = query(conn,requete);
printf("OK\n");
commit(conn);
end(conn);

double Xdouble = std::stod(NBNDOS.ToChar());
std::cout<<std::setprecision(NBNDOS.clen())<<Xdouble<<"   clen : "<<NBNDOS.clen()<<std::endl;




Zdate   zDate;
Zdcml   zNumeric(8,2);
Ztext   zText ;
Zdcml   zKey(7,0);
requete = stm.prepare("SELECT vdate, vnumeric, vtext , vkey FROM  typetable WHERE vkey = ?", 3681210);
//requete = stm.prepare("SELECT vnumeric FROM  typetable WHERE vint = ?", 1);
     std::cout<<requete<<std::endl;

     
begin(conn);
	res = opensql(conn,requete, cursorName);
	do
	{
		if ( fetchEOF ==false )  stm.result(res)>>zDate>>zNumeric>>zText>>zKey;
//recd(res)>>vdate>>vnumeric>>vtext ;
//			recd(res)>>vnumeric;

std::cout << zDate <<" -- "<<zNumeric<<" -- "<<zText<<"  --  "<<zKey<<std::endl;
		if ( fetchEOF ==false ) res = fechsql(conn);
 	}while  (fetchEOF ==false) ;	

end(conn);



begin(conn);
int Nkey = 3681210;
Ndouble = 345678.09;
std::cout<<std::setprecision(10)<<Ndouble<<" affiche une double"<<std::endl;
requete = stm.prepare("UPDATE typetable  SET vnumeric = ?  WHERE vkey = ?",DoubleToChar(Ndouble,2), DoubleToChar(Nkey) );
     std::cout<<requete<<std::endl;
	res = query(conn,requete);

commit(conn);
end(conn);

printf("OK\n");
closeDB(conn);
printf("fin\n");



} catch (const std::exception& e)
{
	  std::cerr<<e.what()<<"fin de procedure catch"<<std::endl;
	  return EXIT_FAILURE ;
 }


std::cout<<"fin de programme  EXIT_SUCCESS "<<std::endl;


   return EXIT_SUCCESS ; 
}


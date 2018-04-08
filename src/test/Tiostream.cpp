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
#include <sstream>

#include <exception>

#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include <ZONED.hpp>

#include <libpqwrp.hpp>


///*************************************************
 


 
 
int main()
{
 
 
 
	const char *conninfo;

	libPQwrp sql;

	std::string requete ;
	
	std::string cursorName = "MYcursor";






Zind_Init_All();

	Zchar nom(15);





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
  
	sql.connectDB(conninfo);
	/// deux exemple pour count(*) 

					printf("\n000    count\n");
	if ( ! sql.is_Table("employees") ) sql.qexec("CREATE TABLE employees (emp_id integer not null, name varchar(32));"); 
	// nbr employe 
	sql.query(" select count(*) from \"employees\" where \"emp_id\" = 1;",false);  	// false oblogatoire pour return valeur count(*)
		 for (int i = 0; i < sql.countrow(); i++)
			for (int j = 0; j < sql.countfield(); j++)
				{
					std::cout<<sql.cfield(j)<<std::endl;
					std::cout<<sql.countfield()<<std::endl;
				}

				printf("\n001  close connect\n");

	sql.closeDB();

	
 	sql.connectDB(conninfo);
 	
	printf("\n002 is table   command    creat\n");

	 // exemple  print  true / false  exist table 
	 if ( ! sql.is_Table("employeesx")  ) sql.qexec(create_employeesx); else std::cout<<"table employeesx exits"<<std::endl;

	printf("\n003 \n");
	if ( ! sql.is_Table("patron")  ) sql.qexec(create_patron);

	printf("\n004   instert \n");
	//requete = sql.prepare("INSERT INTO employeesx (emp_id , name ) VALUES ( ? , ?);",1,"'NOM'");	
	//sql.query(requete);


	printf("\n005   select \n"); 
	requete = sql.prepare("SELECT emp_id , name FROM  employeesx WHERE emp_id = ?;", 1);
	std::cout<<requete<<std::endl;
	sql.query(requete);
     
    printf("\n006 select incorrect  ou list de la selection \n");
    if (select0 ) std::cout<<"tartuf select"<<std::endl; else std::cout<<"table employeesx un enrg "<<std::endl;
    				
		 for (int i = 0; i < sql.countrow(); i++)
			
                for (int j = 0; j < sql.countfield(); j++)
                {
					std::cout<<sql.cfield(j)<< " --> ";				// name
					std::cout<<sql.fetch(i,j)<<std::endl;			// value    probleme numerique 
				}

/// traitement error ok
	printf("\n007.1 update is error \n");
	
	requete = sql.prepare("UPDATE patron SET name = 'Nous'  WHERE emp_id = ?", 1); sql.query(requete);
	if (update0 ) std::cout<<"tartuf update"<<std::endl;else std::cout<<"table patron un enrg update "<<std::endl;


	printf("\n007.2 delete is error \n");
	requete = sql.prepare("DELETE FROM patron WHERE emp_id = ?", 1);  sql.query(requete);
	if (delete0 ) std::cout<<"tartuf delete"<<std::endl;else std::cout<<"table patron un enrg delete "<<std::endl;


	printf("\n007.3 insert is error \n");
	requete = sql.prepare("INSERT INTO patron (emp_id , name ) VALUES ( ? , ?);",1,"'NOM'"); sql.query(requete);
	if (insert0 ) std::cout<<"tartuf insert"<<std::endl; else std::cout<<"table patron un enrg insert "<<std::endl;



	printf("\n007.4 insert is error  force error trow \n");
//	requete = sql.prepare("INSERT INTO patron (emp_id , name ) VALUES ( ? , ?);",1,"'NOM'"); sql.query(requete);



	printf("\n008 update is error\n");
	requete = sql.prepare("UPDATE patron SET name = 'Nous'  WHERE emp_id = ?", 1);  sql.query(requete); // CHG  2
	if (update0 ) std::cout<<"tartuf update"<<std::endl;else std::cout<<"table patron un enrg update "<<std::endl;





	printf("\n008.1 controle commit & savpoint\n");


	// test avec 2 connexions pour controler le comportement  
  
 	libPQwrp *lock = new libPQwrp;
 	lock->connectDB(conninfo);
 	
	lock->begin();
	sql.begin();
	lock->savpoint();

	
	requete = sql.prepare("UPDATE employees SET name = 'laroche'  WHERE emp_id = ?;", 1);
  
	sql.query(requete);
		
	requete = lock->prepare("UPDATE patron SET name = 'Moi'  WHERE emp_id = ?;", 1);

 

  
	lock->query(requete); // le commit impacte que la connexion de begin conn



	printf("sql.commit \n");
	sql.commit();
 
	printf("slock.savpointRollback \n");
	lock->savpointRollback();


//lock.savpointRelease(conn);  // valide la transction

	printf("lock.end() \n");
	lock->clearRes();
	
	lock->clearRes();			/// test clear n fois  
	
		
	lock->end();

		
	printf("lock.closeDB \n"); 
	lock->closeDB();
	delete lock;





	printf("\n009 delete\n"); 

	//sql.query(" DROP TABLE employeesx;");


	int test = 0 ;

	printf("\n009 retrieve to buffer  for select  \n"); 
    requete = sql.prepare("SELECT emp_id , name FROM  patron WHERE emp_id = ?", 1);
    sql.query(requete);

	printf("\n010\n"); 
    for (int row = 0; row < sql.countrow(); row++)
		for (int nf = 0;nf < sql.countfield(); nf++)
			{
				switch (HashStringToInt(sql.cfield(nf)))
				{
					case HashStringToInt("name"):		nom = sql.fetch( row, nf) ;   break;

					case HashStringToInt("emp_id"):		test = atoi(sql.fetch( row, nf)) ;


					break;

					default : 	break;
				}
				
			}


	std::cout<<"nom = sql.fetch( row, nf)  "<<nom.ToChar()<<std::endl;
	std::cout<<"test = atoi(sql.fetch( row, nf))  "<<test<<std::endl;


	printf("\n011 delete\n");

	//sql.query("DROP TABLE employeesx");




	printf("\n011 description de field\n");

/// select the name off field  in table  
requete = sql.prepare(\
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

	sql.query(requete);

	long int   ival;    
	char       *iptr ;



    for (int row = 0; row < sql.countrow(); row++)
		for (int nf = 0;nf < sql.countfield(); nf++)
			{ 

				switch (HashStringToInt(sql.cfield(nf)))
				{
					case HashStringToInt("table_catalog"):	std::cout<<sql.fetch( row, nf)<<"  ";   break;
					
					case HashStringToInt("table_name"):		std::cout<<sql.fetch( row, nf)<<"  ";   break;
					
					case HashStringToInt("column_name"):	std::cout<<sql.fetch(row, nf)<<"  ";   break;
					
					case HashStringToInt("ordre"):			iptr =sql.fetch( row, nf);
															ival = ntohl(*((uint32_t *) iptr));
															std::cout<< ival<<"  ";
															break;

					case HashStringToInt("data_type"):		std::cout<<sql.fetch( row, nf)<<"  "; 	break;

					case HashStringToInt("length"):			iptr =sql.fetch( row, nf);
															ival = ntohl(*((uint32_t *) iptr));
															std::cout<< ival<<"  ";
															break;

					case HashStringToInt("precision"):		iptr =sql.fetch( row, nf);
															ival = ntohl(*((uint32_t *) iptr));
															std::cout<< ival<<"  ";
															break;

					case HashStringToInt("scale"):			iptr =sql.fetch( row, nf);
															ival = ntohl(*((uint32_t *) iptr));
															std::cout<< ival<<"  ";
															break;

					case HashStringToInt("column_comment"):	std::cout<<sql.fetch( row, nf)<<std::endl;   break;

					
					default : std::cout<<sql.cfield(nf)<<std::endl;	break;
				}
				//std::cout<<sql.cfield(nf)<<std::endl;   
				//std::cout<<sql.fetch(row,nf)<<std::endl;
			}



	sql.end();

	printf("\n012 traitement file db2 \n");
	
	sql.begin();

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
 "NBXCOM"=0.00, "NBZIMP"='19X39  ETQ PF BOB 4000'
WHERE "NBNDOS"=21071110 AND "NBCGIM"='S' AND "NBCDPO"=21 AND "NBCSTD"='RDS19X39';

*/

/// select table n° order , prix , name of the print
	requete = sql.prepare("SELECT \"NBNDOS\", \"NBMPRV\", \"NBZIMP\" FROM  public.\"FNBDOS\" WHERE \"NBNDOS\" = '?'",21071110);
	std::cout<<requete<<std::endl;

// mprv = 6.95
/// fetch all 
	sql.fetchAll(requete, cursorName);
	if ( !fetch0)
	{
		printf("-- %s --\n", "FNBDOS");

		for (int row = 0; row < PQntuples(sql.res); row++)
		{

			for (int col = 0; col < PQnfields(sql.res); col++)
			{
				std::cout<<sql.cfield(col)<<"  ";
				std::cout<< PQgetvalue(sql.res, row, col) <<std::endl;
			}
		}
	}

	sql.end();




	printf("\n013 traitement field db2 \n");

	Zdcml NBNDOS(8,0);
	Zdcml NBMPRV(8,2);
	Zchar NBZIMP(30);


	NBNDOS = 15;
	NBMPRV = 78.3;
	NBZIMP = "bonjour" ;


	std::cout<<NBNDOS.ToChar()<<std::endl;  std::cout<<NBMPRV.ToChar()<<std::endl; std::cout<<NBZIMP.ToChar()<<std::endl;

	NBMPRV =16.95 ; NBNDOS=21071110;

	requete = sql.prepare("UPDATE public.\"FNBDOS\"  SET \"NBMPRV\" = ?  WHERE \"NBNDOS\" = '?'",NBMPRV,NBNDOS);

	std::cout<<requete<<std::endl;
     
	sql.query(requete);

	printf("OK\n");

	sql.commit();

	sql.end();



	printf("\n014 traitement display field db2 \n");

	requete = sql.prepare("SELECT \"NBNDOS\", \"NBMPRV\", \"NBZIMP\" FROM  public.\"FNBDOS\" WHERE \"NBNDOS\" = '?'",21071110);
	std::cout<<requete<<std::endl;

	sql.begin();
	sql.opensql(requete, cursorName);
	do
	{
		if ( fetchEOF ==false )
			for (int row = 0; row < sql.rows; row++)
			{
				for (int col = 0; col < sql.cols; col++)
				{
					std::cout<<sql.cfield(col)<<" -row :"<<row<< " -col :"<< col<<"    ";
					std::cout<< PQgetvalue(sql.res, row, col) <<std::endl;
				}
			}
		sql.fechsql(cursorName);
	}	while  (fetchEOF ==false) ;

	sql.end();

	printf("\n015 traitement  update field db2 \n");
	NBMPRV =6.95 ; NBNDOS=21071110;

	requete = sql.prepare("UPDATE public.\"FNBDOS\"  SET \"NBMPRV\" = ?  WHERE \"NBNDOS\" = '?'",NBMPRV,NBNDOS);

	std::cout<<requete<<std::endl;
     
	sql.query(requete);
	
	sql.commit();
	
	sql.end();



	printf("\n016 traitement istream field db2  avec cursor name default\n");
	
	requete = sql.prepare("SELECT \"NBNDOS\", \"NBMPRV\", \"NBZIMP\" FROM  public.\"FNBDOS\" WHERE \"NBNDOS\" = '?'",21071110);
	std::cout<<requete<<std::endl;
	

	sql.begin();
	
	sql.opensql(requete);
	do
	{
		if ( fetchEOF ==false ) sql.result()>>NBNDOS>>NBMPRV>>NBZIMP;

		std::cout<<NBNDOS<<"    ";  std::cout<<NBMPRV<<"    "; std::cout<<NBZIMP<<std::endl;
		
		if ( fetchEOF ==false ) sql.fechsql();
 	}	while  (fetchEOF ==false) ;
	

	sql.end();



 

 
/*
CREATE TABLE public.typetable (
	vdate date NULL,
	vnumeric numeric(8,2) NULL,
	vtext text NULL,
	vonchar bpchar(1) NULL,
	vtime timetz NULL,
	vtimestamp timestamp NULL,
	heure time NULL,
	vkey numeric(7) NOT NULL,
	CONSTRAINT typetable_pk PRIMARY KEY (vkey),
	CONSTRAINT typetable_un UNIQUE (vkey)
)
WITH (
	OIDS=FALSE
) ;
INSERT INTO public.typetable
(vdate, vnumeric, vtext, vonchar, vtime, vtimestamp, heure, vkey)
VALUES('2051-10-12', 345678.09, 'MON NOM LAROCHE', 'C', '12:10:01', '1951-10-12 10:01:00.000', '13:12:10', 1);

INSERT INTO public.typetable
(vdate, vnumeric, vtext, vonchar, vtime, vtimestamp, heure, vkey)
VALUES('2051-10-12', 345678.09, 'MON NOM LAROCHE', 'C', '11:10:01', '1951-10-12 10:01:00.000', '13:12:10', 3681210);


*/
																	

	printf("\n016 traitement non  ZONED \n");
 


 
	char *	vdate = new char[8];
	double Ndouble =0;
	char* Nchar = new char[30];
	int Nint=0 ;


	sql.begin();
	requete = sql.prepare("SELECT vdate, vnumeric, vtext , vkey FROM  typetable WHERE vkey = ?", 3681210);
	std::cout<<requete<<std::endl;

 
	sql.fetchAll(requete, cursorName);
 
    for (int row = 0; row < sql.countrow() && fetchEOF ==false ; row++)
    {
		for (int nf = 0;nf < sql.countfield(); nf++)
			{ 

				switch (HashStringToInt(sql.cfield(nf)))
				{
					case HashStringToInt(NAMEOF(vdate)):	vdate	=	sql.fetch( row, nf);   break; //exemple avec le Nom de la Variable
					
					case HashStringToInt("vnumeric"):		Ndouble	= 	sql.fetchDbl( row, nf);   break;
					
					case HashStringToInt("vtext"):			Nchar	=	sql.fetch( row, nf);   break;
					
					case HashStringToInt("vkey"):			Nint	=	sql.fetchInt( row, nf);
															 

															break;
					
					default : std::cout<<sql.cfield(nf)<<std::endl;	break;
				}
				//std::cout<<sql.cfield(nf)<<std::endl;   
				//std::cout<<sql.fetch(row,nf)<<std::endl;

			}
		std::cout <<"NAMEOF(vdate)  "<< vdate <<" -- vnumeric "<<std::setprecision(8)<<Ndouble<<" -- vtext  "<<Nchar<<"  -- vkey  "<<Nint<<std::endl;
	}


	sql.end();


	printf("\n016 traitement non  ZONED  istream \n");

	requete = sql.prepare("SELECT vdate, vnumeric, vtext , vkey FROM  typetable WHERE vkey = ?", 3681210);
	std::cout<<requete<<std::endl;

	sql.begin();
	sql.opensql(requete, cursorName);
	do
	{
		if ( fetchEOF ==false )  sql.result()>>vdate>>Ndouble>>Nchar>>Nint;

			std::cout <<"NAMEOF(vdate)  "<< vdate <<" -- vnumeric "<<std::setprecision(8)<<Ndouble<<" -- vtext  "<<Nchar<<"  -- vkey  "<<Nint<<std::endl;
			
		if ( fetchEOF ==false ) sql.fechsql(cursorName);
 	}while  (fetchEOF ==false) ;	

	sql.end();




	printf("\n017 traitement double entrer \n");
	requete = sql.prepare("UPDATE typetable  SET vnumeric = '?'  WHERE vkey = ?",36.04, 3681210);
	sql.begin();
	sql.query(requete);
	sql.commit();
	sql.end();

	double Xdouble = std::stod(NBNDOS.ToChar());
	std::cout<<std::setprecision(NBNDOS.clen())<<Xdouble<<"   clen : "<<NBNDOS.clen()<<std::endl;




	printf("\n018 traitement not ZONED istream  ZONED ostream \n");
	Zdate   zDate;
	Zdcml   zNumeric(8,2);
	Ztext   zText ;
	Zdcml   zKey(7,0);


	requete = sql.prepare("SELECT vdate, vnumeric, vtext , vkey FROM  typetable WHERE vkey = ?", 3681210);

     std::cout<<requete<<std::endl;

     
	sql.begin();
	sql.opensql(requete, cursorName);
	do
	{
		if ( fetchEOF ==false )  sql.result()>>zDate>>zNumeric>>zText>>zKey;

		std::cout << zDate <<" -- "<<zNumeric<<" -- "<<zText<<"  --  "<<zKey<<std::endl;
		
		if ( fetchEOF ==false ) sql.fechsql(cursorName);
 	}while  (fetchEOF ==false) ;

	sql.end();



	printf("\n019 traitementrequtte avec des doubles \n");
	
	int Nkey = 3681210;
	Ndouble = 345678.09;
	std::cout<<std::setprecision(10)<<Ndouble<<" affiche une double"<<std::endl;


	requete = sql.prepare("UPDATE typetable  SET vnumeric = ?  WHERE vkey = ?",sql.DoubleToChar(Ndouble,2), sql.DoubleToChar(Nkey) );
	std::cout<<requete<<std::endl;
	
	sql.begin();
	sql.query(requete);

	sql.commit();
	sql.end();

	

	std::cout << "Appuyez sur ENTER pour quitter";    std::cin.ignore (std::cin.rdbuf () -> in_avail () + 1);
	
	printf("sql.closeDB \n");
	sql.closeDB();


} catch (const std::exception& e)
{
	  std::cerr<<e.what()<<"fin de procedure catch"<<std::endl;
	  return EXIT_FAILURE ;
 }


std::cout<<"fin de programme  EXIT_SUCCESS "<<std::endl;


   return EXIT_SUCCESS ; 
}


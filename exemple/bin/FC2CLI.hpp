#include <ZONED.hpp> 
/// INIT-FIELD 

#ifndef  C2CDEP_HPP_INCLUDED
#define  C2CDEP_HPP_INCLUDED
	 Zchar C2CDEP(3);	//	CODE DEPARTEMENT
#endif
#ifndef  C2CEXT_HPP_INCLUDED
#define  C2CEXT_HPP_INCLUDED
	 Zchar C2CEXT(4);	//	CODE TRT EXTERNE
#endif
#ifndef  C2CPAY_HPP_INCLUDED
#define  C2CPAY_HPP_INCLUDED
	 Zchar C2CPAY(3);	//	CODE PAYS
#endif
#ifndef  C2CRGI_HPP_INCLUDED
#define  C2CRGI_HPP_INCLUDED
	 Zdcml C2CRGI(2,0);	//	CODE REGION ECONOMIQ
#endif
#ifndef  C2CST1_HPP_INCLUDED
#define  C2CST1_HPP_INCLUDED
	 Zdcml C2CST1(2,0);	//	CODE REGION  STAT.
#endif
#ifndef  C2CST2_HPP_INCLUDED
#define  C2CST2_HPP_INCLUDED
	 Zdcml C2CST2(2,0);	//	CODE SECTEUR STAT.
#endif
#ifndef  C2CTRP_HPP_INCLUDED
#define  C2CTRP_HPP_INCLUDED
	 Zdcml C2CTRP(3,0);	//	CODE SECTEUR TRANSP.
#endif
#ifndef  C2CTTR_HPP_INCLUDED
#define  C2CTTR_HPP_INCLUDED
	 Zchar C2CTTR(1);	//	CODE TARIF  TRANSP.
#endif
#ifndef  C2999E_HPP_INCLUDED
#define  C2999E_HPP_INCLUDED
	 Zdate C2999E;	//	DATE DE CREATION ENR
#endif
#ifndef  C2999M_HPP_INCLUDED
#define  C2999M_HPP_INCLUDED
	 Zdate C2999M;	//	DATE DE MODIFCATION
#endif
#ifndef  C2ECLI_HPP_INCLUDED
#define  C2ECLI_HPP_INCLUDED
	 Zchar C2ECLI(10);	//	N° CLIENT        EXT
#endif
#ifndef  C2ECLX_HPP_INCLUDED
#define  C2ECLX_HPP_INCLUDED
	 Zchar C2ECLX(5);	//	CLIENT N°INDEX   EXT
#endif
#ifndef  C2NARR_HPP_INCLUDED
#define  C2NARR_HPP_INCLUDED
	 Zdcml C2NARR(3,0);	//	N. ARRONDISSEMENT
#endif
#ifndef  C2NCLI_HPP_INCLUDED
#define  C2NCLI_HPP_INCLUDED
	 Zdcml C2NCLI(6,0);	//	N° CLIENT
#endif
#ifndef  C2NCLX_HPP_INCLUDED
#define  C2NCLX_HPP_INCLUDED
	 Zdcml C2NCLX(4,0);	//	CLIENT N°INDEX  BASE
#endif
#ifndef  C2NLI1_HPP_INCLUDED
#define  C2NLI1_HPP_INCLUDED
	 Zdcml C2NLI1(4,0);	//	SEQ CLIENT LIVRAIS.
#endif
#ifndef  C2NPTT_HPP_INCLUDED
#define  C2NPTT_HPP_INCLUDED
	 Zdcml C2NPTT(5,0);	//	N. CODE POSTAL
#endif
#ifndef  C2NLIV_HPP_INCLUDED
#define  C2NLIV_HPP_INCLUDED
	 Zdcml C2NLIV(6,0);	//	N. NBR. LIVRAISON
#endif
#ifndef  C2SHS_HPP_INCLUDED
#define  C2SHS_HPP_INCLUDED
	 Zdcml C2SHS(1,0);	//	0/1  N=0 O=1 H.SERVC
#endif
#ifndef  C2SMLG_HPP_INCLUDED
#define  C2SMLG_HPP_INCLUDED
	 Zchar C2SMLG(1);	//	O/N MAILING
#endif
#ifndef  C2ZPTT_HPP_INCLUDED
#define  C2ZPTT_HPP_INCLUDED
	 Zchar C2ZPTT(10);	//	Z. CODE POSTAL
#endif
#ifndef  C2ZCND_HPP_INCLUDED
#define  C2ZCND_HPP_INCLUDED
	 Zchar C2ZCND(30);	//	CONDITIONNEMENT SPEC
#endif
#ifndef  C2ZCTC_HPP_INCLUDED
#define  C2ZCTC_HPP_INCLUDED
	 Zchar C2ZCTC(30);	//	PERSONNE A CONTACTER
#endif
#ifndef  C2ZDST_HPP_INCLUDED
#define  C2ZDST_HPP_INCLUDED
	 Zchar C2ZDST(30);	//	PERSONNE DESTINATAIR
#endif
#ifndef  C2ZNOM_HPP_INCLUDED
#define  C2ZNOM_HPP_INCLUDED
	 Zchar C2ZNOM(30);	//	NOM / RAISON SOCIAL
#endif
#ifndef  C2ZOBS_HPP_INCLUDED
#define  C2ZOBS_HPP_INCLUDED
	 Zchar C2ZOBS(30);	//	OBSERVATIONS
#endif
#ifndef  C2ZOB3_HPP_INCLUDED
#define  C2ZOB3_HPP_INCLUDED
	 Zchar C2ZOB3(30);	//	OBSERVATIONS LIV
#endif
#ifndef  C2ZOB4_HPP_INCLUDED
#define  C2ZOB4_HPP_INCLUDED
	 Zchar C2ZOB4(30);	//	OBSERVATIONS LIV
#endif
#ifndef  C2ZRU1_HPP_INCLUDED
#define  C2ZRU1_HPP_INCLUDED
	 Zchar C2ZRU1(30);	//	NOM / RUE1
#endif
#ifndef  C2ZRU2_HPP_INCLUDED
#define  C2ZRU2_HPP_INCLUDED
	 Zchar C2ZRU2(30);	//	SUITE NOM / RUE1
#endif
#ifndef  C2ZTL1_HPP_INCLUDED
#define  C2ZTL1_HPP_INCLUDED
	 Zchar C2ZTL1(15);	//	N. TEL. PERS. A CONT
#endif
#ifndef  C2ZTLX_HPP_INCLUDED
#define  C2ZTLX_HPP_INCLUDED
	 Zchar C2ZTLX(15);	//	N. TELEX / TELECOPIE
#endif
#ifndef  C2ZVIL_HPP_INCLUDED
#define  C2ZVIL_HPP_INCLUDED
	 Zchar C2ZVIL(25);	//	VILLE
#endif






/// check row key
	bool PG_FC2CLI::check(   Zdcml k_C2NCLI , Zdcml k_C2NCLX)
{
try{

	requete= slc.prepare("SELECT count(*) FROM FC2CLI \
	 WHERE (C2NCLI = ? and C2NCLX = ? )",
		k_C2NCLI ,k_C2NCLX  ); 
 
	if ( slc.countqry(requete) > 0 )	return true ;
	return false ;
 
}catch (const std::exception& e)
	{
	  std::cerr<<e.what()<<"Check requete impossible"<<std::endl;
	  return false;
	}
} 



 		 



 		 



 		 

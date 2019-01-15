#include <ZONED.hpp> 
/// INIT-FIELD 

#ifndef  KECSTD_HPP_INCLUDED
#define  KECSTD_HPP_INCLUDED
	 Zchar KECSTD(8);	//	COD_ART._STK/DIVERS
#endif
#ifndef  KECDPO_HPP_INCLUDED
#define  KECDPO_HPP_INCLUDED
	 Zdcml KECDPO(2,0);	//	CODE DEPOT
#endif
#ifndef  KECGIM_HPP_INCLUDED
#define  KECGIM_HPP_INCLUDED
	 Zchar KECGIM(1);	//	COD_GESTION_IMP/STK
#endif
#ifndef  KE0LVD_HPP_INCLUDED
#define  KE0LVD_HPP_INCLUDED
	 Zdate KE0LVD;	//	DATE LIV. DEMANDE
#endif
#ifndef  KE0LVR_HPP_INCLUDED
#define  KE0LVR_HPP_INCLUDED
	 Zdate KE0LVR;	//	DATE LIV. REEL
#endif
#ifndef  KE999E_HPP_INCLUDED
#define  KE999E_HPP_INCLUDED
	 Zdate KE999E;	//	DATE DE CREATION ENR
#endif
#ifndef  KE999M_HPP_INCLUDED
#define  KE999M_HPP_INCLUDED
	 Zdate KE999M;	//	DATE DE MODIFCATION
#endif
#ifndef  KENBL_HPP_INCLUDED
#define  KENBL_HPP_INCLUDED
	 Zdcml KENBL(6,0);	//	N° BON LIVRAISON
#endif
#ifndef  KENCLI_HPP_INCLUDED
#define  KENCLI_HPP_INCLUDED
	 Zdcml KENCLI(6,0);	//	N° CLIENT
#endif
#ifndef  KENCLX_HPP_INCLUDED
#define  KENCLX_HPP_INCLUDED
	 Zdcml KENCLX(4,0);	//	CLIENT N°INDEX  BASE
#endif
#ifndef  KENCLY_HPP_INCLUDED
#define  KENCLY_HPP_INCLUDED
	 Zdcml KENCLY(4,0);	//	CLIENT INDEX /INDEX
#endif
#ifndef  KENDOS_HPP_INCLUDED
#define  KENDOS_HPP_INCLUDED
	 Zdcml KENDOS(8,0);	//	N.CHRONO DOSSIER
#endif
#ifndef  KEQLVD_HPP_INCLUDED
#define  KEQLVD_HPP_INCLUDED
	 Zdcml KEQLVD(10,0);	//	QTE_LIVREE_DEMANDEE
#endif
#ifndef  KEQLVR_HPP_INCLUDED
#define  KEQLVR_HPP_INCLUDED
	 Zdcml KEQLVR(10,0);	//	QTE_LIVREE_REELLE
#endif
#ifndef  KEZLBA_HPP_INCLUDED
#define  KEZLBA_HPP_INCLUDED
	 Zchar KEZLBA(30);	//	LIBELLE  ..30
#endif
#ifndef  KEZWART_HPP_INCLUDED
#define  KEZWART_HPP_INCLUDED
	 Zchar KEZWART(20);	//	REF ARTICLE WEB
#endif
#ifndef  KEZWRF_HPP_INCLUDED
#define  KEZWRF_HPP_INCLUDED
	 Zchar KEZWRF(30);	//	REF DE CDE  WEB
#endif






/// check row key
	bool PG_FKEDOS::check(   Zdcml k_KENDOS , Zdate k_KE0LVD , Zdcml k_KENCLI , Zdcml k_KENCLX , Zdcml k_KENCLY , Zchar k_KECGIM , Zdcml k_KECDPO , Zchar k_KECSTD)
{
try{

	requete= slc.prepare("SELECT count(*) FROM FKEDOS \
	 WHERE (KENDOS = ? and KE0LVD = '?'  and KENCLI = ? and KENCLX = ? and KENCLY = ? and KECGIM = '?'  and KECDPO = ? and KECSTD = '?'  )",
		k_KENDOS ,k_KE0LVD ,k_KENCLI ,k_KENCLX ,k_KENCLY ,k_KECGIM ,k_KECDPO ,k_KECSTD  ); 
 
	if ( slc.countqry(requete) > 0 )	return true ;
	return false ;
 
}catch (const std::exception& e)
	{
	  std::cerr<<e.what()<<"Check requete impossible"<<std::endl;
	  return false;
	}
} 



 		 



 		 



 		 

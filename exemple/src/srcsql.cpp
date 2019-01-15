#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>


#include <errno.h>
#include <signal.h>

#include <exception>
#include <X11/Xlib.h>

#include <regex>
#include <array>
#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <gdk/gdkkeysyms.h>


/// ***************************************
/// ctemplate wrapper 
/// ***************************************
#include <ctemplate/template.h>
#include <list>



 
class field{
        public:
              std::string	column_name ;
              std::string	column_ordre;
              std::string	column_type;
              std::string	column_length;
              std::string	column_precision;
              std::string	column_scale;
              std::string	column_comment;
};
std::list<field> user_list;
field DDS;


class fieldk{
        public:
              std::string	column_name ;
              std::string	column_ordre;
              std::string	column_type;
};
std::list<fieldk> user_listk;
fieldk DDSk;

/// ***************************************
/// sql PostgreSql wrapper 
/// ***************************************

#include <libpqwrp.cxx>


	const char* conninfo;

	libPQwrp slc;

	std::string requete ;
	
	std::string cursorName = "MYcursor";

/// ***************************************
/// definition des objets GTK
/// ***************************************

GtkBuilder *builder = NULL;

// DIALOG choix database 

GtkWidget *WIN01 = NULL;
GtkWidget  *eBOX1;     // conteneur des zones à saisir
GtkEntry   *eDATABASE;
GtkEntry   *eUSER;
GtkEntry   *ePASSWORD;

GtkLabel   *tMsg_Name ;
GtkLabel   *tMsg_text ;

GtkButton  *bEXIT ;
GtkButton  *bCONECT ;

GtkLabel   *eTABLE;
GtkLabel   *eTEXTE;
GtkLabel   *eBASE;


// APPWIN choix table
GtkWidget *WIN02 = NULL;
GtkWidget  *ACTIONBOX2; // conteneur des boutons

GtkTreeView  *view;
GtkListStore *store;
GtkTreeIter   iter;


GtkTreePath * path ;
int * indice;


GtkTreeSelection *selectd;

const gchar * v_nom ;
const gchar * v_text ;

const gchar * sfl_ligne;
const gchar * sfl_first;
const gchar * sfl_end;
const gchar * sfl_recherche;
const gchar  c_esp = ' ';

enum
{
    NOM_COLUMN= 0,
    TEXT_COLUMN
};


GtkButton  *bSLCT ;
GtkButton  *bRTRN ;
GtkButton  *bRUN ;
GtkButton  *bQUIT ;



/// Nom des formats utilisés
enum gFORMAT {Fmt01=1};



/* Prototype for selection handler callback   GRID */
static void tree_selection_cb (GtkWidget *bouton,GtkTreeSelection *selection);
/* Annule selection*/
static void Annuler_Selection();
/* Prototype for selection handler callback  contrôle field value */
static void Validation_Connection();
/* genere la description des fields */
static void Generator_Field();

int  exec_prog(const char* commande);


/// fonction type 5250 pour GTK
#include "C5250.hpp"


int main (int argc, char *argv[])
{


	// ---------------------------------------------------------------------------------------------------------

	/// GTK CONTROLE

	/* Initialisation de la librairie Gtk. */
	gtk_init(&argc, &argv);

	/* Ouverture du fichier Glade de la Fenêtre principale */
	builder = gtk_builder_new();

	/// include special file contenant le fichier glade mise au format gtk3  
	#include "srcsql.hpp"

	/* Récupération du pointeur de la fenêtre principale */
	WIN01 = GTK_WIDGET(gtk_builder_get_object (builder, "DIALOG"));

   
	gtk_window_set_resizable(GTK_WINDOW(WIN01), FALSE);

	eBOX1	= GTK_WIDGET( gtk_builder_get_object( builder, "eBOX1" ) );
	eDATABASE	= GTK_ENTRY(  gtk_builder_get_object( builder, "eDATABASE" ) );	// Name de la database
	eUSER		= GTK_ENTRY(  gtk_builder_get_object( builder, "eUSER" ) );	// texte du user
	ePASSWORD	= GTK_ENTRY(  gtk_builder_get_object( builder, "ePASSWORD" ) );	// texte du password
	/* buffer message-Info */
	tMsg_Name	= GTK_LABEL(  gtk_builder_get_object( builder, "tMsg_Name" ) );	// name de la field
	tMsg_text	= GTK_LABEL(  gtk_builder_get_object( builder, "tMsg_text" ) );	// message help or error

	/* button  Action */
	bEXIT		= GTK_BUTTON(  gtk_builder_get_object( builder, "bEXIT" ) );	// Button exit
	bCONECT		= GTK_BUTTON(  gtk_builder_get_object( builder, "bCONECT" ) );	// button validation

	// button  DIALOGUE 

	/* Signal du bouton Quit */
	g_signal_connect(bEXIT,"clicked", G_CALLBACK (Main_Quit), NULL);

    /* Signal du bouton Valide */
	g_signal_connect(bCONECT,"clicked", G_CALLBACK (Validate), eBOX1);

	/* Signal ALT+F4 */
	g_signal_connect(WIN01,"delete_event", G_CALLBACK (key_press_ALTF4), NULL);

	///=======================================================================================
	// initalisation des buffer pour  DIALOG du file DDS.db pour C5250 (contrôleur de zone)
	UseLng =FR ;
	DSPFmt =Fmt01;
	InitFmt(Fmt01,3 );
	Alpha(Fmt01,0,eDATABASE,_TYPE_FULL, 20,true,"");
	Alpha(Fmt01,1,eUSER,_TYPE_FULL, 20,true,"");
	Alpha(Fmt01,2,ePASSWORD,_TYPE_FULL, 20,true,"");
	///=======================================================================================

	WIN02 = GTK_WIDGET(gtk_builder_get_object (builder, "APPWIN"));
   
	gtk_window_set_resizable(GTK_WINDOW(WIN02), FALSE);

	/* buffer message-Info */
	eTABLE	= GTK_LABEL(  gtk_builder_get_object( builder, "eTABLE" ) );	// name de table
	eTEXTE	= GTK_LABEL(  gtk_builder_get_object( builder, "eTEXTE" ) );	// texte de la table
	eBASE	= GTK_LABEL(  gtk_builder_get_object( builder, "eBASE" )  );	// nom de la database	

	ACTIONBOX2	= GTK_WIDGET( gtk_builder_get_object( builder, "ACTIONBOX2") );
	// Récupération du pointeur du Grid

	// mise en place du Grid
	view		= GTK_TREE_VIEW(gtk_builder_get_object (builder, "TREEVIEW"));

	store		= GTK_LIST_STORE(gtk_builder_get_object (builder, "SFL01"));

	selectd		= gtk_tree_view_get_selection (GTK_TREE_VIEW (view));
    
	gtk_tree_selection_set_mode (selectd, GTK_SELECTION_SINGLE);

	/* button  Action */
	bQUIT		= GTK_BUTTON(  gtk_builder_get_object( builder, "bQUIT" ) );	// Button exit
	bRUN	= GTK_BUTTON(  gtk_builder_get_object( builder, "bRUN" ) );	// button execute generateur src
	bSLCT		= GTK_BUTTON(  gtk_builder_get_object( builder, "bSLCT" ) );	// button SLCT
	bRTRN		= GTK_BUTTON(  gtk_builder_get_object( builder, "bRTRN" ) );	// button RETURN
	
//	gtk_widget_show_all(WIN02);
//    gtk_widget_hide_on_delete((WIN01);
	/* Récupération du pointeur de la fenêtre principale */
   

	/* Signal du bouton Quit */
	g_signal_connect(bQUIT,"clicked", G_CALLBACK (Main_Quit), NULL);
	/* Signal du bouton Annuler */
	g_signal_connect(bRTRN,"clicked", G_CALLBACK (Annuler_Selection), ACTIONBOX2);
    /* Signal du bouton Executer*/
	g_signal_connect(bRUN,"clicked", G_CALLBACK (Generator_Field), ACTIONBOX2);

	/* gestion des signaux */
	g_signal_connect(bSLCT,"clicked", G_CALLBACK (tree_selection_cb),selectd);
	
	/* Signal ALT+F4 */
	g_signal_connect(WIN02,"delete_event", G_CALLBACK (key_press_ALTF4), NULL);
	/* Affichage de la fenêtre principale. */

	gtk_widget_show_all (WIN01);

	gtk_main();

	return EXIT_SUCCESS;
}

/// ici on ferme les fichiers  on fait roolback ect....
void Procedure_end_Pgm()
{

}


///=====================================================================

static void
tree_selection_cb (GtkWidget *bouton,GtkTreeSelection *_selection)
{
		 

	GtkTreeIter _iter;
	GtkTreeModel *_model;
	gchar *_nom, *_text;
	if (gtk_tree_selection_get_selected (_selection, &_model, &_iter))
	{

///--------- recuperation positin cursor
		path= gtk_tree_model_get_path (_model, &_iter ) ;
		indice= gtk_tree_path_get_indices ( path );
///-----------------------------------------------

		gtk_tree_model_get (_model, &_iter, NOM_COLUMN, &_nom, -1);
		gtk_label_set_text (eTABLE,_nom);   
		g_free (_nom);

		gtk_tree_model_get (_model, &_iter, TEXT_COLUMN, &_text, -1);
		gtk_label_set_text (eTEXTE,_text); 
		g_free (_text);

    }

}

///=====================================================================
static void
Annuler_Selection()
{
		gtk_entry_set_text(eUSER,"");
		gtk_entry_set_text(ePASSWORD,"");
		gtk_widget_show_all(WIN01);
		gtk_widget_hide_on_delete(WIN02);
}
///=====================================================================
static void
Validation_Connection()
{

	gchar *_nom, *_text ;

	gtk_list_store_clear (store);

	std::stringstream con ;
	
	con << " host=localhost  port=5432  dbname="<<(char*) gtk_entry_get_text(eDATABASE)<<\
	"  user="<<(char*) gtk_entry_get_text(eUSER) <<"  password="<<(char*) gtk_entry_get_text(ePASSWORD)<<"  application_name=srcsql"<< std::endl;

	std::string constr = con.str();
	conninfo = constr.c_str();

	try
	{
 		gtk_label_set_label (eTABLE,"" );
		gtk_label_set_label (eTEXTE,"" );
		gtk_label_set_label (eBASE,gtk_entry_get_text(eDATABASE));
		
		slc.connectDB(conninfo);
		slc.autocommitOFF();
		slc.begin();
		requete = slc.prepare(\
	"	select  cl.table_catalog ,cl.table_name ,d.description \
	from     information_schema.columns cl , pg_catalog.pg_attribute a inner join pg_catalog.pg_class c on a.attrelid = c.oid, pg_catalog.pg_description d \
	where cl.table_catalog ='?'  and cl.table_name  = c.relname  and d.objoid = a.attrelid  and d.objsubid = 0 and cl.ORDINAL_POSITION = 1 \
	and cl.ORDINAL_POSITION = a.attnum; " ,(char*) gtk_entry_get_text(eDATABASE));

		
		slc.fetchAll(requete, cursorName);

		if ( !slc.fetchEOF)
		{
			for (int row = 0; row < PQntuples(slc.res); row++)
			{
							_nom = PQgetvalue(slc.res, row, 1);
							_text = PQgetvalue(slc.res, row, 2);
							gtk_list_store_append(store, &iter);
							 
							gtk_list_store_set(	store, &iter,
							NOM_COLUMN, _nom,
							TEXT_COLUMN,_text,
							-1);
			}
		}
	
		gtk_widget_show_all(WIN02);
		gtk_widget_hide_on_delete(WIN01);
	slc.end();
	}
	catch (const std::exception& e)
	{
		Msg_clr();
		printMsg("base de donnée inconnue ou password invalide");
		Main_Quit(); 
	}
}

void Generator_Field()
{

	std::string	type_time;
	std::string	column_table	="";
	std::string	column_name		="";
	int			column_ordre	=0;
	std::string	column_type		="";
	int			column_length	=0;
	int			column_precision	=0;
	int			column_scale	=0;
 	std::string	column_comment	="";
//	char *	column_comment ;


  static const char template_text_FRMWRK[] =
"{{#HPP000}}"\
"#include <ZONED.hpp> \n"\
"/// INIT-FIELD \n"\
"\n"\
"{{/HPP000}}"\
"{{#HPP01}}"\
"#ifndef  {{ZONE0}}_HPP_INCLUDED\n"\
"#define  {{ZONE0}}_HPP_INCLUDED\n"\
"	{{ZONE}};	//	{{ZONETEXT}}\n"\
"#endif\n"\
"{{/HPP01}}"\
"\n"\
"\n"\
"\n"\
"{{#CHK0}}"\
"\n"\
"\n"\
"\n"\
"/// check row key\n"\
"	bool PG_{{TABLE}}::check( {{/CHK0}} {{#CHKP}}{{ZPARM}}{{/CHKP}}{{#CHK1}})\n"\
"{\n"\
"try{\n"\
"\n"\
"	requete= slc.prepare(\"SELECT count(*) FROM {{TABLE}} \\\n"\
"	 WHERE ({{/CHK1}}{{#CHKW}}{{ZWHERE}}{{/CHKW}}{{#CHK2}} )\",\n{{/CHK2}}\
		{{#CHKV}}{{ZVAL}}{{/CHKV}} {{#CHK3}} ); \n"\
" \n"\
"	if ( slc.countqry(requete) > 0 )	return true ;\n"\
"	return false ;\n"\
" \n"\
"}catch (const std::exception& e)\n"\
"	{\n"\
"	  std::cerr<<e.what()<<\"Check requete impossible\"<<std::endl;\n"\
"	  return false;\n"\
"	}\n"\
"} {{/CHK3}}\n"\
"\n"\
"\n"\
"\n"\
"{{#INS0}}"\
"\n"\
"\n"\
"\n"\
"/// INSERT row key\n"\
"	bool PG_{{TABLE}}::insert()\n"\
"{\n"\
"try{\n"\
"\n"\
"	requete= slc.prepare(\"INSERT INTO {{TABLE}} \\\n"\
"	({{/INS0}} {{#INSZ}}{{ZONE}}{{/INSZ}}{{#INS1}})\n"\
"	 VALUE ({{/INS1}}{{#INSW}}{{ZWHERE}}{{/INSW}}{{#INS2}} )\",\n{{/INS2}}\
		{{#INSV}}{{ZVAL}}{{/INSV}} {{#INS3}} ); \n"\
" \n"\
"	sql.query(requete);\n"\
"	return true ;\n"\
" \n"\
"}catch (const std::exception& e)\n"\
"	{\n"\
"	  std::cerr<<e.what()<<\"Insert requete impossible\"<<std::endl;\n"\
"	  return false;\n"\
"	}\n"\
"} {{/INS3}}\n"\
"\n"\
"\n"\
"\n"\
"{{#RMV0}}"\
"\n"\
"\n"\
"\n"\
"/// DELETE row key\n"\
"	bool PG_{{TABLE}}::remove( {{/RMV0}} {{#RMVP}}{{ZPARM}}{{/RMVP}}{{#RMV1}})\n"\
"{\n"\
"try{\n"\
"\n"\
"	requete= slc.prepare(\"DELETE FROM {{TABLE}} \\\n"\
"	 WHERE ({{/RMV1}}{{#RMVW}}{{ZWHERE}}{{/RMVW}}{{#RMV2}} )\",\n{{/RMV2}}\
		{{#RMVV}}{{ZVAL}}{{/RMVV}} {{#RMV3}} ); \n"\
" \n"\
"	sql.query(requete);\n"\
"	return true ;\n"\
" \n"\
"}catch (const std::exception& e)\n"\
"	{\n"\
"	  std::cerr<<e.what()<<\"Remove requete impossible\"<<std::endl;\n"\
"	  return false;\n"\
"	}\n"\
"} {{/RMV3}}\n"\
"\n"\
"\n"\
"\n"\
"{{#UPD0}}"\
"\n"\
"\n"\
"\n"\
"/// DELETE row key\n"\
"	bool PG_{{TABLE}}::remove( {{/UPD0}} {{#UPDP}}{{ZPARM}}{{/UPDP}}{{#UPD1}})\n"\
"{\n"\
"try{\n"\
"\n"\
"	requete= slc.prepare(\"DELETE FROM {{TABLE}} \\\n"\
"	 WHERE ({{/UPD1}}{{#UPDW}}{{ZWHERE}}{{/UPDW}}{{#UPD2}} )\",\n{{/UPD2}}\
		{{#UPDV}}{{ZVAL}}{{/UPDV}} {{#UPD3}} ); \n"\
" \n"\
"	sql.query(requete);\n"\
"	return true ;\n"\
" \n"\
"}catch (const std::exception& e)\n"\
"	{\n"\
"	  std::cerr<<e.what()<<\"Remove requete impossible\"<<std::endl;\n"\
"	  return false;\n"\
"	}\n"\
"} {{/UPD3}}\n";
	std::stringstream sqlx ;

	
	char  OUTFILE[256];
	char  buffer [2048] ;
 
	try
	{

	user_list.clear();
	user_listk.clear();




///----------------------------------------
/// RECUPE DEF TABLE  FIELD  
///----------------------------------------
	requete = slc.prepare(\
"SELECT "\
" cl.column_name,"\
" cl.ORDINAL_POSITION,"\
" cl.DATA_TYPE,"\
" cl.CHARACTER_MAXIMUM_LENGTH,"\
" cl.NUMERIC_PRECISION,"\
" cl.NUMERIC_SCALE ,"\
" (select pg_catalog.col_description(oid,cl.ordinal_position::int) "\
" from pg_catalog.pg_class c where c.relname=cl.table_name) as column_comment "\
" FROM information_schema.columns cl "\
" WHERE cl.table_catalog='?'  and cl.table_name='?' "\
"  order by 2 ; " ,(char*) gtk_entry_get_text(eDATABASE) ,(char*) gtk_label_get_text(eTABLE) );
	slc.begin();
	slc.opensql(requete, cursorName); 	printMsg("Generator_Field");
	if ( !slc.errorSQL ) do
	{
		if ( ! slc.fetchEOF )
		{
			sqlx = slc.result();

			sqlx>>column_name>>column_ordre>>column_type>>column_length>>column_precision>>column_scale>>column_comment;

			type_time = column_type.substr(0,4); if( !type_time.compare("time") ) column_type="time" ;
			
//			std::cout<<column_name<<"  "<<column_ordre<<"  "<<column_type<<" :"<<column_length<<":  "<<column_precision<<","<<column_scale<<" >>>> "<<column_comment<<std::endl;

			DDS.column_name			= column_name;
			DDS.column_ordre		= std::to_string(column_ordre);
			DDS.column_type			= column_type;
			DDS.column_length		= std::to_string(column_length);
			DDS.column_precision	= std::to_string(column_precision);
			DDS.column_scale		= std::to_string(column_scale);
			DDS.column_comment		= column_comment;
			user_list.push_back(DDS);
			
			slc.fetchsql(cursorName);
		}
 	}while  ( !slc.fetchEOF ) ;	// iteration
	slc.end();
	std::list<field>::iterator i;

///----------------------------------------
/// RECUPE DEF TABLE  FIELD  KEY 
///----------------------------------------

	requete = slc.prepare(\
"SELECT "\
"  kc.table_name, kc.column_name ,kc.ORDINAL_POSITION,cl.DATA_TYPE "\
" FROM "\
"	information_schema.key_column_usage kc,"\
"	information_schema.columns cl"\
" WHERE "\
"	cl.table_catalog='?' and kc.table_name ='?'"\
"	and kc.table_name = cl.table_name"\
"	and kc.column_name = cl.column_name "\
" order by 3;" ,(char*) gtk_entry_get_text(eDATABASE) ,(char*) gtk_label_get_text(eTABLE) );


//std::cout<<requete<<std::endl;
	slc.begin();
	slc.opensql(requete, cursorName); 
	if ( !slc.errorSQL ) do
	{
		if ( ! slc.fetchEOF )
		{
			sqlx = slc.result();

			sqlx>>column_table>>column_name>>column_ordre>>column_type;

			type_time = column_type.substr(0,4); if( !type_time.compare("time") ) column_type="time" ;
			
//			std::cout<<column_name<<"  "<<column_ordre<<"  "<<column_type<<std::endl;


			DDSk.column_name			= column_name;
			DDSk.column_ordre			= std::to_string(column_ordre);
			DDSk.column_type			= column_type;
			user_listk.push_back(DDSk);
			
			slc.fetchsql(cursorName);
		}
 	}while  ( !slc.fetchEOF ) ;	// iteration

	slc.end();
	std::list<fieldk>::iterator k;
	std::list<fieldk>::iterator q;
	q = user_listk.end(); q--;	


///----------------------------------------
/// définition  ZONED
///---------------------------------------- 
	
	ctemplate::StringToTemplateCache("mytpl", template_text_FRMWRK, ctemplate::DO_NOT_STRIP);
 	ctemplate::TemplateDictionary dict ("MODEL1");
	dict.ShowSection("HPP000");

	
	for(i=user_list.begin(); i!=user_list.end(); i++)
		{
            ctemplate::TemplateDictionary*  HPP01_dict = dict.AddSectionDictionary("HPP01");
            HPP01_dict->AddSectionDictionary("HPP01");
            HPP01_dict->SetValue("ZONE0",i->column_name.c_str());
            sprintf(buffer,"%s","");
			switch (HashStringToInt(i->column_type.c_str()))
				{
					case HashStringToInt("character"):
														strcat(buffer, " Zchar ");
														strcat(buffer,i->column_name.c_str());
														strcat(buffer,"(");
														strcat(buffer, i->column_length.c_str());
														strcat(buffer,")");
														HPP01_dict->SetValue("ZONE",buffer);
														HPP01_dict->SetValue("ZONETEXT",i->column_comment.c_str());
														break;

					case HashStringToInt("numeric"):
														strcat(buffer, " Zdcml ");
														strcat(buffer,i->column_name.c_str());
														strcat(buffer,"(");
														strcat(buffer, i->column_precision.c_str());
														strcat(buffer, ",");
														strcat(buffer, i->column_scale.c_str());
														strcat(buffer,")");
														HPP01_dict->SetValue("ZONE",buffer);
														HPP01_dict->SetValue("ZONETEXT",i->column_comment.c_str());
														break;


					case HashStringToInt("date"):
														strcat(buffer, " Zdate ");
														strcat(buffer,i->column_name.c_str());
														HPP01_dict->SetValue("ZONE",buffer);
														HPP01_dict->SetValue("ZONETEXT",i->column_comment.c_str());
														break;
														
					case HashStringToInt("time"):
														strcat(buffer, " Ztime ");
														strcat(buffer,i->column_name.c_str());
														HPP01_dict->SetValue("ZONE",buffer);
														HPP01_dict->SetValue("ZONETEXT",i->column_comment.c_str());
														break;
														
					case HashStringToInt("text"):
														strcat(buffer, " Ztext ");
														strcat(buffer,i->column_name.c_str());
														HPP01_dict->SetValue("ZONE",buffer);
														HPP01_dict->SetValue("ZONETEXT",i->column_comment.c_str());
														break;
														
					case HashStringToInt("boolean"):
														strcat(buffer, " Zbool ");
														strcat(buffer,i->column_name.c_str());
														HPP01_dict->SetValue("ZONE",buffer);
														HPP01_dict->SetValue("ZONETEXT",i->column_comment.c_str());
														break;

														
					}

            HPP01_dict->ShowSection("HPP01");
              ///cout<<buffer<<endl;

		}





///traitement  des fonctions

///----------------------------------------
/// CHECK
///----------------------------------------	


	dict.SetTemplateGlobalValue("TABLE", (char*) gtk_label_get_text(eTABLE));
	dict.ShowSection("CHK0");
	
	sprintf(buffer,"%s","");
	for(k=user_listk.begin(); k!=user_listk.end(); k++)
		{
            ctemplate::TemplateDictionary*  CHKP01_dict = dict.AddSectionDictionary("CHKP");
            CHKP01_dict->AddSectionDictionary("CHKP");
			switch (HashStringToInt(k->column_type.c_str()))
				{
					case HashStringToInt("character"):
														strcat(buffer, " Zchar k_");
														strcat(buffer,k->column_name.c_str());
														CHKP01_dict->SetValue("ZPARM",buffer);
														break;

					case HashStringToInt("numeric"):
														strcat(buffer, " Zdcml k_");
														strcat(buffer,k->column_name.c_str());
														CHKP01_dict->SetValue("ZPARM",buffer);
														break;


					case HashStringToInt("date"):
														strcat(buffer, " Zdate k_");
														strcat(buffer,k->column_name.c_str());
														CHKP01_dict->SetValue("ZPARM",buffer);
														break;
														
					case HashStringToInt("time"):
														strcat(buffer, " Ztime k_");
														strcat(buffer,k->column_name.c_str());
														CHKP01_dict->SetValue("ZPARM",buffer);
														break;
														
					case HashStringToInt("text"):
														strcat(buffer, " Ztext k_");
														strcat(buffer,k->column_name.c_str());
														CHKP01_dict->SetValue("ZPARM",buffer);
														break;
														
					case HashStringToInt("boolean"):
														strcat(buffer, " Zbool k_");
														strcat(buffer,k->column_name.c_str());
														CHKP01_dict->SetValue("ZPARM",buffer);
														break;

														
					}

            CHKP01_dict->ShowSection("CHKP");
              ///cout<<buffer<<endl;
			if (k != q ) sprintf(buffer,"%s"," ,");
		}
				dict.ShowSection("CHK1");

	sprintf(buffer,"%s","");
	for(k=user_listk.begin(); k!=user_listk.end(); k++)
		{
			if (k!= user_listk.begin())  sprintf(buffer,"%s"," and ");
            ctemplate::TemplateDictionary*  CHKW01_dict = dict.AddSectionDictionary("CHKW");
            CHKW01_dict->AddSectionDictionary("CHKW");
			switch (HashStringToInt(k->column_type.c_str()))
				{
					case HashStringToInt("character"):
					case HashStringToInt("date"):
					case HashStringToInt("time"):
					case HashStringToInt("text"):
					case HashStringToInt("boolean"):
														strcat(buffer,k->column_name.c_str());
														strcat(buffer, " = '?' ");
														CHKW01_dict->SetValue("ZWHERE",buffer);
														break;

					case HashStringToInt("numeric"):
														strcat(buffer,k->column_name.c_str());
														strcat(buffer, " = ?");
														CHKW01_dict->SetValue("ZWHERE",buffer);
														break;

														
					}

            CHKW01_dict->ShowSection("CHKW");
              ///cout<<buffer<<endl;
		}
			dict.ShowSection("CHK2");
	sprintf(buffer,"%s","");
	for(k=user_listk.begin(); k!=user_listk.end(); k++)
		{
            ctemplate::TemplateDictionary*  CHKV01_dict = dict.AddSectionDictionary("CHKV");
            CHKV01_dict->AddSectionDictionary("CHKV");
			switch (HashStringToInt(k->column_type.c_str()))
				{
					case HashStringToInt("character"):
					case HashStringToInt("date"):
					case HashStringToInt("time"):
					case HashStringToInt("text"):
					case HashStringToInt("boolean"):
					case HashStringToInt("numeric"):
														strcat(buffer, "k_");
														strcat(buffer,k->column_name.c_str());
														CHKV01_dict->SetValue("ZVAL",buffer);
														break;

														
					}

            CHKV01_dict->ShowSection("CHKV");
              ///cout<<buffer<<endl;
			if (k != q )  sprintf(buffer,"%s"," ,");
		}
		dict.ShowSection("CHK3");


		
	sprintf(OUTFILE,"./%s.hpp",(char*) gtk_label_get_text(eTABLE));

	FILE* pFile = fopen(OUTFILE, "w");

	std::string output="";
	ctemplate::ExpandTemplate("mytpl", ctemplate::DO_NOT_STRIP, &dict, &output);


	fputs (output.c_str(),pFile);
	fclose (pFile);

	}
	
	catch (const std::exception& e)
	{
		Msg_clr();
		printMsg("Generator_Field invalide vérifier si table incohérente");
	}
	slc.end();


	


 
	

}




//***************************************************************
// execution de commande ou call de programme etc...
//***************************************************************

#include<sys/ipc.h>
#include<sys/wait.h>
int exec_prog(const char* commande)
{
	int retour;
	pid_t pid;
	char * P_cmd = (char*) malloc(1024);
	sprintf(P_cmd,"%s",commande);
	char *argv[] = {(char*)"sh",(char*)"-c",P_cmd,NULL};

	if((pid = fork()) <0)
		return EXIT_FAILURE ;

	if(pid == 0)
	{
		execv("/bin/sh",argv);
		exit(127);
	}

	while ( waitpid(pid, &retour,0) <0)
		if (errno!= EINTR)
			return EXIT_FAILURE ;

	return EXIT_SUCCESS;
}

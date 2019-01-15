#ifndef C5250_HPP_INCLUDED
#define C5250_HPP_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <regex>
#include <array>
#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <gdk/gdkkeysyms.h>







#define KEY_TAB 0xff09
#define KEY_SHIFT_TAB  0xfe20

#define __MAXPNL__ 10
#define __MAXFLD__ 50

enum gLng	{  US=0,CZ,DE,ES,FR,NL,HU,PL,PT,RU,TR};
unsigned int UseLng   ;

unsigned int DSPFmt ;

enum eTYPE 	{_TYPE_ALPHAU, _TYPE_ALPHA ,_TYPE_FULL ,_TYPE_INTEGER ,_TYPE_NUMERIC ,_TYPE_REGEXP,_TYPE_IPV4,_COMBO};
enum eATR	{_ALLCASE , _UPPERCASE, _LOWERCASE};


std::array<std::string, 11> lng_AlphaUpperCase = 
										{
											"^[A-Z]*$",
											"^[A-ZÁČĎÉĚÍŇÓŘŠŤÚŮÝŽ \\-]*+$",
											"^[A-ZÄÖÜß \\-]*$",
											"^[A-ZÁÉÍÑÓÚÜ \\-]*$",
											"^[A-ZÀÂÇÉÈÊËÏÎÔŒÙÛÜŸŶ \\-]*$" ,
											"^[A-ZÉËÏÓÖÜ \\-]*$" ,
											"^[A-ZÁÉÍÓÖŐÚÜŰ \\-]*$",
											"^[A-ZĄĆĘŚŁŃÓŻŹ \\-]*$"
											"^[A-ZÃÁÀÂÇÉÊÍÕÓÔÚÜ \\-]*$",
											"^[А-ЯЁё \\-]*$",
											"^[A-ZÇĞİıÖŞÜ \\-]*$"
										};

const gchar *msg_alphaU = "Caractère Alpha Majuscule / Upercase"  ;

std::array<std::string, 11> lng_Alpha  	= 
										{
											"^[A-Za-z]*$",
											"^[A-Za-zÁČĎÉĚÍŇÓŘŠŤÚŮÝŽ]*$",
											"^[A-Za-zÄÖÜß]*$",
											"^[A-ZÁÉÍÑÓÚÜ]*$",
											"^[A-Za-zäâÀÂâçÇéÉèÈêÊëËïÏîÎöÖôÔœŒùÙüÛûÜïŸîŶ \\-]*$" ,
											"^[A-Za-zÉËÏÓÖÜ \\-]*$" ,
											"^[A-Za-zÁÉÍÓÖŐÚÜŰ \\-]*$",
											"^[A-Za-zĄĆĘŚŁŃÓŻŹ \\-]*$"
											"^[A-Za-zÃÁÀÂÇÉÊÍÕÓÔÚÜ \\-]*$",
											"^[А-ЯЁа-яё \\-]*$",
											"^[A-Za-zÇĞİıÖŞÜ \\-]*$"
										};
const gchar* msg_alpha = " :Caractère Alpha .,()/£$€'- "  ;

std::array<std::string, 11> lng_Full	=
										{
											"^[0-9A-Za-z .,()/£$€\\'\\-]*$",
											"^[0-9A-Za-zÁČĎÉĚÍŇÓŘŠŤÚŮÝŽ .,()/£$€\\'\\-]*$",
											"^[0-9A-Za-zÄÖÜß .,()/£$€\\'\\-]*$",
											"^[A-ZÁÉÍÑÓÚÜ\\-\\' .,()/£$€]*$",
											"^[0-9A-Za-zäâÀÂâçÇéÉèÈêÊëËïÏîÎöÖôÔœŒùÙüÛûÜïŸîŶ .,()/£$€\\'\\-]*$" ,
											"^[0-9A-Za-zÉËÏÓÖÜ .,()/£$€\\'\\-]*$" ,
											"^[0-9A-Za-zÁÉÍÓÖŐÚÜŰ .,()/£$€\\'\\-]*$",
											"^[0-9A-Za-zĄĆĘŚŁŃÓŻŹ .,()/£$€\\'\\-]*$"
											"^[0-9A-Za-zÃÁÀÂÇÉÊÍÕÓÔÚÜ .,()/£$€\\'\\-]*$",
											"^[0-9А-ЯЁа-яё .,()/£$€\\'\\-]*$",
											"^[0-9A-Za-zÇĞİıÖŞÜ .,()/£$€\\'\\-]*$"
										};
const gchar* msg_full = " :Caractère Alpha-numérique .,()/£$€'-"  ;


const gchar* msg_integer = " :Caractère digit"  ;
const gchar* msg_numeric = " :Caractère entier.precision ex: 10.02"  ;

const gchar* msg_Obligatoire = " :Obligatoire"  ;
const gchar* msg_valeur  = " :Valeur incorrect"  ;


const gchar* msg_car  =" :caractère invalide";



struct _FIELD_DATA	{
	const gchar		*zfield; /// nom de la zone

	unsigned int	tfield ; /// type de zone  TYPE_ALPHA , TYPE_ALNUM , TYPE_ALL ,TYPE_INTEGER , TYPE_NUMERIC ,TYPE_REGEXP, TYPE_IPV4
	unsigned int 	afield ; /// type de zone  UPPERCASE, LOWERCASE, ALLCASE
	bool			sfield ; /// type de zone  signed
	unsigned int	lfield ; /// longueur de zone
	unsigned int	ifield ; /// longueur de zone avant la virgule
	unsigned int	dfield ; /// nombre chiffre après la virgule
	bool			ofield ; /// type de zone  obligatoire
	bool			contrl ; /// contrôle value avec mini avec maxi
	double			nmini  ; /// minimum long alpha ou valeur pour numérique
	double			nmaxi  ; /// maximum valeur pour numérique


	char			cregex[200] ; /// regle de contrôle
	unsigned int	cpos   ; /// position field n°champ
	const gchar*	msgerr ; /// message d'erreur
	char * 			value  ;
};

struct _PANEL_DATA	{
	const char		*titre;
	_FIELD_DATA		field[__MAXFLD__];

	unsigned int	nfield;
}PNL[__MAXPNL__];







///****************************************************************************
///AFFECTATION BUFFER       ---------------------------------------------------
///****************************************************************************

bool InitFmt(unsigned int pnlFmt,unsigned int pnlNbrField) ;

void Alpha(int zonFmt, unsigned int zonCpos, GtkEntry *eField, unsigned zonType, unsigned zonLong,
			bool zonObligatoire = false , const  char * zonValue = "" , unsigned zonAtr = _ALLCASE);

void Integer(int zonFmt, unsigned int zonCpos, GtkEntry *eField, unsigned zonEntier,
				bool zonObligatoire = false , const char * zonValue ="" , bool zonContrl=false, int zonMini=0,
				int zonMaxi=0,bool zonSigned = false );

void Numeric(int zonFmt, unsigned int zonCpos, GtkEntry *eField, unsigned zonEntier,  unsigned zonPrecision,
				bool zonObligatoire = false , const  char * zonValue = "" , bool zonContrl=false, double zonMini=0,
				double zonMaxi=0, bool zonSigned = false);

void Combo(int zonFmt, unsigned int zonCpos, GtkComboBoxText *eField, bool zonObligatoire = true, bool zonContrl = true);

/// validation

gboolean controleField(GtkEntry *eField);

gboolean Validate(GtkWidget * p_parent,GtkWidget * p_box);


/// contrôle keyboard
gboolean key_press_ShiftTab(GtkEntry *eField, GdkEventKey *event);
gboolean key_press_Tab(GtkEntry *eField, GdkEventKey *event);
gboolean key_press_ALTF4(GtkWidget *window);

void Msg_Err (const gchar *eField,  const gchar   * p_MsgErr );
void Msg_clr();

void printMsg( const gchar   * p_Msg );
void Main_Quit();


/// ici on ferme les fichiers  on fait roolback ect....
void Procedure_end_Pgm();






///**************************************************************************************************
/// fonction
///**************************************************************************************************




bool InitFmt(unsigned int pnlFmt,unsigned int pnlNbrField)
{
	if( pnlFmt<= __MAXPNL__ )
	{
		PNL[pnlFmt].nfield=pnlNbrField ;
		return  true ;
	} else return false;
}


/// enregistre field format 

void Alpha(int zonPnl, unsigned int zonCpos, GtkEntry *eField, unsigned zonType, unsigned zonLong,
			bool zonObligatoire , const  char * zonValue , unsigned zonAtr )
{

	PNL[zonPnl].field[zonCpos].zfield=gtk_widget_get_name (GTK_WIDGET(eField));
	PNL[zonPnl].field[zonCpos].tfield=zonType;
	PNL[zonPnl].field[zonCpos].afield=zonAtr;
	PNL[zonPnl].field[zonCpos].sfield=false;
	PNL[zonPnl].field[zonCpos].lfield=zonLong;
	PNL[zonPnl].field[zonCpos].ifield=0;
	PNL[zonPnl].field[zonCpos].dfield=0;
	PNL[zonPnl].field[zonCpos].ofield=zonObligatoire;
	PNL[zonPnl].field[zonCpos].contrl=0;
	PNL[zonPnl].field[zonCpos].nmini=0;
	PNL[zonPnl].field[zonCpos].nmaxi=0;
	PNL[zonPnl].field[zonCpos].msgerr="";
	PNL[zonPnl].field[zonCpos].value=(char*)zonValue;

	switch(zonType) 
	{
		case _TYPE_ALPHAU	:
							sprintf (PNL[zonPnl].field[zonCpos].cregex, "%s", lng_AlphaUpperCase[UseLng].c_str());
							PNL[zonPnl].field[zonCpos].msgerr= msg_alphaU;
							PNL[zonPnl].field[zonCpos].afield= _UPPERCASE; /// force uppercase
							break;
		case _TYPE_ALPHA	:
							sprintf (PNL[zonPnl].field[zonCpos].cregex, "%s", lng_Alpha[UseLng].c_str());
							PNL[zonPnl].field[zonCpos].msgerr= msg_alpha;
							break;
		case _TYPE_FULL		:
							sprintf (PNL[zonPnl].field[zonCpos].cregex, "%s", lng_Full[UseLng].c_str());
							PNL[zonPnl].field[zonCpos].msgerr= msg_full;
							break;
	};
	PNL[zonPnl].field[zonCpos].cpos=zonCpos;

	if (zonValue != (char*)"" ) gtk_entry_set_text(eField ,(char*)zonValue);

	if( zonCpos == 0) g_signal_connect(eField ,"key-press-event", G_CALLBACK (key_press_ShiftTab), NULL);
	
	if( zonCpos == PNL[zonPnl].nfield-1 ) g_signal_connect(eField ,"key-press-event", G_CALLBACK (key_press_Tab),NULL);
}



void Integer(int zonPnl, unsigned int zonCpos, GtkEntry *eField, unsigned zonEntier,
			bool zonObligatoire ,  const char * zonValue , bool zonContrl, int zonMini, int zonMaxi, bool zonSigned )
{
	PNL[zonPnl].field[zonCpos].zfield=gtk_widget_get_name (GTK_WIDGET(eField));
	PNL[zonPnl].field[zonCpos].tfield=_TYPE_INTEGER;
	PNL[zonPnl].field[zonCpos].afield=_ALLCASE;
	PNL[zonPnl].field[zonCpos].sfield=zonSigned;
	PNL[zonPnl].field[zonCpos].lfield=0;
	PNL[zonPnl].field[zonCpos].ifield=zonEntier;
	PNL[zonPnl].field[zonCpos].dfield=0;
	PNL[zonPnl].field[zonCpos].ofield=zonObligatoire;
	PNL[zonPnl].field[zonCpos].contrl=zonContrl;
	PNL[zonPnl].field[zonCpos].nmini=zonMini;
	PNL[zonPnl].field[zonCpos].nmaxi=zonMaxi;
	PNL[zonPnl].field[zonCpos].msgerr="";
	PNL[zonPnl].field[zonCpos].value=(char*)zonValue;

	if ( ! zonSigned  )
		sprintf (PNL[zonPnl].field[zonCpos].cregex, "^\\d{0,%d}$",zonEntier);
	else
		sprintf (PNL[zonPnl].field[zonCpos].cregex, "^(\\-?|\\+?)\\d{0,%d}$",zonEntier);

	PNL[zonPnl].field[zonCpos].msgerr= msg_integer;

	PNL[zonPnl].field[zonCpos].cpos=zonCpos;

	if (zonValue != (char*)"" ) gtk_entry_set_text(eField ,(char*)zonValue);

	if( zonCpos == 0)  g_signal_connect(eField ,"key-press-event", G_CALLBACK (key_press_ShiftTab), NULL);
	if( zonCpos == PNL[zonPnl].nfield-1 ) g_signal_connect(eField ,"key-press-event", G_CALLBACK (key_press_Tab),NULL);
}


void Numeric(int zonPnl, unsigned int zonCpos, GtkEntry *eField, unsigned zonEntier,  unsigned zonPrecision,
			bool zonObligatoire, const  char * zonValue, bool zonContrl, double zonMini, double zonMaxi, bool zonSigned)
{

	PNL[zonPnl].field[zonCpos].zfield=gtk_widget_get_name (GTK_WIDGET(eField));
	PNL[zonPnl].field[zonCpos].tfield=_TYPE_NUMERIC;
	PNL[zonPnl].field[zonCpos].afield=_ALLCASE;
	PNL[zonPnl].field[zonCpos].sfield=zonSigned;
	PNL[zonPnl].field[zonCpos].lfield=0;
	PNL[zonPnl].field[zonCpos].ifield=zonEntier;
	PNL[zonPnl].field[zonCpos].dfield=zonPrecision;
	PNL[zonPnl].field[zonCpos].ofield=zonObligatoire;
	PNL[zonPnl].field[zonCpos].contrl=zonContrl;
	PNL[zonPnl].field[zonCpos].nmini=zonMini;
	PNL[zonPnl].field[zonCpos].nmaxi=zonMaxi;
	PNL[zonPnl].field[zonCpos].msgerr="";
	PNL[zonPnl].field[zonCpos].value=(char*)zonValue;

	if ( ! zonSigned )
		sprintf (PNL[zonPnl].field[zonCpos].cregex, "^\\d{0,%d}(\\.(?=\\d)\\d){0,%d}$",zonEntier,zonPrecision);
	else
		sprintf (PNL[zonPnl].field[zonCpos].cregex, "^(\\-?|\\+?)\\d{0,%d}(\\.(?=\\d)\\d){0,%d}$",zonEntier,zonPrecision);

	PNL[zonPnl].field[zonCpos].msgerr= msg_numeric;

	PNL[zonPnl].field[zonCpos].cpos=zonCpos;

	if (zonValue != (char*)"" ) gtk_entry_set_text(eField ,(char*)zonValue);

	if( zonCpos == 0)  g_signal_connect(eField ,"key-press-event", G_CALLBACK (key_press_ShiftTab), NULL);
	if( zonCpos == PNL[zonPnl].nfield-1 ) g_signal_connect(eField ,"key-press-event", G_CALLBACK (key_press_Tab),NULL);
}


void Combo(int zonPnl, unsigned int zonCpos, GtkComboBoxText *eField, bool zonObligatoire, bool zonContrl)
{
	const  char * zonValue ="";
	GtkWidget *pTempEntry;

	pTempEntry = GTK_WIDGET(eField);

	PNL[zonPnl].field[zonCpos].zfield=gtk_widget_get_name (pTempEntry);
	PNL[zonPnl].field[zonCpos].tfield=_COMBO;
	PNL[zonPnl].field[zonCpos].afield=_ALLCASE;
	PNL[zonPnl].field[zonCpos].sfield=false;
	PNL[zonPnl].field[zonCpos].lfield=0;
	PNL[zonPnl].field[zonCpos].ifield=0;
	PNL[zonPnl].field[zonCpos].dfield=0;
	PNL[zonPnl].field[zonCpos].ofield=zonObligatoire;
	PNL[zonPnl].field[zonCpos].contrl=zonContrl;
	PNL[zonPnl].field[zonCpos].nmini=0;
	PNL[zonPnl].field[zonCpos].nmaxi=0;
	PNL[zonPnl].field[zonCpos].msgerr="Choix Obligatoire";
	PNL[zonPnl].field[zonCpos].value=(char*)zonValue;
}

///-------------------------------------
/// petit debug
///-------------------------------------
void printMsg( const gchar   * p_Msg )
{
	GtkWidget     * p_dialog  = NULL;
	p_dialog = 	gtk_message_dialog_new	(
			NULL,
			GTK_DIALOG_MODAL,
			GTK_MESSAGE_INFO,
			GTK_BUTTONS_OK,
			p_Msg,NULL);

	gtk_dialog_run (GTK_DIALOG (p_dialog));
	gtk_widget_destroy (p_dialog);
}



///-------------------------------------
/// Clear message au bas de l'écran
///-------------------------------------
void Msg_clr()
{
	gtk_label_set_text(tMsg_Name, "" );
	gtk_label_set_text(tMsg_text, "" );
}
///-------------------------------------
/// Affiche message au bas de l'écran
///-------------------------------------
void Msg_Err (const gchar *eField,  const gchar   * p_MsgErr )
{
	gtk_label_set_text(tMsg_Name, eField);
	gtk_label_set_text(tMsg_text, p_MsgErr );
}

///-------------------------------------
/// Contrôle field
///-------------------------------------
gboolean controleField(GtkEntry *eField)
{
	std::string T_put		= gtk_entry_get_text( eField );

	const gchar * p_name	= gtk_widget_get_name (GTK_WIDGET(eField));

	int cpos= 0;

	gchar msg_contrl[200] ;	/// text erreur
	gchar fmt_Printf[200] ; /// mise en format du text a afficher

	Msg_clr();



/// recherche la zone a traiter avec ses règles
	for ( unsigned  int xpos= 0 ; xpos < PNL[DSPFmt].nfield; xpos ++)
	{
		if (0 == strcmp (p_name, PNL[DSPFmt].field[xpos].zfield) )
		{
			cpos =PNL[DSPFmt].field[xpos].cpos;
			break ;
		}
	}

/// si majuscule/minuscule obligatoire

	switch(PNL[DSPFmt].field[cpos].afield)
	{
		case _ALLCASE	:
						break;
		case _UPPERCASE :
						std::transform(T_put.begin(), T_put.end(),T_put.begin(), ::toupper);
						gtk_entry_set_text  (eField,(char *)T_put.c_str());
						break;
		case _LOWERCASE :
						std::transform(T_put.begin(), T_put.end(),T_put.begin(), ::tolower);
						gtk_entry_set_text  (eField,(char *)T_put.c_str());
						break;
		default			:
						break;
	}



/// traitement regex contrôle validité buffer

	std::regex T_mask((std::string)PNL[DSPFmt].field[cpos].cregex);

	if(!regex_match(T_put ,T_mask))
	{
		Msg_Err (p_name ,PNL[DSPFmt].field[cpos].msgerr) ;
						/// recuperation Msg_err regle regex
		return false ;
	}

	if( (T_put == "") && (PNL[DSPFmt].field[cpos].ofield ))
	{
		Msg_Err (p_name ,msg_Obligatoire);
						/// traitement saisie Obligatoire
		return false ;
	}



/// traitement numerique

	/// traitement entre valeurs  nombre entier (int)
	if (PNL[DSPFmt].field[cpos].tfield == _TYPE_INTEGER &&  PNL[DSPFmt].field[cpos].contrl )
	{
		int iVal = atoi(gtk_entry_get_text( eField ));

		if (iVal < PNL[DSPFmt].field[cpos].nmini || iVal > PNL[DSPFmt].field[cpos].nmaxi )
		{
			g_sprintf(fmt_Printf,"Invalide mini:%c%d.0%d%c  maxi:%c%d.0%d%c " ,'%',\
				PNL[DSPFmt].field[cpos].ifield,0,'f', '%',PNL[DSPFmt].field[cpos].ifield,0,'f' );

			g_sprintf( msg_contrl,fmt_Printf, PNL[DSPFmt].field[cpos].nmini,  PNL[DSPFmt].field[cpos].nmaxi );

			Msg_Err(p_name ,msg_contrl);
						/// traitement saisie Obligatoire
			return false  ;
        }
	}

	/// traitement entre valeurs nombre decimal (Double)
	if (PNL[DSPFmt].field[cpos].tfield == _TYPE_NUMERIC &&  PNL[DSPFmt].field[cpos].contrl )
	{
		double iVal = atof(gtk_entry_get_text( eField ));

		if (iVal < PNL[DSPFmt].field[cpos].nmini || iVal > PNL[DSPFmt].field[cpos].nmaxi )
		{
			g_sprintf(fmt_Printf,"Invalide mini:%c%d.0%d%c  maxi:%c%d.0%d%c " ,'%',\
			PNL[DSPFmt].field[cpos].ifield,PNL[DSPFmt].field[cpos].dfield,'f', '%',\
			PNL[DSPFmt].field[cpos].ifield,PNL[DSPFmt].field[cpos].dfield,'f' );

			g_sprintf( msg_contrl,fmt_Printf, PNL[DSPFmt].field[cpos].nmini,  PNL[DSPFmt].field[cpos].nmaxi );

			Msg_Err(p_name ,msg_contrl);
							/// traitement saisie Obligatoire
			return false ;
		}
	}


	return true ;
}

///-------------------------------------
/// contrôle format
///-------------------------------------
gboolean Validate(GtkWidget * p_parent,GtkWidget * p_box)
{
	bool    finend ;
	GList   *pList ;
	GList   *pLast ;
	GtkWidget *pTempEntry;

	const gchar   * p_name;
	/* Recuperation de la liste des elements que contient la fenetre  */
	pList = gtk_container_get_children(GTK_CONTAINER(p_box));
	pList = g_list_first(pList);
	pLast = g_list_last(pList);
	pTempEntry = GTK_WIDGET(pList->data);
	p_name    = gtk_widget_get_name (pTempEntry);
//std::cout<<"debut validate"<<std::endl;
	do 
	{
	// check de la zone si erreur arret immediat et affiche message erreur
//std::cout<<p_name<<"--"<<g_type_name(G_TYPE_FROM_INSTANCE(pTempEntry))<<"--"<<std::endl;    

		if ( g_str_equal(  g_type_name(G_TYPE_FROM_INSTANCE(pTempEntry)) , "GtkEntry") )
				if( ! controleField(GTK_ENTRY( pTempEntry ))) return false ;

		if ( g_str_equal(  g_type_name(G_TYPE_FROM_INSTANCE(pTempEntry)) , "GtkComboBoxText") )
		{
//cout<< gtk_combo_box_get_active(GTK_COMBO_BOX (pTempEntry))<<endl; 
			if (-1 ==  gtk_combo_box_get_active(GTK_COMBO_BOX (pTempEntry)))
				{
					Msg_Err(p_name  , "Veuillez choir un type (Attribut)");
					return false ;
				}
		}
		if (pList ==pLast) finend = true;// controle si fin de liste des zones à saisir 
		else 
		{
			finend= false;
			pList = g_list_next(pList);
			pTempEntry = GTK_WIDGET(pList->data);
			p_name    = gtk_widget_get_name (pTempEntry);
		}

	} while (!finend);
	
	Validation_Connection();
	return true;
}

///-------------------------------------
/// fermeture interactif
///-------------------------------------
void Main_Quit()
{
	/// ici on ferme les fichiers  on fait roolback ect....
	Procedure_end_Pgm();

	/// fin de traitement
	gtk_main_quit ();
}

///-------------------------------------
/// KEY_PRESS
///-------------------------------------
gboolean key_press_ShiftTab(GtkEntry *eField, GdkEventKey *event)
{
	/* interdit car Premiere  zone entry*/
	if (event->keyval ==  KEY_SHIFT_TAB  )  return GDK_EVENT_STOP;
	return GDK_EVENT_PROPAGATE;
}

gboolean key_press_Tab(GtkEntry *eField, GdkEventKey *event)
{
	/* interdit car dernière zone entry*/
	if (event->keyval ==  KEY_TAB  )  return GDK_EVENT_STOP;
	return GDK_EVENT_PROPAGATE;
}

///-------------------------------------
/// traitement ALT+F4
///-------------------------------------
gboolean key_press_ALTF4(GtkWidget *window)
{
	GtkWidget *dialog;
	int _reponse ;
	dialog = gtk_message_dialog_new(GTK_WINDOW(window),
				GTK_DIALOG_MODAL,
				GTK_MESSAGE_QUESTION,
				GTK_BUTTONS_YES_NO,
				"êtes-vous sur de quitter?");
	gtk_window_set_title(GTK_WINDOW(dialog), "Question");

	_reponse = gtk_dialog_run(GTK_DIALOG(dialog));

	gtk_widget_destroy(dialog);

	if (_reponse == GTK_RESPONSE_YES)
	{
		Main_Quit();
    } 
    else 
    {
		return GDK_EVENT_STOP;
    }
    
	return 0;
}


#endif // C5250_H_INCLUDED

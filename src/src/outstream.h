#ifndef PQWRPS_H_INCLUDED
#define PQWRPS_H_INCLUDED

#ifndef   _separator_
#define   _separator_		'|'		///  remplace caractère space  de délimitation multibuffer > stringstream
#endif


	 std::stringstream &operator>>(std::stringstream& out ,char  &Argc)
{
	std::string Args ; 
	getline(out,Args,_separator_);
	
	strncpy(&Argc, Args.c_str(),1);
    return out;
}


	 std::stringstream &operator>>(std::stringstream& out ,char * Argc)
{
	std::string Args ; 
	getline(out,Args,_separator_);
	
	sprintf(Argc ,"%s",Args.c_str());
    return out;
}
	 std::stringstream &operator>>(std::stringstream& out , std::string &Args)
{
	getline(out,Args,_separator_);

    return out;
}
 std::stringstream &operator>>(std::stringstream& out , int &Argi)
{
	std::string Args ; 
	getline(out,Args,_separator_);

	Argi =atoi(Args.c_str());
    return out;
}

 std::stringstream &operator>>(std::stringstream& out , double &Argd)
{
	std::string Args ; 
	getline(out,Args,_separator_);

	const struct lconv* loc;
	loc = localeconv();
	std::string ponct=".";
	int pos = Args.find(ponct);
	if (pos >0)
	{
		if (loc->decimal_point[0]==',' && pos >= 0 ) Args.replace(pos,ponct.length(),",");
	} 
	Argd =std::stod(Args.c_str());
    return out;
}

 std::stringstream &operator>>(std::stringstream& out ,float &Argf)
{
	std::string Args ; 
	getline(out,Args,_separator_);
	const struct lconv* loc;
	loc = localeconv();
	std::string ponct=".";
	int pos = Args.find(ponct);
	if (pos >0)
	{
		if (loc->decimal_point[0]==',' && pos >= 0 ) Args.replace(pos,ponct.length(),",");
	}
	Argf =atof(Args.c_str());
    return out;
}

 std::stringstream &operator>>(std::stringstream& out ,bool &Argb)
{
	std::string Args ; 
	getline(out,Args,_separator_);

	Argb =atoi(Args.c_str());
    return out;
}

#ifdef	ZONED_H_INCLUDED
	 std::stringstream &operator>>(std::stringstream& out ,Zchar  &Argc)
{
	std::string Args ; 
	getline(out,Args,_separator_);
	
	Argc = Args.c_str();
    return out;
}

	 std::stringstream &operator>>(std::stringstream& out ,Zdcml  &Argc)
{
	std::string Args ; 
	getline(out,Args,_separator_);
	
	Argc = Args.c_str();
    return out;
}

	 std::stringstream &operator>>(std::stringstream& out ,Zdate  &Argc)
{
	std::string Args ; 
	getline(out,Args,_separator_);
	
	Argc = Args.c_str();
    return out;
}


	 std::stringstream &operator>>(std::stringstream& out ,Ztime  &Argc)
{
	std::string Args ; 
	getline(out,Args,_separator_);
	
	Argc = Args.c_str();
    return out;
}


	 std::stringstream &operator>>(std::stringstream& out ,Zbool  &Argc)
{
	std::string Args ; 
	getline(out,Args,_separator_);
	
	Argc = Args.c_str();
    return out;
}

	 std::stringstream &operator>>(std::stringstream& out ,Ztext  &Argc)
{
	std::string Args ; 
	getline(out,Args,_separator_);
	
	Argc = Args.c_str();
    return out;
}

#endif //ZONED_H_INCLUDED
#endif //PQWRPS_H_INCLUDED

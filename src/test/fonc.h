




 
   


struct with_prepare {
     bool tag = true;
}SQLin;
    bool _first = true;

struct prepare_stream  {
    prepare_stream(std::ostream &stream, bool tag = false)
    : _stream(stream), _tag(tag) {}
    
    template <class Rhs>
    prepare_stream &operator << (Rhs &&rhs) {

    if (_tag == false ) throw std::invalid_argument("bar init requete");
	
        if(_first)
        {
			ss<<std::forward<Rhs>(rhs);
			_first = false;
			
			format = ss.str(); 
			count = xcount_format_specifiers(ss.str());
			ss.str("");
		}
        else
        {
            auto first_format_pos = format.find_first_of(FORMAT_SPECIFIER);
            ss<<format.substr(0, first_format_pos);
			ss<<std::forward<Rhs>(rhs);
            format = format.substr(first_format_pos+1);
            
		}
		_stream.clear(); 
		if ( count == i ) _stream << ss.str();
		i++;
        return *this;
    }

    prepare_stream &operator << (std::ostream &(*manip)(std::ostream&)) {
        manip(_stream);
        return *this;
    }

    private:
    std::ostream &_stream;
    bool _tag;
    unsigned count =0 ;
    std::string format ;
    std::string fin_stage ;
    unsigned int i =0 ;
    std::stringstream ss;
    static constexpr char FORMAT_SPECIFIER = '?';
};

prepare_stream operator << (std::ostream &stream,with_prepare wrqd) {
    return prepare_stream(stream,wrqd.tag);
}

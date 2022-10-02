#include "library.hpp"
#include "scope_http.hpp"

class Configuration {

    public:

    Configuration(){}
    ~Configuration(){}
    Configuration(std::string str){
        _file = str;
        check_brackets();
        extract_http_blocks();
    }
    //equal operation implementation
    void check_brackets();
    void extract_http_blocks();
    std::string chunking();

    private:

    std::string                 _file;
    std::list<HttpScope>       _http_blocks;
    class BracketsException : public std::exception {virtual const char* what() const throw(){return ("Close the brackets in the configuration file please");}};
};
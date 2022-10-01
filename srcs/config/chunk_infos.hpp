#pragma once

// Votre serveur doit pouvoir ecouter sur plusieurs ports.
// Choisir le port et lhost de chaque "serveur".
// Setup server_names ou pas.
// Le premier serveur pour un host:port sera le serveur par défaut pour cet host:port (ce qui signifie quil répondra à toutes les requêtes qui nappartiennent pas à un autre serveur).
// Setup des pages derreur par défaut.
// Limiter la taille du body des clients.
// Setup des routes avec une ou plusieurs des règles/configurations suivantes (les routes nutiliseront pas de regexp) :
    // Définir une liste de méthodes HTTP acceptées pour la route.
    // Définir une redirection HTTP.
    // Définir un répertoire ou un fichier à partir duquel le fichier doit être recherché (par exemple si lurl /kapouet est rootée sur /tmp/www, lurl /kapouet/pouic/toto/pouet est /tmp/www/pouic/toto/pouet).
    // Activer ou désactiver le listing des répertoires.
    // Set un fichier par défaut comme réponse si la requête est un répertoire.
    // Exécuter CGI en fonction de certaines extensions de fichier (par exemple .php).
    // Rendre la route capable daccepter les fichiers téléchargés et configurer où cela doit être enregistré.
        // Vous vous demandez ce quest un CGI ?
        // Parce que vous nallez pas appeler le CGI mais utiliser directement le chemin complet comme PATH_INFO.
        // Souvenezvous simplement que pour les requêtes fragmentées, votre serveur doit la défragmenter et le CGI attendra EOF comme fin du body.
        // Même chosespour la sortie du CGI. Si aucun content_length nest renvoyé par le CGI, EOF signifiera la fin des données renvoyées.
        // Votre programme doit appeler le CGI avec le fichier demandé comme premier argument.
        // Le CGI doit être exécuté dans le bon répertoire pour laccès au fichier de chemin relatif.
        // votre serveur devrait fonctionner avec un seul CGI (phpCGI, Python, etc.). Vous devez fournir des fichiers de configuration et des fichiers de base par défaut pour tester et démontrer que chaque fonctionnalité fonctionne pendant lévaluation.

#include "library.hpp"

class Chunk_Infos {

    public:

    Chunk_Infos(){}
    ~Chunk_Infos(){}

    Chunk_Infos(std::string str){
        _chunk = str;

        _chunk.erase(0, _chunk.find_first_of('{', 0) + 1);
        _chunk.erase(_chunk.find_last_of('}'), 1);
        while (_chunk.find('#', 0) != std::string::npos)
            _chunk.erase(_chunk.find('#', 0), _chunk.find_first_of('\n', _chunk.find('#', 0)) - _chunk.find('#', 0));

        extract_location_blocks();
        extract_directive_lines();

        while (_directive_lines.size() != 0)
        {
            if (_directive_lines.back().find("listen") == _directive_lines.back().find_first_not_of("\t\v\n\r\f "))
            {
                if (_directive_lines.back().find_first_of("\t\v\n\r\f ", _directive_lines.back().find("listen")) == (_directive_lines.back().find("listen") + 6))
                    extract_listen(_directive_lines.back());
            }
            else if (_directive_lines.back().find("server_name") == _directive_lines.back().find_first_not_of("\t\v\n\r\f "))
            {
                if (_directive_lines.back().find_first_of("\t\v\n\r\f ", _directive_lines.back().find("server_name")) == (_directive_lines.back().find("server_name") + 11))
                    std::cout << "server_name found" << std::endl;
            }
            else if (_directive_lines.back().find("error_page") == _directive_lines.back().find_first_not_of("\t\v\n\r\f "))
            {
                if (_directive_lines.back().find_first_of("\t\v\n\r\f ", _directive_lines.back().find("error_page")) == (_directive_lines.back().find("error_page") + 10))
                    std::cout << "error_page found" << std::endl;
            }
            else if (_directive_lines.back().find("client_body_buffer_size") == _directive_lines.back().find_first_not_of("\t\v\n\r\f "))
            {
                if (_directive_lines.back().find_first_of("\t\v\n\r\f ", _directive_lines.back().find("client_body_buffer_size")) == (_directive_lines.back().find("client_body_buffer_size") + 23))
                    std::cout << "client_body_buffer_size found" << std::endl;
            }
            else if (_directive_lines.back().find("root") == _directive_lines.back().find_first_not_of("\t\v\n\r\f "))
            {
                if (_directive_lines.back().find_first_of("\t\v\n\r\f ", _directive_lines.back().find("root")) == (_directive_lines.back().find("root") + 4))
                    std::cout << "root found" << std::endl;
            }
            else if (_directive_lines.back().find("allow_method") == _directive_lines.back().find_first_not_of("\t\v\n\r\f "))
            {
                if (_directive_lines.back().find_first_of("\t\v\n\r\f ", _directive_lines.back().find("allow_method")) == (_directive_lines.back().find("allow_method") + 12))
                    std::cout << "allow_method found" << std::endl;
            }
            else if (_directive_lines.back().find("cgi") == _directive_lines.back().find_first_not_of("\t\v\n\r\f "))
            {
                if (_directive_lines.back().find_first_of("\t\v\n\r\f ", _directive_lines.back().find("cgi")) == (_directive_lines.back().find("cgi") + 3))
                    std::cout << "cgi found" << std::endl;
            }
            else if (_directive_lines.back().find("index") == _directive_lines.back().find_first_not_of("\t\v\n\r\f "))
            {
                if (_directive_lines.back().find_first_of("\t\v\n\r\f ", _directive_lines.back().find("index")) == (_directive_lines.back().find("index") + 5))
                    std::cout << "index found" << std::endl;
            }
            else if (_directive_lines.back().find("autoindex") == _directive_lines.back().find_first_not_of("\t\v\n\r\f "))
            {
                if (_directive_lines.back().find_first_of("\t\v\n\r\f ", _directive_lines.back().find("autoindex")) == (_directive_lines.back().find("autoindex") + 9))
                    std::cout << "autoindex found" << std::endl;
            }
            else if (_directive_lines.back().find("try_files") == _directive_lines.back().find_first_not_of("\t\v\n\r\f "))
            {
                if (_directive_lines.back().find_first_of("\t\v\n\r\f ", _directive_lines.back().find("try_files")) == (_directive_lines.back().find("try_files") + 9))
                    std::cout << "try_files found" << std::endl;
            }
            _directive_lines.pop_back();
        }

        if (_address.size() == 0)
        {
            _address = "*";
            _port = "80";           
        }

        // extract_address_port();
        // extract_server_name();
        // extract_default_error_pages();
        // extract_client_body_buffer_size();
        // extract_root();
        // extract_allow_method();
        // extract_cgi();
        // extract_index();
        // extract_autoindex();
        // extract_try_files();







        // std::ofstream ofs;
        // ofs.open("config_result.txt");

        // int n = 1;
        // for (std::list<std::string>::iterator it = _directive_lines.begin(); it != _directive_lines.end(); ++it)
        //     ofs << "\nthe line " << n++ << " is:\n" << *it << std::endl;;


        // ofs << _chunk;



        // for (std::list<std::string>::iterator it = locations.begin(); it != locations.end(); ++it)
            // ofs << *it << std::endl;

        // ofs << copy;

        // while (_chunk.find(';', 0) != std::string::npos)
        // {
        //     _configs.push_back(_chunk.substr(0, _chunk.find_first_of(';', 0) + 1));
        //     _chunk.erase(0, _chunk.find_first_of(';', 0) + 1);
        // }
        

        // for (std::list<std::string>::iterator it = _configs.begin(); it != _configs.end(); ++it)
        // {
        //     if (it->find_first_not_of("\t\v\n\r\f ", 0) == it->find_first_of("location"))
        //     {

        //     }
        // }

        // extract_location();



        print_result();
    }

    // void extract_location() {
    //     if (_chunk.first_first)
    // }

    void extract_location_blocks();
    void extract_directive_lines();

    void extract_listen(std::string listen_dir);
    void extract_server_name();
    void extract_default_error_pages();
    void extract_client_body_buffer_size();
    void extract_root();
    void extract_allow_method();
    void extract_cgi();
    void extract_index();
    void extract_autoindex();
    void extract_try_files();

    void print_result();

    private:

    std::string _chunk;
    std::list<std::string> _location_blocks;
    std::list<std::string> _directive_lines;





    std::list<std::string> _configs;


    std::string _address;
    std::string _port;
    std::list<std::string> _server_names;

    std::list<std::string> _try_files;
    std::list<std::string> _index;
    std::string _client_body_buffer_size;
    std::string _autoindex;
    std::string _root;
    
    std::list<std::string> _allow_method;
    std::list<std::pair<std::string, std::string> > _cgi;
    std::map<std::string, std::string> _default_error_pages;



    // bool is_http;
};

#pragma once
#include <kvs/CommandLine>
#include <string>


namespace local
{

class Input
{
private:
    kvs::CommandLine m_commandline;

public:
    std::string filename;

    Input( int argc, char** argv )
    {
        m_commandline = kvs::CommandLine( argc, argv );
        m_commandline.addHelpOption();
        m_commandline.addValue( "Input filename.", false );
    }

    bool parse()
    {
        if ( !m_commandline.parse() ) { return false; }
        if ( m_commandline.hasValues() ) { filename = m_commandline.value<std::string>(); }
        return true;
    }
};

} // end of namespace local

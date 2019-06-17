#include <kvs/glut/Application>
#include "Input.h"
#include "Model.h"
#include "View.h"
#include "Controller.h"


int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    local::Input input( argc, argv );
    if ( !input.parse() ) { return 1; }

    local::Model model( input );
    local::View view( &app, &model );
    local::Controller controller( &model, &view );

    return app.run();
}

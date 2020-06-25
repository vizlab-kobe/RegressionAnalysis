#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/python/Interpreter>
#include <kvs/python/Module>
#include <kvs/python/Int>
#include <kvs/python/Dict>
#include <kvs/python/Callable>
#include <kvs/python/Tuple>
#include <kvs/python/Array>
#include <kvs/StructuredVolumeObject>
#include <kvs/RayCastingRenderer>
#include <kvs/Bounds>


kvs::ValueArray<kvs::Real32> drv( const int dimx, const int dimy, const int dimz )
{
    kvs::python::Interpreter intepreter;

    const char* script_file_name = "drv";
    const char* func_name = "main";

    kvs::python::Module module( script_file_name );
    kvs::python::Dict dict = module.dict();
    kvs::python::Callable func( dict.find( func_name ) );

    kvs::python::Tuple args( 3 );
    args.set( 0, kvs::python::Int( dimx ) );
    args.set( 1, kvs::python::Int( dimy ) );
    args.set( 2, kvs::python::Int( dimz ) );

    kvs::python::Array values = func.call( args  );

    return kvs::ValueArray<kvs::Real32>( values );
}

int main( int argc, char** argv )
{
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setTitle( "Deep Regression Volume" );
    screen.create();

    const int dimx = 10;
    const int dimy = 10;
    const int dimz = 10;
    const auto values = drv( dimx, dimy, dimz );

    auto* object = new kvs::StructuredVolumeObject();
    object->setGridTypeToUniform();
    object->setVeclen( 1 );
    object->setResolution( kvs::Vec3u( dimx, dimy, dimz ) );
    object->setValues( values );
    object->updateMinMaxValues();

    screen.registerObject( object, new kvs::Bounds() );
    screen.registerObject( object, new kvs::glsl::RayCastingRenderer() );

    return app.run();
}

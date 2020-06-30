#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/python/Interpreter>
#include <kvs/python/Module>
#include <kvs/python/Int>
#include <kvs/python/String>
#include <kvs/python/Dict>
#include <kvs/python/Callable>
#include <kvs/python/Tuple>
#include <kvs/python/Array>
#include <kvs/StructuredVolumeObject>
#include <kvs/RayCastingRenderer>
#include <kvs/Bounds>
#include <kvs/Json>
#include <kvs/File>


kvs::ValueArray<kvs::Real32> Predict( const kvs::Vec3i dim, const std::string& model_file )
{
    kvs::python::Interpreter intepreter;

    const char* script_file_name = "pred";
    const char* func_name = "main";

    kvs::python::Module module( script_file_name );
    kvs::python::Dict dict = module.dict();
    kvs::python::Callable func( dict.find( func_name ) );

    kvs::python::Tuple args( 4 );
    args.set( 0, kvs::python::Int( dim.x() ) );
    args.set( 1, kvs::python::Int( dim.y() ) );
    args.set( 2, kvs::python::Int( dim.z() ) );
    args.set( 3, kvs::python::String( model_file ) );
    kvs::python::Array values = func.call( args  );

    return kvs::ValueArray<kvs::Real32>( values );
}

int main( int argc, char** argv )
{
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setTitle( "Deep Regression Volume" );
    screen.create();

    kvs::Json input( ( argc > 1 ) ? std::string( argv[1] ) : "input.json" );
    auto& o = input.rootObject();
    const std::string model_file = o["model_file"].get<std::string>();
    const bool output = o["output"].get<bool>();
    kvs::Vec3i dim;
    int i = 0;
    for ( auto& d : o["dim"].get<kvs::Json::Array>() )
    {
        dim[i++] = static_cast<int>( d.get<double>() );
    }

    std::cout << "Prediction ..." << std::endl;
    kvs::Timer timer( kvs::Timer::Start );
    const auto values = Predict( dim, model_file );
    timer.stop();
    std::cout << "Proccesing time: " << timer.sec() << " [sec]" << std::endl;

    auto* object = new kvs::StructuredVolumeObject();
    object->setGridTypeToUniform();
    object->setVeclen( 1 );
    object->setResolution( kvs::Vec3u( dim ) );
    object->setValues( values );
    object->updateMinMaxValues();
    object->print( std::cout );
    if ( output ) { object->write( kvs::File( model_file ).baseName() + ".kvsml" ); }

    auto* renderer = new kvs::glsl::RayCastingRenderer();
    renderer->setTransferFunction( kvs::ColorMap::BrewerSpectral( 256 ) );

    auto* bounds = new kvs::Bounds();
    bounds->setLineWidth( 2.0f );

    screen.registerObject( object, bounds );
    screen.registerObject( object, renderer );

    return app.run();
}

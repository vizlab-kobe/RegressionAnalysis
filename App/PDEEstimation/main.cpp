#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/TornadoVolumeData>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVectorToScalar>
#include <kvs/MersenneTwister>
#include <kvs/UniformGrid>
#include <kvs/LinearRegression>
#include <kvs/Bounds>
#include <kvs/RayCastingRenderer>
#include <kvs/Timer>
#include <kvs/LineObject>
#include <kvs/Label>


namespace
{

kvs::ValueArray<float> Slice( const kvs::ValueArray<float>& vectors, const size_t offset )
{
    kvs::ValueArray<float> scalars( vectors.size() / 3 );
    for ( size_t i = 0; i < scalars.size(); i++ ) { scalars[i] = vectors[ 3 * i + offset ]; }
    return scalars;
}

kvs::StructuredVolumeObject* MagnitudeVolume( kvs::StructuredVolumeObject* volume )
{
    return new kvs::StructuredVectorToScalar( volume );
}

kvs::StructuredVolumeObject* UVolume( kvs::StructuredVolumeObject* volume )
{
    kvs::StructuredVolumeObject* scalar_volume = new kvs::StructuredVolumeObject();
    scalar_volume->setGridTypeToUniform();
    scalar_volume->setVeclen( 1 );
    scalar_volume->setResolution( volume->resolution() );
    scalar_volume->setValues( Slice( volume->values().asValueArray<float>(), 0 ) );
    scalar_volume->updateMinMaxValues();
    scalar_volume->updateMinMaxCoords();
    return scalar_volume;
}

kvs::StructuredVolumeObject* VVolume( kvs::StructuredVolumeObject* volume )
{
    kvs::StructuredVolumeObject* scalar_volume = new kvs::StructuredVolumeObject();
    scalar_volume->setGridTypeToUniform();
    scalar_volume->setVeclen( 1 );
    scalar_volume->setResolution( volume->resolution() );
    scalar_volume->setValues( Slice( volume->values().asValueArray<float>(), 1 ) );
    scalar_volume->updateMinMaxValues();
    scalar_volume->updateMinMaxCoords();
    return scalar_volume;
}

kvs::StructuredVolumeObject* WVolume( kvs::StructuredVolumeObject* volume )
{
    kvs::StructuredVolumeObject* scalar_volume = new kvs::StructuredVolumeObject();
    scalar_volume->setGridTypeToUniform();
    scalar_volume->setVeclen( 1 );
    scalar_volume->setResolution( volume->resolution() );
    scalar_volume->setValues( Slice( volume->values().asValueArray<float>(), 2 ) );
    scalar_volume->updateMinMaxValues();
    scalar_volume->updateMinMaxCoords();
    return scalar_volume;
}

} // end of namespace

kvs::ValueArray<float> Sampling(
    const size_t npoints, // number of generating points
    const kvs::StructuredVolumeObject* volume, // input volume data
    const kvs::Vec3 center, // center of sampling box
    const kvs::Real32 length ) // length of sampling box
{
    kvs::ValueArray<float> values( npoints );

    const unsigned long seed = 0;
    kvs::MersenneTwister R( seed );
    kvs::UniformGrid grid( volume );
    const kvs::Vec3 corner = center - kvs::Vec3::All( length * 0.5f );
    for ( size_t i = 0; i < npoints; i++ )
    {
        const kvs::Vec3 p = corner + kvs::Vec3( R(), R(), R() ) * length;
        grid.bind( p );
        values[i] = grid.scalar();
    }

    return values;
}

kvs::LineObject* Box(
    const kvs::StructuredVolumeObject* object,
    const kvs::Vec3 center,
    const kvs::Real32 length )
{
    const kvs::Vec3 min = center - kvs::Vec3::All( length * 0.5f );
    const kvs::Vec3 max = center + kvs::Vec3::All( length * 0.5f );

    std::vector<kvs::Real32> coords;
    std::vector<kvs::UInt32> connects;

    // Coordinate values.
    coords.push_back( min.x() ); coords.push_back( min.y() ); coords.push_back( min.z() ); // (0)
    coords.push_back( max.x() ); coords.push_back( min.y() ); coords.push_back( min.z() ); // (1)
    coords.push_back( max.x() ); coords.push_back( min.y() ); coords.push_back( max.z() ); // (2)
    coords.push_back( min.x() ); coords.push_back( min.y() ); coords.push_back( max.z() ); // (3)
    coords.push_back( min.x() ); coords.push_back( max.y() ); coords.push_back( min.z() ); // (4)
    coords.push_back( max.x() ); coords.push_back( max.y() ); coords.push_back( min.z() ); // (5)
    coords.push_back( max.x() ); coords.push_back( max.y() ); coords.push_back( max.z() ); // (6)
    coords.push_back( min.x() ); coords.push_back( max.y() ); coords.push_back( max.z() ); // (7)

    // Bottom edges.
    connects.push_back( 0 ); connects.push_back( 1 );
    connects.push_back( 1 ); connects.push_back( 2 );
    connects.push_back( 2 ); connects.push_back( 3 );
    connects.push_back( 3 ); connects.push_back( 0 );

    // Top edges.
    connects.push_back( 4 ); connects.push_back( 5 );
    connects.push_back( 5 ); connects.push_back( 6 );
    connects.push_back( 6 ); connects.push_back( 7 );
    connects.push_back( 7 ); connects.push_back( 4 );

    // Vertical edges.
    connects.push_back( 0 ); connects.push_back( 4 );
    connects.push_back( 1 ); connects.push_back( 5 );
    connects.push_back( 2 ); connects.push_back( 6 );
    connects.push_back( 3 ); connects.push_back( 7 );

    kvs::LineObject* line_object = new kvs::LineObject();
    line_object->setMinMaxObjectCoords( object->minObjectCoord(), object->maxObjectCoord() );
    line_object->setMinMaxExternalCoords( object->minExternalCoord(), object->maxExternalCoord() );
    line_object->setLineTypeToSegment();
    line_object->setColorTypeToLine();
    line_object->setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    line_object->setConnections( kvs::ValueArray<kvs::UInt32>( connects ) );
    line_object->setColor( kvs::RGBColor::Black() );
    line_object->setSize( 2 );

    return line_object;
}

class Label : public kvs::Label
{
public:
    Label( kvs::ScreenBase* screen, const kvs::LinearRegression<float>& regression ):
        kvs::Label( screen )
    {
        setText( "Y = %f + %f * X0 + %f * X1 + %f * X2",
                 regression.coef()[0],
                 regression.coef()[1],
                 regression.coef()[2],
                 regression.coef()[3] );
        addText( "R2: %f", regression.r2() );
    }
};

int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );
    screen.show();

    kvs::StructuredVolumeObject* tornado = new kvs::TornadoVolumeData( kvs::Vec3u( 32, 32, 32 ) );
    kvs::StructuredVolumeObject* mvol = ::MagnitudeVolume( tornado );
    kvs::StructuredVolumeObject* uvol = ::UVolume( tornado );
    kvs::StructuredVolumeObject* vvol = ::VVolume( tornado );
    kvs::StructuredVolumeObject* wvol = ::WVolume( tornado );
    delete tornado;

    const kvs::Vec3 center( 5, 25, 10 );
    const kvs::Real32 length = 5;
    const size_t npoints = 100;

    kvs::Timer timer( kvs::Timer::Start );
    const kvs::ValueArray<float> Y = Sampling( npoints, mvol, center, length );
    const kvs::ValueArray<float> X0 = Sampling( npoints, uvol, center, length );
    const kvs::ValueArray<float> X1 = Sampling( npoints, vvol, center, length );
    const kvs::ValueArray<float> X2 = Sampling( npoints, wvol, center, length );

    kvs::ValueArray<float> dep( Y );
    kvs::ValueTable<float> indep( npoints, 3 );
    indep[0] = X0;
    indep[1] = X1;
    indep[2] = X2;

    kvs::LinearRegression<float> regression( dep, indep );
    timer.stop();

    std::cout << "Calculation time: " << timer.msec() << " [msec]" << std::endl;
    std::cout << "Coef: " << regression.coef() << std::endl;
    std::cout << "R2: " << regression.r2() << std::endl;
    std::cout << "t-values: " << regression.tValues() << std::endl;

    screen.registerObject( Box( mvol, center, length ) );
    screen.registerObject( mvol, new kvs::Bounds );
    screen.registerObject( mvol, new kvs::glsl::RayCastingRenderer );

    Label label( &screen, regression );
    label.setMargin( 10 );
    label.show();

    return app.run();
}

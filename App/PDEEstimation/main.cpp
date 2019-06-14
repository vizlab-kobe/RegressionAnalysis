#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/TornadoVolumeData>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVectorToScalar>
#include <kvs/Bounds>
#include <kvs/RayCastingRenderer>
#include <kvs/Label>

#include "Abs.h"
#include "U.h"
#include "V.h"
#include "W.h"
#include "dX.h"
#include "dY.h"
#include "dZ.h"
#include "dT.h"
#include "Sampling.h"
#include "Box.h"
#include "Label.h"
#include "Regression.h"

typedef kvs::StructuredVolumeObject Volume;

int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );
    screen.show();

    Volume* tornado = new kvs::TornadoVolumeData( kvs::Vec3u( 32, 32, 32 ) );
    Volume* S = local::Abs( tornado );
    Volume* U = local::U( tornado );
    Volume* V = local::V( tornado );
    Volume* W = local::W( tornado );
    delete tornado;

    Volume* dU_dX = local::dX( U );
    Volume* dU_dY = local::dY( U );
    Volume* d2U_dX2 = local::dX( dU_dX );
    Volume* d2U_dXdY = local::dY( dU_dX );
    delete U;
    delete V;
    delete W;

    const kvs::Vec3 center( 5, 25, 10 );
    const kvs::Real32 length = 5;
    const size_t npoints = 100;

    const kvs::ValueArray<float> Y = local::Sampling( npoints, S, center, length );
    const kvs::ValueArray<float> X0 = local::Sampling( npoints, dU_dX, center, length );
    const kvs::ValueArray<float> X1 = local::Sampling( npoints, dU_dY, center, length );
    const kvs::ValueArray<float> X2 = local::Sampling( npoints, d2U_dX2, center, length );
    const kvs::ValueArray<float> X3 = local::Sampling( npoints, d2U_dXdY, center, length );

    local::Regression<float> regression;
    regression.setDependentVariable( Y );
    regression.addIndependentVariable( X0 );
    regression.addIndependentVariable( X1 );
    regression.addIndependentVariable( X2 );
    regression.addIndependentVariable( X3 );
    regression.fit();

    screen.registerObject( local::Box( S, center, length ) );
    screen.registerObject( S, new kvs::Bounds );
    screen.registerObject( S, new kvs::glsl::RayCastingRenderer );

    local::Label label( &screen, regression );
    label.setMargin( 10 );
    label.show();

    return app.run();
}

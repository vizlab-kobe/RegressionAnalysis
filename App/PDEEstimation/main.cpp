#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/TornadoVolumeData>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVectorToScalar>
#include <kvs/Bounds>
#include <kvs/RayCastingRenderer>
#include <kvs/Label>
#include <kvs/Slider>
#include <kvs/RadioButton>
#include <kvs/RadioButtonGroup>

#include "Op.h"
#include "Sampling.h"
#include "Box.h"
#include "Label.h"
#include "Regression.h"

typedef kvs::StructuredVolumeObject Volume;


class BoxCenterSlider : public kvs::Slider
{
public:
    BoxCenterSlider( kvs::glut::Screen* screen ): kvs::Slider( screen )
    {
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "Box Center" );
    }
};

class BoxLengthSlider : public kvs::Slider
{
public:
    BoxLengthSlider( kvs::glut::Screen* screen ): kvs::Slider( screen )
    {
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "Box Length" );
    }
};

class LinearRadioButton : public kvs::RadioButton
{
public:
    LinearRadioButton( kvs::glut::Screen* screen ): kvs::RadioButton( screen )
    {
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "Linear" );
    }
};

class LassoRadioButton : public kvs::RadioButton
{
public:
    LassoRadioButton( kvs::glut::Screen* screen ): kvs::RadioButton( screen )
    {
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "Lasso" );
    }
};

class RidgeRadioButton : public kvs::RadioButton
{
public:
    RidgeRadioButton( kvs::glut::Screen* screen ): kvs::RadioButton( screen )
    {
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "Ridge" );
    }
};

class ElasticNetRadioButton : public kvs::RadioButton
{
public:
    ElasticNetRadioButton( kvs::glut::Screen* screen ): kvs::RadioButton( screen )
    {
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "Elastic-Net" );
    }
};

class ComplexitySlider : public kvs::Slider
{
public:
    ComplexitySlider( kvs::glut::Screen* screen ): kvs::Slider( screen )
    {
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "Complexity" );
    }
};

class L1RatioSlider : public kvs::Slider
{
public:
    L1RatioSlider( kvs::glut::Screen* screen ): kvs::Slider( screen )
    {
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "L1 Ratio" );
    }
};

int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );
    screen.setBackgroundColor( kvs::RGBColor::White() );
    screen.setSize( 800, 600 );
    screen.show();

    Volume* tornado = new kvs::TornadoVolumeData( kvs::Vec3u( 32, 32, 32 ) );
    Volume* S = local::Op::Abs( tornado );
    Volume* U = local::Op::U( tornado );
    Volume* V = local::Op::V( tornado );
    Volume* W = local::Op::W( tornado );
    delete tornado;

    Volume* dU_dX = local::Op::dX( U );
    Volume* dU_dY = local::Op::dY( U );
    Volume* d2U_dX2 = local::Op::dX( dU_dX );
    Volume* d2U_dXdY = local::Op::dY( dU_dX );
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
    regression.setMethodToRidge();
    regression.setComplexity( 0.1 );
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

    const size_t slider_width = 150;
    BoxCenterSlider box_center_slider( &screen );
    box_center_slider.setMargin( 10 );
    {
        const size_t x = screen.width() - ( slider_width + box_center_slider.margin() );
        const size_t y = 0;
        box_center_slider.setPosition( x, y );
        box_center_slider.show();
    }

    BoxLengthSlider box_length_slider( &screen );
    box_length_slider.setMargin( 10 );
    {
        const size_t x = box_center_slider.x();
        const size_t y = box_center_slider.y() + box_center_slider.height();
        box_length_slider.setPosition( x, y );
        box_length_slider.show();
    }

    LinearRadioButton linear_radio_button( &screen );
    linear_radio_button.setMargin( 10 );
    {
        const size_t x = box_length_slider.x();
        const size_t y = box_length_slider.y() + box_length_slider.height();
        linear_radio_button.setPosition( x, y );
        linear_radio_button.show();
    }

    LassoRadioButton lasso_radio_button( &screen );
    lasso_radio_button.setMargin( 10 );
    {
        const size_t x = linear_radio_button.x();
        const size_t y = linear_radio_button.y() + linear_radio_button.height();
        lasso_radio_button.setPosition( x, y );
        lasso_radio_button.show();
    }

    RidgeRadioButton ridge_radio_button( &screen );
    ridge_radio_button.setMargin( 10 );
    {
        const size_t x = lasso_radio_button.x();
        const size_t y = lasso_radio_button.y() + lasso_radio_button.height();
        ridge_radio_button.setPosition( x, y );
        ridge_radio_button.show();
    }

    ElasticNetRadioButton elastic_radio_button( &screen );
    elastic_radio_button.setMargin( 10 );
    {
        const size_t x = ridge_radio_button.x();
        const size_t y = ridge_radio_button.y() + ridge_radio_button.height();
        elastic_radio_button.setPosition( x, y );
        elastic_radio_button.show();
    }

    kvs::RadioButtonGroup radio_group( &screen );
    radio_group.add( &linear_radio_button );
    radio_group.add( &lasso_radio_button );
    radio_group.add( &ridge_radio_button );
    radio_group.add( &elastic_radio_button );

    ComplexitySlider complexity_slider( &screen );
    complexity_slider.setMargin( 10 );
    {
        const size_t x = elastic_radio_button.x();
        const size_t y = elastic_radio_button.y() + elastic_radio_button.height();
        complexity_slider.setPosition( x, y );
        complexity_slider.show();
    }

    L1RatioSlider l1_ratio_slider( &screen );
    l1_ratio_slider.setMargin( 10 );
    {
        const size_t x = complexity_slider.x();
        const size_t y = complexity_slider.y() + complexity_slider.height();
        l1_ratio_slider.setPosition( x, y );
        l1_ratio_slider.show();
    }

    return app.run();
}

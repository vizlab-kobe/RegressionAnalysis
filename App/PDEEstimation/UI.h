#pragma once
#include "Model.h"
#include "View.h"
#include <kvs/ScreenBase>
#include <kvs/Slider>
#include <kvs/RadioButton>
#include <kvs/RadioButtonGroup>
#include <kvs/PushButton>
#include <kvs/CheckBox>
#include <kvs/String>
#include "Model.h"
#include "View.h"


namespace local
{

namespace UI
{

class BoxCenterSlider : public kvs::Slider
{
public:
    enum Coord
    {
        XCoord,
        YCoord,
        ZCoord
    };

private:
    local::Model* m_model;
    local::View* m_view;
    Coord m_coord;

public:
    BoxCenterSlider( local::Model* model, local::View* view, Coord coord ):
        kvs::Slider( &view->screen() ),
        m_model( model ),
        m_view( view ),
        m_coord( coord )
    {
        const kvs::Vec3u dim = m_model->inputVolumeObject()->resolution();

        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        switch ( m_coord )
        {
        case XCoord:
        {
            setRange( 0, dim.x() - 1 );
            setValue( model->region().center().x() );
            setCaption( "Box Center X: " + kvs::String::ToString( this->value() ) );
            break;
        }
        case YCoord:
        {
            setRange( 0, dim.y() - 1 );
            setValue( model->region().center().y() );
            setCaption( "Box Center Y: " + kvs::String::ToString( this->value() ) );
            break;
        }
        case ZCoord:
        {
            setRange( 0, dim.z() - 1 );
            setValue( model->region().center().z() );
            setCaption( "Box Center Z: " + kvs::String::ToString( this->value() ) );
            break;
        }
        default: break;
        }
    }

    void sliderMoved()
    {
        const kvs::Vec3u dim = m_model->inputVolumeObject()->resolution();
        const float length = m_model->region().length();
        kvs::Vec3 center = m_model->region().center();
        switch ( m_coord )
        {
        case XCoord:
        {
            const float max_value = dim.x() - 1.0f;
            const float value = kvs::Math::Clamp( this->value(), length * 0.5f, max_value - length * 0.5f );
            setValue( kvs::Math::Round( value * 10.0 ) / 10.0 );
            setCaption( "Box Center X: " + kvs::String::ToString( this->value() ) );
            center[0] = value;
            break;
        }
        case YCoord:
        {
            const float max_value = dim.y() - 1.0f;
            const float value = kvs::Math::Clamp( this->value(), length * 0.5f, max_value - length * 0.5f );
            setValue( kvs::Math::Round( value * 10.0 ) / 10.0 );
            setCaption( "Box Center Y: " + kvs::String::ToString( this->value() ) );
            center[1] = value;
            break;
        }
        case ZCoord:
        {
            const float max_value = dim.z() - 1.0f;
            const float value = kvs::Math::Clamp( this->value(), length * 0.5f, max_value - length * 0.5f );
            setValue( kvs::Math::Round( value * 10.0 ) / 10.0 );
            setCaption( "Box Center Z: " + kvs::String::ToString( this->value() ) );
            center[2] = value;
            break;
        }
        default: break;
        }

        m_model->region().setCenter( center );
        m_view->screen().scene()->replaceObject( "Region", m_model->region().bounds( m_model->inputVolumeObject() ) );
        if ( m_model->immediate() ) m_model->fitRegressionModel();
    }
};

class BoxLengthSlider : public kvs::Slider
{
private:
    local::Model* m_model;
    local::View* m_view;

public:
    BoxLengthSlider( local::Model* model, local::View* view ):
        kvs::Slider( &view->screen() ),
        m_model( model ),
        m_view( view )
    {
        const kvs::Vec3u dim = m_model->inputVolumeObject()->resolution();
        setRange( 1, kvs::Math::Min( dim.x(), dim.y(), dim.z() ) - 1 );
        setValue( model->region().length() );

        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "Box Length: " + kvs::String::ToString( this->value() ) );
    }

    void sliderMoved()
    {
        const kvs::Vec3 dim( m_model->inputVolumeObject()->resolution() );
        const kvs::Vec3 d1 = m_model->region().center();
        const kvs::Vec3 d2 = dim - kvs::Vec3::Constant(1.0) - d1;
        const float d1_min = kvs::Math::Min( d1.x(), d1.y(), d1.z() );
        const float d2_min = kvs::Math::Min( d2.x(), d2.y(), d2.z() );
        const float max_value = kvs::Math::Min( d1_min, d2_min );
        const float value = 2.0f * kvs::Math::Clamp( this->value() * 0.5f, 1.0f, max_value );

        setValue( kvs::Math::Round( value * 10.0 ) / 10.0 );
        setCaption( "Box Length: " + kvs::String::ToString( this->value() ) );

        m_model->region().setLength( this->value() );
        m_view->screen().scene()->replaceObject( "Region", m_model->region().bounds( m_model->inputVolumeObject() ) );
        if ( m_model->immediate() ) m_model->fitRegressionModel();
    }
};

class LinearRadioButton : public kvs::RadioButton
{
private:
    local::Model* m_model;

public:
    LinearRadioButton( local::Model* model, local::View* view ):
        kvs::RadioButton( &view->screen() ),
        m_model( model )
    {
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "Linear" );
    }

    void pressed()
    {
        m_model->regression().setMethodToLinear();
    }
};

class LassoRadioButton : public kvs::RadioButton
{
private:
    local::Model* m_model;

public:
    LassoRadioButton( local::Model* model, local::View* view ):
        kvs::RadioButton( &view->screen() ),
        m_model( model )
    {
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "Lasso" );
    }

    void pressed()
    {
        m_model->regression().setMethodToLasso();
    }
};

class RidgeRadioButton : public kvs::RadioButton
{
private:
    local::Model* m_model;

public:
    RidgeRadioButton( local::Model* model, local::View* view ):
        kvs::RadioButton( &view->screen() ),
        m_model( model )
    {
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "Ridge" );
    }

    void pressed()
    {
        m_model->regression().setMethodToRidge();
    }
};

class ElasticNetRadioButton : public kvs::RadioButton
{
private:
    local::Model* m_model;

public:
    ElasticNetRadioButton( local::Model* model, local::View* view ):
        kvs::RadioButton( &view->screen() ),
        m_model( model )
    {
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "Elastic-Net" );
    }

    void pressed()
    {
        m_model->regression().setMethodToElasticNet();
    }
};

class ComplexitySlider : public kvs::Slider
{
private:
    local::Model* m_model;

public:
    ComplexitySlider( local::Model* model, local::View* view ):
        kvs::Slider( &view->screen() ),
        m_model( model )
    {
        setRange( 0.0, 1.0 );
        setValue( 1.0 );
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "Complexity: " + kvs::String::ToString( this->value() ) );
    }

    void sliderMoved()
    {
        setValue( kvs::Math::Round( this->value() * 100.0 ) / 100.0 );
        setCaption( "Complexity: " + kvs::String::ToString( this->value() ) );
    }

    void sliderReleased()
    {
        m_model->regression().setComplexity( this->value() );
    }
};

class L1RatioSlider : public kvs::Slider
{
private:
    local::Model* m_model;

public:
    L1RatioSlider( local::Model* model, local::View* view ):
        kvs::Slider( &view->screen() ),
        m_model( model )
    {
        setRange( 0.0, 1.0 );
        setValue( 0.5 );
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "L1 Ratio: " + kvs::String::ToString( this->value() ) );
    }

    void sliderMoved()
    {
        setValue( kvs::Math::Round( this->value() * 100.0 ) / 100.0 );
        setCaption( "L1 Ratio: " + kvs::String::ToString( this->value() ) );
    }

    void sliderReleased()
    {
        m_model->regression().setL1Ratio( this->value() );
    }
};

class PointsSlider : public kvs::Slider
{
private:
    local::Model* m_model;

public:
    PointsSlider( local::Model* model, local::View* view ):
        kvs::Slider( &view->screen() ),
        m_model( model )
    {
        setRange( 0.0, 200.0 );
        setValue( 100.0 );
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "Points: " + kvs::String::ToString( kvs::Math::Round( this->value() ) ) );
    }

    void sliderMoved()
    {
        setValue( kvs::Math::Round( this->value() ) );
        setCaption( "Points: " + kvs::String::ToString( this->value() ) );

        m_model->region().setNumberOfSamplingPoints( this->value() );
        if ( m_model->immediate() ) m_model->fitRegressionModel();
    }
};

class ImmediateCheckBox : public kvs::CheckBox
{
private:
    local::Model* m_model;

public:
    ImmediateCheckBox( local::Model* model, local::View* view ):
        kvs::CheckBox( &view->screen() ),
        m_model( model )
    {
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "Immediate" );
    }

    void stateChanged()
    {
        m_model->setEnabledImmediate( this->state() );
    }
};

class ApplyButton : public kvs::PushButton
{
private:
    local::Model* m_model;

public:
    ApplyButton( local::Model* model, local::View* view ):
        kvs::PushButton( &view->screen() ),
        m_model( model )
    {
        setCaption( "Apply" );
    }

    void pressed()
    {
        m_model->fitRegressionModel();
    }
};

} // end of namespace UI

} // end of namespace local

#pragma once
#include "Model.h"
#include "View.h"
#include <kvs/ScreenBase>
#include <kvs/Slider>
#include <kvs/RadioButton>
#include <kvs/RadioButtonGroup>
#include <kvs/PushButton>
#include <kvs/String>
#include "Model.h"
#include "View.h"


namespace local
{

namespace UI
{

class BoxCenterSlider : public kvs::Slider
{
private:
    local::Model* m_model;

public:
    BoxCenterSlider( local::Model* model, local::View* view ):
        kvs::Slider( &view->screen() ),
        m_model( model )
    {
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "Box Center" );
    }
};

class BoxLengthSlider : public kvs::Slider
{
private:
    local::Model* m_model;

public:
    BoxLengthSlider( local::Model* model, local::View* view ):
        kvs::Slider( &view->screen() ),
        m_model( model )
    {
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "Box Length" );
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

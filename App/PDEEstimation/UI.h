#pragma once
#include <kvs/ScreenBase>
#include <kvs/Label>
#include <kvs/Slider>
#include <kvs/RadioButton>
#include <kvs/RadioButtonGroup>
#include <kvs/PushButton>
#include "Regression.h"


namespace local
{

namespace UI
{

class Info : public kvs::Label
{
public:
    Info( kvs::ScreenBase* screen, const local::Regression<float>& regression ):
        kvs::Label( screen )
    {
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setText( "R2: %7.3f", regression.r2() );
        for ( size_t i = 0; i < regression.coef().size(); i++ )
        {
            addText( "X%d: %7.3f (t-value: %7.3f)", i, regression.coef()[i], regression.tValues()[i] );
        }
    }
};

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

class ApplyButton : public kvs::PushButton
{
public:
    ApplyButton( kvs::glut::Screen* screen ): kvs::PushButton( screen )
    {
//        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setCaption( "Apply" );
    }
};

} // end of namespace UI

} // end of namespace local

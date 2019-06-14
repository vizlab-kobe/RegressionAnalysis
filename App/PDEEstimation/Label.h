#pragma once
#include <kvs/Label>
#include <kvs/ScreenBase>
#include "Regression.h"

namespace local
{

class Label : public kvs::Label
{
public:
    Label( kvs::ScreenBase* screen, const local::Regression<float>& regression ):
        kvs::Label( screen )
    {
        kvs::Font font( kvs::Font::Sans, 24 );
        font.setEnabledShadow( true );
        font.setShadowDistance( 4.0f );
        setFont( font );

        setText( "R2: %.3f", regression.r2() );
        addText( "Intercept: %.3f (t: %.3f)", regression.coef()[0], regression.tValues()[0] );
        for ( size_t i = 1; i < regression.coef().size(); i++ )
        {
            addText( "X%d: %.3f (t: %.3f)", i, regression.coef()[i], regression.tValues()[i] );
        }
    }
};

} // end of namespace local

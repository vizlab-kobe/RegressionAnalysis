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
        setFont( kvs::Font( kvs::Font::Sans, 24 ) );
        setText( "R2: %7.3f", regression.r2() );
        for ( size_t i = 0; i < regression.coef().size(); i++ )
        {
            addText( "X%d: %7.3f (t-value: %7.3f)", i, regression.coef()[i], regression.tValues()[i] );
        }
    }
};

} // end of namespace local

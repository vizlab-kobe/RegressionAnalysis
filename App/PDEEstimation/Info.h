#pragma once
#include <kvs/glut/Screen>
#include <kvs/Label>
#include "Model.h"


namespace local
{

class Info : public kvs::Label
{
private:
    local::Model* m_model;

public:
    Info( kvs::glut::Screen* screen, local::Model* model ):
        kvs::Label( screen ),
        m_model( model )
    {
        setFont( kvs::Font( kvs::Font::Sans, 22 ) );
        setMargin( 10 );
    }

private:
    void screenResized()
    {
        this->painter().begin( this->screen() );
        const kvs::FontMetrics metrics = this->painter().fontMetrics();
        const size_t character_height = metrics.height();
        this->painter().end();
        const size_t nlines = m_model->independentVolumeObjects().size() + 2;
        const size_t x = 0;
        const size_t y = this->screen()->height() - ( nlines * character_height + margin() * 2 );
        setPosition( x, y );
    }

    void screenUpdated()
    {
        if ( m_model->regression().coef().size() > 0 )
        {
            setText( "R2: %7.3f", m_model->regression().r2() );
            for ( size_t i = 0; i < m_model->regression().coef().size(); i++ )
            {
                addText( "X%d: %7.3f (t-value: %7.3f)", i,
                         m_model->regression().coef()[i],
                         m_model->regression().tValues()[i] );
            }
        }
    }
};

} // end of namespace local

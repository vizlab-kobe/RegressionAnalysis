#pragma once
#include "Model.h"
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/Bounds>
#include <kvs/RayCastingRenderer>


namespace local
{

class View
{
private:
    local::Model* m_model;
    kvs::glut::Screen m_screen;

public:
    View( kvs::glut::Application* app, local::Model* model ):
        m_model( model ),
        m_screen( app )
    {
        this->setup();
        this->show();
    }

    kvs::glut::Screen& screen() { return m_screen; }

    void setup()
    {
        m_screen.setBackgroundColor( kvs::RGBColor::White() );
        m_screen.setSize( 800, 600 );

        kvs::StructuredVolumeObject* dep_volume = new kvs::StructuredVolumeObject();
        dep_volume->shallowCopy( *m_model->dependentVolumeObject() );
        m_screen.registerObject( m_model->region().bounds( dep_volume ) );
        m_screen.registerObject( dep_volume, new kvs::Bounds );
        m_screen.registerObject( dep_volume, new kvs::glsl::RayCastingRenderer );
    }

    void show()
    {
        m_screen.show();
    }

    void redraw()
    {
        m_screen.redraw();
    }
};

} // end of namespace local

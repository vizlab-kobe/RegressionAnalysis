#pragma once
#include <kvs/LineObject>
#include <kvs/StructuredVolumeObject>
#include <kvs/MersenneTwister>
#include <kvs/UniformGrid>
#include <kvs/Vector3>
#include <vector>


namespace local
{

class Region
{
private:
    kvs::Vec3 m_center;
    kvs::Real32 m_length;
    size_t m_npoints;

public:
    Region(): m_center( 0, 0, 0 ), m_length( 1 ), m_npoints( 10 ) {}

    const kvs::Vec3& center() const { return m_center; }
    kvs::Real32 length() const { return m_length; }
    size_t numberOfSamplingPoints() const { return m_npoints; }

    void setCenter( const kvs::Vec3& center ) { m_center = center; }
    void setLength( const kvs::Real32 length ) { m_length = length; }
    void setNumberOfSamplingPoints( const size_t npoints ) { m_npoints = npoints; }

    kvs::ValueArray<float> sampling( const kvs::StructuredVolumeObject* volume )
    {
        kvs::ValueArray<float> values( m_npoints );

        const unsigned long seed = 0;
        kvs::MersenneTwister R( seed );
        kvs::UniformGrid grid( volume );
        const kvs::Vec3 corner = m_center - kvs::Vec3::All( m_length * 0.5f );
        for ( size_t i = 0; i < m_npoints; i++ )
        {
            const kvs::Vec3 p = corner + kvs::Vec3( R(), R(), R() ) * m_length;
            grid.bind( p );
            values[i] = grid.scalar();
        }

        return values;
    }

    kvs::LineObject* bounds( const kvs::StructuredVolumeObject* parent )
    {
        const kvs::Vec3 min = m_center - kvs::Vec3::All( m_length * 0.5f );
        const kvs::Vec3 max = m_center + kvs::Vec3::All( m_length * 0.5f );

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

        kvs::LineObject* object = new kvs::LineObject();
        object->setName("Region");
        object->setMinMaxObjectCoords( parent->minObjectCoord(), parent->maxObjectCoord() );
        object->setMinMaxExternalCoords( parent->minExternalCoord(), parent->maxExternalCoord() );
        object->setLineTypeToSegment();
        object->setColorTypeToLine();
        object->setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
        object->setConnections( kvs::ValueArray<kvs::UInt32>( connects ) );
        object->setColor( kvs::RGBColor::Black() );
        object->setSize( 2 );
        return object;
    }
};

} // end of namespace local

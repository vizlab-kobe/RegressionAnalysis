#pragma once
#include <kvs/LineObject>
#include <kvs/StructuredVolumeObject>
#include <kvs/Vector3>
#include <vector>


namespace local
{

inline kvs::LineObject* Box(
    const kvs::StructuredVolumeObject* object,
    const kvs::Vec3 center,
    const kvs::Real32 length )
{
    const kvs::Vec3 min = center - kvs::Vec3::All( length * 0.5f );
    const kvs::Vec3 max = center + kvs::Vec3::All( length * 0.5f );

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

    kvs::LineObject* line_object = new kvs::LineObject();
    line_object->setName("Box");
    line_object->setMinMaxObjectCoords( object->minObjectCoord(), object->maxObjectCoord() );
    line_object->setMinMaxExternalCoords( object->minExternalCoord(), object->maxExternalCoord() );
    line_object->setLineTypeToSegment();
    line_object->setColorTypeToLine();
    line_object->setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    line_object->setConnections( kvs::ValueArray<kvs::UInt32>( connects ) );
    line_object->setColor( kvs::RGBColor::Black() );
    line_object->setSize( 2 );

    return line_object;
}

} // end of namespace local

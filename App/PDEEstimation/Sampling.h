#pragma once
#include <kvs/ValueArray>
#include <kvs/Vector3>
#include <kvs/MersenneTwister>
#include <kvs/UniformGrid>


namespace local
{

inline kvs::ValueArray<float> Sampling(
    const size_t npoints, // number of generating points
    const kvs::StructuredVolumeObject* volume, // input volume data
    const kvs::Vec3 center, // center of sampling box
    const kvs::Real32 length ) // length of sampling box
{
    kvs::ValueArray<float> values( npoints );

    const unsigned long seed = 0;
    kvs::MersenneTwister R( seed );
    kvs::UniformGrid grid( volume );
    const kvs::Vec3 corner = center - kvs::Vec3::All( length * 0.5f );
    for ( size_t i = 0; i < npoints; i++ )
    {
        const kvs::Vec3 p = corner + kvs::Vec3( R(), R(), R() ) * length;
        grid.bind( p );
        values[i] = grid.scalar();
    }

    return values;
}

} // end of namespace local

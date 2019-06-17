#pragma once
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVectorToScalar>
#include <kvs/StructuredExtractScalar>
#include <kvs/Assert>


namespace local
{

namespace Op
{

inline kvs::StructuredVolumeObject* Abs( kvs::StructuredVolumeObject* volume )
{
    return new kvs::StructuredVectorToScalar( volume );
}

inline kvs::StructuredVolumeObject* U( kvs::StructuredVolumeObject* volume )
{
    KVS_ASSERT( volume->veclen() == 3 );
    return new kvs::StructuredExtractScalar( volume, 0 );
}

inline kvs::StructuredVolumeObject* V( kvs::StructuredVolumeObject* volume )
{
    KVS_ASSERT( volume->veclen() == 3 );
    return new kvs::StructuredExtractScalar( volume, 1 );
}

inline kvs::StructuredVolumeObject* W( kvs::StructuredVolumeObject* volume )
{
    KVS_ASSERT( volume->veclen() == 3 );
    return new kvs::StructuredExtractScalar( volume, 2 );
}

inline kvs::StructuredVolumeObject* dT(
    const kvs::StructuredVolumeObject* volume0,
    const kvs::StructuredVolumeObject* volume1,
    const float delta = 1.0f )
{
    KVS_ASSERT( volume0->veclen() == 1 );
    KVS_ASSERT( volume0->veclen() == volume1->volume1() );
    KVS_ASSERT( volume0->resolution() == volume1->resolution() );

    const kvs::ValueArray<kvs::Real32>& S0 = volume0->values().asValueArray<kvs::Real32>();
    const kvs::ValueArray<kvs::Real32>& S1 = volume1->values().asValueArray<kvs::Real32>();
    const kvs::Vec3u dim = volume0->resolution();

    kvs::ValueArray<kvs::Real32> values( S0.size() );
    kvs::Real32* pvalues = values.data();
    for ( size_t k = 0, index = 0; k < dim.z(); k++ )
    {
        for ( size_t j = 0; j < dim.y(); j++ )
        {
            for ( size_t i = 0; i < dim.x(); i++, index++ )
            {
                const float dt = 2.0f * delta;
                const float dS = S1[index] - S0[index];
                *(pvalues++) = dS / dt;
            }
        }
    }

    kvs::StructuredVolumeObject* dvolume = new kvs::StructuredVolumeObject();
    dvolume->setGridTypeToUniform();
    dvolume->setResolution( volume0->resolution() );
    dvolume->setVeclen( 1 );
    dvolume->setValues( values );
    dvolume->updateMinMaxValues();
    dvolume->updateMinMaxCoords();
    return dvolume;
}

inline kvs::StructuredVolumeObject* dX(
    const kvs::StructuredVolumeObject* volume,
    const float delta = 1.0f )
{
    KVS_ASSERT( volume->veclen() == 1 );

    const kvs::ValueArray<kvs::Real32>& S = volume->values().asValueArray<kvs::Real32>();
    const kvs::Vec3u dim = volume->resolution();

    kvs::ValueArray<kvs::Real32> values( S.size() );
    kvs::Real32* pvalues = values.data();
    for ( size_t k = 0; k < dim.z(); k++ )
    {
        for ( size_t j = 0; j < dim.y(); j++ )
        {
            for ( size_t i = 0; i < dim.x(); i++ )
            {
                const size_t x0 = i == 0 ? i : i - 1;
                const size_t x1 = i == dim.x() - 1 ? i : i + 1;
                const float dx = 2.0f * delta;

                const kvs::Real32 S0 = S[ k * dim.x() * dim.y() + j * dim.x() + x0 ];
                const kvs::Real32 S1 = S[ k * dim.x() * dim.y() + j * dim.x() + x1 ];
                const kvs::Real32 dS = S1 - S0;
                *(pvalues++) = dS / dx;
            }
        }
    }

    kvs::StructuredVolumeObject* dvolume = new kvs::StructuredVolumeObject();
    dvolume->setGridTypeToUniform();
    dvolume->setResolution( volume->resolution() );
    dvolume->setVeclen( 1 );
    dvolume->setValues( values );
    dvolume->updateMinMaxValues();
    dvolume->updateMinMaxCoords();
    return dvolume;
}

inline kvs::StructuredVolumeObject* dY(
    const kvs::StructuredVolumeObject* volume,
    const float delta = 1.0f )
{
    KVS_ASSERT( volume->veclen() == 1 );

    const kvs::ValueArray<kvs::Real32>& S = volume->values().asValueArray<kvs::Real32>();
    const kvs::Vec3u dim = volume->resolution();

    kvs::ValueArray<kvs::Real32> values( S.size() );
    kvs::Real32* pvalues = values.data();
    for ( size_t k = 0; k < dim.z(); k++ )
    {
        for ( size_t j = 0; j < dim.y(); j++ )
        {
            const size_t y0 = j == 0 ? j : j - 1;
            const size_t y1 = j == dim.y() - 1 ? j : j + 1;
            const float dy = 2.0f * delta;
            for ( size_t i = 0; i < dim.x(); i++ )
            {
                const kvs::Real32 S0 = S[ k * dim.x() * dim.y() + y0 * dim.x() + i ];
                const kvs::Real32 S1 = S[ k * dim.x() * dim.y() + y1 * dim.x() + i ];
                const kvs::Real32 dS = S1 - S0;
                *(pvalues++) = dS / dy;
            }
        }
    }

    kvs::StructuredVolumeObject* dvolume = new kvs::StructuredVolumeObject();
    dvolume->setGridTypeToUniform();
    dvolume->setResolution( volume->resolution() );
    dvolume->setVeclen( 1 );
    dvolume->setValues( values );
    dvolume->updateMinMaxValues();
    dvolume->updateMinMaxCoords();
    return dvolume;
}

inline kvs::StructuredVolumeObject* dZ(
    const kvs::StructuredVolumeObject* volume,
    const float delta = 1.0f )
{
    KVS_ASSERT( volume->veclen() == 1 );

    const kvs::ValueArray<kvs::Real32>& S = volume->values().asValueArray<kvs::Real32>();
    const kvs::Vec3u dim = volume->resolution();

    kvs::ValueArray<kvs::Real32> values( S.size() );
    kvs::Real32* pvalues = values.data();
    for ( size_t k = 0; k < dim.z(); k++ )
    {
        const size_t z0 = k == 0 ? k : k - 1;
        const size_t z1 = k == dim.z() - 1 ? k : k + 1;
        const float dz = 2.0f * delta;
        for ( size_t j = 0; j < dim.y(); j++ )
        {
            for ( size_t i = 0; i < dim.x(); i++ )
            {
                const kvs::Real32 S0 = S[ z0 * dim.x() * dim.y() + j * dim.x() + i ];
                const kvs::Real32 S1 = S[ z1 * dim.x() * dim.y() + j * dim.x() + i ];
                const kvs::Real32 dS = S1 - S0;
                *(pvalues++) = dS / dz;
            }
        }
    }

    kvs::StructuredVolumeObject* dvolume = new kvs::StructuredVolumeObject();
    dvolume->setGridTypeToUniform();
    dvolume->setResolution( volume->resolution() );
    dvolume->setVeclen( 1 );
    dvolume->setValues( values );
    dvolume->updateMinMaxValues();
    dvolume->updateMinMaxCoords();
    return dvolume;
}

} // end of namespace Op

} // end of namespace local

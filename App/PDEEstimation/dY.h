#pragma once
#include <kvs/StructuredVolumeObject>
#include <kvs/Assert>


namespace local
{

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

} // end of namespace local

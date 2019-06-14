#pragma once
#include <kvs/StructuredVolumeObject>
#include <kvs/Assert>


namespace local
{

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

} // end of namespace local

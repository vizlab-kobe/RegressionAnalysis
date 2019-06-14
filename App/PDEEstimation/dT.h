#pragma once
#include <kvs/StructuredVolumeObject>
#include <kvs/Assert>


namespace local
{

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

} // end of namespace local

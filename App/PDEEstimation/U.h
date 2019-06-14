#pragma once
#include <kvs/StructuredExtractScalar>
#include <kvs/Assert>


namespace local
{

inline kvs::StructuredVolumeObject* U( kvs::StructuredVolumeObject* volume )
{
    KVS_ASSERT( volume->veclen() == 3 );
    return new kvs::StructuredExtractScalar( volume, 0 );
}

} // end of namespace local


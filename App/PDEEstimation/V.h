#pragma once
#include <kvs/StructuredExtractScalar>
#include <kvs/Assert>


namespace local
{

inline kvs::StructuredVolumeObject* V( kvs::StructuredVolumeObject* volume )
{
    KVS_ASSERT( volume->veclen() == 3 );
    return new kvs::StructuredExtractScalar( volume, 1 );
}

} // end of namespace local

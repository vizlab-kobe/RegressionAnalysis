#pragma once
#include <kvs/StructuredVectorToScalar>


namespace local
{

inline kvs::StructuredVolumeObject* Abs( kvs::StructuredVolumeObject* volume )
{
    return new kvs::StructuredVectorToScalar( volume );
}

} // end of namespace local

#pragma once
#include "Input.h"
#include "Regression.h"
#include "Region.h"
#include "Op.h"
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/TornadoVolumeData>


namespace local
{

class Model
{
public:
    typedef kvs::StructuredVolumeObject Volume;
    typedef std::vector<Volume*> VolumeList;

private:
    const local::Input& m_input;
    local::Regression<float> m_regression;
    local::Region m_region;
    Volume* m_input_volume;
    Volume* m_dep_volume;
    VolumeList m_indep_volumes;

public:
    Model( const local::Input& input ):
        m_input( input )
    {
        if ( input.filename.empty() )
        {
            const kvs::Vec3u resolution( 32, 32, 32 );
            m_input_volume = new kvs::TornadoVolumeData( resolution );
        }
        else
        {
            m_input_volume = new kvs::StructuredVolumeImporter( input.filename );
        }

        const kvs::Vec3 min_coord = m_input_volume->minObjectCoord();
        const kvs::Vec3 max_coord = m_input_volume->maxObjectCoord();
        const kvs::Vec3u dim = m_input_volume->resolution();
        const kvs::Vec3 center = ( min_coord + max_coord ) * 0.5;
        const kvs::Real32 length = kvs::Math::Min( dim.x(), dim.y(), dim.z() ) * 0.5;
        const size_t npoints = 100;
        const local::Regression<float>::Method method = local::Regression<float>::Linear;
        const float complexity = 1.0;
        const float l1_ratio = 0.5;

        m_region.setCenter( center );
        m_region.setLength( length );
        m_region.setNumberOfSamplingPoints( npoints );
        m_regression.setMethod( method );
        m_regression.setComplexity( complexity );
        m_regression.setL1Ratio( l1_ratio );
        this->initializeRegressionModel( m_input_volume );
    }

    const local::Input& input() const { return m_input; }
    local::Regression<float>& regression() { return m_regression; }
    local::Region& region() { return m_region; }

    const Volume* inputVolumeObject() const { return m_input_volume; }
    const Volume* dependentVolumeObject() const { return m_dep_volume; }
    const VolumeList& independentVolumeObjects() const { return m_indep_volumes; }
    const Volume* independentVolumeObject( const size_t index = 0 ) const { return m_indep_volumes[index]; }

    void initializeRegressionModel( const kvs::StructuredVolumeObject* volume )
    {
        Volume* S = local::Op::Abs( volume );
        Volume* U = local::Op::U( volume );
        Volume* V = local::Op::V( volume );
        Volume* W = local::Op::W( volume );

        Volume* dU_dX = local::Op::dX( U );
        Volume* dU_dY = local::Op::dY( U );
        Volume* d2U_dX2 = local::Op::dX( dU_dX );
        Volume* d2U_dXdY = local::Op::dY( dU_dX );

        m_dep_volume = S;
        m_indep_volumes.push_back( dU_dX );
        m_indep_volumes.push_back( dU_dY );
        m_indep_volumes.push_back( d2U_dX2 );
        m_indep_volumes.push_back( d2U_dXdY );

        delete U;
        delete V;
        delete W;
    }

    void clearRegressionModel()
    {
        if ( m_dep_volume ) { delete m_dep_volume; }
        for ( size_t i = 0; i < m_indep_volumes.size(); i++ )
        {
            if ( m_indep_volumes[i] ) { delete m_indep_volumes[i]; }
        }
        VolumeList().swap( m_indep_volumes );
    }

    void fitRegressionModel( const bool update_sampling_point = true )
    {
        if ( update_sampling_point )
        {
//            m_regression.resetVariables();

            const kvs::ValueArray<float> Y = m_region.sampling( m_dep_volume );
            m_regression.setDependentVariable( Y );

            kvs::ValueTable<float> X;
            for ( size_t i = 0; i < m_indep_volumes.size(); i++ )
            {
                const kvs::ValueArray<float> Xi = m_region.sampling( m_indep_volumes[i] );
                X.pushBackColumn( Xi );
            }
            m_regression.setIndependentVariables( X );
        }
        m_regression.fit();
    }
};

} // end of namespace local

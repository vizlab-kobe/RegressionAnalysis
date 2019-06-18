#pragma once
#include <kvs/ValueArray>
#include <kvs/ValueTable>
#include <kvs/Vector>
#include <kvs/Type>


namespace RegressionAnalysis
{

namespace sklearn
{

template <typename T>
class ElasticNetRegression
{
private:
    size_t m_dof; ///< degree of freedom
    kvs::Vector<T> m_coef;
    kvs::Real64 m_r2; ///< R square ()
    kvs::Real64 m_adjusted_r2; ///< adjusted R square
    kvs::Vector<T> m_standard_errors; ///< standard errors
    kvs::Vector<T> m_t_values; ///< t-values
    kvs::Vector<T> m_p_values; ///< p-values
    kvs::Real32 m_complexity; ///< model complexity that multiples the L1 term
    kvs::Real32 m_l1_ratio; ///< mixing parameter for the L1 and L2 terms
    bool m_normalize; ///< if true, independent variables will be normalized

public:
    ElasticNetRegression();
    ElasticNetRegression( const kvs::ValueArray<T>& dep, const kvs::ValueTable<T>& indep );

    void setComplexity( const kvs::Real32 complexity ) { m_complexity = complexity; }
    void setL1Ratio( const kvs::Real32 l1_ratio ) { m_l1_ratio = l1_ratio; }
    void setEnabledNormalize( const bool enable ) { m_normalize = enable; }
    void enableNormalize() { setEnabledNormalize( true ); }
    void disableNormalize() { setEnabledNormalize( false ); }

    const kvs::Vector<T>& coef() const { return m_coef; }
    kvs::Real64 r2() const { return m_r2; }
    kvs::Real64 adjustedR2() const { return m_adjusted_r2; }
    const kvs::Vector<T>& standardErrors() const { return m_standard_errors; }
    const kvs::Vector<T>& tValues() const { return m_t_values; }
    const kvs::Vector<T>& pValues() const { return m_p_values; }
    kvs::Real32 complexity() const { return m_complexity; }
    kvs::Real32 l1Ratio() const { return m_l1_ratio; }
    bool normalize() const { return m_normalize; }

    void fit( const kvs::ValueArray<T>& dep, const kvs::ValueTable<T>& indep );
    void test();
};

} // end of namespace sklearn

} // end of namespace RegressionAnalysis

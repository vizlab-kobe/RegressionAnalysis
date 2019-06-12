#pragma once
#include <kvs/ValueArray>
#include <kvs/ValueTable>
#include <kvs/Vector>
#include <kvs/Type>


namespace local
{

namespace sklearn
{

template <typename T>
class RidgeRegression
{
private:
    kvs::Vector<T> m_coef;
    kvs::Real64 m_rss; ///< residual sum of squares (RSS)
    kvs::Real64 m_r2; ///< R square ()
    kvs::Vector<T> m_t_values; // T-values

public:
    RidgeRegression() {}
    RidgeRegression( const kvs::ValueArray<T>& dep, const kvs::ValueTable<T>& indep );

    const kvs::Vector<T>& coef() const { return m_coef; }
    kvs::Real64 rss() const { return m_rss; }
    kvs::Real64 r2() const { return m_r2; }
    const kvs::Vector<T>& tValues() const { return m_t_values; }

    void fit( const kvs::ValueArray<T>& dep, const kvs::ValueTable<T>& indep );
};

} // end of namespace sklearn

} // end of namespace local

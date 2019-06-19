#include <kvs/MersenneTwister>
#include <kvs/ValueArray>
#include <kvs/ValueTable>
#include <kvs/Timer>
#include <iostream>
#include <fstream>
#include <iterator>
#include <RegressionAnalysis/Lib/RidgeRegression.h>
#include <kvs/RidgeRegression>


template <typename T>
std::ostream& operator << ( std::ostream& os, const kvs::ValueArray<T>& values )
{
    std::copy( values.begin(), values.end(), std::ostream_iterator<T>( os, ", " ) );
    return os;
}

template <typename T>
kvs::ValueArray<T> Random( const size_t n, const unsigned long seed )
{
    kvs::MersenneTwister engine( seed );
    kvs::ValueArray<T> values( n );
    for ( size_t i = 0; i < n; i++ )
    {
        values[i] = static_cast<T>( engine.randInteger(100) );
    }
    return values;
}

template <typename Method>
void Regression( const kvs::ValueArray<float>& dep, const kvs::ValueTable<float>& indep )
{
    Method method;
    method.setComplexity( 1.0 );

    kvs::Timer timer( kvs::Timer::Start );
    method.fit( dep, indep );
    timer.stop();
    std::cout << "    Coef: " << method.coef() << std::endl;
    std::cout << "    R2: " << method.r2() << std::endl;
    std::cout << "        Fitting time:" << timer.msec() << " [msec]" << std::endl;

    timer.start();
    method.test();
    timer.stop();
    std::cout << "    Standard errors: " << method.standardErrors() << std::endl;
    std::cout << "    T-values: " << method.tValues() << std::endl;
    std::cout << "    P-values: " << method.pValues() << std::endl;
    std::cout << "        Testing time:" << timer.msec() << " [msec]" << std::endl;
}

int main( int argc, char** argv )
{
    const size_t n = 10; // data size
    const size_t k = 2; // number of independent variables

    kvs::ValueArray<float> Y = Random<float>( n, 0 );
    kvs::ValueArray<float> X0 = Random<float>( n, 1 );
    kvs::ValueArray<float> X1 = Random<float>( n, 2 );

    std::cout << "INPUT DATA" << std::endl;
    std::cout << "    Y  = {" << Y << "}" << std::endl;
    std::cout << "    X0 = {" << X0 << "}" << std::endl;
    std::cout << "    X1 = {" << X1 << "}" << std::endl;

    kvs::ValueArray<float> dep( Y );
    kvs::ValueTable<float> indep( n, k );
    indep[0] = X0;
    indep[1] = X1;

    std::cout << "REGRESSION (RegressionAnalysis::sklearn::RidgeRegression)" << std::endl;
    Regression<RegressionAnalysis::sklearn::RidgeRegression<float> >( dep, indep );
    Regression<RegressionAnalysis::sklearn::RidgeRegression<float> >( dep, indep );

    std::cout << "REGRESSION (local::RidgeRegression)" << std::endl;
    Regression<kvs::RidgeRegression<float> >( dep, indep );

    return 0;
}

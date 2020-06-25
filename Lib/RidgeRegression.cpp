#include "RidgeRegression.h"
#include <kvs/python/Interpreter>
#include <kvs/python/Module>
#include <kvs/python/Callable>
#include <kvs/python/Table>
#include <kvs/python/List>
#include <kvs/python/Array>
#include <kvs/python/Float>
#include <kvs/python/Bool>
#include <kvs/python/NumPy>
#include <kvs/StudentTDistribution>


namespace
{

class Function
{
public:
    static Function& Instance()
    {
        static Function function;
        return function;
    }

    kvs::python::Callable callable()
    {
        return kvs::python::Callable( m_module->dict().find("main") );
    }

private:
    kvs::python::Interpreter* m_interpreter;
    kvs::python::Module* m_module;

    Function()
    {
        m_interpreter = new kvs::python::Interpreter( true );
        m_module = new kvs::python::Module( this->code(), "RidgeRegression" );
    }

    ~Function()
    {
        delete m_interpreter;
        delete m_module;
    }

    const std::string code()
    {
        std::stringstream code;
        code << "import numpy as np" << std::endl;
        code << "from sklearn.linear_model import Ridge" << std::endl;
        code << "def main( X, y, alpha, normalize ):" << std::endl;
        code << "    model = Ridge( alpha=alpha, normalize=normalize )" << std::endl;
        code << "    model.fit( X, y )" << std::endl;
        code << "    r2 = model.score( X, y )" << std::endl;
        code << "    coef = np.append( model.intercept_, model.coef_ )" << std::endl;
        code << "    nx = np.append( np.ones( ( len(X), 1 ) ), X, axis=1 )" << std::endl;
        code << "    rss = sum( ( y-model.predict(X) )**2 )" << std::endl;
        code << "    mse = rss / ( len(nx) - len(nx[0]) )" << std::endl;
        code << "    var = mse * np.linalg.inv( np.dot( nx.T, nx ) ).diagonal()" << std::endl;
        code << "    se = np.sqrt( var.astype( np.float32 ) )" << std::endl;
        code << "    return [ coef, r2, se ]" << std::endl;
        return code.str();
    }
};

}


namespace RegressionAnalysis
{

namespace sklearn
{

template <typename T>
RidgeRegression<T>::RidgeRegression():
    m_dof( 0 ),
    m_r2( 0.0 ),
    m_adjusted_r2( 0.0 ),
    m_complexity( 1.0 ),
    m_normalize( false )
{
}

template <typename T>
RidgeRegression<T>::RidgeRegression( const kvs::ValueArray<T>& dep, const kvs::ValueTable<T>& indep ):
    m_dof( 0 ),
    m_r2( 0.0 ),
    m_adjusted_r2( 0.0 ),
    m_complexity( 1.0 ),
    m_normalize( false )
{
    this->fit( dep, indep );
}

template <typename T>
void RidgeRegression<T>::fit( const kvs::ValueArray<T>& dep, const kvs::ValueTable<T>& indep )
{
    KVS_ASSERT( dep.size() == indep.column(0).size() );

    // Calable python function
    kvs::python::Callable function = ::Function::Instance().callable();

    // Arguments
    kvs::python::Table X( indep );
    kvs::python::Array y( dep );
    kvs::python::Float alpha( m_complexity );
    kvs::python::Bool normalize( m_normalize );
    kvs::python::Tuple args( 4 );
    args.set( 0, X );
    args.set( 1, y );
    args.set( 2, alpha );
    args.set( 3, normalize );

    // Execute the python function
    kvs::python::List list = function.call( args );
    const kvs::ValueArray<T> coef = kvs::ValueArray<T>( kvs::python::Array( list[0] ) );
    m_coef = kvs::Vector<T>( coef.size(), coef.data() );
    m_r2 = kvs::python::Float( list[1] );

    const kvs::ValueArray<T> se = kvs::ValueArray<T>( kvs::python::Array( list[2] ) );
    m_standard_errors = kvs::Vector<T>( se.size(), se.data() );

    const size_t n = dep.size();
    const size_t k = indep.columnSize();
    m_dof = n - k - 1;
    m_adjusted_r2 = 1.0 - ( 1.0 - m_r2 ) * ( n - 1.0 ) / m_dof;
}

template <typename T>
void RidgeRegression<T>::test()
{
    m_t_values.resize( m_coef.size() );
    m_p_values.resize( m_coef.size() );
    kvs::StudentTDistribution tdist( m_dof );
    for ( size_t i = 0; i < m_coef.size(); i++ )
    {
        m_t_values[i] = m_coef[i] / m_standard_errors[i];
        m_p_values[i] = 2.0 * ( 1.0 - tdist.cdf( m_t_values[i] ) );
    }
}

// template instantiation
template class RidgeRegression<float>;

} // end of namespace sklearn

} // end of namespace RegressionAnalysis

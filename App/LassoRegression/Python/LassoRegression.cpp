#include "LassoRegression.h"
#include <kvs/python/Interpreter>
#include <kvs/python/Module>
#include <kvs/python/Callable>
#include <kvs/python/Table>
#include <kvs/python/List>
#include <kvs/python/Array>
#include <kvs/python/Float>
#include <kvs/python/NumPy>


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
        m_module = new kvs::python::Module( this->code(), "Regression" );
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
        code << "from sklearn.linear_model import Lasso" << std::endl;
        code << "def main( X, y ):" << std::endl;
        code << "    model = Lasso( alpha=1.0 )" << std::endl;
        code << "    model.fit( X, y )" << std::endl;
        code << "    r2 = model.score( X, y )" << std::endl;
        code << "    coef = np.append( model.intercept_, model.coef_ )" << std::endl;
        code << "    nx = np.append( np.ones( ( len(X), 1 ) ), X, axis=1 )" << std::endl;
        code << "    rss = sum( ( y-model.predict(X) )**2 )" << std::endl;
        code << "    mse = rss / ( len(nx) - len(nx[0]) )" << std::endl;
        code << "    var = mse * np.linalg.inv( np.dot( nx.T, nx ) ).diagonal()" << std::endl;
        code << "    se = np.sqrt( var.astype( np.float32 ) )" << std::endl;
        code << "    tvals = coef / se" << std::endl;
        code << "    return [ coef, r2, rss, tvals ]" << std::endl;
        return code.str();
    }
};

}


namespace local
{

namespace sklearn
{

template <typename T>
LassoRegression<T>::LassoRegression( const kvs::ValueArray<T>& dep, const kvs::ValueTable<T>& indep )
{
    this->fit( dep, indep );
}

template <typename T>
void LassoRegression<T>::fit( const kvs::ValueArray<T>& dep, const kvs::ValueTable<T>& indep )
{
    KVS_ASSERT( dep.size() == indep.column(0).size() );

    // Calable python function
    kvs::python::Callable function = ::Function::Instance().callable();

    // Arguments
    kvs::python::Table X( indep );
    kvs::python::Array y( dep );
    kvs::python::Tuple args( 2 );
    args.set( 0, X );
    args.set( 1, y );

    // Execute the python function
    kvs::python::List list = function.call( args );
    const kvs::ValueArray<T> coef = kvs::ValueArray<T>( kvs::python::Array( list[0] ) );
    const float r2 = kvs::python::Float( list[1] );
    const float rss = kvs::python::Float( list[2] );
    const kvs::ValueArray<T> tvals = kvs::ValueArray<T>( kvs::python::Array( list[3] ) );

    m_r2 = r2;
    m_rss = rss;
    m_coef = kvs::Vector<T>( coef.size(), coef.data() );
    m_t_values = kvs::Vector<T>( tvals.size(), tvals.data() );
}

// template instantiation
template class LassoRegression<float>;

} // end of namespace sklearn

} // end of namespace local

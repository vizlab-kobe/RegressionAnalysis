#pragma once
#include <kvs/ValueArray>
#include <kvs/ValueTable>
#include <kvs/LinearRegression>
#include <RegressionAnalysis/Lib/LassoRegression.h>
#include <RegressionAnalysis/Lib/RidgeRegression.h>
#include <RegressionAnalysis/Lib/ElasticNetRegression.h>


namespace local
{

template <typename T>
class Regression
{
public:
    enum Method
    {
        Linear = 0,
        Lasso,
        Ridge,
        ElasticNet
    };

private:
    typedef kvs::LinearRegression<T> LinearRegression;
    typedef RegressionAnalysis::sklearn::LassoRegression<T> LassoRegression;
    typedef RegressionAnalysis::sklearn::RidgeRegression<T> RidgeRegression;
    typedef RegressionAnalysis::sklearn::ElasticNetRegression<T> ElasticNetRegression;

    union
    {
        LinearRegression* linear;
        LassoRegression* lasso;
        RidgeRegression* ridge;
        ElasticNetRegression* elastic_net;
    } m_regression;

    Method m_method;
    float m_complexity;
    float m_l1_ratio;
    kvs::ValueArray<T> m_dependent_variable;
    kvs::ValueTable<T> m_independent_variables;

public:
    Regression( const Method method = Linear ):
        m_complexity( 1.0 ),
        m_l1_ratio( 1.0 )
    {
        m_regression.linear = NULL;
        setMethod( method );
    }

    ~Regression()
    {
        if ( m_regression.linear ) { delete m_regression.linear; }
    }

    void setMethod( const Method method )
    {
        if ( m_regression.linear ) { delete m_regression.linear; }
        m_method = method;
        switch ( method )
        {
        case Linear: m_regression.linear = new LinearRegression; break;
        case Lasso: m_regression.lasso = new LassoRegression; break;
        case Ridge: m_regression.ridge = new RidgeRegression; break;
        case ElasticNet: m_regression.elastic_net = new ElasticNetRegression; break;
        default: break;
        }
    }

    void setMethodToLinear() { setMethod( Linear ); }
    void setMethodToLasso() { setMethod( Lasso ); }
    void setMethodToRidge() { setMethod( Ridge ); }
    void setMethodToElasticNet() { setMethod( ElasticNet ); }
    void setComplexity( const float complexity ) { m_complexity = complexity; }
    void setL1Ratio( const float l1_ratio ) { m_l1_ratio = l1_ratio; }

    Method method() const { return m_method; }
    float complexity() const { return m_complexity; }
    float l1Ratio() const { return m_l1_ratio; }

    void setDependentVariable( const kvs::ValueArray<T>& variable )
    {
        m_dependent_variable = variable;
    }

    void setIndependentVariables( const kvs::ValueTable<T>& variables )
    {
        m_independent_variables = variables;
    }

    void setIndependentVariable( const kvs::ValueArray<T>& variable, const size_t index )
    {
        if ( index < m_independent_variables.columnSize() )
        {
            m_independent_variables[index] = variable;
        }
        else
        {
            this->addIndependentVariable( variable );
        }
    }

    void addIndependentVariable( const kvs::ValueArray<T>& variable )
    {
        m_independent_variables.pushBackColumn( variable );
    }

    const kvs::ValueArray<T>& dependentVariable() const { return m_dependent_variable; }
    const kvs::ValueTable<T>& independentVariables() const { return m_independent_variables; }

    float r2() const
    {
        switch ( m_method )
        {
        case Linear: return m_regression.linear->r2();
        case Lasso: return m_regression.lasso->r2();
        case Ridge: return m_regression.ridge->r2();
        case ElasticNet: return m_regression.elastic_net->r2();
        }
    }

    const kvs::Vector<T>& coef() const
    {
        switch ( m_method )
        {
        case Linear: return m_regression.linear->coef();
        case Lasso: return m_regression.lasso->coef();
        case Ridge: return m_regression.ridge->coef();
        case ElasticNet: return m_regression.elastic_net->coef();
        }
    }

    const kvs::Vector<T>& tValues() const
    {
        switch ( m_method )
        {
        case Linear: return m_regression.linear->tValues();
        case Lasso: return m_regression.lasso->tValues();
        case Ridge: return m_regression.ridge->tValues();
        case ElasticNet: return m_regression.elastic_net->tValues();
        }
    }

    const kvs::Vector<T>& pValues() const
    {
        switch ( m_method )
        {
        case Linear: return m_regression.linear->pValues();
        case Lasso: return m_regression.lasso->pValues();
        case Ridge: return m_regression.ridge->pValues();
        case ElasticNet: return m_regression.elastic_net->pValues();
        }
    }

//    void fit( const kvs::ValueArray<float>& dep, const kvs::ValueTable<float>& indep )
    void fit()
    {
        switch ( m_method )
        {
        case Linear:
            m_regression.linear->fit( m_dependent_variable, m_independent_variables );
            m_regression.linear->test();
            break;
        case Lasso:
            m_regression.lasso->setComplexity( m_complexity );
            m_regression.lasso->fit( m_dependent_variable, m_independent_variables );
            m_regression.lasso->test();
            break;
        case Ridge:
            m_regression.ridge->setComplexity( m_complexity );
            m_regression.ridge->fit( m_dependent_variable, m_independent_variables );
            m_regression.ridge->test();
            break;
        case ElasticNet:
            m_regression.elastic_net->setComplexity( m_complexity );
            m_regression.elastic_net->setL1Ratio( m_l1_ratio );
            m_regression.elastic_net->fit( m_dependent_variable, m_independent_variables );
            m_regression.elastic_net->test();
            break;
        }
    }
};

} // end of namespace local

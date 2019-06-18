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

    Method m_method;
    float m_complexity;
    float m_l1_ratio;
    kvs::ValueArray<T> m_dependent_variable;
    kvs::ValueTable<T> m_independent_variables;
    float m_r2;
    kvs::Vector<T> m_coef;
    kvs::Vector<T> m_tvalues;
    kvs::Vector<T> m_pvalues;

public:
    Regression( const Method method = Linear ):
        m_complexity( 1.0 ),
        m_l1_ratio( 0.5 )
    {
        this->setMethod( method );
    }

    ~Regression()
    {
    }

    void setMethod( const Method method ) { m_method = method; }
    void setMethodToLinear() { setMethod( Linear ); }
    void setMethodToLasso() { setMethod( Lasso ); }
    void setMethodToRidge() { setMethod( Ridge ); }
    void setMethodToElasticNet() { setMethod( ElasticNet ); }
    void setComplexity( const float complexity ) { m_complexity = complexity; }
    void setL1Ratio( const float l1_ratio ) { m_l1_ratio = l1_ratio; }

    Method method() const { return m_method; }
    float complexity() const { return m_complexity; }
    float l1Ratio() const { return m_l1_ratio; }

    const kvs::ValueArray<T>& dependentVariable() const { return m_dependent_variable; }
    const kvs::ValueTable<T>& independentVariables() const { return m_independent_variables; }

    void setDependentVariable( const kvs::ValueArray<T>& variable ) { m_dependent_variable = variable; }
    void setIndependentVariables( const kvs::ValueTable<T>& variables ) { m_independent_variables = variables; }

    float r2() const { return m_r2; }
    const kvs::Vector<T>& coef() const { return m_coef; }
    const kvs::Vector<T>& tValues() const { return m_tvalues; }
    const kvs::Vector<T>& pValues() const { return m_pvalues; }

    void fit()
    {
        switch ( m_method )
        {
        case Linear:
        {
            LinearRegression regression;
            regression.fit( m_dependent_variable, m_independent_variables );
            regression.test();
            m_r2 = regression.r2();
            m_coef = regression.coef();
            m_tvalues = regression.tValues();
            m_pvalues = regression.pValues();
            break;
        }
        case Lasso:
        {
            LassoRegression regression;
            regression.setEnabledNormalize( false );
            regression.setComplexity( m_complexity );
            regression.fit( m_dependent_variable, m_independent_variables );
            regression.test();
            m_r2 = regression.r2();
            m_coef = regression.coef();
            m_tvalues = regression.tValues();
            m_pvalues = regression.pValues();
            break;
        }
        case Ridge:
        {
            RidgeRegression regression;
            regression.setEnabledNormalize( false );
            regression.setComplexity( m_complexity );
            regression.fit( m_dependent_variable, m_independent_variables );
            regression.test();
            m_r2 = regression.r2();
            m_coef = regression.coef();
            m_tvalues = regression.tValues();
            m_pvalues = regression.pValues();
            break;
        }
        case ElasticNet:
        {
            ElasticNetRegression regression;
            regression.setEnabledNormalize( false );
            regression.setComplexity( m_complexity );
            regression.setL1Ratio( m_l1_ratio );
            regression.fit( m_dependent_variable, m_independent_variables );
            regression.test();
            m_r2 = regression.r2();
            m_coef = regression.coef();
            m_tvalues = regression.tValues();
            m_pvalues = regression.pValues();
            break;
        }
        }
    }
};

} // end of namespace local

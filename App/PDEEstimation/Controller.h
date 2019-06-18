#pragma once
#include "UI.h"
#include "View.h"


namespace local
{

class Controller
{
private:
    local::View* m_view;
    local::UI::LinearRadioButton m_linear_radio_button;
    local::UI::LassoRadioButton m_lasso_radio_button;
    local::UI::RidgeRadioButton m_ridge_radio_button;
    local::UI::ElasticNetRadioButton m_elastic_radio_button;
    kvs::RadioButtonGroup m_radio_button_group;
    local::UI::ComplexitySlider m_complexity_slider;
    local::UI::L1RatioSlider m_l1_ratio_slider;
    local::UI::BoxCenterSlider m_box_center_slider;
    local::UI::BoxLengthSlider m_box_length_slider;
    local::UI::ApplyButton m_apply_button;

public:
    Controller( local::Model* model, local::View* view ):
        m_view( view ),
        m_linear_radio_button( model, view ),
        m_lasso_radio_button( model, view ),
        m_ridge_radio_button( model, view ),
        m_elastic_radio_button( model, view ),
        m_complexity_slider( model, view ),
        m_l1_ratio_slider( model, view ),
        m_box_center_slider( model, view ),
        m_box_length_slider( model, view ),
        m_apply_button( model, view )
    {
        m_linear_radio_button.setState( true );
        this->layout_widgets();
    }

private:
    void layout_widgets()
    {
        const size_t width = 150;
        const size_t margin = 10;

        // Top-left
        {
            const size_t x = 0;
            const size_t y = 0;
            m_linear_radio_button.setMargin( margin );
            m_linear_radio_button.setPosition( x, y );
            m_linear_radio_button.show();
        }

        {
            const kvs::WidgetBase* parent = &m_linear_radio_button;
            const size_t x = parent->x();
            const size_t y = parent->y() + parent->height();
            m_lasso_radio_button.setMargin( margin );
            m_lasso_radio_button.setPosition( x, y );
            m_lasso_radio_button.show();
        }

        {
            const kvs::WidgetBase* parent = &m_lasso_radio_button;
            const size_t x = parent->x();
            const size_t y = parent->y() + parent->height();
            m_ridge_radio_button.setMargin( margin );
            m_ridge_radio_button.setPosition( x, y );
            m_ridge_radio_button.show();
        }

        {
            const kvs::WidgetBase* parent = &m_ridge_radio_button;
            const size_t x = parent->x();
            const size_t y = parent->y() + parent->height();
            m_elastic_radio_button.setMargin( margin );
            m_elastic_radio_button.setPosition( x, y );
            m_elastic_radio_button.show();
        }

        m_radio_button_group.add( &m_linear_radio_button );
        m_radio_button_group.add( &m_lasso_radio_button );
        m_radio_button_group.add( &m_ridge_radio_button );
        m_radio_button_group.add( &m_elastic_radio_button );

        {
            const kvs::WidgetBase& parent = m_elastic_radio_button;
            const size_t x = parent.x();
            const size_t y = parent.y() + parent.height();
            m_complexity_slider.setMargin( margin );
            m_complexity_slider.setPosition( x, y );
            m_complexity_slider.show();
        }

        {
            const kvs::WidgetBase& parent = m_complexity_slider;
            const size_t x = parent.x();
            const size_t y = parent.y() + parent.height();
            m_l1_ratio_slider.setMargin( 10 );
            m_l1_ratio_slider.setPosition( x, y );
            m_l1_ratio_slider.show();
        }

        // Top-right
        {
            const size_t x = m_view->screen().width() - ( width + margin );
            const size_t y = 0;
            m_box_center_slider.setMargin( margin );
            m_box_center_slider.setPosition( x, y );
            m_box_center_slider.show();
        }

        {
            const kvs::WidgetBase& parent = m_box_center_slider;
            const size_t x = parent.x();
            const size_t y = parent.y() + parent.height();
            m_box_length_slider.setMargin( margin );
            m_box_length_slider.setPosition( x, y );
            m_box_length_slider.show();
        }

        {
            const kvs::WidgetBase& parent = m_box_length_slider;
            const size_t x = parent.x();
            const size_t y = parent.y() + parent.height() + 10;
            m_apply_button.setMargin( margin );
            m_apply_button.setPosition( x, y );
            m_apply_button.setWidth( width );
            m_apply_button.show();
        }
    }
};

} // end of namespace local

#pragma once

#include <QDialog>
#include <QLabel>
#include "qcolorpicker_slider.h"

class QColorPicker : public QDialog
{
public:
    /**
     * @brief Construct a new QColorPicker object
     *
     * @param parent the parent widget
     */
    explicit QColorPicker(QWidget* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~QColorPicker();

    /**
     * @brief Get the color
     *
     * @return the color
     */
    QColor color() const;

    void updateColor();
    void setColor(QColor c);

private:
    QColor _selectedColor;

    QColorPickerSlider* hueSlider;
    QColorPickerSlider* saturationSlider;
    QColorPickerSlider* brightnessSlider;
    QColorPickerSlider* alphaSlider;
    QLabel* redVal;
    QLabel* greenVal;
    QLabel* blueVal;

    QLabel* finalColorPreview;
    QLabel* hexCodeLabel;

};

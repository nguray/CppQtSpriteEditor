#include <QBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QLineEdit>
#include <QLinearGradient>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>

#include "qcolorpicker.h"


QColorPicker::QColorPicker(QWidget* parent)
    : QDialog(parent)
{

    QBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 0);

    finalColorPreview = new QLabel();
    finalColorPreview->setMinimumSize(400, 100);
    mainLayout->addWidget(finalColorPreview);

    QBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(10, 10, 10, 10);
    mainLayout->addLayout(layout);

    QBoxLayout* controls = new QHBoxLayout();
    layout->addLayout(controls);

    hexCodeLabel = new QLabel;
    controls->addWidget(hexCodeLabel);

    QGridLayout* colorSliders = new QGridLayout;

    hueSlider = new QColorPickerSlider(Qt::Horizontal);
    saturationSlider = new QColorPickerSlider(Qt::Horizontal);
    brightnessSlider = new QColorPickerSlider(Qt::Horizontal);
    alphaSlider = new QColorPickerSlider(Qt::Horizontal);

    QLabel* hueLab = new QLabel("Hue:");
    QLabel* hueVal = new QLabel;

    QLabel* saturationLab = new QLabel("Saturation:");
    QLabel* saturationVal = new QLabel;
    QLabel* brightnessLab = new QLabel("Brightness:");
    QLabel* brightnessVal = new QLabel;
    QLabel* alphaLab = new QLabel("Alpĥa:");
    QLabel* alphaVal = new QLabel;

    QLabel* redLab = new QLabel("Red:");
    QLabel* greenLab = new QLabel("Green:");
    QLabel* blueLab = new QLabel("Blue:");
    redVal = new QLabel;
    greenVal = new QLabel;
    blueVal = new QLabel;


    // auto updateColor = [ = ]() {
    //     QColor color;
    //     color.setHsv(
    //         hueSlider->value(),
    //         saturationSlider->value(),
    //         brightnessSlider->value(),
    //         alphaSlider->value()
    //     );
    //     finalColorPreview->setStyleSheet("background-color: " + color.name());
    //     hexCodeLabel->setText(color.name());
    //     _selectedColor = color;
    // };


    connect(hueSlider, &QSlider::valueChanged, this, [ = ](int value) {
        hueVal->setText(QString::number(value));
        saturationSlider->setGradientStops({
            {  0.0 / 255.0, QColor::fromHsv(value,   0, 255)},
            {255.0 / 255.0, QColor::fromHsv(value, 255, 255)}
        });
        brightnessSlider->setGradientStops({
            {  0.0 / 255.0, QColor::fromHsv(value, 255,   0)},
            {255.0 / 255.0, QColor::fromHsv(value, 255, 255)}
        });
        alphaSlider->setGradientStops({
            {  0.0 / 255.0, QColor::fromHsv(value, 255, 255,   0)},
            {255.0 / 255.0, QColor::fromHsv(value, 255, 255, 255)}
        });
        updateColor();
    });
    connect(saturationSlider, &QSlider::valueChanged, this, [ = ](int value) {
        saturationVal->setText(QString::number(value));
        updateColor();
    });
    connect(brightnessSlider, &QSlider::valueChanged, this, [ = ](int value) {
        brightnessVal->setText(QString::number(value));
        updateColor();
    });
    connect(alphaSlider, &QSlider::valueChanged, this, [ = ](int value) {
        alphaVal->setText(QString::number(value));
        updateColor();
    });

    hueVal->setText(QString::number(hueSlider->value()));
    saturationVal->setText(QString::number(saturationSlider->value()));
    brightnessVal->setText(QString::number(brightnessSlider->value()));
    alphaVal->setText(QString::number(alphaSlider->value()));

    hueVal->setMinimumSize(hueVal->fontMetrics().boundingRect("000").size()
    );
    saturationVal->setMinimumSize(
        saturationVal->fontMetrics().boundingRect("000").size()
    );
    brightnessVal->setMinimumSize(
        brightnessVal->fontMetrics().boundingRect("000").size()
    );
    alphaVal->setMinimumSize(
        alphaVal->fontMetrics().boundingRect("000").size()
    );


    hueVal->setAlignment(Qt::AlignRight);
    saturationVal->setAlignment(Qt::AlignRight);
    brightnessVal->setAlignment(Qt::AlignRight);
    alphaVal->setAlignment(Qt::AlignRight);


    redVal->setMinimumSize(
        redVal->fontMetrics().boundingRect("0000").size()
                           );
    greenVal->setMinimumSize(
        greenVal->fontMetrics().boundingRect("0000").size()
        );
    blueVal->setMinimumSize(
        blueVal->fontMetrics().boundingRect("0000").size()
        );

    redVal->setAlignment(Qt::AlignRight);
    greenVal->setAlignment(Qt::AlignRight);
    blueVal->setAlignment(Qt::AlignRight);


    hueSlider->setRange(0, 359);
    hueSlider->setGradientStops({
        {  0.0 / 360.0,     Qt::red},
        { 60.0 / 360.0,  Qt::yellow},
        {120.0 / 360.0,   Qt::green},
        {180.0 / 360.0,    Qt::cyan},
        {240.0 / 360.0,    Qt::blue},
        {300.0 / 360.0, Qt::magenta},
        {359.0 / 360.0,     Qt::red}
    });
    saturationSlider->setRange(0, 255);
    brightnessSlider->setRange(0, 255);
    alphaSlider->setRange(0, 255);

    saturationSlider->setGradientStops({
        {  0.0 / 255.0, QColor::fromHsv(hueSlider->value(),   0, 255)},
        {255.0 / 255.0, QColor::fromHsv(hueSlider->value(), 255, 255)}
    });
    brightnessSlider->setGradientStops({
        {  0.0 / 255.0, QColor::fromHsv(hueSlider->value(), 255,   0)},
        {255.0 / 255.0, QColor::fromHsv(hueSlider->value(), 255, 255)}
    });
    alphaSlider->setRenderCheckerboard(true);
    alphaSlider->setGradientStops({
        {  0.0 / 255.0, QColor::fromHsv(hueSlider->value(), 255, 255,   0)},
        {255.0 / 255.0, QColor::fromHsv(hueSlider->value(), 255, 255, 255)}
    }
    );

    hueSlider->setValue(180);
    saturationSlider->setValue(255);
    brightnessSlider->setValue(255);
    alphaSlider->setValue(255);

    colorSliders->setColumnMinimumWidth(3, 20);

    colorSliders->addWidget(hueLab, 0, 0);
    colorSliders->addWidget(hueSlider, 0, 1);
    colorSliders->addWidget(hueVal, 0, 2);
    colorSliders->addWidget(redLab, 0, 4);
    colorSliders->addWidget(redVal, 0, 5);


    colorSliders->addWidget(brightnessLab, 2, 0);
    colorSliders->addWidget(brightnessSlider, 2, 1);
    colorSliders->addWidget(brightnessVal, 2, 2);
    colorSliders->addWidget(blueLab, 2, 4);
    colorSliders->addWidget(blueVal, 2, 5);

    colorSliders->addWidget(saturationLab, 1, 0);
    colorSliders->addWidget(saturationSlider, 1, 1);
    colorSliders->addWidget(saturationVal, 1, 2);
    colorSliders->addWidget(greenLab, 1, 4);
    colorSliders->addWidget(greenVal, 1, 5);



    colorSliders->addWidget(alphaLab, 3, 0);
    colorSliders->addWidget(alphaSlider, 3, 1);
    colorSliders->addWidget(alphaVal, 3, 2);

    layout->addLayout(colorSliders);

    QPushButton* selectButton = new QPushButton("Select");
    QPushButton* cancelButton = new QPushButton("Cancel");

    connect(selectButton, &QPushButton::clicked, this, &QColorPicker::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QColorPicker::reject);

    QBoxLayout* buttons = new QHBoxLayout();
    buttons->addWidget(selectButton);
    buttons->addWidget(cancelButton);
    layout->addLayout(buttons);

    //updateColor();
}

QColorPicker::~QColorPicker()
{
    // if (hueSlider) delete hueSlider;
    // if (saturationSlider) delete saturationSlider;
    // if (brightnessSlider) delete brightnessSlider;
    // if (alphaSlider) delete alphaSlider;

    // if (finalColorPreview) delete finalColorPreview;
    // if (hexCodeLabel) delete hexCodeLabel;

}

QColor QColorPicker::color() const { return _selectedColor; }


void QColorPicker::updateColor() {
    QColor color;
    color.setHsv(
        hueSlider->value(),
        saturationSlider->value(),
        brightnessSlider->value(),
        alphaSlider->value()
        );

    int r,g,b;
    color.getRgb(&r,&g,&b);
    redVal->setText(QString::number(r));
    greenVal->setText(QString::number(g));
    blueVal->setText(QString::number(b));

    finalColorPreview->setStyleSheet("background-color: " + color.name());
    hexCodeLabel->setText(color.name());
    _selectedColor = color;

    update();

}


void QColorPicker::setColor(QColor c)
{
    _selectedColor = c;
    int h,s,b,a;
    c.getHsv(&h,&s,&b,&a);

    hueSlider->setValue(h);
    saturationSlider->setValue(s);
    brightnessSlider->setValue(b);
    alphaSlider->setValue(a);

    updateColor();

}



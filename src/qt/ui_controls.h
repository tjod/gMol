/********************************************************************************
** Form generated from reading UI file 'controls.ui'
**
** Created: Wed Jul 9 11:45:42 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLS_H
#define UI_CONTROLS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QWidget>
#include "gdial.h"

QT_BEGIN_NAMESPACE

class Ui_Controls
{
public:
    QWidget *layoutWidget_1;
    QGridLayout *gridLayout_1;
    QDoubleSpinBox *doubleSpinBox_1;
    QDoubleSpinBox *doubleSpinBox_5;
    QLabel *label_v1;
    QLabel *label_v5;
    QDoubleSpinBox *doubleSpinBox_2;
    QLabel *label_v2;
    QDoubleSpinBox *doubleSpinBox_3;
    QLabel *label_v3;
    QDoubleSpinBox *doubleSpinBox_4;
    QLabel *label_v4;
    QDoubleSpinBox *doubleSpinBox_6;
    QLabel *label_v6;
    QDoubleSpinBox *doubleSpinBox_7;
    QLabel *label_v7;
    QDoubleSpinBox *doubleSpinBox_8;
    QLabel *label_v8;
    QWidget *layoutWidget_2;
    QGridLayout *gridLayout_2;
    QLabel *label_d5;
    QLabel *label_d8;
    gDial *dial_4;
    QLabel *label_d4;
    gDial *dial_2;
    gDial *dial_7;
    QLabel *label_d6;
    gDial *dial_1;
    gDial *dial_8;
    gDial *dial_6;
    QLabel *label_d2;
    gDial *dial_5;
    QLabel *label_d3;
    QLabel *label_d1;
    QLabel *label_d7;
    gDial *dial_3;
    QWidget *layoutWidget_3;
    QGridLayout *gridLayout_3;
    QLabel *label_s1;
    QLabel *label_s2;
    QLabel *label_s3;
    QLabel *label_s4;
    QSlider *verticalSlider_1;
    QSlider *verticalSlider_2;
    QSlider *verticalSlider_3;
    QSlider *verticalSlider_4;
    QWidget *layoutWidget_4;
    QGridLayout *gridLayout_4;
    QLabel *label_s5;
    QSlider *horizontalSlider_5;
    QLabel *label_s6;
    QSlider *horizontalSlider_6;
    QLabel *label_s7;
    QSlider *horizontalSlider_7;
    QLabel *label_s8;
    QSlider *horizontalSlider_8;

    void setupUi(QWidget *Controls)
    {
        if (Controls->objectName().isEmpty())
            Controls->setObjectName(QString::fromUtf8("Controls"));
        Controls->resize(447, 687);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(220, 226, 204, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        Controls->setPalette(palette);
        Controls->setAutoFillBackground(true);
        layoutWidget_1 = new QWidget(Controls);
        layoutWidget_1->setObjectName(QString::fromUtf8("layoutWidget_1"));
        layoutWidget_1->setGeometry(QRect(330, 260, 111, 421));
        gridLayout_1 = new QGridLayout(layoutWidget_1);
        gridLayout_1->setObjectName(QString::fromUtf8("gridLayout_1"));
        gridLayout_1->setContentsMargins(0, 0, 0, 0);
        doubleSpinBox_1 = new QDoubleSpinBox(layoutWidget_1);
        doubleSpinBox_1->setObjectName(QString::fromUtf8("doubleSpinBox_1"));
        doubleSpinBox_1->setEnabled(false);
        doubleSpinBox_1->setDecimals(3);
        doubleSpinBox_1->setMinimum(-1);
        doubleSpinBox_1->setMaximum(1);
        doubleSpinBox_1->setSingleStep(0.01);

        gridLayout_1->addWidget(doubleSpinBox_1, 0, 1, 1, 1);

        doubleSpinBox_5 = new QDoubleSpinBox(layoutWidget_1);
        doubleSpinBox_5->setObjectName(QString::fromUtf8("doubleSpinBox_5"));
        doubleSpinBox_5->setEnabled(false);
        doubleSpinBox_5->setDecimals(3);
        doubleSpinBox_5->setMinimum(-1);
        doubleSpinBox_5->setMaximum(1);
        doubleSpinBox_5->setSingleStep(0.01);

        gridLayout_1->addWidget(doubleSpinBox_5, 4, 1, 1, 1);

        label_v1 = new QLabel(layoutWidget_1);
        label_v1->setObjectName(QString::fromUtf8("label_v1"));
        label_v1->setEnabled(false);
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush2(QColor(170, 255, 127, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_v1->setPalette(palette1);
        label_v1->setAlignment(Qt::AlignCenter);

        gridLayout_1->addWidget(label_v1, 0, 0, 1, 1);

        label_v5 = new QLabel(layoutWidget_1);
        label_v5->setObjectName(QString::fromUtf8("label_v5"));
        label_v5->setEnabled(false);
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::Base, brush);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_v5->setPalette(palette2);
        label_v5->setAlignment(Qt::AlignCenter);

        gridLayout_1->addWidget(label_v5, 4, 0, 1, 1);

        doubleSpinBox_2 = new QDoubleSpinBox(layoutWidget_1);
        doubleSpinBox_2->setObjectName(QString::fromUtf8("doubleSpinBox_2"));
        doubleSpinBox_2->setEnabled(false);
        doubleSpinBox_2->setDecimals(3);
        doubleSpinBox_2->setMinimum(-1);
        doubleSpinBox_2->setMaximum(1);
        doubleSpinBox_2->setSingleStep(0.01);

        gridLayout_1->addWidget(doubleSpinBox_2, 1, 1, 1, 1);

        label_v2 = new QLabel(layoutWidget_1);
        label_v2->setObjectName(QString::fromUtf8("label_v2"));
        label_v2->setEnabled(false);
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::Base, brush);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_v2->setPalette(palette3);
        label_v2->setAlignment(Qt::AlignCenter);

        gridLayout_1->addWidget(label_v2, 1, 0, 1, 1);

        doubleSpinBox_3 = new QDoubleSpinBox(layoutWidget_1);
        doubleSpinBox_3->setObjectName(QString::fromUtf8("doubleSpinBox_3"));
        doubleSpinBox_3->setEnabled(false);
        doubleSpinBox_3->setDecimals(3);
        doubleSpinBox_3->setMinimum(-1);
        doubleSpinBox_3->setMaximum(1);
        doubleSpinBox_3->setSingleStep(0.01);

        gridLayout_1->addWidget(doubleSpinBox_3, 2, 1, 1, 1);

        label_v3 = new QLabel(layoutWidget_1);
        label_v3->setObjectName(QString::fromUtf8("label_v3"));
        label_v3->setEnabled(false);
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::Base, brush);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_v3->setPalette(palette4);
        label_v3->setAlignment(Qt::AlignCenter);

        gridLayout_1->addWidget(label_v3, 2, 0, 1, 1);

        doubleSpinBox_4 = new QDoubleSpinBox(layoutWidget_1);
        doubleSpinBox_4->setObjectName(QString::fromUtf8("doubleSpinBox_4"));
        doubleSpinBox_4->setEnabled(false);
        doubleSpinBox_4->setDecimals(3);
        doubleSpinBox_4->setMinimum(-1);
        doubleSpinBox_4->setMaximum(1);
        doubleSpinBox_4->setSingleStep(0.01);

        gridLayout_1->addWidget(doubleSpinBox_4, 3, 1, 1, 1);

        label_v4 = new QLabel(layoutWidget_1);
        label_v4->setObjectName(QString::fromUtf8("label_v4"));
        label_v4->setEnabled(false);
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::Base, brush);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_v4->setPalette(palette5);
        label_v4->setAlignment(Qt::AlignCenter);

        gridLayout_1->addWidget(label_v4, 3, 0, 1, 1);

        doubleSpinBox_6 = new QDoubleSpinBox(layoutWidget_1);
        doubleSpinBox_6->setObjectName(QString::fromUtf8("doubleSpinBox_6"));
        doubleSpinBox_6->setEnabled(false);
        doubleSpinBox_6->setDecimals(3);
        doubleSpinBox_6->setMinimum(-1);
        doubleSpinBox_6->setMaximum(1);
        doubleSpinBox_6->setSingleStep(0.01);

        gridLayout_1->addWidget(doubleSpinBox_6, 5, 1, 1, 1);

        label_v6 = new QLabel(layoutWidget_1);
        label_v6->setObjectName(QString::fromUtf8("label_v6"));
        label_v6->setEnabled(false);
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::Base, brush);
        palette6.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_v6->setPalette(palette6);
        label_v6->setAlignment(Qt::AlignCenter);

        gridLayout_1->addWidget(label_v6, 5, 0, 1, 1);

        doubleSpinBox_7 = new QDoubleSpinBox(layoutWidget_1);
        doubleSpinBox_7->setObjectName(QString::fromUtf8("doubleSpinBox_7"));
        doubleSpinBox_7->setEnabled(false);
        doubleSpinBox_7->setDecimals(3);
        doubleSpinBox_7->setMinimum(-1);
        doubleSpinBox_7->setMaximum(1);
        doubleSpinBox_7->setSingleStep(0.01);

        gridLayout_1->addWidget(doubleSpinBox_7, 6, 1, 1, 1);

        label_v7 = new QLabel(layoutWidget_1);
        label_v7->setObjectName(QString::fromUtf8("label_v7"));
        label_v7->setEnabled(false);
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::Base, brush);
        palette7.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette7.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_v7->setPalette(palette7);
        label_v7->setAlignment(Qt::AlignCenter);

        gridLayout_1->addWidget(label_v7, 6, 0, 1, 1);

        doubleSpinBox_8 = new QDoubleSpinBox(layoutWidget_1);
        doubleSpinBox_8->setObjectName(QString::fromUtf8("doubleSpinBox_8"));
        doubleSpinBox_8->setEnabled(false);
        doubleSpinBox_8->setDecimals(3);
        doubleSpinBox_8->setMinimum(-1);
        doubleSpinBox_8->setMaximum(1);
        doubleSpinBox_8->setSingleStep(0.01);

        gridLayout_1->addWidget(doubleSpinBox_8, 7, 1, 1, 1);

        label_v8 = new QLabel(layoutWidget_1);
        label_v8->setObjectName(QString::fromUtf8("label_v8"));
        label_v8->setEnabled(false);
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::Base, brush);
        palette8.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette8.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette8.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette8.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_v8->setPalette(palette8);
        label_v8->setAlignment(Qt::AlignCenter);

        gridLayout_1->addWidget(label_v8, 7, 0, 1, 1);

        layoutWidget_2 = new QWidget(Controls);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(11, 11, 431, 241));
        gridLayout_2 = new QGridLayout(layoutWidget_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_d5 = new QLabel(layoutWidget_2);
        label_d5->setObjectName(QString::fromUtf8("label_d5"));
        label_d5->setEnabled(false);
        QPalette palette9;
        palette9.setBrush(QPalette::Active, QPalette::Base, brush);
        palette9.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette9.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette9.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette9.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_d5->setPalette(palette9);
        label_d5->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_d5, 2, 0, 1, 1);

        label_d8 = new QLabel(layoutWidget_2);
        label_d8->setObjectName(QString::fromUtf8("label_d8"));
        label_d8->setEnabled(false);
        QPalette palette10;
        palette10.setBrush(QPalette::Active, QPalette::Base, brush);
        palette10.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette10.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette10.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette10.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_d8->setPalette(palette10);
        label_d8->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_d8, 2, 3, 1, 1);

        dial_4 = new gDial(layoutWidget_2);
        dial_4->setObjectName(QString::fromUtf8("dial_4"));
        dial_4->setEnabled(false);
        dial_4->setFocusPolicy(Qt::WheelFocus);
        dial_4->setMinimum(-100);
        dial_4->setMaximum(100);
        dial_4->setWrapping(true);

        gridLayout_2->addWidget(dial_4, 1, 3, 1, 1);

        label_d4 = new QLabel(layoutWidget_2);
        label_d4->setObjectName(QString::fromUtf8("label_d4"));
        label_d4->setEnabled(false);
        QPalette palette11;
        palette11.setBrush(QPalette::Active, QPalette::Base, brush);
        palette11.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette11.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette11.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette11.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_d4->setPalette(palette11);
        label_d4->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_d4, 0, 3, 1, 1);

        dial_2 = new gDial(layoutWidget_2);
        dial_2->setObjectName(QString::fromUtf8("dial_2"));
        dial_2->setEnabled(false);
        dial_2->setFocusPolicy(Qt::WheelFocus);
        dial_2->setMinimum(-100);
        dial_2->setMaximum(100);
        dial_2->setWrapping(true);

        gridLayout_2->addWidget(dial_2, 1, 1, 1, 1);

        dial_7 = new gDial(layoutWidget_2);
        dial_7->setObjectName(QString::fromUtf8("dial_7"));
        dial_7->setEnabled(false);
        dial_7->setMinimum(-100);
        dial_7->setMaximum(100);
        dial_7->setNotchTarget(19.7);
        dial_7->setNotchesVisible(true);

        gridLayout_2->addWidget(dial_7, 3, 2, 1, 1);

        label_d6 = new QLabel(layoutWidget_2);
        label_d6->setObjectName(QString::fromUtf8("label_d6"));
        label_d6->setEnabled(false);
        QPalette palette12;
        palette12.setBrush(QPalette::Active, QPalette::Base, brush);
        palette12.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette12.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette12.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette12.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_d6->setPalette(palette12);
        label_d6->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_d6, 2, 1, 1, 1);

        dial_1 = new gDial(layoutWidget_2);
        dial_1->setObjectName(QString::fromUtf8("dial_1"));
        dial_1->setEnabled(false);
        dial_1->setFocusPolicy(Qt::WheelFocus);
        dial_1->setMinimum(-100);
        dial_1->setMaximum(100);
        dial_1->setWrapping(true);

        gridLayout_2->addWidget(dial_1, 1, 0, 1, 1);

        dial_8 = new gDial(layoutWidget_2);
        dial_8->setObjectName(QString::fromUtf8("dial_8"));
        dial_8->setEnabled(false);
        dial_8->setMinimum(-100);
        dial_8->setMaximum(100);
        dial_8->setNotchTarget(19.7);
        dial_8->setNotchesVisible(true);

        gridLayout_2->addWidget(dial_8, 3, 3, 1, 1);

        dial_6 = new gDial(layoutWidget_2);
        dial_6->setObjectName(QString::fromUtf8("dial_6"));
        dial_6->setEnabled(false);
        dial_6->setMinimum(-100);
        dial_6->setMaximum(100);
        dial_6->setNotchTarget(19.7);
        dial_6->setNotchesVisible(true);

        gridLayout_2->addWidget(dial_6, 3, 1, 1, 1);

        label_d2 = new QLabel(layoutWidget_2);
        label_d2->setObjectName(QString::fromUtf8("label_d2"));
        label_d2->setEnabled(false);
        QPalette palette13;
        palette13.setBrush(QPalette::Active, QPalette::Base, brush);
        palette13.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette13.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_d2->setPalette(palette13);
        label_d2->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_d2, 0, 1, 1, 1);

        dial_5 = new gDial(layoutWidget_2);
        dial_5->setObjectName(QString::fromUtf8("dial_5"));
        dial_5->setEnabled(false);
        dial_5->setMinimum(-100);
        dial_5->setMaximum(100);
        dial_5->setNotchTarget(19.7);
        dial_5->setNotchesVisible(true);

        gridLayout_2->addWidget(dial_5, 3, 0, 1, 1);

        label_d3 = new QLabel(layoutWidget_2);
        label_d3->setObjectName(QString::fromUtf8("label_d3"));
        label_d3->setEnabled(false);
        QPalette palette14;
        palette14.setBrush(QPalette::Active, QPalette::Base, brush);
        palette14.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette14.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette14.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette14.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_d3->setPalette(palette14);
        label_d3->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_d3, 0, 2, 1, 1);

        label_d1 = new QLabel(layoutWidget_2);
        label_d1->setObjectName(QString::fromUtf8("label_d1"));
        label_d1->setEnabled(false);
        QPalette palette15;
        palette15.setBrush(QPalette::Active, QPalette::Base, brush);
        palette15.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette15.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette15.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette15.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette15.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_d1->setPalette(palette15);
        label_d1->setFrameShape(QFrame::NoFrame);
        label_d1->setFrameShadow(QFrame::Plain);
        label_d1->setAlignment(Qt::AlignCenter);
        label_d1->setIndent(0);

        gridLayout_2->addWidget(label_d1, 0, 0, 1, 1);

        label_d7 = new QLabel(layoutWidget_2);
        label_d7->setObjectName(QString::fromUtf8("label_d7"));
        label_d7->setEnabled(false);
        QPalette palette16;
        palette16.setBrush(QPalette::Active, QPalette::Base, brush);
        palette16.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette16.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette16.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette16.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_d7->setPalette(palette16);
        label_d7->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_d7, 2, 2, 1, 1);

        dial_3 = new gDial(layoutWidget_2);
        dial_3->setObjectName(QString::fromUtf8("dial_3"));
        dial_3->setEnabled(false);
        dial_3->setFocusPolicy(Qt::WheelFocus);
        dial_3->setMinimum(-100);
        dial_3->setMaximum(100);
        dial_3->setWrapping(true);

        gridLayout_2->addWidget(dial_3, 1, 2, 1, 1);

        layoutWidget_3 = new QWidget(Controls);
        layoutWidget_3->setObjectName(QString::fromUtf8("layoutWidget_3"));
        layoutWidget_3->setGeometry(QRect(10, 260, 291, 211));
        gridLayout_3 = new QGridLayout(layoutWidget_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        label_s1 = new QLabel(layoutWidget_3);
        label_s1->setObjectName(QString::fromUtf8("label_s1"));
        label_s1->setEnabled(false);
        QPalette palette17;
        palette17.setBrush(QPalette::Active, QPalette::Base, brush);
        palette17.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette17.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette17.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette17.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette17.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_s1->setPalette(palette17);
        label_s1->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_s1, 0, 0, 1, 1);

        label_s2 = new QLabel(layoutWidget_3);
        label_s2->setObjectName(QString::fromUtf8("label_s2"));
        label_s2->setEnabled(false);
        QPalette palette18;
        palette18.setBrush(QPalette::Active, QPalette::Base, brush);
        palette18.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette18.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette18.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette18.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette18.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_s2->setPalette(palette18);
        label_s2->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_s2, 0, 1, 1, 1);

        label_s3 = new QLabel(layoutWidget_3);
        label_s3->setObjectName(QString::fromUtf8("label_s3"));
        label_s3->setEnabled(false);
        QPalette palette19;
        palette19.setBrush(QPalette::Active, QPalette::Base, brush);
        palette19.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette19.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette19.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette19.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette19.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_s3->setPalette(palette19);
        label_s3->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_s3, 0, 2, 1, 1);

        label_s4 = new QLabel(layoutWidget_3);
        label_s4->setObjectName(QString::fromUtf8("label_s4"));
        label_s4->setEnabled(false);
        QPalette palette20;
        palette20.setBrush(QPalette::Active, QPalette::Base, brush);
        palette20.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette20.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette20.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette20.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette20.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_s4->setPalette(palette20);
        label_s4->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_s4, 0, 3, 1, 1);

        verticalSlider_1 = new QSlider(layoutWidget_3);
        verticalSlider_1->setObjectName(QString::fromUtf8("verticalSlider_1"));
        verticalSlider_1->setEnabled(false);
        verticalSlider_1->setFocusPolicy(Qt::WheelFocus);
        verticalSlider_1->setMinimum(-100);
        verticalSlider_1->setMaximum(100);
        verticalSlider_1->setOrientation(Qt::Vertical);
        verticalSlider_1->setTickPosition(QSlider::TicksAbove);
        verticalSlider_1->setTickInterval(20);

        gridLayout_3->addWidget(verticalSlider_1, 1, 0, 1, 1);

        verticalSlider_2 = new QSlider(layoutWidget_3);
        verticalSlider_2->setObjectName(QString::fromUtf8("verticalSlider_2"));
        verticalSlider_2->setEnabled(false);
        verticalSlider_2->setFocusPolicy(Qt::WheelFocus);
        verticalSlider_2->setMinimum(-100);
        verticalSlider_2->setMaximum(100);
        verticalSlider_2->setOrientation(Qt::Vertical);
        verticalSlider_2->setTickPosition(QSlider::TicksAbove);
        verticalSlider_2->setTickInterval(20);

        gridLayout_3->addWidget(verticalSlider_2, 1, 1, 1, 1);

        verticalSlider_3 = new QSlider(layoutWidget_3);
        verticalSlider_3->setObjectName(QString::fromUtf8("verticalSlider_3"));
        verticalSlider_3->setEnabled(false);
        verticalSlider_3->setFocusPolicy(Qt::WheelFocus);
        verticalSlider_3->setMinimum(-100);
        verticalSlider_3->setMaximum(100);
        verticalSlider_3->setOrientation(Qt::Vertical);
        verticalSlider_3->setTickPosition(QSlider::TicksAbove);
        verticalSlider_3->setTickInterval(20);

        gridLayout_3->addWidget(verticalSlider_3, 1, 2, 1, 1);

        verticalSlider_4 = new QSlider(layoutWidget_3);
        verticalSlider_4->setObjectName(QString::fromUtf8("verticalSlider_4"));
        verticalSlider_4->setEnabled(false);
        verticalSlider_4->setFocusPolicy(Qt::WheelFocus);
        verticalSlider_4->setMinimum(-100);
        verticalSlider_4->setMaximum(100);
        verticalSlider_4->setOrientation(Qt::Vertical);
        verticalSlider_4->setTickPosition(QSlider::TicksAbove);
        verticalSlider_4->setTickInterval(20);

        gridLayout_3->addWidget(verticalSlider_4, 1, 3, 1, 1);

        layoutWidget_4 = new QWidget(Controls);
        layoutWidget_4->setObjectName(QString::fromUtf8("layoutWidget_4"));
        layoutWidget_4->setGeometry(QRect(10, 470, 291, 211));
        gridLayout_4 = new QGridLayout(layoutWidget_4);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        label_s5 = new QLabel(layoutWidget_4);
        label_s5->setObjectName(QString::fromUtf8("label_s5"));
        label_s5->setEnabled(false);
        QPalette palette21;
        palette21.setBrush(QPalette::Active, QPalette::Base, brush);
        palette21.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette21.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette21.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette21.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette21.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_s5->setPalette(palette21);
        label_s5->setFrameShape(QFrame::NoFrame);
        label_s5->setFrameShadow(QFrame::Plain);
        label_s5->setLineWidth(1);
        label_s5->setScaledContents(false);
        label_s5->setAlignment(Qt::AlignCenter);
        label_s5->setMargin(3);
        label_s5->setIndent(0);

        gridLayout_4->addWidget(label_s5, 0, 0, 1, 1);

        horizontalSlider_5 = new QSlider(layoutWidget_4);
        horizontalSlider_5->setObjectName(QString::fromUtf8("horizontalSlider_5"));
        horizontalSlider_5->setEnabled(false);
        horizontalSlider_5->setFocusPolicy(Qt::WheelFocus);
        horizontalSlider_5->setMinimum(-100);
        horizontalSlider_5->setMaximum(100);
        horizontalSlider_5->setOrientation(Qt::Horizontal);
        horizontalSlider_5->setInvertedAppearance(false);
        horizontalSlider_5->setTickPosition(QSlider::TicksBelow);
        horizontalSlider_5->setTickInterval(20);

        gridLayout_4->addWidget(horizontalSlider_5, 0, 1, 1, 1);

        label_s6 = new QLabel(layoutWidget_4);
        label_s6->setObjectName(QString::fromUtf8("label_s6"));
        label_s6->setEnabled(false);
        QPalette palette22;
        palette22.setBrush(QPalette::Active, QPalette::Base, brush);
        palette22.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette22.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette22.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette22.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette22.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_s6->setPalette(palette22);
        label_s6->setFrameShape(QFrame::NoFrame);
        label_s6->setFrameShadow(QFrame::Plain);
        label_s6->setAlignment(Qt::AlignCenter);
        label_s6->setIndent(0);

        gridLayout_4->addWidget(label_s6, 1, 0, 1, 1);

        horizontalSlider_6 = new QSlider(layoutWidget_4);
        horizontalSlider_6->setObjectName(QString::fromUtf8("horizontalSlider_6"));
        horizontalSlider_6->setEnabled(false);
        horizontalSlider_6->setFocusPolicy(Qt::WheelFocus);
        horizontalSlider_6->setMinimum(-100);
        horizontalSlider_6->setMaximum(100);
        horizontalSlider_6->setOrientation(Qt::Horizontal);
        horizontalSlider_6->setTickPosition(QSlider::TicksBelow);
        horizontalSlider_6->setTickInterval(20);

        gridLayout_4->addWidget(horizontalSlider_6, 1, 1, 1, 1);

        label_s7 = new QLabel(layoutWidget_4);
        label_s7->setObjectName(QString::fromUtf8("label_s7"));
        label_s7->setEnabled(false);
        QPalette palette23;
        palette23.setBrush(QPalette::Active, QPalette::Base, brush);
        palette23.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette23.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette23.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette23.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette23.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_s7->setPalette(palette23);
        label_s7->setAlignment(Qt::AlignCenter);
        label_s7->setIndent(0);

        gridLayout_4->addWidget(label_s7, 2, 0, 1, 1);

        horizontalSlider_7 = new QSlider(layoutWidget_4);
        horizontalSlider_7->setObjectName(QString::fromUtf8("horizontalSlider_7"));
        horizontalSlider_7->setEnabled(false);
        horizontalSlider_7->setFocusPolicy(Qt::WheelFocus);
        horizontalSlider_7->setMinimum(-100);
        horizontalSlider_7->setMaximum(100);
        horizontalSlider_7->setOrientation(Qt::Horizontal);
        horizontalSlider_7->setTickPosition(QSlider::TicksBelow);
        horizontalSlider_7->setTickInterval(20);

        gridLayout_4->addWidget(horizontalSlider_7, 2, 1, 1, 1);

        label_s8 = new QLabel(layoutWidget_4);
        label_s8->setObjectName(QString::fromUtf8("label_s8"));
        label_s8->setEnabled(false);
        QPalette palette24;
        palette24.setBrush(QPalette::Active, QPalette::Base, brush);
        palette24.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette24.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette24.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette24.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette24.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        label_s8->setPalette(palette24);
        label_s8->setAlignment(Qt::AlignCenter);
        label_s8->setIndent(0);

        gridLayout_4->addWidget(label_s8, 3, 0, 1, 1);

        horizontalSlider_8 = new QSlider(layoutWidget_4);
        horizontalSlider_8->setObjectName(QString::fromUtf8("horizontalSlider_8"));
        horizontalSlider_8->setEnabled(false);
        horizontalSlider_8->setFocusPolicy(Qt::WheelFocus);
        horizontalSlider_8->setMinimum(-100);
        horizontalSlider_8->setMaximum(100);
        horizontalSlider_8->setOrientation(Qt::Horizontal);
        horizontalSlider_8->setTickPosition(QSlider::TicksBelow);
        horizontalSlider_8->setTickInterval(20);

        gridLayout_4->addWidget(horizontalSlider_8, 3, 1, 1, 1);


        retranslateUi(Controls);

        QMetaObject::connectSlotsByName(Controls);
    } // setupUi

    void retranslateUi(QWidget *Controls)
    {
        Controls->setWindowTitle(QApplication::translate("Controls", "Form", 0, QApplication::UnicodeUTF8));
        label_v1->setText(QApplication::translate("Controls", "V1", 0, QApplication::UnicodeUTF8));
        label_v5->setText(QApplication::translate("Controls", "V5", 0, QApplication::UnicodeUTF8));
        label_v2->setText(QApplication::translate("Controls", "V2", 0, QApplication::UnicodeUTF8));
        label_v3->setText(QApplication::translate("Controls", "V3", 0, QApplication::UnicodeUTF8));
        label_v4->setText(QApplication::translate("Controls", "V4", 0, QApplication::UnicodeUTF8));
        label_v6->setText(QApplication::translate("Controls", "V6", 0, QApplication::UnicodeUTF8));
        label_v7->setText(QApplication::translate("Controls", "V7", 0, QApplication::UnicodeUTF8));
        label_v8->setText(QApplication::translate("Controls", "V8", 0, QApplication::UnicodeUTF8));
        label_d5->setText(QApplication::translate("Controls", "D5", 0, QApplication::UnicodeUTF8));
        label_d8->setText(QApplication::translate("Controls", "D8", 0, QApplication::UnicodeUTF8));
        label_d4->setText(QApplication::translate("Controls", "D4", 0, QApplication::UnicodeUTF8));
        label_d6->setText(QApplication::translate("Controls", "D6", 0, QApplication::UnicodeUTF8));
        label_d2->setText(QApplication::translate("Controls", "D2", 0, QApplication::UnicodeUTF8));
        label_d3->setText(QApplication::translate("Controls", "D3", 0, QApplication::UnicodeUTF8));
        label_d1->setText(QApplication::translate("Controls", "D1", 0, QApplication::UnicodeUTF8));
        label_d7->setText(QApplication::translate("Controls", "D7", 0, QApplication::UnicodeUTF8));
        label_s1->setText(QApplication::translate("Controls", "S1", 0, QApplication::UnicodeUTF8));
        label_s2->setText(QApplication::translate("Controls", "S2", 0, QApplication::UnicodeUTF8));
        label_s3->setText(QApplication::translate("Controls", "S3", 0, QApplication::UnicodeUTF8));
        label_s4->setText(QApplication::translate("Controls", "S4", 0, QApplication::UnicodeUTF8));
        label_s5->setText(QApplication::translate("Controls", "S5", 0, QApplication::UnicodeUTF8));
        label_s6->setText(QApplication::translate("Controls", "S6", 0, QApplication::UnicodeUTF8));
        label_s7->setText(QApplication::translate("Controls", "S7", 0, QApplication::UnicodeUTF8));
        label_s8->setText(QApplication::translate("Controls", "S8", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Controls: public Ui_Controls {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLS_H

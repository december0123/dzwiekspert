#ifndef GUITARNECK_HPP
#define GUITARNECK_HPP

#include "ui_fretboard.h"

#include "SoundRecognizer.hpp"

#include <deque>
#include <string>

class Fretboard : public QWidget
{
    Q_OBJECT

public:
    explicit Fretboard(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void setStrings(std::deque<std::string> strings);
private:
    Ui::GuitarNeck ui;
    std::deque<std::string> strings_;
    constexpr static int SPACING_FOR_NAMES{30};
    void drawStrings(QPainter &painter);
    void drawFrets(QPainter& painter);
};

#endif // GUITARNECK_HPP

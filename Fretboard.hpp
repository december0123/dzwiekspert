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

public slots:
    void setIdeal(std::string fullName);

private:
    Ui::GuitarNeck ui;
    SoundRecognizer recognizer_;
    std::deque<std::string> strings_;
    constexpr static int SPACING_FOR_NAMES{30};
    void drawStrings(QPainter &painter);
    void drawFrets(QPainter& painter);
    std::string idealFullName_;
};

#endif // GUITARNECK_HPP

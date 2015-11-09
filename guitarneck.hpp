#ifndef GUITARNECK_HPP
#define GUITARNECK_HPP

#include "ui_guitarneck.h"

#include <deque>
#include <string>

class GuitarNeck : public QWidget
{
    Q_OBJECT

public:
    explicit GuitarNeck(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void setStrings(std::deque<std::string> strings);
private:
    Ui::GuitarNeck ui;
    std::deque<std::string> strings_;
    constexpr static int SPACING_FOR_NAMES{30};
};

#endif // GUITARNECK_HPP

#include "guitarneck.hpp"
#include <QPainter>
#include <QDebug>

GuitarNeck::GuitarNeck(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
}

void GuitarNeck::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 10));
    for (int string = 1; string < 12; string += 2)
    {
        painter.drawLine(SPACING_FOR_NAMES, this->height() / 11 * string, this->width(), this->height() / 11 * string);
    }
    for (int fret = 1; fret <= 24; ++fret)
    {
        auto x = (this->width() * 1.3) - SPACING_FOR_NAMES - (((this->width() * 1.3) - SPACING_FOR_NAMES) * 1/(std::pow(2.0, fret/12.0))) + SPACING_FOR_NAMES;
        painter.drawLine(x, height() / 11, x, 143);
        painter.drawText(QPoint(x - 5, 11), QString::number(fret));
    }
    for(int i = SPACING_FOR_NAMES; i <= SPACING_FOR_NAMES + 5; ++i)
    {
        painter.drawLine(i, height() / 11, i, 143);
    }
    painter.drawText(QPoint(SPACING_FOR_NAMES, 11), "0");
}

void GuitarNeck::setStrings(std::deque<std::string> strings)
{
    strings_ = strings;
    for (auto& string : this->findChildren<QLabel*>())
    {
        string->setText(strings.front().c_str());
        strings.pop_front();
    }
}

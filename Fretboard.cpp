#include "Fretboard.hpp"
#include <QPainter>
#include <QDebug>

Fretboard::Fretboard(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
}

void Fretboard::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 10));
    painter.drawText(QPoint(SPACING_FOR_NAMES, 11), "0");
    auto strings = strings_;
    for (int i = SPACING_FOR_NAMES; i <= SPACING_FOR_NAMES + 5; ++i)
    {
        painter.drawLine(i, height() / 11, i, 143);
    }
    for (int string = 1; string < 12; string += 2)
    {
        painter.drawLine(SPACING_FOR_NAMES, this->height() / 11 * string,
                         this->width(), this->height() / 11 * string);
        auto prevX = SPACING_FOR_NAMES;
        for (int fret = 1; fret <= 12; ++fret)
        {
            auto x =
                    (this->width() * 1.9) - SPACING_FOR_NAMES -
                    (((this->width() * 1.9) - SPACING_FOR_NAMES) * 1/(std::pow(2.0, fret/12.0))) +
                    SPACING_FOR_NAMES;

            painter.drawLine(x, this->height() / 11 * string, x, 143);
            painter.drawText(QPoint(x - 5, 11), QString::number(fret));
            painter.drawText(QPoint(x - (x - prevX) * 0.5,
                                    this->height() / 11 * string),
                                    recognizer_.getInInterval(strings.back(), fret).getFullName().c_str());

            prevX = x;
        }
        strings.pop_back();
    }
}

void Fretboard::setStrings(std::deque<std::string> strings)
{
    strings_ = strings;
    for (auto& string : this->findChildren<QLabel*>())
    {
        string->setText(strings.back().c_str());
        strings.pop_back();
    }
}

#include "editableeventfilter.h"
#include <QEvent>
#include <QLineEdit>
#include <QBrush>
#include <QPalette>
#include <QTextEdit>


EditableEventFilter::EditableEventFilter(QObject *parent) : QObject(parent)
{

}


bool EditableEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        auto lineedit = qobject_cast<QLineEdit*>(watched);
        if (lineedit) {
            lineedit->setStyleSheet("background-color:white;");
            lineedit->setFrame(true);
            lineedit->setReadOnly(false);
        }
    }
    else if (event->type() == QEvent::Wheel || event->type() == QEvent::MouseButtonPress) {
        event->ignore();
        return false;
    }
    return QObject::eventFilter(watched, event);
}

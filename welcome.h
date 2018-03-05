#ifndef WELCOME_H
#define WELCOME_H

#include <QMainWindow>


namespace UiWelcome {
class welcome;
}

class welcome
{
    Q_OBJECT

public:
    explicit welcome(QWidget *parent = 0);
    ~welcome();

private slots:


private:
    UiWelcome::welcome *uiW;
};

#endif // welcome_H

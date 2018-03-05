#include "welcome.h"
#include "ui_welcome.h"


welcome::welcome(QWidget *parent) :
    QMainWindow(parent),
    uiW(new UiWelcome::welcome)
{
    uiW->setupUi(this);
}

welcome::~welcome()
{
    delete uiW;
}

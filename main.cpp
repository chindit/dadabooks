#include "dadabooks.h"
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QSplashScreen screen(QPixmap(":/programme/images/chargement.png"));
    screen.show();
    app.processEvents();

    DadaBooks instance;
    instance.show();

    screen.finish(&instance);

    // Initiate new session if not set
    instance.init();

    return app.exec();
}

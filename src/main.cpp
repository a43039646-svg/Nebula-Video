// SPDX-License-Identifier: GPL-3.0-or-later
#include <QApplication>
#include <QIcon>
#include <QMainWindow>
#include <QLocale>
#include <QSettings>
#include <memory>
#include "nebula_theme.h"
#include "nebula_translator.h"
#include "nebula_video_window.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("Nebula Video"));
    QApplication::setOrganizationName(QStringLiteral("Nebula Project"));
    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("org.nebula.Video")));

    QSettings preferences(QStringLiteral("NebulaProject"), QStringLiteral("NebulaVideo"));
    const QLocale::Language systemLanguage = QLocale().language();
    QString defaultLanguage = QStringLiteral("en");
    if (systemLanguage == QLocale::Russian) defaultLanguage = QStringLiteral("ru");
    else if (systemLanguage == QLocale::Spanish) defaultLanguage = QStringLiteral("es");
    else if (systemLanguage == QLocale::French) defaultLanguage = QStringLiteral("fr");
    else if (systemLanguage == QLocale::German) defaultLanguage = QStringLiteral("de");
    else if (systemLanguage == QLocale::Portuguese) defaultLanguage = QStringLiteral("pt_BR");
    else if (systemLanguage == QLocale::Chinese) defaultLanguage = QStringLiteral("zh_CN");
    else if (systemLanguage == QLocale::Japanese) defaultLanguage = QStringLiteral("ja");
    else if (systemLanguage == QLocale::Ukrainian) defaultLanguage = QStringLiteral("uk");
    else if (systemLanguage == QLocale::Belarusian) defaultLanguage = QStringLiteral("be");
    else if (systemLanguage == QLocale::Latvian) defaultLanguage = QStringLiteral("lv");
    else if (systemLanguage == QLocale::Italian) defaultLanguage = QStringLiteral("it");
    else if (systemLanguage == QLocale::Polish) defaultLanguage = QStringLiteral("pl");
    else if (systemLanguage == QLocale::Turkish) defaultLanguage = QStringLiteral("tr");
    else if (systemLanguage == QLocale::Dutch) defaultLanguage = QStringLiteral("nl");
    else if (systemLanguage == QLocale::Korean) defaultLanguage = QStringLiteral("ko");
    else if (systemLanguage == QLocale::Arabic) defaultLanguage = QStringLiteral("ar");
    else if (systemLanguage == QLocale::Hindi) defaultLanguage = QStringLiteral("hi");
    else if (systemLanguage == QLocale::Indonesian) defaultLanguage = QStringLiteral("id");
    else if (systemLanguage == QLocale::Vietnamese) defaultLanguage = QStringLiteral("vi");
    else if (systemLanguage == QLocale::Swedish) defaultLanguage = QStringLiteral("sv");
    else if (systemLanguage == QLocale::Czech) defaultLanguage = QStringLiteral("cs");
    else if (systemLanguage == QLocale::Romanian) defaultLanguage = QStringLiteral("ro");
    else if (systemLanguage == QLocale::Danish) defaultLanguage = QStringLiteral("da");
    else if (systemLanguage == QLocale::Finnish) defaultLanguage = QStringLiteral("fi");
    else if (systemLanguage == QLocale::NorwegianBokmal) defaultLanguage = QStringLiteral("no");
    else if (systemLanguage == QLocale::Greek) defaultLanguage = QStringLiteral("el");
    else if (systemLanguage == QLocale::Bulgarian) defaultLanguage = QStringLiteral("bg");
    else if (systemLanguage == QLocale::Serbian) defaultLanguage = QStringLiteral("sr");
    else if (systemLanguage == QLocale::Hungarian) defaultLanguage = QStringLiteral("hu");
    if (!preferences.contains(QStringLiteral("app/language")))
        preferences.setValue(QStringLiteral("app/language"), defaultLanguage);
    NebulaTranslator translator;
    const QString selectedLanguage = preferences.value(QStringLiteral("app/language")).toString();
    translator.setLanguage(selectedLanguage);
    app.installTranslator(&translator);
    app.setLayoutDirection(selectedLanguage == QStringLiteral("ar") ? Qt::RightToLeft : Qt::LeftToRight);

    applyApplicationTheme(app, preferences.value(QStringLiteral("app/theme"), QStringLiteral("auto")).toString());

    std::unique_ptr<QMainWindow> window(createNebulaVideoWindow());
    window->show();
    return app.exec();
}

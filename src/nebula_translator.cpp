// SPDX-License-Identifier: GPL-3.0-or-later
#include "nebula_translator.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHash>
#include <utility>

void NebulaTranslator::setLanguage(QString language) { language_ = std::move(language); }

QString NebulaTranslator::translate(const char *, const char *sourceText, const char *, int) const {
    if (!sourceText || language_ == QStringLiteral("en")) return {};
    QString locale = language_;
    if (locale == QStringLiteral("ru")) locale = QStringLiteral("ru-RU");
    else if (locale == QStringLiteral("es")) locale = QStringLiteral("es-ES");
    else if (locale == QStringLiteral("fr")) locale = QStringLiteral("fr-FR");
    else if (locale == QStringLiteral("de")) locale = QStringLiteral("de-DE");
    else if (locale == QStringLiteral("pt_BR")) locale = QStringLiteral("pt-BR");
    else if (locale == QStringLiteral("zh_CN")) locale = QStringLiteral("zh-CN");
    else if (locale == QStringLiteral("ja")) locale = QStringLiteral("ja-JP");
    else if (locale == QStringLiteral("uk")) locale = QStringLiteral("uk-UA");
    else if (locale == QStringLiteral("be")) locale = QStringLiteral("be-BY");
    else if (locale == QStringLiteral("lv")) locale = QStringLiteral("lv-LV");
    else if (locale == QStringLiteral("it")) locale = QStringLiteral("it-IT");
    else if (locale == QStringLiteral("pl")) locale = QStringLiteral("pl-PL");
    else if (locale == QStringLiteral("tr")) locale = QStringLiteral("tr-TR");
    else if (locale == QStringLiteral("nl")) locale = QStringLiteral("nl-NL");
    else if (locale == QStringLiteral("ko")) locale = QStringLiteral("ko-KR");
    else if (locale == QStringLiteral("ar")) locale = QStringLiteral("ar-SA");
    else if (locale == QStringLiteral("hi")) locale = QStringLiteral("hi-IN");
    else if (locale == QStringLiteral("id")) locale = QStringLiteral("id-ID");
    else if (locale == QStringLiteral("vi")) locale = QStringLiteral("vi-VN");
    else if (locale == QStringLiteral("sv")) locale = QStringLiteral("sv-SE");
    else if (locale == QStringLiteral("cs")) locale = QStringLiteral("cs-CZ");
    else if (locale == QStringLiteral("ro")) locale = QStringLiteral("ro-RO");
    else if (locale == QStringLiteral("da")) locale = QStringLiteral("da-DK");
    else if (locale == QStringLiteral("fi")) locale = QStringLiteral("fi-FI");
    else if (locale == QStringLiteral("no")) locale = QStringLiteral("nb-NO");
    else if (locale == QStringLiteral("el")) locale = QStringLiteral("el-GR");
    else if (locale == QStringLiteral("bg")) locale = QStringLiteral("bg-BG");
    else if (locale == QStringLiteral("sr")) locale = QStringLiteral("sr-RS");
    else if (locale == QStringLiteral("hu")) locale = QStringLiteral("hu-HU");
    static QHash<QString, QHash<QString, QString>> catalogs;
    if (!catalogs.contains(locale)) {
        QFile file(QStringLiteral(":/locales/") + locale + QStringLiteral(".json"));
        QHash<QString, QString> entries;
        if (file.open(QIODevice::ReadOnly)) {
            const auto object = QJsonDocument::fromJson(file.readAll()).object();
            for (auto it = object.begin(); it != object.end(); ++it) entries.insert(it.key(), it.value().toString());
        }
        catalogs.insert(locale, std::move(entries));
    }
    return catalogs.value(locale).value(QString::fromUtf8(sourceText));
}

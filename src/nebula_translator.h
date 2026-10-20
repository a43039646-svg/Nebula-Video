// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <QTranslator>

class NebulaTranslator final : public QTranslator {
public:
    void setLanguage(QString language);
    QString translate(const char *context, const char *sourceText, const char *disambiguation, int n) const override;
private:
    QString language_ = QStringLiteral("en");
};

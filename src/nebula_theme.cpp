// SPDX-License-Identifier: GPL-3.0-or-later
#include <QApplication>
#include <QFile>
#include <QHash>
#include <QList>
#include <QPair>
#include <QProcessEnvironment>
#include <QSysInfo>
#include <QString>
#include "nebula_theme.h"

void applyApplicationTheme(QApplication &app, QString themeId) {
    if (themeId == QStringLiteral("auto")) {
        QFile release(QStringLiteral("/etc/os-release"));
        QString ids;
        if (release.open(QIODevice::ReadOnly | QIODevice::Text)) {
            while (!release.atEnd()) {
                const QString line = QString::fromUtf8(release.readLine()).trimmed();
                if (line.startsWith(QStringLiteral("ID=")) || line.startsWith(QStringLiteral("ID_LIKE="))) {
                    QString value = line.mid(line.indexOf(QLatin1Char('=')) + 1).trimmed();
                    if (value.size() >= 2 && value.startsWith(QLatin1Char('"')) && value.endsWith(QLatin1Char('"')))
                        value = value.mid(1, value.size() - 2);
                    ids += QLatin1Char(' ') + value.toLower();
                }
            }
        }
        if (ids.contains(QStringLiteral("linuxmint")) || ids.contains(QStringLiteral(" mint"))) themeId = QStringLiteral("mint");
        else if (ids.contains(QStringLiteral("pop"))) themeId = QStringLiteral("pop");
        else if (ids.contains(QStringLiteral("ubuntu"))) themeId = QStringLiteral("ubuntu");
        else if (ids.contains(QStringLiteral("manjaro"))) themeId = QStringLiteral("mint");
        else if (ids.contains(QStringLiteral("endeavour")) || ids.contains(QStringLiteral(" arch"))) themeId = QStringLiteral("ocean");
        else if (ids.contains(QStringLiteral("fedora"))) themeId = QStringLiteral("fedora");
        else if (ids.contains(QStringLiteral("debian"))) themeId = QStringLiteral("debian");
        else if (ids.contains(QStringLiteral("opensuse")) || ids.contains(QStringLiteral(" suse"))) themeId = QStringLiteral("mint");
        else if (ids.contains(QStringLiteral("elementary")) || ids.contains(QStringLiteral("zorin")) || ids.contains(QStringLiteral("kali"))) themeId = QStringLiteral("fedora");
        else if (qEnvironmentVariable("XDG_CURRENT_DESKTOP").contains(QStringLiteral("KDE"), Qt::CaseInsensitive)) themeId = QStringLiteral("ocean");
        else themeId = QStringLiteral("nebula");
    }

    struct ThemeColors { const char *primary; const char *light; const char *hover; const char *soft; const char *bg; const char *surface; const char *sidebar; const char *line; const char *field; };
    const QHash<QString, ThemeColors> palettes = {
        {QStringLiteral("nebula"), {"#7757d8", "#a98bff", "#8e70ed", "#bca9ff", "#0b0b14", "#12121f", "#10101b", "#29293e", "#191929"}},
        {QStringLiteral("mint"), {"#68b62f", "#a8e078", "#7ac744", "#bde995", "#0d120a", "#151d10", "#11190d", "#304326", "#1c2916"}},
        {QStringLiteral("ubuntu"), {"#e95420", "#ff9a62", "#f26b36", "#ffb18d", "#130d0a", "#21140f", "#1b100c", "#43271a", "#291912"}},
        {QStringLiteral("fedora"), {"#51a2da", "#94d2f4", "#68b3e8", "#b4e2fa", "#09101a", "#101a29", "#0d1623", "#25364d", "#162338"}},
        {QStringLiteral("arch"), {"#1793d1", "#66c7f2", "#32a8e0", "#89d8f7", "#081116", "#0d1b22", "#0a171d", "#1e3945", "#10252e"}},
        {QStringLiteral("debian"), {"#d70a53", "#ff709c", "#e83a73", "#ff9bb8", "#140b10", "#211019", "#1b0e15", "#462334", "#2a1520"}},
        {QStringLiteral("manjaro"), {"#35bf5c", "#8ce49e", "#55d178", "#acf0b7", "#0c120d", "#121c14", "#0e180f", "#283c2c", "#18251a"}},
        {QStringLiteral("opensuse"), {"#73ba25", "#b0e66d", "#8aca3f", "#c4ed91", "#0c1209", "#151d10", "#10190c", "#304323", "#1b2914"}},
        {QStringLiteral("pop"), {"#48b9c7", "#91e7ed", "#60cad5", "#b1f0f2", "#091315", "#101d20", "#0d171a", "#263b3f", "#162629"}},
        {QStringLiteral("elementary"), {"#64b9ef", "#a5dcff", "#79c5f4", "#c4e9ff", "#0b1015", "#121b23", "#0e171e", "#283b49", "#182733"}},
        {QStringLiteral("zorin"), {"#0b7fab", "#69c5e8", "#2494bd", "#9cddf5", "#081116", "#0d1b22", "#0a171d", "#1e3945", "#10252e"}},
        {QStringLiteral("kali"), {"#557c94", "#9ebaca", "#6d91a6", "#bfd2dd", "#0c1014", "#151b20", "#10171c", "#2c3b45", "#1b2830"}},
        {QStringLiteral("kde"), {"#3daee9", "#8bd5ff", "#5bbef0", "#a9e2ff", "#091117", "#101b24", "#0d161e", "#263a49", "#162632"}},
        {QStringLiteral("ocean"), {"#2bb7d9", "#83e4f5", "#46c7e4", "#b4f1fb", "#081217", "#0e1a20", "#0b151b", "#203943", "#12262e"}},
        {QStringLiteral("rose"), {"#e86ba5", "#ffa7d0", "#ed81b2", "#ffc5df", "#150c12", "#20131d", "#1a1018", "#40263a", "#291a27"}},
        {QStringLiteral("amber"), {"#f0a202", "#ffd166", "#f5b528", "#ffe19a", "#151109", "#201a0c", "#1a150a", "#40351b", "#2a2311"}}
    };
    const ThemeColors colors = palettes.value(themeId, palettes.value(QStringLiteral("nebula")));

    QString sheet = QStringLiteral(R"(
        QMainWindow, QWidget { background: #0b0b14; color: #eeeeF7; font-family: Inter, 'Noto Sans', sans-serif; }
        #toolbar { background: #12121f; border-bottom: 1px solid #29293e; }
        #brand { color: #f3f0ff; font-weight: 800; letter-spacing: 2px; padding-right: 10px; }
        #sidebar { background: #10101b; border-right: 1px solid #29293e; }
        #sectionTitle { color: #aaa6c1; font-size: 11px; font-weight: 700; letter-spacing: 1.6px; }
        #hint { color: #85829a; font-size: 11px; }
        QLineEdit { background: #191929; border: 1px solid #34344e; border-radius: 8px; padding: 9px 12px; selection-background-color: #7757d8; }
        QLineEdit:focus { border-color: #a98bff; }
        QToolButton { background: transparent; border-radius: 7px; padding: 8px; }
        QToolButton:hover { background: #25243a; }
        QPushButton { background: #25243a; border: 1px solid #3a3956; border-radius: 8px; padding: 9px 14px; font-weight: 600; }
        QPushButton:hover { background: #34314f; border-color: #a98bff; }
        #goButton, #addFolderButton, #playButton { background: #7757d8; border: none; color: white; }
        #goButton:hover, #addFolderButton:hover, #playButton:hover { background: #8e70ed; }
        QListWidget { background: transparent; border: none; outline: none; }
        QListWidget::item { padding: 10px 9px; border-radius: 6px; color: #d5d2e5; }
        QListWidget::item:hover { background: #1e1d31; }
        QListWidget::item:selected { background: #292541; color: white; }
        #videoTitle { font-size: 18px; font-weight: 700; }
        #homeTitle { font-size: 30px; font-weight: 800; color: #f3f0ff; }
        #homeSubtitle { color: #aaa6c1; font-size: 14px; }
        #siteTile, #addShortcutTile { text-align: left; padding: 20px; font-size: 17px; font-weight: 700; background: #151523; border: 1px solid #302f49; border-radius: 12px; }
        #siteTile:hover, #addShortcutTile:hover { background: #211e35; border-color: #a98bff; }
        #siteTile { color: #f3f0ff; }
        #addShortcutTile { color: #bca9ff; border-style: dashed; }
        #timeLabel { color: #aaa6c1; font-size: 12px; min-width: 90px; }
        QSlider::groove:horizontal { height: 5px; background: #34344e; border-radius: 2px; }
        QSlider::sub-page:horizontal { background: #a98bff; border-radius: 2px; }
        QSlider::handle:horizontal { background: #f3f0ff; width: 13px; margin: -5px 0; border-radius: 6px; }
    )");
    const QList<QPair<QString, QString>> replacements = {
        {QStringLiteral("#7757d8"), QString::fromLatin1(colors.primary)},
        {QStringLiteral("#a98bff"), QString::fromLatin1(colors.light)},
        {QStringLiteral("#8e70ed"), QString::fromLatin1(colors.hover)},
        {QStringLiteral("#bca9ff"), QString::fromLatin1(colors.soft)},
        {QStringLiteral("#0b0b14"), QString::fromLatin1(colors.bg)},
        {QStringLiteral("#12121f"), QString::fromLatin1(colors.surface)},
        {QStringLiteral("#10101b"), QString::fromLatin1(colors.sidebar)},
        {QStringLiteral("#29293e"), QString::fromLatin1(colors.line)},
        {QStringLiteral("#191929"), QString::fromLatin1(colors.field)}
    };
    for (const auto &replacement : replacements)
        sheet.replace(replacement.first, replacement.second);
    app.setStyleSheet(sheet);
}


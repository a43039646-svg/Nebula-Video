// SPDX-License-Identifier: GPL-3.0-or-later
#include <QAudioOutput>
#include <QDir>
#include <QDirIterator>
#include <QComboBox>
#include <QDialog>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFileInfo>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QMessageBox>
#include <QPushButton>
#include <QSet>
#include <QSettings>
#include <QSignalBlocker>
#include <QPair>
#include <QSlider>
#include <QStatusBar>
#include <QStackedWidget>
#include <QStandardPaths>
#include <QStyle>
#include <QToolButton>
#include <QUrl>
#include <QVariant>
#include <QVariantMap>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWebEngineFullScreenRequest>
#include <QWebEngineNewWindowRequest>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineUrlRequestInfo>
#include <QWebEngineUrlRequestInterceptor>
#include <QTimer>
#include <QWidget>
#include <QProcess>
#include <QFormLayout>
#include <QHash>
#include <QMediaMetaData>
#include <QProcess>
#include <QTranslator>
#include <QLocale>
#include <QSysInfo>
#include <QSignalBlocker>
#include <QScreen>
#include <QResizeEvent>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVideoWidget>
#include <QMainWindow>
#include <QApplication>

#include <algorithm>
#include <limits>
#include <atomic>
#include <functional>
#include <memory>
#include <utility>

class NebulaTranslator final : public QTranslator {
public:
    void setLanguage(QString language) { language_ = std::move(language); }

    QString translate(const char *, const char *sourceText, const char *, int) const override {
        if (language_ != QStringLiteral("ru") || !sourceText)
            return {};
        return translations().value(QString::fromUtf8(sourceText));
    }

private:
    static const QHash<QString, QString> &translations() {
        static const QHash<QString, QString> map = {
            {QStringLiteral("Back"), QStringLiteral("Назад")},
            {QStringLiteral("Forward"), QStringLiteral("Вперёд")},
            {QStringLiteral("Reload"), QStringLiteral("Обновить")},
            {QStringLiteral("Home"), QStringLiteral("Домой")},
            {QStringLiteral("Library"), QStringLiteral("Библиотека")},
            {QStringLiteral("Show or hide your video library"), QStringLiteral("Показать или скрыть библиотеку видео")},
            {QStringLiteral("Enter an address or search the web"), QStringLiteral("Введите адрес сайта или поисковый запрос")},
            {QStringLiteral("Go"), QStringLiteral("Перейти")},
            {QStringLiteral("System browser"), QStringLiteral("Системный браузер")},
            {QStringLiteral("Open this page in your normal browser if playback does not work here"), QStringLiteral("Открыть страницу в обычном браузере, если здесь видео не запускается")},
            {QStringLiteral("Check video and image support on this page"), QStringLiteral("Проверить видео и изображения на странице")},
            {QStringLiteral("YOUR LIBRARY"), QStringLiteral("МОЯ БИБЛИОТЕКА")},
            {QStringLiteral("＋  Add video folder"), QStringLiteral("＋  Добавить папку с видео")},
            {QStringLiteral("Filter your videos"), QStringLiteral("Поиск видео")},
            {QStringLiteral("Videos found in folders you add"), QStringLiteral("Видео в добавленных папках")},
            {QStringLiteral("Only files in folders you choose are shown here."), QStringLiteral("Здесь показываются видео только из выбранных папок.")},
            {QStringLiteral("What do you want to watch?"), QStringLiteral("Что будем смотреть?")},
            {QStringLiteral("Open a streaming site, search the web, or play something from your library."), QStringLiteral("Откройте видеосайт, найдите видео или выберите файл из библиотеки.")},
            {QStringLiteral("Type a site address or search term in the top bar. Your shortcuts stay on this device."), QStringLiteral("Введите адрес или запрос вверху. Быстрые ссылки хранятся на этом устройстве.")},
            {QStringLiteral("Choose a video from your library"), QStringLiteral("Выберите видео из библиотеки")},
            {QStringLiteral("Play"), QStringLiteral("Воспроизвести")},
            {QStringLiteral("Pause"), QStringLiteral("Пауза")},
            {QStringLiteral("Could not open browser"), QStringLiteral("Не удалось открыть браузер")},
            {QStringLiteral("Loading page…"), QStringLiteral("Загрузка страницы…")},
            {QStringLiteral("Loading page… %1%"), QStringLiteral("Загрузка страницы… %1%")},
            {QStringLiteral("Page loaded"), QStringLiteral("Страница загружена")},
            {QStringLiteral("Page did not load completely — try Reload or System browser"), QStringLiteral("Страница загрузилась не полностью — нажмите «Обновить» или откройте системный браузер")},
            {QStringLiteral("Blocked requests: %1"), QStringLiteral("Заблокировано запросов: %1")},
            {QStringLiteral("Playback error"), QStringLiteral("Ошибка воспроизведения")},
            {QStringLiteral("Videos and channels"), QStringLiteral("Видео и каналы")},
            {QStringLiteral("Movies and series"), QStringLiteral("Фильмы и сериалы")},
            {QStringLiteral("Anime"), QStringLiteral("Аниме")},
            {QStringLiteral("＋\nAdd a site"), QStringLiteral("＋\nДобавить сайт")},
            {QStringLiteral("Add a site"), QStringLiteral("Добавить сайт")},
            {QStringLiteral("Site name:"), QStringLiteral("Название сайта:")},
            {QStringLiteral("Website address:"), QStringLiteral("Адрес сайта:")},
            {QStringLiteral("Invalid address"), QStringLiteral("Неверный адрес")},
            {QStringLiteral("Enter a valid HTTP or HTTPS website address."), QStringLiteral("Введите корректный адрес сайта HTTP или HTTPS.")},
            {QStringLiteral("Custom site"), QStringLiteral("Мой сайт")},
            {QStringLiteral("Media check"), QStringLiteral("Проверка видео")},
            {QStringLiteral("Open a website first, then check its video support."), QStringLiteral("Сначала откройте сайт, затем проверьте поддержку видео.")},
            {QStringLiteral("unavailable"), QStringLiteral("нет поддержки")},
            {QStringLiteral("available"), QStringLiteral("поддерживается")},
            {QStringLiteral("No visible HTML5 video (may be inside a frame or custom player)"), QStringLiteral("HTML5-видеоплеер не найден (возможно, он во фрейме или использует другой плеер)")},
            {QStringLiteral("details below"), QStringLiteral("подробности ниже")},
            {QStringLiteral("none reported"), QStringLiteral("ошибок нет")},
            {QStringLiteral("playback was aborted"), QStringLiteral("воспроизведение прервано")},
            {QStringLiteral("media network error"), QStringLiteral("ошибка сети при загрузке видео")},
            {QStringLiteral("media decoding failed (codec or damaged stream)"), QStringLiteral("не удалось декодировать видео (кодек или повреждённый поток)")},
            {QStringLiteral("media format or source is unsupported"), QStringLiteral("формат видео не поддерживается")},
            {QStringLiteral("Player: readyState %1, networkState %2, error %3"), QStringLiteral("Плеер: готовность %1, сеть %2, ошибка %3")},
            {QStringLiteral("This Qt WebEngine build cannot play common H.264 streams. Use System browser for this page."), QStringLiteral("Эта сборка Qt WebEngine не поддерживает H.264. Откройте страницу в системном браузере.")},
            {QStringLiteral("The player appears to be inside a frame, so its format cannot be inspected from this page."), QStringLiteral("Плеер находится во фрейме, поэтому формат не удалось определить.")},
            {QStringLiteral("Add a video folder"), QStringLiteral("Добавить папку с видео")},
            {QStringLiteral("Video not found"), QStringLiteral("Видео не найдено")},
            {QStringLiteral("The selected file is no longer available."), QStringLiteral("Выбранный файл больше недоступен.")},
            {QStringLiteral("Settings"), QStringLiteral("Настройки")},
            {QStringLiteral("English"), QStringLiteral("Английский")},
            {QStringLiteral("Russian"), QStringLiteral("Русский")},
            {QStringLiteral("Restart Nebula Video to apply the interface language."), QStringLiteral("Перезапустите Nebula Video, чтобы применить язык интерфейса.")},
            {QStringLiteral("VPN"), QStringLiteral("VPN")},
            {QStringLiteral("Audio track"), QStringLiteral("Аудиодорожка")},
            {QStringLiteral("Subtitles"), QStringLiteral("Субтитры")},
            {QStringLiteral("Off"), QStringLiteral("Выкл.")},
            {QStringLiteral("Language"), QStringLiteral("Язык интерфейса")},
            {QStringLiteral("Theme"), QStringLiteral("Тема")},
            {QStringLiteral("Automatic (match distribution)"), QStringLiteral("Автоматически (по дистрибутиву)")},
            {QStringLiteral("Nebula Purple"), QStringLiteral("Nebula — фиолетовая")},
            {QStringLiteral("Ubuntu Orange"), QStringLiteral("Ubuntu — оранжевая")},
            {QStringLiteral("Fedora Blue"), QStringLiteral("Fedora — синяя")},
            {QStringLiteral("Arch Cyan"), QStringLiteral("Arch — бирюзовая")},
            {QStringLiteral("Debian Red"), QStringLiteral("Debian — красная")},
            {QStringLiteral("Linux Mint Green"), QStringLiteral("Linux Mint — зелёная")},
            {QStringLiteral("openSUSE Green"), QStringLiteral("openSUSE — зелёная")},
            {QStringLiteral("KDE Breeze Blue"), QStringLiteral("KDE Breeze — синяя")},
            {QStringLiteral("Linux distribution"), QStringLiteral("Дистрибутив Linux")},
            {QStringLiteral("Language changes apply after restarting Nebula Video. This preference also sets the preferred language sent to websites."), QStringLiteral("Язык интерфейса изменится после перезапуска. Этот выбор также задаёт предпочитаемый язык для сайтов.")},
            {QStringLiteral("Save"), QStringLiteral("Сохранить")},
            {QStringLiteral("Close"), QStringLiteral("Закрыть")},
            {QStringLiteral("VPN connections"), QStringLiteral("VPN-подключения")},
            {QStringLiteral("Manage VPN profiles saved in NetworkManager. Nebula Video does not provide VPN servers or accounts."), QStringLiteral("Управление VPN-профилями NetworkManager. Nebula Video не предоставляет VPN-серверы и учётные записи.")},
            {QStringLiteral("Loading VPN profiles…"), QStringLiteral("Загрузка VPN-профилей…")},
            {QStringLiteral("Import .ovpn"), QStringLiteral("Импорт .ovpn")},
            {QStringLiteral("Connect"), QStringLiteral("Подключить")},
            {QStringLiteral("Disconnect"), QStringLiteral("Отключить")},
            {QStringLiteral("Refresh"), QStringLiteral("Обновить список")},
            {QStringLiteral("No NetworkManager VPN profile found. Import a provider configuration or add one in your system network settings."), QStringLiteral("VPN-профиль NetworkManager не найден. Импортируйте файл провайдера или добавьте профиль в настройках системы.")},
            {QStringLiteral("Connected: %1"), QStringLiteral("Подключено: %1")},
            {QStringLiteral("Disconnected: %1"), QStringLiteral("Отключено: %1")},
            {QStringLiteral("Working with NetworkManager…"), QStringLiteral("Связь с NetworkManager…")},
            {QStringLiteral("Could not read NetworkManager profiles."), QStringLiteral("Не удалось прочитать профили NetworkManager.")},
            {QStringLiteral("Import VPN configuration"), QStringLiteral("Импорт VPN-конфигурации")},
            {QStringLiteral("OpenVPN configuration (*.ovpn);;All files (*)"), QStringLiteral("Конфигурация OpenVPN (*.ovpn);;Все файлы (*)")},
            {QStringLiteral("VPN profile imported. Configure credentials in NetworkManager if requested."), QStringLiteral("VPN-профиль импортирован. Если потребуется, настройте учётные данные в NetworkManager.")},
            {QStringLiteral("VPN import failed"), QStringLiteral("Не удалось импортировать VPN")},
            {QStringLiteral("VPN operation failed. Check the profile and permissions in NetworkManager."), QStringLiteral("Ошибка VPN. Проверьте профиль и разрешения NetworkManager.")},
            {QStringLiteral("VPN disconnected."), QStringLiteral("VPN отключён.")},
            {QStringLiteral("VPN connected."), QStringLiteral("VPN подключён.")},
            {QStringLiteral("nmcli is unavailable. Install NetworkManager to use VPN controls."), QStringLiteral("Команда nmcli недоступна. Установите NetworkManager для управления VPN.")},
            {QStringLiteral("Adblock %1 · %2"), QStringLiteral("Блокировка %1 · %2")},
            {QStringLiteral("ON"), QStringLiteral("ВКЛ")},
            {QStringLiteral("OFF"), QStringLiteral("ВЫКЛ")},
            {QStringLiteral("The page did not expose media information. Its player may be inside a protected or cross-origin frame. Try System browser."), QStringLiteral("Страница не предоставляет данные о видео. Возможно, плеер находится в защищённом или стороннем фрейме. Попробуйте системный браузер.")},
            {QStringLiteral("Embedded browser support on this page:\n\nMP4 / H.264: %1\nWebM / VP9: %2\nVideo elements found: %3\nEmbedded frames: %4\nImages loaded: %5 of %6\nImages with load errors: %7\nHTML5 player state: %8"), QStringLiteral("Поддержка видео во встроенном браузере:\n\nMP4 / H.264: %1\nWebM / VP9: %2\nВидеоплееров найдено: %3\nВстроенных фреймов: %4\nИзображений загружено: %5 из %6\nОшибок загрузки изображений: %7\nСостояние HTML5-плеера: %8")},
            {QStringLiteral("First image URLs that failed:"), QStringLiteral("Адреса изображений с ошибкой загрузки:")},
            {QStringLiteral("Common ad and tracker domains blocked this session: %1. Click to toggle; the page reloads after a change."), QStringLiteral("За сеанс заблокировано запросов к рекламным доменам: %1. Нажмите, чтобы переключить блокировку; страница перезагрузится.")},
            {QStringLiteral("Block requests to common advertising and tracking networks; reload the page after changing this setting"), QStringLiteral("Блокировать запросы к известным рекламным и следящим сетям; после переключения страница перезагрузится")},
            {QStringLiteral("No system browser is available."), QStringLiteral("Системный браузер не найден.")}
        };
        return map;
    }

    QString language_ = QStringLiteral("en");
};

static void applyApplicationTheme(QApplication &app, QString themeId);

class NebulaAdBlocker final : public QWebEngineUrlRequestInterceptor {
public:
    explicit NebulaAdBlocker(QObject *parent = nullptr) : QWebEngineUrlRequestInterceptor(parent) {
        blockedHosts_ = {
            QStringLiteral("doubleclick.net"), QStringLiteral("googlesyndication.com"),
            QStringLiteral("googleadservices.com"), QStringLiteral("adservice.google.com"),
            QStringLiteral("googletagservices.com"), QStringLiteral("adnxs.com"),
            QStringLiteral("amazon-adsystem.com"), QStringLiteral("adform.net"),
            QStringLiteral("adroll.com"), QStringLiteral("criteo.com"),
            QStringLiteral("taboola.com"), QStringLiteral("outbrain.com"),
            QStringLiteral("popads.net"), QStringLiteral("popcash.net"),
            QStringLiteral("propellerads.com"), QStringLiteral("exoclick.com"),
            QStringLiteral("trafficjunky.net"), QStringLiteral("adsterra.com"),
            QStringLiteral("adriver.ru"), QStringLiteral("betweendigital.com"),
            QStringLiteral("onclickalgo.com"), QStringLiteral("onclickmega.com"),
            QStringLiteral("push.house"), QStringLiteral("pushground.com"),
            QStringLiteral("richads.com"), QStringLiteral("hilltopads.com"),
            QStringLiteral("juicyads.com"), QStringLiteral("trafficstars.com")
        };
    }

    void setEnabled(bool enabled) { enabled_.store(enabled, std::memory_order_relaxed); }
    bool isEnabled() const { return enabled_.load(std::memory_order_relaxed); }
    unsigned int blockedCount() const { return blockedCount_.load(std::memory_order_relaxed); }

    void interceptRequest(QWebEngineUrlRequestInfo &info) override {
        if (!isEnabled())
            return;

        const QString host = info.requestUrl().host().toLower();
        if (host.isEmpty())
            return;

        for (const QString &blockedHost : blockedHosts_) {
            if (host == blockedHost || host.endsWith(QStringLiteral(".") + blockedHost)) {
                info.block(true);
                blockedCount_.fetch_add(1, std::memory_order_relaxed);
                return;
            }
        }
    }

private:
    QSet<QString> blockedHosts_;
    std::atomic_bool enabled_{true};
    std::atomic_uint blockedCount_{0};
};

class NebulaVideoWindow final : public QMainWindow {
public:
    NebulaVideoWindow() {
        setWindowTitle(QStringLiteral("Nebula Video"));
        if (QScreen *screen = QGuiApplication::primaryScreen()) {
            const QRect available = screen->availableGeometry();
            const int minimumWidth = qMin(640, available.width());
            const int minimumHeight = qMin(420, available.height());
            setMinimumSize(minimumWidth, minimumHeight);
            resize(qMax(minimumWidth, qMin(1280, available.width() - 32)),
                   qMax(minimumHeight, qMin(800, available.height() - 48)));
            move(available.center() - rect().center());
        } else {
            resize(1000, 680);
            setMinimumSize(640, 420);
        }

        auto *root = new QWidget(this);
        auto *rootLayout = new QVBoxLayout(root);
        rootLayout->setContentsMargins(0, 0, 0, 0);
        rootLayout->setSpacing(0);

        auto *toolbar = new QWidget(root);
        toolbar_ = toolbar;
        toolbar->setObjectName(QStringLiteral("toolbar"));
        auto *toolbarLayout = new QHBoxLayout(toolbar);
        toolbarLayout->setContentsMargins(16, 12, 16, 12);
        toolbarLayout->setSpacing(8);

        auto *brand = new QLabel(QStringLiteral("NEBULA <span style='color:#a98bff'>VIDEO</span>"), toolbar);
        brand_ = brand;
        brand->setObjectName(QStringLiteral("brand"));
        brand->setTextFormat(Qt::RichText);
        toolbarLayout->addWidget(brand);

        auto *backButton = makeToolButton(style()->standardIcon(QStyle::SP_ArrowBack), tr("Back"));
        auto *forwardButton = makeToolButton(style()->standardIcon(QStyle::SP_ArrowForward), tr("Forward"));
        auto *reloadButton = makeToolButton(style()->standardIcon(QStyle::SP_BrowserReload), tr("Reload"));
        auto *homeButton = makeToolButton(style()->standardIcon(QStyle::SP_DirHomeIcon), tr("Home"));
        toolbarLayout->addWidget(backButton);
        toolbarLayout->addWidget(forwardButton);
        toolbarLayout->addWidget(reloadButton);
        toolbarLayout->addWidget(homeButton);
        libraryToggleButton_ = new QToolButton(toolbar);
        libraryToggleButton_->setText(tr("Library"));
        libraryToggleButton_->setCheckable(true);
        libraryToggleButton_->setToolTip(tr("Show or hide your video library"));
        toolbarLayout->addWidget(libraryToggleButton_);

        addressBar_ = new QLineEdit(toolbar);
        addressBar_->setObjectName(QStringLiteral("addressBar"));
        addressBar_->setPlaceholderText(tr("Enter an address or search the web"));
        addressBar_->setClearButtonEnabled(true);
        toolbarLayout->addWidget(addressBar_, 1);

        auto *goButton = new QPushButton(tr("Go"), toolbar);
        goButton_ = goButton;
        goButton->setObjectName(QStringLiteral("goButton"));
        toolbarLayout->addWidget(goButton);
        auto *externalButton = new QPushButton(tr("System browser"), toolbar);
        externalButton_ = externalButton;
        externalButton->setObjectName(QStringLiteral("externalButton"));
        externalButton->setToolTip(tr("Open this page in your normal browser if playback does not work here"));
        toolbarLayout->addWidget(externalButton);
        auto *mediaCheckButton = new QToolButton(toolbar);
        mediaCheckButton->setIcon(style()->standardIcon(QStyle::SP_MessageBoxQuestion));
        mediaCheckButton->setToolTip(tr("Check video and image support on this page"));
        toolbarLayout->addWidget(mediaCheckButton);
        auto *vpnButton = new QPushButton(tr("VPN"), toolbar);
        toolbarLayout->addWidget(vpnButton);
        auto *settingsButton = new QPushButton(tr("Settings"), toolbar);
        settingsButton_ = settingsButton;
        toolbarLayout->addWidget(settingsButton);
        adBlockButton_ = new QToolButton(toolbar);
        adBlockButton_->setCheckable(true);
        adBlockButton_->setChecked(settings_.value(QStringLiteral("browser/adBlockEnabled"), true).toBool());
        adBlockButton_->setToolTip(tr("Block requests to common advertising and tracking networks; reload the page after changing this setting"));
        toolbarLayout->addWidget(adBlockButton_);
        rootLayout->addWidget(toolbar);

        auto *body = new QWidget(root);
        auto *bodyLayout = new QHBoxLayout(body);
        bodyLayout->setContentsMargins(0, 0, 0, 0);
        bodyLayout->setSpacing(0);

        auto *sidebar = new QWidget(body);
        sidebar_ = sidebar;
        sidebar->setObjectName(QStringLiteral("sidebar"));
        sidebar->setMinimumWidth(190);
        sidebar->setMaximumWidth(300);
        auto *sidebarLayout = new QVBoxLayout(sidebar);
        sidebarLayout->setContentsMargins(14, 18, 14, 14);
        sidebarLayout->setSpacing(10);

        auto *libraryTitle = new QLabel(tr("YOUR LIBRARY"), sidebar);
        libraryTitle->setObjectName(QStringLiteral("sectionTitle"));
        sidebarLayout->addWidget(libraryTitle);

        auto *addFolderButton = new QPushButton(tr("＋  Add video folder"), sidebar);
        addFolderButton->setObjectName(QStringLiteral("addFolderButton"));
        sidebarLayout->addWidget(addFolderButton);

        libraryFilter_ = new QLineEdit(sidebar);
        libraryFilter_->setPlaceholderText(tr("Filter your videos"));
        libraryFilter_->setClearButtonEnabled(true);
        sidebarLayout->addWidget(libraryFilter_);

        videoList_ = new QListWidget(sidebar);
        videoList_->setObjectName(QStringLiteral("videoList"));
        videoList_->setAlternatingRowColors(false);
        videoList_->setToolTip(tr("Videos found in folders you add"));
        sidebarLayout->addWidget(videoList_, 1);

        auto *libraryHint = new QLabel(tr("Only files in folders you choose are shown here."), sidebar);
        libraryHint->setObjectName(QStringLiteral("hint"));
        libraryHint->setWordWrap(true);
        sidebarLayout->addWidget(libraryHint);
        bodyLayout->addWidget(sidebar);

        pages_ = new QStackedWidget(body);
        homePage_ = new QWidget(pages_);
        auto *homeLayout = new QVBoxLayout(homePage_);
        homeLayout->setContentsMargins(28, 32, 28, 24);
        homeLayout->setSpacing(18);

        auto *welcome = new QLabel(tr("What do you want to watch?"), homePage_);
        welcome->setObjectName(QStringLiteral("homeTitle"));
        homeLayout->addWidget(welcome);
        auto *homeSubtitle = new QLabel(tr("Open a streaming site, search the web, or play something from your library."), homePage_);
        homeSubtitle->setObjectName(QStringLiteral("homeSubtitle"));
        homeLayout->addWidget(homeSubtitle);

        quickLinksGrid_ = new QGridLayout();
        quickLinksGrid_->setSpacing(14);
        homeLayout->addSpacing(16);
        homeLayout->addLayout(quickLinksGrid_);
        homeLayout->addStretch(1);
        auto *homeTip = new QLabel(tr("Type a site address or search term in the top bar. Your shortcuts stay on this device."), homePage_);
        homeTip->setObjectName(QStringLiteral("hint"));
        homeTip->setWordWrap(true);
        homeLayout->addWidget(homeTip);

        const QString profilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QStringLiteral("/web-profile");
        QDir().mkpath(profilePath);
        browserProfile_ = new QWebEngineProfile(QStringLiteral("NebulaVideo"), this);
        browserProfile_->setPersistentStoragePath(profilePath);
        browserProfile_->setCachePath(profilePath + QStringLiteral("/cache"));
        browserProfile_->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);
        browserProfile_->setHttpAcceptLanguage(selectedHttpLanguage());
        adBlocker_ = new NebulaAdBlocker(browserProfile_);
        adBlocker_->setEnabled(adBlockButton_->isChecked());
        browserProfile_->setUrlRequestInterceptor(adBlocker_);

        browser_ = new QWebEngineView(pages_);
        auto *browserPage = new QWebEnginePage(browserProfile_, browser_);
        browser_->setPage(browserPage);
        browser_->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
        browser_->settings()->setAttribute(QWebEngineSettings::AutoLoadImages, true);
        browser_->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
        browser_->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
        browser_->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
        playerPage_ = new QWidget(pages_);
        auto *playerLayout = new QVBoxLayout(playerPage_);
        playerLayout->setContentsMargins(22, 22, 22, 18);
        playerLayout->setSpacing(12);

        videoTitle_ = new QLabel(tr("Choose a video from your library"), playerPage_);
        videoTitle_->setObjectName(QStringLiteral("videoTitle"));
        playerLayout->addWidget(videoTitle_);

        videoWidget_ = new QVideoWidget(playerPage_);
        videoWidget_->setObjectName(QStringLiteral("videoWidget"));
        videoWidget_->setMinimumSize(200, 140);
        playerLayout->addWidget(videoWidget_, 1);

        auto *controls = new QHBoxLayout();
        playButton_ = new QPushButton(tr("Play"), playerPage_);
        playButton_->setObjectName(QStringLiteral("playButton"));
        positionSlider_ = new QSlider(Qt::Horizontal, playerPage_);
        positionSlider_->setRange(0, 0);
        timeLabel_ = new QLabel(QStringLiteral("00:00 / 00:00"), playerPage_);
        timeLabel_->setObjectName(QStringLiteral("timeLabel"));
        controls->addWidget(playButton_);
        controls->addWidget(positionSlider_, 1);
        audioTrackSelector_ = new QComboBox(playerPage_);
        audioTrackSelector_->setToolTip(tr("Audio track"));
        subtitleTrackSelector_ = new QComboBox(playerPage_);
        subtitleTrackSelector_->setToolTip(tr("Subtitles"));
        controls->addWidget(audioTrackSelector_);
        controls->addWidget(subtitleTrackSelector_);
        controls->addWidget(timeLabel_);
        playerLayout->addLayout(controls);

        pages_->addWidget(homePage_);
        pages_->addWidget(browser_);
        pages_->addWidget(playerPage_);
        bodyLayout->addWidget(pages_, 1);
        rootLayout->addWidget(body, 1);
        setCentralWidget(root);

        audioOutput_ = new QAudioOutput(this);
        mediaPlayer_ = new QMediaPlayer(this);
        mediaPlayer_->setAudioOutput(audioOutput_);
        mediaPlayer_->setVideoOutput(videoWidget_);
        initializeQuickLinks();

        connect(backButton, &QToolButton::clicked, browser_, &QWebEngineView::back);
        connect(forwardButton, &QToolButton::clicked, browser_, &QWebEngineView::forward);
        connect(reloadButton, &QToolButton::clicked, browser_, &QWebEngineView::reload);
        connect(externalButton, &QPushButton::clicked, this, [this] {
            if (browser_->url().isValid() && !browser_->url().isEmpty() && !QDesktopServices::openUrl(browser_->url()))
                QMessageBox::warning(this, tr("Could not open browser"), tr("No system browser is available."));
        });
        connect(mediaCheckButton, &QToolButton::clicked, this, [this] { checkMediaSupport(); });
        connect(libraryToggleButton_, &QToolButton::toggled, this, [this](bool visible) {
            if (compactMode_)
                sidebar_->setVisible(visible);
        });
        connect(vpnButton, &QPushButton::clicked, this, [this] { showVpnManager(); });
        connect(settingsButton, &QPushButton::clicked, this, [this] { showSettingsDialog(); });
        connect(adBlockButton_, &QToolButton::toggled, this, [this](bool enabled) {
            adBlocker_->setEnabled(enabled);
            settings_.setValue(QStringLiteral("browser/adBlockEnabled"), enabled);
            updateAdBlockButton();
            if (browser_->url().isValid() && !browser_->url().isEmpty())
                browser_->reload();
        });
        connect(homeButton, &QToolButton::clicked, this, [this] { showHomePage(); });
        connect(goButton, &QPushButton::clicked, this, [this] { navigateFromAddress(); });
        connect(addressBar_, &QLineEdit::returnPressed, this, [this] { navigateFromAddress(); });
        connect(browser_, &QWebEngineView::urlChanged, this, [this](const QUrl &url) {
            if (pages_->currentWidget() == browser_)
                addressBar_->setText(url.toString());
        });
        connect(browser_, &QWebEngineView::titleChanged, this, [this](const QString &title) {
            if (pages_->currentWidget() == browser_)
                setWindowTitle(title.isEmpty() ? QStringLiteral("Nebula Video") : title + QStringLiteral(" — Nebula Video"));
        });
        connect(browser_, &QWebEngineView::loadStarted, this, [this] {
            statusBar()->showMessage(tr("Loading page…"));
        });
        connect(browser_, &QWebEngineView::loadProgress, this, [this](int progress) {
            statusBar()->showMessage(tr("Loading page… %1%").arg(progress));
        });
        connect(browser_, &QWebEngineView::loadFinished, this, [this](bool ok) {
            const QString result = ok ? tr("Page loaded") : tr("Page did not load completely — try Reload or System browser");
            statusBar()->showMessage(result + QStringLiteral("  ·  ") + tr("Blocked requests: %1").arg(adBlocker_->blockedCount()), 7000);
        });
        connect(browserPage, &QWebEnginePage::fullScreenRequested, this, [this](QWebEngineFullScreenRequest request) {
            request.accept();
            if (request.toggleOn()) {
                wasMaximizedBeforeFullscreen_ = isMaximized();
                toolbar_->hide();
                sidebar_->hide();
                showFullScreen();
            } else {
                if (wasMaximizedBeforeFullscreen_)
                    showMaximized();
                else
                    showNormal();
                toolbar_->show();
                sidebar_->show();
            }
        });
        connect(browserPage, &QWebEnginePage::newWindowRequested, this, [this](QWebEngineNewWindowRequest &request) {
            if (request.isUserInitiated()) {
                pages_->setCurrentWidget(browser_);
                request.openIn(browser_->page());
            }
        });
        connect(addFolderButton, &QPushButton::clicked, this, [this] { chooseVideoFolder(); });
        connect(libraryFilter_, &QLineEdit::textChanged, this, [this](const QString &text) {
            for (int i = 0; i < videoList_->count(); ++i) {
                auto *item = videoList_->item(i);
                item->setHidden(!item->text().contains(text, Qt::CaseInsensitive) &&
                                !item->toolTip().contains(text, Qt::CaseInsensitive));
            }
        });
        connect(videoList_, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item) {
            playVideo(item->data(Qt::UserRole).toString());
        });
        connect(playButton_, &QPushButton::clicked, this, [this] {
            if (mediaPlayer_->playbackState() == QMediaPlayer::PlayingState) {
                mediaPlayer_->pause();
            } else {
                mediaPlayer_->play();
            }
        });
        connect(mediaPlayer_, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state) {
            playButton_->setText(state == QMediaPlayer::PlayingState ? tr("Pause") : tr("Play"));
        });
        connect(mediaPlayer_, &QMediaPlayer::durationChanged, this, [this](qint64 duration) {
            positionSlider_->setRange(0, static_cast<int>(qMin<qint64>(duration, std::numeric_limits<int>::max())));
            updateTimeLabel();
        });
        connect(mediaPlayer_, &QMediaPlayer::tracksChanged, this, [this] { refreshMediaTrackSelectors(); });
        connect(audioTrackSelector_, &QComboBox::currentIndexChanged, this, [this](int index) {
            if (index >= 0)
                mediaPlayer_->setActiveAudioTrack(audioTrackSelector_->itemData(index).toInt());
        });
        connect(subtitleTrackSelector_, &QComboBox::currentIndexChanged, this, [this](int index) {
            if (index >= 0)
                mediaPlayer_->setActiveSubtitleTrack(subtitleTrackSelector_->itemData(index).toInt());
        });
        connect(mediaPlayer_, &QMediaPlayer::positionChanged, this, [this](qint64 position) {
            if (!positionSlider_->isSliderDown())
                positionSlider_->setValue(static_cast<int>(position));
            updateTimeLabel();
        });
        connect(positionSlider_, &QSlider::sliderMoved, this, [this](int position) {
            mediaPlayer_->setPosition(position);
        });
        connect(mediaPlayer_, &QMediaPlayer::errorOccurred, this,
                [this](QMediaPlayer::Error, const QString &error) {
                    if (!error.isEmpty())
                        QMessageBox::warning(this, tr("Playback error"), error);
                });

        loadSavedFolders();
        updateAdBlockButton();
        auto *adBlockUiTimer = new QTimer(this);
        adBlockUiTimer->setInterval(1000);
        connect(adBlockUiTimer, &QTimer::timeout, this, [this] { updateAdBlockButton(); });
        adBlockUiTimer->start();
        pages_->setCurrentWidget(homePage_);
        updateResponsiveLayout(true);
    }

protected:
    void resizeEvent(QResizeEvent *event) override {
        QMainWindow::resizeEvent(event);
        updateResponsiveLayout();
    }

private:
    void updateResponsiveLayout(bool force = false) {
        if (!sidebar_ || !libraryToggleButton_)
            return;
        const bool compact = width() < 1050;
        if (force || compact != compactMode_) {
            compactMode_ = compact;
            if (compactMode_) {
                sidebar_->hide();
                libraryToggleButton_->setChecked(false);
                libraryToggleButton_->show();
                brand_->hide();
                goButton_->hide();
                externalButton_->setText(QString());
                externalButton_->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
                externalButton_->setToolTip(tr("System browser"));
                settingsButton_->setText(QStringLiteral("⚙"));
                settingsButton_->setToolTip(tr("Settings"));
            } else {
                sidebar_->show();
                libraryToggleButton_->hide();
                brand_->show();
                goButton_->show();
                externalButton_->setText(tr("System browser"));
                externalButton_->setIcon(QIcon());
                settingsButton_->setText(tr("Settings"));
            }
        }
        const int columns = width() < 560 ? 1 : width() < 850 ? 2 : 3;
        if (columns != quickLinksColumns_) {
            quickLinksColumns_ = columns;
            rebuildQuickLinks();
        }
        updateAdBlockButton();
    }

    void updateAdBlockButton() {
        if (!adBlockButton_ || !adBlocker_)
            return;
        const QString state = adBlocker_->isEnabled() ? tr("ON") : tr("OFF");
        adBlockButton_->setText(compactMode_ ? tr("Ad %1").arg(state)
                                             : tr("Adblock %1 · %2").arg(state).arg(adBlocker_->blockedCount()));
        adBlockButton_->setToolTip(tr("Common ad and tracker domains blocked this session: %1. Click to toggle; the page reloads after a change.")
                                       .arg(adBlocker_->blockedCount()));
    }

    QString selectedHttpLanguage() const {
        return settings_.value(QStringLiteral("app/language"), QStringLiteral("ru")).toString() == QStringLiteral("ru")
            ? QStringLiteral("ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7")
            : QStringLiteral("en-US,en;q=0.9,ru;q=0.7");
    }

    void showSettingsDialog() {
        QDialog dialog(this);
        dialog.setWindowTitle(tr("Settings"));
        dialog.setMinimumWidth(440);
        auto *layout = new QVBoxLayout(&dialog);
        auto *form = new QFormLayout();
        auto *language = new QComboBox(&dialog);
        language->addItem(tr("English"), QStringLiteral("en"));
        language->addItem(tr("Russian"), QStringLiteral("ru"));
        const QString current = settings_.value(QStringLiteral("app/language"), QStringLiteral("ru")).toString();
        language->setCurrentIndex(qMax(0, language->findData(current)));
        form->addRow(tr("Language"), language);
        auto *theme = new QComboBox(&dialog);
        const QList<QPair<QString, QString>> themes = {
            {QStringLiteral("auto"), tr("Automatic (match distribution)")},
            {QStringLiteral("nebula"), tr("Nebula Purple")},
            {QStringLiteral("ubuntu"), tr("Ubuntu Orange")},
            {QStringLiteral("fedora"), tr("Fedora Blue")},
            {QStringLiteral("arch"), tr("Arch Cyan")},
            {QStringLiteral("debian"), tr("Debian Red")},
            {QStringLiteral("mint"), tr("Linux Mint Green")},
            {QStringLiteral("opensuse"), tr("openSUSE Green")},
            {QStringLiteral("kde"), tr("KDE Breeze Blue")}
        };
        for (const auto &entry : themes)
            theme->addItem(entry.second, entry.first);
        const QString currentTheme = settings_.value(QStringLiteral("app/theme"), QStringLiteral("auto")).toString();
        theme->setCurrentIndex(qMax(0, theme->findData(currentTheme)));
        form->addRow(tr("Theme"), theme);
        auto *system = new QLabel(QSysInfo::prettyProductName(), &dialog);
        form->addRow(tr("Linux distribution"), system);
        layout->addLayout(form);
        auto *note = new QLabel(tr("Language changes apply after restarting Nebula Video. This preference also sets the preferred language sent to websites."), &dialog);
        note->setWordWrap(true);
        layout->addWidget(note);
        auto *buttons = new QHBoxLayout();
        auto *save = new QPushButton(tr("Save"), &dialog);
        auto *close = new QPushButton(tr("Close"), &dialog);
        buttons->addStretch();
        buttons->addWidget(save);
        buttons->addWidget(close);
        layout->addLayout(buttons);
        connect(close, &QPushButton::clicked, &dialog, &QDialog::reject);
        connect(save, &QPushButton::clicked, &dialog, [&dialog, this, language, theme, current] {
            const QString chosen = language->currentData().toString();
            if (chosen != current) {
                settings_.setValue(QStringLiteral("app/language"), chosen);
                QMessageBox::information(&dialog, tr("Settings"), tr("Restart Nebula Video to apply the interface language."));
            }
            const QString chosenTheme = theme->currentData().toString();
            settings_.setValue(QStringLiteral("app/theme"), chosenTheme);
            applyApplicationTheme(*qApp, chosenTheme);
            dialog.accept();
        });
        dialog.exec();
    }

    void showVpnManager() {
        auto *dialog = new QDialog(this);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->setWindowTitle(tr("VPN connections"));
        dialog->setMinimumWidth(520);
        auto *layout = new QVBoxLayout(dialog);
        auto *description = new QLabel(tr("Manage VPN profiles saved in NetworkManager. Nebula Video does not provide VPN servers or accounts."), dialog);
        description->setWordWrap(true);
        layout->addWidget(description);
        auto *profiles = new QComboBox(dialog);
        layout->addWidget(profiles);
        auto *status = new QLabel(tr("Loading VPN profiles…"), dialog);
        status->setWordWrap(true);
        layout->addWidget(status);
        auto *actions = new QHBoxLayout();
        auto *import = new QPushButton(tr("Import .ovpn"), dialog);
        auto *connectButton = new QPushButton(tr("Connect"), dialog);
        auto *disconnectButton = new QPushButton(tr("Disconnect"), dialog);
        auto *refresh = new QPushButton(tr("Refresh"), dialog);
        auto *close = new QPushButton(tr("Close"), dialog);
        actions->addWidget(import);
        actions->addWidget(connectButton);
        actions->addWidget(disconnectButton);
        actions->addWidget(refresh);
        actions->addWidget(close);
        layout->addLayout(actions);

        auto activeNames = std::make_shared<QSet<QString>>();
        auto updateStatus = [profiles, status, activeNames, connectButton, disconnectButton, this] {
            const QString name = profiles->currentText();
            const bool active = activeNames->contains(name);
            if (name.isEmpty())
                status->setText(tr("No NetworkManager VPN profile found. Import a provider configuration or add one in your system network settings."));
            else
                status->setText(active ? tr("Connected: %1").arg(name) : tr("Disconnected: %1").arg(name));
            connectButton->setEnabled(!name.isEmpty() && !active);
            disconnectButton->setEnabled(!name.isEmpty() && active);
        };

        auto runNmcli = [dialog, status](const QStringList &arguments, std::function<void(int, const QString &)> done) {
            auto *process = new QProcess(dialog);
            process->setProgram(QStringLiteral("nmcli"));
            process->setArguments(arguments);
            auto completed = std::make_shared<bool>(false);
            auto finish = [process, done, completed](int code) {
                if (*completed)
                    return;
                *completed = true;
                const QString output = QString::fromLocal8Bit(process->readAllStandardOutput()).trimmed();
                const QString error = QString::fromLocal8Bit(process->readAllStandardError()).trimmed();
                done(code, output.isEmpty() ? error : output);
                process->deleteLater();
            };
            QObject::connect(process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), process,
                [finish](int code, QProcess::ExitStatus) { finish(code); });
            QObject::connect(process, &QProcess::errorOccurred, process, [finish, status](QProcess::ProcessError error) {
                if (error == QProcess::FailedToStart) {
                    status->setText(QObject::tr("nmcli is unavailable. Install NetworkManager to use VPN controls."));
                    finish(-1);
                }
            });
            status->setText(tr("Working with NetworkManager…"));
            process->start();
        };

        auto refreshProfiles = [runNmcli, profiles, activeNames, updateStatus, status] {
            runNmcli({QStringLiteral("-t"), QStringLiteral("-f"), QStringLiteral("NAME,TYPE"), QStringLiteral("connection"), QStringLiteral("show")},
                [runNmcli, profiles, activeNames, updateStatus, status](int code, const QString &output) {
                    profiles->clear();
                    if (code != 0) {
                        status->setText(output.isEmpty() ? QObject::tr("Could not read NetworkManager profiles.") : output);
                        return;
                    }
                    for (const QString &line : output.split(QLatin1Char('\n'), Qt::SkipEmptyParts)) {
                        const qsizetype separator = line.lastIndexOf(QLatin1Char(':'));
                        if (separator < 0)
                            continue;
                        const QString name = line.left(separator).trimmed();
                        const QString type = line.mid(separator + 1).trimmed();
                        if (type == QStringLiteral("vpn") || type == QStringLiteral("wireguard"))
                            profiles->addItem(name);
                    }
                    runNmcli({QStringLiteral("-t"), QStringLiteral("-f"), QStringLiteral("NAME,TYPE"), QStringLiteral("connection"), QStringLiteral("show"), QStringLiteral("--active")},
                        [profiles, activeNames, updateStatus](int activeCode, const QString &activeOutput) {
                            activeNames->clear();
                            if (activeCode == 0) {
                                for (const QString &line : activeOutput.split(QLatin1Char('\n'), Qt::SkipEmptyParts)) {
                                    const qsizetype separator = line.lastIndexOf(QLatin1Char(':'));
                                    if (separator < 0)
                                        continue;
                                    const QString name = line.left(separator).trimmed();
                                    const QString type = line.mid(separator + 1).trimmed();
                                    if (type == QStringLiteral("vpn") || type == QStringLiteral("wireguard"))
                                        activeNames->insert(name);
                                }
                            }
                            updateStatus();
                        });
                });
        };

        connect(refresh, &QPushButton::clicked, dialog, refreshProfiles);
        connect(profiles, &QComboBox::currentIndexChanged, dialog, [updateStatus] { updateStatus(); });
        connect(import, &QPushButton::clicked, dialog, [this, dialog, runNmcli, refresh] {
            const QString path = QFileDialog::getOpenFileName(dialog, tr("Import VPN configuration"), QDir::homePath(), tr("OpenVPN configuration (*.ovpn);;All files (*)"));
            if (path.isEmpty())
                return;
            runNmcli({QStringLiteral("connection"), QStringLiteral("import"), QStringLiteral("type"), QStringLiteral("openvpn"), QStringLiteral("file"), path},
                [dialog, refresh](int code, const QString &output) {
                    if (code == 0)
                        QMessageBox::information(dialog, QObject::tr("VPN"), QObject::tr("VPN profile imported. Configure credentials in NetworkManager if requested."));
                    else
                        QMessageBox::warning(dialog, QObject::tr("VPN import failed"), output);
                    QMetaObject::invokeMethod(refresh, "click", Qt::QueuedConnection);
                });
        });
        auto activate = [profiles, activeNames, runNmcli, refresh, status](bool disconnect) {
            const QString name = profiles->currentText();
            if (name.isEmpty())
                return;
            QStringList arguments{QStringLiteral("connection"), disconnect ? QStringLiteral("down") : QStringLiteral("up"), QStringLiteral("id"), name};
            if (!disconnect) {
                arguments.prepend(QStringLiteral("30"));
                arguments.prepend(QStringLiteral("--wait"));
            }
            runNmcli(arguments, [disconnect, refresh, status](int code, const QString &output) {
                if (code != 0)
                    status->setText(output.isEmpty() ? QObject::tr("VPN operation failed. Check the profile and permissions in NetworkManager.") : output);
                else
                    status->setText(disconnect ? QObject::tr("VPN disconnected.") : QObject::tr("VPN connected."));
                QMetaObject::invokeMethod(refresh, "click", Qt::QueuedConnection);
            });
        };
        connect(connectButton, &QPushButton::clicked, dialog, [activate] { activate(false); });
        connect(disconnectButton, &QPushButton::clicked, dialog, [activate] { activate(true); });
        connect(close, &QPushButton::clicked, dialog, &QDialog::close);
        dialog->show();
        refreshProfiles();
    }

    void refreshMediaTrackSelectors() {
        const QSignalBlocker audioBlocker(audioTrackSelector_);
        const QSignalBlocker subtitleBlocker(subtitleTrackSelector_);
        audioTrackSelector_->clear();
        subtitleTrackSelector_->clear();
        const auto audioTracks = mediaPlayer_->audioTracks();
        for (int i = 0; i < audioTracks.size(); ++i) {
            QString label = audioTracks.at(i).stringValue(QMediaMetaData::Language);
            if (label.isEmpty())
                label = tr("Track %1").arg(i + 1);
            audioTrackSelector_->addItem(label, i);
        }
        const auto subtitleTracks = mediaPlayer_->subtitleTracks();
        subtitleTrackSelector_->addItem(tr("Off"), -1);
        for (int i = 0; i < subtitleTracks.size(); ++i) {
            QString label = subtitleTracks.at(i).stringValue(QMediaMetaData::Language);
            if (label.isEmpty())
                label = tr("Track %1").arg(i + 1);
            subtitleTrackSelector_->addItem(label, i);
        }
        audioTrackSelector_->setEnabled(!audioTracks.isEmpty());
        subtitleTrackSelector_->setEnabled(!subtitleTracks.isEmpty());
        audioTrackSelector_->setCurrentIndex(audioTrackSelector_->findData(mediaPlayer_->activeAudioTrack()));
        const int subtitle = subtitleTrackSelector_->findData(mediaPlayer_->activeSubtitleTrack());
        subtitleTrackSelector_->setCurrentIndex(subtitle < 0 ? 0 : subtitle);
    }

    static QToolButton *makeToolButton(const QIcon &icon, const QString &tip) {
        auto *button = new QToolButton();
        button->setIcon(icon);
        button->setToolTip(tip);
        button->setAutoRaise(true);
        return button;
    }

    void navigateFromAddress() {
        QString input = addressBar_->text().trimmed();
        if (input.isEmpty())
            return;

        pages_->setCurrentWidget(browser_);
        setWindowTitle(QStringLiteral("Nebula Video"));
        QUrl url = QUrl::fromUserInput(input);
        const bool looksLikeAddress = input.contains(QStringLiteral(".")) &&
                                      !input.contains(QLatin1Char(' ')) &&
                                      !input.startsWith(QStringLiteral("?"));
        if (!url.isValid() || (!looksLikeAddress && !input.startsWith(QStringLiteral("http://")) &&
                               !input.startsWith(QStringLiteral("https://")))) {
            url = QUrl(QStringLiteral("https://www.google.com/search?q=") +
                       QString::fromLatin1(QUrl::toPercentEncoding(input)));
        } else if (url.scheme().isEmpty()) {
            url.setScheme(QStringLiteral("https"));
        }
        browser_->setUrl(url);
    }

    void showHomePage() {
        pages_->setCurrentWidget(homePage_);
        addressBar_->clear();
        setWindowTitle(QStringLiteral("Nebula Video"));
    }

    void initializeQuickLinks() {
        const QString key = QStringLiteral("home/shortcuts");
        QVariantList shortcuts = settings_.value(key).toList();
        shortcuts.erase(std::remove_if(shortcuts.begin(), shortcuts.end(), [](const QVariant &value) {
            const QVariantMap item = value.toMap();
            return item.value(QStringLiteral("name")).toString().contains(QStringLiteral("rezka"), Qt::CaseInsensitive)
                || item.value(QStringLiteral("url")).toString().contains(QStringLiteral("rezka"), Qt::CaseInsensitive);
        }), shortcuts.end());
        const auto addDefault = [&shortcuts](const QString &name, const QString &url, const QString &description) {
            for (const QVariant &value : shortcuts) {
                if (value.toMap().value(QStringLiteral("url")).toString() == url)
                    return;
            }
            QVariantMap item;
            item.insert(QStringLiteral("name"), name);
            item.insert(QStringLiteral("url"), url);
            item.insert(QStringLiteral("description"), description);
            shortcuts.append(item);
        };
        addDefault(QStringLiteral("YouTube"), QStringLiteral("https://www.youtube.com"), tr("Videos and channels"));
        addDefault(QStringLiteral("Netflix"), QStringLiteral("https://www.netflix.com"), tr("Movies and series"));
        addDefault(QStringLiteral("Crunchyroll"), QStringLiteral("https://www.crunchyroll.com"), tr("Anime"));
        addDefault(QStringLiteral("YummyAnime TV"), QStringLiteral("https://yummyanime.tv"), tr("Anime"));
        settings_.setValue(key, shortcuts);
        rebuildQuickLinks();
    }

    void rebuildQuickLinks() {
        while (QLayoutItem *item = quickLinksGrid_->takeAt(0)) {
            if (QWidget *widget = item->widget())
                widget->deleteLater();
            delete item;
        }

        const QVariantList shortcuts = settings_.value(QStringLiteral("home/shortcuts")).toList();
        int index = 0;
        for (const QVariant &value : shortcuts) {
            const QVariantMap shortcut = value.toMap();
            const QString name = shortcut.value(QStringLiteral("name")).toString();
            const QString url = shortcut.value(QStringLiteral("url")).toString();
            const QString description = shortcut.value(QStringLiteral("description")).toString();
            if (name.isEmpty() || url.isEmpty())
                continue;

            auto *tile = new QPushButton(name + QStringLiteral("\n") + description, homePage_);
            tile->setObjectName(QStringLiteral("siteTile"));
            tile->setMinimumSize(150, 96);
            tile->setCursor(Qt::PointingHandCursor);
            tile->setToolTip(url);
            connect(tile, &QPushButton::clicked, this, [this, url] { openSite(url); });
            quickLinksGrid_->addWidget(tile, index / quickLinksColumns_, index % quickLinksColumns_);
            ++index;
        }

        auto *addTile = new QPushButton(tr("＋\nAdd a site"), homePage_);
        addTile->setObjectName(QStringLiteral("addShortcutTile"));
        addTile->setMinimumSize(150, 96);
        addTile->setCursor(Qt::PointingHandCursor);
        connect(addTile, &QPushButton::clicked, this, [this] { addQuickLink(); });
        quickLinksGrid_->addWidget(addTile, index / quickLinksColumns_, index % quickLinksColumns_);
    }

    void addQuickLink() {
        bool accepted = false;
        const QString name = QInputDialog::getText(this, tr("Add a site"), tr("Site name:"),
                                                    QLineEdit::Normal, QString(), &accepted).trimmed();
        if (!accepted || name.isEmpty())
            return;

        const QString input = QInputDialog::getText(this, tr("Add a site"), tr("Website address:"),
                                                    QLineEdit::Normal, QStringLiteral("https://"), &accepted).trimmed();
        if (!accepted || input.isEmpty())
            return;

        QString normalized = input;
        if (!normalized.contains(QStringLiteral("://")))
            normalized.prepend(QStringLiteral("https://"));
        const QUrl url(normalized);
        if (!url.isValid() || url.host().isEmpty() || (url.scheme() != QStringLiteral("http") && url.scheme() != QStringLiteral("https"))) {
            QMessageBox::warning(this, tr("Invalid address"), tr("Enter a valid HTTP or HTTPS website address."));
            return;
        }

        QVariantMap shortcut;
        shortcut.insert(QStringLiteral("name"), name);
        shortcut.insert(QStringLiteral("url"), url.toString());
        shortcut.insert(QStringLiteral("description"), tr("Custom site"));
        QVariantList shortcuts = settings_.value(QStringLiteral("home/shortcuts")).toList();
        shortcuts.append(shortcut);
        settings_.setValue(QStringLiteral("home/shortcuts"), shortcuts);
        rebuildQuickLinks();
    }

    void openSite(const QString &address) {
        QUrl url(address);
        if (!url.isValid() || url.host().isEmpty())
            return;
        addressBar_->setText(url.toString());
        pages_->setCurrentWidget(browser_);
        setWindowTitle(QStringLiteral("Nebula Video"));
        browser_->setUrl(url);
    }

    void checkMediaSupport() {
        if (pages_->currentWidget() != browser_) {
            QMessageBox::information(this, tr("Media check"), tr("Open a website first, then check its video support."));
            return;
        }

        const QString script = QStringLiteral(R"JS(
            (() => {
                const probe = document.createElement('video');
                const images = Array.from(document.images);
                return JSON.stringify({
                    h264: probe.canPlayType('video/mp4; codecs="avc1.42E01E"'),
                    webm: probe.canPlayType('video/webm; codecs="vp9"'),
                    videos: document.querySelectorAll('video').length,
                    frames: document.querySelectorAll('iframe').length,
                    images: images.length,
                    loadedImages: images.filter(image => image.complete && image.naturalWidth > 0).length,
                    failedImages: images.filter(image => image.complete && image.naturalWidth === 0 && image.currentSrc).length,
                    failedImageUrls: images
                        .filter(image => image.complete && image.naturalWidth === 0 && image.currentSrc)
                        .slice(0, 5)
                        .map(image => image.currentSrc),
                    videoState: Array.from(document.querySelectorAll('video')).slice(0, 3).map(video => ({
                        readyState: video.readyState,
                        networkState: video.networkState,
                        error: video.error ? video.error.code : 0
                    }))
                });
            })()
        )JS");

        browser_->page()->runJavaScript(script, [this](const QVariant &result) {
            const QJsonObject report = QJsonDocument::fromJson(result.toString().toUtf8()).object();
            if (report.isEmpty()) {
                QMessageBox box(QMessageBox::Information, tr("Media check"),
                    tr("The page did not expose media information. Its player may be inside a protected or cross-origin frame. Try System browser."), QMessageBox::Ok, this);
                box.setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
                box.exec();
                return;
            }

            const QString h264 = report.value(QStringLiteral("h264")).toString();
            const QString webm = report.value(QStringLiteral("webm")).toString();
            const QJsonArray failedImages = report.value(QStringLiteral("failedImageUrls")).toArray();
            const QJsonArray videoStates = report.value(QStringLiteral("videoState")).toArray();
            QString message = tr("Embedded browser support on this page:\n\n"
                                 "MP4 / H.264: %1\n"
                                 "WebM / VP9: %2\n"
                                 "Video elements found: %3\n"
                                 "Embedded frames: %4\n"
                                 "Images loaded: %5 of %6\n"
                                 "Images with load errors: %7\n"
                                 "HTML5 player state: %8")
                .arg(h264.isEmpty() ? tr("unavailable") : tr("available"))
                .arg(webm.isEmpty() ? tr("unavailable") : tr("available"))
                .arg(report.value(QStringLiteral("videos")).toInt())
                .arg(report.value(QStringLiteral("frames")).toInt())
                .arg(report.value(QStringLiteral("loadedImages")).toInt())
                .arg(report.value(QStringLiteral("images")).toInt())
                .arg(report.value(QStringLiteral("failedImages")).toInt())
                .arg(videoStates.isEmpty() ? tr("No visible HTML5 video (may be inside a frame or custom player)") : tr("details below"));

            for (const QJsonValue &value : videoStates) {
                const QJsonObject video = value.toObject();
                const int errorCode = video.value(QStringLiteral("error")).toInt();
                QString errorMeaning = tr("none reported");
                if (errorCode == 1)
                    errorMeaning = tr("playback was aborted");
                else if (errorCode == 2)
                    errorMeaning = tr("media network error");
                else if (errorCode == 3)
                    errorMeaning = tr("media decoding failed (codec or damaged stream)");
                else if (errorCode == 4)
                    errorMeaning = tr("media format or source is unsupported");
                message += QStringLiteral("\n\n") + tr("Player: readyState %1, networkState %2, error %3")
                    .arg(video.value(QStringLiteral("readyState")).toInt())
                    .arg(video.value(QStringLiteral("networkState")).toInt())
                    .arg(errorCode);
                message += QStringLiteral(" — ") + errorMeaning;
            }

            if (!failedImages.isEmpty()) {
                message += QStringLiteral("\n\n") + tr("First image URLs that failed:");
                for (const QJsonValue &value : failedImages)
                    message += QStringLiteral("\n• ") + value.toString();
            }

            if (h264.isEmpty())
                message += QStringLiteral("\n\n") + tr("This Qt WebEngine build cannot play common H.264 streams. Use System browser for this page.");
            if (report.value(QStringLiteral("frames")).toInt() > 0 && report.value(QStringLiteral("videos")).toInt() == 0)
                message += QStringLiteral("\n\n") + tr("The player appears to be inside a frame, so its format cannot be inspected from this page.");
            QMessageBox box(QMessageBox::Information, tr("Media check"), message, QMessageBox::Ok, this);
            box.setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
            box.exec();
        });
    }

    void chooseVideoFolder() {
        const QString folder = QFileDialog::getExistingDirectory(this, tr("Add a video folder"),
                                                                  QStandardPaths::writableLocation(QStandardPaths::MoviesLocation));
        if (folder.isEmpty())
            return;

        QFileInfo info(folder);
        const QString canonical = info.canonicalFilePath();
        if (canonical.isEmpty())
            return;

        QStringList folders = settings_.value(QStringLiteral("library/folders")).toStringList();
        if (!folders.contains(canonical)) {
            folders.append(canonical);
            settings_.setValue(QStringLiteral("library/folders"), folders);
        }
        scanFolder(canonical);
    }

    void loadSavedFolders() {
        QStringList folders = settings_.value(QStringLiteral("library/folders")).toStringList();
        if (folders.isEmpty()) {
            const QStringList defaults = {
                QStandardPaths::writableLocation(QStandardPaths::MoviesLocation),
                QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)
            };
            for (const QString &folder : defaults) {
                const QString canonical = QFileInfo(folder).canonicalFilePath();
                if (!canonical.isEmpty() && !folders.contains(canonical))
                    folders.append(canonical);
            }
            settings_.setValue(QStringLiteral("library/folders"), folders);
        }
        for (const QString &folder : folders)
            scanFolder(folder);
    }

    void scanFolder(const QString &folder) {
        if (!QDir(folder).exists())
            return;

        static const QSet<QString> supportedExtensions = {
            QStringLiteral("mp4"), QStringLiteral("m4v"), QStringLiteral("mkv"),
            QStringLiteral("mov"), QStringLiteral("avi"), QStringLiteral("webm"),
            QStringLiteral("mpeg"), QStringLiteral("mpg"), QStringLiteral("ogv"),
            QStringLiteral("3gp"), QStringLiteral("ts")
        };

        QDirIterator it(folder, QDir::Files | QDir::Readable, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            const QString path = it.next();
            const QFileInfo info = it.fileInfo();
            if (!supportedExtensions.contains(info.suffix().toLower()) || videoPaths_.contains(path))
                continue;

            videoPaths_.insert(path);
            auto *item = new QListWidgetItem(info.completeBaseName(), videoList_);
            item->setToolTip(path);
            item->setData(Qt::UserRole, path);
        }
    }

    void playVideo(const QString &path) {
        const QFileInfo info(path);
        if (!info.exists() || !info.isFile()) {
            QMessageBox::warning(this, tr("Video not found"), tr("The selected file is no longer available."));
            return;
        }

        videoTitle_->setText(info.completeBaseName());
        setWindowTitle(info.completeBaseName() + QStringLiteral(" — Nebula Video"));
        pages_->setCurrentWidget(playerPage_);
        mediaPlayer_->setSource(QUrl::fromLocalFile(info.absoluteFilePath()));
        mediaPlayer_->play();
    }

    void updateTimeLabel() {
        const auto format = [](qint64 ms) {
            const qint64 seconds = ms / 1000;
            const qint64 hours = seconds / 3600;
            const qint64 minutes = (seconds % 3600) / 60;
            const qint64 remainder = seconds % 60;
            if (hours > 0)
                return QStringLiteral("%1:%2:%3").arg(hours).arg(minutes, 2, 10, QLatin1Char('0')).arg(remainder, 2, 10, QLatin1Char('0'));
            return QStringLiteral("%1:%2").arg(minutes).arg(remainder, 2, 10, QLatin1Char('0'));
        };
        timeLabel_->setText(format(mediaPlayer_->position()) + QStringLiteral(" / ") + format(mediaPlayer_->duration()));
    }

    QSettings settings_{QStringLiteral("NebulaProject"), QStringLiteral("NebulaVideo")};
    QLineEdit *addressBar_ = nullptr;
    QLineEdit *libraryFilter_ = nullptr;
    QListWidget *videoList_ = nullptr;
    QStackedWidget *pages_ = nullptr;
    QWidget *homePage_ = nullptr;
    QGridLayout *quickLinksGrid_ = nullptr;
    QWebEngineView *browser_ = nullptr;
    QWebEngineProfile *browserProfile_ = nullptr;
    NebulaAdBlocker *adBlocker_ = nullptr;
    QToolButton *adBlockButton_ = nullptr;
    QToolButton *libraryToggleButton_ = nullptr;
    QLabel *brand_ = nullptr;
    QPushButton *goButton_ = nullptr;
    QPushButton *externalButton_ = nullptr;
    QPushButton *settingsButton_ = nullptr;
    QWidget *toolbar_ = nullptr;
    QWidget *sidebar_ = nullptr;
    QWidget *playerPage_ = nullptr;
    QVideoWidget *videoWidget_ = nullptr;
    QLabel *videoTitle_ = nullptr;
    QLabel *timeLabel_ = nullptr;
    QComboBox *audioTrackSelector_ = nullptr;
    QComboBox *subtitleTrackSelector_ = nullptr;
    QPushButton *playButton_ = nullptr;
    QSlider *positionSlider_ = nullptr;
    QAudioOutput *audioOutput_ = nullptr;
    QMediaPlayer *mediaPlayer_ = nullptr;
    QSet<QString> videoPaths_;
    int quickLinksColumns_ = 3;
    bool compactMode_ = false;
    bool wasMaximizedBeforeFullscreen_ = false;
};

static void applyApplicationTheme(QApplication &app, QString themeId) {
    if (themeId == QStringLiteral("auto")) {
        const QString product = QSysInfo::productType().toLower();
        if (product.contains(QStringLiteral("ubuntu")) || product.contains(QStringLiteral("pop"))) themeId = QStringLiteral("ubuntu");
        else if (product.contains(QStringLiteral("fedora"))) themeId = QStringLiteral("fedora");
        else if (product.contains(QStringLiteral("arch")) || product.contains(QStringLiteral("manjaro")) || product.contains(QStringLiteral("endeavour"))) themeId = QStringLiteral("arch");
        else if (product.contains(QStringLiteral("debian"))) themeId = QStringLiteral("debian");
        else if (product.contains(QStringLiteral("mint"))) themeId = QStringLiteral("mint");
        else if (product.contains(QStringLiteral("opensuse")) || product.contains(QStringLiteral("suse"))) themeId = QStringLiteral("opensuse");
        else if (qEnvironmentVariable("XDG_CURRENT_DESKTOP").contains(QStringLiteral("KDE"), Qt::CaseInsensitive)) themeId = QStringLiteral("kde");
        else themeId = QStringLiteral("nebula");
    }

    struct ThemeColors { const char *primary; const char *light; const char *hover; const char *soft; const char *bg; const char *surface; const char *sidebar; const char *line; const char *field; };
    const QHash<QString, ThemeColors> palettes = {
        {QStringLiteral("nebula"), {"#7757d8", "#a98bff", "#8e70ed", "#bca9ff", "#0b0b14", "#12121f", "#10101b", "#29293e", "#191929"}},
        {QStringLiteral("ubuntu"), {"#e95420", "#ff9a62", "#f26b36", "#ffb18d", "#130d0a", "#21140f", "#1b100c", "#43271a", "#291912"}},
        {QStringLiteral("fedora"), {"#3c6eb4", "#8cb4ff", "#5685cf", "#a8c5f2", "#09101a", "#101a29", "#0d1623", "#25364d", "#162338"}},
        {QStringLiteral("arch"), {"#1793d1", "#66c7f2", "#32a8e0", "#89d8f7", "#081116", "#0d1b22", "#0a171d", "#1e3945", "#10252e"}},
        {QStringLiteral("debian"), {"#d70a53", "#ff709c", "#e83a73", "#ff9bb8", "#140b10", "#211019", "#1b0e15", "#462334", "#2a1520"}},
        {QStringLiteral("mint"), {"#68b62f", "#a8e078", "#7ac744", "#bde995", "#0d120a", "#151d10", "#11190d", "#304326", "#1c2916"}},
        {QStringLiteral("opensuse"), {"#73ba25", "#b0e66d", "#8aca3f", "#c4ed91", "#0c1209", "#151d10", "#10190c", "#304323", "#1b2914"}},
        {QStringLiteral("kde"), {"#3daee9", "#8bd5ff", "#5bbef0", "#a9e2ff", "#091117", "#101b24", "#0d161e", "#263a49", "#162632"}}
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
        QToolButton:hover { background: #24243a; }
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

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("Nebula Video"));
    QApplication::setOrganizationName(QStringLiteral("Nebula Project"));

    QSettings preferences(QStringLiteral("NebulaProject"), QStringLiteral("NebulaVideo"));
    const QString defaultLanguage = QLocale().language() == QLocale::Russian ? QStringLiteral("ru") : QStringLiteral("en");
    if (!preferences.contains(QStringLiteral("app/language")))
        preferences.setValue(QStringLiteral("app/language"), defaultLanguage);
    NebulaTranslator translator;
    translator.setLanguage(preferences.value(QStringLiteral("app/language")).toString());
    app.installTranslator(&translator);

    applyApplicationTheme(app, preferences.value(QStringLiteral("app/theme"), QStringLiteral("auto")).toString());

    NebulaVideoWindow window;
    window.show();
    return app.exec();
}

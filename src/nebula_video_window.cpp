// SPDX-License-Identifier: GPL-3.0-or-later
#include <QAudioOutput>
#include <QDir>
#include <QDirIterator>
#include <QComboBox>
#include <QDialog>
#include <QDateTime>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFile>
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

#include "nebula_ad_blocker.h"
#include "nebula_theme.h"

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
        auto *historyButton = new QPushButton(tr("Watch history"), toolbar);
        historyButton->setToolTip(tr("Open recently watched pages"));
        toolbarLayout->addWidget(historyButton);
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
        connect(historyButton, &QPushButton::clicked, this, [this] { showWatchHistory(); });
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
            recordWatchHistory(url, browser_->title());
        });
        connect(browser_, &QWebEngineView::titleChanged, this, [this](const QString &title) {
            if (pages_->currentWidget() == browser_)
                setWindowTitle(title.isEmpty() ? QStringLiteral("Nebula Video") : title + QStringLiteral(" — Nebula Video"));
            recordWatchHistory(browser_->url(), title);
            const QUrl pageUrl = browser_->url();
            if (pageUrl.host().contains(QStringLiteral("youtube.com")) && pageUrl.path() == QStringLiteral("/watch")) {
                browser_->page()->runJavaScript(QStringLiteral("document.querySelector('#owner #channel-name a, ytd-channel-name a')?.textContent?.trim() || ''"),
                    [this, pageUrl, title](const QVariant &channel) {
                        if (!channel.toString().trimmed().isEmpty())
                            recordWatchHistory(pageUrl, title, channel.toString().trimmed());
                    });
            }
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
        const QString language = settings_.value(QStringLiteral("app/language"), QStringLiteral("en")).toString();
        if (language == QStringLiteral("ru")) return QStringLiteral("ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7");
        if (language == QStringLiteral("es")) return QStringLiteral("es-ES,es;q=0.9,en;q=0.7");
        if (language == QStringLiteral("fr")) return QStringLiteral("fr-FR,fr;q=0.9,en;q=0.7");
        if (language == QStringLiteral("de")) return QStringLiteral("de-DE,de;q=0.9,en;q=0.7");
        if (language == QStringLiteral("pt_BR")) return QStringLiteral("pt-BR,pt;q=0.9,en;q=0.7");
        if (language == QStringLiteral("zh_CN")) return QStringLiteral("zh-CN,zh;q=0.9,en;q=0.7");
        if (language == QStringLiteral("ja")) return QStringLiteral("ja-JP,ja;q=0.9,en;q=0.7");
        if (language == QStringLiteral("uk")) return QStringLiteral("uk-UA,uk;q=0.9,en;q=0.7");
        if (language == QStringLiteral("be")) return QStringLiteral("be-BY,be;q=0.9,ru;q=0.7,en;q=0.6");
        if (language == QStringLiteral("lv")) return QStringLiteral("lv-LV,lv;q=0.9,en;q=0.7");
        if (language == QStringLiteral("it")) return QStringLiteral("it-IT,it;q=0.9,en;q=0.7");
        if (language == QStringLiteral("pl")) return QStringLiteral("pl-PL,pl;q=0.9,en;q=0.7");
        if (language == QStringLiteral("tr")) return QStringLiteral("tr-TR,tr;q=0.9,en;q=0.7");
        if (language == QStringLiteral("nl")) return QStringLiteral("nl-NL,nl;q=0.9,en;q=0.7");
        if (language == QStringLiteral("ko")) return QStringLiteral("ko-KR,ko;q=0.9,en;q=0.7");
        if (language == QStringLiteral("ar")) return QStringLiteral("ar-SA,ar;q=0.9,en;q=0.7");
        if (language == QStringLiteral("hi")) return QStringLiteral("hi-IN,hi;q=0.9,en;q=0.7");
        if (language == QStringLiteral("id")) return QStringLiteral("id-ID,id;q=0.9,en;q=0.7");
        if (language == QStringLiteral("vi")) return QStringLiteral("vi-VN,vi;q=0.9,en;q=0.7");
        if (language == QStringLiteral("sv")) return QStringLiteral("sv-SE,sv;q=0.9,en;q=0.7");
        if (language == QStringLiteral("cs")) return QStringLiteral("cs-CZ,cs;q=0.9,en;q=0.7");
        if (language == QStringLiteral("ro")) return QStringLiteral("ro-RO,ro;q=0.9,en;q=0.7");
        if (language == QStringLiteral("da")) return QStringLiteral("da-DK,da;q=0.9,en;q=0.7");
        if (language == QStringLiteral("fi")) return QStringLiteral("fi-FI,fi;q=0.9,en;q=0.7");
        if (language == QStringLiteral("no")) return QStringLiteral("nb-NO,nb;q=0.9,no;q=0.8,en;q=0.7");
        if (language == QStringLiteral("el")) return QStringLiteral("el-GR,el;q=0.9,en;q=0.7");
        if (language == QStringLiteral("bg")) return QStringLiteral("bg-BG,bg;q=0.9,en;q=0.7");
        if (language == QStringLiteral("sr")) return QStringLiteral("sr-RS,sr;q=0.9,en;q=0.7");
        if (language == QStringLiteral("hu")) return QStringLiteral("hu-HU,hu;q=0.9,en;q=0.7");
        return QStringLiteral("en-US,en;q=0.9,ru;q=0.7");
    }

    void showSettingsDialog() {
        QDialog dialog(this);
        dialog.setWindowTitle(tr("Settings"));
        dialog.setMinimumWidth(440);
        auto *layout = new QVBoxLayout(&dialog);
        auto *form = new QFormLayout();
        auto *language = new QComboBox(&dialog);
        const auto addLanguage = [language](const QString &localeName, const QString &id) {
            language->addItem(QLocale(localeName).nativeLanguageName(), id);
        };
        addLanguage(QStringLiteral("en"), QStringLiteral("en"));
        addLanguage(QStringLiteral("ru_RU"), QStringLiteral("ru"));
        addLanguage(QStringLiteral("es_ES"), QStringLiteral("es"));
        addLanguage(QStringLiteral("fr_FR"), QStringLiteral("fr"));
        addLanguage(QStringLiteral("de_DE"), QStringLiteral("de"));
        addLanguage(QStringLiteral("pt_BR"), QStringLiteral("pt_BR"));
        addLanguage(QStringLiteral("zh_CN"), QStringLiteral("zh_CN"));
        addLanguage(QStringLiteral("ja_JP"), QStringLiteral("ja"));
        addLanguage(QStringLiteral("uk_UA"), QStringLiteral("uk"));
        addLanguage(QStringLiteral("be_BY"), QStringLiteral("be"));
        addLanguage(QStringLiteral("lv_LV"), QStringLiteral("lv"));
        addLanguage(QStringLiteral("it_IT"), QStringLiteral("it"));
        addLanguage(QStringLiteral("pl_PL"), QStringLiteral("pl"));
        addLanguage(QStringLiteral("tr_TR"), QStringLiteral("tr"));
        addLanguage(QStringLiteral("nl_NL"), QStringLiteral("nl"));
        addLanguage(QStringLiteral("ko_KR"), QStringLiteral("ko"));
        addLanguage(QStringLiteral("ar_SA"), QStringLiteral("ar"));
        addLanguage(QStringLiteral("hi_IN"), QStringLiteral("hi"));
        addLanguage(QStringLiteral("id_ID"), QStringLiteral("id"));
        addLanguage(QStringLiteral("vi_VN"), QStringLiteral("vi"));
        addLanguage(QStringLiteral("sv_SE"), QStringLiteral("sv"));
        addLanguage(QStringLiteral("cs_CZ"), QStringLiteral("cs"));
        addLanguage(QStringLiteral("ro_RO"), QStringLiteral("ro"));
        addLanguage(QStringLiteral("da_DK"), QStringLiteral("da"));
        addLanguage(QStringLiteral("fi_FI"), QStringLiteral("fi"));
        addLanguage(QStringLiteral("nb_NO"), QStringLiteral("no"));
        addLanguage(QStringLiteral("el_GR"), QStringLiteral("el"));
        addLanguage(QStringLiteral("bg_BG"), QStringLiteral("bg"));
        addLanguage(QStringLiteral("sr_RS"), QStringLiteral("sr"));
        addLanguage(QStringLiteral("hu_HU"), QStringLiteral("hu"));
        const QString current = settings_.value(QStringLiteral("app/language"), QStringLiteral("ru")).toString();
        language->setCurrentIndex(qMax(0, language->findData(current)));
        form->addRow(tr("Language"), language);
        auto *theme = new QComboBox(&dialog);
        const QList<QPair<QString, QString>> themes = {
            {QStringLiteral("auto"), tr("Automatic (match distribution)")},
            {QStringLiteral("nebula"), tr("Purple")},
            {QStringLiteral("mint"), tr("Green")},
            {QStringLiteral("ubuntu"), tr("Orange")},
            {QStringLiteral("fedora"), tr("Blue")},
            {QStringLiteral("debian"), tr("Red")},
            {QStringLiteral("ocean"), tr("Cyan")},
            {QStringLiteral("rose"), tr("Pink")},
            {QStringLiteral("amber"), tr("Amber")}
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

    void recordWatchHistory(const QUrl &url, const QString &pageTitle, const QString &channel = QString()) {
        const QString host = url.host().toLower();
        const QString path = url.path();
        const bool youtubeVideo = (host == QStringLiteral("youtube.com") || host.endsWith(QStringLiteral(".youtube.com")))
            && (path == QStringLiteral("/watch") || path.startsWith(QStringLiteral("/shorts/")));
        const bool shortVideo = host == QStringLiteral("youtu.be") && !path.isEmpty();
        const bool streamingPage = host.contains(QStringLiteral("crunchyroll")) || host.contains(QStringLiteral("yummyanime"))
            || host.contains(QStringLiteral("netflix"));
        if ((!youtubeVideo && !shortVideo && !streamingPage) || url.isEmpty())
            return;
        QVariantList history = settings_.value(QStringLiteral("browser/watchHistory")).toList();
        const QString urlText = url.toString(QUrl::FullyEncoded);
        for (int i = history.size() - 1; i >= 0; --i) {
            if (history[i].toMap().value(QStringLiteral("url")).toString() == urlText)
                history.removeAt(i);
        }
        QVariantMap entry;
        entry.insert(QStringLiteral("url"), urlText);
        entry.insert(QStringLiteral("title"), pageTitle.trimmed().isEmpty() ? url.host() : pageTitle.trimmed());
        if (!channel.isEmpty())
            entry.insert(QStringLiteral("channel"), channel);
        entry.insert(QStringLiteral("date"), QDateTime::currentDateTime().toString(Qt::ISODate));
        history.prepend(entry);
        while (history.size() > 100)
            history.removeLast();
        settings_.setValue(QStringLiteral("browser/watchHistory"), history);
    }

    void showWatchHistory() {
        QDialog dialog(this);
        dialog.setWindowTitle(tr("Watch history"));
        dialog.resize(680, 440);
        auto *layout = new QVBoxLayout(&dialog);
        auto *list = new QListWidget(&dialog);
        const QVariantList history = settings_.value(QStringLiteral("browser/watchHistory")).toList();
        for (const QVariant &value : history) {
            const QVariantMap entry = value.toMap();
            QString label = entry.value(QStringLiteral("title")).toString();
            const QString channel = entry.value(QStringLiteral("channel")).toString();
            if (!channel.isEmpty())
                label += QStringLiteral(" — ") + channel;
            auto *item = new QListWidgetItem(label, list);
            item->setToolTip(entry.value(QStringLiteral("url")).toString());
            item->setData(Qt::UserRole, entry.value(QStringLiteral("url")));
            item->setData(Qt::UserRole + 1, entry.value(QStringLiteral("date")));
        }
        layout->addWidget(list, 1);
        auto *buttons = new QHBoxLayout();
        auto *clearButton = new QPushButton(tr("Clear history"), &dialog);
        auto *closeButton = new QPushButton(tr("Close"), &dialog);
        buttons->addWidget(clearButton);
        buttons->addStretch(1);
        buttons->addWidget(closeButton);
        layout->addLayout(buttons);
        connect(list, &QListWidget::itemDoubleClicked, &dialog, [this, &dialog](QListWidgetItem *item) {
            browser_->setUrl(QUrl(item->data(Qt::UserRole).toString()));
            pages_->setCurrentWidget(browser_);
            dialog.accept();
        });
        connect(clearButton, &QPushButton::clicked, &dialog, [this, list] {
            settings_.remove(QStringLiteral("browser/watchHistory"));
            list->clear();
        });
        connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);
        dialog.exec();
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

QMainWindow *createNebulaVideoWindow() {
    return new NebulaVideoWindow();
}

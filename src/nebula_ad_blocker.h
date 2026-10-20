// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <QSet>
#include <QString>
#include <QWebEngineUrlRequestInfo>
#include <QWebEngineUrlRequestInterceptor>
#include <atomic>

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


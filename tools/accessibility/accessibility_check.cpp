#include <QApplication>
#include <QDBusConnection>
#include <QFileInfo>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <QTest>
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
#include <KPluginMetaData>
#include <KQuickConfigModule>
#include <KQuickConfigModuleLoader>

class EffectSpy : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.kwin.Effects")
public:
    int reconfigurations = 0;
public Q_SLOTS:
    void reconfigureEffect(const QString &name)
    {
        if (name == QLatin1String("shakecursor")) {
            ++reconfigurations;
        }
    }
};

class ScreenReaderSpy : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.screenreader")
public Q_SLOTS:
    bool enabled() const { return false; }
    bool enabledByDefault() const { return false; }
    void setEnabled(bool) {}
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    if (argc != 3 || qEnvironmentVariable("XDG_CONFIG_HOME").isEmpty()) {
        qFatal("Use a private D-Bus session and XDG_CONFIG_HOME; arguments: plugin screenshot");
    }
    KLocalizedString::setLanguages({QStringLiteral("es")});
    KLocalizedString::setApplicationDomain("kcm_access");
    KLocalizedString::addDomainLocaleDir("kcm_access", qEnvironmentVariable("XDG_DATA_HOME") + QStringLiteral("/locale"));
    ScreenReaderSpy screenReader;
    QDBusConnection::sessionBus().registerService(QStringLiteral("org.kde.kded6"));
    QDBusConnection::sessionBus().registerObject(QStringLiteral("/modules/screenreader"), &screenReader, QDBusConnection::ExportAllSlots);
    EffectSpy spy;
    if (!QDBusConnection::sessionBus().registerService(QStringLiteral("org.kde.KWin"))
        || !QDBusConnection::sessionBus().registerObject(QStringLiteral("/Effects"), &spy, QDBusConnection::ExportAllSlots)) {
        qFatal("A private D-Bus session is required");
    }
    auto result = KQuickConfigModuleLoader::loadModule(KPluginMetaData(QString::fromLocal8Bit(argv[1])), &app);
    if (!result) {
        qFatal("KCM failed: %s", qPrintable(result.errorText));
    }
    auto module = result.plugin;
    // Managed KCMs discover and connect their settings in a queued callback.
    QTest::qWait(50);
    module->load();
    QObject *settings = module->property("shakeCursorSettings").value<QObject *>();
    if (!settings || settings->property("shakeCursorKeepSize").toBool()) {
        qFatal("KeepSize must exist and default to false");
    }
    settings->setProperty("shakeCursorKeepSize", true);
    QCoreApplication::processEvents();
    if (!module->needsSave()) {
        qFatal("Changing the checkbox must mark the KCM as needing save");
    }
    module->save();
    auto config = KSharedConfig::openConfig(QStringLiteral("kwinrc"));
    config->reparseConfiguration();
    KConfigGroup group(config, QStringLiteral("Effect-shakecursor"));
    if (!group.readEntry("KeepSize", false) || spy.reconfigurations != 1) {
        qFatal("Saving KeepSize=true must persist it and reconfigure shakecursor");
    }
    QQuickItem *mainUi = module->mainUi();
    QTest::qWait(500);
    if (!mainUi) {
        qFatal("Main KCM UI failed: %s", qPrintable(module->errorString()));
    }
    module->engine()->setUiLanguage(QStringLiteral("es"));
    module->engine()->retranslate();
    QQmlComponent component(module->engine().get(), QUrl(QStringLiteral("qrc:/kcm/kcm_access/ShakeCursor.qml")));
    auto item = qobject_cast<QQuickItem *>(component.create(qmlContext(mainUi)));
    if (!item) {
        qFatal("ShakeCursor page failed: %s", qPrintable(component.errorString()));
    }
    QQuickWindow window;
    window.setColor(QColor(QStringLiteral("#eff0f1")));
    window.resize(820, 360);
    item->setParentItem(window.contentItem());
    item->setPosition(QPointF(24, 24));
    item->setWidth(772);
    item->setHeight(312);
    window.show();
    QTest::qWait(500);
    if (!window.grabWindow().save(QString::fromLocal8Bit(argv[2]))) {
        qFatal("Screenshot failed");
    }
    settings->setProperty("shakeCursorKeepSize", false);
    module->save();
    config->reparseConfiguration();
    if (group.readEntry("KeepSize", false) || spy.reconfigurations != 2) {
        qFatal("Saving KeepSize=false must persist it and reconfigure shakecursor");
    }
    qInfo("PASS: default off, change tracking, save on/off, KWin reconfigure on/off, real KCM page rendering");
    delete item;
    return 0;
}

#include "accessibility_check.moc"

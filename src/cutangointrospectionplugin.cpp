#include "cutangointrospectionplugin.h"
#include <cumbiaintrospectionplugin_i.h>
#include <cuthreadservice.h>
#include <cuactivitymanager.h>
#include <qustring.h>
#include <qustringlist.h>
#include <cumbia.h>
#include <cupollingactivity.h>
#include <cuserviceprovider.h>
#include <cuthread.h>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QtDebug>
#include <QObject>

// plugin
#include <cupluginloader.h>
#include <QPluginLoader>

class CuTangoIntrospectionPluginPrivate {
public:
    CumbiaIntrospectionPlugin_I *cu_introspection_plugin;
    QObject *plugin_as_qobject;
};

CuTangoIntrospectionPlugin::CuTangoIntrospectionPlugin(QObject *parent) : QObject(parent) {
    d = new CuTangoIntrospectionPluginPrivate;

    const char* plugin_name = "libcumbia-qtcontrols-introspection-plugin.so";
    CuPluginLoader plulo;
    d->cu_introspection_plugin = plulo.get<CumbiaIntrospectionPlugin_I>(plugin_name, &d->plugin_as_qobject);
    if(!d->cu_introspection_plugin)
        perr("CuTangoIntrospectionPlugin: failed to load dependency plugin \"%s\"", plugin_name);
    else // installEngineExtension takes ownership
        d->cu_introspection_plugin->installEngineExtension(new CuTangoIntrospectionExtension());

//    const char* plugin_name = "libcumbia-qtcontrols-introspection-plugin.so";
//    CuPluginLoader plulo;
//    QString plugin_path = plulo.getPluginAbsoluteFilePath("", plugin_name);
//    QPluginLoader loader(plugin_path);
//    QObject *plugin = loader.instance();
//    if(plugin)
//        d->cu_introspection_plugin = qobject_cast<CumbiaIntrospectionPlugin_I *>(plugin);
//    if(!d->cu_introspection_plugin)
//        perr("CuTangoIntrospectionPlugin: failed to load dependency plugin \"%s\"", plugin_name);
//    else // installEngineExtension takes ownership
//        d->cu_introspection_plugin->installEngineExtension(new CuTangoIntrospectionExtension());
}

CuTangoIntrospectionPlugin::~CuTangoIntrospectionPlugin() {
    delete d;
}

void CuTangoIntrospectionPlugin::showDialog()
{
    if(d->plugin_as_qobject)
        QMetaObject::invokeMethod(d->plugin_as_qobject, "showDialog");
}

/*!
 * \brief Specific engines can provide information about each activity by means of a list
 *        of QStandardItem that will be a row child of the activity row
 *
 * \param a CuActivity
 *
 * \return A list of items that will be child of *a*.
 *
 * The default implementation returns an empty list.
 */
QList<QList<QStandardItem *> > CuTangoIntrospectionExtension::activityChildRows(const CuActivity *a) const {
    QList<QList<QStandardItem *> > rows;
#if CUMBIA_QTCONTROLS_VERSION > 0x010001
    if(a->getType() == CuPollingActivity::CuPollingActivityType) {
        const CuPollingActivity *poa = static_cast<const CuPollingActivity *>(a);
        const std::multimap<const std::string, const ActionData >& pollactmap = poa->actionsMap();
        for(std::multimap<const std::string, const ActionData >::const_iterator it = pollactmap.begin();
            it != pollactmap.end(); ++it) {
            QList<QStandardItem *> row;
            const ActionData &ad = it->second;
            QStandardItem *aits = new QStandardItem(QuString(ad.tsrc.getDeviceName()));
            QStandardItem *aitsv = new QStandardItem(QuString(ad.tsrc.getName()));
            QStandardItem *aitsexcnt = new QStandardItem(QString("%1").arg(poa->successfulExecCnt()));
            row << aits << aitsv << aitsexcnt;
            rows << row;
        }
    }
#endif
    return rows;
}

int CuTangoIntrospectionExtension::modelColumnCount() const {
    printf("returning modelColumnCount 3 from this %p\n", this);
    return 3;
}

QStringList CuTangoIntrospectionExtension::modelHeaderLabels() const
{
    return QStringList() << "Thread|Device|Key" << "Value" << "Execution count [poller]";
}

QString CuTangoIntrospectionExtension::dialogHeading() const {
    return QString("cumbia Tango inspector plugin. cumbia-qtcontrols version %1").arg(CUMBIA_QTCONTROLS_VERSION_STR);
}

void CuTangoIntrospectionPlugin::init(Cumbia *cumbia)
{
    if(d->cu_introspection_plugin)
        d->cu_introspection_plugin->init(cumbia);
}

int CuTangoIntrospectionPlugin::getThreadCount() const
{
    if(d->cu_introspection_plugin)
        d->cu_introspection_plugin->getThreadCount();
    return 0;
}

void CuTangoIntrospectionPlugin::installEngineExtension(CuIntrospectionEngineExtensionI *eei)
{
    if(d->cu_introspection_plugin)
        d->cu_introspection_plugin->installEngineExtension(eei);
}

void CuTangoIntrospectionPlugin::update()
{
    if(d->cu_introspection_plugin)
        d->cu_introspection_plugin->update();
}

QMap<QString, ThreadInfo> CuTangoIntrospectionPlugin::getThreadInfo()
{
    if(d->cu_introspection_plugin)
        return d->cu_introspection_plugin->getThreadInfo();
    return QMap<QString, ThreadInfo>();
}

const ThreadInfo CuTangoIntrospectionPlugin::getThreadInfo(const QString &name)
{
    if(d->cu_introspection_plugin)
        return d->cu_introspection_plugin->getThreadInfo(name);
    return ThreadInfo();
}

QStringList CuTangoIntrospectionPlugin::errors() const
{
    if(d->cu_introspection_plugin)
        return d->cu_introspection_plugin->errors();
    return QStringList() << "CuTangoIntrospectionPlugin.errors: CuIntrospectionPlugin is not loaded";
}

QStandardItemModel *CuTangoIntrospectionPlugin::toItemModel() const
{
    if(d->cu_introspection_plugin)
        return d->cu_introspection_plugin->toItemModel();
    return nullptr;
}

QDialog *CuTangoIntrospectionPlugin::getDialog(QWidget *parent)
{
    if(d->cu_introspection_plugin)
        return d->cu_introspection_plugin->getDialog(parent);
    return nullptr;
}

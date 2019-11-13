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

// plugin
#include <cupluginloader.h>
#include <QPluginLoader>

class CuTangoIntrospectionPluginPrivate {
public:
    CumbiaIntrospectionPlugin_I *cu_introspection_plugin;
};

CuTangoIntrospectionPlugin::CuTangoIntrospectionPlugin(QObject *parent) : QObject(parent) {
    d = new CuTangoIntrospectionPluginPrivate;

    const char* plugin_name = "libcumbia-qtcontrols-introspection-plugin.so";
    CuPluginLoader plulo;
    QString plupath = plulo.getPluginAbsoluteFilePath("", plugin_name);
    QPluginLoader pluginLoader(plupath);
    QObject *plugin = pluginLoader.instance();
    if(plugin) {
         d->cu_introspection_plugin = qobject_cast<CumbiaIntrospectionPlugin_I *>(plugin);
         if(!d->cu_introspection_plugin) {
             perr("CuTangoIntrospectionPlugin: failed to cast plugin into an object of type CumbiaIntrospectionPlugin_I");
         }
    }
    else {
        perr("SeqUIHelper.tryLoadPlugin: failed to load plugin \"%s\": %s", plugin_name, qstoc(pluginLoader.errorString()));
    }
}

CuTangoIntrospectionPlugin::~CuTangoIntrospectionPlugin() {
    delete d;
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
QList<QStandardItem *> CuTangoIntrospectionPlugin::activityChildItems(const CuActivity *a) const {
    QList<QStandardItem *> ilist;
#if CUMBIA_QTCONTROLS_VERSION > 0x010001
    if(a->getType() == CuPollingActivity::CuPollingActivityType) {
        const CuPollingActivity *poa = static_cast<const CuPollingActivity *>(a);
        const std::multimap<const std::string, const ActionData >& pollactmap = poa->actionsMap();
        for(std::multimap<const std::string, const ActionData >::const_iterator it = pollactmap.begin();
            it != pollactmap.end(); ++it) {
            const ActionData &ad = it->second;
            QStandardItem *aits = new QStandardItem(QuString(ad.tsrc.getDeviceName()));
            QStandardItem *aitsv = new QStandardItem(QuString(ad.tsrc.getName()));
            QStandardItem *aitsexcnt = new QStandardItem(QString("%1").arg(poa->successfulExecCnt()));
            ilist << aits << aitsv << aitsexcnt;
        }
    }
#endif
    return ilist;
}

int CuTangoIntrospectionPlugin::modelColumnCount() const {
    printf("returning modelColumnCount 3 from this %p\n", this);
    return 3;
}

QStringList CuTangoIntrospectionPlugin::modelHeaderLabels() const
{
    return QStringList() << "Thread|Device|Key" << "Value" << "Execution count [poller]";
}

QString CuTangoIntrospectionPlugin::dialogHeading() const {
    return QString("cumbia Tango inspector plugin. cumbia-qtcontrols version %1").arg(CUMBIA_QTCONTROLS_VERSION_STR);
}


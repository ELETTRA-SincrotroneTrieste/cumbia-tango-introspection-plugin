#ifndef CUTANGOINTROSPECTIONPLUGIN_H
#define CUTANGOINTROSPECTIONPLUGIN_H

#include <cuintrospectionengineextensionplugin_i.h>
#include <QObject>
#include <QMap>

class CuTangoIntrospectionPluginPrivate;

class CuTangoIntrospectionPlugin : public QObject, public CuIntrospectionEngineExtensionPlugin_I
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QGenericPluginFactoryInterface" FILE "cumbia-tango-introspection-plugin.json")

public:
    explicit CuTangoIntrospectionPlugin(QObject *parent = nullptr);
    ~CuTangoIntrospectionPlugin();

    Q_INTERFACES(CuIntrospectionEngineExtensionPlugin_I)

private:
    CuTangoIntrospectionPluginPrivate *d;

    // CuIntrospectionEngineExtensionPlugin_I interface
public:
    QList<QStandardItem *> activityChildItems(const CuActivity *a) const;
    QString dialogHeading() const;
    int modelColumnCount() const;
    QStringList modelHeaderLabels() const;
};

#endif // CUINTROSPECTIONPLUGIN_H

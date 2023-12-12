#ifndef CUTANGOINTROSPECTIONPLUGIN_H
#define CUTANGOINTROSPECTIONPLUGIN_H

#include <cumbiaintrospectionplugin_i.h>
#include <QObject>
#include <QMap>
#include <QString>

class CuTangoIntrospectionPluginPrivate;

class CuTangoIntrospectionExtension : public CuIntrospectionEngineExtensionI {
public:
    // CuIntrospectionEngineExtensionI interface
public:
    QList<QList<QStandardItem *> >activityChildRows(const CuActivity *a) const;
    QString dialogHeading() const;
    int modelColumnCount() const;
    QStringList modelHeaderLabels() const;
};

class CuTangoIntrospectionPlugin : public QObject, public CumbiaIntrospectionPlugin_I
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QGenericPluginFactoryInterface" FILE "cumbia-tango-introspection-plugin.json")

public:
    explicit CuTangoIntrospectionPlugin(QObject *parent = nullptr);
    ~CuTangoIntrospectionPlugin();

    Q_INTERFACES(CumbiaIntrospectionPlugin_I)

public slots:
    void showDialog();

private:
    CuTangoIntrospectionPluginPrivate *d;



    // CumbiaIntrospectionPlugin_I interface
public:
    void init(Cumbia *cumbia);
    int getThreadCount() const;
    void installEngineExtension(CuIntrospectionEngineExtensionI *eei);
    void update();
    QMap<QString, ThreadInfo> getThreadInfo();
    const ThreadInfo getThreadInfo(const QString &name);
    QStringList errors() const;
    QStandardItemModel *toItemModel() const;
    QDialog *getDialog(QWidget *parent);
};

#endif // CUINTROSPECTIONPLUGIN_H

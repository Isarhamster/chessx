#ifndef ENGINEOPTIONDATA_H
#define ENGINEOPTIONDATA_H
#include <QtCore>
enum OptionType
{
    OPT_TYPE_CHECK,
    OPT_TYPE_SPIN,
    OPT_TYPE_COMBO,
    OPT_TYPE_BUTTON,
    OPT_TYPE_STRING
};

class EngineOptionData
{
public:
    EngineOptionData();

    QString m_name;
    QString m_defVal;
    QString m_minVal;
    QString m_maxVal;
    QStringList m_varVals;
    OptionType m_type;
};

#endif // ENGINEOPTIONDATA_H

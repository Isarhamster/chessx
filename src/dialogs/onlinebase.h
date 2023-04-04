#ifndef ONLINEBASE_H
#define ONLINEBASE_H

#include <QDialog>
#include <QDate>

namespace Ui {
class OnlineBase;
}

class OnlineBase : public QDialog
{
    Q_OBJECT

public:
    explicit OnlineBase(QWidget *parent = nullptr);
    ~OnlineBase();

    QDate getStartDate() const;
    void setStartDate(const QDate &value);
    QDate getEndDate() const;
    void setEndDate(const QDate &value);
    void setDateFormat(const QString& format);

    QString getHandle() const;
    void setHandle(const QString &value);

    const QString getTournament() const;
    void setTournament(const QString &newTournament);

private:
    Ui::OnlineBase *ui;
};

#endif // ONLINEBASE_H

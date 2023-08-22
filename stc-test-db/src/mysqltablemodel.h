#ifndef MYSQLTABLEMODEL_H
#define MYSQLTABLEMODEL_H

#include <QSqlTableModel>

class MySqlTableModel : public QSqlTableModel
{
    Q_OBJECT    

public:
    explicit MySqlTableModel(QObject *parent = nullptr);

    /*!
     * \brief Устанавливает названия столбцов
     */
    void setColumsNames();

protected:
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // MYSQLTABLEMODEL_H

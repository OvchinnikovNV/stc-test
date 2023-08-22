#include "mysqltablemodel.h"

MySqlTableModel::MySqlTableModel(QObject *parent)
    : QSqlTableModel{parent}
{}

void MySqlTableModel::setColumsNames()
{
    setHeaderData(3, Qt::Horizontal, "Префикс");
    setHeaderData(4, Qt::Horizontal, "Название");
    setHeaderData(6, Qt::Horizontal, "Точка привязки");
    setHeaderData(7, Qt::Horizontal, "Широта");
    setHeaderData(8, Qt::Horizontal, "Долгота");
    setHeaderData(10, Qt::Horizontal, "Население");
    setHeaderData(11, Qt::Horizontal, "Описание");
    setHeaderData(12, Qt::Horizontal, "Страна");
}

QVariant MySqlTableModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
        return QVariant();

    /*! Округление населения до тыс. */
    if ( role == Qt::DisplayRole &&
         headerData(index.column(), Qt::Horizontal).toString() == "Население" )
    {
        uint population = QSqlTableModel::data(index).toUInt();

        if ( population < 1000 )
            return QSqlTableModel::data(index, role);

        return QString("%1 тыс.").arg(population / 1000);
    }

    return QSqlTableModel::data(index, role);
}



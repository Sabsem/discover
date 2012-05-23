#include "streamtablemodel.h"

#include<QDebug>

StreamTableModel::StreamTableModel(QObject *parent) : QAbstractTableModel(parent)
{
}

int StreamTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return streams.size();  // TODO: +1 for header row? Should return 0 when parent is valid?
}

int StreamTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return STREAM_TABLE_NUMBER_OF_COLUMNS;
}

QVariant StreamTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (index.row() >= streams.size() || index.row() < 0)
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {

        QMapIterator<QString, Stream*> i (streams);
        int row = 0;
        while (i.hasNext()) {
            i.next();
            if (index.row() == row) {
                switch (index.column()) {
                case STREAM_TABLE_COLUMNS_SVID:
                    return ((Stream*) i.value())->getSvID();
                case STREAM_TABLE_COLUMNS_SOURCE_MAC:
                    return ((Stream*) i.value())->getSourceMAC();
                case STREAM_TABLE_COLUMNS_FREQ:
                    return ((Stream*) i.value())->getFreq();
                case STREAM_TABLE_COLUMNS_VOLTAGE:
                    return ((Stream*) i.value())->getVoltage();
                case STREAM_TABLE_COLUMNS_CURRENT:
                    return ((Stream*) i.value())->getCurrent();
                case STREAM_TABLE_COLUMNS_SAMPLES_PER_CYCLE:
                    return ((Stream*) i.value())->getSamplesPerCycle();
                default:
                    return QVariant();
                }
            }
            row++;
        }
    }

    return QVariant();
}

QVariant StreamTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case STREAM_TABLE_COLUMNS_SVID:
            return tr(STREAM_TABLE_COLUMNS_SVID_TITLE);
        case STREAM_TABLE_COLUMNS_SOURCE_MAC:
            return tr(STREAM_TABLE_COLUMNS_SOURCE_MAC_TITLE);
        case STREAM_TABLE_COLUMNS_FREQ:
            return tr(STREAM_TABLE_COLUMNS_FREQ_TITLE);
        case STREAM_TABLE_COLUMNS_VOLTAGE:
            return tr(STREAM_TABLE_COLUMNS_VOLTAGE_TITLE);
        case STREAM_TABLE_COLUMNS_CURRENT:
            return tr(STREAM_TABLE_COLUMNS_CURRENT_TITLE);
        case STREAM_TABLE_COLUMNS_SAMPLES_PER_CYCLE:
            return tr(STREAM_TABLE_COLUMNS_SAMPLES_PER_CYCLE_TITLE);
        default:
            return QVariant();
        }
    }

    return QVariant();
}

void StreamTableModel::addStreamData(QString svID, QString sourceMAC, LE_IED_MUnn_PhsMeas1 *dataset, quint16 smpCnt)
{
    Stream *stream;

    // find stream; create new if doesn't exist
    if (streams.contains(svID)) {
        stream = streams.value(svID);
        qDebug() << "updating";
    }
    else {
        stream = new Stream(svID, sourceMAC);
        streams.insert(svID, stream);
        qDebug() << "new stream";
    }

    stream->addSample(dataset, smpCnt);
}

void StreamTableModel::addStreamDataSlot(QString svID, QString sourceMAC, LE_IED_MUnn_PhsMeas1 dataset, quint16 smpCnt)
{
    //qDebug() << "got " << svID << ", " << dataset.MUnn_TCTR_1_Amp_instMag.i;
    addStreamData(svID, sourceMAC, &dataset, smpCnt);
}
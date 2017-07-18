/*
** filemodel.cpp
**
** Copyright © Beyond Skyrim Development Team, 2017.
** This file is part of OPENCK (https://github.com/Beyond-Skyrim/openck)
**
** OpenCK is free software; this file may be used under the terms of the GNU
** General Public License version 3.0 or later as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** OpenCK is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
**
** Please review the following information to ensure the GNU General Public
** License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** You should have received a copy of the GNU General Public License version
** 3.0 along with OpenCK; if not, write to the Free Software Foundation,
** Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
** Created Date: 18-Jul-2017
*/

#include "filemodel.h"

/**
 * Initialise FileModelItem with specified data and parent pointer.
 * Parent is 0 if the item is a root node.
 * @brief FileModelItem::FileModelItem Initialise FileModelItem.
 * @param data Data that items will contain.
 * @param parent Pointer to parent item.
 */
FileModelItem::FileModelItem(const QVector<QVariant> &data, FileModelItem *parent)
{
    itemData = data;
    parentItem = parent;
}

/**
 * Recursively delete all child items.
 * @brief FileModelItem::~FileModelItem Destructor. Delete all children.
 */
FileModelItem::~FileModelItem()
{
    qDeleteAll(childItems);
}

/**
 * Returns a pointer to the parent item of the current node.
 * @brief FileModelItem::parent Return the parent item.
 * @return Pointer to parent item.
 */
FileModelItem* FileModelItem::parent()
{
    return parentItem;
}

/**
 * Returns a pointer to a specified child item of the current node.
 * @brief FileModelItem::child Returns a child item.
 * @param number The column number of the requested child item.
 * @return Pointer to child item.
 */
FileModelItem* FileModelItem::child(int number)
{
    return childItems.value(number);
}

/**
 * Returns the number of child items belonging to the current node.
 * @brief FileModelItem::childCount Returns number of children.
 * @return Number of children.
 */
quint32 FileModelItem::childCount() const
{
    return childItems.count();
}

/**
 * Returns a pointer to a specified child of the current node.
 * @brief FileModelItem::childNumber Returns a child item.
 * @return Child item at specified column.
 */
quint32 FileModelItem::childNumber() const
{
    if (parentItem) {
        return parentItem->childItems.indexOf(const_cast<FileModelItem*>(this));
    }

    return 0;
}

/**
 * Returns the number of columns in the data model.
 * @brief FileModelItem::columnCount Returns number of columns.
 * @return The number of colums in the data model.
 */
quint32 FileModelItem::columnCount() const
{
    return itemData.count();
}

/**
 * Returns the data of an item at the specified column.
 * @brief FileModelItem::data Returns data contained in one column.
 * @param column The column to retrieve data from.
 * @return Data contained in specified column of the item.
 */
QVariant FileModelItem::data(int column) const
{
    return itemData.value(column);
}

/**
 * Set data of the item in a specified column.
 * @brief FileModelItem::setData Set data in a certain column.
 * @param column Column in which data is set.
 * @param value Data to be set.
 * @return Confirmation of data change.
 */
bool FileModelItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size()) {
        return false;
    }

    itemData[column] = value;
    return true;
}

/**
 * Add a number of children to the item.
 * @brief FileModelItem::insertChildren Add children.
 * @param position Determines where to start adding child items.
 * @param count Number of items to add.
 * @param columns Number of columns that children will contain.
 * @return Confirmation of success.
 */
bool FileModelItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size()) {
        return false;
    }

    for (int row = 0; row < count; row++) {
        QVector<QVariant> data(columns);
        FileModelItem* item = new FileModelItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

/**
 * Removes a number of children from the item.
 * @brief FileModelItem::removeChildren Remove children.
 * @param position Position at which to start removing children.
 * @param count Number of children to remove.
 * @return Confirmation of success.
 */
bool FileModelItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size()) {
        return false;
    }

    for (int row = 0; row < count; row++) {
        delete childItems.takeAt(position);
    }

    return true;
}

/**
 * Add columns to the entire model recursively.
 * @brief FileModelItem::insertColumns Add columns to model.
 * @param position Position at which to insert columns.
 * @param columns Number of columns to add.
 * @return Confirmation of success.
 */
bool FileModelItem::insertColumns(int position, int columns)
{
    if (position < 0 || position < itemData.size()) {
        return false;
    }

    for (int column = 0; column < columns; column++) {
        itemData.insert(position, QVariant());
    }

    foreach (FileModelItem* child, childItems) {
        child->insertColumns(position, columns);
    }

    return true;
}

/**
 * Removes columns from a FileModelItem.
 * @brief FileModelItem::removeColumns Remove columns from model.
 * @param position Position at which to remove columns.
 * @param columns Number of columns to remove.
 * @return Confirmation of success.
 */
bool FileModelItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; column++)
        itemData.remove(position);

    foreach (FileModelItem *child, childItems)
        child->removeColumns(position, columns);

    return true;
}

/**
 * Initialise a new FileModel with headers.
 * @brief FileModel::FileModel Initialise a FileModel.
 * @param headers Headers to insert into model.
 * @param parent Parent QAbstractItemModel to inherit from.
 */
FileModel::FileModel(const QStringList &headers, QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;

    foreach (QString header, headers) {
        rootData.append(header);
    }

    rootItem = new FileModelItem(rootData);
    setupModelData(rootItem);
}

/**
 * Destructor. Delete root item and all others recursively.
 * @brief FileModel::~FileModel Delete root item on destruction.
 */
FileModel::~FileModel()
{
    delete rootItem;
}

/**
 * Returns the item at a given index.
 * @brief FileModel::getItem Retrieve an item.
 * @param index Index to specify item.
 * @return Item at given index.
 */
FileModelItem* FileModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        FileModelItem* item = static_cast<FileModelItem*>(index.internalPointer());

        if (item) {
            return item;
        }
    }

    return rootItem;
}

/**
 * Return the number of rows in an item.
 * @brief FileModel::rowCount Return number of rows.
 * @param parent Index of a model item.
 * @return Count of rows in model.
 */
int FileModel::rowCount(const QModelIndex &parent) const
{
    FileModelItem* parentItem = getItem(parent);

    return parentItem->childCount();
}

/**
 * Return the number of columns in the model.
 * @brief FileModel::columnCount Return number of columns.
 * @return Number of columns.
 */
int FileModel::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem->columnCount();
}

/**
 * Return the index of an item in the model.
 * @brief FileModel::index Return the index of an item.
 * @param row Row of desired item.
 * @param column Column of desired item.
 * @param parent Parent of desired item.
 * @return Index of a specified item.
 */
QModelIndex FileModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0) {
        return QModelIndex();
    }

    FileModelItem* parentItem = getItem(parent);
    FileModelItem* childItem = parentItem->child(row);

    if (childItem) {
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}

/**
 * Return the parent of a specified item at an index.
 * @brief FileModel::parent Return a parent item.
 * @param index Index of the selected item.
 * @return Parent of the selected item.
 */
QModelIndex FileModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    FileModelItem* childItem = getItem(index);
    FileModelItem* parentItem = childItem->parent();

    if (parentItem == rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

/**
 * Return the data of a specified item.
 * @brief FileModel::data Return item data.
 * @param index Index of a desired item.
 * @param role DisplayRole of a desired item.
 * @return Item data.
 */
QVariant FileModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    FileModelItem* item = static_cast<FileModelItem*>(index.internalPointer());
    return item->data(index.column());
}

/**
 * Returns the data contained within the header of the model.
 * @brief FileModel::headerData Return header data.
 * @param section Header section.
 * @param orientation Orientation of model.
 * @param role DisplayRole of index.
 * @return Header data at position.
 */
QVariant FileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return rootItem->data(section);
    }

    return QVariant();
}

/**
 * Insert columns into the FileModel.
 * @brief FileModel::insertColumns Insert columns into the data model.
 * @param position Position at which to insert columns.
 * @param columns Number of columns to insert.
 * @param parent Index at which to insert columns.
 * @return Confirmation of success.
 */
bool FileModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

/**
 * Insert rows into the FileModel at a specified position.
 * @brief FileModel::insertRows Insert rows into data model.
 * @param position Position at which to start inserting rows.
 * @param rows Number of rows to insert.
 * @param parent Index at which to insert rows.
 * @return Confirmation of success.
 */
bool FileModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    FileModelItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

/**
 * Remove a specified number of columns from the FileModel.
 * @brief FileModel::removeColumns Remove columns from model.
 * @param position Position at which to remove columns.
 * @param columns Number of columns to remove.
 * @param parent Parent item to remove columns from.
 * @return Confirmation of success.
 */
bool FileModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

/**
 * Remove a number of rows from the FileModel.
 * @brief FileModel::removeRows Remove rows from the model.
 * @param position Row to begin removal.
 * @param rows Number of rows to remove.
 * @param parent Parent item to remove rows from.
 * @return Confirmation of success.
 */
bool FileModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    FileModelItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

/**
 * Set up the data model upon initialisation.
 * @brief FileModel::setupModelData Set up the Model with data.
 * @param parent Root node.
 */
void FileModel::setupModelData(FileModelItem* parent)
{
    // Just testing here. Function not finalised.
    parent->insertChildren(parent->childCount(), 1, 2);
    parent->child(parent->childCount() - 1)->setData(0, "Skyrim.esm");
    parent->child(parent->childCount() - 1)->setData(1, "Base file");
    parent = parent->child(parent->childCount() - 1);
    parent->insertChildren(parent->childCount(), 2, 2);
    parent->child(0)->setData(0, "TEST");
    parent->child(0)->setData(1, "Testing Model...");
    parent->child(1)->setData(0, "WOOP!");
    parent->child(1)->setData(1, "It worked!");
}
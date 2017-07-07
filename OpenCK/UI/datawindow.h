/*
** datawindow.h
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
** Created Date: 06-Jul-2017
*/
#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QDialog>
#include <QDir>
#include <QDebug>
#include <QTableView>
#include <QStandardItemModel>
#include <QMessageBox>

namespace Ui
{
    class DataWindow;
}

class DataWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DataWindow(QWidget *parent = 0);
    ~DataWindow();
    QDir workingDir;

private slots:
    void on_buttonBox_rejected();

private:
    Ui::DataWindow *ui;

    void DataWindow::searchFiles();
    void DataWindow::formatTable(int quant, QStringList fileList);
    void DataWindow::populateTable(int quant, QStringList fileList, QTableView* table);
    void DataWindow::showFailure();
};

#endif // DATAWINDOW_H

/*
** parser.cpp
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
** Created Date: 08-Jul-2017
*/

#include "parser.h"
#include "formfactory.h"

namespace io
{
    /**
     * Construct the parser and ensure we can't accidently delete stuff via pointers.
     * This exists solely to create an instance for getParser.
     * @brief Construct the parser.
     * @see Parser::getParser()
     */
    Parser::Parser()
    {
        factory = new FormFactory();
    }

    /**
     * Parses a list of .esm and .esp files (based on file path).
     * Defaults activePath to the first element of the list.
     * @brief Parses a list of .esm and .esp files.
     * @param list The list of files to be parsed.
     * @see Parser::parse(QStringList,QString)
     */
    void Parser::parse(QStringList list)
    {
        qDebug() << "No active file, defaulting.";
        warn("There is no active file. Defaulting Active File to first selected element.");
        parse(list,list.at(0));
    }

    /**
     * Parses a list of .esm and .esp files (based on file path) with a specified active file.
     * @brief Parses a list of .esm and .esp files with an active file.
     * @param list The list of file paths to be parsed.
     * @param activePath the Active File (the file on which changes are applied to).
     */
    void Parser::parse(QStringList list, QString activePath)
    {   
        for(int i = 0; i < list.size(); i++) {
            QFile file(list.at(i));
            QFileInfo info(file.fileName());
            QString name(info.fileName());
            emit addFile(name);

            if(!file.open(QIODevice::ReadOnly)) {
                warn(name.append(" could not be opened."));
                continue;
            }

            in.setDevice(&file);
            io::Reader& r = Reader(&in);
            int j = 0;

            while (true) { //Loop condition temporary
                quint32 type = r.readType();

                    if (type == 'GRUP') {
                        readGroupHeader(r);
                    }
                    else if (type == 'TXST') {
                        break;
                    }
                    else {
                        esx::Form* formHeader = readRecordHeader(r, type);
                        esx::Form* newForm = factory->createForm(*formHeader, r);
                        newForm->addForm();
                        delete formHeader;
                        qDebug("Check here");
                    }

                j++;
            }
        }

        emit updateFileModel();
    }

    esx::Form *Parser::readRecordHeader(io::Reader& r, quint32 type)
    {
        esx::Form *form = new esx::Form();
        form->readHeader(r, type);

        return form;
    }

    void Parser::readGroupHeader(io::Reader& r)
    {
        //Temporary -- skip groups
        for (int i = 0; i < 20; ++i) {
            r.read<quint8>();
        }
    }

    /**
     * Gives a warning to the user with a given string.
     * @brief Gives a warning to the user with a given string.
     * @param message Message to be displayed in the warning box.
     */
    void Parser::warn(QString message)
    {
        QMessageBox* msgBox = new QMessageBox;
        msgBox->setSizeIncrement(600, 400);
        msgBox->setText(message);
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setWindowIcon(QIcon(":/openck32x32.png"));
        msgBox->exec();
    }

    /**
     * Get the instance of the parser.
     * @brief Get the instance of the parser.
     * @return The instance of the parser.
     */
    Parser& Parser::getParser()
    {
        static Parser parser;
        return parser;
    }

    void Parser::init(models::FileModel* fileModel, models::FormModel* formModel)
    {
        this->fileModel = fileModel;
        this->formModel = formModel;

        connect(this, &Parser::addFile,
                this->fileModel, &models::FileModel::insertFile);
    }

    models::FileModel& Parser::getFileModel()
    {
        return *fileModel;
    }

    models::FormModel& Parser::getFormModel()
    {
        return *formModel;
    }
}

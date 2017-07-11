/*
** parser.h
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

#ifndef PARSER_H
#define PARSER_H

#include <QStringList>
#include <QDebug>
#include <QMessageBox>
#include <QIcon>
#include <QFile>
#include <QFileInfo>
#include <inttypes.h>
#include <stdio.h>
#include <string>

#include "parsed.h"
#include "tes4record.h"

namespace Parse
{
    class Parser;
}

class Parser
{

public:
    static void parse(QStringList list, QString activePath);
    static void parse(QStringList list);
    static void readHeader(QDataStream* in, TES4Record* TES4);
    static char* readCharArray(QDataStream* in, QByteArray* buffer);
    static uint32_t readUInt32_t(QDataStream* in, QByteArray* buffer);
    static uint32_t getUInt32_t(QByteArray* array);
    static QList<Parsed> getParsed();
    static void warn(QString message);
    static void debug(QStringList list); //THIS IS A DEBUG FUNCTION, IF YOU USE THIS IN PRODUCTION CODE NOTOH WILL MURDER YOUR FAMILY
                                         //AS IF 'DEBUG' WASN'T OBVIOUS ENOUGH
};

#endif // PARSER_H
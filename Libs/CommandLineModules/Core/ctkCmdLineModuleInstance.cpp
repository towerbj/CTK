/*=============================================================================
  
  Library: CTK
  
  Copyright (c) German Cancer Research Center,
    Division of Medical and Biological Informatics
    
  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at
  
    http://www.apache.org/licenses/LICENSE-2.0
    
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
  
=============================================================================*/

#include "ctkCmdLineModuleInstance.h"
#include "ctkCmdLineModuleObjectHierarchyReader.h"

#include <QStringList>

namespace {

QString normalizeFlag(const QString& flag)
{
  return flag.trimmed().remove(QRegExp("^-*"));
}

}

struct ctkCmdLineModuleInstancePrivate
{
  QStringList createCommandLineArgs(QObject *hierarchy)
  {
    ctkCmdLineModuleObjectHierarchyReader reader(hierarchy);

    QStringList cmdLineArgs;
    QHash<int, QString> indexedArgs;
    while(reader.readNextParameter())
    {
      if (reader.index() > -1)
      {
        indexedArgs.insert(reader.index(), reader.value());
      }
      else
      {
        QString argFlag;
        if (reader.longFlag().isEmpty())
        {
          argFlag = QString("-") + normalizeFlag(reader.flag());
        }
        else
        {
          argFlag = QString("--") + normalizeFlag(reader.longFlag());
        }

        QStringList args;
        if (reader.isMultiple())
        {
          args = reader.value().split(',', QString::SkipEmptyParts);
        }
        else
        {
          args.push_back(reader.value());
        }

        foreach(QString arg, args)
        {
          cmdLineArgs << argFlag << arg;
        }
      }
    }

    QList<int> indexes = indexedArgs.keys();
    qSort(indexes.begin(), indexes.end());
    foreach(int index, indexes)
    {
      cmdLineArgs << indexedArgs[index];
    }

    return cmdLineArgs;
  }
};
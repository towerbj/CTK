/*=========================================================================

  Library:   CTK
 
  Copyright (c) 2010  Kitware Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.commontk.org/LICENSE

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 
=========================================================================*/

#ifndef __ctkPixmapIconEngine_h
#define __ctkPixmapIconEngine_h

#include <QIconEngineV2>
#include <QPixmap>
#include <QVector>

struct ctkPixmapIconEngineEntry
{
    ctkPixmapIconEngineEntry():mode(QIcon::Normal), state(QIcon::Off){}
    ctkPixmapIconEngineEntry(const QPixmap &pm, QIcon::Mode m = QIcon::Normal, QIcon::State s = QIcon::Off)
        :pixmap(pm), size(pm.size()), mode(m), state(s){}
    ctkPixmapIconEngineEntry(const QString &file, const QSize &sz = QSize(), QIcon::Mode m = QIcon::Normal, QIcon::State s = QIcon::Off)
        :fileName(file), size(sz), mode(m), state(s){}
    QPixmap pixmap;
    QString fileName;
    QSize size;
    QIcon::Mode mode;
    QIcon::State state;
    bool isNull() const {return (fileName.isEmpty() && pixmap.isNull()); }
};



class ctkPixmapIconEngine : public QIconEngineV2 {
public:
    ctkPixmapIconEngine();
    ctkPixmapIconEngine(const ctkPixmapIconEngine &);
    ~ctkPixmapIconEngine();
    void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state);
    QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state);
    ctkPixmapIconEngineEntry *bestMatch(const QSize &size, QIcon::Mode mode, QIcon::State state, bool sizeOnly);
    QSize actualSize(const QSize &size, QIcon::Mode mode, QIcon::State state);
    void addPixmap(const QPixmap &pixmap, QIcon::Mode mode, QIcon::State state);
    void addFile(const QString &fileName, const QSize &size, QIcon::Mode mode, QIcon::State state);

    // v2 functions
    QString key() const;
    QIconEngineV2 *clone() const;
    bool read(QDataStream &in);
    bool write(QDataStream &out) const;
    void virtual_hook(int id, void *data);

private:
    ctkPixmapIconEngineEntry *tryMatch(const QSize &size, QIcon::Mode mode, QIcon::State state);
    QVector<ctkPixmapIconEngineEntry> pixmaps;

    friend QDataStream &operator<<(QDataStream &s, const QIcon &icon);
    friend class QIconThemeEngine;
};


#endif
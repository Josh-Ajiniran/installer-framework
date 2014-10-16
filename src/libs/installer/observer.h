/**************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Installer Framework.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
**
** $QT_END_LICENSE$
**
**************************************************************************/

#ifndef OBSERVER_H
#define OBSERVER_H

#include <QCryptographicHash>
#include <QObject>

namespace QInstaller {

class Observer : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Observer)

public:
    Observer() {}
    virtual ~Observer() {}

    virtual int progressValue() const = 0;
    virtual QString progressText() const = 0;
};

class FileTaskObserver : public Observer
{
    Q_OBJECT
    Q_DISABLE_COPY(FileTaskObserver)

public:
    FileTaskObserver(QCryptographicHash::Algorithm algorithm);
    ~FileTaskObserver();

    int progressValue() const;
    QString progressText() const;

    QByteArray checkSum() const;
    void addCheckSumData(const char *data, int length);

    void addSample(qint64 sample);
    void timerEvent(QTimerEvent *event);

    void setBytesTransfered(qint64 bytesTransfered);
    void addBytesTransfered(qint64 bytesTransfered);
    void setBytesToTransfer(qint64 bytesToTransfer);

private:
    void init();

private:
    int m_timerId;
    int m_timerInterval;

    qint64 m_bytesTransfered;
    qint64 m_bytesToTransfer;

    qint64 m_samples[50];
    quint32 m_sampleIndex;
    qint64 m_bytesPerSecond;
    qint64 m_currentSpeedBin;

    QCryptographicHash m_hash;
};

}   // namespace QInstaller

#endif // OBSERVER_H

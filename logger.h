#ifndef LOGGER_H
#define LOGGER_H
#ifndef __MY_LOGGER__
#define __MY_LOGGER__

#define LOG_FILE_PATH "log.txt"

//#define LOG_TO_FILE
//#define QT_NO_DEBUG
#define NO_TIME_STAMP true
#define NO_LINE_INFO true
#define LOG_LEVEL -1

#include <QtGlobal>
#include <QMutex>
#include <QMutexLocker>
#include <QString>
#include <QFile>
#include <QtMsgHandler>
#include <QDateTime>
#include <QTextStream>
#include <iostream>

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    QMutexLocker lock(&mutex);

    if(LOG_LEVEL >= type)
    {
        return;
    }

    QString text;
    switch(type)
    {
    case QtDebugMsg:
    {
#ifdef QT_NO_DEBUG
        return;
#endif
        text = QString("[Debug:]");
        break;
    }
    case QtWarningMsg:
    {
        text = QString("[Warning:]");
        break;
    }
    case QtCriticalMsg:
    {
        text = QString("[Critical:]");
        break;
    }
    case QtFatalMsg:
    {
        text = QString("[Fatal:]");
        break;
    }
    case QtInfoMsg:
    {
        text = QString("[Info:]");
        break;
    }

    }

    QString current_date_time = "";

    QString current_date = "";

    QString message;

    if(!NO_TIME_STAMP)
    {
        current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
        current_date = QString("(%1)").arg(current_date_time);
    }

#ifndef QT_NO_DEBUG
    QString context_info = "";
    if(!NO_LINE_INFO)
    {
        context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file),context.line);
    }
    message = QString("%1 %2 %3 %4").arg(text,msg,current_date,context_info);
#else
    message = QString("%1 %2 %3").arg(text,msg,current_date);
#endif

#ifdef LOG_TO_FILE
#ifdef COVER_LOGGING
    QFile file(LOG_FILE_PATH);
    file.open(QIODevice::WriteOnly | QIODevice::OpenModeFlag::Truncate | QIODevice::OpenModeFlag::Unbuffered);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.close();
#else
    QFile file(LOG_FILE_PATH);
    file.open(QIODevice::WriteOnly | QIODevice::OpenModeFlag::Append | QIODevice::OpenModeFlag::Unbuffered);
    QTextStream text_stream(&file);
    text_stream << message << "\n";
    file.close();
#endif
#endif

    std::cerr << qPrintable(message) << "\n";

}

#endif

#endif // LOGGER_H

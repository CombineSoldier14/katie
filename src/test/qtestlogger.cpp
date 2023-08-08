/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtTest module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qtestlogger_p.h"
#include "qtestelement.h"
#include "qtestxmlstreamer.h"
#include "qtestfilelogger.h"
#include "qtestcase.h"
#include "qtestresult_p.h"
#include "qbenchmark_p.h"
#include "qcorecommon_p.h"

#include <string.h>

QT_BEGIN_NAMESPACE

QTestLogger::QTestLogger(TestLoggerFormat fm)
    : listOfTestcases(0), currentLogElement(0),
    logFormatter(0), format(fm), filelogger(new QTestFileLogger()),
    testCounter(0), passCounter(0), failureCounter(0), errorCounter(0),
    skipCounter(0), randomSeed_(0), hasRandomSeed_(false)
{
}

QTestLogger::~QTestLogger()
{
    delete listOfTestcases;
    delete logFormatter;
    delete filelogger;
}

void QTestLogger::startLogging()
{
    switch(format){
        case TLF_XML: {
            logFormatter = new QTestXmlStreamer;
            filelogger->init();
            break;
        }
    }

    logFormatter->setLogger(this);
    logFormatter->startStreaming();
}

void QTestLogger::stopLogging()
{
    logFormatter->output(listOfTestcases);
    logFormatter->stopStreaming();
}

void QTestLogger::enterTestFunction(const char *function)
{
    QSTACKARRAY(char, buf, 1024);
    QTest::qt_snprintf(buf, sizeof(buf), "Entered test-function: %s\n", function);
    filelogger->flush(buf);

    currentLogElement = new QTestElement(QTest::LET_TestCase);
    currentLogElement->addAttribute(QTest::AI_Name, function);
    currentLogElement->addToList(&listOfTestcases);

    ++testCounter;
}

void QTestLogger::leaveTestFunction()
{
}

void QTestLogger::addIncident(IncidentTypes type, const char *description,
                     const char *file, int line)
{
    const char *typeBuf = 0;
    QSTACKARRAY(char, buf, 100);

    switch (type) {
    case QAbstractTestLogger::XPass:
        ++failureCounter;
        typeBuf = "xpass";
        break;
    case QAbstractTestLogger::Pass:
        ++passCounter;
        typeBuf = "pass";
        break;
    case QAbstractTestLogger::XFail:
        ++passCounter;
        typeBuf = "xfail";
        break;
    case QAbstractTestLogger::Fail:
        ++failureCounter;
        typeBuf = "fail";
        break;
    default:
        typeBuf = "??????";
        break;
    }

    if (type == QAbstractTestLogger::Fail || type == QAbstractTestLogger::XPass
            || type == QAbstractTestLogger::XFail) {
        QTestElement *failureElement = new QTestElement(QTest::LET_Failure);
        failureElement->addAttribute(QTest::AI_Result, typeBuf);
        if(file)
            failureElement->addAttribute(QTest::AI_File, file);
        else
            failureElement->addAttribute(QTest::AI_File, "");
        QTest::qt_snprintf(buf, sizeof(buf), "%i", line);
        failureElement->addAttribute(QTest::AI_Line, buf);
        failureElement->addAttribute(QTest::AI_Description, description);
        addTag(failureElement);
        currentLogElement->addLogElement(failureElement);
    }

    /*
        Only one result can be shown for the whole testfunction.
        Check if we currently have a result, and if so, overwrite it
        iff the new result is worse.
    */
    QTestElementAttribute* resultAttr =
        const_cast<QTestElementAttribute*>(currentLogElement->attribute(QTest::AI_Result));
    if (resultAttr) {
        const char* oldResult = resultAttr->value();
        bool overwrite = false;
        if (strcmp(oldResult, "pass") == 0) {
            overwrite = true;
        } else if (strcmp(oldResult, "xfail") == 0) {
            overwrite = (type == QAbstractTestLogger::XPass || type == QAbstractTestLogger::Fail);
        } else if (strcmp(oldResult, "xpass") == 0) {
            overwrite = (type == QAbstractTestLogger::Fail);
        }
        if (overwrite) {
            resultAttr->setPair(QTest::AI_Result, typeBuf);
        }
    } else {
        currentLogElement->addAttribute(QTest::AI_Result, typeBuf);
    }

    if(file)
        currentLogElement->addAttribute(QTest::AI_File, file);
    else
        currentLogElement->addAttribute(QTest::AI_File, "");

    QTest::qt_snprintf(buf, sizeof(buf), "%i", line);
    currentLogElement->addAttribute(QTest::AI_Line, buf);
}

void QTestLogger::addBenchmarkResult(const QBenchmarkResult &result)
{
    QTestElement *benchmarkElement = new QTestElement(QTest::LET_Benchmark);
//    printf("element %i", benchmarkElement->elementType());

    benchmarkElement->addAttribute(
        QTest::AI_Metric,
        QTest::benchmarkMetricName(QBenchmarkTestMethodData::current->result.metric));
    benchmarkElement->addAttribute(QTest::AI_Tag, result.context.tag.toAscii().data());
    benchmarkElement->addAttribute(QTest::AI_Value, QByteArray::number(result.value).constData());

    QSTACKARRAY(char, buf, 100);
    QTest::qt_snprintf(buf, sizeof(buf), "%i", result.iterations);
    benchmarkElement->addAttribute(QTest::AI_Iterations, buf);
    currentLogElement->addLogElement(benchmarkElement);
}

void QTestLogger::addTag(QTestElement* element)
{
    const char *tag = QTestResult::currentDataTag();
    const char *gtag = QTestResult::currentGlobalDataTag();
    const char *filler = (tag && gtag) ? ":" : "";
    if ((!tag || !tag[0]) && (!gtag || !gtag[0])) {
        return;
    }

    if (!tag) {
        tag = "";
    }
    if (!gtag) {
        gtag = "";
    }

    QTestCharBuffer buf;
    QTest::qt_asprintf(&buf, "%s%s%s", gtag, filler, tag);
    element->addAttribute(QTest::AI_Tag, buf.constData());
}

void QTestLogger::addMessage(MessageTypes type, const char *message, const char *file, int line)
{
    QTestElement *errorElement = new QTestElement(QTest::LET_Error);
    const char *typeBuf = 0;

    switch (type) {
    case QAbstractTestLogger::Warn:
        typeBuf = "warn";
        break;
    case QAbstractTestLogger::QDebug:
        typeBuf = "qdebug";
        break;
    case QAbstractTestLogger::QWarning:
        typeBuf = "qwarn";
        break;
    case QAbstractTestLogger::QCritical:
        typeBuf = "qcritical";
        break;
    case QAbstractTestLogger::QFatal:
        typeBuf = "qfatal";
        break;
    case QAbstractTestLogger::Skip:
        ++skipCounter;
        typeBuf = "skip";
        break;
    case QAbstractTestLogger::Info:
        typeBuf = "info";
        break;
    default:
        typeBuf = "??????";
        break;
    }

    errorElement->addAttribute(QTest::AI_Type, typeBuf);
    errorElement->addAttribute(QTest::AI_Description, message);
    addTag(errorElement);

    if(file)
        errorElement->addAttribute(QTest::AI_File, file);
    else
        errorElement->addAttribute(QTest::AI_File, "");

    QSTACKARRAY(char, buf, 100);
    QTest::qt_snprintf(buf, sizeof(buf), "%i", line);
    errorElement->addAttribute(QTest::AI_Line, buf);

    currentLogElement->addLogElement(errorElement);
    ++errorCounter;
}

void QTestLogger::registerRandomSeed(unsigned int seed)
{
    randomSeed_ = seed;
    hasRandomSeed_ = true;
}

unsigned int QTestLogger::randomSeed() const
{
    return randomSeed_;
}

bool QTestLogger::hasRandomSeed() const
{
    return hasRandomSeed_;
}

QT_END_NAMESPACE


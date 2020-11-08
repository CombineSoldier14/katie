/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QURL_H
#define QURL_H

#include <QtCore/qhash.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


class QUrlPrivate;
class QDataStream;
class QMutexLocker;

class Q_CORE_EXPORT QUrl
{
public:
    enum ParsingMode {
        TolerantMode,
        StrictMode
    };

    // encoding / toString values
    enum FormattingOption {
        None = 0x0,
        RemoveScheme = 0x1,
        RemovePassword = 0x2,
        RemoveUserInfo = RemovePassword | 0x4,
        RemovePort = 0x8,
        RemoveAuthority = RemoveUserInfo | RemovePort | 0x10,
        RemovePath = 0x20,
        RemoveQuery = 0x40,
        RemoveFragment = 0x80,
        // 0x100: private: normalized

        StripTrailingSlash = 0x10000
    };
    Q_DECLARE_FLAGS(FormattingOptions, FormattingOption)

    QUrl();
#ifdef QT_NO_URL_CAST_FROM_STRING
    explicit
#endif
    QUrl(const QString &url, ParsingMode mode = TolerantMode);
    QUrl(const QUrl &copy);
    QUrl &operator =(const QUrl &copy);
#ifndef QT_NO_URL_CAST_FROM_STRING
    QUrl &operator =(const QString &url);
#endif
#ifdef Q_COMPILER_RVALUE_REFS
    inline QUrl &operator=(QUrl &&other)
    { qSwap(d, other.d); return *this; }
#endif
    ~QUrl();

    inline void swap(QUrl &other) { qSwap(d, other.d); }

    void setUrl(const QString &url, ParsingMode mode = TolerantMode);
    void setEncodedUrl(const QByteArray &url, ParsingMode mode = TolerantMode);

    bool isValid() const;

    bool isEmpty() const;

    void clear();

    void setScheme(const QString &scheme);
    QString scheme() const;

    void setAuthority(const QString &authority);
    QString authority() const;

    void setUserInfo(const QString &userInfo);
    QString userInfo() const;

    void setUserName(const QString &userName);
    QString userName() const;
    void setEncodedUserName(const QByteArray &userName);
    QByteArray encodedUserName() const;

    void setPassword(const QString &password);
    QString password() const;
    void setEncodedPassword(const QByteArray &password);
    QByteArray encodedPassword() const;

    void setHost(const QString &host);
    QString host() const;
    void setEncodedHost(const QByteArray &host);
    QByteArray encodedHost() const;

    void setPort(int port);
    int port(int defaultPort = -1) const;

    void setPath(const QString &path);
    QString path() const;
    void setEncodedPath(const QByteArray &path);
    QByteArray encodedPath() const;

    bool hasQuery() const;

    void setEncodedQuery(const QByteArray &query);
    QByteArray encodedQuery() const;

    void setQueryDelimiters(char valueDelimiter, char pairDelimiter);
    char queryValueDelimiter() const;
    char queryPairDelimiter() const;

    void setQueryItems(const QList<QPair<QString, QString> > &query);
    void addQueryItem(const QString &key, const QString &value);
    QList<QPair<QString, QString> > queryItems() const;
    bool hasQueryItem(const QString &key) const;
    QString queryItemValue(const QString &key) const;
    QStringList allQueryItemValues(const QString &key) const;
    void removeQueryItem(const QString &key);
    void removeAllQueryItems(const QString &key);

    void setEncodedQueryItems(const QList<QPair<QByteArray, QByteArray> > &query);
    void addEncodedQueryItem(const QByteArray &key, const QByteArray &value);
    QList<QPair<QByteArray, QByteArray> > encodedQueryItems() const;
    bool hasEncodedQueryItem(const QByteArray &key) const;
    QByteArray encodedQueryItemValue(const QByteArray &key) const;
    QList<QByteArray> allEncodedQueryItemValues(const QByteArray &key) const;
    void removeEncodedQueryItem(const QByteArray &key);
    void removeAllEncodedQueryItems(const QByteArray &key);

    void setFragment(const QString &fragment);
    QString fragment() const;
    void setEncodedFragment(const QByteArray &fragment);
    QByteArray encodedFragment() const;
    bool hasFragment() const;
    QString topLevelDomain() const;

    QUrl resolved(const QUrl &relative) const;

    bool isRelative() const;
    bool isParentOf(const QUrl &url) const;

    static QUrl fromLocalFile(const QString &localfile);
    QString toLocalFile() const;
    bool isLocalFile() const;

    QString toString(FormattingOptions options = None) const;

    QByteArray toEncoded(FormattingOptions options = None) const;
    static QUrl fromEncoded(const QByteArray &url, ParsingMode mode = TolerantMode);

    static QUrl fromUserInput(const QString &userInput);

    void detach();
    bool isDetached() const;

    bool operator <(const QUrl &url) const;
    bool operator ==(const QUrl &url) const;
    bool operator !=(const QUrl &url) const;

    static QString fromPercentEncoding(const QByteArray &);
    static QByteArray toPercentEncoding(const QString &,
                                        const QByteArray &exclude = QByteArray(),
                                        const QByteArray &include = QByteArray());
    static QString fromPunycode(const QByteArray &);
    static QByteArray toPunycode(const QString &);
    static QString fromAce(const QByteArray &);
    static QByteArray toAce(const QString &);
    static QStringList idnWhitelist();
    static void setIdnWhitelist(const QStringList &);


    QString errorString() const;

private:
    void detach(QMutexLocker &locker);
    QUrlPrivate *d;
public:
    typedef QUrlPrivate* DataPtr;
    inline DataPtr &data_ptr() { return d; }
};

inline uint qHash(const QUrl &url)
{
    return qHash(url.toEncoded(QUrl::FormattingOption(0x100)));
}

Q_DECLARE_TYPEINFO(QUrl, Q_MOVABLE_TYPE);
Q_DECLARE_SHARED(QUrl)
Q_DECLARE_OPERATORS_FOR_FLAGS(QUrl::FormattingOptions)

#ifndef QT_NO_DATASTREAM
Q_CORE_EXPORT QDataStream &operator<<(QDataStream &, const QUrl &);
Q_CORE_EXPORT QDataStream &operator>>(QDataStream &, QUrl &);
#endif

#ifndef QT_NO_DEBUG_STREAM
Q_CORE_EXPORT QDebug operator<<(QDebug, const QUrl &);
#endif

QT_END_NAMESPACE

QT_END_HEADER

#endif // QURL_H

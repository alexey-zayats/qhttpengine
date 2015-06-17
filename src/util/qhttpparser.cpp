/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Nathan Osman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <QByteArray>

#include "qhttpparser.h"

void QHttpParser::split(const QByteArray &data, const QByteArray &delim, int maxSplit, QList<QByteArray> &parts)
{
    int index = 0;

    for(int i = 0; !maxSplit || i < maxSplit; ++i) {
        int nextIndex = data.indexOf(delim, index);

        if(nextIndex == -1) {
            break;
        }

        parts.append(data.mid(index, nextIndex - index));
        index = nextIndex + delim.length();
    }

    // Append whatever remains to the list
    parts.append(data.mid(index));
}

bool QHttpParser::parseHeaderList(const QList<QByteArray> &lines, QHttpHeaderMap &headers)
{
    for(QList<QByteArray>::const_iterator i = lines.constBegin(); i != lines.constEnd(); ++i) {

        QList<QByteArray> parts;
        split(*i, ":", 1, parts);

        // Ensure that the delimiter (":") was encountered at least once
        if(parts.count() != 2) {
            return false;
        }

        headers.insert(parts[0].trimmed(), parts[1].trimmed());
    }

    return true;
}

bool QHttpParser::parseHeaders(const QByteArray &data, QList<QByteArray> &parts, QHttpHeaderMap &headers)
{
    // Split the data into individual lines
    QList<QByteArray> lines;
    split(data, "\r\n", 0, lines);

    // Split the status line into a maximum of three parts
    split(lines.takeFirst(), " ", 2, parts);
    if(parts.count() != 3) {
        return false;
    }

    return parseHeaderList(lines, headers);
}

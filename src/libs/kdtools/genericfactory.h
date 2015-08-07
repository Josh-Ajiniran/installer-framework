/****************************************************************************
**
** Copyright (C) 2013 Klaralvdalens Datakonsult AB (KDAB)
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Installer Framework.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef GENERICFACTORY_H
#define GENERICFACTORY_H

#include "kdtoolsglobal.h"

#include <QtCore/QHash>

template <typename BASE, typename IDENTIFIER = QString, typename... ARGUMENTS>
class KDGenericFactory
{
public:
    virtual ~KDGenericFactory() {}

    typedef BASE *(*FactoryFunction)(ARGUMENTS...);

    template <typename DERIVED>
    void registerProduct(const IDENTIFIER &id)
    {
        m_hash.insert(id, &KDGenericFactory::create<DERIVED>);
    }

    void registerProduct(const IDENTIFIER &id, FactoryFunction func)
    {
        m_hash.insert(id, func);
    }

    bool containsProduct(const IDENTIFIER &id) const
    {
        return m_hash.contains(id);
    }

    BASE *create(const IDENTIFIER &id, ARGUMENTS... args) const
    {
        const auto it = m_hash.constFind(id);
        if (it == m_hash.constEnd())
            return 0;
        return (*it)(std::forward<ARGUMENTS>(args)...);
    }

protected:
    KDGenericFactory() = default;

private:
    template <typename DERIVED>
    static BASE *create(ARGUMENTS... args)
    {
        return new DERIVED(std::forward<ARGUMENTS>(args)...);
    }

    KDGenericFactory(const KDGenericFactory &) = delete;
    KDGenericFactory &operator=(const KDGenericFactory &) = delete;

private:
    QHash<IDENTIFIER, FactoryFunction> m_hash;
};

#endif // GENERICFACTORY_H
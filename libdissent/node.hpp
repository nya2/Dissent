/* libdissent/node.hpp
   Dissent participant node interface.

   Author: Shu-Chun Weng <scweng _AT_ cs .DOT. yale *DOT* edu>
 */
/* ====================================================================
 * Dissent: Accountable Group Anonymity
 * Copyright (c) 2010 Yale University.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to
 *
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor,
 *   Boston, MA  02110-1301  USA
 */

#ifndef _DISSENT_LIBDISSENT_NODE_H_
#define _DISSENT_LIBDISSENT_NODE_H_ 1

#include <QObject>
#include <QScopedPointer>
#include <QString>

#include "config.hpp"
#include "node_impl.hpp"

namespace Dissent{
class Network;

class Node : public QObject{
  Q_OBJECT
  public:
    Node(const Configuration& config);

    int GetRound() const{ return _protocolRound; }
    Configuration* GetConfig(){ return &_config; }
    Network* GetNetwork() const{ return _network; }

  signals:
    void startIncomingNetwork(QString phase);
    void stopIncomingNetwork();

  public slots:
    void StartProtocol();

    void StartIncomingNetwork(const QString& phase){
        emit startIncomingNetwork(phase);
    }
    void StopIncomingNetwork(){
        emit stopIncomingNetwork();
    }

  private slots:
    void ChangeImpl(NodeImpl* impl);

  private:
    void StartProtocolRound();

    Configuration _config;
    QScopedPointer<NodeImpl> _impl;
    Network* _network;
    int _protocolRound;
};
}
#endif  // _DISSENT_LIBDISSENT_NODE_H_
// -*- vim:sw=4:expandtab:cindent:
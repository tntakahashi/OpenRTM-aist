// -*- C++ -*-
/*!
 * @file  InPortDirectProvider.cpp
 * @brief InPortDirectProvider class
 * @date  $Date: 2008-01-14 07:49:59 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2009-2010
 *     Noriaki Ando
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id: InPortDirectProvider.cpp 1244 2009-03-13 07:25:42Z n-ando $
 *
 */

#include <rtm/InPortDirectProvider.h>

#ifdef WIN32
#pragma warning( disable : 4290 )
#endif

namespace RTC
{
  /*!
   * @if jp
   * @brief ���󥹥ȥ饯��
   * @else
   * @brief Constructor
   * @endif
   */
  InPortDirectProvider::InPortDirectProvider(void)
   : m_buffer(0) 
  {
    // PortProfile setting
    setInterfaceType("direct");
  }

  /*!
   * @if jp
   * @brief �ǥ��ȥ饯��
   * @else
   * @brief Destructor
   * @endif
   */
  InPortDirectProvider::~InPortDirectProvider(void)
  {
  }

  void InPortDirectProvider::init(coil::Properties& prop)
  {
  }

  /*!
   * @if jp
   * @brief �Хåե��򥻥åȤ���
   * @else
   * @brief Setting outside buffer's pointer
   * @endif
   */
  void InPortDirectProvider::
  setBuffer(BufferBase<cdrMemoryStream>* buffer)
  {
    m_buffer = buffer;
  }

  /*!
   * @if jp
   * @brief �ꥹ�ʤ����ꤹ��
   * @else
   * @brief Set the listener
   * @endif
   */
  void InPortDirectProvider::setListener(ConnectorInfo& info,
                                         ConnectorListeners* listeners)
  {
    m_profile = info;
    m_listeners = listeners;
  }

  /*!
   * @if jp
   * @brief Connector�����ꤹ�롣
   * @else
   * @brief set Connector
   * @endif
   */
  void InPortDirectProvider::setConnector(InPortConnector* connector)
  {
    m_connector = connector;
  }

};     // namespace RTC


extern "C"
{
  /*!
   * @if jp
   * @brief �⥸�塼�������ؿ�
   * @else
   * @brief Module initialization
   * @endif
   */
  void InPortDirectProviderInit(void)
  {
    RTC::InPortProviderFactory& factory(RTC::InPortProviderFactory::instance());
    factory.addFactory("direct",
                       ::coil::Creator< ::RTC::InPortProvider,
                                        ::RTC::InPortDirectProvider>,
                       ::coil::Destructor< ::RTC::InPortProvider,
                                           ::RTC::InPortDirectProvider>);
  }
};
﻿// -*- C++ -*-
/*!
 * @file  InPortCorbaCdrProvider.h
 * @brief InPortCorbaCdrProvider class
 * @date  $Date: 2008-01-14 07:49:59 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2009
 *     Noriaki Ando
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id: InPortCorbaCdrProvider.h 1244 2009-03-13 07:25:42Z n-ando $
 *
 */

#ifndef RTC_INPORTCORBACDRPROVIDER_H
#define RTC_INPORTCORBACDRPROVIDER_H

#include <rtm/idl/DataPort_OpenRTMSkel.h>
#include <rtm/BufferBase.h>
#include <rtm/InPortProvider.h>
#include <rtm/CORBA_SeqUtil.h>
#include <rtm/Manager.h>
#include <rtm/ConnectorListener.h>
#include <rtm/ConnectorBase.h>

#ifdef WIN32
#pragma warning( disable : 4290 )
#endif

namespace RTC
{
  /*!
   * @if jp
   * @class InPortCorbaCdrProvider
   * @brief InPortCorbaCdrProvider クラス
   *
   * InPortProvider
   *
   * データ転送に CORBA の OpenRTM::InPortCdr インターフェースを利用し
   * た、push 型データフロー型を実現する InPort プロバイダクラス。
   *
   * @since 0.4.0
   *
   * @else
   * @class InPortCorbaCdrProvider
   * @brief InPortCorbaCdrProvider class
   *
   * The InPort provider class which uses the OpenRTM::InPortCdr
   * interface in CORBA for data transfer and realizes a push-type
   * dataflow.
   *
   * @since 0.4.0
   *
   * @endif
   */
  class InPortCorbaCdrProvider
    : public InPortProvider,
      public virtual POA_OpenRTM::InPortCdr,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    /*!
     * @if jp
     * @brief コンストラクタ
     *
     * コンストラクタ
     *
     * @else
     * @brief Constructor
     *
     * Constructor
     *
     * @endif
     */
    InPortCorbaCdrProvider(void);

    /*!
     * @if jp
     * @brief デストラクタ
     *
     * デストラクタ
     *
     * @else
     * @brief Destructor
     *
     * Destructor
     *
     * @endif
     */
    virtual ~InPortCorbaCdrProvider(void);

    /*!
     * @if jp
     * @brief 設定初期化
     *
     * InPortCorbaCdrProvider の各種設定を行う。与えられた
     * Propertiesから必要な情報を取得して各種設定を行う。この init() 関
     * 数は、InPortProvider生成直後および、接続時にそれぞれ呼ばれる可
     * 能性がある。したがって、この関数は複数回呼ばれることを想定して記
     * 述されるべきである。
     *
     * @param prop 設定情報
     *
     * @else
     *
     * @brief Initializing configuration
     *
     * This operation would be called to configure in initialization.
     * In the concrete class, configuration should be performed
     * getting appropriate information from the given Properties data.
     * This function might be called right after instantiation and
     * connection sequence respectivly.  Therefore, this function
     * should be implemented assuming multiple call.
     *
     * @param prop Configuration information
     *
     * @endif
     */
    virtual void init(coil::Properties& prop);

    /*!
     * @if jp
     * @brief バッファをセットする
     *
     * OutPortProvider がデータを取り出すバッファをセットする。
     * すでにセットされたバッファがある場合、以前のバッファへの
     * ポインタに対して上書きされる。
     * OutPortProviderはバッファの所有権を仮定していないので、
     * バッファの削除はユーザの責任で行わなければならない。
     *
     * @param buffer OutPortProviderがデータを取り出すバッファへのポインタ
     *
     * @else
     * @brief Setting outside buffer's pointer
     *
     * A pointer to a buffer from which OutPortProvider retrieve data.
     * If already buffer is set, previous buffer's pointer will be
     * overwritten by the given pointer to a buffer.  Since
     * OutPortProvider does not assume ownership of the buffer
     * pointer, destructor of the buffer should be done by user.
     *
     * @param buffer A pointer to a data buffer to be used by OutPortProvider
     *
     * @endif
     */
    virtual void setBuffer(BufferBase<cdrMemoryStream>* buffer);

    /*!
     * @if jp
     * @brief リスナを設定する。
     *
     * InPort はデータ送信処理における各種イベントに対して特定のリスナ
     * オブジェクトをコールするコールバック機構を提供する。詳細は
     * ConnectorListener.h の ConnectorDataListener, ConnectorListener
     * 等を参照のこと。InPortCorbaCdrProvider では、以下のコールバック
     * が提供される。
     *
     * - ON_BUFFER_WRITE
     * - ON_BUFFER_FULL
     * - ON_BUFFER_WRITE_TIMEOUT
     * - ON_BUFFER_OVERWRITE
     * - ON_RECEIVED
     * - ON_RECEIVER_FULL
     * - ON_RECEIVER_FULL
     * - ON_RECEIVER_TIMEOUT
     * - ON_RECEIVER_ERROR
     *
     * @param info 接続情報
     * @param listeners リスナオブジェクト
     *
     * @else
     * @brief Set the listener.
     *
     * InPort provides callback functionality that calls specific
     * listener objects according to the events in the data publishing
     * process. For details, see documentation of
     * ConnectorDataListener class and ConnectorListener class in
     * ConnectorListener.h. In this InPortCorbaCdrProvider provides
     * the following callbacks.
     *
     * - ON_BUFFER_WRITE
     * - ON_BUFFER_FULL
     * - ON_BUFFER_WRITE_TIMEOUT
     * - ON_BUFFER_OVERWRITE
     * - ON_RECEIVED
     * - ON_RECEIVER_FULL
     * - ON_RECEIVER_FULL
     * - ON_RECEIVER_TIMEOUT
     * - ON_RECEIVER_ERROR
     *
     * @param info Connector information
     * @param listeners Listener objects
     *
     * @endif
     */
    virtual void setListener(ConnectorInfo& info,
                             ConnectorListeners* listeners);

    /*!
     * @if jp
     * @brief Connectorを設定する。
     *
     * InPort は接続確立時に InPortConnector オブジェクトを生成し、生
     * 成したオブジェクトのポインタと共にこの関数を呼び出す。所有権は
     * InPort が保持するので InPortProvider は InPortConnector を削
     * 除してはいけない。
     *
     * @param connector InPortConnector
     *
     * @else
     * @brief set Connector
     *
     * InPort creates InPortConnector object when it establishes
     * connection between InPort and InPort, and it calls this
     * function with a pointer to the connector object. Since the
     * InPort has the ownership of this connector, InPortProvider
     * should not delete it.
     *
     * @param connector InPortConnector
     *
     * @endif
     */
    virtual void setConnector(InPortConnector* connector);

    /*!
     * @if jp
     * @brief [CORBA interface] バッファにデータを書き込む
     *
     * 設定されたバッファにデータを書き込む。
     *
     * @param data 書込対象データ
     *
     * @else
     * @brief [CORBA interface] Write data into the buffer
     *
     * Write data into the specified buffer.
     *
     * @param data The target data for writing
     *
     * @endif
     */
    virtual ::OpenRTM::PortStatus put(const ::OpenRTM::CdrData& data)
#if __cplusplus < 201703L
      throw (CORBA::SystemException);
#else
    ;
#endif

  private:
    /*!
     * @if jp
     * @brief リターンコード変換
     * @else
     * @brief Return codes conversion
     * @endif
     */
    ::OpenRTM::PortStatus
    convertReturn(BufferStatus::Enum status,
                  cdrMemoryStream& data);


    /*!
     * @if jp
     * @brief ON_BUFFER_WRITE のリスナへ通知する。
     * @param data cdrMemoryStream
     * @else
     * @brief Notify an ON_BUFFER_WRITE event to listeners
     * @param data cdrMemoryStream
     * @endif
     */
    inline void onBufferWrite(cdrMemoryStream& data)
    {
      m_listeners->
        connectorData_[ON_BUFFER_WRITE].notify(m_profile, data);
    }

    /*!
     * @if jp
     * @brief ON_BUFFER_FULL のリスナへ通知する。
     * @param data cdrMemoryStream
     * @else
     * @brief Notify an ON_BUFFER_FULL event to listeners
     * @param data cdrMemoryStream
     * @endif
     */
    inline void onBufferFull(cdrMemoryStream& data)
    {
      m_listeners->
        connectorData_[ON_BUFFER_FULL].notify(m_profile, data);
    }

    /*!
     * @if jp
     * @brief ON_BUFFER_WRITE_TIMEOUT のリスナへ通知する。
     * @param data cdrMemoryStream
     * @else
     * @brief Notify an ON_BUFFER_WRITE_TIMEOUT event to listeners
     * @param data cdrMemoryStream
     * @endif
     */
    inline void onBufferWriteTimeout(cdrMemoryStream& data)
    {
      m_listeners->
        connectorData_[ON_BUFFER_WRITE_TIMEOUT].notify(m_profile, data);
    }

    /*!
     * @if jp
     * @brief ON_BUFFER_WRITE_OVERWRITE のリスナへ通知する。
     * @param data cdrMemoryStream
     * @else
     * @brief Notify an ON_BUFFER_WRITE_OVERWRITE event to listeners
     * @param data cdrMemoryStream
     * @endif
     */
    inline void onBufferWriteOverwrite(cdrMemoryStream& data)
    {
      m_listeners->
        connectorData_[ON_BUFFER_OVERWRITE].notify(m_profile, data);
    }

    /*!
     * @if jp
     * @brief ON_RECEIVED のリスナへ通知する。
     * @param data cdrMemoryStream
     * @else
     * @brief Notify an ON_RECEIVED event to listeners
     * @param data cdrMemoryStream
     * @endif
     */
    inline void onReceived(cdrMemoryStream& data)
    {
      m_listeners->
        connectorData_[ON_RECEIVED].notify(m_profile, data);
    }

    /*!
     * @if jp
     * @brief ON_RECEIVER_FULL のリスナへ通知する。
     * @param data cdrMemoryStream
     * @else
     * @brief Notify an ON_RECEIVER_FULL event to listeners
     * @param data cdrMemoryStream
     * @endif
     */
    inline void onReceiverFull(cdrMemoryStream& data)
    {
      m_listeners->
        connectorData_[ON_RECEIVER_FULL].notify(m_profile, data);
    }

    /*!
     * @if jp
     * @brief ON_RECEIVER_TIMEOUT のリスナへ通知する。
     * @else
     * @brief Notify an ON_RECEIVER_TIMEOUT event to listeners
     * @endif
     */
    inline void onReceiverTimeout(cdrMemoryStream& data)
    {
      m_listeners->
        connectorData_[ON_RECEIVER_TIMEOUT].notify(m_profile, data);
    }

    /*!
     * @if jp
     * @brief ON_RECEIVER_ERRORのリスナへ通知する。
     * @else
     * @Brief Notify an ON_RECEIVER_ERROR event to listeners
     * @endif
     */
    inline void onReceiverError(cdrMemoryStream& data)
    {
      m_listeners->
        connectorData_[ON_RECEIVER_ERROR].notify(m_profile, data);
    }

  private:
    CdrBufferBase* m_buffer;
    ::OpenRTM::InPortCdr_var m_objref;
    ConnectorListeners* m_listeners;
    ConnectorInfo m_profile;
    InPortConnector* m_connector;

  };  // class InPortCorbaCdrProvider
};  // namespace RTC

extern "C"
{
  /*!
   * @if jp
   * @brief モジュール初期化関数
   *
   * InPortCorbaCdrConsumer のファクトリを登録する初期化関数。
   *
   * @else
   * @brief Module initialization
   *
   * This initialization function registers InPortCorbaCdrConsumer's factory.
   *
   * @endif
   */
  void InPortCorbaCdrProviderInit(void);
};

#ifdef WIN32
#pragma warning( default : 4290 )
#endif

#endif  // RTC_INPORTCORBACDRPROVIDER_H


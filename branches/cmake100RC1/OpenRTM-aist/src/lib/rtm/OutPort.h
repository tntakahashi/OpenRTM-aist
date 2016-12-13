// -*- C++ -*-
/*!
 * @file OutPort.h
 * @brief OutPort class
 * @date $Date: 2007-12-31 03:08:05 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2006-2008
 *     Noriaki Ando
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id$
 *
 */

#ifndef OutPort_h
#define OutPort_h

#include <iostream>
#include <string>

#include <coil/TimeValue.h>
#include <coil/Time.h>
#include <coil/TimeMeasure.h>
#include <coil/OS.h>

#include <rtm/RTC.h>
#include <rtm/Typename.h>
#include <rtm/OutPortBase.h>
#include <rtm/CdrBufferBase.h>
#include <rtm/PortCallBack.h>
#include <rtm/OutPortConnector.h>

namespace RTC
{
  /*!
   * @if jp
   *
   * @class OutPort
   *
   * @brief OutPort テンプレートクラス
   * 
   * OutPort テンプレートクラス、テンプレートの引数として、OutPortが
   * 扱うデータ型 DataType, OutPortのバッファ型 BufferType をとる。
   *
   * @since 0.2.0
   *
   * @else
   *
   * @brief OutPort template class
   * 
   * This is the OutPort template class. 
   * The data type "DateType" supported by OutPort and the buffer type
   * "BufferType" are used as template arguments.
   *
   * @since 0.2.0
   *
   * @endif
   */
  template <class DataType>
  class OutPort
    : public OutPortBase
  {
  public:
    /*!
     * @if jp
     *
     * @brief コンストラクタ
     *
     * コンストラクタ
     *
     * @param name ポート名
     * @param value このポートにバインドされるデータ変数
     * @param length バッファ長(デフォルト値:8)
     *
     * @else
     *
     * @brief Constructor
     *
     * Constructor
     *
     * @param name Port's name
     * @param value Data variable bound to this port
     * @param length Buffer length (The default value:8)
     *
     * @endif
     */
    OutPort(const char* name, DataType& value)
      : OutPortBase(name, toTypename<DataType>()), m_value(value),
	m_timeoutTick(1000), // timeout tick: 1ms
	m_readBlock(false), m_readTimeout(0),
	m_writeBlock(false), m_writeTimeout(0),
	m_OnWrite(NULL), m_OnWriteConvert(NULL),
	m_OnRead(NULL),  m_OnReadConvert(NULL),
	m_OnOverflow(NULL), m_OnUnderflow(NULL)
    {
    }
    
    /*!
     * @if jp
     *
     * @brief デストラクタ
     * 
     * デストラクタ
     * 
     * @else
     *
     * @brief Destructor
     * 
     * Destructor
     * 
     * @endif
     */
    virtual ~OutPort(void)
    {
    }
    
    /*!
     * @if jp
     *
     * @brief データ書き込み
     *
     * ポートへデータを書き込む。
     *
     * - コールバックファンクタ OnWrite がセットされている場合、
     *   OutPort が保持するバッファに書き込む前に OnWrite が呼ばれる。
     * - OutPort が保持するバッファがオーバーフローを検出できるバッファであり、
     *   かつ、書き込む際にバッファがオーバーフローを検出した場合、
     *   コールバックファンクタ OnOverflow が呼ばれる。
     * - コールバックファンクタ OnWriteConvert がセットされている場合、
     *   バッファ書き込み時に、 OnWriteConvert の operator() の戻り値が
     *   バッファに書き込まれる。
     *
     * @param value 書き込み対象データ
     *
     * @return 書き込み処理結果(書き込み成功:true、書き込み失敗:false)
     *
     * @else
     *
     * @brief Write data
     *
     * Write data in the port.
     *
     * - When callback functor OnWrite is already set, OnWrite will be
     *   invoked before writing into the buffer held by OutPort.
     * - When the buffer held by OutPort can detect the overflow,
     *   and when it detected the overflow at writing, callback functor
     *   OnOverflow will be invoked.
     * - When callback functor OnWriteConvert is already set, 
     *   the return value of operator() of OnWriteConvert will be written
     *   into the buffer at the writing.
     *
     * @param value The target data for writing
     *
     * @return Writing result (Successful:true, Failed:false)
     *
     * @endif
     */
    virtual bool write(DataType& value)
    {
      if (m_OnWrite != NULL)
	{
	  (*m_OnWrite)(value);
	}

      // check number of connectors
      size_t conn_size(m_connectors.size());
      if (!(conn_size > 0)) { return true; }
        
      // set timestamp
      coil::TimeValue tm(coil::gettimeofday());
      value.tm.sec  = tm.sec();
      value.tm.nsec = tm.usec() * 1000;

      // data -> (conversion) -> CDR stream
      m_cdr.rewindPtrs();
      if (m_OnWriteConvert != NULL)
        {
          ((*m_OnWriteConvert)(value)) >>= m_cdr;
        }
      else
        {
          value >>= m_cdr;
        }

      bool result(true);
      for (size_t i(0), len(conn_size); i < len; ++i)
        {
          ReturnCode ret;
          ret = m_connectors[i]->write(m_cdr);
          if (ret != PORT_OK)
            {
              result = false;
              if (ret == CONNECTION_LOST)
                {
                  disconnect(m_connectors[i]->id());
                }
            }
        }
      return result;
    }
    
    /*!
     * @if jp
     *
     * @brief データ書き込み
     *
     * ポートへデータを書き込む。
     * バインドされた変数に設定された値をポートに書き込む。
     *
     * @return 書き込み処理結果(書き込み成功:true、書き込み失敗:false)
     *
     * @else
     *
     * @brief Write data
     *
     * Write data to the port.
     * Write the value, which was set to the bound variable, to the port.
     *
     * @return Writing result (Successful:true, Failed:false)
     *
     * @endif
     */
    bool write()
    {
      return write(m_value);
    }
    
    /*!
     * @if jp
     *
     * @brief データ書き込み
     *
     * ポートへデータを書き込む。
     * 設定された値をポートに書き込む。
     *
     * @param value 書き込み対象データ
     *
     * @return 書き込み処理結果(書き込み成功:true、書き込み失敗:false)
     *
     * @else
     *
     * @brief Write data
     *
     * Write data to the port.
     * Write the set value to the port.
     *
     * @param value The target data for writing
     *
     * @return Writing result (Successful:true, Failed:false)
     *
     * @endif
     */
    bool operator<<(DataType& value)
    {
      return write(value);
    }
    
    
    /*!
     * @if jp
     *
     * @brief データ読み出し処理のブロックモードの設定
     *
     * 読み出し処理に対してブロックモードを設定する。
     * ブロックモードを指定した場合、読み出せるデータを受信するかタイムアウト
     * が発生するまで、read() メソッドの呼びだしがブロックされる。
     *
     * @param block ブロックモードフラグ
     *
     * @else
     *
     * @brief Set blocking mode of the data read processing
     *
     * Set the blocking mode for the readout.
     * When the block mode is specified, the invoke of read() method is
     * blocked until the readout data is received or timeout occurs.
     *
     * @param block Flag of blocking mode
     *
     * @endif
     */
    void setReadBlock(bool block)
    {
      m_readBlock = block;
    }
    
    /*!
     * @if jp
     *
     * @brief データ書き込み処理のブロックモードの設定
     *
     * 書き込み処理に対してブロックモードを設定する。
     * ブロックモードを指定した場合、バッファに書き込む領域ができるか
     * タイムアウトが発生するまで write() メソッドの呼びだしがブロックされる。
     *
     * @param block ブロックモードフラグ
     *
     * @else
     *
     * @brief Set blocking mode of the data writing processing
     *
     * Set the blocking mode for the writing.
     * When the block mode is specified, the invoke of write() method is
     * blocked until the area written into the buffer can be used or timeout
     * occurs.
     *
     * @param block Flag of blocking mode
     *
     * @endif
     */
    void setWriteBlock(bool block)
    {
      m_writeBlock = block;
    }
    
    /*!
     * @if jp
     *
     * @brief 読み出し処理のタイムアウト時間の設定
     * 
     * read() のタイムアウト時間を usec で設定する。
     * read() はブロックモードでなければならない。
     *
     * @param timeout タイムアウト時間 [usec]
     *
     * @else
     *
     * @brief Set timeout of the data read processing
     * 
     * Set the timeout period of read() with usec.
     * read() must be a block mode.
     *
     * @param timeout Timeout period[usec]
     *
     * @else
     *
     * @endif
     */
    void setReadTimeout(long int timeout)
    {
      m_readTimeout = timeout;
    }
    
    /*!
     * @if jp
     *
     * @brief 書き込み処理のタイムアウト時間の設定
     * 
     * write() のタイムアウト時間を usec で設定する。
     * write() はブロックモードでなければならない。
     *
     * @param timeout タイムアウト時間 [usec]
     *
     * @else
     *
     * @brief Set timeout of the data writing processing
     * 
     * Set the timeout period of write() with usec.
     * write() must be a block mode.
     *
     * @param timeout Timeout period[usec]
     *
     * @endif
     */
    void setWriteTimeout(long int timeout)
    {
      m_writeTimeout = timeout;
    }
    
    /*!
     * @if jp
     *
     * @brief OnWrite コールバックの設定
     *
     * データ書き込み直前に呼ばれる OnWrite コールバックファンクタを設定する。
     *
     * @param on_write OnWrite コールバックファンクタ
     *
     * @else
     *
     * @brief Set OnWrite callback
     *
     * Set OnWrite callback functor invoked immediately before data is written.
     *
     * @param on_write OnWrite callback functor
     *
     * @endif
     */
    inline void setOnWrite(OnWrite<DataType>* on_write)
    {
      m_OnWrite = on_write;
    }
    
    /*!
     * @if jp
     *
     * @brief OnWriteConvert コールバックの設定
     *
     * データ書き込み時に呼ばれる OnWriteConvert コールバックファンクタを設定
     * する。
     * このコールバック関数の処理結果が書き込まれる。
     * このため書き込みデータのフィルタリングが可能となる。
     *
     * @param on_wconvert OnWriteConvert コールバックファンクタ
     *
     * @else
     *
     * @brief Set OnWriteConvert callback
     *
     * Set OnWriteConvert callback functor invoked when data is written.
     * The processing result of this callback function is written.
     * Therefore, filtering of the writing data will be possible.
     *
     * @param on_wconvert OnWriteConvert callback functor
     *
     * @endif
     */
    inline void setOnWriteConvert(OnWriteConvert<DataType>* on_wconvert)
    {
      m_OnWriteConvert = on_wconvert;
    }
    
    /*!
     * @if jp
     *
     * @brief OnOverflow コールバックの設定
     *
     * バッファフルによりデータ書き込みができない場合に呼び出される OnOverflow
     * コールバックファンクタを設定する。
     *
     * @param on_overflow OnOverflow コールバックファンクタ
     *
     * @else
     *
     * @brief Set OnOverflow callback
     *
     * Set OnOverflow callback functor, which is invoked when data cannot be
     * written because of the buffer full.
     *
     * @param on_overflow OnOverflow callback functor
     *
     * @endif
     */
    inline void setOnOverflow(OnOverflow<DataType>* on_overflow)
    {
      m_OnOverflow = on_overflow;
    }
    
    /*!
     * @if jp
     *
     * @brief OnRead コールバックの設定
     *
     * データ読み出し直前に呼び出される OnRead コールバックファンクタを設定
     * する。
     *
     * @param on_read OnRead コールバックファンクタ
     *
     * @else
     *
     * @brief Set OnRead callback
     *
     * Set OnWrite callback functor invoked immediately before data is readout.
     *
     * @param on_read OnRead callback functor
     *
     * @endif
     */
    inline void setOnRead(OnRead<DataType>* on_read)
    {
      m_OnRead = on_read;
    }
    
    /*!
     * @if jp
     *
     * @brief OnReadConvert コールバックの設定
     *
     * データ読み出し時に呼ばれる OnReadConvert コールバックファンクタを設定
     * する。
     * このコールバック関数の処理結果が読み込まれる。
     * このため読み込みデータのフィルタリングが可能となる。
     *
     * @param on_rconvert OnReadConvert コールバックファンクタ
     *
     * @else
     *
     * @brief Set OnReadConvert callback
     *
     * Set OnReadConvert callback functor invoked when data is readout.
     * The processing result of this callback function is readout.
     * Therefore, filtering of the read data will be possible.
     *
     * @param on_rconvert OnReadConvert callback functor
     *
     * @endif
     */
    inline void setOnReadConvert(OnReadConvert<DataType>* on_rconvert)
    {
      m_OnReadConvert = on_rconvert;
    }
    
    /*!
     * @if jp
     *
     * @brief OnUnderflow コールバックの設定
     *
     * バッファエンプティにより読み出せるデータがない場合に呼び出される
     * コールバックファンクタ OnUnderflow を設定する。
     *
     * @param on_underflow OnUnderflow コールバックファンクタ
     *
     * @else
     *
     * @brief Set OnUnderflow callback
     *
     * Set OnUnderflow callback functor, which is invoked when data cannot be
     * readout because of the buffer empty.
     *
     * @param on_underflow OnUnderflow callback functor
     *
     * @endif
     */
    inline void setOnUnderflow(OnUnderflow<DataType>* on_underflow)
    {
      m_OnUnderflow = on_underflow;
    }    


    inline void setOnConnect(OnConnect* on_connect)
    {
      m_OnConnect = on_connect;
    }
    
    inline void setOnDisconnect(OnConnect* on_disconnect)
    {
      m_OnDisconnect = on_disconnect;
    }
    
    virtual void onConnect(const char* id, PublisherBase* publisher)
    {
      std::cout << "onConnect(id = " << id << ")" << std::endl;
    }

    virtual void onDisconnect(const char* id)
    {
      std::cout << "onDisconnect(id = " << id << ")" << std::endl;
    }



    struct subscribe
    {
      subscribe(const ConnectorProfile& prof)
	: m_prof(&prof), _consumer(NULL) 
      {
      }
      
      subscribe(const subscribe& subs)
	: m_prof(subs.m_prof),
	  _consumer(subs._consumer)
      {
      }
      
      subscribe& operator=(const subscribe& subs)
      {
	if (this == &subs) return *this;
	m_prof = subs.m_prof;
	_consumer = subs._consumer;
	return *this;
      }
      
      void operator()(InPortConsumer* cons)
      {
	if (cons->subscribeInterface(m_prof->properties))
	  {
	    _consumer = cons;
	  }
      }
      const ConnectorProfile* m_prof;
      InPortConsumer* _consumer;
    };


  private:
    std::string m_typename;
    /*!
     * @if jp
     * @brief バインドされる T 型の変数への参照
     * @else
     * @brief The reference to type-T variable that is bound.
     * @endif
     */
    DataType& m_value;
    
    /*!
     * @if jp
     * @brief タイムアウトのポーリング周期 [usec]
     * @else
     * @brief Polling cycle of time-out [usec]
     * @endif
     */
    long int m_timeoutTick;
    
    /*!
     * @if jp
     * @brief 読み込み処理時のブロック・非ブロックモードフラグ
     * @else
     * @brief Flag of read()'s blocking/non-blocking mode
     * @endif
     */
    bool m_readBlock;
    
    /*!
     * @if jp
     * @brief 読み込み処理のタイムアウト時間 [usec]
     * @else
     * @brief Timeout of read() [usec]
     * @endif
     */
    long int m_readTimeout;
    
    /*!
     * @if jp
     * @brief 書き込み処理時のブロック・非ブロックモードフラグ
     * @else
     * @brief Flag of write()'s blocking/non-blocking mode
     * @endif
     */
    bool m_writeBlock;
    
    /*!
     * @if jp
     * @brief 書き込み処理のタイムアウト時間 [usec]
     * @else
     * @brief Timeout of write() [usec]
     * @endif
     */
    long int m_writeTimeout;
    
    /*!
     * @if jp
     * @brief OnWrite コールバックファンクタへのポインタ
     * @else
     * @brief Pointer to OnWrite callback functor
     * @endif
     */
    OnWrite<DataType>* m_OnWrite;
    
    /*!
     * @if jp
     * @brief OnWriteConvert コールバックファンクタへのポインタ
     * @else
     * @brief Pointer to OnWriteConvert callback functor
     * @endif
     */
    OnWriteConvert<DataType>* m_OnWriteConvert;
    
    /*!
     * @if jp
     * @brief OnRead コールバックファンクタへのポインタ
     * @else
     * @brief Pointer to OnRead callback functor
     * @endif
     */
    OnRead<DataType>* m_OnRead;
    
    /*!
     * @if jp
     * @brief OnReadConvert コールバックファンクタへのポインタ
     * @else
     * @brief Pointer to OnReadConvert callback functor
     * @endif
     */
    OnReadConvert<DataType>* m_OnReadConvert;
    
    /*!
     * @if jp
     * @brief OnOverflow コールバックファンクタへのポインタ
     * @else
     * @brief Pointer to OnOverflow callback functor
     * @endif
     */
    OnOverflow<DataType>* m_OnOverflow;
    
    /*!
     * @if jp
     * @brief OnUnderflow コールバックファンクタへのポインタ
     * @else
     * @brief Pointer to OnUnderflow callback functor
     *
     * @endif
     */
    OnUnderflow<DataType>* m_OnUnderflow;
    
    OnConnect* m_OnConnect;
    OnDisconnect* m_OnDisconnect;


    std::vector<OutPortProvider*> m_providers;
    std::vector<InPortConsumer*> m_consumers;
    //    std::vector<ConnectorBase*> m_connectors;
    coil::TimeMeasure m_cdrtime;
    static const long int usec_per_sec = 1000000;
    cdrMemoryStream m_cdr;
  };
}; // namespace RTC

#endif // OutPort_h

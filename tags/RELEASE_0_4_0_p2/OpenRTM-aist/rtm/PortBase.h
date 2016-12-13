
// -*- C++ -*-
/*!
 * @file PortBase.h
 * @brief RTC's Port base class
 * @date $Date: 2007-07-20 16:01:45 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2006
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id: PortBase.h,v 1.10.2.1 2007-07-20 16:01:45 n-ando Exp $
 *
 */

/*
 * $Log: not supported by cvs2svn $
 * Revision 1.10  2007/04/26 15:31:34  n-ando
 * The header include order was modified to define _REENTRANT before
 * including ace/config-lite.h in Linux systems.
 * In ace 5.4.7 or later, _REENTRANT flag should be defined explicitly.
 *
 * Revision 1.9  2007/04/13 15:53:03  n-ando
 * RTC::OK was changed to RTC::RTC_OK.
 *
 * Revision 1.8  2007/02/04 17:00:59  n-ando
 * Object reference variable was added.
 *
 * Revision 1.7  2007/01/04 00:43:35  n-ando
 * Now, notify_connect() and notify_disconnect() behavior can be customized
 * publishInterfaces(), subscribeInterfaces() and unsubscribeInterfaces().
 *
 * Revision 1.6  2006/12/02 18:50:22  n-ando
 * A trivial fix.
 *
 * Revision 1.5  2006/11/27 09:57:09  n-ando
 * addProvider() function was added for registration of provider.
 * addConsumer() function was added for registration of consumer.
 *
 * Revision 1.4  2006/11/06 01:46:47  n-ando
 * #include <assert.h> was added.
 *
 * Revision 1.3  2006/11/06 01:16:39  n-ando
 * Now PortBase doesn't depend on PortProfileHelper.
 * Class refference manual has been updated.
 *
 * Revision 1.2  2006/10/17 19:06:45  n-ando
 * connect(), disconnect() and disconnect_all() was implemented.
 *
 * Revision 1.1  2006/10/17 10:22:24  n-ando
 * The first commitment.
 *
 */

#ifndef PortBase_h
#define PortBase_h

#include <rtm/RTC.h>

#include <string>
#include <vector>
#include <ace/Guard_T.h>
#include <ace/Recursive_Thread_Mutex.h>
#include <rtm/idl/RTCSkel.h>
#include <rtm/CORBA_SeqUtil.h>
#include <rtm/NVUtil.h>

#include <iostream>
namespace RTC
{
  //  class ConsumerBase;

  /*!
   * @if jp
   * @class PortBase
   * @brief Port の基底クラス
   *
   * RTC::Port の基底となるクラス。
   * RTC::Port はほぼ UML Port の概念を継承しており、ほぼ同等のものとみなす
   * ことができる。RT コンポーネントのコンセプトにおいては、
   * Port はコンポーネントに付属し、コンポーネントが他のコンポーネントと相互作用
   * を行う接点であり、通常幾つかのインターフェースと関連付けられる。
   * コンポーネントは Port を通して外部に対しインターフェースを提供または要求
   * することができ、Portはその接続を管理する役割を担う。
   * <p>
   * Port の具象クラスは、通常 RT コンポーネントインスタンス生成時に同時に
   * 生成され、提供・要求インターフェースを登録した後、RT コンポーネントに
   * 登録され、外部からアクセス可能な Port として機能することを想定している。
   * <p>
   * RTC::Port は CORBA インターフェースとして以下のオペレーションを提供する。
   *
   * - get_port_profile()
   * - get_connector_profiles()
   * - get_connector_profile()
   * - connect()
   * - notify_connect()
   * - disconnect()
   * - notify_disconnect()
   * - disconnect_all()
   *
   * このクラスでは、これらのオペレーションの実装を提供する。
   * <p>
   * これらのオペレーションのうち、get_port_profile(), get_connector_profiles(),
   * get_connector_profile(), connect(), disconnect(), disconnect_all() は、
   * サブクラスにおいて特に振る舞いを変更する必要がないため、オーバーライド
   * することは推奨されない。
   * <p>
   * notify_connect(), notify_disconnect() については、サブクラスが提供・要求
   * するインターフェースの種類に応じて、振る舞いを変更する必要が生ずる
   * かもしれないが、これらを直接オーバーライドすることは推奨されず、
   * 後述の notify_connect(), notify_disconnect() の項においても述べられる通り
   * これらの関数に関連した protected 関数をオーバーライドすることにより
   * 振る舞いを変更することが推奨される。
   *
   * @else
   *
   *
   * @endif
   */  
  class PortBase
    : public virtual POA_RTC::Port,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    /*!
     * @if jp
     * @brief コンストラクタ
     *
     * PortBase のコンストラクタは Port 名 name を引数に取り初期化を行う
     * と同時に、自分自身を CORBA Object として活性化し、自身の PortProfile
     * の port_ref に自身のオブジェクトリファレンスを格納する。
     *
     * @param name Port の名前
     *
     * @else
     *
     * @brief Constructor
     *
     * The constructor of the ProtBase class is given the name of this Port
     * and initialized. At the same time, the PortBase activates itself
     * as CORBA object and stores its object reference to the PortProfile's 
     * port_ref member.
     *
     * @param name The name of Port 
     *
     * @endif
     */
    PortBase(const char* name = "");


    /*!
     * @if jp
     *
     * @brief デストラクタ
     *
     * @else
     *
     * @brief Destructor
     *
     * @endif
     */
    virtual ~PortBase();


    /*!
     * @if jp
     *
     * @brief [CORBA interface] PortProfileを取得する
     *
     * Portが保持するPortProfileを返す。
     * PortProfile 構造体は以下のメンバーを持つ。
     *
     * - name              [string 型] Port の名前。
     * - interfaces        [PortInterfaceProfileList 型] Port が保持する
     *                     PortInterfaceProfile のシーケンス
     * - port_ref          [Port Object 型] Port 自身のオブジェクトリファレンス
     * - connector_profile [ConnectorProfileList 型] Port が現在保持する
     *                     ConnectorProfile のシーケンス
     * - owner             [RTObject Object 型] この Port を所有する
     *                     RTObjectのリファレンス
     * - properties        [NVList 型] その他のプロパティ。
     *
     * @return この Port の PortProfile
     *
     * @else
     *
     * @brief [CORBA interface] Get the PortProfile of the Port
     *
     * This operation returns the PortProfile of the Port.
     * PortProfile struct has the following members,
     *
     * - name              [string ] The name of the Port.
     * - interfaces        [PortInterfaceProfileList 型] The sequence of 
     *                     PortInterfaceProfile owned by the Port
     * - port_ref          [Port Object] The object reference of the Port.
     * - connector_profile [ConnectorProfileList 型] The sequence of 
     *                     ConnectorProfile owned by the Port.
     * - owner             [RTObject Object] The object reference of 
     *                     RTObject that is owner of the Port.
     * - properties        [NVList] The other properties.
     *
     * @return the PortProfile of the Port
     *
     * @endif
     */
    virtual PortProfile* get_port_profile();


    /*!
     * @if jp
     *
     * @brief [CORBA interface] ConnectorProfileListを取得する
     *
     * Portが保持する ConnectorProfile の sequence を返す。
     * ConnectorProfile は Port 間の接続プロファイル情報を保持する構造体であり、
     * 接続時にPort間で情報交換を行い、関連するすべての Port で同一の値が
     * 保持される。
     * ConnectorProfile は以下のメンバーを保持している。
     *
     * - name         [string 型] このコネクタの名前。
     * - connector_id [string 型] ユニークなID
     * - ports        [Port sequnce] このコネクタに関連する Port のオブジェクト
     *                リファレンスのシーケンス。
     * - properties   [NVList 型] その他のプロパティ。
     *
     * @return この Port の ConnectorProfile
     *
     * @else
     *
     * @brief [CORBA interface] Get the ConnectorProfileList of the Port
     *
     * This operation returns a list of the ConnectorProfiles of the Port.
     * ConnectorProfile includes the connection information that describes 
     * relation between (among) Ports, and Ports exchange the ConnectionProfile
     * on connection process and hold the same information in each Port.
     * ConnectionProfile has the following members,
     *
     * - name         [string] The name of the connection.
     * - connector_id [string] Unique identifier.
     * - ports        [Port sequnce] The sequence of Port's object reference
     *                that are related the connection.
     * - properties   [NVList] The other properties.
     *
     * @return the ConnectorProfileList of the Port
     *
     * @endif
     */
    virtual ConnectorProfileList* get_connector_profiles();


    /*!
     * @if jp
     *
     * @brief [CORBA interface] ConnectorProfile を取得する
     *
     * connector_id で指定された ConnectorProfile を返す。
     *
     * @param connector_id ConnectorProfile の ID
     * @return connector_id を持つ ConnectorProfile
     *
     * @else
     *
     * @brief [CORBA interface] Get the ConnectorProfile
     *
     * This operation returns the ConnectorProfiles specified connector_id.
     *
     * @param connector_id ID of the ConnectorProfile
     * @return the ConnectorProfile identified by the connector_id
     *
     * @endif
     */
    virtual ConnectorProfile* get_connector_profile(const char* connector_id);


    /*!
     * @if jp
     *
     * @brief [CORBA interface] Port の接続を行う
     *
     * 与えられた ConnectoionProfile にしたがって、Port間の接続を確立する。
     * アプリケーションプログラム側は、幾つかのコンポーネントが持つ複数の
     * Port を接続したい場合、適切な値をセットした ConnectorProfile を
     * connect() の引数として与えてコールすることにより、関連する Port の
     * 接続を確立する。
     *
     * connect() に与える ConnectorProfile のメンバーのうち、name, ports, 
     * (properties) メンバーに対してデータをセットしなければならない。
     *
     * @param connector_profile ConnectorProfile
     * @return ReturnCode_t オペレーションのリターンコード
     *
     * @else
     *
     * @brief [CORBA interface] Connect the Port
     *
     * This operation establishes connection according to the given 
     * ConnectionProfile inforamtion. 
     * Application programs, which is going to establish the connection 
     * among Ports owned by RT-Components, have to set valid values to the 
     * ConnectorProfile and give it to the argument of connect() operation.
     * 
     * name, ports, (properties) members of ConnectorProfile should be set
     * valid values before giving to the argument of connect() operation.
     *
     * @param connector_profile The ConnectorProfile.
     * @return ReturnCode_t The return code of this operation.
     *
     * @endif
     */
    virtual ReturnCode_t connect(ConnectorProfile& connector_profile);


    /*!
     * @if jp
     *
     * @brief [CORBA interface] Port の接続通知を行う
     *
     * このオペレーションは、Port間の接続が行われる際に、Port間で内部的に
     * 呼ばれるオペレーションである。
     *
     * @param connector_profile ConnectorProfile
     * @return ReturnCode_t オペレーションのリターンコード
     *
     * @else
     *
     * @brief [CORBA interface] Notify the Ports connection
     *
     * This operation is invoked between Ports internally when the connection
     * is established.
     *
     * @param connector_profile The ConnectorProfile.
     * @return ReturnCode_t The return code of this operation.
     *
     * @endif
     */
    virtual ReturnCode_t notify_connect(ConnectorProfile& connector_profile);


    /*!
     * @if jp
     *
     * @brief [CORBA interface] Port の接続を解除する
     *
     * このオペレーションは接続確立時に接続に対して与えられる connector_id に
     * 対応するピア Port との接続を解除する。
     *
     * @param connector_id ConnectorProfile の ID
     * @return ReturnCode_t オペレーションのリターンコード
     *
     * @else
     *
     * @brief [CORBA interface] Connect the Port
     *
     * This operation destroys connection between this port and the peer port
     * according to given id that is given when the connection established.
     *
     * @param connector_id The ID of the ConnectorProfile.
     * @return ReturnCode_t The return code of this operation.
     *
     * @endif
     */
    virtual ReturnCode_t disconnect(const char* connector_id);


    /*!
     * @if jp
     *
     * @brief [CORBA interface] Port の接続解除通知を行う
     *
     * このオペレーションは、Port間の接続解除が行われる際に、Port間で内部的に
     * 呼ばれるオペレーションである。
     *
     * @param connector_id ConnectorProfile の ID
     * @return ReturnCode_t オペレーションのリターンコード
     *
     * @else
     *
     * @brief [CORBA interface] Notify the Ports disconnection
     *
     * This operation is invoked between Ports internally when the connection
     * is destroied.
     *
     * @param connector_id The ID of the ConnectorProfile.
     * @return ReturnCode_t The return code of this operation.
     *
     * @endif
     */
    virtual ReturnCode_t notify_disconnect(const char* connector_id);


    /*!
     * @if jp
     *
     * @brief [CORBA interface] Port の全接続を解除する
     *
     * このオペレーションはこの Port に関連した全ての接続を解除する。
     *
     * @return ReturnCode_t オペレーションのリターンコード
     *
     * @else
     *
     * @brief [CORBA interface] Connect the Port
     *
     * This operation destroys all connection channels owned by the Port.
     *
     * @return ReturnCode_t The return code of this operation.
     *
     * @endif
     */
    virtual ReturnCode_t disconnect_all();


    //============================================================
    // Local operations
    //============================================================
    /*!
     * @if jp
     * @brief Port の名前を設定する
     *
     * Port の名前を設定する。この名前は Port が保持する PortProfile.name
     * に反映される。
     *
     * @param name Port の名前
     *
     * @else
     * @brief Set the name of this Port
     *
     * This operation sets the name of this Port. The given Port's name is
     * applied to Port's PortProfile.name.
     *
     * @param name The name of this Port.
     *
     * @endif
     */
    void setName(const char* name);


    /*!
     * @if jp
     * @brief PortProfileを取得する
     *
     * Portが保持する PortProfile の const 参照を返す。
     *
     * @return この Port の PortProfile
     *
     * @else
     * @brief Get the PortProfile of the Port
     *
     * This operation returns const reference of the PortProfile.
     *
     * @return the PortProfile of the Port
     *
     * @endif
     */
    const PortProfile& getProfile() const;
    

    /*!
     * @if jp
     *
     * @brief Port のオブジェクト参照を設定する
     *
     * このオペレーションは Port の PortProfile にこの Port 自身の
     * オブジェクト参照を設定する。
     *
     * @param この Port のオブジェクト参照
     *
     * @else
     *
     * @brief Set the object reference of this Port
     *
     * This operation sets the object reference itself
     * to the Port's PortProfile.
     *
     * @param The object reference of this Port.
     *
     * @endif
     */
    void setPortRef(Port_ptr port_ref);


    /*!
     * @if jp
     *
     * @brief Port のオブジェクト参照を取得する
     *
     * このオペレーションは Port の PortProfile が保持している
     * この Port 自身のオブジェクト参照を取得する。
     *
     * @return この Port のオブジェクト参照
     *
     * @else
     *
     * @brief Get the object reference of this Port
     *
     * This operation returns the object reference
     * that is stored in the Port's PortProfile.
     *
     * @return The object reference of this Port.
     *
     * @endif
     */
    Port_ptr getPortRef();


    /*!
     * @if jp
     *
     * @brief Port の owner の RTObject を指定する
     *
     * このオペレーションは Port の PortProfile.owner を設定する。
     *
     * @param owner この Port を所有する RTObject の参照
     *
     * @else
     *
     * @brief Set the owner RTObject of the Port
     *
     * This operation sets the owner RTObject of this Port.
     *
     * @param owner The owner RTObject's reference of this Port
     *
     * @endif
     */
    void setOwner(RTObject_ptr owner);


    //============================================================
    // protected operations
    //============================================================
  protected:
    /*! @if jp
     *
     * @brief Interface 情報を公開する
     *
     * このオペレーションは、notify_connect() 処理シーケンスの始めにコール
     * される純粋仮想関数である。
     * notify_connect() では、
     *
     * - publishInterfaces()
     * - connectNext()
     * - subscribeInterfaces()
     * - updateConnectorProfile()
     *
     * の順に protected 関数がコールされ接続処理が行われる。
     * <br>
     * 具象 Port ではこのオペレーションをオーバーライドし、引数として
     * 与えられた ConnectorProfile に従い処理を行い、パラメータが不適切
     * であれば、RteurnCode_t 型のエラーコードを返す。
     * 通常 publishInterafaces() 内においては、この Port に属する
     * インターフェースに関する情報を ConnectorProfile に対して適切に設定し
     * 他の Port に通知しなければならない。
     * <br>
     * また、この関数がコールされる段階では、他の Port の Interface に関する
     * 情報はすべて含まれていないので、他の Port の Interface を取得する処理
     * は subscribeInterfaces() 内で行われるべきである。
     * <br>
     * このオペレーションは、新規の connector_id に対しては接続の生成、
     * 既存の connector_id に対しては更新が適切に行われる必要がある。
     *
     * @param connector_profile 接続に関するプロファイル情報
     * @return ReturnCode_t 型のリターンコード
     *
     * @else
     *
     * @brief Publish interface information
     *
     * This operation is pure virutal method that would be called at the
     * beginning of the notify_connect() process sequence.
     * In the notify_connect(), the following methods would be called in order.
     *
     * - publishInterfaces()
     * - connectNext()
     * - subscribeInterfaces()
     * - updateConnectorProfile() 
     *
     * In the concrete Port, this method should be overridden. This method
     * processes the given ConnectorProfile argument and if the given parameter
     * is invalid, it would return error code of ReturnCode_t.
     * Usually, publishInterfaces() method should set interfaces information
     * owned by this Port, and publish it to the other Ports.
     * <br>
     * When this method is called, other Ports' interfaces information may not
     * be completed. Therefore, the process to obtain other Port's interfaces
     * information should be done in the subscribeInterfaces() method.
     * <br>
     * This operation should create the new connection for the new
     * connector_id, and should update the connection for the existing
     * connection_id.
     *
     * @param connector_profile The connection profile information
     * @return The return code of ReturnCode_t type.
     *
     * @endif
     */
    virtual ReturnCode_t
    publishInterfaces(ConnectorProfile& connector_profile) = 0;
    

    /*!
     * @if jp
     *
     * @brief 次の Port に対して notify_connect() をコールする
     *
     * ConnectorProfile の port_ref 内に格納されている Port のオブジェクト
     * リファレンスのシーケンスの中から、自身の Port の次の Port に対して
     * notify_connect() をコールする。
     *
     * @param connector_profile 接続に関するプロファイル情報
     * @return ReturnCode_t 型のリターンコード
     *
     * @else
     *
     * @brief Call notify_connect() of the next Port
     *
     * This operation calls the notify_connect() of the next Port's 
     * that stored in ConnectorProfile's port_ref sequence.
     *
     * @param connector_profile The connection profile information
     * @return The return code of ReturnCode_t type.
     *
     * @endif
     */
    virtual ReturnCode_t connectNext(ConnectorProfile& connector_profile);


    /*!
     * @if jp
     *
     * @brief 次の Port に対して notify_disconnect() をコールする
     *
     * ConnectorProfile の port_ref 内に格納されている Port のオブジェクト
     * リファレンスのシーケンスの中から、自身の Port の次の Port に対して
     * notify_disconnect() をコールする。
     *
     * @param connector_profile 接続に関するプロファイル情報
     * @return ReturnCode_t 型のリターンコード
     *
     * @else
     *
     * @brief Call notify_disconnect() of the next Port
     *
     * This operation calls the notify_disconnect() of the next Port's 
     * that stored in ConnectorProfile's port_ref sequence.
     *
     * @param connector_profile The connection profile information
     * @return The return code of ReturnCode_t type.
     *
     * @endif
     */
    virtual ReturnCode_t disconnectNext(ConnectorProfile& connector_profile);


    /*! @if jp
     *
     * @brief Interface 情報を取得する
     *
     * このオペレーションは、notify_connect() 処理シーケンスの中間にコール
     * される純粋仮想関数である。
     * notify_connect() では、
     *
     * - publishInterfaces()
     * - connectNext()
     * - subscribeInterfaces()
     * - updateConnectorProfile()
     *
     * の順に protected 関数がコールされ接続処理が行われる。
     * <br>
     * 具象 Port ではこのオペレーションをオーバーライドし、引数として
     * 与えられた ConnectorProfile に従い処理を行い、パラメータが不適切
     * であれば、RteurnCode_t 型のエラーコードを返す。
     * 引数 ConnectorProfile には他の Port の Interface に関する情報が
     * 全て含まれている。
     * 通常 subscribeInterafaces() 内においては、この Port が使用する
     * Interface に関する情報を取得し、要求側のインターフェースに対して
     * 情報を設定しなければならない。
     * <br>
     * このオペレーションは、新規の connector_id に対しては接続の生成、
     * 既存の connector_id に対しては更新が適切に行われる必要がある。
     *
     * @param connector_profile 接続に関するプロファイル情報
     * @return ReturnCode_t 型のリターンコード
     *
     * @else
     *
     * @brief Publish interface information
     *
     * This operation is pure virutal method that would be called at the
     * mid-flow of the notify_connect() process sequence.
     * In the notify_connect(), the following methods would be called in order.
     *
     * - publishInterfaces()
     * - connectNext()
     * - subscribeInterfaces()
     * - updateConnectorProfile()
     *
     * In the concrete Port, this method should be overridden. This method
     * processes the given ConnectorProfile argument and if the given parameter
     * is invalid, it would return error code of ReturnCode_t.
     * The given argument ConnectorProfile includes all the interfaces
     * information in it.
     * Usually, subscribeInterafaces() method obtains information of interfaces
     * from ConnectorProfile, and should set it to the interfaces that require
     * them.
     * <br>
     * This operation should create the new connection for the new
     * connector_id, and should update the connection for the existing
     * connection_id.
     *
     * @param connector_profile The connection profile information
     * @return The return code of ReturnCode_t type.
     *
     * @endif
     */
    virtual ReturnCode_t
    subscribeInterfaces(const ConnectorProfile& connector_profile) = 0;
    
    
    /*! @if jp
     *
     * @brief Interface の接続を解除する
     *
     * このオペレーションは、notify_disconnect() 処理シーケンスの終わりにコール
     * される純粋仮想関数である。
     * notify_disconnect() では、
     * - disconnectNext()
     * - unsubscribeInterfaces()
     * - eraseConnectorProfile()
     * の順に protected 関数がコールされ接続解除処理が行われる。
     * <br>
     * 具象 Port ではこのオペレーションをオーバーライドし、引数として
     * 与えられた ConnectorProfile に従い接続解除処理を行う。
     *
     * @param connector_profile 接続に関するプロファイル情報
     *
     * @else
     *
     * @brief Disconnect interface connection
     *
     * This operation is pure virutal method that would be called at the
     * end of the notify_disconnect() process sequence.
     * In the notify_disconnect(), the following methods would be called.
     * - disconnectNext()
     * - unsubscribeInterfaces()
     * - eraseConnectorProfile() 
     *  <br>
     * In the concrete Port, this method should be overridden. This method
     * processes the given ConnectorProfile argument and disconnect interface
     * connection.
     *
     * @param connector_profile The connection profile information
     *
     * @endif
     */
    virtual void
    unsubscribeInterfaces(const ConnectorProfile& connector_profile) = 0;


    //============================================================
    // protected utility functions
    //============================================================
    /*!
     * @if jp
     *
     * @brief ConnectorProfile の connector_id フィールドが空かどうか判定
     *
     * @return 引数で与えられた ConnectorProfile の connector_id が空であれば、
     *         true、そうでなければ false を返す。
     *
     * @else
     *
     * @brief Whether connector_id of ConnectorProfile is empty
     *
     * @return If the given ConnectorProfile's connector_id is empty string,
     *         it returns true.
     *
     * @endif
     */
    bool isEmptyId(const ConnectorProfile& connector_profile) const;


    /*!
     * @if jp
     *
     * @brief UUIDを生成する
     *
     * このオペレーションは UUID を生成する。
     *
     * @return uuid
     *
     * @else
     *
     * @brief Get the UUID
     *
     * This operation generates UUID.
     *
     * @return uuid
     *
     * @endif
     */
    const std::string getUUID() const;


    /*!
     * @if jp
     *
     * @brief UUIDを生成し ConnectorProfile にセットする
     *
     * このオペレーションは UUID を生成し、ConnectorProfile にセットする。
     *
     * @param connector_profile connector_id をセットする ConnectorProfile
     *
     * @else
     *
     * @brief Create and set the UUID to the ConnectorProfile
     *
     * This operation generates and set UUID to the ConnectorProfile.
     *
     * @param connector_profile ConnectorProfile to be set connector_id
     *
     * @endif
     */
    void setUUID(ConnectorProfile& connector_profile) const;


    /*!
     * @if jp
     *
     * @brief id が既存の ConnectorProfile のものかどうか判定する
     *
     * このオペレーションは与えられた ID が既存の ConnectorProfile のリスト中に
     * 存在するかどうか判定する。
     *
     * @param id 判定する connector_id
     *
     * @else
     *
     * @brief Whether the given id exists in stored ConnectorProfiles
     *
     * This operation returns boolean whether the given id exists in 
     * the Port's ConnectorProfiles.
     *
     * @param id connector_id to be find in Port's ConnectorProfiles
     *
     * @endif
     */
    bool isExistingConnId(const char* id);


    /*!
     * @if jp
     *
     * @brief id を持つ ConnectorProfile を探す
     *
     * このオペレーションは与えられた ID を持つ ConnectorProfile を Port が
     * もつ ConnectorProfile のリスト中から探す。
     * もし、同一の id を持つ ConnectorProfile がなければ、空の ConnectorProfile
     * が返される。
     *
     * @param id 検索する connector_id
     * @return connector_id を持つ ConnectorProfile
     *
     * @else
     *
     * @brief Find ConnectorProfile with id
     *
     * This operation returns ConnectorProfile with the given id from Port's
     * ConnectorProfiles' list.
     * If the ConnectorProfile with connector id that is identical with the
     * given id does not exist, empty ConnectorProfile is returned.
     *
     * @param id the connector_id to be searched in Port's ConnectorProfiles
     * @return CoonectorProfile with connector_id
     *
     * @endif
     */
    ConnectorProfile findConnProfile(const char* id);
    

    /*!
     * @if jp
     *
     * @brief id を持つ ConnectorProfile を探す
     *
     * このオペレーションは与えられた ID を持つ ConnectorProfile を Port が
     * もつ ConnectorProfile のリスト中から探しインデックスを返す。
     * もし、同一の id を持つ ConnectorProfile がなければ、-1 を返す。
     *
     * @param id 検索する connector_id
     * @return Port の ConnectorProfile リストのインデックス
     *
     * @else
     *
     * @brief Find ConnectorProfile with id
     *
     * This operation returns ConnectorProfile with the given id from Port's
     * ConnectorProfiles' list.
     * If the ConnectorProfile with connector id that is identical with the
     * given id does not exist, empty ConnectorProfile is returned.
     *
     * @param id the connector_id to be searched in Port's ConnectorProfiles
     * @return The index of ConnectorProfile of the Port
     *
     * @endif
     */
    CORBA::Long findConnProfileIndex(const char* id);


    /*!
     * @if jp
     *
     * @brief ConnectorProfile の追加もしくは更新
     *
     * このオペレーションは与えられた与えられた ConnectorProfile を
     * Port に追加もしくは更新保存する。
     * 与えられた ConnectorProfile の connector_id と同じ ID を持つ
     * ConnectorProfile がリストになければ、リストに追加し、
     * 同じ ID が存在すれば ConnectorProfile を上書き保存する。
     *
     * @param coonector_profile 追加もしくは更新する ConnectorProfile
     *
     * @else
     *
     * @brief Append or update the ConnectorProfile list
     *
     * This operation appends or updates ConnectorProfile of the Port
     * by the given ConnectorProfile.
     * If the connector_id of the given ConnectorProfile does not exist
     * in the Port's ConnectorProfile list, the given ConnectorProfile would be
     * append to the list. If the same id exists, the list would be updated.
     *
     * @param connector_profile the ConnectorProfile to be appended or updated
     *
     * @endif
     */
    void updateConnectorProfile(const ConnectorProfile& connector_profile);


    /*!
     * @if jp
     *
     * @brief ConnectorProfile を削除する
     *
     * このオペレーションは Port の PortProfile が保持している
     * ConnectorProfileList のうち与えられた id を持つ ConnectorProfile
     * を削除する。
     *
     * @param id 削除する ConnectorProfile の id
     *
     * @else
     *
     * @brief Delete the ConnectorProfile
     *
     * This operation deletes a ConnectorProfile specified by id from
     * ConnectorProfileList owned by PortProfile of this Port.
     *
     * @param id The id of the ConnectorProfile to be deleted.
     *
     * @endif
     */
    bool eraseConnectorProfile(const char* id);


    /*!
     * @if jp
     *
     * @brief PortInterfaceProfile に インターフェースを登録する
     *
     * このオペレーションは Port が持つ PortProfile の、PortInterfaceProfile
     * にインターフェースの情報を追加する。
     * この情報は、get_port_profile() 似よって得られる PortProfile のうち
     * PortInterfaceProfile の値を変更するのみであり、実際にインターフェースを
     * 提供したり要求したりする場合には、サブクラスで、publishInterface(),
     * subscribeInterface() 等の関数を適切にオーバーライドしインターフェースの
     * 提供、要求処理を行わなければならない。
     *
     * インターフェース(のインスタンス)名は Port 内で一意でなければならない。
     * 同名のインターフェースがすでに登録されている場合、この関数は false を
     * 返す。
     *
     * @param name インターフェースのインスタンスの名前
     * @param type_name インターフェースの型の名前
     * @param pol インターフェースの属性 (RTC::PROVIDED もしくは RTC:REQUIRED)
     * @return 同名のインターフェースが既に登録されていれば false を返す。
     *
     * @else
     *
     * @brief Append an interface to the PortInterfaceProfile
     *
     * This operation appends interface information to the PortInterfaceProfile
     * that is owned by the Port.
     * The given interfaces information only updates PortInterfaceProfile of
     * PortProfile that is obtained through get_port_profile().
     * In order to provide and require interfaces, proper functions (for
     * example publishInterface(), subscribeInterface() and so on) should be
     * overridden in subclasses, and these functions provide concrete interface
     * connection and disconnection functionality.
     *
     * The interface (instance) name have to be unique in the Port.
     * If the given interface name is identical with stored interface name,
     * this function returns false.
     *
     * @param name The instance name of the interface.
     * @param type_name The type name of the interface.
     * @param pol The interface's polarity (RTC::PROVIDED or RTC:REQUIRED)
     * @return false would be returned if the same name is already registered.
     *
     * @endif
     */
    bool appendInterface(const char* name, const char* type_name,
			 PortInterfacePolarity pol);


    /*!
     * @if jp
     *
     * @brief PortInterfaceProfile からインターフェース登録を削除する
     *
     * このオペレーションは Port が持つ PortProfile の、PortInterfaceProfile
     * からインターフェースの情報を削除する。
     *
     * @param name インターフェースのインスタンスの名前
     * @param pol インターフェースの属性 (RTC::PROVIDED もしくは RTC:REQUIRED)
     * @return インターフェースが登録されていなければ false を返す。
     *
     * @else
     *
     * @brief Delete an interface from the PortInterfaceProfile
     *
     * This operation deletes interface information from the
     * PortInterfaceProfile that is owned by the Port.
     *
     * @param name The instance name of the interface.
     * @param pol The interface's polarity (RTC::PROVIDED or RTC:REQUIRED)
     * @return false would be returned if the given name is not registered.
     *
     * @endif
     */
    bool deleteInterface(const char* name, PortInterfacePolarity pol);
    

    /*!
     * @if jp
     *
     * @brief PortProfile の properties に NameValue 値を追加する
     *
     * @param ValueType properties の value に追加する値の型
     * @param key properties の name
     * @param value properties の value
     *
     * @else
     *
     * @brief Add NameValue data to PortProfile's properties
     *
     * @param ValueType The type of value
     * @param key The name of properties
     * @param value The value of properties
     *
     * @endif
     */
    template <class ValueType>
    void addProperty(const char* key, ValueType value)
    {
      CORBA_SeqUtil::push_back(m_profile.properties,
			       NVUtil::newNV(key, value));
    }


  protected:
    /*!
     * @if jp
     * @brief Port の PortProfile
     * @else
     * @brief PortProfile of the Port
     * @endif
     */
    PortProfile m_profile;
    RTC::Port_var m_objref;
    mutable ACE_Recursive_Thread_Mutex m_profile_mutex;
    typedef ACE_Guard<ACE_Recursive_Thread_Mutex> Guard;
    
    //============================================================
    // Functor
    //============================================================
    /*!
     * @if jp
     * @brief instance_name を持つ PortInterfaceProfile を探す Functor
     * @else
     * @brief A functor to find a PortInterfaceProfile named instance_name
     * @endif
     */
    struct if_name
    {
      if_name(const char* name) : m_name(name) {};
      bool operator()(const PortInterfaceProfile& prof)
      {
	return m_name == std::string(prof.instance_name);
      }
      std::string m_name;
    };
    
    
    /*!
     * @if jp
     * @brief id を持つ ConnectorProfile を探す Functor
     * @else
     * @brief A functor to find a ConnectorProfile named id
     * @endif
     */
    struct find_conn_id
    {
      find_conn_id(const char* id) : m_id(id) {};
      bool operator()(const ConnectorProfile& cprof)
      {
	return m_id == std::string(cprof.connector_id);
      }
      std::string m_id;
    };


    /*!
     * @if jp
     * @brief コンストラクタ引数 port_ref と同じオブジェクト参照を探す Functor
     * @else
     * @brief A functor to find the object reference that is identical port_ref
     * @endif
     */
    struct find_port_ref
    {
      find_port_ref(Port_ptr port_ref) : m_port(port_ref) {};
      bool operator()(Port_ptr port_ref)
      {
	return m_port->_is_equivalent(port_ref);
      }
      Port_ptr m_port;
    };
	
    
    /*!
     * @if jp
     * @brief Port の接続を行う Functor
     * @else
     * @brief A functor to connect Ports
     * @endif
     */
    struct connect_func
    {
      Port_var port_ref;
      ConnectorProfile connector_profile;
      ReturnCode_t return_code;
      
      connect_func() {};
      connect_func(Port_ptr p, ConnectorProfile& prof)
	: port_ref(p), connector_profile(prof) {};
      void operator()(Port_ptr p)
      {
	if (!port_ref->_is_equivalent(p))
	  {
	    ReturnCode_t retval;
	    retval = p->notify_connect(connector_profile);
	    if (retval != RTC::RTC_OK)
	      {
		return_code = retval;
	      }
	  }
      }
    };
    
    
    /*!
     * @if jp
     * @brief Port の接続解除を行う Functor
     * @else
     * @brief A functor to disconnect Ports
     * @endif
     */
    struct disconnect_func
    {
      Port_var port_ref;
      ConnectorProfile connector_profile;
      ReturnCode_t return_code;
      
      disconnect_func() : return_code(RTC::RTC_OK) {};
      disconnect_func(Port_ptr p, ConnectorProfile& prof)
	: port_ref(p), connector_profile(prof), return_code(RTC::RTC_OK) {};
      void operator()(Port_ptr p)
      {
	if (!port_ref->_is_equivalent(p))
	  {
	    ReturnCode_t retval;
	    retval = p->disconnect(connector_profile.connector_id);
	    if (retval != RTC::RTC_OK)
	      {
		return_code = retval;
	      }
	  }
      }
    };
    
    
    /*!
     * @if jp
     * @brief Port の全接続解除を行う Functor
     * @else
     * @brief A functor to disconnect all Ports
     * @endif
     */
    struct disconnect_all_func
    {
      ReturnCode_t return_code;
      PortBase* port;
      
      disconnect_all_func() {};
      disconnect_all_func(PortBase* p) 
	: return_code(RTC::RTC_OK), port(p) {};
      void operator()(ConnectorProfile& p)
      {
	ReturnCode_t retval;
	retval = port->disconnect(p.connector_id);
	if (retval != RTC::RTC_OK)
	  {
	    return_code = retval;
	  }
      }
    };
    

    /*!
     * @if jp
     * @brief name と polarity から interface を探す Functor
     * @else
     * @brief A functor to find interface from name and polarity
     * @endif
     */
    struct find_interface
    {
      find_interface(const char* name, PortInterfacePolarity pol)
	: m_name(name), m_pol(pol)
      {}

      bool operator()(const PortInterfaceProfile& prof)
      {
	std::string name(CORBA::string_dup(prof.instance_name));
	return ((m_name == name) && (m_pol == prof.polarity));
      }
      std::string m_name;
      PortInterfacePolarity m_pol;
    };
  };
};
#endif // PortBase_h

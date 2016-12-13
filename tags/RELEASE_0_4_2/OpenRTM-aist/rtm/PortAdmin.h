// -*- C++ -*-
/*!
 * @file PortAdmin.h
 * @brief RTC's Port administration class
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

#ifndef PortAdmin_h
#define PortAdmin_h

#include <rtm/idl/RTCSkel.h>
#include <rtm/PortBase.h>
#include <rtm/ObjectManager.h>

namespace RTC
{
  /*!
   * @if jp
   * @class PortAdmin
   * @brief PortAdmin クラス
   *
   * 各種 Port の管理を行うクラス。
   * Port の登録/登録解除など各種管理操作を実行するとともに、登録されている Port
   * Port の管理を行うクラス。
   *
   * @since 0.4.0
   *
   * @else
   * @class PortAdmin
   * @brief PortAdmin class
   *
   * This is a class to manage various Ports.
   * It executes various management operations such as registering and 
   * unregistering Port etc and also manages registered ports.
   *
   * @since 0.4.0
   *
   * @endif
   */
  class PortAdmin
  {
  public:
    /*!
     * @if jp
     * @brief コンストラクタ
     *
     * コンストラクタ
     *
     * @param orb ORB
     * @param poa POA
     *
     * @else
     * @brief Constructor
     *
     * Constructor
     *
     * @param orb ORB
     * @param poa POA
     *
     * @endif
     */
    PortAdmin(CORBA::ORB_ptr orb, PortableServer::POA_ptr poa);
    
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
    virtual ~PortAdmin(){};
    
    /*!
     * @if jp
     *
     * @brief Port リストの取得
     *
     * registerPort() により登録された Port の リストを取得する。
     *
     * @return Port リスト
     *
     * @else
     *
     * @brief Get PortList
     *
     * This operation returns the pointer to the PortList of Ports registered
     * by registerPort().
     *
     * @return The pointer points PortList
     *
     * @endif
     */
    PortList* getPortList() const;
    
    /*!
     * @if jp
     *
     * @brief PorProfile リストの取得
     *
     * registerPort() により登録された Port の Profile リストを取得する。
     *
     * @return PortProfile リスト
     *
     * @else
     *
     * @brief Get PorProfileList
     *
     * This operation gets the Profile list of Ports registered by 
     * registerPort().
     *
     * @return The pointer points PortProfile list
     *
     * @endif
     */
    PortProfileList getPortProfileList() const;
    
    /*!
     * @if jp
     *
     * @brief Port のオブジェクト参照の取得
     *
     * port_name で指定した Port のオブジェクト参照を返す。
     * port_name で指定する Port はあらかじめ registerPort() で登録されてい
     * なければならない。
     *
     * @param port_name 参照を返すPortの名前
     *
     * @return Port_ptr Portのオブジェクト参照
     *
     * @else
     *
     * @brief Get the reference to Port object
     *
     * This operation returns the reference of Port object specified
     * by port_name.
     * The port specified by port_name must be already registered in 
     * registerPort().
     *
     * @param port_name The name of Port to be returned the reference.
     *
     * @return Port_ptr Port object reference.
     *
     * @endif
     */
    Port_ptr getPortRef(const char* port_name) const;
    
    /*!
     * @if jp
     *
     * @brief Port のサーバントのポインタの取得
     *
     * port_name で指定した Port のサーバントのポインタを返す。
     * port_name で指定する Port はあらかじめ registerPort() で登録されてい
     * なければならない。
     *
     * @param port_name 参照を返すPortの名前
     *
     * @return PortBase* Portサーバント基底クラスのポインタ
     *
     * @else
     *
     * @brief Get pointer to the Port's servant
     *
     * This operation returns the pointer to the PortBase servant registered
     * by registerPort().
     * The port specified by port_name must be already registered in 
     * registerPort().
     *
     * @param port_name The name of Port to be returned the servant pointer.
     *
     * @return PortBase* The pointer to Port's servant.
     *
     * @endif
     */
    PortBase* getPort(const char* port_name) const;
    
    /*!
     * @if jp
     *
     * @brief Port を登録する
     *
     * 引数 port で指定された Port のサーバントを登録する。
     * 登録された Port のサーバントはコンストラクタで与えられたPOA 上で
     * activate され、そのオブジェクト参照はPortのProfileにセットされる。
     *
     * @param port Port サーバント
     *
     * @else
     *
     * @brief Regsiter the Port
     *
     * This operation registers the Port's servant given by argument.
     * The given Port's servant will be activated on the POA that is given
     * to the constructor, and the created object reference is set
     * to the Port's profile.
     *
     * @param port The Port's servant.
     *
     * @endif
     */
    void registerPort(PortBase& port);
    
    /*!
     * @if jp
     *
     * @brief Port の登録を解除する
     *
     * 引数 port で指定された Port の登録を解除する。
     * 削除時に Port は deactivate され、PortのProfileのリファレンスには、
     * nil値が代入される。
     *
     * @param port Port サーバント
     *
     * @else
     *
     * @brief Unregister the Port registration
     *
     * This operation unregisters the Port registration.
     * When the Port is unregistered, Port is deactivated, and the object
     * reference in the Port's profile is set to nil.
     *
     * @param port The Port's servant.
     *
     * @endif
     */
    void deletePort(PortBase& port);
    
    /*!
     * @if jp
     *
     * @brief 名称指定によりPort の登録を解除する
     *
     * 引数で指定された名前を持つ Port の登録を削除する。
     * 削除時に Port は deactivate され、PortのProfileのリファレンスには、
     * nil値が代入される。
     *
     * @param port_name Port の名前
     *
     * @else
     *
     * @brief Unregister the Port's registration by its name
     *
     * This operation unregister the Port's registration specified by port_ name.
     * When the Port is unregistered, Port is deactivated, and the object
     * reference in the Port's profile is set to nil.
     *
     * @param port_name The Port's name.
     *
     * @endif
     */
    void deletePortByName(const char* port_name);
    
    /*!
     * @if jp
     *
     * @brief 全ての Port をdeactivateし登録を削除する
     *
     * 登録されている全てのPortに対して、サーバントのdeactivateを行い、
     * 登録リストから削除する。
     *
     * @else
     *
     * @brief Deactivate all Ports and unregister them
     *
     * This operation deactivates the all Port and deletes the all Port's
     * registrations from the list.
     *
     * @endif
     */
    void finalizePorts();
    
  private:
    // ORB へのポインタ
    CORBA::ORB_var m_pORB;
    
    // POA へのポインタ
    PortableServer::POA_var m_pPOA;
    
    // Portのオブジェクトリファレンスのリスト. PortList
    PortList m_portRefs;
    
    
    template <class T>
    class comp_op
    {
    public:
      comp_op(const char* _name)
	: m_name(_name)
      {
      }
      comp_op(T* obj) 
	: m_name((const char*)(obj->getProfile().name))
      {
      }
      bool operator()(T* obj)
      {
	std::string name((const char*)obj->getProfile().name);
	return m_name == name;
      }
    private:
      std::string m_name;
    };
    
    class port_prof_collect
    {
    public:
      port_prof_collect(PortProfileList& p) : m_p(p) {}
      void operator()(const PortBase* port)
      {
	CORBA_SeqUtil::push_back(m_p, port->getPortProfile());
      }
    private:
      PortProfileList& m_p;
    };
    
    struct find_port_name;
    struct del_port;
    
    // サーバントを直接格納するオブジェクトマネージャ
    ObjectManager<const char*, PortBase, comp_op<PortBase> > m_portServants;
    
  };
};     // namespace RTC
#endif // PortAdmin_h

// -*- C++ -*-
/*!
 * @file   PortBaseTests.cpp
 * @brief  PortBase test class
 * @date   $Date: 2008/02/08 10:57:23 $
 * @author Shinji Kurihara
 *         Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2006
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

/*
 * $Log: PortBaseTests.cpp,v $
 * Revision 1.2  2008/02/08 10:57:23  arafune
 * Some tests were added.
 *
 * Revision 1.1  2007/12/20 07:50:17  arafune
 * *** empty log message ***
 *
 * Revision 1.3  2007/04/13 15:05:10  n-ando
 * Now RTC::OK becomes RTC::RTC_OK in RTC.idl.
 *
 * Revision 1.2  2007/01/12 14:44:43  n-ando
 * Some fixes for distribution control.
 *
 * Revision 1.1  2006/11/27 08:35:12  n-ando
 * TestSuites are devided into each directory.
 *
 * Revision 1.2  2006/11/13 12:30:06  kurihara
 *
 * document is added.
 *
 * Revision 1.1  2006/11/08 01:19:07  kurihara
 *
 * test program for PortBase class.
 *
 */

#ifndef PortBase_cpp
#define PortBase_cpp

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>

#include <vector>
#include <string>
#include <rtm/RTC.h>
#include <rtm/PortBase.h>
#include <rtm/RTObject.h>

/*!
 * @class PortBaseTests class
 * @brief PortBase test
 */
namespace PortBase
{
  class PortBaseMock : public RTC::PortBase
  {
  public:
		
    PortBaseMock(const RTC::PortProfile& profile)
    {
      this->m_profile = profile;
      this->m_profile.port_ref = this->m_objref;
    }

    const std::string getUUID() const
    {
      return RTC::PortBase::getUUID();
    }
		
    virtual RTC::ReturnCode_t notify_connect(RTC::ConnectorProfile& connector_profile)
      throw (CORBA::SystemException)
    {
      _notifyConnectTimes.push_back(getNow());
      return PortBase::notify_connect(connector_profile);
    }
		
    virtual RTC::ReturnCode_t notify_disconnect(const char* connector_id)
      throw (CORBA::SystemException)
    {
      _notifyDisconnectTimes.push_back(getNow());
      return PortBase::notify_disconnect(connector_id);
    }
		
  protected:
	
    virtual RTC::ReturnCode_t publishInterfaces(RTC::ConnectorProfile& connector_profile)
    {
      _publishIfsTimes.push_back(getNow());
      return RTC::RTC_OK;
    }
		
    virtual RTC::ReturnCode_t subscribeInterfaces(const RTC::ConnectorProfile& connector_profile)
    {
      _subscribeIfsTimes.push_back(getNow());
      return RTC::RTC_OK;
    }
		
    virtual void unsubscribeInterfaces(const RTC::ConnectorProfile& connector_profile)
    {
      _unsubscribeIfsTimes.push_back(getNow());
    }
	
  private:
	
    std::vector<timeval> _notifyConnectTimes;
    std::vector<timeval> _notifyDisconnectTimes;
    std::vector<timeval> _publishIfsTimes;
    std::vector<timeval> _subscribeIfsTimes;
    std::vector<timeval> _unsubscribeIfsTimes;
	
  private:
	
    timeval getNow() const
    {
      timeval now;
      gettimeofday(&now, 0);
      return now;
    }
		
  public:
	
    const std::vector<timeval>& getNotifyConnectTimes() const
    {
      return _notifyConnectTimes;
    }
		
    const std::vector<timeval>& getNotifyDisconnectTimes() const
    {
      return _notifyDisconnectTimes;
    }
		
    const std::vector<timeval>& getPublishIfsTimes() const
    {
      return _publishIfsTimes;
    }
		
    const std::vector<timeval>& getSubscribeIfsTimes() const
    {
      return _subscribeIfsTimes;
    }
		
    const std::vector<timeval>& getUnsubscribeIfsTimes() const
    {
      return _unsubscribeIfsTimes;
    }
  };

  
  int g_argc;
  std::vector<std::string> g_argv;

  class PortBaseTests
    : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(PortBaseTests);
		
    CPPUNIT_TEST(test_get_port_profile);
    CPPUNIT_TEST(test_getPortProfile);
    CPPUNIT_TEST(test_get_connector_profiles);
    CPPUNIT_TEST(test_get_connector_profile);
    CPPUNIT_TEST(test_connect);
    CPPUNIT_TEST(test_notify_connect);
    CPPUNIT_TEST(test_disconnect);
    CPPUNIT_TEST(test_setName);
    CPPUNIT_TEST(test_getProfile);
    CPPUNIT_TEST(test_setPortRef);
    CPPUNIT_TEST(test_getPortRef);
    CPPUNIT_TEST(test_getUUID);
		
    CPPUNIT_TEST_SUITE_END();
		
  private:
	
    CORBA::ORB_ptr m_orb;
    RTC::PortBase* m_pPortBase;

  public:
	
    /*!
     * @brief Constructor
     */
    PortBaseTests()
    {
    }
		
    /*!
     * @brief Destructor
     */
    ~PortBaseTests()
    {
    }
		
    /*!
     * @brief 初期化
     *    (1) ORBの初期化,POAのactivate
     *    (2) PortBaseのインスタンス生成
     *    (3) PortInterfaceProfileオブジェクト要素のセット
     *    (4) PortInterfaceProfileListオブジェクト要素
     *        (PortProfileの要素)のセット
     *    (5) ConnectorProfileオブジェクト要素のセット
     *    (6) ConnectorProfileListオブジェクト要素(PortProfileの要素)のセット
     *    (7) PortProfileオブジェクト要素のセット
     *    (8) PortProfileオブジェクトのセット
     */
    virtual void setUp()
    {
      char* argv[g_argc];
      for (int i = 0; i < g_argc; i++) {
	argv[i] = (char *)g_argv[i].c_str();
      }

      // ORBの初期化
      m_orb = CORBA::ORB_init(g_argc, argv);
      PortableServer::POA_ptr poa = PortableServer::POA::_narrow(
								 m_orb->resolve_initial_references("RootPOA"));
			
      // PortProfile.interfacesの構築準備
      RTC::PortInterfaceProfile portIfProfile;
      portIfProfile.instance_name = "PortInterfaceProfile-instance_name";
      portIfProfile.type_name = "PortInterfaceProfile-type_name";
      portIfProfile.polarity = RTC::REQUIRED;

      RTC::PortInterfaceProfileList portIfProfiles;
      portIfProfiles.length(1);
      portIfProfiles[0] = portIfProfile;

      // PortProfile.connector_profilesの構築準備
      SDOPackage::NameValue connProfileProperty;
      connProfileProperty.name = "ConnectorProfile-properties0-name";
      connProfileProperty.value <<= CORBA::Float(1.1);
			
      SDOPackage::NVList connProfileProperties;
      connProfileProperties.length(1);
      connProfileProperties[0] = connProfileProperty;
      RTC::ConnectorProfile connProfile;
      connProfile.name = "ConnectorProfile-name";
      connProfile.connector_id = "connect_id0";
      connProfile.properties = connProfileProperties;

      RTC::ConnectorProfileList connProfiles;
      connProfiles.length(1);
      connProfiles[0] = connProfile;

      // PortProfile.propertiesの構築準備
      SDOPackage::NameValue portProfileProperty;
      portProfileProperty.name = "PortProfile-properties0-name";
      portProfileProperty.value <<= CORBA::Float(2.2);
      SDOPackage::NVList portProfileProperties;
      portProfileProperties.length(1);
      portProfileProperties[0] = portProfileProperty;

      // PortProfileを構築する
      RTC::PortProfile portProfile;
      portProfile.name = "inport0";
      portProfile.interfaces = portIfProfiles;
      portProfile.connector_profiles = connProfiles;
      portProfile.properties = portProfileProperties;

      // PortBaseのインスタンスを生成する
      m_pPortBase = new PortBaseMock(portProfile);

      // POAを活性化する			
      PortableServer::POAManager_var poaMgr = poa->the_POAManager();
      poaMgr->activate();
    }
		
    /*!
     * @brief Test finalization
     */
    virtual void tearDown()
    {
      //if (m_orb != 0) {
      // m_orb->destroy();
      // m_orb = 0;
      // m_pPortBase = 0;
      // }
    }
		
    /*!
     * @brief get_port_profile()メソッドのテスト
     * 
     * - オブジェクト参照経由で、get_port_profile()に正しくアクセスできるか？
     * - PortProfile.nameを正しく取得できるか？
     * - PortProfile.interfaceを正しく取得できるか？
     * - PortProfile.connector_profilesを正しく取得できるか？
     * - PortProfile.propertiesを正しく取得できるか？
     */
    void test_get_port_profile()
    {
      // (1) オブジェクト参照経由で、get_port_profile()に正しくアクセスできるか？
      // get_port_profile()はCORBAインタフェースなので、オブジェクト参照経由でアクセスし、
      // CORBAインタフェースとして機能していることを確認する
      const RTC::PortService_ptr portRef = m_pPortBase->getPortRef();
      const RTC::PortProfile* pPortProfile = portRef->get_port_profile();
			
      // (2) PortProfile.nameを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("inport0"),
			   std::string(pPortProfile->name));

      // (3) PortProfile.interfaceを正しく取得できるか？
      const RTC::PortInterfaceProfile& portIfProfile = pPortProfile->interfaces[0];
      // (3-a) PortInterfaceProfile.instance_nameを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("PortInterfaceProfile-instance_name"),
			   std::string(portIfProfile.instance_name));

      // (3-b) PortInterfaceProfile.type_nameを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("PortInterfaceProfile-type_name"),
			   std::string(portIfProfile.type_name));

      // (3-c) PortInterfaceProfile.polarityを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(RTC::REQUIRED, portIfProfile.polarity);

      // (4) PortProfile.connector_profilesを正しく取得できるか？
      const RTC::ConnectorProfile& connProfile = pPortProfile->connector_profiles[0];
      // (4-a) ConnectorProfile.nameを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("ConnectorProfile-name"),
			   std::string(connProfile.name));
      
      // (4-b) ConnectorProfile.connector_idを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("connect_id0"),
			   std::string(connProfile.connector_id));
			
      // (4-c) ConnectorProfile.propertiesを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("ConnectorProfile-properties0-name"),
			   std::string(connProfile.properties[0].name));
			
      {
	CORBA::Float value;
	connProfile.properties[0].value >>= value;
	CPPUNIT_ASSERT_EQUAL(CORBA::Float(1.1), value);
      }
			
      // (5) PortProfile.propertiesを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("PortProfile-properties0-name"),
			   std::string(pPortProfile->properties[0].name));

      {
	CORBA::Float value;
	pPortProfile->properties[0].value >>= value;
	CPPUNIT_ASSERT_EQUAL(CORBA::Float(2.2), value);
      }
    }
		
    /*!
     * @brief getPortProfile()メソッドのテスト
     * 
     * - PortProfile.nameを正しく取得できるか？
     * - PortProfile.interfaceを正しく取得できるか？
     * - PortProfile.connector_profilesを正しく取得できるか？
     * - PortProfile.propertiesを正しく取得できるか？
     */
    void test_getPortProfile()
    {
      const RTC::PortProfile& portProfile = m_pPortBase->getPortProfile();
			
      // (1) PortProfile.nameを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("inport0"),
			   std::string(portProfile.name));

      // (2) PortProfile.interfaceを正しく取得できるか？
      const RTC::PortInterfaceProfile& portIfProfile = portProfile.interfaces[0];
      // (2-a) PortInterfaceProfile.instance_nameを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("PortInterfaceProfile-instance_name"),
			   std::string(portIfProfile.instance_name));

      // (2-b) PortInterfaceProfile.type_nameを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("PortInterfaceProfile-type_name"),
			   std::string(portIfProfile.type_name));

      // (2-c) PortInterfaceProfile.polarityを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(RTC::REQUIRED, portIfProfile.polarity);

      // (3) PortProfile.connector_profilesを正しく取得できるか？
      const RTC::ConnectorProfile& connProfile = portProfile.connector_profiles[0];
      // (3-a) ConnectorProfile.nameを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("ConnectorProfile-name"),
			   std::string(connProfile.name));
      
      // (3-b) ConnectorProfile.connector_idを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("connect_id0"),
			   std::string(connProfile.connector_id));
			
      // (3-c) ConnectorProfile.propertiesを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("ConnectorProfile-properties0-name"),
			   std::string(connProfile.properties[0].name));
			
      {
	CORBA::Float value;
	connProfile.properties[0].value >>= value;
	CPPUNIT_ASSERT_EQUAL(CORBA::Float(1.1), value);
      }
			
      // (4) PortProfile.propertiesを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("PortProfile-properties0-name"),
			   std::string(portProfile.properties[0].name));

      {
	CORBA::Float value;
	portProfile.properties[0].value >>= value;
	CPPUNIT_ASSERT_EQUAL(CORBA::Float(2.2), value);
      }
    }
		
    /*!
     * @brief get_connector_profiles()メソッドのテスト
     * 
     * - オブジェクト参照経由で、get_connector_profiles()に正しくアクセスできるか？
     * - ConnectorProfile.nameを正しく取得できるか？
     * - ConnectorProfile.connector_idを正しく取得できるか？
     * - ConnectorProfile.propertiesを正しく取得できるか？
     */
    void test_get_connector_profiles()
    {
      // (1) オブジェクト参照経由で、get_connector_profiles()に正しくアクセスできるか？
      // get_connector_profiles()はCORBAインタフェースなので、オブジェクト参照経由でアクセスし、
      // CORBAインタフェースとして機能していることを確認する
      const RTC::PortService_ptr portRef = m_pPortBase->getPortRef();
      const RTC::ConnectorProfileList* pConnProfList = portRef->get_connector_profiles();

      // ConnectorProfileList内のConnectorProfileのチェック
      const RTC::ConnectorProfile& connProfile = (*pConnProfList)[0];
      // (2) ConnectorProfile.nameを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("ConnectorProfile-name"),
			   std::string(connProfile.name));
			
      // (3) ConnectorProfile.connector_idを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("connect_id0"),
			   std::string(connProfile.connector_id));

      // (4) ConnectorProfile.propertiesを正しく取得できるか？
      const SDOPackage::NameValue& property = connProfile.properties[0];
      // (4-a) nameを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("ConnectorProfile-properties0-name"),
			   std::string(property.name));
			
      // (4-b) valueを正しく取得できるか？
      {
	CORBA::Float value;
	property.value >>= value;
	CPPUNIT_ASSERT_EQUAL(CORBA::Float(1.1), value);
      }
    }
		
    /*!
     * @brief get_connector_profile()メソッドのテスト
     * 
     * - オブジェクト参照経由で、get_connector_profile()に正しくアクセスできるか？
     * - ConnectorProfile.nameを正しく取得できるか？
     * - ConnectorProfile.connector_idを正しく取得できるか？
     * - ConnectorProfile.propertiesを正しく取得できるか？
     */
    void test_get_connector_profile()
    {
      // (1) オブジェクト参照経由で、get_connector_profile()に正しくアクセスできるか？
      // get_connector_profile()はCORBAインタフェースなので、オブジェクト参照経由でアクセスし、
      // CORBAインタフェースとして機能していることを確認する
      const RTC::PortService_ptr portRef = m_pPortBase->getPortRef();
      const RTC::ConnectorProfile* pConnProfile = portRef->get_connector_profile("connect_id0");

      // (2) ConnectorProfile.nameを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("ConnectorProfile-name"),
			   std::string(pConnProfile->name));

      // (3) ConnectorProfile.connector_idを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("connect_id0"),
			   std::string(pConnProfile->connector_id));

      // (4) ConnectorProfile.propertiesを正しく取得できるか？
      const SDOPackage::NameValue& property = pConnProfile->properties[0];
      // (4-a) nameを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("ConnectorProfile-properties0-name"),
			   std::string(property.name));

      // (4-b) valueを正しく取得できるか？
      {
	CORBA::Float value;
	property.value >>= value;
	CPPUNIT_ASSERT_EQUAL(CORBA::Float(1.1), value);
      }
    }
		
    /*!
     * @brief connect()メソッドのテスト
     * 
     * - オブジェクト参照経由で、connect()に正しくアクセスできるか？
     * - 接続が成功するか？
     * - 接続時にnotify_connect()が意図どおりに１回だけ呼び出されたか？
     */
    void test_connect()
    {
      // (1) オブジェクト参照経由で、connect()に正しくアクセスできるか？
      // connect()はCORBAインタフェースなので、オブジェクト参照経由でアクセスし、
      // CORBAインタフェースとして機能していることを確認する
      RTC::PortService_ptr portRef = m_pPortBase->getPortRef();
			
      // 接続時に必要となるConnectorProfileを構築する
      RTC::ConnectorProfile connProfile;
      connProfile.name = "ConnectorProfile-name";
      connProfile.connector_id = "connect_id0";
      connProfile.ports.length(1);
      connProfile.ports[0] = portRef;

      // (2) 接続が成功するか？
      CPPUNIT_ASSERT_EQUAL(RTC::RTC_OK, portRef->connect(connProfile));
      
      // (3) 接続時にnotify_connect()が意図どおりに１回だけ呼び出されたか？
      const PortBaseMock* pPortBaseMock = dynamic_cast<const PortBaseMock*>(m_pPortBase);
      CPPUNIT_ASSERT(pPortBaseMock != 0);
      CPPUNIT_ASSERT_EQUAL(1, (int) pPortBaseMock->getNotifyConnectTimes().size());
    }
		
    /*!
     * @brief notify_connect()メソッドのテスト
     */
    void test_notify_connect()
    {
      // notify_connect()メソッドは、test_connectにて間接的にテストされているので、ここではテスト不要である
    }
		
    /*!
     * @brief disconnect()メソッドのテスト
     * 
     * - オブジェクト参照経由で、disconnect()に正しくアクセスできるか？
     * - 切断が成功するか？
     * - 切断時にnotify_disconnect()が、意図どおり１回だけ呼び出されているか？
     */
    void test_disconnect()
    {
      // (1) オブジェクト参照経由で、disconnect()に正しくアクセスできるか？
      // disconnect()はCORBAインタフェースなので、オブジェクト参照経由でアクセスし、
      // CORBAインタフェースとして機能していることを確認する
      RTC::PortService_ptr portRef = m_pPortBase->getPortRef();
			
      // 接続時に必要となるConnectorProfileを構築する
      RTC::ConnectorProfile connProfile;
      connProfile.name = "ConnectorProfile-name";
      connProfile.connector_id = "connect_id0";
      connProfile.ports.length(1);
      connProfile.ports[0] = portRef;

      // まずは接続する
      CPPUNIT_ASSERT_EQUAL(RTC::RTC_OK, portRef->connect(connProfile));
      
      // (2) 切断が成功するか？
      CPPUNIT_ASSERT_EQUAL(RTC::RTC_OK, portRef->disconnect(connProfile.connector_id));
      
      // (3) 切断時にnotify_disconnect()が、意図どおり１回だけ呼び出されているか？
      const PortBaseMock* pPortBaseMock = dynamic_cast<const PortBaseMock*>(m_pPortBase);
      CPPUNIT_ASSERT(pPortBaseMock != 0);
      CPPUNIT_ASSERT_EQUAL(1, (int) pPortBaseMock->getNotifyDisconnectTimes().size());
    }
		
    void test_disconnect_all()
    {
      // TODO 未テスト
    }
		
    /*!
     * @brief setName()メソッドのテスト
     * 
     * - setName()により、意図どおりにPortProfile.nameが書き換えられているか？
     */
    void test_setName()
    {
      // setName()を用いて、PortProfile.nameを書き換える
      m_pPortBase->setName("inport0-changed");
			
      // setName()により、意図どおりにPortProfile.nameが書き換えられているか？
      const RTC::PortProfile& portProfile = m_pPortBase->getPortProfile();
      CPPUNIT_ASSERT_EQUAL(std::string("inport0-changed"), std::string(portProfile.name));
    }
		
    /*!
     * @brief getProfile()メソッドのテスト
     * 
     * - PortProfile.nameを正しく取得できるか？
     * - PortProfile.interfacesを正しく取得できるか？
     * - PortProfile.connector_profilesを正しく取得できるか？
     * - PortProfile.propertiesを正しく取得できるか？
     */
    void test_getProfile()
    {
      const RTC::PortProfile& portProfile = m_pPortBase->getProfile();

      // (1) PortProfile.nameを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(std::string("inport0"), std::string(portProfile.name));

      // (2) PortProfile.interfacesを正しく取得できるか？
      const RTC::PortInterfaceProfile& portIfProfile = portProfile.interfaces[0];
      // (2-a) PortInterfaceProfile.instance_nameを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("PortInterfaceProfile-instance_name"),
			   std::string(portIfProfile.instance_name));

      // (2-b) PortInterfaceProfile.type_nameを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("PortInterfaceProfile-type_name"),
			   std::string(portIfProfile.type_name));
			
      // (2-c) PortInterfaceProfile.polarityを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(RTC::REQUIRED, portIfProfile.polarity);

      // (3) PortProfile.connector_profilesを正しく取得できるか？
      const RTC::ConnectorProfile& connProfile = portProfile.connector_profiles[0];
      // (3-a) ConnectorProfile.nameを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("ConnectorProfile-name"),
			   std::string(connProfile.name));
			
      // (3-b) ConnectorProfile.connector_idを正しく取得できるか？
      CPPUNIT_ASSERT_EQUAL(
			   std::string("connect_id0"),
			   std::string(connProfile.connector_id));

      // (3-c) ConnectorPofile.propertiesを正しく取得できるか？
      {
	const SDOPackage::NameValue& property = connProfile.properties[0];
	// (3-c-1) nameを正しく取得できるか？
	CPPUNIT_ASSERT_EQUAL(
			     std::string("ConnectorProfile-properties0-name"),
			     std::string(property.name));
			
	// (3-c-2) valueを正しく取得できるか？
	{
	  CORBA::Float value;
	  property.value >>= value;
	  CPPUNIT_ASSERT_EQUAL(CORBA::Float(1.1), value);
	}
      }

      // (4) PortProfile.propertiesを正しく取得できるか？
      {
	const SDOPackage::NameValue& property = portProfile.properties[0];
	// (4-a) nameを正しく取得できるか？
	CPPUNIT_ASSERT_EQUAL(
			     std::string("PortProfile-properties0-name"),
			     std::string(property.name));
				
	// (4-b) valueを正しく取得できるか？
	{
	  CORBA::Float value;
	  property.value >>= value;
	  CPPUNIT_ASSERT_EQUAL(CORBA::Float(2.2), value);
	}
      }
    }
		
    /*!
     * @brief setPortRef()メソッドのテスト
     * 
     * - setPortRef()を用いて、PortBaseオブジェクト参照を正しく設定できるか？
     */
    void test_setPortRef()
    {
      // 一旦、設定済みのPortBaseオブジェクト参照をリセットしておく
      m_pPortBase->_remove_ref();

      // setPortRef()を用いて、PortBaseオブジェクト参照を正しく設定できるか？
      // (getPortRef()を用いて、Portインタフェースのオブジェクト参照を取得し、
      // 取得した参照が、あらかじめ設定した参照と一致することを確認する)
      RTC::PortService_var port = m_pPortBase->_this();
      RTC::PortService_ptr portRef = port._retn();
      m_pPortBase->setPortRef(portRef);

      CPPUNIT_ASSERT_EQUAL(portRef, m_pPortBase->getPortRef());
    }

    /*!
     * @brief getPortRef()メソッドのテスト
     */
    void test_getPortRef()
    {
      // test_setPortRef()によりテストされている
    }
		
    /*!
     * @brief getUUID()メソッドのテスト
     * 
     * - UUIDを取得できるか？（空文字列でないかどうかのみでチェック）
     */
    void test_getUUID()
    {
      // getUUID()メソッドはprotectedであるため、PortBaseMockにダウンキャストしてからアクセスする
      PortBaseMock* pPortBase = dynamic_cast<PortBaseMock*>(m_pPortBase);
      CPPUNIT_ASSERT(pPortBase != 0);
			
      // UUIDを取得できるか？（空文字列でないかどうかのみでチェック）
      std::string uuid = pPortBase->getUUID();
      CPPUNIT_ASSERT(uuid.length() > 0);
      //std::cout << std::endl << "uuid: " << uuid << std::endl;
    }
		
  };
}; // namespace PortBase

/*
 * Register test suite
 */
CPPUNIT_TEST_SUITE_REGISTRATION(PortBase::PortBaseTests);

#ifdef LOCAL_MAIN
int main(int argc, char* argv[])
{

  FORMAT format = TEXT_OUT;
  int target = 0;
  std::string xsl;
  std::string ns;
  std::string fname;
  std::ofstream ofs;

  int i(1);
  while (i < argc)
    {
      std::string arg(argv[i]);
      std::string next_arg;
      if (i + 1 < argc) next_arg = argv[i + 1];
      else              next_arg = "";

      if (arg == "--text") { format = TEXT_OUT; break; }
      if (arg == "--xml")
	{
	  if (next_arg == "")
	    {
	      fname = argv[0];
	      fname += ".xml";
	    }
	  else
	    {
	      fname = next_arg;
	    }
	  format = XML_OUT;
	  ofs.open(fname.c_str());
	}
      if ( arg == "--compiler"  ) { format = COMPILER_OUT; break; }
      if ( arg == "--cerr"      ) { target = 1; break; }
      if ( arg == "--xsl"       )
	{
	  if (next_arg == "") xsl = "default.xsl"; 
	  else                xsl = next_arg;
	}
      if ( arg == "--namespace" )
	{
	  if (next_arg == "")
	    {
	      std::cerr << "no namespace specified" << std::endl;
	      exit(1); 
	    }
	  else
	    {
	      xsl = next_arg;
	    }
	}
      ++i;
    }
  CppUnit::TextUi::TestRunner runner;
  if ( ns.empty() )
    runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
  else
    runner.addTest(CppUnit::TestFactoryRegistry::getRegistry(ns).makeTest());
  CppUnit::Outputter* outputter = 0;
  std::ostream* stream = target ? &std::cerr : &std::cout;
  switch ( format )
    {
    case TEXT_OUT :
      outputter = new CppUnit::TextOutputter(&runner.result(),*stream);
      break;
    case XML_OUT :
      std::cout << "XML_OUT" << std::endl;
      outputter = new CppUnit::XmlOutputter(&runner.result(),
					    ofs, "shift_jis");
      static_cast<CppUnit::XmlOutputter*>(outputter)->setStyleSheet(xsl);
      break;
    case COMPILER_OUT :
      outputter = new CppUnit::CompilerOutputter(&runner.result(),*stream);
      break;
    }
  runner.setOutputter(outputter);
  runner.run();
  return 0; // runner.run() ? 0 : 1;
}
#endif // MAIN
#endif // PortBase_cpp

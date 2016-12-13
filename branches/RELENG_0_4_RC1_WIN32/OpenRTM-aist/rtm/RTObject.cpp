// -*- C++ -*-
/*!
 * @file RTObject.cpp
 * @brief RT component base class
 * @date $Date: 2007-01-21 10:36:39 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2006
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id: RTObject.cpp,v 1.4 2007-01-21 10:36:39 n-ando Exp $
 *
 */

/*
 * $Log: not supported by cvs2svn $
 * Revision 1.3  2007/01/14 19:46:09  n-ando
 * The component action implementation functions for Users' business logic
 * were added (i.e onInitialize(), onExecute(), etc..)
 *
 * Revision 1.2  2007/01/09 15:21:40  n-ando
 * SDO interfaces are marged.
 * Some RTObject's operation signatures were changed.
 *
 * Revision 1.1  2006/09/11 19:04:29  n-ando
 * The first commit.
 *
 *
 */

#include <assert.h>
#include "rtm/RTObject.h"
#include "rtm/SdoConfiguration.h"
#include "rtm/CORBA_SeqUtil.h"
#include "rtm/Manager.h"
#include <rtm/StringUtil.h>

namespace RTC
{
  RTObject_impl::RTObject_impl(Manager* manager)
    : m_portAdmin(manager->getORB(), manager->getPOA()),
      m_created(true), m_alive(false)
  {


  }

  RTObject_impl::RTObject_impl(CORBA::ORB_ptr orb,
			       PortableServer::POA_ptr poa)
    : m_portAdmin(orb, poa),
      m_created(true), m_alive(false)
  {


  }

  RTObject_impl::~RTObject_impl()
  {
  }


    //============================================================
    // Overridden functions
    //============================================================
    // The initialize action (on CREATED->ALIVE transition)
    // formaer rtc_init_entry() 
    ReturnCode_t RTObject_impl::onInitialize()
    {
      return RTC::OK;
    }

    // The finalize action (on ALIVE->END transition)
    // formaer rtc_exiting_entry()
    ReturnCode_t RTObject_impl::onFinalize()
    {
      return RTC::OK;
    }
    
    // The startup action when ExecutionContext startup
    // former rtc_starting_entry()
    ReturnCode_t RTObject_impl::onStartup(RTC::UniqueId ec_id)
    {
      return RTC::OK;
    }
    
    // The shutdown action when ExecutionContext stop
    // former rtc_stopping_entry()
    ReturnCode_t RTObject_impl::onShutdown(RTC::UniqueId ec_id)
    {
      return RTC::OK;
    }
    
    // The activated action (Active state entry action)
    // former rtc_active_entry()
    ReturnCode_t RTObject_impl::onActivated(RTC::UniqueId ec_id)
    {
      return RTC::OK;
    }

    // The deactivated action (Active state exit action)
    // former rtc_active_exit()
    ReturnCode_t RTObject_impl::onDeactivated(RTC::UniqueId ec_id)
    {
      return RTC::OK;
    }
    
    // The execution action that is invoked periodically
    // former rtc_active_do()
    ReturnCode_t RTObject_impl::onExecute(RTC::UniqueId ec_id)
    {
      return RTC::OK;
    }
    
    // The aborting action when main logic error occurred.
    // former rtc_aborting_entry()
    ReturnCode_t RTObject_impl::onAborting(RTC::UniqueId ec_id)
    {
      return RTC::OK;
    }
    
    // The error action in ERROR state
    // former rtc_error_do()
    ReturnCode_t RTObject_impl::onError(RTC::UniqueId ec_id)
    {
      return RTC::OK;
    }
    
    // The reset action that is invoked resetting
    // This is same but different the former rtc_init_entry()
    ReturnCode_t RTObject_impl::onReset(RTC::UniqueId ec_id)
    {
      return RTC::OK;
    }
    
    // The state update action that is invoked after onExecute() action
    // no corresponding operation exists in OpenRTm-aist-0.2.0
    ReturnCode_t RTObject_impl::onStateUpdate(RTC::UniqueId ec_id)
    {
      return RTC::OK;
    }

    // The action that is invoked when execution context's rate is changed
    // no corresponding operation exists in OpenRTm-aist-0.2.0
    ReturnCode_t RTObject_impl::onRateChanged(RTC::UniqueId ec_id)
    {
      return RTC::OK;
    }


  //============================================================
  // RTC::LightweightRTObject
  //============================================================
  /*!
   * @if jp
   * @breif [CORBA interface] RTCを初期化する
   * @else
   * @breif [CORBA interface] Initialize the RTC
   * @endif
   */
  ReturnCode_t RTObject_impl::initialize()
  {
    ReturnCode_t ret;
    ret = on_initialize();
    m_created = false;

    if (ret == RTC::OK)
      {
	if (m_execContexts.length() > 0)
	  {
	    m_execContexts[0]->start();
	    m_alive = true;
	  }
      }
    return ret;
  }

  /*!
   * @if jp
   * @brief [CORBA interface] RTC 終了処理
   * @else
   * @brief [CORBA interface] Finalize the RTC
   * @endif
   */
  ReturnCode_t RTObject_impl::finalize()
  {
    if (m_created) return RTC::PRECONDITION_NOT_MET;

    for (CORBA::ULong i(0), n(m_execContexts.length()); i < n; ++i)
      {
	if (m_execContexts[i]->is_running())
	  return RTC::PRECONDITION_NOT_MET;
      }

    ReturnCode_t ret;
    ret = on_finalize();
    return ret;
  }


  /*!
   * @if jp
   * @brief [CORBA interface] RTC を停止終了させる
   * @else
   * @brief [CORBA interface]　Stop the RTC's and finalize
   * @endif
   */
  ReturnCode_t RTObject_impl::exit()
  {
    if (m_execContexts.length() > 0)
      {
	m_execContexts[0]->stop();
	m_alive = false;
      }
    CORBA_SeqUtil::for_each(m_execContexts,
	    deactivate_comps(RTC::LightweightRTObject::_duplicate(m_objref)));
    return finalize();
  }


  /*!
   * @if jp
   * @brief [CORBA interface] RTC が alive 状態かどうか
   * @else
   * @brief [CORBA interface] If RTC is alive state
   * @endif
   */
  CORBA::Boolean RTObject_impl::is_alive()
  {
    return m_alive;
  }


  /*!
   * @if jp
   * @brief [CORBA interface] ExecutionContextListを取得する
   * @else
   * @brief [CORBA interface] Get ExecutionContextList.
   * @endif
   */
  ExecutionContextList* RTObject_impl::get_contexts()
  {
    ExecutionContextList_var execlist;
    execlist = new ExecutionContextList();

    CORBA_SeqUtil::for_each(m_execContexts, ec_copy(execlist));

    return execlist._retn();
  }


  UniqueId 
  RTObject_impl::
  set_execution_context_service(const ExecutionContextService_ptr ec)
  {
    CORBA_SeqUtil::
      push_back(m_execContexts, ExecutionContextService::_duplicate(ec));

    return m_execContexts.length() - 1;
  }

  //============================================================
  // RTC::RTObject
  //============================================================

  /*!
   * @if jp
   * @brief [RTCObject interface] コンポーネントプロファイルの取得
   * @else
   * @brief [RTCObject interface] Getting RTC's profile
   * @endif
   */
  ComponentProfile* RTObject_impl::get_component_profile()
  {
    try
      {
	ComponentProfile_var profile
	  = new ComponentProfile(m_profile);
	return profile._retn();
      }
    catch (...)
      {
	; // This operation throws no exception.
      }
    assert(false);
    return 0;
  }


  /*!
   * @if jp
   * @brief [RTCObject interface] ポートの取得
   * @else
   * @brief [RTCObject interface] Get Ports
   * @endif
   */
  PortList* RTObject_impl::get_ports()
  {
    try
      {
	return m_portAdmin.getPortList();
      }
    catch (...)
      {
	; // This operation throws no exception.
      }
    assert(false);
    return 0;
  }


  /*!
   * @if jp
   * @brief [RTCObject interface] ExecutionContextAdmin の取得
   * @else
   * @brief [RTCObject interface] Get ExecutionContextAdmin
   * @endif
   */
  ExecutionContextServiceList* RTObject_impl::get_execution_context_services()
  {
    try
      {
	ExecutionContextServiceList_var exec_context;
	exec_context = new ExecutionContextServiceList(m_execContexts);
	return exec_context._retn();
      }
    catch (...)
      {
	; // This operation throws no exception.
      }
    assert(false);
    return 0;
  }



  //============================================================
  // RTC::ComponentAction
  //============================================================
  /*!
   * @if jp
   * @brief [ComponentAction interface]
   * @else
   * @brief [ComponentAction interface]
   * @endif
   */
  ReturnCode_t RTObject_impl::on_initialize()
  {
    ReturnCode_t ret(RTC::ERROR);
    try
      {
	ret = onInitialize();
      }
    catch (...)
      {
	return RTC::ERROR;
      }
    return ret;
  }

  /*!
   * @if jp
   * @brief [ComponentAction interface]
   * @else
   * @brief [ComponentAction interface]
   * @endif
   */
  ReturnCode_t RTObject_impl::on_finalize()
  {
    ReturnCode_t ret(RTC::ERROR);
    try
      {
	ret = onFinalize();
      }
    catch (...)
      {
	return RTC::ERROR;
      }
    return ret;
  }

  /*!
   * @if jp
   * @brief [ComponentAction interface]
   * @else
   * @brief [ComponentAction interface]
   * @endif
   */
  ReturnCode_t RTObject_impl::on_startup(UniqueId ec_id)
  {
    ReturnCode_t ret(RTC::ERROR);
    try
      {
	ret = onStartup(ec_id);
      }
    catch (...)
      {
	return RTC::ERROR;
      }
    return ret;
  }

  /*!
   * @if jp
   * @brief [ComponentAction interface]
   * @else
   * @brief [ComponentAction interface]
   * @endif
   */
  ReturnCode_t RTObject_impl::on_shutdown(UniqueId ec_id)
  {
    ReturnCode_t ret(RTC::ERROR);
    try
      {
	ret = onShutdown(ec_id);
      }
    catch (...)
      {
	return RTC::ERROR;
      }
    return ret;
  }

  /*!
   * @if jp
   * @brief [ComponentAction interface]
   * @else
   * @brief [ComponentAction interface]
   * @endif
   */
  ReturnCode_t RTObject_impl::on_activated(UniqueId ec_id)
  {
    ReturnCode_t ret(RTC::ERROR);
    try
      {
	ret = onActivated(ec_id);
      }
    catch (...)
      {
	return RTC::ERROR;
      }
    return ret;
  }

  /*!
   * @if jp
   * @brief [ComponentAction interface]
   * @else
   * @brief [ComponentAction interface]
   * @endif
   */
  ReturnCode_t RTObject_impl::on_deactivated(UniqueId ec_id)
  {
    ReturnCode_t ret(RTC::ERROR);
    try
      {
	ret = onDeactivated(ec_id);
      }
    catch (...)
      {
	return RTC::ERROR;
      }
    return ret;
  }

  /*!
   * @if jp
   * @brief [ComponentAction interface]
   * @else
   * @brief [ComponentAction interface]
   * @endif
   */
  ReturnCode_t RTObject_impl::on_aborting(UniqueId ec_id)
  {
    ReturnCode_t ret(RTC::ERROR);
    try
      {
	ret = onAborting(ec_id);
      }
    catch (...)
      {
	return RTC::ERROR;
      }
    return ret;
  }

  /*!
   * @if jp
   * @brief [ComponentAction interface]
   * @else
   * @brief [ComponentAction interface]
   * @endif
   */
  ReturnCode_t RTObject_impl::on_error(UniqueId ec_id)
  {
    ReturnCode_t ret(RTC::ERROR);
    try
      {
	ret = onError(ec_id);
      }
    catch (...)
      {
	return RTC::ERROR;
      }
    return ret;
  }

  /*!
   * @if jp
   * @brief [ComponentAction interface]
   * @else
   * @brief [ComponentAction interface]
   * @endif
   */
  ReturnCode_t RTObject_impl::on_reset(UniqueId ec_id)
  {
    ReturnCode_t ret(RTC::ERROR);
    try
      {
	ret = onReset(ec_id);
      }
    catch (...)
      {
	return RTC::ERROR;
      }
    return ret;
  }

  ReturnCode_t RTObject_impl::on_execute(UniqueId ec_id)
  {
    ReturnCode_t ret(RTC::ERROR);
    try
      {
	ret = onExecute(ec_id);
      }
    catch (...)
      {
	return RTC::ERROR;
      }
    return ret;
  }

  ReturnCode_t RTObject_impl::on_state_update(UniqueId ec_id)
  {
    ReturnCode_t ret(RTC::ERROR);
    try
      {
	ret = onStateUpdate(ec_id);
      }
    catch (...)
      {
	return RTC::ERROR;
      }
    return ret;
  }

  ReturnCode_t RTObject_impl::on_rate_changed(UniqueId ec_id)
  {
    ReturnCode_t ret(RTC::ERROR);
    try
      {
	ret = onRateChanged(ec_id);
      }
    catch (...)
      {
	return RTC::ERROR;
      }
    return ret;
  }

  //============================================================
  // SDO interfaces
  //============================================================
  /*!
   * @if jp
   * @brief [SDO interface] Organization リストの取得 
   * @else
   * @brief [SDO interface] Getting Organizations
   * @endif
   */
  SDOPackage::OrganizationList* RTObject_impl::get_owned_organizations()
    throw (SDOPackage::NotAvailable)
  {
    try
      {
	SDOPackage::OrganizationList_var org_list;
	org_list = new SDOPackage::OrganizationList(m_sdoOwnedOrganizations);
	return org_list._retn();
      }
    catch (...)
      {
	throw SDOPackage::NotAvailable();
      }
    return new SDOPackage::OrganizationList();
  }

  // SDOPackage::SDO
  /*!
   * @if jp
   * @brief [SDO interface] SDO ID の取得
   * @else
   * @brief [SDO interface] Getting SDO ID
   * @endif
   */
  char* RTObject_impl::get_sdo_id()
    throw (SDOPackage::NotAvailable, SDOPackage::InternalError)
  {
    try
      {
	CORBA::String_var sdo_id;
	sdo_id = CORBA::string_dup(m_profile.instance_name);
	return sdo_id._retn();
      }
    catch (...)
      {
	throw SDOPackage::InternalError("get_sdo_id()");
      }
  }
  
  /*!
   * @if jp
   * @brief [SDO interface] SDO タイプの取得
   * @else
   * @brief [SDO interface] Getting SDO type
   * @endif
   */
  char* RTObject_impl::get_sdo_type()
    throw (SDOPackage::NotAvailable, SDOPackage::InternalError)
  {
    try
      {
	CORBA::String_var sdo_type;
	sdo_type = CORBA::string_dup(m_profile.description);
	return sdo_type._retn();
      }
    catch (...)
      {
	throw SDOPackage::InternalError("get_sdo_type()");
      }
    return "";
  }

  /*!
   * @if jp
   * @brief [SDO interface] SDO DeviceProfile リストの取得 
   * @else
   * @brief [SDO interface] Getting SDO DeviceProfile
   * @endif
   */
  SDOPackage::DeviceProfile* RTObject_impl::get_device_profile()
    throw (SDOPackage::NotAvailable, SDOPackage::InternalError)
  {
    try
      {
	SDOPackage::DeviceProfile_var dprofile;
	dprofile = new SDOPackage::DeviceProfile();
	dprofile->device_type  = CORBA::string_dup(m_profile.category);
	dprofile->manufacturer = CORBA::string_dup(m_profile.vendor);
	dprofile->model        = CORBA::string_dup(m_profile.type_name);
	dprofile->version      = CORBA::string_dup(m_profile.version);
	dprofile->properties   = m_profile.properties;
	return dprofile._retn();
      }
    catch (...)
      {
	throw SDOPackage::InternalError("get_device_profile()");
      }
    return new SDOPackage::DeviceProfile();
  }

  /*!
   * @if jp
   * @brief [SDO interface] SDO ServiceProfile の取得 
   * @else
   * @brief [SDO interface] Getting SDO ServiceProfile
   * @endif
   */
  SDOPackage::ServiceProfileList* RTObject_impl::get_service_profiles()
    throw (SDOPackage::InvalidParameter, SDOPackage::NotAvailable,
	   SDOPackage::InternalError)
  {
    try
      {
	SDOPackage::ServiceProfileList_var sprofiles;
	sprofiles = new SDOPackage::ServiceProfileList(m_sdoSvcProfiles);
	return sprofiles._retn();
      }
    catch (...)
      {
	throw SDOPackage::InternalError("get_service_profiles()");
      }
    return new SDOPackage::ServiceProfileList();
  }


  /*!
   * @if jp
   * @brief [SDO interface] 特定のServiceProfileの取得 
   * @else
   * @brief [SDO interface] Getting Organizations
   * @endif
   */
  SDOPackage::ServiceProfile*
  RTObject_impl::get_service_profile(const char* id)
    throw (SDOPackage::InvalidParameter, SDOPackage::NotAvailable,
	   SDOPackage::InternalError)
  {
    if (!id)
      throw SDOPackage::InvalidParameter("get_service_profile(): Empty name.");

    try
      {
	CORBA::Long index;
	index = CORBA_SeqUtil::find(m_sdoSvcProfiles, svc_name(id));

	SDOPackage::ServiceProfile_var sprofile;
	sprofile = new SDOPackage::ServiceProfile(m_sdoSvcProfiles[index]);
	return sprofile._retn();
      }
    catch (...)
      {
	throw SDOPackage::InternalError("get_service_profile()");
      }
    return new SDOPackage::ServiceProfile();
  }
  
  /*!
   * @if jp
   * @brief [SDO interface] 指定された SDO Service の取得
   * @else
   * @brief [SDO interface] Getting specified SDO Service's reference
   * @endif
   */
  SDOPackage::SDOService_ptr RTObject_impl::get_sdo_service(const char* id)
    throw (SDOPackage::InvalidParameter, SDOPackage::NotAvailable,
	   SDOPackage::InternalError)
  {
    if (!id)
      throw SDOPackage::InvalidParameter("get_service(): Empty name.");

    try
      {
	CORBA::Long index;
	index = CORBA_SeqUtil::find(m_sdoSvcProfiles, svc_name(id));

	SDOPackage::SDOService_var service;
	service = m_sdoSvcProfiles[index].service;
	return service._retn();
      }
    catch (...)
      {
	throw SDOPackage::InternalError("get_service()");
      }
    return SDOPackage::SDOService::_nil();
  }

  /*!
   * @if jp
   * @brief [SDO interface] Configuration オブジェクトの取得 
   * @else
   * @brief [SDO interface] Getting Configuration object
   * @endif
   */
  SDOPackage::Configuration_ptr RTObject_impl::get_configuration()
    throw (SDOPackage::InterfaceNotImplemented, SDOPackage::NotAvailable,
	   SDOPackage::InternalError)
  {
    if (m_pSdoConfig == NULL)
      throw SDOPackage::InterfaceNotImplemented();
    try
      {
	SDOPackage::Configuration_var config;
	config = m_pSdoConfig;
	return config;
      }
    catch (...)
      {
	SDOPackage::InternalError("get_configuration()");
      }
    return SDOPackage::Configuration::_nil();
  }
  
  /*!
   * @if jp
   * @brief [SDO interface] Monitoring オブジェクトの取得 
   * @else
   * @brief [SDO interface] Get Monitoring object
   * @endif
   */
  SDOPackage::Monitoring_ptr RTObject_impl::get_monitoring()
    throw (SDOPackage::InterfaceNotImplemented, SDOPackage::NotAvailable,
	   SDOPackage::InternalError)
  {
    throw SDOPackage::InterfaceNotImplemented();
    return SDOPackage::Monitoring::_nil();
  }

    /*!
     * @if jp
     * @brief [SDO interface] Organization リストの取得 
     * @else
     * @brief [SDO interface] Getting Organizations
     * @endif
     */
  SDOPackage::OrganizationList* RTObject_impl::get_organizations()
    throw (SDOPackage::NotAvailable, SDOPackage::InternalError)
  {
    try
      {
	SDOPackage::OrganizationList_var org;
	org = new SDOPackage::OrganizationList(m_sdoOrganizations);
	return org._retn();
      }
    catch (...)
      {
	throw SDOPackage::InternalError("get_organizations()");
      }
    return new SDOPackage::OrganizationList(0);
  }

  /*!
   * @if jp
   * @brief [SDO interface] SDO Status リストの取得 
   * @else
   * @brief [SDO interface] Get SDO Status
   * @endif
   */
  SDOPackage::NVList* RTObject_impl::get_status_list()
    throw (SDOPackage::NotAvailable, SDOPackage::InternalError)
  {
    try
      {
	NVList_var status;
	status = new NVList(m_sdoStatus);
	return status._retn();
      }
    catch (...)
      {
	SDOPackage::InternalError("get_status_list()");
      }
    return new SDOPackage::NVList(0);
  }


  /*!
   * @if jp
   * @brief [SDO interface] SDO Status の取得 
   * @else
   * @brief [SDO interface] Get SDO Status
   * @endif
   */
  CORBA::Any* RTObject_impl::get_status(const char* name)
    throw (SDOPackage::InvalidParameter, SDOPackage::NotAvailable,
	   SDOPackage::InternalError)
  {
    CORBA::Long index;
    index = CORBA_SeqUtil::find(m_sdoStatus, nv_name(name));
    if (index < 0)
      throw SDOPackage::InvalidParameter("get_status(): Not found");
    try
      {
	CORBA::Any_var status;
	status = new CORBA::Any(m_sdoStatus[index].value);
	return status._retn();
      }
    catch (...)
      {
	throw SDOPackage::InternalError("get_status()");
      }
    return new CORBA::Any();
  }


  //============================================================
  // Local methods
  //============================================================

  void RTObject_impl::setInstanceName(const char* instance_name)
  {
    m_properties["instance_name"] = instance_name;
    m_profile.instance_name = m_properties["instance_name"].c_str();
  }


  std::vector<std::string> RTObject_impl::getNamingNames()
  {
    std::vector<std::string> naming_names;
    return split(m_properties.getProperty("naming_names"), ",");
  }


  void RTObject_impl::setObjRef(const RTObject_ptr rtobj)
  {
    m_objref = rtobj;
  }


  RTObject_ptr RTObject_impl::getObjRef() const
  {
    return RTC::RTObject::_duplicate(m_objref);
  }


  void RTObject_impl::setProperties(const Properties& prop)
  {
    m_properties = prop;
    m_profile.instance_name = m_properties["instance_name"].c_str();
    m_profile.type_name     = m_properties["type_name"].c_str();
    m_profile.description   = m_properties["description"].c_str();
    m_profile.version       = m_properties["version"].c_str();
    m_profile.vendor        = m_properties["vendor"].c_str();
    m_profile.category      = m_properties["category"].c_str();
    //    m_componentProfile.port_profiles;
    //    m_componentProfile.parent;
    //    m_componentProfile.properties = toNVList(m_prop);
  }


  Properties& RTObject_impl::getProperties()
  {
    return m_properties;
  }


  void RTObject_impl::registerPort(PortBase& port)
  {
    m_portAdmin.registerPort(port);
    return;
  }
  
  void RTObject_impl::deletePort(PortBase& port)
  {
    m_portAdmin.deletePort(port);
    return;
  }
  
  void RTObject_impl::deletePortByName(const char* port_name)
  {
    m_portAdmin.deletePortByName(port_name);
    return;
  }

  void RTObject_impl::finalizePorts()
  {
    m_portAdmin.finalizePorts();
  }

};

/*!
 * @file ExecutionContext.h
 * @brief ExecutionContext class
 * @date $Date: 2007-01-21 13:43:13 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2006
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id: ExecutionContext.cpp,v 1.1 2007-01-21 13:43:13 n-ando Exp $
 *
 */

/*
 * $Log: not supported by cvs2svn $
 */

#include <rtm/ExecutionContext.h>
#include <rtm/CORBA_SeqUtil.h>

namespace RTC
{
  
  ExecutionContextBase::ExecutionContextBase(RTObject_ptr owner)
    : m_running(false)
  {
    m_profile.kind = OTHER;
    m_profile.rate = 0.0;
    m_profile.owner = owner;
    m_profile.participants.length(0);
  }

  /*!
   * @if jp
   * @brief ExecutionContext が実行中かどうかのテスト
   * @else
   * @brief Test for ExecutionContext running state
   * @endif
   */
  CORBA::Boolean ExecutionContextBase::is_running()
  {
    return m_running;
  }
  

  /*!
   * @if jp
   * @brief ExecutionContext をスタートさせる
   * @else
   * @brief Start the ExecutionContext
   * @endif
   */
  ReturnCode_t ExecutionContextBase::start()
  {
    return RTC::OK;
  }
  

  /*!
   * @if jp
   * @brief ExecutionContext をストップさせる
   * @else
   * @brief Stop the ExecutionContext
   * @endif
   */
  ReturnCode_t ExecutionContextBase::stop()
  {
    return RTC::OK;
  }
  
  
  /*!
   * @if jp
   * @brief 実行周期(Hz)を取得する
   * @else
   * @brief Get executionrate(Hz)
   * @endif
   */
  CORBA::Double ExecutionContextBase::get_rate()
  {
    return m_profile.rate;
  }
  
  
  /*!
   * @if jp
   * @brief 実行周期(Hz)を与える 
   * @else
   * @brief Set rate (Hz)
   * @endif
   */
  ReturnCode_t ExecutionContextBase::set_rate(CORBA::Double rate)
  {
    if (rate > 0.0)
      {
	m_profile.rate = rate;
	return RTC::OK;
      }
    return RTC::BAD_PARAMETER;
  }
  

  /*!
   * @if jp
   * @brief コンポーネントをアクティブ化する
   * @else
   * @brief Activate a component
   * @endif
   */ 
  ReturnCode_t
  ExecutionContextBase::activate_component(LightweightRTObject_ptr comp)
  {
    return RTC::OK;
  }
  

  /*!
   * @if jp
   * @brief コンポーネントを非アクティブ化する
   * @else
   * @brief Deactivate a component
   * @endif
   */  
  ReturnCode_t
  ExecutionContextBase::deactivate_component(LightweightRTObject_ptr comp)
  {
    return RTC::OK;
  }
  

  /*!
   * @if jp
   * @brief コンポーネントを非アクティブ化する
   * @else
   * @brief Deactivate a component
   * @endif
   */  
  ReturnCode_t
  ExecutionContextBase::reset_component(LightweightRTObject_ptr comp)
  {
    return RTC::OK;
  }
  
  
  /*!
   * @if jp
   * @brief コンポーネントの状態を取得する
   * @else
   * @brief Get component's state
   * @endif
   */
  LifeCycleState
  ExecutionContextBase::get_component_state(LightweightRTObject_ptr comp)
  {
    return RTC::INACTIVE_STATE;
  }
  
  
  /*!
   * @if jp
   * @brief ExecutionKind を取得する
   * @else
   * @brief Get the ExecutionKind
   * @endif
   */
  ExecutionKind ExecutionContextBase::get_kind()
  {
    return m_profile.kind;
  }
  

  /*!
   * @if jp
   * @brief コンポーネントを追加する
   * @else
   * @brief Add a component
   * @endif
   */
  ReturnCode_t ExecutionContextBase::add(LightweightRTObject_ptr comp)
  {
    if (!CORBA::is_nil(comp))
      {
	CORBA_SeqUtil::push_back(m_profile.participants,
				 RTC::RTObject::_narrow(comp));
	return RTC::OK;
      }
    return RTC::BAD_PARAMETER;
  }
  
  
  /*!
   * @if jp
   * @brief コンポーネントをコンポーネントリストから削除する
   * @else
   * @brief Remove the component from component list
   * @endif
   */	
  ReturnCode_t
  ExecutionContextBase::remove(LightweightRTObject_ptr comp)
  {
    CORBA::ULong index;
    index = CORBA_SeqUtil::find(m_profile.participants,
		find_objref<RTObject_ptr>(RTC::RTObject::_narrow(comp)));
				
    if (index < 0) return RTC::BAD_PARAMETER;
    CORBA_SeqUtil::erase(m_profile.participants, index);
    return RTC::OK;
  }


  //============================================================
  // ExecutionContextAdmin interfaces
  //============================================================
  /*!
   * @if jp
   * @brief ExecutionContextProfile を取得する
   * @else
   * @brief Get the ExecutionContextProfile
   * @endif
   */
  ExecutionContextProfile* ExecutionContextBase::get_profile()
  {
    ExecutionContextProfile_var p;
    p = new ExecutionContextProfile(m_profile);
    return p._retn();
  }
  
  
  
}; // namespace RTC

﻿// -*- C++ -*-
/*!
 * @file PeriodicExecutionContext.cpp
 * @brief PeriodicExecutionContext class
 * @date $Date: 2008-01-14 07:53:01 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2006-2008,2012
 *     Noriaki Ando
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id$
 *
 */

#include <coil/Time.h>
#include <coil/TimeValue.h>

#include <rtm/PeriodicExecutionContext.h>
#include <rtm/RTObjectStateMachine.h>

#ifdef RTM_OS_LINUX
#define _GNU_SOURCE
#include <pthread.h>
#include <algorithm>
#endif // RTM_OS_LINUX

#include <string.h>
#include <algorithm>
#include <iostream>
#include <string>

#define DEEFAULT_PERIOD 0.000001
namespace RTC_exp
{
  /*!
   * @if jp
   * @brief デフォルトコンストラクタ
   * @else
   * @brief Default constructor
   * @endif
   */
  PeriodicExecutionContext::
  PeriodicExecutionContext()
    : ExecutionContextBase("periodic_ec"),
      rtclog("periodic_ec"),
      m_svc(false), m_nowait(false)
  {
    RTC_TRACE(("PeriodicExecutionContext()"));

    // getting my reference
    setObjRef(this->_this());

    // profile initialization
    setKind(RTC::PERIODIC);
    setRate(1.0 / static_cast<double>(DEEFAULT_PERIOD));

    RTC_DEBUG(("Actual period: %d [sec], %d [usec]",
               m_profile.getPeriod().sec(), m_profile.getPeriod().usec()));
  }

  /*!
   * @if jp
   * @brief デストラクタ
   * @else
   * @brief Destructor
   * @endif
   */
  PeriodicExecutionContext::~PeriodicExecutionContext()
  {
    RTC_TRACE(("~PeriodicExecutionContext()"));
    {
      Guard guard(m_svcmutex);
      m_svc = false;
    }
    {
      Guard guard(m_workerthread.mutex_);
      m_workerthread.running_ = true;
      m_workerthread.cond_.signal();
    }
    wait();
  }

  void PeriodicExecutionContext::init(coil::Properties& props)
  {
    RTC_TRACE(("init()"));
    ExecutionContextBase::init(props);

    setCpuAffinity(props);

    RTC_DEBUG(("init() done"));
  }

  /*------------------------------------------------------------
   * Start activity
   * ACE_Task class method over ride.
   *------------------------------------------------------------*/
  /*!
   * @if jp
   * @brief ExecutionContext用アクティビティスレッドを生成する
   * @else
   * @brief Generate internal activity thread for ExecutionContext
   * @endif
   */
  int PeriodicExecutionContext::open(void *args)
  {
    RTC_TRACE(("open()"));
    activate();
    return 0;
  }

  /*------------------------------------------------------------
   * Run by a daemon thread to handle deferred processing
   * ACE_Task class method over ride.
   *------------------------------------------------------------*/
  /*!
   * @if jp
   * @brief ExecutionContext 用のスレッド実行関数
   * @else
   * @brief Thread execution function for ExecutionContext
   * @endif
   */
  int PeriodicExecutionContext::svc(void)
  {
    RTC_TRACE(("svc()"));
    int count(0);

    bool result = coil::setThreadCpuAffinity(m_cpu);

    if (!result)
      {
        RTC_ERROR(("setThreadCpuAffinity():"
                   "CPU affinity mask setting failed"));
      };
	
	coil::CpuMask ret_cpu;
	result = coil::getThreadCpuAffinity(ret_cpu);
	

#ifdef RTM_OS_LINUX
	std::sort(ret_cpu.begin(), ret_cpu.end());
	std::sort(m_cpu.begin(), m_cpu.end());
	if (result && ret_cpu.size() > 0 && m_cpu.size() > 0 && ret_cpu.size() == m_cpu.size()
        && std::equal(ret_cpu.begin(), ret_cpu.end(), m_cpu.begin()))
	{

	}
	else
	{
		RTC_ERROR(("pthread_getaffinity_np(): returned error."));
	}
#endif

    do
      {
        ExecutionContextBase::invokeWorkerPreDo();
        // Thread will stopped when all RTCs are INACTIVE.
        // Therefore WorkerPreDo(updating state) have to be invoked
        // before stopping thread.
        {
          Guard guard(m_workerthread.mutex_);
          while (!m_workerthread.running_)
            {
              m_workerthread.cond_.wait();
            }
        }
        coil::TimeValue t0(coil::clock());
        ExecutionContextBase::invokeWorkerDo();
        ExecutionContextBase::invokeWorkerPostDo();
        coil::TimeValue t1(coil::clock());

        coil::TimeValue period(getPeriod());
        if (count > 1000)
          {
            RTC_PARANOID(("Period:    %f [s]", static_cast<double>(period)));
            RTC_PARANOID(("Execution: %f [s]", static_cast<double>(t1 - t0)));
            RTC_PARANOID(("Sleep:     %f [s]",
                                    static_cast<double>(period - (t1 - t0))));
          }
        coil::TimeValue t2(coil::clock());
        if (!m_nowait && period > (t1 - t0))
          {
            if (count > 1000) { RTC_PARANOID(("sleeping...")); }
            coil::sleep((coil::TimeValue)(period - (t1 - t0)));
          }
        if (count > 1000)
          {
            coil::TimeValue t3(coil::clock());
            RTC_PARANOID(("Slept:     %f [s]", static_cast<double>(t3 - t2)));
            count = 0;
          }
        ++count;
      } while (threadRunning());
      
    RTC_DEBUG(("Thread terminated."));
    return 0;
  }

  /*!
   * @if jp
   * @brief ExecutionContext 用のスレッド実行関数
   * @else
   * @brief Thread execution function for ExecutionContext
   * @endif
   */
  int PeriodicExecutionContext::close(unsigned long flags)
  {
    RTC_TRACE(("close()"));
    // At this point, this component have to be finished.
    // Current state and Next state should be RTC_EXITING.
    return 0;
  }


  //============================================================
  // ExecutionContext CORBA operations
  //============================================================
  /*!
   * @if jp
   * @brief ExecutionContext 実行状態確認関数
   * @else
   * @brief Check for ExecutionContext running state
   * @endif
   */
  CORBA::Boolean PeriodicExecutionContext::is_running()
#if __cplusplus < 201703L
    throw (CORBA::SystemException)
#endif
  {
    return ExecutionContextBase::isRunning();
  }

  /*!
   * @if jp
   * @brief ExecutionContext の実行を開始
   * @else
   * @brief Start the ExecutionContext
   * @endif
   */
  RTC::ReturnCode_t PeriodicExecutionContext::start()
#if __cplusplus < 201703L
    throw (CORBA::SystemException)
#endif
  {
    return ExecutionContextBase::start();
  }

  /*!
   * @if jp
   * @brief ExecutionContext の実行を停止
   * @else
   * @brief Stop the ExecutionContext
   * @endif
   */
  RTC::ReturnCode_t PeriodicExecutionContext::stop()
#if __cplusplus < 201703L
    throw (CORBA::SystemException)
#endif
  {
    return ExecutionContextBase::stop();
  }



  /*!
   * @if jp
   * @brief ExecutionContext の実行周期(Hz)を取得する
   * @else
   * @brief Get execution rate(Hz) of ExecutionContext
   * @endif
   */
  CORBA::Double PeriodicExecutionContext::get_rate()
#if __cplusplus < 201703L
    throw (CORBA::SystemException)
#endif
  {
    return ExecutionContextBase::getRate();
  }

  /*!
   * @if jp
   * @brief ExecutionContext の実行周期(Hz)を設定する
   * @else
   * @brief Set execution rate(Hz) of ExecutionContext
   * @endif
   */
  RTC::ReturnCode_t PeriodicExecutionContext::set_rate(CORBA::Double rate)
#if __cplusplus < 201703L
    throw (CORBA::SystemException)
#endif
  {
    return ExecutionContextBase::setRate(rate);
  }

  /*!
   * @if jp
   * @brief RTコンポーネントを追加する
   * @else
   * @brief Add an RT-Component
   * @endif
   */
  RTC::ReturnCode_t
  PeriodicExecutionContext::add_component(RTC::LightweightRTObject_ptr comp)
#if __cplusplus < 201703L
    throw (CORBA::SystemException)
#endif
  {
    return ExecutionContextBase::addComponent(comp);
  }

  /*!
   * @if jp
   * @brief コンポーネントをコンポーネントリストから削除する
   * @else
   * @brief Remove the RT-Component from participant list
   * @endif
   */
  RTC::ReturnCode_t PeriodicExecutionContext::
  remove_component(RTC::LightweightRTObject_ptr comp)
#if __cplusplus < 201703L
    throw (CORBA::SystemException)
#endif
  {
    return ExecutionContextBase::removeComponent(comp);
  }

  /*!
   * @if jp
   * @brief RTコンポーネントをアクティブ化する
   * @else
   * @brief Activate an RT-Component
   * @endif
   */
  RTC::ReturnCode_t PeriodicExecutionContext::
  activate_component(RTC::LightweightRTObject_ptr comp)
#if __cplusplus < 201703L
    throw (CORBA::SystemException)
#endif
  {
    return ExecutionContextBase::activateComponent(comp);
  }

  /*!
   * @if jp
   * @brief RTコンポーネントを非アクティブ化する
   * @else
   * @brief Deactivate an RT-Component
   * @endif
   */
  RTC::ReturnCode_t PeriodicExecutionContext::
  deactivate_component(RTC::LightweightRTObject_ptr comp)
#if __cplusplus < 201703L
    throw (CORBA::SystemException)
#endif
  {
    return ExecutionContextBase::deactivateComponent(comp);
  }

  /*!
   * @if jp
   * @brief RTコンポーネントをリセットする
   * @else
   * @brief Reset the RT-Component
   * @endif
   */
  RTC::ReturnCode_t PeriodicExecutionContext::
  reset_component(RTC::LightweightRTObject_ptr comp)
#if __cplusplus < 201703L
    throw (CORBA::SystemException)
#endif
  {
    return ExecutionContextBase::resetComponent(comp);
  }

  /*!
   * @if jp
   * @brief RTコンポーネントの状態を取得する
   * @else
   * @brief Get RT-Component's state
   * @endif
   */
  RTC::LifeCycleState PeriodicExecutionContext::
  get_component_state(RTC::LightweightRTObject_ptr comp)
#if __cplusplus < 201703L
    throw (CORBA::SystemException)
#endif
  {
    RTC::LifeCycleState ret = ExecutionContextBase::getComponentState(comp);
    return ret;
  }

  /*!
   * @if jp
   * @brief ExecutionKind を取得する
   * @else
   * @brief Get the ExecutionKind
   * @endif
   */
  RTC::ExecutionKind PeriodicExecutionContext::get_kind()
#if __cplusplus < 201703L
    throw (CORBA::SystemException)
#endif
  {
    return ExecutionContextBase::getKind();
  }

  //------------------------------------------------------------
  // ExecutionContextService interfaces
  //------------------------------------------------------------
  /*!
   * @if jp
   * @brief ExecutionContextProfile を取得する
   * @else
   * @brief Get the ExecutionContextProfile
   * @endif
   */
  RTC::ExecutionContextProfile* PeriodicExecutionContext::get_profile()
#if __cplusplus < 201703L
    throw (CORBA::SystemException)
#endif
  {
    return ExecutionContextBase::getProfile();
  }


  //============================================================
  // protected functions
  //============================================================
  /*!
   * @brief onStarted() template function
   */
  RTC::ReturnCode_t PeriodicExecutionContext::onStarted()
  {
    // change EC thread state
    {
      Guard guard(m_svcmutex);
      if (!m_svc)
        {
          m_svc = true;
          this->open(0);
        }
    }
    if (isAllNextState(RTC::INACTIVE_STATE))
      {
        Guard guard(m_workerthread.mutex_);
        m_workerthread.running_ = false;
      }
    else
      {
        Guard guard(m_workerthread.mutex_);
        m_workerthread.running_ = true;
        m_workerthread.cond_.signal();
      }
    return RTC::RTC_OK;
  }

  /*!
   * @brief onStopping() template function
   */
  RTC::ReturnCode_t PeriodicExecutionContext::onStopping()
  {
    // stop thread
    Guard guard(m_workerthread.mutex_);
    m_workerthread.running_ = false;
    return RTC::RTC_OK;
  }

  // template virtual functions adding/removing component
  /*!
   * @brief onAddedComponent() template function
   */
  RTC::ReturnCode_t PeriodicExecutionContext::
  onAddedComponent(RTC::LightweightRTObject_ptr rtobj)
  {
    Guard guard(m_workerthread.mutex_);
    if (m_workerthread.running_ == false)
      {
        m_worker.updateComponentList();
      }
    return RTC::RTC_OK;
  }
  /*!
   * @brief onRemovedComponent() template function
   */
  RTC::ReturnCode_t PeriodicExecutionContext::
  onRemovedComponent(RTC::LightweightRTObject_ptr rtobj)
  {
    Guard guard(m_workerthread.mutex_);
    if (m_workerthread.running_ == false)
      {
        m_worker.updateComponentList();
      }
    return RTC::RTC_OK;
  }

  /*!
   * @brief onWaitingActivated() template function
   */
  RTC::ReturnCode_t PeriodicExecutionContext::
  onWaitingActivated(RTC_impl::RTObjectStateMachine* comp, long int count)
  {
    RTC_TRACE(("onWaitingActivated(count = %d)", count));
    RTC_PARANOID(("curr: %s, next: %s",
                  getStateString(comp->getStates().curr),
                  getStateString(comp->getStates().next)));
    // Now comp's next state must be ACTIVE state
    // If worker thread is stopped, restart worker thread.
    if (isRunning())
    {
        Guard guard(m_workerthread.mutex_);
        if (m_workerthread.running_ == false)
        {
            m_workerthread.running_ = true;
            m_workerthread.cond_.signal();
        }
    }
    return RTC::RTC_OK;
  }

  /*!
   * @brief onActivated() template function
   */
  RTC::ReturnCode_t PeriodicExecutionContext::
  onActivated(RTC_impl::RTObjectStateMachine* comp, long int count)
  {
    RTC_TRACE(("onActivated(count = %d)", count));
    RTC_PARANOID(("curr: %s, next: %s",
                  getStateString(comp->getStates().curr),
                  getStateString(comp->getStates().next)));
    // count = -1; Asynch mode. Since onWaitingActivated is not
    // called, onActivated() have to send restart singnal to worker
    // thread.
    // count > 0: Synch mode.

    // Now comp's next state must be ACTIVE state
    // If worker thread is stopped, restart worker thread.
    if (isRunning())
    {
        Guard guard(m_workerthread.mutex_);
        if (m_workerthread.running_ == false)
        {
            m_workerthread.running_ = true;
            m_workerthread.cond_.signal();
        }
    }
    return RTC::RTC_OK;
  }

  /*!
   * @brief onWaitingDeactivated() template function
   */
  RTC::ReturnCode_t PeriodicExecutionContext::
  onWaitingDeactivated(RTC_impl::RTObjectStateMachine* comp, long int count)
  {
    RTC_TRACE(("onWaitingDeactivated(count = %d)", count));
    RTC_PARANOID(("curr: %s, next: %s",
                  getStateString(comp->getStates().curr),
                  getStateString(comp->getStates().next)));
    if (isAllNextState(RTC::INACTIVE_STATE))
      {
        Guard guard(m_workerthread.mutex_);
        if (m_workerthread.running_ == true)
          {
            m_workerthread.running_ = false;
            RTC_TRACE(("All RTCs are INACTIVE. Stopping worker thread."));
          }
      }
    return RTC::RTC_OK;
  }

  /*!
   * @brief onDeactivated() template function
   */
  RTC::ReturnCode_t PeriodicExecutionContext::
  onDeactivated(RTC_impl::RTObjectStateMachine* comp, long int count)
  {
    RTC_TRACE(("onDeactivated(count = %d)", count));
    RTC_PARANOID(("curr: %s, next: %s",
                  getStateString(comp->getStates().curr),
                  getStateString(comp->getStates().next)));
    if (isAllNextState(RTC::INACTIVE_STATE))
      {
        Guard guard(m_workerthread.mutex_);
        if (m_workerthread.running_ == true)
          {
            m_workerthread.running_ = false;
            RTC_TRACE(("All RTCs are INACTIVE. Stopping worker thread."));
          }
      }
    return RTC::RTC_OK;
  }

  /*!
   * @brief onWaitingReset() template function
   */
  RTC::ReturnCode_t PeriodicExecutionContext::
  onWaitingReset(RTC_impl::RTObjectStateMachine* comp, long int count)
  {
    RTC_TRACE(("onWaitingReset(count = %d)", count));
    RTC_PARANOID(("curr: %s, next: %s",
                  getStateString(comp->getStates().curr),
                  getStateString(comp->getStates().next)));
    if (isAllNextState(RTC::INACTIVE_STATE))
      {
        Guard guard(m_workerthread.mutex_);
        if (m_workerthread.running_ == true)
          {
            m_workerthread.running_ = false;
            RTC_TRACE(("All RTCs are INACTIVE. Stopping worker thread."));
          }
      }
    return RTC::RTC_OK;
  }

  /*!
   * @brief onReset() template function
   */
  RTC::ReturnCode_t PeriodicExecutionContext::
  onReset(RTC_impl::RTObjectStateMachine* comp, long int count)
  {
    RTC_TRACE(("onReset(count = %d)", count));
    RTC_PARANOID(("curr: %s, next: %s",
                  getStateString(comp->getStates().curr),
                  getStateString(comp->getStates().next)));
    if (isAllNextState(RTC::INACTIVE_STATE))
      {
        Guard guard(m_workerthread.mutex_);
        if (m_workerthread.running_ == true)
          {
            m_workerthread.running_ = false;
            RTC_TRACE(("All RTCs are INACTIVE. Stopping worker thread."));
          }
      }
    return RTC::RTC_OK;
  }

  void PeriodicExecutionContext::setCpuAffinity(coil::Properties& props)
  {
    RTC_TRACE(("setCpuAffinity()"));
    std::string affinity;
    getProperty(props, "cpu_affinity", affinity);
    RTC_DEBUG(("CPU affinity property: %s", affinity.c_str()));

    coil::vstring tmp = coil::split(affinity, ",", true);
    m_cpu.clear();

    for (size_t i(0); i < tmp.size(); ++i)
      {
        int num;
        if (coil::stringTo(num, tmp[i].c_str()))
          {
            m_cpu.push_back(num);
            RTC_DEBUG(("CPU affinity int value: %d added.", num));
          }
      }
  }

};  // namespace RTC_exp

extern "C"
{
  /*!
   * @if jp
   * @brief ECFactoryへの登録のための初期化関数
   * @else
   * @brief Initialization function to register to ECFactory
   * @endif
   */

  void PeriodicExecutionContextInit(RTC::Manager* manager)
  {
    RTC::ExecutionContextFactory::
      instance().addFactory("PeriodicExecutionContext",
                            ::coil::Creator< ::RTC::ExecutionContextBase,
                            ::RTC_exp::PeriodicExecutionContext>,
                            ::coil::Destructor< ::RTC::ExecutionContextBase,
                            ::RTC_exp::PeriodicExecutionContext>);

    coil::vstring ecs;
    ecs = RTC::ExecutionContextFactory::instance().getIdentifiers();
  }
};


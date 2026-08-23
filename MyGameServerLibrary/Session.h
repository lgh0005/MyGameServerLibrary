#pragma once
#include "ISessionContext.h"
#include "NetIOCPObject.h"
#include "NetIOCPEvent.h"
#include "SessionBuffer.h"
#include "SessionConnection.h"

namespace MGSL::Net
{
	MGSL_CLASS_PTR(Service)
	MGSL_CLASS_PTR(SendBuffer)

	class Session : public NetIOCPObject, public ISessionContext
	{
	public:
		Session();
		virtual ~Session();

	/*=============================================//
	//   NetIOCPObject interface implementations   //
	//=============================================*/
	public:
		virtual HANDLE	GetObjectHandle() override;
		virtual void	Dispatch(NetIOCPEvent* iocpEvent,  Shared::int32 numOfBytes = 0) override;

	/*===============================================//
	//   ISessionContext interface implementations   //
	//===============================================*/
	public:
		virtual SOCKET     GetSocket() const override { return m_socket; }
		virtual bool       IsConnected() const override { return m_connection.IsConnected(); }
		virtual void       Disconnect(Shared::cstr cause) override { m_connection.Disconnect(cause); }
		virtual void       HandleError(Shared::int32 errorCode) override;
		virtual SessionPtr GetSessionPtr() override { return static_pointer_cast<Session>(shared_from_this()); }

	/*============================//
	//   abstracts for contents   //
	//============================*/
	protected:
		virtual void		  OnConnected()							  = 0;
		virtual void		  OnDisconnected()						  = 0;
		virtual Shared::int32 OnRecv(BYTE* buffer, Shared::int32 len) = 0;
		virtual void		  OnSend(Shared::int32 len)				  = 0;

	/*=============================//
	//   default session members   //
	//=============================*/
	public:
		bool Connect() { return m_connection.Connect(); }

	public:
		void		SetService(ServicePtr service);
		void		SetNetAddress(NetAddress address) { m_netAddress = address; }
		NetAddress	GetAddress() const { return m_netAddress; }
		ServicePtr  GetService() { return m_servcie.lock(); }
		SessionBuffer& GetSessionBuffer() { return m_buffer; }
		SessionConnection& GetSessionConnection() { return m_connection; }

	private:
		ServiceWPtr			 m_servcie;
		SOCKET				 m_socket	  = INVALID_SOCKET;
		NetAddress			 m_netAddress = {};

		SessionBuffer m_buffer
		{
			*this,
			[this](BYTE* buf, Shared::int32 len) { return OnRecv(buf, len); },
			[this](Shared::int32 len)			 { OnSend(len); }
		};

		SessionConnection m_connection
		{
			*this,
			[this]() { OnConnected(); },
			[this]() { OnDisconnected(); },
			[this]() { m_buffer.RegisterRecv(); }
		};
	};
}


#ifndef I2PSERVICE_H__
#define I2PSERVICE_H__

#include <atomic>
#include <mutex>
#include <unordered_set>
#include <memory>
#include <boost/asio.hpp>
#include "Destination.h"
#include "Identity.h"

namespace i2p
{
namespace client
{
	class I2PServiceHandler;
	class I2PService
	{
		public:
			I2PService (ClientDestination * localDestination  = nullptr);
			I2PService (i2p::data::SigningKeyType kt);
			virtual ~I2PService () { ClearHandlers (); }

			inline void AddHandler (std::shared_ptr<I2PServiceHandler> conn) {
				std::unique_lock<std::mutex> l(m_HandlersMutex);
				m_Handlers.insert(conn);
			}
			inline void RemoveHandler (std::shared_ptr<I2PServiceHandler> conn) {
				std::unique_lock<std::mutex> l(m_HandlersMutex);
				m_Handlers.erase(conn);
			}
			inline void ClearHandlers () {
				std::unique_lock<std::mutex> l(m_HandlersMutex);
				m_Handlers.clear();
			}

			inline ClientDestination * GetLocalDestination () { return m_LocalDestination; };
			inline void SetLocalDestination (ClientDestination * dest) { m_LocalDestination = dest; };
			void CreateStream (StreamRequestComplete streamRequestComplete, const std::string& dest, int port = 0);

			inline boost::asio::io_service& GetService () { return m_LocalDestination->GetService (); };

			virtual void Start () = 0;
			virtual void Stop () = 0;

		private:

			ClientDestination * m_LocalDestination;
			std::unordered_set<std::shared_ptr<I2PServiceHandler> > m_Handlers;
			std::mutex m_HandlersMutex;
	};

	/*Simple interface for I2PHandlers, allows detection of finalization amongst other things */
	class I2PServiceHandler
	{
		public:
			I2PServiceHandler(I2PService * parent) : m_Service(parent), m_Dead(false) { }
			virtual ~I2PServiceHandler() { }
		protected:
			// Call when terminating or handing over to avoid race conditions
			inline bool Kill() { return m_Dead.exchange(true); }
			// Call to know if the handler is dead
			inline bool Dead() { return m_Dead; }
			// Call when done to clean up (make sure Kill is called first)
			inline void Done(std::shared_ptr<I2PServiceHandler> me) { if(m_Service) m_Service->RemoveHandler(me); }
			// Call to talk with the owner
			inline I2PService * GetOwner() { return m_Service; }
		private:
			I2PService *m_Service;
			std::atomic<bool> m_Dead; //To avoid cleaning up multiple times
	};
}
}

#endif
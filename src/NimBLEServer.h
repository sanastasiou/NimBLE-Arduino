/*
 * NimBLEServer.h
 *
 *  Created: on March 2, 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEServer.h
 *
 *  Created on: Apr 16, 2017
 *      Author: kolban
 */

#ifndef MAIN_NIMBLESERVER_H_
#define MAIN_NIMBLESERVER_H_
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "NimBLEAddress.h"
#include "NimBLEUUID.h"
#include "NimBLEAdvertising.h"
#include "NimBLEService.h"
#include "NimBLESecurity.h"
#include "FreeRTOS.h"


class NimBLEService;
class NimBLECharacteristic;
class NimBLEServerCallbacks;


/**
 * @brief A data structure that manages the %BLE servers owned by a BLE server.
 */
class NimBLEServiceMap {
public:
//    NimBLEService* getByHandle(uint16_t handle);
    NimBLEService* getByUUID(const char* uuid);
    NimBLEService* getByUUID(const NimBLEUUID &uuid);
//    void           setByHandle(uint16_t handle, NimBLEService* service);
    void           addService(NimBLEService* service);
    std::string    toString();
    NimBLEService* getFirst();
    NimBLEService* getNext();
    void           removeService(NimBLEService *service);
    int            getRegisteredServiceCount();

private:
    std::vector<NimBLEService*> m_svcVec;
    std::vector<NimBLEService*>::iterator m_iterator;
};


/**
 * @brief The model of a %BLE server.
 */
class NimBLEServer {
public:
    size_t                getConnectedCount();
    NimBLEService*        createService(const char* uuid);
    NimBLEService*        createService(const NimBLEUUID &uuid, uint32_t numHandles=15, uint8_t inst_id=0);
    NimBLEAdvertising*    getAdvertising();
    void                  setCallbacks(NimBLEServerCallbacks* pCallbacks);
    void                  startAdvertising();
    void                  stopAdvertising();
    void                  start();
//    void                  removeService(BLEService* service);
    NimBLEService*        getServiceByUUID(const char* uuid);
    NimBLEService*        getServiceByUUID(const NimBLEUUID &uuid);
    int                   disconnect(uint16_t connID, uint8_t reason = BLE_ERR_REM_USER_CONN_TERM);
    void                  updateConnParams(uint16_t conn_handle,
                                           uint16_t minInterval, uint16_t maxInterval,
                                           uint16_t latency, uint16_t timeout);
    uint16_t              getPeerMTU(uint16_t conn_id);
    std::vector<uint16_t> getPeerDevices();

private:
    NimBLEServer();
    friend class           NimBLECharacteristic;
    friend class           NimBLEDevice;
    friend class           NimBLEAdvertising;

    uint16_t               m_svcChgChrHdl;
    bool                   m_gattsStarted;
    NimBLEServiceMap       m_svcVec;
    NimBLEServerCallbacks* m_pServerCallbacks;
    std::vector<uint16_t>  m_connectedPeersVec;
    std::vector<NimBLECharacteristic*> m_notifyChrVec;

    static int             handleGapEvent(struct ble_gap_event *event, void *arg);
}; // NimBLEServer


/**
 * @brief Callbacks associated with the operation of a %BLE server.
 */
class NimBLEServerCallbacks {
public:
    virtual ~NimBLEServerCallbacks() {};
    /**
     * @brief Handle a new client connection.
     *
     * When a new client connects, we are invoked.
     *
     * @param [in] pServer A reference to the %BLE server that received the client connection.
     */
    virtual void onConnect(NimBLEServer* pServer);
    virtual void onConnect(NimBLEServer* pServer, ble_gap_conn_desc* desc);
    /**
     * @brief Handle an existing client disconnection.
     *
     * When an existing client disconnects, we are invoked.
     *
     * @param [in] pServer A reference to the %BLE server that received the existing client disconnection.
     */
    virtual void onDisconnect(NimBLEServer* pServer);

    virtual uint32_t onPassKeyRequest(); //{return 0;}
    virtual void onPassKeyNotify(uint32_t pass_key); //{}
    virtual bool onSecurityRequest(); //{return true;}
    virtual void onAuthenticationComplete(ble_gap_conn_desc* desc);//{};
    virtual bool onConfirmPIN(uint32_t pin);//{return true;}
}; // NimBLEServerCallbacks


#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
#endif /* CONFIG_BT_ENABLED */
#endif /* MAIN_NIMBLESERVER_H_ */

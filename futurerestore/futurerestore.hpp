//
//  futurerestore.hpp
//  futurerestore
//
//  Created by tihmstar on 14.09.16.
//  Copyright © 2016 tihmstar. All rights reserved.
//

#ifndef futurerestore_hpp
#define futurerestore_hpp

#include "config.h"
#include <stdio.h>
#include <functional>
#include <vector>
#include "idevicerestore.h"
#include "jsmn.h"
#include <plist/plist.h>

using namespace std;

template <typename T>
class ptr_smart {
    std::function<void(T)> _ptr_free = NULL;
public:
    T _p;
    ptr_smart(T p, function<void(T)> ptr_free){static_assert(is_pointer<T>(), "error: this is for pointers only\n"); _p = p;_ptr_free = ptr_free;}
    ptr_smart(T p){_p = p;}
    ptr_smart(){_p = NULL;}
    T operator=(T p){return _p = p;}
    T *operator&(){return &_p;}
    ~ptr_smart(){if (_p) (_ptr_free) ? _ptr_free(_p) : free((void*)_p);}
};

class futurerestore {
    struct idevicerestore_client_t* _client;
    char *_ibootBuild = NULL;
    bool _didInit = false;
    vector<plist_t> _aptickets;
    vector<char *>_im4ms;
    int _foundnonce = -1;
    bool _isUpdateInstall = false;
    
    char *_firmwareJson = NULL;
    jsmntok_t *_firmwareTokens = NULL;;
    char *__latestManifest = NULL;
    char *__latestFirmwareUrl = NULL;
    
    plist_t _sepbuildmanifest = NULL;
    plist_t _basebandbuildmanifest = NULL;
    
    const char *_basebandPath = NULL;;
    const char *_sepbuildmanifestPath = NULL;
    const char *_basebandbuildmanifestPath = NULL;
    
    
public:
    futurerestore();
    futurerestore(bool isUpdateInstall);
    bool init();
    int getDeviceMode(bool reRequest);
    uint64_t getDeviceEcid();
    void putDeviceIntoRecovery();
    void setAutoboot(bool val);
    void waitForNonce();
    void waitForNonce(vector<const char *>nonces, size_t nonceSize);
    void loadAPTickets(const vector<const char *> &apticketPaths);
    char *getiBootBuild();
    
    plist_t nonceMatchesApTickets();
    const char *nonceMatchesIM4Ms();

    void loadFirmwareTokens();
    const char *getDeviceModelNoCopy();
    const char *getDeviceBoardNoCopy();
    char *getLatestManifest();
    char *getLatestFirmwareUrl();
    void loadLatestBaseband();
    void loadLatestSep();
    
    void setSepManifestPath(const char *sepManifestPath);
    void setBasebandManifestPath(const char *basebandManifestPath);
    void loadSep(const char *sepPath);
    void setBasebandPath(const char *basebandPath);
    bool isUpdateInstall(){return _isUpdateInstall;};
    
    
    plist_t sepManifest(){return _sepbuildmanifest;};
    plist_t basebandManifest(){return _basebandbuildmanifest;};
    const char *sepManifestPath(){return _sepbuildmanifestPath;};
    const char *basebandManifestPath(){return _basebandbuildmanifestPath;};
    bool is32bit(){return !is_image4_supported(_client);};
    
    
    uint64_t getBasebandGoldCertIDFromDevice();
    
    int doRestore(const char *ipsw);
    
    ~futurerestore();
    
    static const char *getRamdiskHashFromSCAB(const char* scab, size_t *hashSize);
    static char *getNonceFromSCAB(const char* scab, size_t *nonceSize);
    static uint64_t getEcidFromSCAB(const char* scab);
    static char *getNonceFromIM4M(const char* im4m, size_t *nonceSize);
    static uint64_t getEcidFromIM4M(const char* im4m);
    static char *getNonceFromAPTicket(const char* apticketPath);
    static plist_t loadPlistFromFile(const char *path);
    static void saveStringToFile(const char *str, const char *path);
    static char *getPathOfElementInManifest(const char *element, const char *manifeststr, const char *model, int isUpdateInstall);

};




#endif /* futurerestore_hpp */

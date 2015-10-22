#ifndef MOZILLA_DEFS_H
#define MOZILLA_DEFS_H

#define PRBool   int

typedef enum {
    SECWouldBlock = -2,
    SECFailure = -1,
    SECSuccess = 0
} SECStatus;

typedef enum {
    siBuffer,
    siClearDataBuffer,
    siCipherDataBuffer,
    siDERCertBuffer,
    siEncodedCertBuffer,
    siDERNameBuffer,
    siEncodedNameBuffer,
    siAsciiNameString,
    siAsciiString,
    siDEROID
} SECItemType;

struct SECItemStr {
    SECItemType type;
    unsigned char *data;
    unsigned int len;
};

typedef struct SECItemStr SECItem;

typedef struct PK11SlotInfoStr PK11SlotInfo;

typedef SECStatus      (*_NSS_Init) (const char *configdir);
typedef SECStatus      (*_NSS_Shutdown) (void);
typedef PK11SlotInfo*  (*_PK11_GetInternalKeySlot) (void);
typedef void           (*_PK11_FreeSlot) (PK11SlotInfo *slot);
typedef SECStatus      (*_PK11_CheckUserPassword) (PK11SlotInfo *slot,char *pw);
typedef SECStatus      (*_PK11_Authenticate) (PK11SlotInfo *slot, PRBool loadCerts, void *wincx);
typedef SECStatus      (*_PK11SDR_Decrypt) (SECItem *data, SECItem *result, void *cx);
typedef SECItem*       (*_NSSBase64_DecodeBuffer) (void *arenaOpt, SECItem *outItemOpt, const char *inStr, unsigned int inLen);
typedef SECItem*       (*_SECITEM_AllocItem)(void *arena, SECItem *item, unsigned int len);
typedef void           (*_SECITEM_FreeItem)(SECItem *zap, PRBool freeit);

extern _NSS_Init NSS_Init;
extern _NSS_Shutdown NSS_Shutdown;
extern _PK11_GetInternalKeySlot PK11_GetInternalKeySlot;
extern _PK11_FreeSlot PK11_FreeSlot;
extern _PK11_CheckUserPassword PK11_CheckUserPassword;
extern _PK11_Authenticate PK11_Authenticate;
extern _PK11SDR_Decrypt PK11SDR_Decrypt;
extern _NSSBase64_DecodeBuffer NSSBase64_DecodeBuffer;
extern _SECITEM_AllocItem SECITEM_AllocItem;
extern _SECITEM_FreeItem SECITEM_FreeItem;

#endif // MOZILLA_DEFS_H

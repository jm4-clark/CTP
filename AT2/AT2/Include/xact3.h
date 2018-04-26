/**************************************************************************
 *
 * Copyright (c) Microsoft Corporation. All rights reserved.
 *
 * Module Name:
 * 
 *     xact3.h
 * 
 * Abstract:
 * 
 *     XACT public interfaces, functions and data types
 *
 **************************************************************************/

#pragma once

#ifndef _XACT3_H_
#define _XACT3_H_

//------------------------------------------------------------------------------
// XACT class and interface IDs (Version 3.7)
//------------------------------------------------------------------------------
#ifndef _XBOX // XACT COM support only exists on Windows
    #include <comdecl.h> // For DEFINE_CLSID, DEFINE_IID and DECLARE_INTERFACE
    DEFINE_CLSID(XACTEngine,         bcc782bc, 6492, 4c22, 8c, 35, f5, d7, 2f, e7, 3c, 6e);
    DEFINE_CLSID(XACTAuditionEngine, 9ecdd80d, 0e81, 40d8, 89, 03, 2b, f7, b1, 31, ac, 43);
    DEFINE_CLSID(XACTDebugEngine,    02860630, bf3b, 42a8, b1, 4e, 91, ed, a2, f5, 1e, a5);
    DEFINE_IID(IXACT3Engine,         b1ee676a, d9cd, 4d2a, 89, a8, fa, 53, eb, 9e, 48, 0b);
#endif

// Ignore the rest of this header if only the GUID definitions were requested:
#ifndef GUID_DEFS_ONLY

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#ifndef _XBOX
    #include <windows.h>
    #include <objbase.h>
    #include <float.h>
#endif
#include <limits.h>
#include <xact3wb.h>
#include <xaudio2.h>

//------------------------------------------------------------------------------
// Forward Declarations
//------------------------------------------------------------------------------

typedef struct IXACT3SoundBank       IXACT3SoundBank;
typedef struct IXACT3WaveBank        IXACT3WaveBank;
typedef struct IXACT3Cue             IXACT3Cue;
typedef struct IXACT3Wave            IXACT3Wave;
typedef struct IXACT3Engine          IXACT3Engine;
typedef struct XACT_NOTIFICATION     XACT_NOTIFICATION;


//------------------------------------------------------------------------------
// Typedefs
//------------------------------------------------------------------------------

typedef WORD  XACTINDEX;            // All normal indices
typedef BYTE  XACTNOTIFICATIONTYPE; // Notification type
typedef FLOAT XACTVARIABLEVALUE;    // Variable value
typedef WORD  XACTVARIABLEINDEX;    // Variable index
typedef WORD  XACTCATEGORY;         // Sound category
typedef BYTE  XACTCHANNEL;          // Audio channel
typedef FLOAT XACTVOLUME;           // Volume value
typedef LONG  XACTTIME;             // Time (in ms)
typedef SHORT XACTPITCH;            // Pitch value
typedef BYTE  XACTLOOPCOUNT;        // For all loops / recurrences
typedef BYTE  XACTVARIATIONWEIGHT;  // Variation weight
typedef BYTE  XACTPRIORITY;         // Sound priority
typedef BYTE  XACTINSTANCELIMIT;    // Instance limitations

//------------------------------------------------------------------------------
// Standard win32 multimedia definitions
//------------------------------------------------------------------------------
#ifndef WAVE_FORMAT_IEEE_FLOAT
    #define  WAVE_FORMAT_IEEE_FLOAT 0x0003
#endif

#ifndef WAVE_FORMAT_EXTENSIBLE
    #define  WAVE_FORMAT_EXTENSIBLE 0xFFFE
#endif

#ifndef _WAVEFORMATEX_
#define _WAVEFORMATEX_
    #pragma pack(push, 1)
    typedef struct tWAVEFORMATEX
    {
        WORD    wFormatTag;      // format type
        WORD    nChannels;       // number of channels (i.e. mono, stereo...)
        DWORD   nSamplesPerSec;  // sample rate
        DWORD   nAvgBytesPerSec; // for buffer estimation
        WORD    nBlockAlign;     // block size of data
        WORD    wBitsPerSample;  // Number of bits per sample of mono data
        WORD    cbSize;          // The count in bytes of the size of extra information (after cbSize)

    } WAVEFORMATEX, *PWAVEFORMATEX;
    typedef WAVEFORMATEX NEAR *NPWAVEFORMATEX;
    typedef WAVEFORMATEX FAR  *LPWAVEFORMATEX;
    #pragma pack(pop)
#endif

#ifndef _WAVEFORMATEXTENSIBLE_
#define _WAVEFORMATEXTENSIBLE_
    #pragma pack(push, 1)
    typedef struct
    {
        WAVEFORMATEX    Format;              // WAVEFORMATEX data

        union
        {
            WORD        wValidBitsPerSample; // Bits of precision
            WORD        wSamplesPerBlock;    // Samples per block of audio data, valid if wBitsPerSample==0
            WORD        wReserved;           // Unused -- If neither applies, set to zero.
        } Samples;

        DWORD           dwChannelMask;       // Speaker usage bitmask
        GUID            SubFormat;           // Sub-format identifier
    } WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE;
    #pragma pack(pop)
#endif

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------
static const XACTTIME               XACTTIME_MIN                    = LONG_MIN;
static const XACTTIME               XACTTIME_MAX                    = LONG_MAX; // 24 days 20:31:23.647
static const XACTTIME               XACTTIME_INFINITE               = LONG_MAX;
static const XACTINSTANCELIMIT      XACTINSTANCELIMIT_INFINITE      = 0xff;
static const XACTINSTANCELIMIT      XACTINSTANCELIMIT_MIN           = 0x00; // == 1 instance total (0 additional instances)
static const XACTINSTANCELIMIT      XACTINSTANCELIMIT_MAX           = 0xfe; // == 255 instances total (254 additional instances)
static const XACTINDEX              XACTINDEX_MIN                   = 0x0;
static const XACTINDEX              XACTINDEX_MAX                   = 0xfffe;
static const XACTINDEX              XACTINDEX_INVALID               = 0xffff;
static const XACTNOTIFICATIONTYPE   XACTNOTIFICATIONTYPE_MIN        = 0x00;
static const XACTNOTIFICATIONTYPE   XACTNOTIFICATIONTYPE_MAX        = 0xff;
static const XACTVARIABLEVALUE      XACTVARIABLEVALUE_MIN           = -FLT_MAX;
static const XACTVARIABLEVALUE      XACTVARIABLEVALUE_MAX           = FLT_MAX;
static const XACTVARIABLEINDEX      XACTVARIABLEINDEX_MIN           = 0x0000;
static const XACTVARIABLEINDEX      XACTVARIABLEINDEX_MAX           = 0xfffe;
static const XACTVARIABLEINDEX      XACTVARIABLEINDEX_INVALID       = 0xffff;
static const XACTCATEGORY           XACTCATEGORY_MIN                = 0x0;
static const XACTCATEGORY           XACTCATEGORY_MAX                = 0xfffe;
static const XACTCATEGORY           XACTCATEGORY_INVALID            = 0xffff;
static const XACTCHANNEL            XACTCHANNEL_MIN                 = 0;
static const XACTCHANNEL            XACTCHANNEL_MAX                 = 0xFF;
static const XACTPITCH              XACTPITCH_MIN                   = -1200; // pitch change allowable per individual content field
static const XACTPITCH              XACTPITCH_MAX                   = 1200;
static const XACTPITCH              XACTPITCH_MIN_TOTAL             = -2400; // total allowable pitch change, use with IXACTWave.SetPitch()
static const XACTPITCH              XACTPITCH_MAX_TOTAL             = 2400;
static const XACTVOLUME             XACTVOLUME_MIN                  = 0.0f;
static const XACTVOLUME             XACTVOLUME_MAX                  = 16777216.0f;   // Maximum acceptable volume level (2^24) - matches XAudio2 max volume
static const XACTVARIABLEVALUE      XACTPARAMETERVALUE_MIN          = -FLT_MAX;
static const XACTVARIABLEVALUE      XACTPARAMETERVALUE_MAX          = FLT_MAX;
static const XACTLOOPCOUNT          XACTLOOPCOUNT_MIN               = 0x0;
static const XACTLOOPCOUNT          XACTLOOPCOUNT_MAX               = 0xfe;
static const XACTLOOPCOUNT          XACTLOOPCOUNT_INFINITE          = 0xff;
static const DWORD                  XACTWAVEALIGNMENT_MIN           = 2048;
#ifdef _XBOX
static const BYTE                   XACTMAXOUTPUTVOICECOUNT         = 3;
#endif // _XBOX


// -----------------------------------------------------------------------------
// Cue friendly name length
// -----------------------------------------------------------------------------
#define XACT_CUE_NAME_LENGTH        0xFF

// -----------------------------------------------------------------------------
// Current Content Tool Version
// -----------------------------------------------------------------------------
#define XACT_CONTENT_VERSION        46

// -----------------------------------------------------------------------------
// XACT Stop Flags
// -----------------------------------------------------------------------------
static const DWORD XACT_FLAG_STOP_RELEASE       = 0x00000000; // Stop with release envelope (or as authored), for looping waves this acts as break loop.
static const DWORD XACT_FLAG_STOP_IMMEDIATE     = 0x00000001; // Stop immediately

// -----------------------------------------------------------------------------
// XACT Manage Data Flag - XACT will manage the lifetime of this data
// -----------------------------------------------------------------------------
static const DWORD XACT_FLAG_MANAGEDATA         = 0x00000001;

// -----------------------------------------------------------------------------
// XACT Content Preparation Flags
// -----------------------------------------------------------------------------
static const DWORD XACT_FLAG_BACKGROUND_MUSIC   = 0x00000002; // Marks the waves as background music.
static const DWORD XACT_FLAG_UNITS_MS           = 0x00000004; // Indicates that the units passed in are in milliseconds.
static const DWORD XACT_FLAG_UNITS_SAMPLES      = 0x00000008; // Indicates that the units passed in are in samples.

// -----------------------------------------------------------------------------
// XACT State flags
// -----------------------------------------------------------------------------
static const DWORD XACT_STATE_CREATED           = 0x00000001; // Created, but nothing else
static const DWORD XACT_STATE_PREPARING         = 0x00000002; // In the middle of preparing
static const DWORD XACT_STATE_PREPARED          = 0x00000004; // Prepared, but not yet played
static const DWORD XACT_STATE_PLAYING           = 0x00000008; // Playing (though could be paused)
static const DWORD XACT_STATE_STOPPING          = 0x00000010; // Stopping
static const DWORD XACT_STATE_STOPPED           = 0x00000020; // Stopped
static const DWORD XACT_STATE_PAUSED            = 0x00000040; // Paused (Can be combined with some of the other state flags above)
static const DWORD XACT_STATE_INUSE             = 0x00000080; // Object is in use (used by wavebanks and soundbanks).
static const DWORD XACT_STATE_PREPAREFAILED     = 0x80000000; // Object preparation failed.

//------------------------------------------------------------------------------
// XACT Parameters
//------------------------------------------------------------------------------

#define XACT_FLAG_GLOBAL_SETTINGS_MANAGEDATA    XACT_FLAG_MANAGEDATA

// -----------------------------------------------------------------------------
// File IO Callbacks
// -----------------------------------------------------------------------------
typedef BOOL (__stdcall * XACT_READFILE_CALLBACK)(__in HANDLE hFile, __out_bcount(nNumberOfBytesToRead) LPVOID lpBuffer, DWORD nNumberOfBytesToRead, __out LPDWORD lpNumberOfBytesRead, __inout LPOVERLAPPED lpOverlapped);
typedef BOOL (__stdcall * XACT_GETOVERLAPPEDRESULT_CALLBACK)(__in HANDLE hFile, __inout LPOVERLAPPED lpOverlapped, __out LPDWORD lpNumberOfBytesTransferred, BOOL bWait);

typedef struct XACT_FILEIO_CALLBACKS
{
    XACT_READFILE_CALLBACK              readFileCallback;
    XACT_GETOVERLAPPEDRESULT_CALLBACK   getOverlappedResultCallback;

} XACT_FILEIO_CALLBACKS, *PXACT_FILEIO_CALLBACKS;
typedef const XACT_FILEIO_CALLBACKS *PCXACT_FILEIO_CALLBACKS;

// -----------------------------------------------------------------------------
// Notification Callback
// -----------------------------------------------------------------------------
typedef void (__stdcall * XACT_NOTIFICATION_CALLBACK)(__in const XACT_NOTIFICATION* pNotification);

#define XACT_RENDERER_ID_LENGTH                 0xff    // Maximum number of characters allowed in the renderer ID
#define XACT_RENDERER_NAME_LENGTH               0xff    // Maximum number of characters allowed in the renderer display name.

// -----------------------------------------------------------------------------
// Renderer Details
// -----------------------------------------------------------------------------
typedef struct XACT_RENDERER_DETAILS
{
    WCHAR rendererID[XACT_RENDERER_ID_LENGTH];          // The string ID for the rendering device.
    WCHAR displayName[XACT_RENDERER_NAME_LENGTH];       // A friendly name suitable for display to a human.
    BOOL  defaultDevice;                                // Set to TRUE if this device is the primary audio device on the system.

} XACT_RENDERER_DETAILS, *LPXACT_RENDERER_DETAILS;

// -----------------------------------------------------------------------------
// Engine Look-Ahead Time
// -----------------------------------------------------------------------------
#define XACT_ENGINE_LOOKAHEAD_DEFAULT           250     // Default look-ahead time of 250ms can be used during XACT engine initialization.

// -----------------------------------------------------------------------------
// Runtime (engine) parameters
// -----------------------------------------------------------------------------
typedef struct XACT_RUNTIME_PARAMETERS
{
    DWORD                           lookAheadTime;                  // Time in ms
    void*                           pGlobalSettingsBuffer;          // Buffer containing the global settings file
    DWORD                           globalSettingsBufferSize;       // Size of global settings buffer
    DWORD                           globalSettingsFlags;            // Flags for global settings
    DWORD                           globalSettingsAllocAttributes;  // Global settings buffer allocation attributes (see XMemAlloc)
    XACT_FILEIO_CALLBACKS           fileIOCallbacks;                // File I/O callbacks
    XACT_NOTIFICATION_CALLBACK      fnNotificationCallback;         // Callback that receives notifications.
    PWSTR                           pRendererID;                    // Ptr to the ID for the audio renderer the engine should connect to.
    IXAudio2*                       pXAudio2;                       // XAudio2 object to be used by the engine (NULL if one needs to be created)
    IXAudio2MasteringVoice*         pMasteringVoice;                // Mastering voice to be used by the engine, if pXAudio2 is not NULL.

} XACT_RUNTIME_PARAMETERS, *LPXACT_RUNTIME_PARAMETERS;
typedef const XACT_RUNTIME_PARAMETERS *LPCXACT_RUNTIME_PARAMETERS;

//------------------------------------------------------------------------------
// Streaming Parameters
//------------------------------------------------------------------------------

typedef struct XACT_STREAMING_PARAMETERS
{
    HANDLE  file;            // File handle associated with wavebank data
    DWORD   offset;          // Offset within file of wavebank header (must be sector aligned)
    DWORD   flags;           // Flags (none currently)
    WORD    packetSize;      // Stream packet size (in sectors) to use for each stream (min = 2)
                             //   number of sectors (DVD = 2048 bytes: 2 = 4096, 3 = 6144, 4 = 8192 etc.)
                             //   optimal DVD size is a multiple of 16 (DVD block = 16 DVD sectors)

} XACT_WAVEBANK_STREAMING_PARAMETERS, *LPXACT_WAVEBANK_STREAMING_PARAMETERS, XACT_STREAMING_PARAMETERS, *LPXACT_STREAMING_PARAMETERS;
typedef const XACT_STREAMING_PARAMETERS *LPCXACT_STREAMING_PARAMETERS;
typedef const XACT_WAVEBANK_STREAMING_PARAMETERS *LPCXACT_WAVEBANK_STREAMING_PARAMETERS;

// Structure used to report cue properties back to the client.
typedef struct XACT_CUE_PROPERTIES
{
    CHAR                friendlyName[XACT_CUE_NAME_LENGTH]; // Empty if the soundbank doesn't contain any friendly names
    BOOL                interactive;                        // TRUE if an IA cue; FALSE otherwise
    XACTINDEX           iaVariableIndex;                    // Only valid for IA cues; XACTINDEX_INVALID otherwise
    XACTINDEX           numVariations;                      // Number of variations in the cue
    XACTINSTANCELIMIT   maxInstances;                       // Number of maximum instances for this cue
    XACTINSTANCELIMIT   currentInstances;                   // Current active instances of this cue

} XACT_CUE_PROPERTIES, *LPXACT_CUE_PROPERTIES;

// Strucutre used to return the track properties.
typedef struct XACT_TRACK_PROPERTIES
{
    XACTTIME        duration;                   // Duration of the track in ms
    XACTINDEX       numVariations;              // Number of wave variations in the track
    XACTCHANNEL     numChannels;                // Number of channels for the active wave variation on this track
    XACTINDEX       waveVariation;              // Index of the active wave variation
    XACTLOOPCOUNT   loopCount;                  // Current loop count on this track

} XACT_TRACK_PROPERTIES, *LPXACT_TRACK_PROPERTIES;

// Structure used to return the properties of a variation.
typedef struct XACT_VARIATION_PROPERTIES
{
    XACTINDEX               index;              // Index of the variation in the cue's variation list
    XACTVARIATIONWEIGHT     weight;             // Weight for the active variation. Valid only for complex cues
    XACTVARIABLEVALUE       iaVariableMin;      // Valid only for IA cues
    XACTVARIABLEVALUE       iaVariableMax;      // Valid only for IA cues
    BOOL                    linger;             // Valid only for IA cues

} XACT_VARIATION_PROPERTIES, *LPXACT_VARIATION_PROPERTIES;

// Structure used to return the properties of the sound referenced by a variation.
typedef struct XACT_SOUND_PROPERTIES
{
    XACTCATEGORY            category;               // Category this sound belongs to
    BYTE                    priority;               // Priority of this variation
    XACTPITCH               pitch;                  // Current pitch set on the active variation
    XACTVOLUME              volume;                 // Current volume set on the active variation
    XACTINDEX               numTracks;              // Number of tracks in the active variation
    XACT_TRACK_PROPERTIES   arrTrackProperties[1];  // Array of active track properties (has numTracks number of elements)

} XACT_SOUND_PROPERTIES, *LPXACT_SOUND_PROPERTIES;

// Structure used to return the properties of the active variation and the sound referenced.
typedef struct XACT_SOUND_VARIATION_PROPERTIES
{
    XACT_VARIATION_PROPERTIES   variationProperties;// Properties for this variation
    XACT_SOUND_PROPERTIES       soundProperties;    // Proeprties for the sound referenced by this variation

} XACT_SOUND_VARIATION_PROPERTIES, *LPXACT_SOUND_VARIATION_PROPERTIES;

// Structure used to return the properties of an active cue instance.
typedef struct XACT_CUE_INSTANCE_PROPERTIES
{
    DWORD                            allocAttributes;            // Buffer allocation attributes (see XMemAlloc)
    XACT_CUE_PROPERTIES              cueProperties;              // Properties of the cue that are shared by all instances.
    XACT_SOUND_VARIATION_PROPERTIES  activeVariationProperties;  // Properties if the currently active variation.

} XACT_CUE_INSTANCE_PROPERTIES, *LPXACT_CUE_INSTANCE_PROPERTIES;

// Structure used to return the common wave properties.
typedef struct XACT_WAVE_PROPERTIES
{
    char                    friendlyName[WAVEBANK_ENTRYNAME_LENGTH];   // Friendly name for the wave; empty if the wavebank doesn't contain friendly names.
    WAVEBANKMINIWAVEFORMAT  format;                                    // Format for the wave.
    DWORD                   durationInSamples;                         // Duration of the wave in units of one sample
    WAVEBANKSAMPLEREGION    loopRegion;                                // Loop region defined in samples.
    BOOL                    streaming;                                 // Set to TRUE if the wave is streaming; FALSE otherwise.

} XACT_WAVE_PROPERTIES, *LPXACT_WAVE_PROPERTIES;
typedef const XACT_WAVE_PROPERTIES* LPCXACT_WAVE_PROPERTIES;

// Structure used to return the properties specific to a wave instance.
typedef struct XACT_WAVE_INSTANCE_PROPERTIES
{
    XACT_WAVE_PROPERTIES    properties;                                 // Static properties common to all the wave instances.
    BOOL                    backgroundMusic;                            // Set to TRUE if the wave is tagged as background music; FALSE otherwise.

} XACT_WAVE_INSTANCE_PROPERTIES, *LPXACT_WAVE_INSTANCE_PROPERTIES;
typedef const XACT_WAVE_INSTANCE_PROPERTIES* LPCXACT_WAVE_INSTANCE_PROPERTIES;

//------------------------------------------------------------------------------
// Channel Mapping / Speaker Panning
//------------------------------------------------------------------------------

typedef struct XACTCHANNELMAPENTRY
{
    XACTCHANNEL   InputChannel;
    XACTCHANNEL   OutputChannel;
    XACTVOLUME    Volume;

} XACTCHANNELMAPENTRY, *LPXACTCHANNELMAPENTRY;
typedef const XACTCHANNELMAPENTRY *LPCXACTCHANNELMAPENTRY;

typedef struct XACTCHANNELMAP
{
    XACTCHANNEL             EntryCount;
    XACTCHANNELMAPENTRY*    paEntries;

} XACTCHANNELMAP, *LPXACTCHANNELMAP;
typedef const XACTCHANNELMAP *LPCXACTCHANNELMAP;

typedef struct XACTCHANNELVOLUMEENTRY
{
    XACTCHANNEL   EntryIndex;
    XACTVOLUME    Volume;

} XACTCHANNELVOLUMEENTRY, *LPXACTCHANNELVOLUMEENTRY;
typedef const XACTCHANNELVOLUMEENTRY *LPCXACTCHANNELVOLUMEENTRY;

typedef struct XACTCHANNELVOLUME
{
    XACTCHANNEL             EntryCount;
    XACTCHANNELVOLUMEENTRY* paEntries;

} XACTCHANNELVOLUME, *LPXACTCHANNELVOLUME;
typedef const XACTCHANNELVOLUME *LPCXACTCHANNELVOLUME;

//------------------------------------------------------------------------------
// Notifications
//------------------------------------------------------------------------------

static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_CUEPREPARED                      = 1;  // None, SoundBank, SoundBank & cue index, cue instance
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_CUEPLAY                          = 2;  // None, SoundBank, SoundBank & cue index, cue instance
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_CUESTOP                          = 3;  // None, SoundBank, SoundBank & cue index, cue instance
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_CUEDESTROYED                     = 4;  // None, SoundBank, SoundBank & cue index, cue instance
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_MARKER                           = 5;  // None, SoundBank, SoundBank & cue index, cue instance
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_SOUNDBANKDESTROYED               = 6;  // None, SoundBank
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_WAVEBANKDESTROYED                = 7;  // None, WaveBank
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_LOCALVARIABLECHANGED             = 8;  // None, SoundBank, SoundBank & cue index, cue instance
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_GLOBALVARIABLECHANGED            = 9;  // None
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_GUICONNECTED                     = 10; // None
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_GUIDISCONNECTED                  = 11; // None
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_WAVEPREPARED                     = 12; // None, WaveBank & wave index, wave instance.
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_WAVEPLAY                         = 13; // None, SoundBank, SoundBank & cue index, cue instance, WaveBank, wave instance
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_WAVESTOP                         = 14; // None, SoundBank, SoundBank & cue index, cue instance, WaveBank, wave instance
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_WAVELOOPED                       = 15; // None, SoundBank, SoundBank & cue index, cue instance, WaveBank, wave instance
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_WAVEDESTROYED                    = 16; // None, WaveBank & wave index, wave instance.
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_WAVEBANKPREPARED                 = 17; // None, WaveBank
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_WAVEBANKSTREAMING_INVALIDCONTENT = 18; // None, WaveBank

static const BYTE XACT_FLAG_NOTIFICATION_PERSIST = 0x01;

// Pack the notification structures
#pragma pack(push, 1)

// Notification description used for registering, un-registering and flushing notifications
typedef struct XACT_NOTIFICATION_DESCRIPTION
{
    XACTNOTIFICATIONTYPE type;          // Notification type
    BYTE                 flags;         // Flags
    IXACT3SoundBank*      pSoundBank;    // SoundBank instance
    IXACT3WaveBank*       pWaveBank;     // WaveBank instance
    IXACT3Cue*            pCue;          // Cue instance
    IXACT3Wave*           pWave;         // Wave instance
    XACTINDEX            cueIndex;      // Cue index
    XACTINDEX            waveIndex;     // Wave index
    PVOID                pvContext;     // User context (optional)

} XACT_NOTIFICATION_DESCRIPTION, *LPXACT_NOTIFICATION_DESCRIPTION;
typedef const XACT_NOTIFICATION_DESCRIPTION *LPCXACT_NOTIFICATION_DESCRIPTION;

// Notification structure for all XACTNOTIFICATIONTYPE_CUE* notifications
typedef struct XACT_NOTIFICATION_CUE
{
    XACTINDEX       cueIndex;   // Cue index
    IXACT3SoundBank* pSoundBank; // SoundBank instance
    IXACT3Cue*       pCue;       // Cue instance

} XACT_NOTIFICATION_CUE, *LPXACT_NOTIFICATION_CUE;
typedef const XACT_NOTIFICATION_CUE *LPCXACT_NOTIFICATION_CUE;

// Notification structure for all XACTNOTIFICATIONTYPE_MARKER* notifications
typedef struct XACT_NOTIFICATION_MARKER
{
    XACTINDEX       cueIndex;   // Cue index
    IXACT3SoundBank* pSoundBank; // SoundBank instance
    IXACT3Cue*       pCue;       // Cue instance
    DWORD           marker;     // Marker value

} XACT_NOTIFICATION_MARKER, *LPXACT_NOTIFICATION_MARKER;
typedef const XACT_NOTIFICATION_MARKER *LPCXACT_NOTIFICATION_MARKER;

// Notification structure for all XACTNOTIFICATIONTYPE_SOUNDBANK* notifications
typedef struct XACT_NOTIFICATION_SOUNDBANK
{
    IXACT3SoundBank* pSoundBank; // SoundBank instance

} XACT_NOTIFICATION_SOUNDBANK, *LPXACT_NOTIFICATION_SOUNDBANK;
typedef const XACT_NOTIFICATION_SOUNDBANK *LPCXACT_NOTIFICATION_SOUNDBANK;

// Notification structure for all XACTNOTIFICATIONTYPE_WAVEBANK* notifications
typedef struct XACT_NOTIFICATION_WAVEBANK
{
    IXACT3WaveBank*  pWaveBank;  // WaveBank instance

} XACT_NOTIFICATION_WAVEBANK, *LPXACT_NOTIFICATION_WAVEBANK;
typedef const XACT_NOTIFICATION_WAVEBANK *LPCXACT_NOTIFICATION_WAVEBANK;

// Notification structure for all XACTNOTIFICATIONTYPE_*VARIABLE* notifications
typedef struct XACT_NOTIFICATION_VARIABLE
{
    XACTINDEX           cueIndex;       // Cue index
    IXACT3SoundBank*     pSoundBank;     // SoundBank instance
    IXACT3Cue*           pCue;           // Cue instance
    XACTVARIABLEINDEX   variableIndex;  // Variable index
    XACTVARIABLEVALUE   variableValue;  // Variable value
    BOOL                local;          // TRUE if a local variable

} XACT_NOTIFICATION_VARIABLE, *LPXACT_NOTIFICATION_VARIABLE;
typedef const XACT_NOTIFICATION_VARIABLE *LPCXACT_NOTIFICATION_VARIABLE;

// Notification structure for all XACTNOTIFICATIONTYPE_GUI* notifications
typedef struct XACT_NOTIFICATION_GUI
{
    DWORD   reserved; // Reserved
} XACT_NOTIFICATION_GUI, *LPXACT_NOTIFICATION_GUI;
typedef const XACT_NOTIFICATION_GUI *LPCXACT_NOTIFICATION_GUI;

// Notification structure for all XACTNOTIFICATIONTYPE_WAVE* notifications
typedef struct XACT_NOTIFICATION_WAVE
{
    IXACT3WaveBank*  pWaveBank;  // WaveBank
    XACTINDEX       waveIndex;  // Wave index
    XACTINDEX       cueIndex;   // Cue index
    IXACT3SoundBank* pSoundBank; // SoundBank instance
    IXACT3Cue*       pCue;       // Cue instance
    IXACT3Wave*      pWave;      // Wave instance

} XACT_NOTIFICATION_WAVE, *LPXACT_NOTIFICATION_WAVE;
typedef const XACT_NOTIFICATION_WAVE *LPCXACT_NOTIFICATION_WAVE;

// General notification structure
typedef struct XACT_NOTIFICATION
{
    XACTNOTIFICATIONTYPE    type;        // Notification type
    LONG                    timeStamp;   // Timestamp of notification (milliseconds)
    PVOID                   pvContext;   // User context (optional)
    union
    {
        XACT_NOTIFICATION_CUE       cue;        // XACTNOTIFICATIONTYPE_CUE*
        XACT_NOTIFICATION_MARKER    marker;     // XACTNOTIFICATIONTYPE_MARKER*
        XACT_NOTIFICATION_SOUNDBANK soundBank;  // XACTNOTIFICATIONTYPE_SOUNDBANK*
        XACT_NOTIFICATION_WAVEBANK  waveBank;   // XACTNOTIFICATIONTYPE_WAVEBANK*
        XACT_NOTIFICATION_VARIABLE  variable;   // XACTNOTIFICATIONTYPE_VARIABLE*
        XACT_NOTIFICATION_GUI       gui;        // XACTNOTIFICATIONTYPE_GUI*
        XACT_NOTIFICATION_WAVE      wave;       // XACTNOTIFICATIONTYPE_WAVE*
    };

} XACT_NOTIFICATION, *LPXACT_NOTIFICATION;
typedef const XACT_NOTIFICATION *LPCXACT_NOTIFICATION;

#pragma pack(pop)

//------------------------------------------------------------------------------
// IXACT3SoundBank
//------------------------------------------------------------------------------

#define XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE  XACT_FLAG_STOP_IMMEDIATE
#define XACT_SOUNDBANKSTATE_INUSE           XACT_STATE_INUSE

STDAPI_(XACTINDEX) IXACT3SoundBank_GetCueIndex(__in IXACT3SoundBank* pSoundBank, __in PCSTR szFriendlyName);
STDAPI IXACT3SoundBank_GetNumCues(__in IXACT3SoundBank* pSoundBank, __out XACTINDEX* pnNumCues);
STDAPI IXACT3SoundBank_GetCueProperties(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, __out LPXACT_CUE_PROPERTIES pProperties);
STDAPI IXACT3SoundBank_Prepare(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, DWORD dwFlags, XACTTIME timeOffset, __deref_out IXACT3Cue** ppCue);
STDAPI IXACT3SoundBank_Play(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, DWORD dwFlags, XACTTIME timeOffset, __deref_opt_out IXACT3Cue** ppCue);
STDAPI IXACT3SoundBank_Stop(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, DWORD dwFlags);
STDAPI IXACT3SoundBank_Destroy(__in IXACT3SoundBank* pSoundBank);
STDAPI IXACT3SoundBank_GetState(__in IXACT3SoundBank* pSoundBank, __out DWORD* pdwState);

#undef INTERFACE
#define INTERFACE IXACT3SoundBank

DECLARE_INTERFACE(IXACT3SoundBank)
{
    STDMETHOD_(XACTINDEX, GetCueIndex)(THIS_ __in PCSTR szFriendlyName) PURE;
    STDMETHOD(GetNumCues)(THIS_ __out XACTINDEX* pnNumCues) PURE;
    STDMETHOD(GetCueProperties)(THIS_ XACTINDEX nCueIndex, __out LPXACT_CUE_PROPERTIES pProperties) PURE;
    STDMETHOD(Prepare)(THIS_ XACTINDEX nCueIndex, DWORD dwFlags, XACTTIME timeOffset, __deref_out IXACT3Cue** ppCue) PURE;
    STDMETHOD(Play)(THIS_ XACTINDEX nCueIndex, DWORD dwFlags, XACTTIME timeOffset, __deref_opt_out IXACT3Cue** ppCue) PURE;
    STDMETHOD(Stop)(THIS_ XACTINDEX nCueIndex, DWORD dwFlags) PURE;
    STDMETHOD(Destroy)(THIS) PURE;
    STDMETHOD(GetState)(THIS_ __out DWORD* pdwState) PURE;
};

#ifdef __cplusplus

__inline HRESULT __stdcall IXACT3SoundBank_Destroy(__in IXACT3SoundBank* pSoundBank)
{
    return pSoundBank->Destroy();
}

__inline XACTINDEX __stdcall IXACT3SoundBank_GetCueIndex(__in IXACT3SoundBank* pSoundBank, __in PCSTR szFriendlyName)
{
    return pSoundBank->GetCueIndex(szFriendlyName);
}

__inline HRESULT __stdcall IXACT3SoundBank_GetNumCues(__in IXACT3SoundBank* pSoundBank, __out XACTINDEX* pnNumCues)
{
    return pSoundBank->GetNumCues(pnNumCues);
}

__inline HRESULT __stdcall IXACT3SoundBank_GetCueProperties(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, __out LPXACT_CUE_PROPERTIES pProperties)
{
    return pSoundBank->GetCueProperties(nCueIndex, pProperties);
}

__inline HRESULT __stdcall IXACT3SoundBank_Prepare(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, DWORD dwFlags, XACTTIME timeOffset, __deref_out IXACT3Cue** ppCue)
{
    return pSoundBank->Prepare(nCueIndex, dwFlags, timeOffset, ppCue);
}

__inline HRESULT __stdcall IXACT3SoundBank_Play(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, DWORD dwFlags, XACTTIME timeOffset, __deref_opt_out IXACT3Cue** ppCue)
{
    return pSoundBank->Play(nCueIndex, dwFlags, timeOffset, ppCue);
}

__inline HRESULT __stdcall IXACT3SoundBank_Stop(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, DWORD dwFlags)
{
    return pSoundBank->Stop(nCueIndex, dwFlags);
}

__inline HRESULT __stdcall IXACT3SoundBank_GetState(__in IXACT3SoundBank* pSoundBank, __out DWORD* pdwState)
{
    return pSoundBank->GetState(pdwState);
}

#else // __cplusplus

__inline HRESULT __stdcall IXACT3SoundBank_Destroy(__in IXACT3SoundBank* pSoundBank)
{
    return pSoundBank->lpVtbl->Destroy(pSoundBank);
}

__inline XACTINDEX __stdcall IXACT3SoundBank_GetCueIndex(__in IXACT3SoundBank* pSoundBank, __in PCSTR szFriendlyName)
{
    return pSoundBank->lpVtbl->GetCueIndex(pSoundBank, szFriendlyName);
}

__inline HRESULT __stdcall IXACT3SoundBank_GetNumCues(__in IXACT3SoundBank* pSoundBank, __out XACTINDEX* pnNumCues)
{
    return pSoundBank->lpVtbl->GetNumCues(pSoundBank, pnNumCues);
}

__inline HRESULT __stdcall IXACT3SoundBank_GetCueProperties(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, __out LPXACT_CUE_PROPERTIES pProperties)
{
    return pSoundBank->lpVtbl->GetCueProperties(pSoundBank, nCueIndex, pProperties);
}

__inline HRESULT __stdcall IXACT3SoundBank_Prepare(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, DWORD dwFlags, XACTTIME timeOffset, __deref_out IXACT3Cue** ppCue)
{
    return pSoundBank->lpVtbl->Prepare(pSoundBank, nCueIndex, dwFlags, timeOffset, ppCue);
}

__inline HRESULT __stdcall IXACT3SoundBank_Play(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, DWORD dwFlags, XACTTIME timeOffset, __deref_opt_out IXACT3Cue** ppCue)
{
    return pSoundBank->lpVtbl->Play(pSoundBank, nCueIndex, dwFlags, timeOffset, ppCue);
}

__inline HRESULT __stdcall IXACT3SoundBank_Stop(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, DWORD dwFlags)
{
    return pSoundBank->lpVtbl->Stop(pSoundBank, nCueIndex, dwFlags);
}

__inline HRESULT __stdcall IXACT3SoundBank_GetState(__in IXACT3SoundBank* pSoundBank, __out DWORD* pdwState)
{
    return pSoundBank->lpVtbl->GetState(pSoundBank, pdwState);
}

#endif // __cplusplus

//------------------------------------------------------------------------------
// IXACT3WaveBank
//------------------------------------------------------------------------------
#define XACT_WAVEBANKSTATE_INUSE            XACT_STATE_INUSE         // Currently in-use
#define XACT_WAVEBANKSTATE_PREPARED         XACT_STATE_PREPARED      // Prepared
#define XACT_WAVEBANKSTATE_PREPAREFAILED    XACT_STATE_PREPAREFAILED // Prepare failed.


STDAPI IXACT3WaveBank_Destroy(__in IXACT3WaveBank* pWaveBank);
STDAPI IXACT3WaveBank_GetState(__in IXACT3WaveBank* pWaveBank, __out DWORD* pdwState);
STDAPI IXACT3WaveBank_GetNumWaves(__in IXACT3WaveBank* pWaveBank, __out XACTINDEX* pnNumWaves);
STDAPI_(XACTINDEX) IXACT3WaveBank_GetWaveIndex(__in IXACT3WaveBank* pWaveBank, __in PCSTR szFriendlyName);
STDAPI IXACT3WaveBank_GetWaveProperties(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, __out LPXACT_WAVE_PROPERTIES pWaveProperties);
STDAPI IXACT3WaveBank_Prepare(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, DWORD dwFlags, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave);
STDAPI IXACT3WaveBank_Play(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, DWORD dwFlags, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave);
STDAPI IXACT3WaveBank_Stop(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, DWORD dwFlags);

#undef INTERFACE
#define INTERFACE IXACT3WaveBank

DECLARE_INTERFACE(IXACT3WaveBank)
{
    STDMETHOD(Destroy)(THIS) PURE;
    STDMETHOD(GetNumWaves)(THIS_ __out XACTINDEX* pnNumWaves) PURE;
    STDMETHOD_(XACTINDEX, GetWaveIndex)(THIS_ __in PCSTR szFriendlyName) PURE;
    STDMETHOD(GetWaveProperties)(THIS_ XACTINDEX nWaveIndex, __out LPXACT_WAVE_PROPERTIES pWaveProperties) PURE;
    STDMETHOD(Prepare)(THIS_ XACTINDEX nWaveIndex, DWORD dwFlags, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave) PURE;
    STDMETHOD(Play)(THIS_ XACTINDEX nWaveIndex, DWORD dwFlags, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave) PURE;
    STDMETHOD(Stop)(THIS_ XACTINDEX nWaveIndex, DWORD dwFlags) PURE;
    STDMETHOD(GetState)(THIS_ __out DWORD* pdwState) PURE;
};

#ifdef __cplusplus

__inline HRESULT __stdcall IXACT3WaveBank_Destroy(__in IXACT3WaveBank* pWaveBank)
{
    return pWaveBank->Destroy();
}

__inline HRESULT __stdcall IXACT3WaveBank_GetNumWaves(__in IXACT3WaveBank* pWaveBank, __out XACTINDEX* pnNumWaves)
{
    return pWaveBank->GetNumWaves(pnNumWaves);
}

__inline XACTINDEX __stdcall IXACT3WaveBank_GetWaveIndex(__in IXACT3WaveBank* pWaveBank, __in PCSTR szFriendlyName)
{
    return pWaveBank->GetWaveIndex(szFriendlyName);
}

__inline HRESULT __stdcall IXACT3WaveBank_GetWaveProperties(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, __out LPXACT_WAVE_PROPERTIES pWaveProperties)
{
    return pWaveBank->GetWaveProperties(nWaveIndex, pWaveProperties);
}

__inline HRESULT __stdcall IXACT3WaveBank_Prepare(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, DWORD dwFlags, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave)
{
    return pWaveBank->Prepare(nWaveIndex, dwFlags, dwPlayOffset, nLoopCount, ppWave);
}

__inline HRESULT __stdcall IXACT3WaveBank_Play(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, DWORD dwFlags, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave)
{
    return pWaveBank->Play(nWaveIndex, dwFlags, dwPlayOffset, nLoopCount, ppWave);
}

__inline HRESULT __stdcall IXACT3WaveBank_Stop(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, DWORD dwFlags)
{
    return pWaveBank->Stop(nWaveIndex, dwFlags);
}

__inline HRESULT __stdcall IXACT3WaveBank_GetState(__in IXACT3WaveBank* pWaveBank, __out DWORD* pdwState)
{
    return pWaveBank->GetState(pdwState);
}

#else // __cplusplus

__inline HRESULT __stdcall IXACT3WaveBank_Destroy(__in IXACT3WaveBank* pWaveBank)
{
    return pWaveBank->lpVtbl->Destroy(pWaveBank);
}

__inline HRESULT __stdcall IXACT3WaveBank_GetNumWaves(__in IXACT3WaveBank* pWaveBank, __out XACTINDEX* pnNumWaves)
{
    return pWaveBank->lpVtbl->GetNumWaves(pWaveBank, pnNumWaves);
}

__inline XACTINDEX __stdcall IXACT3WaveBank_GetWaveIndex(__in IXACT3WaveBank* pWaveBank, __in PCSTR szFriendlyName)
{
    return pWaveBank->lpVtbl->GetWaveIndex(pWaveBank, szFriendlyName);
}

__inline HRESULT __stdcall IXACT3WaveBank_GetWaveProperties(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, __out LPXACT_WAVE_PROPERTIES pWaveProperties)
{
    return pWaveBank->lpVtbl->GetWaveProperties(pWaveBank, nWaveIndex, pWaveProperties);
}

__inline HRESULT __stdcall IXACT3WaveBank_Prepare(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, DWORD dwFlags, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave)
{
    return pWaveBank->lpVtbl->Prepare(pWaveBank, nWaveIndex, dwFlags, dwPlayOffset, nLoopCount, ppWave);
}

__inline HRESULT __stdcall IXACT3WaveBank_Play(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, DWORD dwFlags, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave)
{
    return pWaveBank->lpVtbl->Play(pWaveBank, nWaveIndex, dwFlags, dwPlayOffset, nLoopCount, ppWave);
}

__inline HRESULT __stdcall IXACT3WaveBank_Stop(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, DWORD dwFlags)
{
    return pWaveBank->lpVtbl->Stop(pWaveBank, nWaveIndex, dwFlags);
}

__inline HRESULT __stdcall IXACT3WaveBank_GetState(__in IXACT3WaveBank* pWaveBank, __out DWORD* pdwState)
{
    return pWaveBank->lpVtbl->GetState(pWaveBank, pdwState);
}
#endif // __cplusplus


//------------------------------------------------------------------------------
// IXACT3Wave
//------------------------------------------------------------------------------

STDAPI IXACT3Wave_Destroy(__in IXACT3Wave* pWave);
STDAPI IXACT3Wave_Play(__in IXACT3Wave* pWave);
STDAPI IXACT3Wave_Stop(__in IXACT3Wave* pWave, DWORD dwFlags);
STDAPI IXACT3Wave_Pause(__in IXACT3Wave* pWave, BOOL fPause);
STDAPI IXACT3Wave_GetState(__in IXACT3Wave* pWave, __out DWORD* pdwState);
STDAPI IXACT3Wave_SetPitch(__in IXACT3Wave* pWave, XACTPITCH pitch);
STDAPI IXACT3Wave_SetVolume(__in IXACT3Wave* pWave, XACTVOLUME volume);
STDAPI IXACT3Wave_SetMatrixCoefficients(__in IXACT3Wave* pWave, UINT32 uSrcChannelCount, UINT32 uDstChannelCount, __in float* pMatrixCoefficients);
STDAPI IXACT3Wave_GetProperties(__in IXACT3Wave* pWave, __out LPXACT_WAVE_INSTANCE_PROPERTIES pProperties);

#undef INTERFACE
#define INTERFACE IXACT3Wave

DECLARE_INTERFACE(IXACT3Wave)
{
    STDMETHOD(Destroy)(THIS) PURE;
    STDMETHOD(Play)(THIS) PURE;
    STDMETHOD(Stop)(THIS_ DWORD dwFlags) PURE;
    STDMETHOD(Pause)(THIS_ BOOL fPause) PURE;
    STDMETHOD(GetState)(THIS_ __out DWORD* pdwState) PURE;
    STDMETHOD(SetPitch)(THIS_ XACTPITCH pitch) PURE;
    STDMETHOD(SetVolume)(THIS_ XACTVOLUME volume) PURE;
    STDMETHOD(SetMatrixCoefficients)(THIS_ UINT32 uSrcChannelCount, UINT32 uDstChannelCount, __in float* pMatrixCoefficients) PURE;
    STDMETHOD(GetProperties)(THIS_ __out LPXACT_WAVE_INSTANCE_PROPERTIES pProperties) PURE;
};

#ifdef __cplusplus

__inline HRESULT __stdcall IXACT3Wave_Destroy(__in IXACT3Wave* pWave)
{
    return pWave->Destroy();
}

__inline HRESULT __stdcall IXACT3Wave_Play(__in IXACT3Wave* pWave)
{
    return pWave->Play();
}

__inline HRESULT __stdcall IXACT3Wave_Stop(__in IXACT3Wave* pWave, DWORD dwFlags)
{
    return pWave->Stop(dwFlags);
}

__inline HRESULT __stdcall IXACT3Wave_Pause(__in IXACT3Wave* pWave, BOOL fPause)
{
    return pWave->Pause(fPause);
}

__inline HRESULT __stdcall IXACT3Wave_GetState(__in IXACT3Wave* pWave, __out DWORD* pdwState)
{
    return pWave->GetState(pdwState);
}

__inline HRESULT __stdcall IXACT3Wave_SetPitch(__in IXACT3Wave* pWave, XACTPITCH pitch)
{
    return pWave->SetPitch(pitch);
}

__inline HRESULT __stdcall IXACT3Wave_SetVolume(__in IXACT3Wave* pWave, XACTVOLUME volume)
{
    return pWave->SetVolume(volume);
}

__inline HRESULT __stdcall IXACT3Wave_SetMatrixCoefficients(__in IXACT3Wave* pWave, UINT32 uSrcChannelCount, UINT32 uDstChannelCount, __in float* pMatrixCoefficients)
{
    return pWave->SetMatrixCoefficients(uSrcChannelCount, uDstChannelCount, pMatrixCoefficients);
}

__inline HRESULT __stdcall IXACT3Wave_GetProperties(__in IXACT3Wave* pWave, __out LPXACT_WAVE_INSTANCE_PROPERTIES pProperties)
{
    return pWave->GetProperties(pProperties);
}

#else // __cplusplus

__inline HRESULT __stdcall IXACT3Wave_Destroy(__in IXACT3Wave* pWave)
{
    return pWave->lpVtbl->Destroy(pWave);
}

__inline HRESULT __stdcall IXACT3Wave_Play(__in IXACT3Wave* pWave)
{
    return pWave->lpVtbl->Play(pWave);
}

__inline HRESULT __stdcall IXACT3Wave_Stop(__in IXACT3Wave* pWave, DWORD dwFlags)
{
    return pWave->lpVtbl->Stop(pWave, dwFlags);
}

__inline HRESULT __stdcall IXACT3Wave_Pause(__in IXACT3Wave* pWave, BOOL fPause)
{
    return pWave->lpVtbl->Pause(pWave, fPause);
}

__inline HRESULT __stdcall IXACT3Wave_GetState(__in IXACT3Wave* pWave, __out DWORD* pdwState)
{
    return pWave->lpVtbl->GetState(pWave, pdwState);
}

__inline HRESULT __stdcall IXACT3Wave_SetPitch(__in IXACT3Wave* pWave, XACTPITCH pitch)
{
    return pWave->lpVtbl->SetPitch(pWave, pitch);
}

__inline HRESULT __stdcall IXACT3Wave_SetVolume(__in IXACT3Wave* pWave, XACTVOLUME volume)
{
    return pWave->lpVtbl->SetVolume(pWave, volume);
}

__inline HRESULT __stdcall IXACT3Wave_SetMatrixCoefficients(__in IXACT3Wave* pWave, UINT32 uSrcChannelCount, UINT32 uDstChannelCount, __in float* pMatrixCoefficients)
{
    return pWave->lpVtbl->SetMatrixCoefficients(pWave, uSrcChannelCount, uDstChannelCount, pMatrixCoefficients);
}

__inline HRESULT __stdcall IXACT3Wave_GetProperties(__in IXACT3Wave* pWave, __out LPXACT_WAVE_INSTANCE_PROPERTIES pProperties)
{
    return pWave->lpVtbl->GetProperties(pWave, pProperties);
}
#endif // __cplusplus

//------------------------------------------------------------------------------
// IXACT3Cue
//------------------------------------------------------------------------------

// Cue Flags
#define XACT_FLAG_CUE_STOP_RELEASE      XACT_FLAG_STOP_RELEASE
#define XACT_FLAG_CUE_STOP_IMMEDIATE    XACT_FLAG_STOP_IMMEDIATE

// Mutually exclusive states
#define XACT_CUESTATE_CREATED           XACT_STATE_CREATED   // Created, but nothing else
#define XACT_CUESTATE_PREPARING         XACT_STATE_PREPARING // In the middle of preparing
#define XACT_CUESTATE_PREPARED          XACT_STATE_PREPARED  // Prepared, but not yet played
#define XACT_CUESTATE_PLAYING           XACT_STATE_PLAYING   // Playing (though could be paused)
#define XACT_CUESTATE_STOPPING          XACT_STATE_STOPPING  // Stopping
#define XACT_CUESTATE_STOPPED           XACT_STATE_STOPPED   // Stopped
#define XACT_CUESTATE_PAUSED            XACT_STATE_PAUSED    // Paused (can be combined with other states)

STDAPI IXACT3Cue_Destroy(__in IXACT3Cue* pCue);
STDAPI IXACT3Cue_Play(__in IXACT3Cue* pCue);
STDAPI IXACT3Cue_Stop(__in IXACT3Cue* pCue, DWORD dwFlags);
STDAPI IXACT3Cue_GetState(__in IXACT3Cue* pCue, __out DWORD* pdwState);
STDAPI IXACT3Cue_SetMatrixCoefficients(__in IXACT3Cue*, UINT32 uSrcChannelCount, UINT32 uDstChannelCount, __in float* pMatrixCoefficients);
STDAPI_(XACTVARIABLEINDEX) IXACT3Cue_GetVariableIndex(__in IXACT3Cue* pCue, __in PCSTR szFriendlyName);
STDAPI IXACT3Cue_SetVariable(__in IXACT3Cue* pCue, XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE nValue);
STDAPI IXACT3Cue_GetVariable(__in IXACT3Cue* pCue, XACTVARIABLEINDEX nIndex, __out XACTVARIABLEVALUE* nValue);
STDAPI IXACT3Cue_Pause(__in IXACT3Cue* pCue, BOOL fPause);
STDAPI IXACT3Cue_GetProperties(__in IXACT3Cue* pCue, __out LPXACT_CUE_INSTANCE_PROPERTIES* ppProperties);
STDAPI IXACT3Cue_SetOutputVoices(__in IXACT3Cue* pCue, __in_opt const XAUDIO2_VOICE_SENDS* pSendList);
STDAPI IXACT3Cue_SetOutputVoiceMatrix(__in IXACT3Cue* pCue, __in_opt IXAudio2Voice* pDestinationVoice, UINT32 SourceChannels, UINT32 DestinationChannels, __in_ecount(SourceChannels * DestinationChannels) const float* pLevelMatrix);

#undef INTERFACE
#define INTERFACE IXACT3Cue

DECLARE_INTERFACE(IXACT3Cue)
{
    STDMETHOD(Play)(THIS) PURE;
    STDMETHOD(Stop)(THIS_ DWORD dwFlags) PURE;
    STDMETHOD(GetState)(THIS_ __out DWORD* pdwState) PURE;
    STDMETHOD(Destroy)(THIS) PURE;
    STDMETHOD(SetMatrixCoefficients)(THIS_ UINT32 uSrcChannelCount, UINT32 uDstChannelCount, __in float* pMatrixCoefficients) PURE;
    STDMETHOD_(XACTVARIABLEINDEX, GetVariableIndex)(THIS_ __in PCSTR szFriendlyName) PURE;
    STDMETHOD(SetVariable)(THIS_ XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE nValue) PURE;
    STDMETHOD(GetVariable)(THIS_ XACTVARIABLEINDEX nIndex, __out XACTVARIABLEVALUE* nValue) PURE;
    STDMETHOD(Pause)(THIS_ BOOL fPause) PURE;
    STDMETHOD(GetProperties)(THIS_ __out LPXACT_CUE_INSTANCE_PROPERTIES* ppProperties) PURE;
    STDMETHOD(SetOutputVoices)(THIS_ __in_opt const XAUDIO2_VOICE_SENDS* pSendList) PURE;
    STDMETHOD(SetOutputVoiceMatrix)(THIS_ __in_opt IXAudio2Voice* pDestinationVoice, UINT32 SourceChannels, UINT32 DestinationChannels, __in_ecount(SourceChannels * DestinationChannels) const float* pLevelMatrix) PURE;
};

#ifdef __cplusplus

__inline HRESULT __stdcall IXACT3Cue_Play(__in IXACT3Cue* pCue)
{
    return pCue->Play();
}

__inline HRESULT __stdcall IXACT3Cue_Stop(__in IXACT3Cue* pCue, DWORD dwFlags)
{
    return pCue->Stop(dwFlags);
}

__inline HRESULT __stdcall IXACT3Cue_GetState(__in IXACT3Cue* pCue, __out DWORD* pdwState)
{
    return pCue->GetState(pdwState);
}

__inline HRESULT __stdcall IXACT3Cue_Destroy(__in IXACT3Cue* pCue)
{
    return pCue->Destroy();
}

__inline HRESULT __stdcall IXACT3Cue_SetMatrixCoefficients(__in IXACT3Cue* pCue, UINT32 uSrcChannelCount, UINT32 uDstChannelCount, __in float* pMatrixCoefficients)
{
    return pCue->SetMatrixCoefficients(uSrcChannelCount, uDstChannelCount, pMatrixCoefficients);
}

__inline XACTVARIABLEINDEX __stdcall IXACT3Cue_GetVariableIndex(__in IXACT3Cue* pCue, __in PCSTR szFriendlyName)
{
    return pCue->GetVariableIndex(szFriendlyName);
}

__inline HRESULT __stdcall IXACT3Cue_SetVariable(__in IXACT3Cue* pCue, XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE nValue)
{
    return pCue->SetVariable(nIndex, nValue);
}

__inline HRESULT __stdcall IXACT3Cue_GetVariable(__in IXACT3Cue* pCue, XACTVARIABLEINDEX nIndex, __out XACTVARIABLEVALUE* pnValue)
{
    return pCue->GetVariable(nIndex, pnValue);
}

__inline HRESULT __stdcall IXACT3Cue_Pause(__in IXACT3Cue* pCue, BOOL fPause)
{
    return pCue->Pause(fPause);
}

__inline HRESULT __stdcall IXACT3Cue_GetProperties(__in IXACT3Cue* pCue, __out LPXACT_CUE_INSTANCE_PROPERTIES* ppProperties)
{
    return pCue->GetProperties(ppProperties);
}

__inline HRESULT __stdcall IXACT3Cue_SetOutputVoices(__in IXACT3Cue* pCue, __in_opt const XAUDIO2_VOICE_SENDS* pSendList)
{
    return pCue->SetOutputVoices(pSendList);
}

__inline HRESULT __stdcall IXACT3Cue_SetOutputVoiceMatrix(__in IXACT3Cue* pCue, __in_opt IXAudio2Voice* pDestinationVoice, UINT32 SourceChannels, UINT32 DestinationChannels, __in_ecount(SourceChannels * DestinationChannels) const float* pLevelMatrix)
{
    return pCue->SetOutputVoiceMatrix(pDestinationVoice, SourceChannels, DestinationChannels, pLevelMatrix);
}

#else // __cplusplus

__inline HRESULT __stdcall IXACT3Cue_Play(__in IXACT3Cue* pCue)
{
    return pCue->lpVtbl->Play(pCue);
}

__inline HRESULT __stdcall IXACT3Cue_Stop(__in IXACT3Cue* pCue, DWORD dwFlags)
{
    return pCue->lpVtbl->Stop(pCue, dwFlags);
}

__inline HRESULT __stdcall IXACT3Cue_GetState(__in IXACT3Cue* pCue, __out DWORD* pdwState)
{
    return pCue->lpVtbl->GetState(pCue, pdwState);
}

__inline HRESULT __stdcall IXACT3Cue_Destroy(__in IXACT3Cue* pCue)
{
    return pCue->lpVtbl->Destroy(pCue);
}

__inline HRESULT __stdcall IXACT3Cue_SetMatrixCoefficients(__in IXACT3Cue* pCue, UINT32 uSrcChannelCount, UINT32 uDstChannelCount, __in float* pMatrixCoefficients)
{
    return pCue->lpVtbl->SetMatrixCoefficients(pCue, uSrcChannelCount, uDstChannelCount, pMatrixCoefficients);
}

__inline XACTVARIABLEINDEX __stdcall IXACT3Cue_GetVariableIndex(__in IXACT3Cue* pCue, __in PCSTR szFriendlyName)
{
    return pCue->lpVtbl->GetVariableIndex(pCue, szFriendlyName);
}

__inline HRESULT __stdcall IXACT3Cue_SetVariable(__in IXACT3Cue* pCue, XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE nValue)
{
    return pCue->lpVtbl->SetVariable(pCue, nIndex, nValue);
}

__inline HRESULT __stdcall IXACT3Cue_GetVariable(__in IXACT3Cue* pCue, XACTVARIABLEINDEX nIndex, __out XACTVARIABLEVALUE* pnValue)
{
    return pCue->lpVtbl->GetVariable(pCue, nIndex, pnValue);
}

__inline HRESULT __stdcall IXACT3Cue_Pause(__in IXACT3Cue* pCue, BOOL fPause)
{
    return pCue->lpVtbl->Pause(pCue, fPause);
}

__inline HRESULT __stdcall IXACT3Cue_GetProperties(__in IXACT3Cue* pCue, __out LPXACT_CUE_INSTANCE_PROPERTIES* ppProperties)
{
    return pCue->lpVtbl->GetProperties(pCue, ppProperties);
}

__inline HRESULT __stdcall IXACT3Cue_SetOutputVoices(__in IXACT3Cue* pCue, __in_opt const XAUDIO2_VOICE_SENDS* pSendList)
{
    return pCue->lpVtbl->SetOutputVoices(pSendList);
}

__inline HRESULT __stdcall IXACT3Cue_SetOutputVoiceMatrix(__in IXACT3Cue* pCue, __in_opt IXAudio2Voice* pDestinationVoice, UINT32 SourceChannels, UINT32 DestinationChannels, __in_ecount(SourceChannels * DestinationChannels) const float* pLevelMatrix)
{
    return pCue->lpVtbl->SetOutputVoiceMatrix(pDestinationVoice, SourceChannels, DestinationChannels, pLevelMatrix);
}

#endif // __cplusplus

//------------------------------------------------------------------------------
// IXACT3Engine
//------------------------------------------------------------------------------

// Engine flags
#define XACT_FLAG_ENGINE_CREATE_MANAGEDATA    XACT_FLAG_MANAGEDATA
#define XACT_FLAG_ENGINE_STOP_IMMEDIATE       XACT_FLAG_STOP_IMMEDIATE

STDAPI_(ULONG) IXACT3Engine_AddRef(__in IXACT3Engine* pEngine);
STDAPI_(ULONG) IXACT3Engine_Release(__in IXACT3Engine* pEngine);
STDAPI IXACT3Engine_GetRendererCount(__in IXACT3Engine* pEngine, __out XACTINDEX* pnRendererCount);
STDAPI IXACT3Engine_GetRendererDetails(__in IXACT3Engine* pEngine, XACTINDEX nRendererIndex, __out LPXACT_RENDERER_DETAILS pRendererDetails);
STDAPI IXACT3Engine_GetFinalMixFormat(__in IXACT3Engine* pEngine, __out WAVEFORMATEXTENSIBLE* pFinalMixFormat);
STDAPI IXACT3Engine_Initialize(__in IXACT3Engine* pEngine, __in const XACT_RUNTIME_PARAMETERS* pParams);
STDAPI IXACT3Engine_ShutDown(__in IXACT3Engine* pEngine);
STDAPI IXACT3Engine_DoWork(__in IXACT3Engine* pEngine);
STDAPI IXACT3Engine_CreateSoundBank(__in IXACT3Engine* pEngine, __in const void* pvBuffer, DWORD dwSize, DWORD dwFlags, DWORD dwAllocAttributes, __deref_out IXACT3SoundBank** ppSoundBank);
STDAPI IXACT3Engine_CreateInMemoryWaveBank(__in IXACT3Engine* pEngine, __in const void* pvBuffer, DWORD dwSize, DWORD dwFlags, DWORD dwAllocAttributes, __deref_out IXACT3WaveBank** ppWaveBank);
STDAPI IXACT3Engine_CreateStreamingWaveBank(__in IXACT3Engine* pEngine, __in const XACT_WAVEBANK_STREAMING_PARAMETERS* pParms, __deref_out IXACT3WaveBank** ppWaveBank);
STDAPI IXACT3Engine_PrepareWave(__in IXACT3Engine* pEngine, DWORD dwFlags, __in PCSTR szWavePath, WORD wStreamingPacketSize, DWORD dwAlignment, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave);
STDAPI IXACT3Engine_PrepareInMemoryWave(__in IXACT3Engine* pEngine, DWORD dwFlags, WAVEBANKENTRY entry, __in_opt DWORD* pdwSeekTable, __in_opt BYTE* pbWaveData, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave);
STDAPI IXACT3Engine_PrepareStreamingWave(__in IXACT3Engine* pEngine, DWORD dwFlags, WAVEBANKENTRY entry, XACT_STREAMING_PARAMETERS streamingParams, DWORD dwAlignment, __in_opt DWORD* pdwSeekTable, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave);
STDAPI IXACT3Engine_RegisterNotification(__in IXACT3Engine* pEngine, __in const XACT_NOTIFICATION_DESCRIPTION* pNotificationDesc);
STDAPI IXACT3Engine_UnRegisterNotification(__in IXACT3Engine* pEngine, __in const XACT_NOTIFICATION_DESCRIPTION* pNotificationDesc);
STDAPI_(XACTCATEGORY) IXACT3Engine_GetCategory(__in IXACT3Engine* pEngine, __in PCSTR szFriendlyName);
STDAPI IXACT3Engine_Stop(__in IXACT3Engine* pEngine, XACTCATEGORY nCategory, DWORD dwFlags);
STDAPI IXACT3Engine_SetVolume(__in IXACT3Engine* pEngine, XACTCATEGORY nCategory, XACTVOLUME nVolume);
STDAPI IXACT3Engine_Pause(__in IXACT3Engine* pEngine, XACTCATEGORY nCategory, BOOL fPause);
STDAPI_(XACTVARIABLEINDEX) IXACT3Engine_GetGlobalVariableIndex(__in IXACT3Engine* pEngine, __in PCSTR szFriendlyName);
STDAPI IXACT3Engine_SetGlobalVariable(__in IXACT3Engine* pEngine, XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE nValue);
STDAPI IXACT3Engine_GetGlobalVariable(__in IXACT3Engine* pEngine, XACTVARIABLEINDEX nIndex, __out XACTVARIABLEVALUE* pnValue);

#undef INTERFACE
#define INTERFACE IXACT3Engine

#ifdef _XBOX
DECLARE_INTERFACE(IXACT3Engine)
{
#else
DECLARE_INTERFACE_(IXACT3Engine, IUnknown)
{
    STDMETHOD(QueryInterface)(THIS_ __in REFIID riid, __deref_out void** ppvObj) PURE;
#endif

    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    STDMETHOD(GetRendererCount)(THIS_ __out XACTINDEX* pnRendererCount) PURE;
    STDMETHOD(GetRendererDetails)(THIS_ XACTINDEX nRendererIndex, __out LPXACT_RENDERER_DETAILS pRendererDetails) PURE;

    STDMETHOD(GetFinalMixFormat)(THIS_ __out WAVEFORMATEXTENSIBLE* pFinalMixFormat) PURE;
    STDMETHOD(Initialize)(THIS_ __in const XACT_RUNTIME_PARAMETERS* pParams) PURE;
    STDMETHOD(ShutDown)(THIS) PURE;

    STDMETHOD(DoWork)(THIS) PURE;

    STDMETHOD(CreateSoundBank)(THIS_ __in const void* pvBuffer, DWORD dwSize, DWORD dwFlags, DWORD dwAllocAttributes, __deref_out IXACT3SoundBank** ppSoundBank) PURE;
    STDMETHOD(CreateInMemoryWaveBank)(THIS_ __in const void* pvBuffer, DWORD dwSize, DWORD dwFlags, DWORD dwAllocAttributes, __deref_out IXACT3WaveBank** ppWaveBank) PURE;
    STDMETHOD(CreateStreamingWaveBank)(THIS_ __in const XACT_WAVEBANK_STREAMING_PARAMETERS* pParms, __deref_out IXACT3WaveBank** ppWaveBank) PURE;

    STDMETHOD(PrepareWave)(THIS_ DWORD dwFlags, __in PCSTR szWavePath, WORD wStreamingPacketSize, DWORD dwAlignment, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave) PURE;
    STDMETHOD(PrepareInMemoryWave)(THIS_ DWORD dwFlags, WAVEBANKENTRY entry, __in_opt DWORD* pdwSeekTable, __in_opt BYTE* pbWaveData, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave) PURE;
    STDMETHOD(PrepareStreamingWave)(THIS_ DWORD dwFlags, WAVEBANKENTRY entry, XACT_STREAMING_PARAMETERS streamingParams, DWORD dwAlignment, __in_opt DWORD* pdwSeekTable, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave) PURE;

    STDMETHOD(RegisterNotification)(THIS_ __in const XACT_NOTIFICATION_DESCRIPTION* pNotificationDesc) PURE;
    STDMETHOD(UnRegisterNotification)(THIS_ __in const XACT_NOTIFICATION_DESCRIPTION* pNotificationDesc) PURE;

    STDMETHOD_(XACTCATEGORY, GetCategory)(THIS_ __in PCSTR szFriendlyName) PURE;
    STDMETHOD(Stop)(THIS_ XACTCATEGORY nCategory, DWORD dwFlags) PURE;
    STDMETHOD(SetVolume)(THIS_ XACTCATEGORY nCategory, XACTVOLUME nVolume) PURE;
    STDMETHOD(Pause)(THIS_ XACTCATEGORY nCategory, BOOL fPause) PURE;

    STDMETHOD_(XACTVARIABLEINDEX, GetGlobalVariableIndex)(THIS_ __in PCSTR szFriendlyName) PURE;
    STDMETHOD(SetGlobalVariable)(THIS_ XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE nValue) PURE;
    STDMETHOD(GetGlobalVariable)(THIS_ XACTVARIABLEINDEX nIndex, __out XACTVARIABLEVALUE* nValue) PURE;
};

#ifdef __cplusplus

__inline ULONG __stdcall IXACT3Engine_AddRef(__in IXACT3Engine* pEngine)
{
    return pEngine->AddRef();
}

__inline ULONG __stdcall IXACT3Engine_Release(__in IXACT3Engine* pEngine)
{
    return pEngine->Release();
}

__inline HRESULT __stdcall IXACT3Engine_GetRendererCount(__in IXACT3Engine* pEngine, __out XACTINDEX* pnRendererCount)
{
    return pEngine->GetRendererCount(pnRendererCount);
}

__inline HRESULT __stdcall IXACT3Engine_GetRendererDetails(__in IXACT3Engine* pEngine, XACTINDEX nRendererIndex, __out LPXACT_RENDERER_DETAILS pRendererDetails)
{
    return pEngine->GetRendererDetails(nRendererIndex, pRendererDetails);
}

__inline HRESULT __stdcall IXACT3Engine_GetFinalMixFormat(__in IXACT3Engine* pEngine, __out WAVEFORMATEXTENSIBLE* pFinalMixFormat)
{
    return pEngine->GetFinalMixFormat(pFinalMixFormat);
}

__inline HRESULT __stdcall IXACT3Engine_Initialize(__in IXACT3Engine* pEngine, __in const XACT_RUNTIME_PARAMETERS* pParams)
{
    return pEngine->Initialize(pParams);
}

__inline HRESULT __stdcall IXACT3Engine_ShutDown(__in IXACT3Engine* pEngine)
{
    return pEngine->ShutDown();
}

__inline HRESULT __stdcall IXACT3Engine_DoWork(__in IXACT3Engine* pEngine)
{
    return pEngine->DoWork();
}

__inline HRESULT __stdcall IXACT3Engine_CreateSoundBank(__in IXACT3Engine* pEngine, __in const void* pvBuffer, DWORD dwSize, DWORD dwFlags, DWORD dwAllocAttributes, __deref_out IXACT3SoundBank** ppSoundBank)
{
    return pEngine->CreateSoundBank(pvBuffer, dwSize, dwFlags, dwAllocAttributes, ppSoundBank);
}

__inline HRESULT __stdcall IXACT3Engine_CreateInMemoryWaveBank(__in IXACT3Engine* pEngine, __in const void* pvBuffer, DWORD dwSize, DWORD dwFlags, DWORD dwAllocAttributes, __deref_out IXACT3WaveBank** ppWaveBank)
{
    return pEngine->CreateInMemoryWaveBank(pvBuffer, dwSize, dwFlags, dwAllocAttributes, ppWaveBank);
}

__inline HRESULT __stdcall IXACT3Engine_CreateStreamingWaveBank(__in IXACT3Engine* pEngine, __in const XACT_WAVEBANK_STREAMING_PARAMETERS* pParms, __deref_out IXACT3WaveBank** ppWaveBank)
{
    return pEngine->CreateStreamingWaveBank(pParms, ppWaveBank);
}

__inline HRESULT __stdcall IXACT3Engine_PrepareWave(__in IXACT3Engine* pEngine, DWORD dwFlags, __in PCSTR szWavePath, WORD wStreamingPacketSize, DWORD dwAlignment, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave)
{
    return pEngine->PrepareWave(dwFlags, szWavePath, wStreamingPacketSize, dwAlignment, dwPlayOffset, nLoopCount, ppWave);
}

__inline HRESULT __stdcall IXACT3Engine_PrepareInMemoryWave(__in IXACT3Engine* pEngine, DWORD dwFlags, WAVEBANKENTRY entry, __in_opt DWORD* pdwSeekTable, __in_opt BYTE* pbWaveData, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave)
{
    return pEngine->PrepareInMemoryWave(dwFlags, entry, pdwSeekTable, pbWaveData, dwPlayOffset, nLoopCount, ppWave);
}

__inline HRESULT __stdcall IXACT3Engine_PrepareStreamingWave(__in IXACT3Engine* pEngine, DWORD dwFlags, WAVEBANKENTRY entry, XACT_STREAMING_PARAMETERS streamingParams, DWORD dwAlignment, __in_opt DWORD* pdwSeekTable, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave)
{
    return pEngine->PrepareStreamingWave(dwFlags, entry, streamingParams, dwAlignment, pdwSeekTable, dwPlayOffset, nLoopCount, ppWave);
}

__inline HRESULT __stdcall IXACT3Engine_RegisterNotification(__in IXACT3Engine* pEngine, __in const XACT_NOTIFICATION_DESCRIPTION* pNotificationDesc)
{
    return pEngine->RegisterNotification(pNotificationDesc);
}

__inline HRESULT __stdcall IXACT3Engine_UnRegisterNotification(__in IXACT3Engine* pEngine, __in const XACT_NOTIFICATION_DESCRIPTION* pNotificationDesc)
{
    return pEngine->UnRegisterNotification(pNotificationDesc);
}

__inline XACTCATEGORY __stdcall IXACT3Engine_GetCategory(__in IXACT3Engine* pEngine, __in PCSTR szFriendlyName)
{
    return pEngine->GetCategory(szFriendlyName);
}

__inline HRESULT __stdcall IXACT3Engine_Stop(__in IXACT3Engine* pEngine, XACTCATEGORY nCategory, DWORD dwFlags)
{
    return pEngine->Stop(nCategory, dwFlags);
}

__inline HRESULT __stdcall IXACT3Engine_SetVolume(__in IXACT3Engine* pEngine, XACTCATEGORY nCategory, XACTVOLUME nVolume)
{
    return pEngine->SetVolume(nCategory, nVolume);
}

__inline HRESULT __stdcall IXACT3Engine_Pause(__in IXACT3Engine* pEngine, XACTCATEGORY nCategory, BOOL fPause)
{
    return pEngine->Pause(nCategory, fPause);
}

__inline XACTVARIABLEINDEX __stdcall IXACT3Engine_GetGlobalVariableIndex(__in IXACT3Engine* pEngine, __in PCSTR szFriendlyName)
{
    return pEngine->GetGlobalVariableIndex(szFriendlyName);
}

__inline HRESULT __stdcall IXACT3Engine_SetGlobalVariable(__in IXACT3Engine* pEngine, XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE nValue)
{
    return pEngine->SetGlobalVariable(nIndex, nValue);
}

__inline HRESULT __stdcall IXACT3Engine_GetGlobalVariable(__in IXACT3Engine* pEngine, XACTVARIABLEINDEX nIndex, __out XACTVARIABLEVALUE* nValue)
{
    return pEngine->GetGlobalVariable(nIndex, nValue);
}

#else // __cplusplus

__inline ULONG __stdcall IXACT3Engine_AddRef(__in IXACT3Engine* pEngine)
{
    return pEngine->lpVtbl->AddRef(pEngine);
}

__inline ULONG __stdcall IXACT3Engine_Release(__in IXACT3Engine* pEngine)
{
    return pEngine->lpVtbl->Release(pEngine);
}

__inline HRESULT __stdcall IXACT3Engine_GetRendererCount(__in IXACT3Engine* pEngine, __out XACTINDEX* pnRendererCount)
{
    return pEngine->lpVtbl->GetRendererCount(pEngine, pnRendererCount);
}

__inline HRESULT __stdcall IXACT3Engine_GetRendererDetails(__in IXACT3Engine* pEngine, XACTINDEX nRendererIndex, __out LPXACT_RENDERER_DETAILS pRendererDetails)
{
    return pEngine->lpVtbl->GetRendererDetails(pEngine, nRendererIndex, pRendererDetails);
}

__inline HRESULT __stdcall IXACT3Engine_GetFinalMixFormat(__in IXACT3Engine* pEngine, __out WAVEFORMATEXTENSIBLE* pFinalMixFormat)
{
    return pEngine->lpVtbl->GetFinalMixFormat(pEngine, pFinalMixFormat);
}

__inline HRESULT __stdcall IXACT3Engine_Initialize(__in IXACT3Engine* pEngine, __in const XACT_RUNTIME_PARAMETERS* pParams)
{
    return pEngine->lpVtbl->Initialize(pEngine, pParams);
}

__inline HRESULT __stdcall IXACT3Engine_ShutDown(__in IXACT3Engine* pEngine)
{
    return pEngine->lpVtbl->ShutDown(pEngine);
}

__inline HRESULT __stdcall IXACT3Engine_DoWork(__in IXACT3Engine* pEngine)
{
    return pEngine->lpVtbl->DoWork(pEngine);
}

__inline HRESULT __stdcall IXACT3Engine_CreateSoundBank(__in IXACT3Engine* pEngine, __in const void* pvBuffer, DWORD dwSize, DWORD dwFlags, DWORD dwAllocAttributes, __deref_out IXACT3SoundBank** ppSoundBank)
{
    return pEngine->lpVtbl->CreateSoundBank(pEngine, pvBuffer, dwSize, dwFlags, dwAllocAttributes, ppSoundBank);
}

__inline HRESULT __stdcall IXACT3Engine_CreateInMemoryWaveBank(__in IXACT3Engine* pEngine, __in const void* pvBuffer, DWORD dwSize, DWORD dwFlags, DWORD dwAllocAttributes, __deref_out IXACT3WaveBank** ppWaveBank)
{
    return pEngine->lpVtbl->CreateInMemoryWaveBank(pEngine, pvBuffer, dwSize, dwFlags, dwAllocAttributes, ppWaveBank);
}

__inline HRESULT __stdcall IXACT3Engine_CreateStreamingWaveBank(__in IXACT3Engine* pEngine, __in const XACT_WAVEBANK_STREAMING_PARAMETERS* pParms, __deref_out IXACT3WaveBank** ppWaveBank)
{
    return pEngine->lpVtbl->CreateStreamingWaveBank(pEngine, pParms, ppWaveBank);
}

__inline HRESULT __stdcall IXACT3Engine_PrepareWave(__in IXACT3Engine* pEngine, DWORD dwFlags, __in PCSTR szWavePath, WORD wStreamingPacketSize, DWORD dwAlignment, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave)
{
    return pEngine->lpVtbl->PrepareWave(pEngine, dwFlags, szWavePath, wStreamingPacketSize, dwAlignment, dwPlayOffset, nLoopCount, ppWave);
}

__inline HRESULT __stdcall IXACT3Engine_PrepareInMemoryWave(__in IXACT3Engine* pEngine, DWORD dwFlags, WAVEBANKENTRY entry, __in_opt DWORD* pdwSeekTable, __in_opt BYTE* pbWaveData, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave)
{
    return pEngine->lpVtbl->PrepareInMemoryWave(pEngine, dwFlags, entry, pdwSeekTable, pbWaveData, dwPlayOffset, nLoopCount, ppWave);47��?x*���2��d�j��� ��!5v�c�W���$DZ�7�
%b���oU֦�A��-QV�['ٺH)x�Ƭ��*?@*!NsS ��(	vG�# Azz�5ӱ է��di/���	�j��h�K�b�J�j��M Hu%�9�*������_�C��}���%�=ǽ�<N[;�EU�Q0��<�J��cdO��iAq( j�$�̍�TO���X�q٪wܫ�SZ��jE.;g����p�5tkQ|�$�؉��C�*2�3^#�"�S��Z�@$�<��*=�Z=R��;|�3#Я��=|-$.~Z�i~�}���嘾�`�+�~:����`[��U���PFF3��ʙ�Z�w�T�Bv���}��n����A[�B@<�*I���	^���r�-�`�2�l�PbC$$n���R0��P��:�oh��w���U1FH�_�/n�ȿ���IIs�����+G7	m�Y�L~��{/-t�'�%,͎��|ࢗ)��u���X�UI{X��)']���z�V��~P��k�?x|sr��@nE!��=п
P�c0ʵ�dΑ�_�X�"�!6�Hd� ����o���C��P�1��&�P������7�����땺E��D8���IYɆ48xݺ��P����ATf�<#��p��:+)7S6�6��e����f�����7G?���vu�/c�t�5kO� ⶑ�B�H�$�G���$��p����ä���)����C,���BZp.�G��C�\]��Hm'׾OgI� ,�1l�w�5
-TN��	p�g���^e�>��jg$،����{s���ǧ�6�S�d��V�z}�?c�z�C!�k�7��������7RޙT5F����%�z+�C����)<�ȼ�����Qfg(]N���d	&w͕��޵Z;2�J�x����@�j>(
�� �U+�ֲ��C�,��;�1kz������c�a�j�Z�h��G�VS�`�V8
wg-:�t1́mo�"�ῃ��S��<Engine, __in PCSTR szFriendlyName)
{
    return pEngine->lpVtbl->GetCategory(pEngine, szFriendlyName);
}

__inline HRESULT __stdcall IXACT3Engine_Stop(__in IXACT3Engine* pEngine, XACTCATEGORY nCategory, DWORD dwFlags)
{
    return pEngine->lpVtbl->Stop(pEngine, nCategory, dwFlags);
}

__inline HRESULT __stdcall IXACT3Engine_SetVolume(__in IXACT3Engine* pEngine, XACTCATEGORY nCategory, XACTVOLUME nVolume)
{
    return pEngine->lpVtbl->SetVolume(pEngine, nCategory, nVolume);
}

__inline HRESULT __stdcall IXACT3Engine_Pause(__in IXACT3Engine* pEngine, XACTCATEGORY nCategory, BOOL fPause)
{
    return pEngine->lpVtbl->Pause(pEngine, nCategory, fPause);
}

__inline XACTVARIABLEINDEX __stdcall IXACT3Engine_GetGlobalVariableIndex(__in IXACT3Engine* pEngine, __in PCSTR szFriendlyName)
{
    return pEngine->lpVtbl->GetGlobalVariableIndex(pEngine, szFriendlyName);
}

__inline HRESULT __stdcall IXACT3Engine_SetGlobalVariable(__in IXACT3Engine* pEngine, XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE��3;-A{���7��/�\��	$`n�ӷ����{𷃛`���FSpɡ�G�͞L�k\В�'y�1bVDD'[N꾖0b�媯E�� TIp|����7/�� ��<v���.��k���R�ټȁ;=HBJiU�nu���cq:D��,�fԲ�*��]E���]J(wj�F��. 7b�9�Y��]ƨ\���&Q-r���D��u1�E�en�M�>j�<c�	f��&3,n�BD�'�61���gҨs�(�6��,�f�?0b���OK�9`��U�7p���[(�;�O*�:;ǻ�
�������6V����A׆j�8�r�����u�J�1*Fh�+���)�+�qtu(�:�`��:|�][�i�a%@O'`̾��K��]x��5�T�Q�D2�����S�:����/g��X:_#�ݢY��O��	�&���v���D`k��hd���O/������0"����etbv4���ż�vc�ǇSÆ�{y�� X��
�`�xEz���,k�du������C��(o(�A	<��7Ť5J�~���g�zГ�,��>�L�D��1�(Qo�f�|����Ȧ7�����))N��%'�^����K�J����R��u�J�ǀ�P��0������Z����B����\XH��������.���kbsY�Ců8:~+1��&3Mv��T�4|kD��)K3�mj��9! �ݷ�e^Yט��l�lbP���l��<�J�ٙ*M�*c`T�q��xV�_T�wnV�X��S�
�ď����z<��$�d\W٭>��RvB�0�o�ϼޫ�!���:�SGW#� �ݣKZ&F�~�4�qg�K����e�cC:��ؘ��tMb�������)����Cd�dc�pO@�+�̀�c�cӟ��n��i���C�g�`f-��Nqw0���  c�w2�����`$�h��Wt��0��] w�K\�8��A�yx
\�����	{�~��|�!���g�&&��ine XACT_DEBUGENGINE_REGISTRY_KEY   TEXT("Software\\Microsoft\\XACT")
#define XACT_DEBUGENGINE_REGISTRY_VALUE TEXT("DebugEngine")


#ifdef __cplusplus

__inline HRESULT __stdcall XACT3CreateEngine(DWORD dwCreationFlags, __deref_out IXACT3Engine** ppEngine)
{
    HRESULT hr;
    HKEY    key;
    DWORD   data;
    DWORD   type     = REG_DWORD;
    DWORD   dataSize = sizeof(DWORD);
    BOOL    debug    = (dwCreationFlags & XACT_FLAG_API_DEBUG_MODE) ? TRUE : FALSE;
    BOOL    audition = (dwCreationFlags & XACT_FLAG_API_AUDITION_MODE) ? TRUE : FALSE;

    // If neither the debug nor audition flags are set, see if the debug registry key is set
    if(!debug && !audition &&
       (RegOpenKeyEx(HKEY_LOCAL_MACHINE, XACT_DEBUGENGINE_REGISTRY_KEY, 0, KEY_READ, &key) == ERROR_SUCCESS))
    {
        if(RegQueryValueEx(key, XACT_DEBUGENGINE_REGISTRY_VALUE, NULL, &type, (LPBYTE)&data, &dataSize) == ERROR_SUCCESS)
        {
            if(data)
            {
                debug = TRUE;
            }
        }
        RegCloseKey(key);
    }

    // Priority order: Audition, Debug, Retail
    hr = CoCreateInstance(audition ? __uuidof(XACTAuditionEngine)
                          : (debug ? __uuidof(XACTDebugEngine) : __uuidof(XACTEngine)),
                          NULL, CLSCTX_INPROC_SERVER, __uuidof(IXACT3Engine), (void**)ppEngine);

    // If debug engine does not exist fallback to retail version
    if(FAILED(hr) && debug && !audition)
    {
        hr = CoCreateInstance(__uuidof(XACTEngine), NULL, CLSCTX_INPROC_SERVER, __uuidof(IXACT3Engine), (void**)ppEngine);
    }

    return hr;
}

#else // #ifdef __cplusplus

__inline HRESULT __stdcall XACT3CreateEngine(DWORD dwCreationFlags, __deref_out IXACT3Engine** ppEngine)
{
    HRESULT hr;
    HKEY    key;
    DWORD   data;
    DWORD   type     = REG_DWORD;
    DWORD   dataSize = sizeof(DWORD);
    BOOL    debug    = (dwCreationFlags & XACT_FLAG_API_DEBUG_MODE) ? TRUE : FALSE;
    BOOL    audition = (dwCreationFlags & XACn"<H�/�t�����<�h�Hc+ň]��Os���t#��iy���u�Zc<��.�������E[�$��x�XaCaF���w��n��K�j��P�,�rh9��`#Vt �S���3 R����[�I��
y;.W`?����gc��O"���A��O���6�bW��_��#&�wT�6�����);��p������r@>�����y:gȴ���Y(&�����9_o��S"�@fW����ߛ�ވFa��2o�:�J~���d��\����b 14o�fO/=A����H�c�,���e7i�LŃ��F}Kt�ij��8�N����"��%�moƶbg�0]�Qْ��s8>j}NI�>��pJ�N�s	a줆���|_���#'G?��?s�"d�N��EX� �����ZV��n;��l�6�$�����(Z�l���kB���ȭpe��QU�S.�"��wBBMM�dV��d#�ʙ)�޻�E��ĸ�����l���#9ڸI�lqW���\S��P�}���M�z`ş<���[5��ƨ+�U������֕i��q�%�u�L�|A�/�U����g���V=���'�S�j?v���2���X��u�N�b�����&+�Z�V�
.�8���aB�ø��h��:~Ac��v`[����â2���~P�1�=��P_,X��Ww^w��I�c1?;[��<"���'�Y�}��2�I��Ĭ�h�x����k/+{�D��������%��1�}c=����*��`�ї��⦇"���!C��~S#3�,���[?�A�NhZ�fcZ��dL�Ok�}Rq�
�
���_`���q��_,��������4�\5�(Y�E�3�9�� $C���~�Ub��Ѿ��1;H���!���maK���X"N��@L��	���������S���v<	�L�i��.�E�y�q�����gは�(�	<Ƿ7�OW�#J�6/��"cf����1��H����A��ї\�\� �rHHW���C_SERVER, &IID_IXACT3Engine, (void**)ppEngine);
    }

    return hr;
}

#endif // #ifdef __cplusplus

#endif // #ifdef _XBOX

//------------------------------------------------------------------------------
// XACT specific error codes
//------------------------------------------------------------------------------

#define FACILITY_XACTENGINE 0xAC7
#define XACTENGINEERROR(n) MAKE_HRESULT(SEVERITY_ERROR, FACILITY_XACTENGINE, n)

#define XACTENGINE_E_OUTOFMEMORY               E_OUTOFMEMORY      // Out of memory
#define XACTENGINE_E_INVALIDARG                E_INVALIDARG       // Invalid arg
#define XACTENGINE_E_NOTIMPL                   E_NOTIMPL          // Not implemented
#define XACTENGINE_E_FAIL                      E_FAIL             // Unknown error

#define XACTENGINE_E_ALREADYINITIALIZED        XACTENGINEERROR(0x001)   // The engine is already initialized
#define XACTENGINE_E_NOTINITIALIZED            XACTENGINEERROR(0x002)   // The engine has not been initialized
#define XACTENGINE_E_EXPIRED                   XACTENGINEERROR(0x003)   // The engine has expired (demo or pre-release version)
#define XACTENGINE_E_NONOTIFICATIONCALLBACK    XACTENGINEERROR(0x004)   // No notification callback
#define XACTENGINE_E_NOTIFICATIONREGISTERED    XACTENGINEERROR(0x005)   // Notification already registered
#define XACTENGINE_E_INVALIDUSAGE              XACTENGINEERROR(0x006)   // Invalid usage
#define XACTENGINE_E_INVALIDDATA               XACTENGINEERROR(0x007)   // Invalid data
#define XACTENGINE_E_INSTANCELIMITFAILTOPLAY   XACTENGINEERROR(0x008)   // Fail to play due to instance limit
#define XACTENGINE_E_NOGLOBALSETTINGS          XACTENGINEERROR(0x009)   // Global Settings not loaded
#define XACTENGINE_E_INVALIDVARIABLEINDEX      XACTENGINEERROR(0x00a)   // Invalid variable index
#define XACTENGINE_E_INVALIDCATEGORY           XACTENGINEERROR(0x00b)   // Invalid category
#define XACTENGINE_E_INVALIDCUEINDEX           XACTENGINEERROR(0x00c)   // Invalid cue index
#define XACTENGINE_E_INVALIDWAVGINDEX          XACTENGINEERROR(0�00d)   /o I.valid wav� index#de�ine XATENGINE_E_INVALIDTRACKINDEX         XACTENGINEERROR(0x00e)   // Invalid track index
#define XACTENGInE_E_INVALIDSNUNDOFFSETORINDEX XACTENGIEERROR(0x00f)   // Invalid swund offse4 or index
#defhne XACTENGIE_E_R�ADFILE              0   X�CTENGINEERROR(8x010)   // Error rea�ing a bile
#define XACTENGINE_E_UNKNOWNEVENT              XABtENGInEERROR(0h011-   // Unknown evmnt type
#define XACTENGINE_E_IN�ALLBACK       !        XACTENGINEERROR(0x012)   //$InvalId call og methof of function from callback#define XACTENGINE_E_NOWAVEBANK                XACTENG	NEERROR(0x013)   // No wavmbank exists for desive� operation
#def�ne XACTENGINE_E_SLECTVARIAT�ON      !    XACTENGINEeRROR(8x014!   // wnable �o seldct a variatioN
#define XACTENGINE_U_MUNTIPLEAUDITIONENGINES   XACTENGI�EERROR(0y015)   // There cin fe only one audition engine�#define!XACTENgINE_E_WAVEBANKNOTPrEPARED       XACTENFINEERROR(0x016)   // The wavebank is not prepared
#define XACTENGINE_E_NORENDERER                XACTENGINEERROR(0x017)   // No audio device found on.
#define XACTENGINE_E_INVALIDENTRYCOUNT         XACTENGINEERROR(0x018)   // Invalid entry count for channel maps
#define XACTENGINE_E_SEEKTIMEBEYONDCUEEND      XACTENGINEERROR(0x019)   // Time offset for seeking is beyond the cue end.
#define XACTENGINE_E_SEEKTIMEBEYONDWAVEEND     XACTENGINEERROR(0x01a)   // Time offset for seeking is beyond the wave end.
#define XACTENGINE_E_NOFRIENDLYNAMES           XACTENGINEERROR(0x01b)   // Friendly names are not included in the bank.

#define XACTENGINE_E_AUDITION_WRITEFILE             XACTENGINEERROR(0x101)  // Error writing a file during auditioning
#define XACTENGINE_E_AUDITION_NOSOUNDBANK           XACTENGINEERROR(0x102)  // Missing a soundbank
#define XACTENGINE_E_AUDITION_INVALIDRPCINDEX       XACTENGINEERROR(0x103)  // Missing an RPC curve
#define XACTENGINE_E_AUDITION_MISSINGDATA           XACTENGINEERROR(0x104)  // Missing data for an audition command
#define XACTENGINE_E_QUDITION_UNKNOW�COMMAND        XACTENGINEERROR(0x105)  // Unonown command
#define XACTE�GINE_E_AUD	TION_INVALIDDSPINDEX   "   XACTENGINEERROR(0x106) !// Missing i DSP paraoeter
#defin� YACTENGINE_E_AUDITIONMISSINGWAVE           XACTENGINEERROR(0x107)  // V!ve does not exisu in auditioned wavebank
#de&ine XACTENGINE_E_AUDITION_CREATEFIRECTORYFAILED XACTENGANEERROR(0x108(  // FaiLed t� create a directory for streaming wiveja.k data
#define XACTENGINE_E_AUDITION_HNVALIESESSI�N        XACTENGINEERROR(0x108)  // Invalid`audition session

#endif // #ifndef GUID_DEFS_ONLY
�
#endif /' �ifndef _XACT3_H_
